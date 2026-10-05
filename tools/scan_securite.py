#!/usr/bin/env python3
"""Contrôle statique minimal : fonctions C dangereuses, secrets, idiomes risqués."""

import re
import subprocess
import sys
from pathlib import Path

RACINE = Path(__file__).resolve().parent.parent
REGLES = {
    ".c": [r"\b(strcpy|strcat|sprintf|vsprintf|gets)\s*\("],
    ".h": [r"\b(strcpy|strcat|sprintf|vsprintf|gets)\s*\("],
    ".py": [r"\bos\.system\s*\(", r"shell\s*=\s*True", r"\beval\s*\(", r"\bexec\s*\("],
    ".sh": [r"\beval\s", r"curl[^|\n]*\|\s*(ba)?sh"],
}
SECRETS = [
    r"ghp_[A-Za-z0-9]{30,}",
    r"github_pat_[A-Za-z0-9_]{30,}",
    r"AKIA[0-9A-Z]{16}",
    r"-----BEGIN [A-Z ]*PRIVATE KEY-----",
]
EXCLUS = ("third_party/", "build/", "LICENSES/")


def fichiers():
    sortie = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "-z"],
        cwd=RACINE,
        capture_output=True,
        check=True,
    ).stdout.decode()
    return [f for f in sortie.split("\0") if f and not f.startswith(EXCLUS)]


def analyser(chemin):
    texte = (RACINE / chemin).read_text(errors="replace")
    motifs = REGLES.get(Path(chemin).suffix, []) + SECRETS
    trouves = []
    for numero, ligne in enumerate(texte.splitlines(), start=1):
        if any(re.search(m, ligne) for m in motifs):
            trouves.append(f"{chemin}:{numero}: {ligne.strip()[:100]}")
    return trouves


def main():
    trouves = []
    for chemin in fichiers():
        if (RACINE / chemin).is_file():
            trouves += analyser(chemin)
    print("\n".join(trouves))
    print(f"scan sécurité : {len(trouves)} alerte(s)", file=sys.stderr)
    return 1 if trouves else 0


if __name__ == "__main__":
    sys.exit(main())
