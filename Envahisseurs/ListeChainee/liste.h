#ifndef LISTE_H
#define LISTE_H

#include "entite.h"

/* Un maillon de la liste */
typedef struct Maillon {
    Entite entite;
    struct Maillon *suivant;
} Maillon;

/* La liste chainee */
typedef struct {
    Maillon *tete;
} ListeEntites;

void initialiserListe(ListeEntites *liste);
void ajouterEntite(ListeEntites *liste, Entite e);
void libererListe(ListeEntites *liste);

#endif
