#include <stdlib.h>
#include "jeu.h"

void initialiserMonstres(Jeu *jeu, int lm1a, int lm1b, int lm2a, int lm2b, int lm3a, int lm3b) {
    int i, j;
    Entite e;

    initialiserListe(&jeu->monstres);
    initialiserListe(&jeu->bombes);
    initialiserListe(&jeu->missiles);
    jeu->direction = 1;
    jeu->compteur_bombe = 0;
    jeu->compteur_anim = 0;
    jeu->anim = 0;
    jeu->compteur_tir = 0;
    jeu->lutin_m1a = lm1a; jeu->lutin_m1b = lm1b;
    jeu->lutin_m2a = lm2a; jeu->lutin_m2b = lm2b;
    jeu->lutin_m3a = lm3a; jeu->lutin_m3b = lm3b;

    for (j = 0; j < NB_LIGNES; j++) {
        for (i = 0; i < NB_COLONNES; i++) {
            e.x = DEBUT_X + i * ESPACEMENT_X;
            e.y = DEBUT_Y + j * ESPACEMENT_Y;
            if (j == 0) e.lutin = lm1a;
            else if (j == 1) e.lutin = lm2a;
            else e.lutin = lm3a;
            ajouterEntite(&jeu->monstres, e);
        }
    }
}

void initialiserVaisseau(Jeu *jeu, int lutin_vaisseau) {
    jeu->vaisseau.x = LARGEUR / 2;
    jeu->vaisseau.y = VAISSEAU_Y;
    jeu->vaisseau.lutin = lutin_vaisseau;
}

void initialiserBoucliers(Jeu *jeu, int lutin_bouclier) {
    int i;
    Entite e;

    initialiserListe(&jeu->boucliers);
    for (i = 0; i < NB_BOUCLIERS; i++) {
        e.x = 280 + i * 220;
        e.y = BOUCLIER_Y;
        e.lutin = lutin_bouclier;
        ajouterEntite(&jeu->boucliers, e);
    }
}

// Verifier si un monstre touche le bord 
int bordAtteint(Jeu *jeu) {
    Maillon *m = jeu->monstres.tete;
    while (m != NULL) {
        if (m->entite.x + jeu->vitesse_monstre * jeu->direction > LARGEUR - 50)
            return 1;
        if (m->entite.x + jeu->vitesse_monstre * jeu->direction < 10)
            return 1;
        m = m->suivant;
    }
    return 0;
}

void descendreMonstres(Jeu *jeu) {
    Maillon *m = jeu->monstres.tete;
    while (m != NULL) {
        m->entite.y += PAS_DESCENTE;
        m = m->suivant;
    }
}

void deplacerHorizontal(Jeu *jeu) {
    Maillon *m = jeu->monstres.tete;
    while (m != NULL) {
        m->entite.x += jeu->vitesse_monstre * jeu->direction;
        m = m->suivant;
    }
}

void deplacerMonstres(Jeu *jeu) {
    if (bordAtteint(jeu)) {
        jeu->direction = -jeu->direction;
        descendreMonstres(jeu);
    } else {
        deplacerHorizontal(jeu);
    }
}

void changerSpriteMonstre(Maillon *m, Jeu *jeu) {
    if (m->entite.lutin == jeu->lutin_m1a || m->entite.lutin == jeu->lutin_m1b)
        m->entite.lutin = (jeu->anim == 0) ? jeu->lutin_m1a : jeu->lutin_m1b;
    else if (m->entite.lutin == jeu->lutin_m2a || m->entite.lutin == jeu->lutin_m2b)
        m->entite.lutin = (jeu->anim == 0) ? jeu->lutin_m2a : jeu->lutin_m2b;
    else
        m->entite.lutin = (jeu->anim == 0) ? jeu->lutin_m3a : jeu->lutin_m3b;
}

void animerMonstres(Jeu *jeu) {
    Maillon *m;

    jeu->compteur_anim++;
    if (jeu->compteur_anim < INTERVALLE_ANIMATION) return;
    jeu->compteur_anim = 0;
    jeu->anim = 1 - jeu->anim;

    m = jeu->monstres.tete;
    while (m != NULL) {
        changerSpriteMonstre(m, jeu);
        m = m->suivant;
    }
}
 
int trouverYMax(Jeu *jeu) {
    int y_max = -1;
    Maillon *m = jeu->monstres.tete;
    while (m != NULL) {
        if (m->entite.y > y_max)
            y_max = m->entite.y;
        m = m->suivant;
    }
    return y_max;
}

int compterMonstresBasse(Jeu *jeu, int y_max) {
    int nb = 0;
    Maillon *m = jeu->monstres.tete;
    while (m != NULL) {
        if (m->entite.y == y_max) nb++;
        m = m->suivant;
    }
    return nb;
}

// Choisir un monstre aleatoire sur la ligne du bas
Maillon* choisirMonstreBas(Jeu *jeu, int y_max, int choix) {
    int i = 0;
    Maillon *m = jeu->monstres.tete;
    while (m != NULL) {
        if (m->entite.y == y_max) {
            if (i == choix) return m;
            i++;
        }
        m = m->suivant;
    }
    return NULL;
}

void lacherBombe(Jeu *jeu) {
    Entite bombe;
    int y_max, nb_bas;
    Maillon *m;

    jeu->compteur_bombe++;
    if (jeu->compteur_bombe < jeu->intervalle_bombe) return;
    jeu->compteur_bombe = 0;
    if (jeu->monstres.tete == NULL) return;

    y_max  = trouverYMax(jeu);
    nb_bas = compterMonstresBasse(jeu, y_max);
    m      = choisirMonstreBas(jeu, y_max, rand() % nb_bas);

    if (m != NULL) {
        bombe.x = m->entite.x + 10;
        bombe.y = m->entite.y + 30;
        bombe.lutin = jeu->lutin_bombe;
        ajouterEntite(&jeu->bombes, bombe);
    }
}
 
Maillon* supprimerMaillon(ListeEntites *liste, Maillon *precedent, Maillon *courant) {
    Maillon *suivant = courant->suivant;
    if (precedent == NULL) //au debut de la liste
        liste->tete = suivant;
    else
        precedent->suivant = suivant;
    free(courant);
    return suivant;
}

void deplacerBombes(Jeu *jeu) {
    Maillon *m = jeu->bombes.tete;
    Maillon *precedent = NULL;

    while (m != NULL) {
      m->entite.y += VITESSE_BOMBE; //vers bas
        if (m->entite.y > HAUTEUR)
            m = supprimerMaillon(&jeu->bombes, precedent, m);
        else {
            precedent = m;
            m = m->suivant;
        }
    }
}

void deplacerMissiles(Jeu *jeu) {
    Maillon *m = jeu->missiles.tete;
    Maillon *precedent = NULL;

    while (m != NULL) {
      m->entite.y -= VITESSE_MISSILE; //vers haut
        if (m->entite.y < 0)
            m = supprimerMaillon(&jeu->missiles, precedent, m);
        else {
            precedent = m;
            m = m->suivant;
        }
    }
}

void deplacerVaisseau(Jeu *jeu, int direction) {
    jeu->vaisseau.x += VITESSE_VAISSEAU * direction;
    if (jeu->vaisseau.x < 0)
        jeu->vaisseau.x = 0;
    if (jeu->vaisseau.x > LARGEUR - 50)
        jeu->vaisseau.x = LARGEUR - 50;
}

void tirerMissile(Jeu *jeu) {
    Entite missile;

    jeu->compteur_tir++;
    if (jeu->compteur_tir < INTERVALLE_TIR) return;
    jeu->compteur_tir = 0;

    missile.x = jeu->vaisseau.x + 15;
    missile.y = jeu->vaisseau.y - 10;
    missile.lutin = jeu->lutin_missile;
    ajouterEntite(&jeu->missiles, missile);
}
//40*40 pixels
int collision(Entite a, Entite b) {
    if (a.x + 40 < b.x) return 0;
    if (b.x + 40 < a.x) return 0;
    if (a.y + 40 < b.y) return 0;
    if (b.y + 40 < a.y) return 0;
    return 1;
}

int collisionAvecListe(Entite e, ListeEntites *liste) {
    Maillon *m = liste->tete;
    while (m != NULL) {
        if (collision(e, m->entite)) return 1;
        m = m->suivant;
    }
    return 0;
}

int collisionsVaisseau(Jeu *jeu) {
    if (collisionAvecListe(jeu->vaisseau, &jeu->bombes))   return 1;
    if (collisionAvecListe(jeu->vaisseau, &jeu->monstres)) return 1;
    return 0;
}

void collisionsListeListe(ListeEntites *liste1, ListeEntites *liste2) {
    Maillon *m1 = liste1->tete;
    Maillon *p1 = NULL;

    while (m1 != NULL) {
        Maillon *m2 = liste2->tete;
        Maillon *p2 = NULL;
        int touche = 0;

        while (m2 != NULL) {
            if (collision(m1->entite, m2->entite)) {
                supprimerMaillon(liste2, p2, m2);
                touche = 1;
                break;
            }
            p2 = m2;
            m2 = m2->suivant;
        }

        if (touche)
            m1 = supprimerMaillon(liste1, p1, m1);
        else {
            p1 = m1;
            m1 = m1->suivant;
        }
    }
}
