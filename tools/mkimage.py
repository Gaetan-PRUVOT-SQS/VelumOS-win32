#!/usr/bin/env python3
import argparse
import importlib.util
import shutil
import sys
from pathlib import Path


def charger_outil():
    chemin = shutil.which("image-disque")
    if chemin is None:
        raise SystemExit("mkimage: image-disque est introuvable dans le PATH")
    spec = importlib.util.spec_from_file_location("image_disque", Path(chemin).resolve())
    module = importlib.util.module_from_spec(spec)
    sys.modules["image_disque"] = module
    spec.loader.exec_module(module)
    return module


def config_avec_resolution(module, resolution):
    de_base = module.config_limine

    def config(nom, protocole, initrd, cmdline):
        texte = de_base(nom, protocole, initrd, cmdline).replace("serial: yes\n", "").rstrip("\n")
        return f"{texte}\n    resolution: {resolution}x32\n"

    return config


def analyser(argv):
    parseur = argparse.ArgumentParser(description="Image disque VelumOS (Limine, BIOS et UEFI)")
    parseur.add_argument("--noyau", required=True)
    parseur.add_argument("--sortie", required=True)
    parseur.add_argument("--initrd")
    parseur.add_argument("--racine")
    parseur.add_argument("--resolution", default="1024x768")
    parseur.add_argument("--cmdline", default="")
    parseur.add_argument("--taille-mio", default="128")
    parseur.add_argument("--esp-mio", default="64")
    return parseur.parse_args(argv)


def main(argv):
    args = analyser(argv)
    module = charger_outil()
    module.config_limine = config_avec_resolution(module, args.resolution)
    commande = [
        "--noyau",
        args.noyau,
        "--sortie",
        args.sortie,
        "--protocole",
        "limine",
        "--nom",
        "VelumOS",
        "--taille-mio",
        args.taille_mio,
        "--esp-mio",
        args.esp_mio,
        "--cmdline",
        args.cmdline,
    ]
    if args.initrd:
        commande += ["--initrd", args.initrd]
    if args.racine:
        commande += ["--racine", args.racine]
    return module.main(commande)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
