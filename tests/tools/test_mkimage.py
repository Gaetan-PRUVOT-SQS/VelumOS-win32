import shutil
import subprocess
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parent.parent.parent / "tools"))
import mkimage  # noqa: E402


def fabriquer_arbre(racine):
    (racine / "system" / "bin").mkdir(parents=True)
    (racine / "system" / "etc").mkdir()
    (racine / "system" / "bin" / "init").write_bytes(b"\x7fELF" + b"x" * 5)
    (racine / "system" / "bin" / "init").chmod(0o755)
    (racine / "system" / "etc" / "os-release").write_bytes(b"VelumOS\n")
    (racine / "vide").write_bytes(b"")


def lire_cpio(brut):
    entrees, pos = {}, 0
    while True:
        assert brut[pos : pos + 6] == b"070701"
        champs = [int(brut[pos + 6 + 8 * i : pos + 14 + 8 * i], 16) for i in range(13)]
        mode, taille, nom_len = champs[1], champs[6], champs[11]
        debut_nom = pos + 110
        nom = brut[debut_nom : debut_nom + nom_len - 1].decode()
        debut_donnees = (debut_nom + nom_len + 3) & ~3
        if nom == "TRAILER!!!":
            return entrees
        entrees[nom] = (mode, brut[debut_donnees : debut_donnees + taille])
        pos = (debut_donnees + taille + 3) & ~3


def test_cpio_contient_fichiers_et_dossiers(tmp_path):
    fabriquer_arbre(tmp_path)
    entrees = lire_cpio(mkimage.cpio_newc(tmp_path))
    assert set(entrees) == {
        ".", "system", "system/bin", "system/bin/init", "system/etc",
        "system/etc/os-release", "vide",
    }  # fmt: skip
    assert entrees["system/bin/init"][1] == b"\x7fELF" + b"x" * 5
    assert entrees["system/bin/init"][0] == 0o100755
    assert entrees["system/etc/os-release"][0] == 0o100644
    assert entrees["system"][0] == 0o040755
    assert entrees["vide"][1] == b""


def test_cpio_est_deterministe_et_aligne(tmp_path):
    fabriquer_arbre(tmp_path)
    premier = mkimage.cpio_newc(tmp_path)
    assert premier == mkimage.cpio_newc(tmp_path)
    assert len(premier) % 4 == 0


def test_cpio_refuse_les_liens_symboliques(tmp_path):
    fabriquer_arbre(tmp_path)
    (tmp_path / "lien").symlink_to(tmp_path / "vide")
    with pytest.raises(mkimage.Erreur):
        mkimage.cpio_newc(tmp_path)


@pytest.mark.skipif(shutil.which("cpio") is None, reason="cpio absent")
def test_cpio_relu_par_gnu_cpio(tmp_path):
    fabriquer_arbre(tmp_path)
    sortie = subprocess.run(
        ["cpio", "-t", "--quiet"],
        input=mkimage.cpio_newc(tmp_path),
        capture_output=True,
        check=True,
    ).stdout.decode()
    assert "system/bin/init" in sortie.split()


def test_plan_aligne_et_refuse_une_esp_trop_petite():
    plan = mkimage.planifier(64)
    assert plan.esp_debut % mkimage.MIO == 0
    assert plan.taille == plan.esp_debut + plan.esp_taille + mkimage.MIO
    with pytest.raises(mkimage.Erreur):
        mkimage.planifier(4)


def test_config_limine_contient_module_et_resolution():
    texte = mkimage.config_limine("1024x768", "quiet", True)
    assert "module_path: boot():/boot/initrd.cpio" in texte
    assert "resolution: 1024x768x32" in texte
    assert "cmdline: quiet" in texte
    assert "initrd" not in mkimage.config_limine("800x600", "", False)
