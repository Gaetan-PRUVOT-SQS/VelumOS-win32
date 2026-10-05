#!/usr/bin/env python3
"""Image disque démarrable de VelumOS : GPT, Limine en BIOS et en UEFI, sans droits root.

mkimage.py --noyau kernel.elf --sortie disk.img [--initrd DOSSIER] [--limine DOSSIER]
           [--resolution 1024x768] [--cmdline TEXTE] [--esp-mio 64]

Partition 1 : ESP FAT (Limine, limine.conf, noyau, initrd au format cpio newc).
Partition 2 : démarrage BIOS (1 Mio, second étage de Limine pour GPT).
Outils externes : sgdisk (gdisk), mtools (mformat, mmd, mcopy) et l'outil `limine`
que `make limine` prépare dans build/limine.
"""

import argparse
import os
import shutil
import stat
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

SECTEUR = 512
MIO = 1 << 20
RACINE = Path(__file__).resolve().parent.parent
FICHIERS_EFI = ("BOOTX64.EFI", "BOOTIA32.EFI", "BOOTAA64.EFI", "BOOTRISCV64.EFI")
NOM_ENTREE = "VelumOS"


class Erreur(Exception):
    pass


@dataclass(frozen=True)
class Plan:
    taille: int
    bios_debut: int
    bios_taille: int
    esp_debut: int
    esp_taille: int


def planifier(esp_mio):
    if esp_mio < 8:
        raise Erreur("ESP de 8 Mio au moins")
    bios_debut = MIO
    esp_debut = 2 * MIO
    esp_taille = esp_mio * MIO
    taille = esp_debut + esp_taille + MIO
    return Plan(taille, bios_debut, MIO, esp_debut, esp_taille)


def lancer(commande):
    try:
        resultat = subprocess.run(commande, capture_output=True, check=False)
    except FileNotFoundError as erreur:
        raise Erreur(f"outil absent : {commande[0]}") from erreur
    if resultat.returncode != 0:
        detail = resultat.stderr.decode(errors="replace").strip()[-400:]
        raise Erreur(f"{Path(commande[0]).name} a échoué : {detail}")


def entree_cpio(ino, mode, taille, nom):
    nom_nul = nom.encode() + b"\0"
    nlink = 2 if stat.S_ISDIR(mode) else 1
    mtime = int(os.environ.get("SOURCE_DATE_EPOCH", "0"))
    champs = (ino, mode, 0, 0, nlink, mtime, taille, 0, 0, 0, 0, len(nom_nul), 0)
    brut = b"070701" + "".join(f"{c:08X}" for c in champs).encode() + nom_nul
    return brut + b"\0" * (-len(brut) % 4)


def cpio_newc(dossier):
    sortie = bytearray(entree_cpio(1, stat.S_IFDIR | 0o755, 0, "."))
    chemins = sorted(dossier.rglob("*"), key=lambda p: p.relative_to(dossier).as_posix())
    for numero, chemin in enumerate(chemins, start=2):
        nom = chemin.relative_to(dossier).as_posix()
        if chemin.is_symlink():
            raise Erreur(f"lien symbolique refusé dans l'initrd : {nom}")
        if chemin.is_dir():
            sortie += entree_cpio(numero, stat.S_IFDIR | 0o755, 0, nom)
            continue
        donnees = chemin.read_bytes()
        droits = 0o755 if os.access(chemin, os.X_OK) else 0o644
        sortie += entree_cpio(numero, stat.S_IFREG | droits, len(donnees), nom)
        sortie += donnees + b"\0" * (-len(donnees) % 4)
    sortie += entree_cpio(0, 0, 0, "TRAILER!!!")
    return bytes(sortie)


def config_limine(resolution, cmdline, avec_initrd):
    lignes = [
        "timeout: 0",
        "",
        f"/{NOM_ENTREE}",
        "    protocol: limine",
        "    path: boot():/boot/kernel.elf",
    ]
    if cmdline:
        lignes.append(f"    cmdline: {cmdline}")
    if avec_initrd:
        lignes.append("    module_path: boot():/boot/initrd.cpio")
    lignes.append(f"    resolution: {resolution}x32")
    return "\n".join(lignes) + "\n"


def commande_gpt(image, plan):
    esp_fin = (plan.esp_debut + plan.esp_taille) // SECTEUR - 1
    bios_fin = (plan.bios_debut + plan.bios_taille) // SECTEUR - 1
    return [
        "sgdisk", "-o",
        "-n", f"1:{plan.esp_debut // SECTEUR}:{esp_fin}", "-t", "1:EF00", "-c", "1:ESP",
        "-n", f"2:{plan.bios_debut // SECTEUR}:{bios_fin}", "-t", "2:EF02", "-c", "2:bios",
        str(image),
    ]  # fmt: skip


def remplir_esp(image, plan, fichiers):
    cible = f"{image}@@{plan.esp_debut}"
    lancer(["mformat", "-i", cible, "-T", str(plan.esp_taille // SECTEUR),
            "-h", "64", "-s", "32", "-v", "ESP", "::"])  # fmt: skip
    lancer(["mmd", "-i", cible, "::/EFI", "::/EFI/BOOT", "::/boot", "::/boot/limine"])
    for destination, source in sorted(fichiers.items()):
        lancer(["mcopy", "-i", cible, str(source), "::" + destination])


def fichiers_esp(travail, args, limine, initrd):
    conf = travail / "limine.conf"
    conf.write_text(config_limine(args.resolution, args.cmdline, initrd is not None))
    fichiers = {
        "/boot/kernel.elf": args.noyau,
        "/boot/limine/limine.conf": conf,
        "/boot/limine/limine-bios.sys": limine / "limine-bios.sys",
    }
    for nom in FICHIERS_EFI:
        fichiers[f"/EFI/BOOT/{nom}"] = limine / nom
    if initrd is not None:
        archive = travail / "initrd.cpio"
        archive.write_bytes(initrd)
        fichiers["/boot/initrd.cpio"] = archive
    return fichiers


def verifier_entrees(args, limine):
    if not args.noyau.is_file():
        raise Erreur(f"noyau introuvable : {args.noyau}")
    if args.initrd is not None and not args.initrd.is_dir():
        raise Erreur(f"dossier introuvable : {args.initrd}")
    manquants = [
        n for n in ("limine", "limine-bios.sys", *FICHIERS_EFI) if not (limine / n).is_file()
    ]
    if manquants:
        raise Erreur(
            f"Limine incomplet dans {limine} ({', '.join(manquants)}) : lancer `make limine`"
        )


def fabriquer(args):
    limine = args.limine
    verifier_entrees(args, limine)
    plan = planifier(args.esp_mio)
    image = args.sortie
    travail = image.parent / (image.name + ".travail")
    travail.mkdir(parents=True, exist_ok=True)
    initrd = cpio_newc(args.initrd) if args.initrd else None
    image.unlink(missing_ok=True)
    with image.open("wb") as flux:
        flux.truncate(plan.taille)
    lancer(commande_gpt(image, plan))
    remplir_esp(image, plan, fichiers_esp(travail, args, limine, initrd))
    lancer([str(limine / "limine"), "bios-install", str(image)])
    lancer(["sgdisk", "-v", str(image)])
    shutil.rmtree(travail)
    return plan


def analyser(argv):
    parseur = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parseur.add_argument("--noyau", type=Path, required=True)
    parseur.add_argument("--sortie", type=Path, required=True)
    parseur.add_argument("--initrd", type=Path)
    defaut = Path(os.environ.get("LIMINE_DIR", RACINE / "build" / "limine"))
    parseur.add_argument("--limine", type=Path, default=defaut)
    parseur.add_argument("--resolution", default="1024x768")
    parseur.add_argument("--cmdline", default="")
    parseur.add_argument("--esp-mio", type=int, default=64)
    return parseur.parse_args(argv)


def main(argv):
    args = analyser(argv)
    try:
        plan = fabriquer(args)
    except Erreur as erreur:
        print(f"mkimage : {erreur}", file=sys.stderr)
        return 1
    taille, esp = plan.taille // MIO, plan.esp_taille // MIO
    print(f"{args.sortie} : {taille} Mio, ESP {esp} Mio, Limine BIOS et UEFI")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
