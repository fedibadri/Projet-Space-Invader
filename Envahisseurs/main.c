#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "Graphique-2.0/libgraph.h"
#include "ListeChainee/liste.h"
#include "ListeChainee/entite.h"
#include "jeu.h"

typedef struct {
    int lm1a, lm1b;  
    int lm2a, lm2b;   
    int lm3a, lm3b;   
    int vaisseau;
    int bouclier;
    int bombe;
    int missile;
} Lutins;

void afficherMessage(int lutin_msg) {
    effacerSurface();
    afficherLutin(lutin_msg, LARGEUR/2 - 100, HAUTEUR/2 - 20);
    majSurface();
    pauseIHM(3000);
}

void afficherListe(ListeEntites *liste) {
    Maillon *m = liste->tete;
    while (m != NULL) {
        afficherLutin(m->entite.lutin, m->entite.x, m->entite.y);
        m = m->suivant;
    }
}

void initialiserPartie(Jeu *jeu, int niveau, Lutins *l) {
    initialiserMonstres(jeu, l->lm1a, l->lm1b, l->lm2a, l->lm2b, l->lm3a, l->lm3b);
    initialiserVaisseau(jeu, l->vaisseau);
    initialiserBoucliers(jeu, l->bouclier);
    jeu->lutin_bombe   = l->bombe;
    jeu->lutin_missile = l->missile;

    if (niveau == 1) {
        jeu->vitesse_monstre = 1;
        jeu->intervalle_bombe = 80;
    } else if (niveau == 2) {
        jeu->vitesse_monstre = 2;
        jeu->intervalle_bombe = 50;
    } else {
        jeu->vitesse_monstre = 3;
        jeu->intervalle_bombe = 30;
    }
}

int chargerLutins(Lutins *l) {
    l->lm1a = chargerLutin("Lutins/invader_monstre1_1.bmp", COULEUR_BLANC);
    l->lm1b = chargerLutin("Lutins/invader_monstre1_2.bmp", COULEUR_BLANC);
    l->lm2a = chargerLutin("Lutins/invader_monstre2_1.bmp", COULEUR_BLANC);
    l->lm2b = chargerLutin("Lutins/invader_monstre2_2.bmp", COULEUR_BLANC);
    l->lm3a = chargerLutin("Lutins/invader_monstre3_1.bmp", COULEUR_BLANC);
    l->lm3b = chargerLutin("Lutins/invader_monstre3_2.bmp", COULEUR_BLANC);
    l->vaisseau = chargerLutin("Lutins/invader_canon.bmp", COULEUR_VERT);
    l->bouclier = chargerLutin("Lutins/invader_bouclier.bmp", COULEUR_GRIS);
    l->bombe = chargerLutin("Lutins/invader_bombe.bmp", COULEUR_ROUGE);
    l->missile = chargerLutin("Lutins/invader_missile.bmp", COULEUR_BLANC);

    if (l->lm1a==-1 || l->lm1b==-1 || l->lm2a==-1 || l->lm2b==-1 ||
        l->lm3a==-1 || l->lm3b==-1 || l->vaisseau==-1 ||
        l->bouclier==-1 || l->bombe==-1 || l->missile==-1)
        return 0;
    return 1;
}

void afficherJeu(Jeu *jeu) {
    effacerSurface();
    afficherListe(&jeu->monstres);
    afficherListe(&jeu->boucliers);
    afficherListe(&jeu->bombes);
    afficherListe(&jeu->missiles);
    afficherLutin(jeu->vaisseau.lutin, jeu->vaisseau.x, jeu->vaisseau.y);
    majSurface();
}

// lire les evenements clavier 
void lireClavier(Jeu *jeu, int *continuer, int *rejouer) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *continuer = 0;
            *rejouer = 0;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_q) {
                *continuer = 0;
                *rejouer = 0;
            }
            if (event.key.keysym.sym == SDLK_SPACE)
                tirerMissile(jeu);
        }
    }

    const Uint8 *touches = SDL_GetKeyboardState(NULL);
    if (touches[SDL_SCANCODE_LEFT])  deplacerVaisseau(jeu, -1);
    if (touches[SDL_SCANCODE_RIGHT]) deplacerVaisseau(jeu,  1);
    if (touches[SDL_SCANCODE_SPACE]) tirerMissile(jeu);
}

// deplacement + collisions 
void mettreAJourJeu(Jeu *jeu, int *continuer, int *gagne) {
    deplacerMonstres(jeu);
    animerMonstres(jeu);
    lacherBombe(jeu);
    deplacerBombes(jeu);
    deplacerMissiles(jeu);

    collisionsListeListe(&jeu->missiles, &jeu->monstres);
    collisionsListeListe(&jeu->missiles, &jeu->bombes);
    collisionsListeListe(&jeu->bombes,   &jeu->boucliers);

    if (collisionsVaisseau(jeu)) {
        *continuer = 0;
        *gagne = 0;
    }
    if (jeu->monstres.tete == NULL) {
        *continuer = 0;
        *gagne = 1;
    }
}

void afficherNiveau(int niveau, int niv1, int niv2, int niv3) {
    effacerSurface();
    if (niveau == 1) afficherLutin(niv1, LARGEUR/2 - 100, HAUTEUR/2 - 20);
    if (niveau == 2) afficherLutin(niv2, LARGEUR/2 - 100, HAUTEUR/2 - 20);
    if (niveau == 3) afficherLutin(niv3, LARGEUR/2 - 100, HAUTEUR/2 - 20);
    majSurface();
    pauseIHM(2000);
}

void gererFinPartie(int gagne, int *niveau, int *rejouer,int lutin_gagne, int lutin_perdu) {
    if (gagne) {
        afficherMessage(lutin_gagne);
        if (*niveau < 3)
            (*niveau)++;
        else
            *rejouer = 0;
    } else {
        afficherMessage(lutin_perdu);
        *rejouer = 0;
    }
}

int main(void) {

    srand(time(NULL));

    if (!creerSurface(LARGEUR, HAUTEUR, "Space Invaders")) {
        fprintf(stderr, "Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    Lutins l;
    if (!chargerLutins(&l)) {
        fprintf(stderr, "Erreur chargement lutin\n");
        fermerSurface();
        return 1;
    }

    int lutin_gagne = lutinTexte("VOUS AVEZ GAGNE !", COULEUR_VERT);
    int lutin_perdu = lutinTexte("VOUS AVEZ PERDU !", COULEUR_ROUGE);
    int lutin_niv1 = lutinTexte("NIVEAU 1 - FACILE", COULEUR_BLANC);
    int lutin_niv2 = lutinTexte("NIVEAU 2 - MOYEN", COULEUR_BLANC);
    int lutin_niv3 = lutinTexte("NIVEAU 3 - DIFFICILE", COULEUR_ROUGE);

    int rejouer = 1;
    int niveau = 1;

    while (rejouer) {

        afficherNiveau(niveau, lutin_niv1, lutin_niv2, lutin_niv3);

        Jeu jeu;
        initialiserPartie(&jeu, niveau, &l);

        int continuer = 1;
        int gagne = 0;

        while (continuer) {
            lireClavier(&jeu, &continuer, &rejouer);
            mettreAJourJeu(&jeu, &continuer, &gagne);
            afficherJeu(&jeu);
            pauseIHM(16);
        }

        // Liberer la memoire 
        libererListe(&jeu.monstres);
        libererListe(&jeu.boucliers);
        libererListe(&jeu.bombes);
        libererListe(&jeu.missiles);

        if (rejouer)
            gererFinPartie(gagne, &niveau, &rejouer,lutin_gagne, lutin_perdu);
    }

    nettoyerLutins();
    fermerSurface();
    return 0;
}
