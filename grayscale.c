#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "grayscale.h"

//25/10 Léa
//Convetit une image en couleur en noir et blanc
//prend l'image de base retoune sa version en noir et blanc
void grayscale(SDL_Surface* img)
{
  Uint8 r, g, b;
  int width=img->w;
  int height=img->h;
  for(int i=0;i<width;i++)
    {
      for(int j=0;j<height;j++)
	{
	  Uint32 pixel = get_pixel(img, i, j);
	  SDL_GetRGB(pixel, img->format, &r, &g, &b);
	  Uint8 average=0.3*r + 0.59*g + 0.11*b;
	  pixel = SDL_MapRGB(img->format, average, average, average);
	  put_pixel(img, i, j, pixel);
	  
	}
    }
}
