# Space Invaders - Projet Informatique IOT3 TPI

## Compilation et lancement
```bash
make clean && make
DISPLAY=:1 ./envahisseurs
```

## Contrôles
| Touche | Action |
|--------|--------|
| ← flèche gauche | Déplacer le vaisseau à gauche |
| → flèche droite | Déplacer le vaisseau à droite |
| Espace | Tirer un missile |
| Q | Quitter le jeu |

## Modification de la bibliothèque graphique
Dans `Graphique-2.0/libgraph.c`, ligne 60, modification de :
```c
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
```
en :
```c
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
```
**Raison :** `SDL_RENDERER_ACCELERATED` nécessite un accès GPU non disponible sur mon pc personnel. `SDL_RENDERER_SOFTWARE` utilise le CPU et fonctionne dans tous les cas.En recompilant `libgraph.c` avec notre modification, le problème est résolu .

### SDL2 — Gestion des flèches directionnelles
La bibliothèque `libgraph` ne supporte pas les touches spéciales (flèches).
On utilise SDL2 directement pour deux choses :

**1. `SDL_PollEvent`** → lire les événements (croix, touche Q, Espace)

**2. `SDL_GetKeyboardState`** → déplacement fluide avec les flèches.
Tant que la flèche est maintenue, le vaisseau continue de bouger.

**Référence SDL2 :** https://wiki.libsdl.org/SDL3/SDL_PollEvent ** https://wiki.libsdl.org/SDL3/SDL_GetKeyboardState

## Etapes réalisées

### Etape 1 - Affichage, boucle principale, temporisation
- Création de la fenêtre graphique (1200x700)
- Chargement de sprites de monstres
- Boucle principale avec gestion des événements clavier
- Alternance entre deux sprites toutes les 60 frames

### Etape 2 - Structures de données
- Structure Entite : position (x,y) et numéro de lutin
- Liste chaînée : stockage des monstres, bombes et missiles
- Fonctions : initialiserListe, ajouterEntite, libererListe

### Etape 3 - Initialisation du jeu (avec bonus)
- 3 lignes de 8 monstres en haut de l'écran
- 3 types de monstres différents
- Un vaisseau en bas au centre
- 4 boucliers entre les monstres et le vaisseau

### Etape 4 - Déplacement des entités (avec bonus)
- Monstres se déplacent de gauche à droite
- Changement de sens et descente quand ils atteignent un bord
- Animation des monstres : alternance entre deux sprites (bonus)
- Bombes lâchées aléatoirement par un monstre de la ligne du bas (bonus)
- Bombes supprimées quand elles sortent de l'écran

### Etape 5 - Contrôle du vaisseau (avec bonus)
- Déplacement gauche/droite avec les flèches directionnelles
- Déplacement fluide : le vaisseau bouge tant que la touche est maintenue (bonus)
- Tir de missiles avec la touche Espace
- Missiles supprimés quand ils sortent par le haut
- Cadence de tir limitée : 1 missile toutes les 30 frames (bonus)

### Etape 6 - Calcul des collisions (avec bonus)
- Fonction de collision entre deux entités (comparaison des positions)
- Collision missile/monstre : les deux disparaissent
- Collision vaisseau/bombe ou vaisseau/monstre : fin de partie
- Collision missile/bombe : les deux disparaissent (bonus)
- Collision bombe/bouclier : le bouclier disparaît (bonus)

### Etape 7 - Fin de jeu (avec bonus)
- Gagné si tous les monstres sont éliminés
- Perdu si le vaisseau est touché par une bombe ou un monstre
- Affichage du message "VOUS AVEZ GAGNE !" ou "VOUS AVEZ PERDU !" (bonus)
- 3 niveaux de difficulté (bonus) :
  - Niveau 1 : monstres lents, peu de bombes
  - Niveau 2 : monstres moyens, bombes fréquentes
  - Niveau 3 : monstres rapides, bombes très fréquentes
- Passage automatique au niveau suivant si gagné

## Structure du projet