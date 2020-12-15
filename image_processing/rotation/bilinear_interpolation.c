//
// Created by Paul-alexandre
//

#include "bilinear_interpolation.h"


// Function that performs bilinear interpolation to determine the new pixel
// value.
float bilinearly_interpolate (int top, int bottom, int left, int right,
                              float horizontal_position, float
                              vertical_position, SDL_Surface* img)
{
    // Determine the values of the corners.

    Uint32 top_left_pixel = get_pixel(img, top, left);
    Uint32 top_right_pixel = get_pixel(img, top, right);
    Uint32 bottom_right_pixel = get_pixel(img, bottom, left);
    Uint32 bottom_left_pixel = get_pixel(img, bottom, right);

    Uint8 rtl, gtl ,btl,rtr,gtr,btr,rbl,gbl,bbl,rbr,gbr,bbr;

    SDL_GetRGB(top_left_pixel, img->format, &rtl, &gtl, &btl);
    SDL_GetRGB(top_right_pixel, img->format, &rtr, &gtr, &btr);
    SDL_GetRGB(bottom_left_pixel, img->format, &rbl, &gbl, &bbl);
    SDL_GetRGB(bottom_right_pixel, img->format, &rbr, &gbr, &bbr);

    float top_left = rtl;
    float top_right = rtr;
    float bottom_left = rbl;
    float bottom_right = rbr;

    // Figure out "how far" the output pixel being considered is
    // between left and right.
    float horizontal_progress = horizontal_position -
                                (float) left;
    float vertical_progress = vertical_position -
                              (float) top;

    // Combine top values.
    float top_block = top_left + horizontal_progress
                                 * (top_right - top_left);
    // Combine bottom values.
    float bottom_block = bottom_left +
                         horizontal_progress
                         * (bottom_right - bottom_left);

    // Combine top and bottom using vertical interpolation.
    // Return the resulting pixel.
    return top_block + vertical_progress * (bottom_block - top_block);
}