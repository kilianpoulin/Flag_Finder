#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Couleur
{
    Uint32 *pixels;

}Couleur;

typedef struct Pixel
{
    struct Pixel *succ;
    int r;
    int v;
    int b;
    int x;
    int y;
    int dec;

} Pixel;
typedef struct Drapeau
{
    SDL_Texture *texture;
    /*Coordonnees *coordonnees;Tableau dynamique pour stocker les coord'*/
    Couleur *couleur;/*Liste chainée pour stocker les différents pixels du drapeau*/
    void* mPixels;/*Les pixels de l'image*/
    int mPitch;/*Longueur de l'image*/
}Drapeau;

typedef struct FlagFINDER
{
    int continuer;
    Uint32 tempsCourant, prochainRendu;
    SDL_Renderer *renderer;/*Pointeur "contenant" la zone d'affichage*/
    SDL_Window *window;/*Pointeur "contenant" la fenêtre*/
    Drapeau drapeauFrance;

}FlagFINDER;
#endif // STRUCTS_H
