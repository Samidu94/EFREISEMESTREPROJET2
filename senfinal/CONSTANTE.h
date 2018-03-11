#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#define TAILLE_BLOC 30
#define HAUTEUR_TAB 42
#define LARGEUR_TAB 24
#define LARGEUR_FENETRE LARGEUR_TAB * TAILLE_BLOC
#define HAUTEUR_FENETRE HAUTEUR_TAB * TAILLE_BLOC      // on prend un header pour mettre tous les define (c'est-à-dire les variables qu'on souhaite mettre dans le tableau.)
#define MARGE 0
#define IA 1
#define FRITE 2
#define OBSTACLE 5
#define VIDE 4
#define BURGER 3


enum {HAUT, BAS, GAUCHE, DROITE};       // type de déclaration, struct de variable

#endif      //
