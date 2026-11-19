## Structure du dépôt

```text
tpi/
├── README.md
└── Envahisseurs/
    ├── main.c              ← boucle principale du jeu
    ├── jeu.h               ← constantes et structure du jeu
    ├── jeu.c               ← initialisation et déplacement
    ├── Makefile            ← compilation du projet
    ├── ListeChainee/
    │   ├── entite.h        ← structure Entite
    │   ├── liste.h         ← structure ListeEntites
    │   └── liste.c         ← fonctions de la liste chaînée
    ├── Graphique-2.0/
    │   ├── libgraph.h      ← bibliothèque graphique (interface)
    │   ├── libgraph.c      ← bibliothèque graphique (implémentation)
    │   └── libgraph.a      ← bibliothèque compilée
    └── Lutins/
        ├── invader_monstre1_1.bmp
        ├── invader_monstre1_2.bmp
        ├── invader_monstre2_1.bmp
        ├── invader_monstre2_2.bmp
        ├── invader_monstre3_1.bmp
        ├── invader_monstre3_2.bmp
        ├── invader_canon.bmp
        ├── invader_bouclier.bmp
        └── invader_bombe.bmp