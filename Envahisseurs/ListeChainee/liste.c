#include <stdlib.h>
#include "liste.h"
#include "entite.h"  

void initialiserListe(ListeEntites *liste) {
    liste->tete = NULL;
}

void ajouterEntite(ListeEntites *liste, Entite e) {
    Maillon *nouveau = malloc(sizeof(Maillon));
    nouveau->entite = e;
    nouveau->suivant = liste->tete;
    liste->tete = nouveau;
}

void libererListe(ListeEntites *liste) {
    Maillon *m = liste->tete;
    while (m != NULL) {
        Maillon *suivant = m->suivant;
        free(m);
        m = suivant;
    }
    liste->tete = NULL;
}


