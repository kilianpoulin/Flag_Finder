#include "input.h"

void test_input(int *continuer)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))//tant qu'il y a un evenement
    {
        switch (event.type) //de quel type devenement il s'agit
        {
        case SDL_QUIT ://si on appui sur echap
            *continuer = 0;
            break;
        case SDL_KEYDOWN :
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE :
                *continuer = 0;
                break;
                /*case SDLK_UP :
                    labyrinthe->input.haut = 1;
                    break;
                case SDLK_RIGHT :
                    labyrinthe->input.droite = 1;
                    break;
                case SDLK_LEFT :
                    labyrinthe->input.gauche = 1;
                    break;
                case SDLK_DOWN :
                    labyrinthe->input.bas = 1;
                case SDLK_c :
                    labyrinthe->input.c = 1;
                    break;
                case SDLK_q :
                    *continuer = 0;
                    break;
                case SDLK_m :
                    labyrinthe->input.m = 1;
                    break;
                case SDLK_l :
                    labyrinthe->input.l = 1;
                    break;
                case SDLK_r :
                    labyrinthe->input.r = 1;
                    break;
                }
                break;
                case SDL_KEYUP :
                switch(event.key.keysym.sym)
                {
                case SDLK_UP :
                    labyrinthe->input.haut = 0;
                    break;
                case SDLK_RIGHT :
                    labyrinthe->input.droite = 0;
                    break;
                case SDLK_LEFT :
                    labyrinthe->input.gauche = 0;
                    break;
                case SDLK_DOWN :
                    labyrinthe->input.bas = 0;
                    break;
                case SDLK_c :
                    labyrinthe->input.c = 0;
                    break;
                case SDLK_m :
                    labyrinthe->input.m = 0;
                    break;
                case SDLK_l :
                    labyrinthe->input.l = 0;
                    break;
                case SDLK_r :
                    labyrinthe->input.r = 0;
                    break;
                }
                break;
                */
            }
        }
    }
}



