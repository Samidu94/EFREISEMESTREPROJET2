#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmod.h>
#include "../jeu.h"
#include "../CONSTANTE.h"



int main(int argc, char* argv[])
{
    srand(time(NULL));          // pour activer la disparition et apparition des frites aléatoirement
    int continuer = 1;
    SDL_Surface *ecran, *menu;          // ce main sert pour l'affichage sdl
    SDL_Event event;
    SDL_Rect position;
    position.x = 0;     // coordonnées en 2D
    position.y = 0;

    SDL_Init(SDL_INIT_VIDEO);       // se charge du systeme d'affichage
    ecran = SDL_SetVideoMode(HAUTEUR_FENETRE + ((MARGE * 2) * TAILLE_BLOC), LARGEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);       // HW_surface sert à ce que la memoire soit chargé dans la memoire video, on lutilise car c plus fast
    menu = IMG_Load("finalmenu.jpg");        // charge l'image mit dans le dossier                           sdl_doublebuf sert à ce que les mouvements du perso soit plus fluide
    SDL_BlitSurface(menu, NULL, ecran, &position);  // sert à blitter des choses dans l'écran
    SDL_Flip(ecran);

        FMOD_SYSTEM *system;
    FMOD_SOUND *musique;
    FMOD_RESULT resultat;
    FMOD_CHANNELGROUP *canal;
    FMOD_BOOL etat;


    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);

    /* On ouvre la musique */
    resultat = FMOD_System_CreateSound(system, "uefa.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);// mets à jour l'écran

    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0, NULL);
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)            //menu
            {
            case SDLK_2:
                exit(EXIT_SUCCESS);         // si on appuie sur 2, le jeu quitte
                break;
            case SDLK_1:        // le jeu commence quand on appuie sur 1
                FMOD_System_GetMasterChannelGroup(system, &canal);      // la FMOD commence une autre musique en interrompant la 1ere
                FMOD_ChannelGroup_GetPaused(canal, &etat);

                if (etat == 1) // Si la chanson est en pause
                    FMOD_ChannelGroup_SetPaused(canal, 0); // On enlève la pause
                else // Sinon, elle est en cours de lecture
                    FMOD_ChannelGroup_SetPaused(canal, 1); // On active la pause
                jeu(ecran);
                break;
            }
        }
    }
    SDL_FreeSurface(menu);      // libere la surface apres execution
    cleanup();





    FMOD_Sound_Release(musique);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
 return EXIT_SUCCESS;
}
