#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "CONSTANTE.h"
#include "jeu.h"

void cleanup()
{
    IMG_Quit();
    SDL_Quit();
}


void jeu(SDL_Surface *ecran)
{



    joueur joueur;
    joueur.point = 0;

    int carte[HAUTEUR_TAB][LARGEUR_TAB], i, j, continuer = 1, timerApparition1 = 0, timerApparition2 = 0;
    int timerDisparition1 = 0, timerDisparition2 = 0;

    SDL_Surface *ia[4] = {NULL};
    SDL_Surface *iaActuel, *vide, *particule, *obstacle;
    SDL_Event event;
    SDL_Rect positionObjet, positionjoueur, positionscore;

    positionscore.x = 0;
    positionscore.y = 0;


    vide = IMG_Load("vide.png");
    particule = IMG_Load("particule.png");

    ia[HAUT] = IMG_Load("tetris_haut.png");
    ia[BAS] = IMG_Load("tetris_bas.png");
    ia[GAUCHE] = IMG_Load("tetris_gauche.png");
    ia[DROITE] = IMG_Load("tetris_droite.png");

    SDL_SetColorKey(particule, SDL_SRCCOLORKEY, SDL_MapRGB(particule->format, 255, 255, 255));


      SDL_SetColorKey(ia[HAUT], SDL_SRCCOLORKEY, SDL_MapRGB(ia[HAUT]->format, 255, 255, 255));
      SDL_SetColorKey(ia[BAS], SDL_SRCCOLORKEY, SDL_MapRGB(ia[BAS]->format, 255, 255, 255));
      SDL_SetColorKey(ia[GAUCHE], SDL_SRCCOLORKEY, SDL_MapRGB(ia[GAUCHE]->format, 255, 255, 255));
      SDL_SetColorKey(ia[DROITE], SDL_SRCCOLORKEY, SDL_MapRGB(ia[DROITE]->format, 255, 255, 255));



    for(i = 0; i < HAUTEUR_TAB ; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            carte[i][j] = VIDE;
        }
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    carte[5][3] = IA;
    iaActuel = ia[BAS];



    for(i = 0; i < HAUTEUR_TAB; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            if(carte[i][j] == IA)
            {
                positionjoueur.x = i;
                positionjoueur.y = j;
            }
        }
    }

    apparition_particule(carte);


    while(continuer)
    {
        SDL_EnableKeyRepeat(1000, 500);
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            cleanup();
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    exit(EXIT_SUCCESS);
                    break;
                case SDLK_UP:
                    if(carte[positionjoueur.x][positionjoueur.y - 1] == PARTICULE)
                    {
                        joueur.point++;
                    }
                    iaActuel = ia[HAUT];
                    deplacer_joueur(carte, HAUT, &positionjoueur);
                    break;
                case SDLK_DOWN:
                    if(carte[positionjoueur.x][positionjoueur.y + 1] == PARTICULE)
                    {
                        joueur.point++;
                    }
                    iaActuel = ia[BAS];
                    deplacer_joueur(carte, BAS,&positionjoueur);
                    break;
                case SDLK_LEFT:
                    if(carte[positionjoueur.x - 1][positionjoueur.y] == PARTICULE)
                    {
                        joueur.point++;
                    }
                    iaActuel = ia[GAUCHE];
                    deplacer_joueur(carte, GAUCHE, &positionjoueur);
                    break;
                case SDLK_RIGHT:
                    if(carte[positionjoueur.x + 1][positionjoueur.y] == PARTICULE)
                    {
                        joueur.point++;
                    }
                    iaActuel = ia[DROITE];
                    deplacer_joueur(carte, DROITE, &positionjoueur);
                    break;
            }
            break;
        }

    timerApparition1 = SDL_GetTicks();
    if(timerApparition1 - timerApparition2 > 5000)
    {
        apparition_particule(carte);
        timerApparition2 = timerApparition1;
    }

    timerDisparition1 = SDL_GetTicks();
    if(timerDisparition1 - timerDisparition2 > 5000)
    {
        disparition_particule(carte);
        timerDisparition2 = timerDisparition1;
    }

    for(i = 0; i < HAUTEUR_TAB; i++)
    {
        for(j = 0; j < LARGEUR_TAB; j++)
        {
            positionObjet.x = i * TAILLE_BLOC + (MARGE * TAILLE_BLOC);
            positionObjet.y = j * TAILLE_BLOC;
            switch(carte[i][j])
            {
            case VIDE:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);
                break;
            case OBSTACLE:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(obstacle, NULL, ecran, &positionObjet);
                break;
            case PARTICULE:
                SDL_BlitSurface(vide, NULL, ecran, &positionObjet);
                SDL_BlitSurface(particule, NULL, ecran, &positionObjet);
                break;
            case IA:
                positionObjet.x = positionjoueur.x * TAILLE_BLOC + (MARGE * TAILLE_BLOC);
                positionObjet.y = positionjoueur.y * TAILLE_BLOC;
                SDL_BlitSurface(iaActuel, NULL, ecran, &positionObjet);
                break;
            }
        }
    }
SDL_Flip(ecran);

    }


    SDL_FreeSurface(iaActuel);
    SDL_FreeSurface(obstacle);
    SDL_FreeSurface(particule);

    for(i = 0; i < 4; i++)
    {
        SDL_FreeSurface(ia[i]);
    }
}


void apparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int nbParticule, posParticule_x, posParticule_y, i, j, k, go = 1;
    nbParticule = rand() % 100 ;
    for(i = 0; i < nbParticule; i++)
    {
       posParticule_x = rand() % HAUTEUR_TAB;
       posParticule_y = rand() % LARGEUR_TAB;
       while(carte[posParticule_x][posParticule_y] == TEMPETE)
       {
           posParticule_x = rand() % HAUTEUR_TAB;
           posParticule_y = rand() % LARGEUR_TAB;
       }
       for(j = 0; j < HAUTEUR_TAB; j++)
       {
           for (k = 0; k < LARGEUR_TAB; k++)
           {

               if(((j == posParticule_x) && (k == posParticule_y)))
               {
                   carte[j][k] = PARTICULE;
                   j = HAUTEUR_TAB;
                   k = LARGEUR_TAB;
               }
           }
       }
    }
}

void deplacer_joueur(int carte[HAUTEUR_TAB][LARGEUR_TAB], int direction, SDL_Rect *position)
{

    switch(direction)
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
        }
        carte[position->x][position->y] = VIDE;
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

void disparition_particule(int carte[HAUTEUR_TAB][LARGEUR_TAB])
{
    int i, j, k,  posx, posy, nbparticule;
    nbparticule = rand() % 100;
    for(i = 0; i < nbparticule; i++)
    {
       posx = rand() % HAUTEUR_TAB;
       posy = rand() % LARGEUR_TAB;
       while(carte[posx][posy] == TEMPETE)
       {
           posx = rand() % HAUTEUR_TAB;
           posy = rand() % LARGEUR_TAB;
       }
        for(j = posx; j < HAUTEUR_TAB; j++)
        {
            for(k = posy; k < LARGEUR_TAB; k++)
            {
                if(carte[j][k] == PARTICULE)
                {
                    carte[j][k] = VIDE;
                    j = HAUTEUR_TAB;
                    k = LARGEUR_TAB;
                }
            }
        }
    }
}


