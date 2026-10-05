import os
import re
import shutil
import socket
import subprocess
import time
from pathlib import Path

OVMF_CANDIDATS = (
    ("/usr/share/edk2/ovmf/OVMF_CODE.fd", "/usr/share/edk2/ovmf/OVMF_VARS.fd"),
    ("/usr/share/OVMF/OVMF_CODE_4M.fd", "/usr/share/OVMF/OVMF_VARS_4M.fd"),
    ("/usr/share/OVMF/OVMF_CODE.fd", "/usr/share/OVMF/OVMF_VARS.fd"),
    ("/usr/share/edk2/x64/OVMF_CODE.4m.fd", "/usr/share/edk2/x64/OVMF_VARS.4m.fd"),
)


def trouver_ovmf():
    code, variables = os.environ.get("OVMF_CODE"), os.environ.get("OVMF_VARS")
    if code and variables:
        return code, variables
    for candidat in OVMF_CANDIDATS:
        if all(os.path.exists(c) for c in candidat):
            return candidat
    raise FileNotFoundError("OVMF introuvable : définir OVMF_CODE et OVMF_VARS")


OVMF_CODE, OVMF_VARS = trouver_ovmf()


class VMError(Exception):
    pass


class Image:
    def __init__(self, largeur, hauteur, donnees):
        self.largeur = largeur
        self.hauteur = hauteur
        self.donnees = donnees

    @classmethod
    def lire_ppm(cls, chemin):
        brut = Path(chemin).read_bytes()
        champs = re.match(rb"P6\s+(\d+)\s+(\d+)\s+255\s", brut)
        if champs is None:
            raise VMError(f"ppm illisible : {chemin}")
        largeur, hauteur = int(champs.group(1)), int(champs.group(2))
        return cls(largeur, hauteur, brut[champs.end() :])

    def pixel(self, x, y):
        debut = (y * self.largeur + x) * 3
        return tuple(self.donnees[debut : debut + 3])

    def compter(self, rect, couleur, tolerance=0):
        x0, y0, largeur, hauteur = rect
        total = 0
        for y in range(y0, y0 + hauteur):
            for x in range(x0, x0 + largeur):
                reel = self.pixel(x, y)
                if all(abs(a - b) <= tolerance for a, b in zip(reel, couleur, strict=True)):
                    total += 1
        return total

    def differences(self, autre, tolerance=0):
        if (self.largeur, self.hauteur) != (autre.largeur, autre.hauteur):
            return self.largeur * self.hauteur
        total = 0
        for i in range(0, len(self.donnees), 3):
            a, b = self.donnees[i : i + 3], autre.donnees[i : i + 3]
            if any(abs(p - q) > tolerance for p, q in zip(a, b, strict=True)):
                total += 1
        return total

    def enregistrer_png(self, chemin):
        from PIL import Image as PilImage

        PilImage.frombytes("RGB", (self.largeur, self.hauteur), bytes(self.donnees)).save(
            chemin, optimize=True
        )


class VM:
    def __init__(self, disque, dossier, nom="vm", uefi=False, memoire="256M", smp=1, extra=()):
        self.disque = Path(disque)
        self.dossier = Path(dossier)
        self.nom = nom
        self.uefi = uefi
        self.memoire = memoire
        self.smp = smp
        self.extra = list(extra)
        self.processus = None
        self.journal = self.dossier / f"{nom}.serie.log"
        self.socket_moniteur = self.dossier / f"{nom}.mon"
        self.capture_n = 0

    def commande(self):
        accel = (
            ["-enable-kvm", "-cpu", "host"] if os.access("/dev/kvm", os.W_OK) else ["-cpu", "max"]
        )
        base = [
            "qemu-system-x86_64",
            "-machine",
            "q35",
            "-m",
            self.memoire,
            "-smp",
            str(self.smp),
            *accel,
            "-display",
            "none",
            "-vga",
            "std",
            "-no-reboot",
            "-drive",
            f"file={self.disque},format=raw,snapshot=on",
            "-serial",
            f"file:{self.journal}",
            "-monitor",
            f"unix:{self.socket_moniteur},server,nowait",
            "-device",
            "isa-debug-exit,iobase=0xf4,iosize=0x04",
        ]
        if self.uefi:
            vars_copie = self.dossier / f"{self.nom}.vars.fd"
            shutil.copyfile(OVMF_VARS, vars_copie)
            base += [
                "-drive",
                f"if=pflash,format=raw,readonly=on,file={OVMF_CODE}",
                "-drive",
                f"if=pflash,format=raw,file={vars_copie}",
            ]
        return base + self.extra

    def demarrer(self):
        self.dossier.mkdir(parents=True, exist_ok=True)
        self.journal.write_text("")
        self.processus = subprocess.Popen(
            self.commande(), stdout=subprocess.DEVNULL, stderr=subprocess.PIPE
        )
        debut = time.monotonic()
        while not self.socket_moniteur.exists():
            if time.monotonic() - debut > 10:
                raise VMError("moniteur QEMU absent")
            time.sleep(0.05)
        return self

    def serie(self):
        return self.journal.read_text(errors="replace")

    def attendre(self, motif, delai=30):
        regex = re.compile(motif)
        fin = time.monotonic() + delai
        while time.monotonic() < fin:
            texte = self.serie()
            trouve = regex.search(texte)
            if trouve:
                return trouve
            if "PANIC:" in texte and "PANIC" not in motif:
                raise VMError(f"panique pendant l'attente de {motif!r}\n{texte[-2000:]}")
            if self.processus.poll() is not None:
                raise VMError(f"QEMU arrêté (code {self.processus.returncode})\n{texte[-2000:]}")
            time.sleep(0.05)
        raise VMError(f"délai dépassé pour {motif!r}\n{self.serie()[-2000:]}")

    def moniteur(self, commande, delai=5):
        with socket.socket(socket.AF_UNIX) as sock:
            sock.settimeout(delai)
            sock.connect(str(self.socket_moniteur))
            self._lire_invite(sock)
            sock.sendall(commande.encode() + b"\n")
            return self._lire_invite(sock)

    @staticmethod
    def _lire_invite(sock):
        tampon = b""
        while not tampon.endswith(b"(qemu) "):
            morceau = sock.recv(4096)
            if not morceau:
                break
            tampon += morceau
        return tampon.decode(errors="replace")

    def capture(self, nom=None):
        self.capture_n += 1
        chemin = self.dossier / (nom or f"{self.nom}.{self.capture_n}.ppm")
        self.moniteur(f"screendump {chemin}")
        fin = time.monotonic() + 5
        while not chemin.exists() and time.monotonic() < fin:
            time.sleep(0.05)
        return Image.lire_ppm(chemin)

    def touche(self, nom):
        self.moniteur(f"sendkey {nom}")

    def souris_deplacer(self, dx, dy):
        self.moniteur(f"mouse_move {dx} {dy}")

    def souris_bouton(self, masque):
        self.moniteur(f"mouse_button {masque}")

    def arreter(self):
        if self.processus and self.processus.poll() is None:
            self.processus.kill()
        if self.processus:
            self.processus.wait()

    def __enter__(self):
        return self.demarrer()

    def __exit__(self, *exc):
        self.arreter()
