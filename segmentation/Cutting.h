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
struct Image[] vertical(char*M,struct Image input,struct Image img,char h,
                        size_t s,size_t *cut_list_length);
struct Image[] horizontal(char*M,struct Image input, struct Image img,
        char v, size_t s,size_t *cut_list_length);
struct Image[] lines(char *M, struct Image input,struct Image img,
        size_t*cut_list_length);
struct Image[] cols(char *M, size_t s, struct Image input,struct Image img
        , size_t *cut_list_length);

#endif //OCR_CUTTING_H



