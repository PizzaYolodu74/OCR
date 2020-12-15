//
// Created by Paul-alexandre
//

#ifndef OCR_ROTATE_H
#define OCR_ROTATE_H
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Cutting.h"
#include <math.h>

SDL_Surface* rotate (SDL_Surface* img, int height, int width, int degree_angle);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

#endif //OCR_ROTATE_H
