#ifndef BINARISATION_H_
#define BINARISATION_H_
//25/10 Léa

//Calcul de l'image intégrale d'un pixel en position x, y de notre image
Uint32 I_img(SDL_Surface*img,int x,int y);

//Calcul de la moyenne locale d'un pixel en position x, y de notre image en utilisant la valeur de son image intégrale
Uint32 mean(SDL_Surface*img,int x,int y);

//Calcul du seuil local d'un pixel en position x, y de notre image en utilisant la valeur de sa moyenne locale
Uint32 s( SDL_Surface*img, int x, int y);

//Conversion d'une image en image binaire
void binarisation(SDL_Surface*img);

#endif
