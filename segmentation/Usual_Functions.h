//
// Created by Paul-alexandre
//

#ifndef OCR_USUAL_FUNCTIONS_H
#define OCR_USUAL_FUNCTIONS_H

void cut_matrix(char M[],size_t i,size_t j,size_t length,size_t width);
int* create_histogram(size_t nb,size_t H[]);
int* thresholding(size_t H[],int s, size_t l);
void wipe_white_borders(M,size_t length,size_t width);

#endif //OCR_USUAL_FUNCTIONS_H



