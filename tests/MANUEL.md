# Plan de tests manuels

Fichier généré par `tools/matrice_couverture.py` à partir de [`matrice.toml`](matrice.toml). Il couvre ce que
l'automatisation ne vérifie pas : jugement visuel, ergonomie, clavier et matériel réels, endurance, revue
linguistique, reproductibilité sur machine propre. Les liens avec les fonctionnalités sont dans
[`MATRICE.md`](MATRICE.md).

Aucun de ces cas n'a été exécuté par une personne en session interactive. Les cas « Relu sur capture » ont été
vérifiés le 5 octobre 2026 par inspection des captures produites par les scénarios, ce qui ne remplace pas une session
réelle. À chaque exécution, noter la date, la version (identifiant de build affiché au démarrage), le résultat et les
anomalies dans le registre des défauts de [`../TESTS.md`](../TESTS.md).

21 cas, durée estimée 14 h 35 min. Statuts : 17 a exécuter, 1 partiel, 3 relu sur capture.

| ID | Cas | Durée (min) | Statut |
|----|-----|-------------|--------|
| MT-01 | Écran de démarrage | 5 | Relu sur capture |
| MT-02 | Démarrage en UEFI | 10 | A exécuter |
| MT-04 | Fidélité visuelle au style Luna | 45 | A exécuter |
| MT-05 | Lisibilité des polices | 10 | Relu sur capture |
| MT-06 | Manipulation des fenêtres à la souris | 15 | A exécuter |
| MT-07 | Menu Démarrer au clavier | 10 | A exécuter |
| MT-08 | Boîtes Exécuter et Éteindre | 10 | A exécuter |
| MT-09 | Redémarrage ACPI | 5 | A exécuter |
| MT-10 | Horloge de la barre des tâches | 5 | A exécuter |
| MT-11 | Saisie au clavier AZERTY | 15 | A exécuter |
| MT-13 | Session entière au clavier seul | 15 | A exécuter |
| MT-14 | Contraste des textes | 10 | Relu sur capture |
| MT-16 | Plusieurs fenêtres dans la barre des tâches | 10 | A exécuter |
| MT-17 | Endurance | 480 | A exécuter |
| MT-18 | Matériel réel | 60 | A exécuter |
| MT-19 | Fluidité perçue | 10 | A exécuter |
| MT-20 | Mort du serveur de fenêtres | 15 | A exécuter |
| MT-21 | Lisibilité de l'écran d'arrêt | 5 | A exécuter |
| MT-22 | Revue linguistique de l'interface | 20 | A exécuter |
| MT-23 | FAT32 sur une image réelle | 30 | A exécuter |
| MT-24 | Construction sur une machine propre | 90 | Partiel |

## Fiches

### MT-01 : Écran de démarrage

- Fonctionnalités : F47
- Objectif : Vérifier l'aspect de l'écran de démarrage et la progression de la barre.
- Préconditions : make all, image construite.
- Durée estimée : 5 min
- Statut : Relu sur capture

1. Lancer make run.
2. Observer l'écran dès l'ouverture de la fenêtre QEMU.
3. Noter l'emblème, le nom, la barre à trois blocs et le texte.

Résultat attendu : Fond noir, emblème centré, nom VelumOS, barre de progression qui avance, aucun artefact.

### MT-02 : Démarrage en UEFI

- Fonctionnalités : F79
- Objectif : Vérifier que le système démarre avec OVMF et affiche la même session qu'en BIOS.
- Préconditions : OVMF installé, image construite.
- Durée estimée : 10 min
- Statut : A exécuter

1. Lancer make run-uefi.
2. Ouvrir la session.
3. Comparer avec le démarrage BIOS.

Résultat attendu : Même écran de démarrage, même bureau, mêmes entrées clavier et souris.

### MT-04 : Fidélité visuelle au style Luna

- Fonctionnalités : F61
- Objectif : Comparer les dimensions et les couleurs avec des captures de référence documentées.
- Préconditions : Captures de référence de l'interface d'époque (documentation publique).
- Durée estimée : 45 min
- Statut : A exécuter

1. Capturer le bureau, une fenêtre et le menu Démarrer à 1024x768.
2. Mesurer barre des tâches, barre de titre, boutons de légende, bouton Démarrer.
3. Noter les écarts en pixels.

Résultat attendu : Écarts de un pixel au plus sur les dimensions, couleurs proches de la palette Luna.

### MT-05 : Lisibilité des polices

- Fonctionnalités : F58
- Objectif : Vérifier la netteté des textes et le rendu des accents français.
- Préconditions : Session ouverte.
- Durée estimée : 10 min
- Statut : Relu sur capture

1. Ouvrir la fenêtre Bonjour.
2. Lire les libellés des icônes, le titre, le menu Démarrer.
3. Vérifier é, è, ê, à, ç, œ, «, ».

Résultat attendu : Textes lisibles à 100 %, accents complets, graisses régulières.

### MT-06 : Manipulation des fenêtres à la souris

- Fonctionnalités : F65
- Objectif : Déplacer, redimensionner, réduire, agrandir, restaurer et fermer une fenêtre.
- Préconditions : Session ouverte, fenêtre Bonjour ouverte.
- Durée estimée : 15 min
- Statut : A exécuter

1. Glisser la barre de titre.
2. Redimensionner par les bords et les coins.
3. Utiliser les trois boutons de légende.
4. Cliquer une autre fenêtre et vérifier l'ordre.

Résultat attendu : Mouvements fluides, tailles minimales respectées, ordre et focus cohérents.

### MT-07 : Menu Démarrer au clavier

- Fonctionnalités : F74
- Objectif : Parcourir le menu avec les flèches, Entrée, Échap et la touche Windows.
- Préconditions : Session ouverte.
- Durée estimée : 10 min
- Statut : A exécuter

1. Appuyer sur la touche Windows.
2. Naviguer avec les flèches, ouvrir Tous les programmes.
3. Fermer avec Échap.

Résultat attendu : Sélection visible, sous-menu ouvert et fermé correctement, focus rendu à la fenêtre précédente.

### MT-08 : Boîtes Exécuter et Éteindre

- Fonctionnalités : F74
- Objectif : Vérifier les boutons Arrêter, Redémarrer et Annuler.
- Préconditions : Session ouverte.
- Durée estimée : 10 min
- Statut : A exécuter

1. Menu Démarrer puis Éteindre.
2. Essayer Annuler, puis Redémarrer, puis Arrêter.
3. Ouvrir Exécuter et saisir une commande inconnue.

Résultat attendu : Annuler ferme la boîte, Redémarrer relance le système, Arrêter coupe la machine, message d'erreur clair pour la commande.

### MT-09 : Redémarrage ACPI

- Fonctionnalités : F20
- Objectif : Vérifier que Redémarrer relance bien la machine.
- Préconditions : Image construite.
- Durée estimée : 5 min
- Statut : A exécuter

1. Choisir Redémarrer dans la boîte d'extinction.
2. Observer le redémarrage sous QEMU sans l'option -no-reboot.

Résultat attendu : La machine redémarre et affiche de nouveau l'écran de démarrage.

### MT-10 : Horloge de la barre des tâches

- Fonctionnalités : F75
- Objectif : Vérifier l'heure affichée et sa mise à jour.
- Préconditions : Session ouverte.
- Durée estimée : 5 min
- Statut : A exécuter

1. Comparer l'heure affichée avec l'heure de l'hôte.
2. Attendre un changement de minute.

Résultat attendu : Heure cohérente (UTC pour l'instant), mise à jour à la minute.

### MT-11 : Saisie au clavier AZERTY

- Fonctionnalités : F38
- Objectif : Vérifier les caractères spéciaux, AltGr et touches mortes.
- Préconditions : Session ouverte, un champ de saisie (boîte Exécuter).
- Durée estimée : 15 min
- Statut : A exécuter

1. Taper les chiffres avec Maj.
2. Taper @ # { [ | \ ^ ] } avec AltGr.
3. Taper ^e, ¨u, ~n et `a avec les touches mortes.

Résultat attendu : Caractères conformes à la disposition française ; les touches mortes ~ et ` sont à confirmer.

### MT-13 : Session entière au clavier seul

- Fonctionnalités : F70
- Objectif : Parcourir connexion, bureau et menu sans souris.
- Préconditions : Image construite.
- Durée estimée : 15 min
- Statut : A exécuter

1. Valider la connexion avec Tab et Entrée.
2. Ouvrir le menu Démarrer, lancer Bonjour.
3. Fermer la fenêtre et éteindre.

Résultat attendu : Tout est atteignable au clavier, le focus est toujours visible.

### MT-14 : Contraste des textes

- Fonctionnalités : F58
- Objectif : Vérifier la lisibilité des textes sur leurs arrière-plans.
- Préconditions : Session ouverte.
- Durée estimée : 10 min
- Statut : Relu sur capture

1. Observer libellés d'icônes, horloge, invite de connexion, menu Démarrer.

Résultat attendu : Aucun texte difficile à lire (rapport de contraste d'au moins 4,5 pour le texte courant).

### MT-16 : Plusieurs fenêtres dans la barre des tâches

- Fonctionnalités : F73
- Objectif : Vérifier le rétrécissement des boutons, l'activation et la réduction.
- Préconditions : Session ouverte.
- Durée estimée : 10 min
- Statut : A exécuter

1. Ouvrir plusieurs fois Bonjour.
2. Cliquer les boutons de la barre des tâches.
3. Réduire et restaurer.

Résultat attendu : Boutons de largeur régulière, clic = activer ou réduire, ordre cohérent.

### MT-17 : Endurance

- Fonctionnalités : F80
- Objectif : Détecter fuites de mémoire et gels sur une longue durée.
- Préconditions : Session ouverte, journal série ouvert.
- Durée estimée : 480 min
- Statut : A exécuter

1. Laisser le système tourner 8 heures avec des actions répétées (ouverture et fermeture de fenêtres).
2. Relever mémoire libre et tas toutes les 30 minutes.

Résultat attendu : Mémoire libre stable, aucun gel, aucune panique.

### MT-18 : Matériel réel

- Fonctionnalités : F79
- Objectif : Démarrer sur une machine physique.
- Préconditions : Clé USB écrite avec l'image, machine UEFI avec clavier PS/2 ou émulation.
- Durée estimée : 60 min
- Statut : A exécuter

1. Écrire l'image sur la clé.
2. Démarrer la machine.
3. Ouvrir la session, utiliser clavier et souris.

Résultat attendu : Démarrage complet ; sinon consigner le point d'arrêt à partir du journal série.

### MT-19 : Fluidité perçue

- Fonctionnalités : F55
- Objectif : Évaluer la latence de la souris et le déplacement d'une fenêtre.
- Préconditions : Session ouverte, KVM actif.
- Durée estimée : 10 min
- Statut : A exécuter

1. Déplacer rapidement le curseur.
2. Glisser une grande fenêtre.

Résultat attendu : Pas de saccade visible ni de retard notable.

### MT-20 : Mort du serveur de fenêtres

- Fonctionnalités : F29
- Objectif : Vérifier que init relance le serveur et la session.
- Préconditions : Session ouverte, accès à un moyen d'arrêter winsrv (outil de test).
- Durée estimée : 15 min
- Statut : A exécuter

1. Arrêter le processus winsrv.
2. Observer l'écran et le journal.

Résultat attendu : L'écran de connexion revient, le noyau ne panique pas, le journal montre la relance.

### MT-21 : Lisibilité de l'écran d'arrêt

- Fonctionnalités : F45
- Objectif : Relire le texte de l'écran d'arrêt.
- Préconditions : Image construite.
- Durée estimée : 5 min
- Statut : A exécuter

1. Démarrer avec l'option fault=panic.
2. Lire l'écran.

Résultat attendu : Fond bleu, texte en français lisible, code d'arrêt et identifiant de build présents.

### MT-22 : Revue linguistique de l'interface

- Fonctionnalités : F81
- Objectif : Relire tous les textes de l'interface.
- Préconditions : Session ouverte.
- Durée estimée : 20 min
- Statut : A exécuter

1. Parcourir connexion, bureau, menu, boîtes de dialogue, fenêtre Bonjour.
2. Noter fautes, espaces insécables, majuscules.

Résultat attendu : Orthographe et typographie françaises correctes.

### MT-23 : FAT32 sur une image réelle

- Fonctionnalités : F43
- Objectif : Vérifier lecture, écriture et cohérence d'un volume FAT32.
- Préconditions : mtools et dosfstools installés.
- Durée estimée : 30 min
- Statut : A exécuter

1. Créer une image FAT32 avec mformat et y copier des fichiers (mcopy).
2. La brancher en virtio-blk et démarrer avec selftest.
3. Lire la ligne d'autotest du VFS.
4. Contrôler l'image avec fsck.vfat.

Résultat attendu : Autotest du VFS réussi, aucune erreur de fsck.vfat.

### MT-24 : Construction sur une machine propre

- Fonctionnalités : F82
- Objectif : Rejouer la procédure du README sur une machine vierge.
- Préconditions : Une distribution Linux récente.
- Durée estimée : 90 min
- Statut : Partiel

1. Installer les prérequis.
2. Lancer tools/build-toolchain.sh.
3. make limine, make all, make test-qemu.

Résultat attendu : Chaque commande réussit sans intervention.

Note : La chaîne d'outils et l'image ont été reconstruites avec succès le 5 octobre 2026 sur la machine de développement, pas sur une machine vierge.
