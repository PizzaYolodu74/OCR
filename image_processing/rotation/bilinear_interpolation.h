//
// Created by Paul-alexandre
//

#ifndef OCR_BILINEAR_INTERPOLATION_H
#define OCR_BILINEAR_INTERPOLATION_H
#include <SDL2/SDL.h>
#include "rotate.h"

float bilinearly_interpolate (int top, int bottom, int left, int right,
                              float horizontal_position, float
                              vertical_position, SDL_Surface* img);

#endif //OCR_BILINEAR_INTERPOLATION_H
