#ifndef MAIN_H
#define MAIN_H

#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

int initSDL(FlagFINDER *flagFinder);
int loadFiles(FlagFINDER *flagFinder);
SDL_Texture* loadTexture(char *path , FlagFINDER *flagFinder);
void liberation(SDL_Window *window , SDL_Renderer *renderer);
void test_input(int *continuer);

#endif // MAIN_H

