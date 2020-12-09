//
// Created by Paul-alexandre
//

#include "Usual_Functions.h"
#include "Cutting.h"
#include <stdio.h>
#include <stdlib.h>

//##############################################################################
//#                       Usual fonctions for the cutting                      #
//##############################################################################


// Cut the matrix according to the i or j cut_list depending of height or width
// the cutting function preserve the reading direction because in occidental
// language,
// we read from top to bottom and from left to right as the cutting function
// work

///todo:faire fonction concatenate
///todo:fonction concatenate
///todo:fonction concatenate
///todo:fonction concatenate
///todo:fonction concatenate
///todo:fonction concatenate
///todo:fonction concatenate

void cut_matrix(Image img,Image *matrix_list,size_t *cut_list
        ,size_t length,char vertical){
    if (length==0){
        *matrix_list=img;
        return;
    }
    if (vertical){                  // (length cuts= length+1 structs)
        // Allow to have length matrix from one matrix
        size_t pos = img.begin_h;
        for (size_t posi = 0; posi < length; ++posi) {
            size_t height = *(cut_list+posi)-pos;
            Image new_img = {
                    pos,            // begin_h
                    img.begin_w,    // begin_w
                    height,         // height
                    img.width,      // width
            };
            *(matrix_list+posi)=new_img;
            pos=*(cut_list+posi);   // move the origin to the next position
        }
        Image new_img = {
                pos,                  // begin_h
                img.begin_w,          // begin_w
                img.height+img.begin_h-pos,// height
                img.width,            // width
        };
        *(matrix_list+length)=new_img;

    }
    else{
         // Allow to have length matrix from one matrix
        size_t pos = img.begin_w;
        for (size_t posi = 0; posi < length; ++posi) {
            size_t width = *(cut_list+posi)-pos;
            Image new_img = {
                    img.begin_h,    // begin_h
                    pos,            // begin_w
                    img.height,     // height
                    width,          // width
            };
            *(matrix_list+posi)=new_img;
            pos=*(cut_list+posi);   // move the origin to the next position
        }
        Image new_img = {
                img.begin_h,               // begin_h
                pos,                      // begin_w
                img.height,               // height
                img.width+img.begin_w-pos,// width
        };
        *(matrix_list+length)=new_img;
    }
}

// Create an empty histogram (initialized at 0)
void create_histogram(size_t size,size_t H[]){
    for (size_t i = 0; i < size; ++i) {
        H[i]=0;
    }
}


// Using the threshold and the length of the matrix,
// return a list of row(or columns) where we will need to cut

size_t* thresholding(size_t H[],size_t Hlength,size_t s,
                      size_t length,size_t *cut_list_length){
    // Return where we need to cut in function of the threshold
    size_t cut_list[length];
    size_t n=0;
    for (size_t index = 0; index < Hlength ; ++index) {
        if (H[index] == length){
            // If the index position of the histogram is equal to
            // length, it means that it is a white
            // row or column in the picture, so we add one to the
            // counter 'n'
            n++;
        }
        else{
            if (n>s){
                // If this white part of the histogram is higher than
                // the threshold 's'
                // add the middle of the place where we need to cut in
                // the cut_list
                cut_list[*cut_list_length]=(index-(n/2));
                *cut_list_length++;
            }
            n=0;
        }
    }

    // Adjust the cut_list matrix to be of length *cut_list_length
    size_t *new_cut_list=(size_t*) malloc(*cut_list_length * sizeof(size_t));
    for(size_t i=0;i<*cut_list_length;++i){
        *(new_cut_list+i)=cut_list[i];
    }
    return new_cut_list;
}

//todo: il peut etre opti d'appeler wipe dans les fonctions de cutting en
// ayant deja en parametre l'histogramme
void wipe_white_borders(char *M,Image input,Image img){
    // Vertical run
    size_t HV[img.width];
    create_histogram(img.width, HV);
    for (size_t col = img.begin_w; col < img.width+img.begin_w; ++col) {
        for (size_t line = img.begin_h; line < img.height+img.begin_h; ++line) {
            HV[col-img.begin_w] += *(M+line*input.width+col)/255;
        }
    }
    size_t width_max=img.width+img.begin_w;
    ///todo: si un pb, ça peut venir d'ici
    while (img.begin_w<width_max && HV[img.begin_w]==img.height){
        img.begin_w++;
        img.width--;
    }
    if (img.begin_w==width_max)
        ///todo: réfléchir à comment faire lorsque la matrice se retrouve
        /// totalement vide, pour le moment la structure reste juste vide
        return;
    ///todo: si un pb, ça peut venir d'ici
    while (width_max>img.begin_w && HV[width_max-1]==img.height){
        img.width--;
        width_max--;
    }

    // Horizontal run
    size_t HH[img.height];
    create_histogram(img.height, HH);
    for (size_t line = img.begin_h; line < img.height+img.begin_h; ++line) {
        for (size_t col = img.begin_w; col < img.width+img.begin_w; ++col) {
            HH[line-img.begin_h] += *(M+line*input.width+col)/255;
        }
    }
    size_t height_max=img.height+img.begin_h;
    ///todo: si un pb, ça peut venir d'ici
    while (img.begin_h<height_max && HV[img.begin_h]==img.width){
        img.begin_h++;
        img.height--;
    }
    if (img.begin_h==height_max)
        ///todo: réfléchir à comment faire lorsque la matrice se retrouve
        /// totalement vide, pour le moment la structure reste juste vide
        return;
    ///todo: si un pb, ça peut venir d'ici
    while (height_max>img.begin_h && HH[height_max-1]==img.width){
        img.height--;
        height_max--;
    }

}



char* square_reshaped(char *M,Image input, Image img, char
        *new_M, char vertical){
    if (vertical==1){
        char newM[img.height*img.height];
        for (size_t line = 0; line < img.height; ++line) {
            for (size_t col = 0; col < (img.height-img.width)/2; ++col) {
                *(newM+line*img.width+col)=255;
            }
            for (size_t col = (img.height-img.width)/2;
            col < img.width+(img.height-img.width)/2; ++col) {
                *(newM+line*img.width+col)=*(M+line*input.width+col);
            }
            for (size_t col = img.width+(img.height-img.width)/2;
            col < img.height; ++col) {
                *(newM+line*img.width+col)=255;
            }
        }
    }
    else{
        char newM[img.width*img.width];
        for (size_t line = 0; line < img.width; ++line) {
            for (size_t col = 0; col < (img.width-img.height)/2; ++col) {
                *(newM+line*img.height+col)=255;
            }
            for (size_t col = (img.width-img.height)/2;
                 col < img.height+(img.width-img.height)/2; ++col) {
                *(newM+line*img.height+col)=*(M+line*input.height+col);
            }
            for (size_t col = img.height+(img.width-img.height)/2;
                 col < img.width; ++col) {
                *(newM+line*img.height+col)=255;
            }
        }
    }
    return new_M;
}

/*
char* adjust_matrix_size(char *M,Image input, Image img, char
*new_M, char vertical){
    if (img.height<16){
        if (img.width<16){
            if (img.height>img.width){
                square_M = (char *) malloc(img.height*img.height * sizeof
                        (char));
                square_reshaped(M,input,img,square_M,1);
                img.width=img.height;
            }
            if (img.height<img.width) {
                square_M = (char *) malloc(img.width * img.width * sizeof
                        (char));
                square_reshaped(M, input, img, square_M, 0);
                img.height = img.width;
            }
            if (img.height==img.width){
                char *square_M = M;
            }
            for (size_t i = 0; i < 16; ++i) {
                for (size_t j = 0; j < 16; ++j) {
                    *(new_M+
                }
            }
        }
    }
    if (l<l0){

    }


}
 */

/* todo: resize >16 cassé
char* resize(char *M,size_t left,size_t right,size_t l,size_t left0
 ,size_t right0,size_t l0){
    char newM[right-left];
    for (size_t i = left; i < right; ++i) {
        newM[i-left]=M[i];
    }
}
*/