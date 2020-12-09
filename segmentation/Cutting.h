//
// Created by Paul-alexandre
//

#ifndef OCR_CUTTING_H
#define OCR_CUTTING_H
#include <stdio.h>
#include <stdlib.h>
#define SIZE

typedef struct Image Image;
struct Image
{
    size_t begin_h;
    size_t begin_w;
    size_t height;
    size_t width;
};
Image[] vertical(char*M,Image input,Image img,char h,
                        size_t s,size_t *cut_list_length);
Image[] horizontal(char*M,Image input, Image img,
        char v, size_t s,size_t *cut_list_length);
Image[] lines(char *M, Image input,Image img,
        size_t*cut_list_length);
Image[] cols(char *M, size_t s, Image input,Image img
        , size_t *cut_list_length);

#endif //OCR_CUTTING_H



