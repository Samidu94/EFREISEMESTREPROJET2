#include "CONSTANTE.h"
void cleanup();
void jeu(SDL_Surface *ecran);
void apparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void deplacer_joueur(int carte[HAUTEUR_TAB][LARGEUR_TAB], int direction, SDL_Rect *position);
void disparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB]);


struct joueur
{
    int point;
    int posx;
    int posy;

};

typedef struct joueur joueur;
