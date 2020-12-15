//
// Created by Paul-alexandre
//

#ifndef OCR_USUAL_FUNCTIONS_H
#define OCR_USUAL_FUNCTIONS_H
#include "Cutting.h"
#include <stdio.h>
#include <stdlib.h>


void cut_matrix(Image img,Image *matrix_list,size_t *cut_list
        ,size_t length,Uint8 vertical);
size_t* thresholding(size_t H[],size_t Hlength,size_t s,size_t length,
                      size_t *cut_list_length);
//Uint8* adjust_matrix_size(Uint8* M,size_t length,size_t width);
Uint8* square_reshaped(Uint8 *M, Image input,  Image img, Uint8
*new_M, Uint8 vertical);
void wipe_white_borders(Uint8 *M, Image input, Image *img);
void concatenate(Image *M1,Image *M2,size_t *M1_length,size_t *M2_length);

#endif //OCR_USUAL_FUNCTIONS_H
