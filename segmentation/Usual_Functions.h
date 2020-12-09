//
// Created by Paul-alexandre
//

#ifndef OCR_USUAL_FUNCTIONS_H
#define OCR_USUAL_FUNCTIONS_H
#include "<Cutting.h>"
#include <stdio.h>
#include <stdlib.h>


void cut_matrix(char* M, Image img, Image matrix_list[],
        int *cut_list,int length,int v);
void create_histogram(size_t size,size_t H[]);
size_t[] thresholding(size_t H[],size_t Hlength,size_t s,size_t length,
                      size_t *cut_list_length);
//char* adjust_matrix_size(char* M,size_t length,size_t width);
char* square_reshaped(char *M, Image input,  Image img, char
*new_M, char vertical);
void wipe_white_borders(char *M, Image input, Image img);

#endif //OCR_USUAL_FUNCTIONS_H



