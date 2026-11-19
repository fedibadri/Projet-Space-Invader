#ifndef JEU_H
#define JEU_H

#include "ListeChainee/liste.h"
#include "ListeChainee/entite.h"

#define LARGEUR 1200
#define HAUTEUR 700

#define NB_COLONNES 8
#define NB_LIGNES 3
#define ESPACEMENT_X 100
#define ESPACEMENT_Y 70
#define DEBUT_X 100
#define DEBUT_Y 50

#define NB_BOUCLIERS 4
#define BOUCLIER_Y 560
#define VAISSEAU_Y 620

#define PAS_DESCENTE 20
#define VITESSE_BOMBE 5
#define INTERVALLE_ANIMATION 30
#define VITESSE_VAISSEAU 5
#define VITESSE_MISSILE 8
#define INTERVALLE_TIR 30

typedef struct {
    ListeEntites monstres;
    ListeEntites boucliers;
    ListeEntites bombes;
    ListeEntites missiles;
    Entite vaisseau;
    int direction;
    int lutin_bombe;
    int lutin_missile;
    int compteur_bombe;
    int lutin_m1a, lutin_m1b;
    int lutin_m2a, lutin_m2b;
    int lutin_m3a, lutin_m3b;
    int compteur_anim;
    int anim;
    int compteur_tir;
    int vitesse_monstre;
    int intervalle_bombe;
} Jeu;

void initialiserMonstres(Jeu *jeu, int lm1a, int lm1b, int lm2a, int lm2b, int lm3a, int lm3b);
void initialiserVaisseau(Jeu *jeu, int lutin_vaisseau);
void initialiserBoucliers(Jeu *jeu, int lutin_bouclier);

// Deplacement monstres 
int bordAtteint(Jeu *jeu);
void descendreMonstres(Jeu *jeu);
void deplacerHorizontal(Jeu *jeu);
void deplacerMonstres(Jeu *jeu);
void changerSpriteMonstre(Maillon *m, Jeu *jeu);
void animerMonstres(Jeu *jeu);

// Bombes
int trouverYMax(Jeu *jeu);
int compterMonstresBasse(Jeu *jeu, int y_max);
Maillon* choisirMonstreBas(Jeu *jeu, int y_max, int choix);
void lacherBombe(Jeu *jeu);

// Deplacement projectiles 
Maillon* supprimerMaillon(ListeEntites *liste, Maillon *precedent, Maillon *courant);
void deplacerBombes(Jeu *jeu);
void deplacerMissiles(Jeu *jeu);

// Vaisseau
void deplacerVaisseau(Jeu *jeu, int direction);
void tirerMissile(Jeu *jeu);

// Collisions
int collision(Entite a, Entite b);
int collisionAvecListe(Entite e, ListeEntites *liste);
int collisionsVaisseau(Jeu *jeu);
void collisionsListeListe(ListeEntites *liste1, ListeEntites *liste2);

#endif
