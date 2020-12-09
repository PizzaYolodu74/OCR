//
// Created by Paul-alexandre
//

#include "Cutting.h"
#include "Usual_Functions.h"
#include <stdio.h>
#include <stdlib.h>


///todo:refaire ce commentaire
// Vertical scan of the image (matrix) in order to list the different vertical blocks
// call also the same horizontal path recursively
// until all blocks have the minimum size above the threshold
// recursivity stops when a vertical and horizontal path are negative
// we use the h or v boolean var to know if the last call was negative or not
// return the list of cut matrix
// The input struct is for the initial picture, struct img is the actual image.
Image* vertical(char*M,Image input,Image img,char h,
        size_t s,size_t *cut_list_length){

    // Creates the histogram whose variables are the number of white pixels
    // on each row of the matrix
    size_t HV[img.width];
    create_histogram(img.width, HV);
    for (size_t col = img.begin_w; col < img.width+img.begin_w; ++col) {
        for (size_t line = img.begin_h; line < img.height+img.begin_h; ++line) {
            HV[col-img.begin_w] += *(M+line*input.width+col)/255;
        }
    }

    // Handles thresholding using the threshold and the full histogram.
    size_t *cut_list = thresholding(HV,img.width,s,img.height,cut_list_length);

    // We make a list of cut areas and if this one is empty we will stop
    // either if the horizontal call is negative either immediately if the
    // last vertical call was negative.
    if (cut_list_length==0 && h==0){
        Image *matrix_list=(Image*) malloc(sizeof(Image));
        *matrix_list=img;
        return matrix_list;
    }
    else{
        if (cut_list_length==0){
            return horizontal(M,input,img,0,s,cut_list_length);
        }
        else{
            // Set the position to cut starting at the beginning of the matrix:
            // img.begin_h .
            for (int i = 0; i < *cut_list_length; ++i) {
                *(cut_list+i)+=img.begin_h;
            }

            // Proceed the matrix cutting for each column of the cut-list
            Image *matrix_list=
                    (Image*) malloc(*(cut_list_length+1)*sizeof(Image));
            cut_matrix(img,matrix_list,cut_list,*cut_list_length,1);

            // Make a recursive horizontal call for each of the new matrix
            size_t *cut_Hlist_length;
            Image *matrix_list2 =
                    horizontal(M,input,*(matrix_list),1,s,cut_Hlist_length);
            size_t matrix_list2_length=*cut_Hlist_length;
/// todo: verifier qu'il faut bien un +1 a cut_list_length
            for (size_t pos = 1; pos < *(cut_list_length+1); ++pos) {
                *cut_Hlist_length=0;
                // The concatenate function modify matrix_list2_length to
                // the new concatenate size.
                // horizontal() modify the cut_Hlist_length too.
                concatenate(matrix_list2,horizontal
                (M,input,*(matrix_list+pos),1,s,cut_Hlist_length),
                &matrix_list2_length,cut_Hlist_length);
            }
            *cut_list_length=matrix_list2_length;
            return matrix_list2;
        }
    }
}


///todo:refaire ce commentaire
// horizontal scan of the image (matrix) in order to list the different
// horizontal blocks
// call also the same vertical path recursively
// until all blocks have the minimum size above the threshold
// recursivity stops when a horizontal and vertical path are negative
// we use the h or v boolean var to know if the last call was negative or not
// return the list of cut matrix
// The input struct is for the initial picture, struct img is the actual image.
Image* horizontal(char*M,Image input, Image img,char v,
        size_t s,size_t *cut_list_length){
    // Creates the histogram whose variables are the number of white pixels
    // on each line of the matrix
    size_t HH[img.height];
    create_histogram(img.height, HH);
    for (size_t line = img.begin_h; line < img.height+img.begin_h; ++line) {
        for (size_t col = img.begin_w; col < img.width+img.begin_w; ++col) {
            HH[line-img.begin_h] += *(M+line*input.width+col)/255;
        }
    }
    // Handles thresholding using the threshold and the full histogram
    size_t *cut_list = thresholding(HH,img.height,s,img.width,cut_list_length);

    // We make a list of cut areas and if this one is empty we will stop
    // either if the vertical call is negative either immediately if the
    // last horizontal call was negative
    if (cut_list_length==0 && v==0){
        Image *matrix_list = (Image*) malloc(sizeof(Image));
        *matrix_list=img;
        return matrix_list;
    }
    else{
        if (cut_list_length==0){
            return vertical(M,input,img,0,s,cut_list_length);
        }
        else{
            // Set the position to cut starting at the beginning of the matrix:
            // img.begin_w .
            for (int i = 0; i < *cut_list_length; ++i) {
                *(cut_list+i)+=img.begin_w;
            }

            // Proceed the matrix cutting for each column of the cut-list
            Image *matrix_list=
                    (Image*) malloc(*(cut_list_length+1)*sizeof(Image));
            cut_matrix(img,matrix_list,cut_list,*cut_list_length,0);

            // Make a recursive vertical call for each of the new matrix
            // The concatenate function modify matrix_list2_length to the new concatenate size
            size_t *cut_Vlist_length;
            Image *matrix_list2 =
                    vertical(M,input,*(matrix_list),1,s,cut_Vlist_length);
            size_t matrix_list2_length=*cut_Vlist_length;
/// todo: verifier qu'il faut bien un +1 a cut_list_length
            for (size_t pos = 1; pos < *(cut_list_length+1); ++pos) {
                *cut_Vlist_length=0;
                // The concatenate function modify matrix_list2_length to
                // the new concatenate size.
                // vertical() modify the cut_Vlist_length too.
                concatenate(matrix_list2,vertical
                (M,input,*(matrix_list+pos),1,s,cut_Vlist_length),
                &matrix_list2_length,cut_Vlist_length);
            }
            *cut_list_length=matrix_list2_length;
            return matrix_list2;
        }
    }
    //free useless pointers todo: à faire en dehors de la fonction!
    //free(cut_list_length);
    // free(matrix_list)
}

///todo:commentaires a refaire
Image* lines(char *M, Image input,Image img, size_t *cut_list_length){
    // Creates the histogram whose variables are the number of white pixels
    // on each line of the matrix
    size_t HL[img.height];
    create_histogram(img.height, HL);
    for (size_t line = img.begin_h; line < img.height+img.begin_h; ++line) {
        for (size_t col = img.begin_w; col < img.width+img.begin_w; ++col) {
            HL[line-img.begin_h] += *(M+line*input.width+col)/255;
        }
    }

    ///todo: fonction seuil lignes (remplacer s)
    size_t s = (size_t) 0.25*img.width;
    // Handles thresholding using the threshold and the full histogram
    /// todo: test si *cut_list_length bien est utilisé pour l'array après l'appel de thresholding
    size_t *cut_list = thresholding(HL,img.height,s,img.width,cut_list_length);

    // Make a list of cut_areas
    if (cut_list_length==0){
        Image *matrix_list = (Image*) malloc(sizeof(Image));
        *matrix_list=img;
        return matrix_list;
    }
    else{
        for (int i = 0; i < *cut_list_length; ++i) {
            *(cut_list+i)+=img.begin_w;
        }
        // Proceed the matrix cutting for each column of the cut-list
        Image *matrix_list =
                (Image*) malloc(*(cut_list_length+1)*sizeof(Image));
        cut_matrix(img,matrix_list,cut_list,*cut_list_length,0);
        return matrix_list;
    }

    //free useless pointers todo: à faire en dehors de la fonction!
    // free(cut_list_length);
    // free(matrix_list)
}

///todo: commentaires à refaire
Image* cols(char *M, char word, Image input,Image img, size_t *cut_list_length){
    // Creates the histogram whose variables are the number of white pixels
    // on each row of the matrix
    size_t HC[img.width];
    create_histogram(img.width, HC);
    for (size_t col = img.begin_w; col < img.width; ++col) {
        for (size_t line = img.begin_h; line < img.height; ++line) {
            HC[col] += *(M+line*input.width+col)/255;
        }
    }
    size_t s;
    if (word==1){
        s = 0.25*img.width;
    }
    else{
    ///todo: à ajuster
        s = 1;
    }


    // Handles thresholding using the threshold and the full histogram
    size_t *cut_list = thresholding(HC,img.width,s,img.height,cut_list_length);

    // Make a list of cut_areas
    if (cut_list_length==0){
        Image *matrix_list=(Image*)malloc(sizeof(Image));
        *matrix_list=img;
        return matrix_list;
    }
    else{
        for (int i = 0; i < *cut_list_length; ++i) {
            cut_list[i]+=img.begin_h;
        }
        Image *matrix_list =
                (Image*) malloc(*(cut_list_length+1)*sizeof(Image));
        cut_matrix(img,matrix_list,cut_list,*cut_list_length,1);
        return matrix_list;
    }
    //free useless pointers todo: à faire en dehors de la fonction!
    // free(cut_list_length);
    // free(matrix_list)
}