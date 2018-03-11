#include "CONSTANTE.h"
void cleanup();
void jeu(SDL_Surface *ecran);
void apparition_frite(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void move_player(int carte[HAUTEUR_TAB][LARGEUR_TAB], int direction, SDL_Rect *position);       // fonctions utilisées mis dans headers
void disparition_frite(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void apparition_burger(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void disparition_burger(int carte[HAUTEUR_TAB][LARGEUR_TAB]);
void apparition_obstacle(int carte[HAUTEUR_TAB][LARGEUR_TAB]);


struct Player
{
    int point;
    int posx;       // type de variable de joueur afin de définir sa position
    int posy;

};

typedef struct Player Player;
