//
// Created by Paul-alexandre
//

#ifndef OCR_BILINEAR_INTERPOLATION_H
#define OCR_BILINEAR_INTERPOLATION_H

float bilinearly_interpolate (int top, int bottom, int left, int right,
                              float horizontal_position, float vertical_position, SDL_surface* img);

#endif //OCR_BILINEAR_INTERPOLATION_H
