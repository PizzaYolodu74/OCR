//
// Created by Paul-alexandre
//

#ifndef OCR_CUTTING_H
#define OCR_CUTTING_H
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


// This struct is the base of the segmentation
// It allow to cut matrix without insane complexity by saving
// position of cut matrix and only recreate matrix at the end of the
// segmentation.
typedef struct Image Image;
struct Image
{
    size_t begin_h;
    size_t begin_w;
    size_t height;
    size_t width;
};
Image* vertical(Uint8* M,Image input,Image img,Uint8 h,
                        size_t s,size_t *cut_list_length);
Image* horizontal(Uint8* M,Image input, Image img,
        Uint8 v, size_t s,size_t *cut_list_length);
Image* lines(Uint8 *M, Image input,Image img,
        size_t*cut_list_length);
Image* cols(Uint8 *M, Uint8 word, Image input,Image img
        , size_t *cut_list_length);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif //OCR_CUTTING_H



