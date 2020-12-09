//
// Created by Paul-alexandre
//

#ifndef OCR_USUAL_FUNCTIONS_H
#define OCR_USUAL_FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>


void cut_matrix(char* M,struct Image img,struct Image matrix_list[],
        int *cut_list,int length,int v);
void create_histogram(size_t size,size_t H[]);
size_t[] thresholding(size_t H[],size_t Hlength,size_t s,size_t length,
                      size_t *cut_list_length);
//char* adjust_matrix_size(char* M,size_t length,size_t width);
char* square_reshaped(char *M,struct Image input, struct Image img, char
*new_M, char vertical);
void wipe_white_borders(char *M,struct Image input,struct Image img);

#endif //OCR_USUAL_FUNCTIONS_H



