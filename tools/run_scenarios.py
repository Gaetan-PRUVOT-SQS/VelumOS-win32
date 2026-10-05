#!/usr/bin/env python3
import argparse
import concurrent.futures
import hashlib
import importlib.util
import subprocess
import sys
import time
import traceback
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from vtest import VM, VMError  # noqa: E402

RACINE = Path(__file__).resolve().parent.parent


def charger_scenarios(filtre):
    trouves = []
    for chemin in sorted((RACINE / "tests" / "qemu").glob("*.py")):
        if filtre and filtre not in chemin.stem:
            continue
        spec = importlib.util.spec_from_file_location(chemin.stem, chemin)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        trouves.append((chemin.stem, module))
    return trouves


def plus_recent(args):
    dates = [Path(args.kernel).stat().st_mtime]
    dates += [f.stat().st_mtime for f in Path(args.root).rglob("*") if f.is_file()]
    return max(dates)


def image_pour(args, cmdline, sortie):
    cle = hashlib.sha256(f"{cmdline}|{args.resolution}".encode()).hexdigest()[:10]
    image = sortie / f"disque-{cle}.img"
    if not image.exists() or image.stat().st_mtime < plus_recent(args):
        commande = [
            sys.executable,
            str(RACINE / "tools" / "mkimage.py"),
            "--noyau",
            args.kernel,
            "--sortie",
            str(image),
            "--initrd",
            args.root,
            "--resolution",
            args.resolution,
            "--cmdline",
            cmdline,
        ]
        subprocess.run(commande, check=True, capture_output=True)
    return image


def jouer(args, nom, module, firmware, sortie, image):
    debut = time.monotonic()
    machine = VM(
        image,
        sortie / nom,
        nom=f"{nom}-{firmware}",
        uefi=(firmware == "uefi"),
        memoire=getattr(module, "MEMOIRE", "256M"),
        smp=getattr(module, "SMP", 1),
        extra=getattr(module, "QEMU_EXTRA", ()),
    )
    try:
        with machine:
            module.run(machine)
        return nom, firmware, None, time.monotonic() - debut
    except (VMError, AssertionError, OSError) as erreur:
        return (
            nom,
            firmware,
            f"{erreur}\n{traceback.format_exc(limit=3)}",
            time.monotonic() - debut,
        )


def main(argv):
    parseur = argparse.ArgumentParser()
    parseur.add_argument("--kernel", required=True)
    parseur.add_argument("--root", required=True)
    parseur.add_argument("--out", required=True)
    parseur.add_argument("--jobs", type=int, default=4)
    parseur.add_argument("--filter", default="")
    parseur.add_argument("--resolution", default="1024x768")
    args = parseur.parse_args(argv)
    sortie = Path(args.out)
    sortie.mkdir(parents=True, exist_ok=True)
    travaux = []
    for nom, module in charger_scenarios(args.filter):
        for firmware in getattr(module, "FIRMWARES", ("bios", "uefi")):
            travaux.append((nom, module, firmware))
    images = {}
    for _, module, _ in travaux:
        cmdline = getattr(module, "CMDLINE", "quiet")
        if cmdline not in images:
            images[cmdline] = image_pour(args, cmdline, sortie)
    echecs = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futurs = [
            pool.submit(jouer, args, n, m, f, sortie, images[getattr(m, "CMDLINE", "quiet")])
            for n, m, f in travaux
        ]
        for futur in futurs:
            nom, firmware, erreur, duree = futur.result()
            etat = "OK" if erreur is None else "ECHEC"
            print(f"{etat:6} {nom} [{firmware}] {duree:5.1f}s")
            if erreur:
                echecs += 1
                print(erreur, file=sys.stderr)
    print(f"scénarios : {len(travaux) - echecs}/{len(travaux)} réussis")
    return 1 if echecs else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
