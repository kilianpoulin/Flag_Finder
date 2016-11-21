#include "main.h"
#define X_MAX 225
#define Y_MAX 150


int initSDL(FlagFINDER *flagFinder)
{
    int success = 1;

    if(SDL_Init(SDL_INIT_EVERYTHING) == -1) //Si on n'a pas pu initialiser la SDL
    {
        printf("Erreur avec l'initialisation de la SDL : %s\n" , SDL_GetError());
        success = 0;
    }
    else
    {
        flagFinder->window = SDL_CreateWindow("Flag Finder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  640, 480, SDL_WINDOW_OPENGL); // 640, 480 : taille de la fenêtre
        if(flagFinder->window == NULL)
        {
            printf("Erreur window : %s\n", SDL_GetError());
            success = 0;
        }
        else
        {
            flagFinder->renderer = SDL_CreateRenderer(flagFinder->window, -1 , SDL_RENDERER_ACCELERATED);/*On créer la zone d'affichage*/

            if(flagFinder->renderer == NULL)
            {
                printf("Erreur Renderer : %s\n" , SDL_GetError());
                success = 0;
            }

            else
            {
                if(TTF_Init() == -1)/*Si on ne peut pas initialiser TTF*/
                {
                    printf("Erreur initialisation TTF : %s\n" , TTF_GetError());
                    success = 0;
                }
            }
        }
    }

    return success;
}

int loadFiles(FlagFINDER *flagFinder)
{
    flagFinder->drapeauFrance.texture = loadTexture("10.png" , flagFinder);/*On charge l'image du drapeau de la France*/
    if(flagFinder->drapeauFrance.texture == NULL)
        return 0;
    return 1;
}

SDL_Texture* loadTexture(char *path , FlagFINDER *flagFinder)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        /*Convertit la surface au "pixel format" de l'écran (=résolution)*/
        SDL_Surface* formattedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(flagFinder->window)->format, NULL);

        if( formattedSurface == NULL )
        {
            printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            /*On créer une texture vide pour pouvoir y accéder avec SDL_TEXTUREACCESS_TARGET / SDL_TEXTUREACCESS_STREAMING */
            newTexture = SDL_CreateTexture( flagFinder->renderer, SDL_GetWindowPixelFormat(flagFinder->window), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
            if( newTexture == NULL )
            {
                printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                /*On copie les pixels de la surface dans la texture*/
                 SDL_LockTexture(newTexture, NULL, &flagFinder->drapeauFrance.mPixels, &flagFinder->drapeauFrance.mPitch);/*Pour copier, il faut verouiller la texture*/

                memcpy(flagFinder->drapeauFrance.mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);/*Pour copier*/

                //On met à jour en déverouillant
                SDL_UnlockTexture(newTexture);
            }

            SDL_FreeSurface(formattedSurface);//On supprime l'ancienne image formaté
        }

        //On supprime l'ancienne image
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void liberation(SDL_Window *window , SDL_Renderer *renderer)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
}
int hexa(int dec)
{
    if(dec == 0)
        return 'G';
    else if(dec < 9)
        return dec;
    else if(dec == 10)
        return 'A';
    else if(dec == 11)
        return 'B';
    else if(dec == 12)
        return 'C';
    else if(dec == 13)
        return 'D';
    else if(dec == 14)
        return 'E';
    else
        return 'F';
}

char* dectohexa(int dec)
{
    char *tab = malloc(sizeof(char));
    int dernierTour = 0;
    int i = 0;
    while(dec / 16 != 0 || dernierTour == 1)
    {
        tab = realloc(tab, (i + 1) * sizeof(char));

        tab[i] = hexa(dec % 16);

        dec = dec / 16;
        if(dec < 16)
        {
            if(dernierTour == 0)
                dernierTour = 1;
            else if(dernierTour == 1)
               dernierTour = 0;
        }
        i++;
    }
    tab[i] = '\0';


    return tab;
}
char* retourne(char *tab)
{
    char *tab2 = malloc(6 * sizeof(char));
    int i = 0, j, z = 0;
    /// do while à l'origine
    if(tab[i] == '\0')
        i = 0;
    else
    {
        do
        {
            i++;
        }
        while(tab[i] != '\0');
    }

    for(j = 0; j < 6 - i; j++)
    {
        tab2[j] = 0;
    }
    z = i - 1;
    for(j = 6 - i; j < 6; j++)
    {
        if(tab[z] == 'G')
            tab2[j] = 0;

        else
            tab2[j] = tab[z];
        z--;
    }

    return tab2;
}

int stripes_v(Pixel *pixel)
{
    if((pixel->succ)->y == pixel->y)
        return 1 + stripes_v(pixel->succ);
    else
        return 0;
}
int stripes_h(Pixel *pixel)
{
    if((pixel->succ)->x == pixel->x)
        return 1 + stripes_h(pixel->succ);
    else
        return 0;
}
/*
int cross(Pixel *pixel)
{
    if(pixel->succ)
}*/

int rvb(char hexa[6], char col[1])
{
    int code;
    int i = 0, z = 0;

    char color[2];

    if(col == "B")
        z = 4;
    else if(col == "V")
        z = 2;
    else
        z = 0;

    for(i = 0; i < 2; i++)
    {
         color[i] = hexa[i + z];

        switch(hexa[i + z])
        {
            case 'A' :
                color[i] = 10;
            break;
            case 'B' :
                color[i] = 11;
            break;
            case 'C' :
                color[i] = 12;
            break;
            case 'D' :
                color[i] = 13;
            break;
            case 'E' :
                color[i] = 14;
            break;
            case 'F' :
                color[i] = 15;
             break;
            case 'G' :
                color[i] = 0;
            break;
        }

    }

    code = ((color[0]*16) + color[1]);

    return code;
}

int croix(Pixel *test, int pixel[X_MAX * Y_MAX])
{
    char *tab = NULL;
    int dec, tmp, i, gap = 0;
    if(test->x == 0 || test->y == 0)
        test = test->succ;

    printf("(X = %d, Y = %d) \n", test->x, test->y);
        /// On recule d'un pixel en longueur
        tmp = (test->x + test->y * 225) - 1;
        dec = pixel[(test->x + test->y * 225) - 1];

            printf("dec = %d --> %d\n\n", dec, test->dec);


        /// On cherche si une bande horizontale vient couper cette bande verticale
        while(tmp < X_MAX * Y_MAX && test->dec != dec && gap != 0)
        {
            tmp = tmp + 225;
            dec = pixel[tmp];
            if(test->dec != dec)
                gap = 1;
        }
        /// S'il a trouuvé une bande horizontale,
        if(tmp < X_MAX * Y_MAX - 225)
        {
            printf("tmp = %d / %d", tmp, X_MAX*Y_MAX - 225);
            /// on recule d'un pixel en hauteur
            tmp = tmp - 225;
            dec = pixel[tmp];

            /// On recherche la bande verticale du départ, cette fois ci au contact de notre bande horizontale
            while(tmp % 225 != 0 && test->dec != dec)
            {
                tmp = tmp + 1;
                dec = pixel[tmp];
            }

            /// On a retrouvé la bande verticale, il y a une croix
            if(tmp % 225 != 0)
                return 1;
        }
    return 0;
}

int main(int argc, char *argv[])
{
    FlagFINDER flagFinder;/*Contient toutes les variables nécessaires*/
    flagFinder.continuer = 1;
    flagFinder.prochainRendu = 0;

    Uint32 *pixels;

    if(initSDL(&flagFinder))
    {
        if(loadFiles(&flagFinder))
        {
            pixels = (Uint32*)flagFinder.drapeauFrance.mPixels;


            char *tab = NULL;
            char hexa[6];
            int x, y, j;
            Pixel *pixel = malloc(sizeof(Pixel));
            Pixel *pixel_tmp = NULL;
            int tete = pixel;
            int pix = 0;
            printf("PIXEL = %d \n", pixels[6750]);
            /// Lignes horizontales (3)
            for(y = (Y_MAX / 5); y < Y_MAX - 1; y += (Y_MAX / 5))
            {
                for(x = 0; x < X_MAX; x++)
                {

                    pix = (x % X_MAX + (y * X_MAX));
                    tab = dectohexa(pixels[pix]);
                    tab = retourne(tab);

                    /// Si la couleur actuelle n'est pas égale à la dernière stockée, on l'enregistre.
                    if(pixel->b != rvb(tab, "B") || pixel->v != rvb(tab, "V") || pixel->r != rvb(tab, "R"))
                    {
                        pixel_tmp = malloc(sizeof(Pixel));
                        pixel_tmp->b = rvb(tab, "B");
                        pixel_tmp->v = rvb(tab, "V");
                        pixel_tmp->r = rvb(tab, "R");
                        pixel_tmp->x = x;
                        pixel_tmp->y = y;
                        pixel_tmp->dec = pixels[pix];
                        pixel_tmp->succ = NULL;

                        pixel->succ = pixel_tmp;
                        pixel = pixel->succ;
                    }
                }
            }

            /// Lignes verticales (3)
            for(x = (X_MAX / 5); x < X_MAX - 1; x += (X_MAX / 5))
            {
                for(y = 0; y < Y_MAX; y++)
                {
                    pix = (x % X_MAX + (y * X_MAX));
                    tab = dectohexa(pixels[pix]);
                    tab = retourne(tab);

                    /// Si la couleur actuelle n'est pas égale à la dernière stockée, on l'enregistre.
                    if(pixel->b != rvb(tab, "B") || pixel->v != rvb(tab, "V") || pixel->r != rvb(tab, "R"))
                    {
                        pixel_tmp = malloc(sizeof(Pixel));
                        pixel_tmp->b = rvb(tab, "B");
                        pixel_tmp->v = rvb(tab, "V");
                        pixel_tmp->r = rvb(tab, "R");
                        pixel_tmp->x = x;
                        pixel_tmp->y = y;
                        pixel_tmp->dec = pixels[pix];
                        pixel_tmp->succ = NULL;

                        pixel->succ = pixel_tmp;
                        pixel = pixel->succ;
                    }
                }

            }

            printf("\n\n");
            pixel = tete;
            pixel = pixel->succ;
            while(pixel != NULL)
            {
                printf("B = %d, V = %d, R = %d  (X = %d, Y = %d)", pixel->b, pixel->v, pixel->r, pixel->x, pixel->y);
                printf("\n\n");
                pixel = pixel->succ;
            }
            pixel = tete;

            int v_stripes = stripes_v(pixel->succ);
            int h_stripes = stripes_h(pixel->succ);
            if(h_stripes != 0)
                h_stripes++;
            if(v_stripes != 0)
                v_stripes++;
            printf("\n STRIPES Verticales = %d", v_stripes);
            printf("\n STRIPES Horizontales = %d\n\n", h_stripes);


            pixel = tete;
            if(croix(pixel->succ, pixels) == 1)
                printf("1 CROIX");
            else
                printf("0 CROIX");


            while(flagFinder.continuer)//boucle principale du programme
            {
                test_input(&flagFinder.continuer);/*Teste les appuis de touche, si on quitte le programme alors continuer = 0 dans cette fonction*/
                //update
                flagFinder.tempsCourant = SDL_GetTicks(); /*Pour gérer les FPS*/
                if(flagFinder.tempsCourant > flagFinder.prochainRendu)
                {
                    flagFinder.prochainRendu = flagFinder.tempsCourant + 20;

                    SDL_SetRenderDrawColor(flagFinder.renderer , 0 , 0 , 0 , 0);
                    SDL_RenderClear(flagFinder.renderer);/*On efface la zone d'affichage*/

                    SDL_RenderCopy(flagFinder.renderer, flagFinder.drapeauFrance.texture, NULL, NULL);
                    //SDL_RenderDrawLine(flagFinder.renderer, 1 % 225 , 1 / 225, (1%225) + 10, (1 / 225));

                    /*pixel = tete;
                    pixel = pixel->succ;
                    while(pixel != NULL)
                    {
                        SDL_RenderDrawLine(flagFinder.renderer, pixel->x, pixel->y, pixel->x + 2, pixel->y);
                        pixel = pixel->succ;
                    }*/


                    SDL_RenderPresent(flagFinder.renderer);/*On met à jour*/
                }
            }

            liberation(flagFinder.window , flagFinder.renderer);
        }
    }

    return 0;
}



}

int loadFiles(FlagFINDER *flagFinder)
{
    flagFinder->drapeauFrance.texture = loadTexture("10.png" , flagFinder);/*On charge l'image du drapeau de la France*/
    if(flagFinder->drapeauFrance.texture == NULL)
        return 0;
    return 1;
}

SDL_Texture* loadTexture(char *path , FlagFINDER *flagFinder)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        /*Convertit la surface au "pixel format" de l'écran (=résolution)*/
        SDL_Surface* formattedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(flagFinder->window)->format, NULL);

        if( formattedSurface == NULL )
        {
            printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            /*On créer une texture vide pour pouvoir y accéder avec SDL_TEXTUREACCESS_TARGET / SDL_TEXTUREACCESS_STREAMING */
            newTexture = SDL_CreateTexture( flagFinder->renderer, SDL_GetWindowPixelFormat(flagFinder->window), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
            if( newTexture == NULL )
            {
                printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                /*On copie les pixels de la surface dans la texture*/
                 SDL_LockTexture(newTexture, NULL, &flagFinder->drapeauFrance.mPixels, &flagFinder->drapeauFrance.mPitch);/*Pour copier, il faut verouiller la texture*/

                memcpy(flagFinder->drapeauFrance.mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);/*Pour copier*/

                //On met à jour en déverouillant
                SDL_UnlockTexture(newTexture);
            }

            SDL_FreeSurface(formattedSurface);//On supprime l'ancienne image formaté
        }

        //On supprime l'ancienne image
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void liberation(SDL_Window *window , SDL_Renderer *renderer)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
}
int hexa(int dec)
{
    if(dec == 0)
        return 'G';
    else if(dec < 9)
        return dec;
    else if(dec == 10)
        return 'A';
    else if(dec == 11)
        return 'B';
    else if(dec == 12)
        return 'C';
    else if(dec == 13)
        return 'D';
    else if(dec == 14)
        return 'E';
    else
        return 'F';
}

char* dectohexa(int dec)
{
    char *tab = malloc(sizeof(char));
    int dernierTour = 0;
    int i = 0;
    while(dec / 16 != 0 || dernierTour == 1)
    {
        tab = realloc(tab, (i + 1) * sizeof(char));

        tab[i] = hexa(dec % 16);

        dec = dec / 16;
        if(dec < 16)
        {
            if(dernierTour == 0)
                dernierTour = 1;
            else if(dernierTour == 1)
               dernierTour = 0;
        }
        i++;
    }
    tab[i] = '\0';


    return tab;
}
char* retourne(char *tab)
{
    char *tab2 = malloc(6 * sizeof(char));
    int i = 0, j, z = 0;
    /// do while à l'origine
    if(tab[i] == '\0')
        i = 0;
    else
    {
        do
        {
            i++;
        }
        while(tab[i] != '\0');
    }

    for(j = 0; j < 6 - i; j++)
    {
        tab2[j] = 0;
    }
    z = i - 1;
    for(j = 6 - i; j < 6; j++)
    {
        if(tab[z] == 'G')
            tab2[j] = 0;

        else
            tab2[j] = tab[z];
        z--;
    }

    return tab2;
}

int stripes_v(Pixel *pixel)
{
    if((pixel->succ)->y == pixel->y)
        return 1 + stripes_v(pixel->succ);
    else
        return 0;
}
int stripes_h(Pixel *pixel)
{
    if((pixel->succ)->x == pixel->x)
        return 1 + stripes_h(pixel->succ);
    else
        return 0;
}
/*
int cross(Pixel *pixel)
{
    if(pixel->succ)
}*/

int rvb(char hexa[6], char col[1])
{
    int code;
    int i = 0, z = 0;

    char color[2];

    if(col == "B")
        z = 4;
    else if(col == "V")
        z = 2;
    else
        z = 0;

    for(i = 0; i < 2; i++)
    {
         color[i] = hexa[i + z];

        switch(hexa[i + z])
        {
            case 'A' :
                color[i] = 10;
            break;
            case 'B' :
                color[i] = 11;
            break;
            case 'C' :
                color[i] = 12;
            break;
            case 'D' :
                color[i] = 13;
            break;
            case 'E' :
                color[i] = 14;
            break;
            case 'F' :
                color[i] = 15;
             break;
            case 'G' :
                color[i] = 0;
            break;
        }

    }

    code = ((color[0]*16) + color[1]);

    return code;
}

int croix(Pixel *test, int pixel[X_MAX * Y_MAX])
{
    char *tab = NULL;
    int dec, tmp, i, gap = 0;
    if(test->x == 0 || test->y == 0)
        test = test->succ;

    printf("(X = %d, Y = %d) \n", test->x, test->y);
        /// On recule d'un pixel en longueur
        tmp = (test->x + test->y * 225) - 1;
        dec = pixel[(test->x + test->y * 225) - 1];

            printf("dec = %d --> %d\n\n", dec, test->dec);


        /// On cherche si une bande horizontale vient couper cette bande verticale
        while(tmp < X_MAX * Y_MAX && test->dec != dec && gap != 0)
        {
            tmp = tmp + 225;
            dec = pixel[tmp];
            if(test->dec != dec)
                gap = 1;
        }
        /// S'il a trouuvé une bande horizontale,
        if(tmp < X_MAX * Y_MAX - 225)
        {
            printf("tmp = %d / %d", tmp, X_MAX*Y_MAX - 225);
            /// on recule d'un pixel en hauteur
            tmp = tmp - 225;
            dec = pixel[tmp];

            /// On recherche la bande verticale du départ, cette fois ci au contact de notre bande horizontale
            while(tmp % 225 != 0 && test->dec != dec)
            {
                tmp = tmp + 1;
                dec = pixel[tmp];
            }

            /// On a retrouvé la bande verticale, il y a une croix
            if(tmp % 225 != 0)
                return 1;
        }
    return 0;
}

int main(int argc, char *argv[])
{
    FlagFINDER flagFinder;/*Contient toutes les variables nécessaires*/
    flagFinder.continuer = 1;
    flagFinder.prochainRendu = 0;

    Uint32 *pixels;

    if(initSDL(&flagFinder))
    {
        if(loadFiles(&flagFinder))
        {
            pixels = (Uint32*)flagFinder.drapeauFrance.mPixels;


            char *tab = NULL;
            char hexa[6];
            int x, y, j;
            Pixel *pixel = malloc(sizeof(Pixel));
            Pixel *pixel_tmp = NULL;
            int tete = pixel;
            int pix = 0;
            printf("PIXEL = %d \n", pixels[6750]);
            /// Lignes horizontales (3)
            for(y = (Y_MAX / 5); y < Y_MAX - 1; y += (Y_MAX / 5))
            {
                for(x = 0; x < X_MAX; x++)
                {

                    pix = (x % X_MAX + (y * X_MAX));
                    tab = dectohexa(pixels[pix]);
                    tab = retourne(tab);

                    /// Si la couleur actuelle n'est pas égale à la dernière stockée, on l'enregistre.
                    if(pixel->b != rvb(tab, "B") || pixel->v != rvb(tab, "V") || pixel->r != rvb(tab, "R"))
                    {
                        pixel_tmp = malloc(sizeof(Pixel));
                        pixel_tmp->b = rvb(tab, "B");
                        pixel_tmp->v = rvb(tab, "V");
                        pixel_tmp->r = rvb(tab, "R");
                        pixel_tmp->x = x;
                        pixel_tmp->y = y;
                        pixel_tmp->dec = pixels[pix];
                        pixel_tmp->succ = NULL;

                        pixel->succ = pixel_tmp;
                        pixel = pixel->succ;
                    }
                }
            }

            /// Lignes verticales (3)
            for(x = (X_MAX / 5); x < X_MAX - 1; x += (X_MAX / 5))
            {
                for(y = 0; y < Y_MAX; y++)
                {
                    pix = (x % X_MAX + (y * X_MAX));
                    tab = dectohexa(pixels[pix]);
                    tab = retourne(tab);

                    /// Si la couleur actuelle n'est pas égale à la dernière stockée, on l'enregistre.
                    if(pixel->b != rvb(tab, "B") || pixel->v != rvb(tab, "V") || pixel->r != rvb(tab, "R"))
                    {
                        pixel_tmp = malloc(sizeof(Pixel));
                        pixel_tmp->b = rvb(tab, "B");
                        pixel_tmp->v = rvb(tab, "V");
                        pixel_tmp->r = rvb(tab, "R");
                        pixel_tmp->x = x;
                        pixel_tmp->y = y;
                        pixel_tmp->dec = pixels[pix];
                        pixel_tmp->succ = NULL;

                        pixel->succ = pixel_tmp;
                        pixel = pixel->succ;
                    }
                }

            }

            printf("\n\n");
            pixel = tete;
            pixel = pixel->succ;
            while(pixel != NULL)
            {
                printf("B = %d, V = %d, R = %d  (X = %d, Y = %d)", pixel->b, pixel->v, pixel->r, pixel->x, pixel->y);
                printf("\n\n");
                pixel = pixel->succ;
            }
            pixel = tete;

            int v_stripes = stripes_v(pixel->succ);
            int h_stripes = stripes_h(pixel->succ);
            if(h_stripes != 0)
                h_stripes++;
            if(v_stripes != 0)
                v_stripes++;
            printf("\n STRIPES Verticales = %d", v_stripes);
            printf("\n STRIPES Horizontales = %d\n\n", h_stripes);


            pixel = tete;
            if(croix(pixel->succ, pixels) == 1)
                printf("1 CROIX");
            else
                printf("0 CROIX");


            while(flagFinder.continuer)//boucle principale du programme
            {
                test_input(&flagFinder.continuer);/*Teste les appuis de touche, si on quitte le programme alors continuer = 0 dans cette fonction*/
                //update
                flagFinder.tempsCourant = SDL_GetTicks(); /*Pour gérer les FPS*/
                if(flagFinder.tempsCourant > flagFinder.prochainRendu)
                {
                    flagFinder.prochainRendu = flagFinder.tempsCourant + 20;

                    SDL_SetRenderDrawColor(flagFinder.renderer , 0 , 0 , 0 , 0);
                    SDL_RenderClear(flagFinder.renderer);/*On efface la zone d'affichage*/

                    SDL_RenderCopy(flagFinder.renderer, flagFinder.drapeauFrance.texture, NULL, NULL);
                    //SDL_RenderDrawLine(flagFinder.renderer, 1 % 225 , 1 / 225, (1%225) + 10, (1 / 225));

                    /*pixel = tete;
                    pixel = pixel->succ;
                    while(pixel != NULL)
                    {
                        SDL_RenderDrawLine(flagFinder.renderer, pixel->x, pixel->y, pixel->x + 2, pixel->y);
                        pixel = pixel->succ;
                    }*/


                    SDL_RenderPresent(flagFinder.renderer);/*On met à jour*/
                }
            }

            liberation(flagFinder.window , flagFinder.renderer);
        }
    }

    return 0;
}


