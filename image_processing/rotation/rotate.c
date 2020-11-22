//
// Created by Paul-alexandre
//

#include "rotate.h"
#include "bilinear_interpolation.h"

#include <SDL.h> // !! Il faut modifier le chemin
#include <stdio.h>
#include <math.h>

/*
//USUAL METHODS FOR SDL images
SDL_Surface* img;
img = load_image("my_image.jpg");
int width=img->w;
int height->h;

Uint8 r, g ,b;//changer en int si on fait des calculs dessin
SDL_GetRGB(pixel, img->format, &r, &g, &b);
Uint32 pixel;
pixel = get_pixel(img, h, l);//h represente la hauteur!
pixel = SDL_MapRGB(img->format, 255, 255, 255);//place les valeurs rgb
putpixel(output_img,i,j,pixel);
*/

/*
// Prompt the angle we want to rotate
float degree_angle;
printf("Enter the degree you want to rotate the image (counterclockwise!).\n");//ccw=counterclockwise
scanf("%f", &degree_angle);
*/

SDL_Surface* rotate (SDL_Surface* img, int height, int width)
{
    // Define the center of the image.
    int height_center = floor(height / 2);
    int width_center = floor(width / 2);

    // Loop through each pixel of the new image, select the new vertical
    // and horizontal positions, and interpolate the image to make the change.

    SDL_Surface* output_img = SDL_CreateRGBSurface(0, height, width, 32, 0, 0, 0, 0);//wrong size need to fix it

    int i, j;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Convert degree angle to radian angle
            // And take the new position of the pixel
            double angle = degree_angle * (double) Pi / 180;
            float vertical_position = (float) cos(angle) *
                                      (i - height_center) + sin(angle) * (j - width_center)
                                      + height_center;
            float horizontal_position = (float) -sin(angle) *
                                        (i - height_center) + cos(angle) * (j - width_center)
                                        + width_center;

            // Take the four locations and pixels that we want to interpolate from the input image
            int top = floor(vertical_position);
            int bottom = top + 1;
            int left = floor(horizontal_position);
            int right = left + 1;

            // Take the current pixel
            if ( SDL_MUSTLOCK(output_img) ) {
                if ( SDL_LockSurface(output_img) < 0 ) {
                    fprintf(stderr, "Can't lock output_img: %s\n", SDL_GetError());
                    return;
                }
            }
            pixel = get_pixel(img, i, j);
            Uint8 r, g ,b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            // Check if any of the four locations are invalid. If they are,
            // skip interpolating this pixel. Otherwise, interpolate the
            // pixel and set the resulting pixel.
            if (top >= 0 && bottom < height && left >= 0 && right < width ) {
                float interpolated = bilinearly_interpolate(top, bottom,left, right, horizontal_position,
                                                            vertical_position,img);

                // Set rgb value of the new pixel in the output img
                if (interpolated % 1 > 0.5 )
                    r=floorf(interpolated)++;
                else
                    r=floorf(interpolated);
                g=r;
                b=r;

                pixel = SDL_MapRGB(output_img->format, r, g, b);
                putpixel(output_img,i,j,pixel);
                if ( SDL_MUSTLOCK(output_img) ) {
                    SDL_UnlockSurface(output_img);
                    }
                }
            }
        }
    }

    return output_img;
}