#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "CONSTANTE.h"              // on inclut les dofférents headers afin qu'on les utilise dans le.c
#include "jeu.h"
#include <fmod.h>
#include <sdl/sdl_ttf.h>



void cleanup()
{
    IMG_Quit();
    SDL_Quit();
}


void jeu(SDL_Surface *ecran)
{



    Player Player;      // joueur est un type de variable grace à struct.
    Player.point = 0;

    int carte[HAUTEUR_TAB][LARGEUR_TAB], i, j, continuer = 1, timerApparition1 = 0, timerApparition2 = 0;       // voir constante
    int timerDisparition1 = 0, timerDisparition2 = 0, timer = 0;

    SDL_Surface *ia[4] = {NULL};
    SDL_Surface *iaActuel, *vide, *frite, *burger, *obstacle ;            // pointeurs pour surface de la fenêtre
    SDL_Event event;
    SDL_Rect positionObjet, positionjoueur, positionscore;      // SDL_Surface sert de base pour l'affichage de la surface

    positionscore.x = 0;
    positionscore.y = 0;


    vide = IMG_Load("vide.png");
    frite = IMG_Load("frite.png");      // img_load sert à charger l'image à partir de la variable
    burger = IMG_Load("burger.png");
    obstacle = IMG_Load("mur.png");
    ia[HAUT] = IMG_Load("tetris_haut.png");
    ia[BAS] = IMG_Load("tetris_bas.png");
    ia[GAUCHE] = IMG_Load("tetris_gauche.png");
    ia[DROITE] = IMG_Load("tetris_droite.png");     // TETRIS = SONIC dans les différentes positions

    SDL_SetColorKey(frite, SDL_SRCCOLORKEY, SDL_MapRGB(frite->format, 255, 255, 255));      // sdl_colorkey sert à faire la transparence pr les frites et sonic

    SDL_SetColorKey(burger, SDL_SRCCOLORKEY, SDL_MapRGB(ia[HAUT]->format, 255, 255, 255));
    SDL_SetColorKey(obstacle, SDL_SRCCOLORKEY, SDL_MapRGB(ia[HAUT]->format, 255, 255, 255));
      SDL_SetColorKey(ia[HAUT], SDL_SRCCOLORKEY, SDL_MapRGB(ia[HAUT]->format, 255, 255, 255));
      SDL_SetColorKey(ia[BAS], SDL_SRCCOLORKEY, SDL_MapRGB(ia[BAS]->format, 255, 255, 255));
      SDL_SetColorKey(ia[GAUCHE], SDL_SRCCOLORKEY, SDL_MapRGB(ia[GAUCHE]->format, 255, 255, 255));
      SDL_SetColorKey(ia[DROITE], SDL_SRCCOLORKEY, SDL_MapRGB(ia[DROITE]->format, 255, 255, 255));



    for(i = 0; i < HAUTEUR_TAB ; i++)               // initialisation du tableau 2D
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            carte[i][j] = VIDE;     // VIDE = l'affichage en noir pour que Sonic se déplace
        }
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));        // on remplit le tableau et le colore
    carte[5][3] = IA;
    iaActuel = ia[BAS];     // quand il "spawn" (apparaitre) il apparait de bas



    for(i = 0; i < HAUTEUR_TAB; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            if(carte[i][j] == IA)               // il servira plus tard: cad sauvegarder la position du joueur grace aux entiers itérateurs
            {
                positionjoueur.x = i;
                positionjoueur.y = j;       // sauvegarder la position du joueur en deux positions: abscisse et ordonnée
            }
        }
    }

    apparition_frite(carte);
    apparition_burger(carte);
    apparition_obstacle(carte);        // les frites apparaissent aléatoirement sur la carte

                  FMOD_SYSTEM *system;
    FMOD_SOUND *musique;
    FMOD_RESULT resultat;
    FMOD_CHANNELGROUP *canal;       //FMOD nous sert à mettre les différentes musiques utilisés au menu, et dans le gameplay.
    FMOD_BOOL etat;


    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);        // Init nous initialise le début de FMOD
    resultat = FMOD_System_CreateSound(system, "emile.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);// mets à jour l'écran

    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0, NULL);


    while(continuer)        // continuer est la variable nous servant à continuer l'évènement
    {
        SDL_EnableKeyRepeat(1000, 500);     // permet au joueur d'avancer plusieurs fois en touchznt le bouton longtemps
        SDL_WaitEvent(&event);              // meme chose que wait_event juste qu'elle dit si un évènement s'est produit ou non
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)    // savoir quel touche est enfoncé
            {
                case SDLK_ESCAPE:
                    exit(EXIT_SUCCESS);
                    break;                  // exit du jeu (echap)
                case SDLK_UP:
                    if(carte[positionjoueur.x][positionjoueur.y - 1] == OBSTACLE)
                    {
                        break;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x][positionjoueur.y - 1] == FRITE)
                    {
                        Player.point++;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x][positionjoueur.y - 1] == BURGER)
                    {
                        Player.point = Player.point + 2;     // si il zavance sur la frite, il la mange
                    }
                    iaActuel = ia[HAUT];
                    move_player(carte, HAUT, &positionjoueur);
                    break;
                case SDLK_DOWN:
                   if(carte[positionjoueur.x][positionjoueur.y + 1] == OBSTACLE)
                    {
                        break;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x][positionjoueur.y + 1] == FRITE)
                    {
                        Player.point++;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x][positionjoueur.y + 1] == BURGER)
                    {
                        Player.point = Player.point + 2;    // si il zavance sur la frite, il la mange
                    }
                    iaActuel = ia[BAS];
                    move_player(carte, BAS,&positionjoueur);        // on prend la position bas car le joueur s'incline vers le bas et on le pointe grace au pointeur de sa position dans sa carte
                    break;
                case SDLK_LEFT:
                    if(carte[positionjoueur.x-1][positionjoueur.y] == OBSTACLE)
                    {
                        break;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x-1][positionjoueur.y] == FRITE)
                    {
                        Player.point++;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x-1][positionjoueur.y] == BURGER)
                    {
                        Player.point = Player.point + 2;     // si il zavance sur la frite, il la mange
                    }
                    iaActuel = ia[GAUCHE];
                    move_player(carte, GAUCHE, &positionjoueur);        // move_player sert à déplacer le joueur vers la position, sa position est indiqué grâce au pointeur de la variable positionjoueur
                    break;
                case SDLK_RIGHT:
                   if(carte[positionjoueur.x+1][positionjoueur.y] == OBSTACLE)
                    {
                        break;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x+1][positionjoueur.y] == FRITE)
                    {
                        Player.point++;     // si il zavance sur la frite, il la mange
                    }
                    if(carte[positionjoueur.x+1][positionjoueur.y] == BURGER)
                    {
                        Player.point = Player.point + 2;    // si il zavance sur la frite, il la mange
                    }
                    iaActuel = ia[DROITE];
                    move_player(carte, DROITE, &positionjoueur);
                    break;
            }
            break;

        }

    timerApparition1 = SDL_GetTicks();          // retourne le nombre de milliseconcdes écoulées depuis le lancement
    if(timerApparition1 - timerApparition2 > 5000)
    {
        apparition_frite(carte);
        apparition_burger(carte);                // permet l'apparition de nouvelles frites
        timerApparition2 = timerApparition1;
    }
    timer = SDL_GetTicks();
    if(timer > 240000)
        exit(0);

    timerDisparition1 = SDL_GetTicks();
    if(timerDisparition1 - timerDisparition2 > 5000)
    {
        disparition_frite(carte);
        disparition_burger(carte);
        timerDisparition2 = timerDisparition1;
    }

    for(i = 0; i < HAUTEUR_TAB; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            positionObjet.x = i * TAILLE_BLOC + (MARGE * TAILLE_BLOC);
            positionObjet.y = j * TAILLE_BLOC;
            switch(carte[i][j])                                             //marge va servir à noter résultat
            {
            case VIDE:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);     //"blitter" pour remplir la surface
                break;
            case BURGER:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);     //"blitter" pour remplir la surface
                SDL_BlitSurface(burger, NULL, ecran, &positionObjet);     //"blitter" pour remplir la surface
                break;
            case OBSTACLE:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);     //"blitter" pour remplir la surface
                SDL_BlitSurface(obstacle, NULL, ecran, &positionObjet);     //"blitter" pour remplir la surface
                break;
            case FRITE:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);     //SDL BlitzSurface remplit le VIDE de frite et de sa position
                SDL_BlitSurface(frite, NULL, ecran, &positionObjet);
                break;
            case IA:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(iaActuel, NULL, ecran, &positionObjet);
                break;
            }
        }
    }
SDL_Flip(ecran);

    }


    SDL_FreeSurface(iaActuel);      // on libère la surface sur les différentes variables
    SDL_FreeSurface(burger);
    SDL_FreeSurface(obstacle);
    SDL_FreeSurface(frite);

    for(i = 0; i < 4; i++)
    {
        SDL_FreeSurface(ia[i]);     // on libere la surface prise par l'IA pour pas avoir des données en masse
    }
}


void apparition_frite(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int nbfrite, posfrite_x, posfrite_y, i, j, k, go = 1;
    nbfrite = rand() % 100 ;        // apparition hasardeuse grace a rand, et il peut y avoir 100 frites qui peuvent aapparaitre au total
    for(i = 0; i < nbfrite; i++)
    {
       posfrite_x = rand() % HAUTEUR_TAB;       // position des frites aux hasards en x et y
       posfrite_y = rand() % LARGEUR_TAB;

       for(j = 0; j < HAUTEUR_TAB; j++)
       {
           for (k = 0; k < LARGEUR_TAB; k++)
           {

               if(((j == posfrite_x) && (k == posfrite_y)))
               {
                   carte[j][k] = FRITE;
                   j = HAUTEUR_TAB;
                   k = LARGEUR_TAB;
               }
           }
       }
    }
}




void apparition_burger(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int nbburger, posburger_x, posburger_y, i, j, k, go = 1;
    nbburger = rand() % 50 ;
    for(i = 0; i < nbburger; i++)
    {
       posburger_x = rand() % HAUTEUR_TAB;
       posburger_y = rand() % LARGEUR_TAB;

       for(j = 0; j < HAUTEUR_TAB; j++)
       {
           for (k = 0; k < LARGEUR_TAB; k++)
           {

               if(((j == posburger_x) && (k == posburger_y)))
               {
                   carte[j][k] = BURGER;
                   j = HAUTEUR_TAB;
                   k = LARGEUR_TAB;
               }
           }
       }
    }
}

void move_player(int carte[HAUTEUR_TAB][LARGEUR_TAB], int direction, SDL_Rect *position)    // fonction servant à déplacer le joueur
{

    switch(direction)           // variable princiaple permettant de bouger le joueur: la direction dépend de deux paramtres: x et y.
    {
    case HAUT:
        if((position->y - 1) < 0)
        {
            break;
        }
        carte[position->x][position->y] = VIDE;
        carte[position->x][position->y - 1] = IA;
        position->y--;
        break;
    case BAS:
        if((position->y + 1) > LARGEUR_TAB - 1)
        {
            break;
        }                                           // différents cas possibles pour la position que l'IA prend
        carte[position->x][position->y] = VIDE; // cela grâce à enum: cette structure permet d'avoir les 4 types de déplacement possibles
        carte[position->x][position->y + 1] = IA;
        position->y++;
        break;
    case GAUCHE:
        if((position->x - 1) < 0)
        {
            break;
        }
        carte[position->x][position->y] = VIDE;
        carte[position->x - 1][position->y] = IA;
        position->x--;
        break;
    case DROITE:
        if((position->x + 1) > HAUTEUR_TAB - 1)
        {
            break;
        }
        carte[position->x + 1][position->y] = IA;
        carte[position->x][position->y] = VIDE;
        position->x++;
        break;
    }
}

void disparition_frite(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int i, j, k,  posx, posy, nbfrite;
    nbfrite = rand() % 100;
    for(i = 0; i < nbfrite; i++)
    {
       posx = rand() % HAUTEUR_TAB;         // la disparition de frite se joue également au hasard:
                                            // elles disparaissent au hasard et réapparaissent dans le tableau
                                            // cela grace aux positions x et y, et des HAUTEUR ET LARGEUR du tableau 2D
       posy = rand() % LARGEUR_TAB;

        for(j = posx; j < HAUTEUR_TAB; j++)
        {
            for(k = posy; k < LARGEUR_TAB; k++)
            {
                if(carte[j][k] == FRITE)
                {
                    carte[j][k] = VIDE;
                    j = HAUTEUR_TAB;
                    k = LARGEUR_TAB;
                }
            }
        }
    }
}





void disparition_burger(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int i, j, k,  posx, posy, nbburger;
    nbburger = rand() % 50;
    for(i = 0; i < nbburger; i++)
    {
       posx = rand() % HAUTEUR_TAB;
       posy = rand() % LARGEUR_TAB;

        for(j = posx; j < HAUTEUR_TAB; j++)
        {
            for(k = posy; k < LARGEUR_TAB; k++)
            {
                if(carte[j][k] == BURGER)
                {
                    carte[j][k] = VIDE;
                    j = HAUTEUR_TAB;
                    k = LARGEUR_TAB;
                }
            }
        }
    }
}

void apparition_obstacle(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int nbobstacle, posobstacle_x, posobstacle_y, i, j, k, go = 1;
    nbobstacle = (rand() % 15) + 30 ;
    for(i = 0; i < nbobstacle; i++)
    {
       posobstacle_x = rand() % HAUTEUR_TAB;
       posobstacle_y = rand() % LARGEUR_TAB;

       for(j = 0; j < HAUTEUR_TAB; j++)
       {
           for (k = 0; k < LARGEUR_TAB; k++)
           {

               if(((j == posobstacle_x) && (k == posobstacle_y)))
               {
                   carte[j][k] = OBSTACLE;
                   j = HAUTEUR_TAB;
                   k = LARGEUR_TAB;
               }
           }
       }
    }
}
