#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "binarisation.h"

//25/10 Léa
//Calcul de l'image intégrale du pixel en position x et y sur l'image que l'on veut traiter
//prend l'image , la position x du pixel , la position y du pixel
Uint32 I_img(SDL_Surface*img,int x,int y)
{
  Uint32 tot= get_pixel(img,x,y);
  if(x!=0)
   {
     for(int i=0; i<x; i++)
      {
	 tot+=get_pixel(img, i, y);
      }
    }
  if(y!=0)
   {
     for(int j =0; j<y; j++)
      {
	tot+= get_pixel(img, x, j);
      }
   }
   return tot;
}

//25/10 Léa
//Calcul de la moyenne locale du pixel après avoir obtenu la valeur dans son Image intégrale
//Prend en arguments: l'image de base, la position x et la position y du pixel
Uint32 mean(SDL_Surface*img,int x,int y)
{
  int width =img->w;
  int height = img->h;
  Uint32 tot= get_pixel(img, x, y);
  if( x-60/2 >= 0 && y-60/2 >= 0)
    {
      if(x+60/2 < width && y+60/2 < height)
       {
	 tot=(I_img(img,x+60/2, y+60/2)+I_img(img,x-60/2,y-60/2)-I_img(img,x+60/2,y-60/2)-I_img(img,x-60/2,y+60/2))/pow(60,2);
       }
    }
      return tot;
}

//25/10 Léa
//Calcul du seuil local d'un pixel ce qui va permettre d'établir le seuil de la binarisation
//Prend en arguments: l'image de base, la position x et la position y du pixel return la valeur du seuil du pixel Uint32
Uint32 s( SDL_Surface*img, int x, int y)
{
  int width =img->w;
  int height = img->h;
  Uint32 tot=0;
  Uint32 final= get_pixel(img, x, y);
  for(int i = x-60/2; i < x+60/2; i++)
   {
     for(int j = y-60/2; j< y+60/2; j++)
      {
	if(i>=0 && i < width && j>=0 && j < height)
	 {
	   tot +=get_pixel(img, i, j);
	 }
      }
      final = pow((tot-mean(img,x,y)),2)/pow(60,2);
   }

  return final;
}
  
//Convertit une image en image binaire
//Pour chaque pixel de notre image de base nous appliquons les formules faites avant pour trouver le seuil du pixel en position x et y si la valeur de notre pixel est > à ce seuil il sera blanc =1 sinon il sera noir.
//Prend en paramètre notre image de base retourne notre image transformée.
void binarisation(SDL_Surface*img)
{
  int height =img->h;
  int width =img->w;
  for(int x=0; x < width; x++)
    {
      for(int y=0; y < height; y++)
	{
	  Uint32 T = mean(img,x,y)*(1+0.4*((s(img,x,y)/128)-1));
	    if(get_pixel(img, x, y)< T)
	      {
		 put_pixel(img, x, y, 0);
	      }
	    else
	      {
		put_pixel(img, x, y, 1);
	      }
	}
    }
}
