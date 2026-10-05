import subprocess

NAME = "bureau : connexion au clavier, barre des tâches, menu Démarrer, extinction"
CMDLINE = "quiet"
MEMOIRE = "256M"

LARGEUR = 1024
HAUTEUR = 768
HAUTEUR_BARRE = 30
BARRE = (0, HAUTEUR - HAUTEUR_BARRE, LARGEUR, HAUTEUR_BARRE)
BOUTON_DEMARRER = (0, HAUTEUR - HAUTEUR_BARRE, 110, HAUTEUR_BARRE)
COIN_DROIT = (LARGEUR - 200, HAUTEUR - HAUTEUR_BARRE, 200, HAUTEUR_BARRE)
BLEU_BARRE = (0x24, 0x5E, 0xDC)
VERT_DEMARRER = (0x3C, 0x8A, 0x2E)
TOLERANCE_BLEU = 40
TOLERANCE_VERT = 48
PART_BLEUE_MINI = 0.30
PIXELS_VERTS_MINI = 200
PIXELS_MENU_MINI = 20_000
CARTE_COIN = (592, 283)


def verifier_ecran_de_connexion(image):
    rouge, vert, bleu = image.pixel(*CARTE_COIN)
    ecart = max(abs(rouge - 0xEC), abs(vert - 0xE9), abs(bleu - 0xD8))
    assert ecart <= 8, f"carte de connexion couleur fenêtre attendue ({rouge}, {vert}, {bleu})"
    assert image.compter(BARRE, BLEU_BARRE, 0) < LARGEUR * HAUTEUR_BARRE, "pas de barre des tâches"


def verifier_barre_des_taches(image):
    bleus = image.compter(BARRE, BLEU_BARRE, TOLERANCE_BLEU)
    surface = LARGEUR * HAUTEUR_BARRE
    assert bleus > PART_BLEUE_MINI * surface, f"barre bleue attendue ({bleus} pixels sur {surface})"
    verts = image.compter(BOUTON_DEMARRER, VERT_DEMARRER, TOLERANCE_VERT)
    assert verts > PIXELS_VERTS_MINI, f"bouton Démarrer vert à gauche attendu ({verts} pixels)"
    assert image.compter(COIN_DROIT, VERT_DEMARRER, 20) == 0, "pas de vert à droite de la barre"


def attendre_arret(vm, delai):
    try:
        vm.processus.wait(timeout=delai)
    except subprocess.TimeoutExpired as erreur:
        raise AssertionError("QEMU devait quitter après « Arrêter »") from erreur


def run(vm):
    vm.attendre(r"logon: écran de connexion prêt", delai=120)
    ecran_connexion = vm.capture("connexion.ppm")
    assert ecran_connexion.largeur == LARGEUR and ecran_connexion.hauteur == HAUTEUR
    verifier_ecran_de_connexion(ecran_connexion)

    vm.touche("ret")
    vm.attendre(r"logon: session ouverte", delai=60)
    vm.attendre(r"shell: bureau prêt", delai=60)
    bureau = vm.capture("bureau.ppm")
    verifier_barre_des_taches(bureau)

    vm.touche("meta_l")
    menu = vm.capture("menu.ppm")
    assert menu.differences(bureau) > PIXELS_MENU_MINI, "le menu Démarrer doit s'afficher"
    vm.touche("esc")
    ferme = vm.capture("menu_ferme.ppm")
    assert ferme.differences(bureau) < PIXELS_MENU_MINI, "Échap doit refermer le menu"

    vm.touche("meta_l")
    vm.touche("end")
    vm.touche("ret")
    vm.touche("ret")
    attendre_arret(vm, delai=60)
