//
// Created by Paul-alexandre
//

#include "Cutting.h"
#include "Usual_Functions.h"
#include <stdio.h>
#include <stdlib.h>


// Vertical scan of the image (matrix) in order to list the different vertical blocks
// call also the same horizontal path recursively
// until all blocks have the minimum size above the threshold
// recursivity stops when a vertical and horizontal path are negative
// we use the h or v boolean var to know if the last call was negative or not
// return the list of cut matrix
// The input struct is for the initial picture, struct img is the actual image.
Image[] vertical(char*M,Image input,Image img,char h,
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

    // Handles thresholding using the threshold and the full histogram
    /// todo: test si *cut_list_length bien est utilisé pour l'array
    /// après l'appel de thresholding
    size_t cut_list[*cut_list_length] =
            thresholding(HV,img.width,s,img.height,cut_list_length);

    // We make a list of cut areas and if this one is empty we will stop
    // either if the horizontal call is negative either immediately if the
    // last horizontal call was negative
    if (cut_list_length==0 && h==0){
        Image matrix_list[1]={img};
        return matrix_list;
    }
    else{
        if (cut_list_length==0){
            return horizontal(M,img,0,s,cut_list_length);
        }
        else{
        ////////////////////////////////////////////////////////////
/// todo:je fais un +begin.h (et .w sur horizontal) sur chaque membre de
/// cut_list car pour le moment on considere que le begin.h(ou .w) commence a
/// 0, ce qui est faux si on est sur des sous-blocks
            for (int i = 0; i < *cut_list_length; ++i) {
                cut_list[i]+=begin.h;
            }
///////////////////////////////////////////////////////////////////////////
            // Proceed the matrix cutting for each column of the cut-list
            Image matrix_list[cut_list_length+1];
            cut_matrix(M,img,matrix_list,cut_list,cut_list_length,1);

            // Make a recursive horizontal call for each of the new matrix
            size_t *matrix_list2_length=0;

            // The concatenate function modify matrix_list2_length to
            // the new concatenate size
            size_t *cut_Hlist_length;
/// todo: test si *matrix_list2_length bien est utilisé pour l'array
/// après l'appel de concatenate
            Image matrix_list2[*matrix_list2_length]=
                    concatenate(matrix_list2,horizontal
                    (M,matrix_list[pos],1,s,cut_Hlist_length)
                            ,matrix_list2_length,cut_Hlist_length);
            free(cut_Hlist_length);
/// todo: verifier qu'il faut bien un +1 a cut_list_length
            for (int pos = 1; pos < cut_list_length+1; ++pos) {
                size_t *cut_Hlist_length;
/// todo: test si *matrix_list2_length bien est utilisé pour l'array
/// après l'appel de concatenate
                Image matrix_list2[matrix_list2_length]=
                        concatenate(matrix_list2,horizontal
                        (M,matrix_list[pos],1,s,cut_Hlist_length)
                                    ,matrix_list2_length,cut_Hlist_length);
                free(cut_Hlist_length);
            }
            *cut_list_length=*matrix_list2_length;
            free(matrix_list2_length);
            return matrix_list2;
        }
    }

}

// horizontal scan of the image (matrix) in order to list the different
// horizontal blocks
// call also the same vertical path recursively
// until all blocks have the minimum size above the threshold
// recursivity stops when a horizontal and vertical path are negative
// we use the h or v boolean var to know if the last call was negative or not
// return the list of cut matrix
// The input struct is for the initial picture, struct img is the actual image.
Image[] horizontal(char*M,Image input, Image img,char v,
        size_t s,size_t *cut_list_length){
    // Creates the histogram whose variables are the number of white pixels
    // on each row of the matrix
    size_t HH[img.height];
    create_histogram(img.height, HH);
    for (size_t line = img.begin_h; line < img.height+img.begin_h; ++line) {
        for (size_t col = img.begin_w; col < img.width+img.begin_w; ++col) {
            HH[line-img.begin_h] += *(M+line*input.width+col)/255;
        }
    }
    // Handles thresholding using the threshold and the full histogram
    /// todo: test si *cut_list_length bien est utilisé pour l'array après l'appel de thresholding
    size_t cut_list[*cut_list_length] = thresholding(HH,img.height,s,img.width,cut_list_length);

    // We make a list of cut areas and if this one is empty we will stop either if
    // the horizontal call is negative either immediately if the last horizontal call was negative
    if (cut_list_length==0 && v==0){
        Image matrix_list[1]={img};
        return matrix_list;
    }
    else{

        if (cut_list_length==0){
            return vertical(M,img,0,s,cut_list_length);
        }
        else{
            ////////////////////////////////////////////////////////////
/// todo:je fais un +begin.h (et .w sur horizontal) sur chaque membre de
/// cut_list car pour le moment on considere que le begin.h(ou .w) commence a
/// 0, ce qui est faux si on est sur des sous-blocks
            for (int i = 0; i < *cut_list_length; ++i) {
                cut_list[i]+=begin.w;
            }
///////////////////////////////////////////////////////////////////////////

            // Proceed the matrix cutting for each column of the cut-list
            Image matrix_list[cut_list_length+1];
            ///todo:tester si un array se comporte comme une liste python
            cut_matrix(M,img,matrix_list,cut_list,cut_list_length,0);

            // Make a recursive horizontal call for each of the new matrix
            size_t *matrix_list2_length=0;

            // The concatenate function modify matrix_list2_length to the new concatenate size
            size_t *cut_Vlist_length;
            /// todo: test si *matrix_list2_length bien est utilisé pour l'array après l'appel de concatenate
            Image matrix_list2[*matrix_list2_length]=
                    concatenate(matrix_list2,horizontal(M,matrix_list[pos],1,s,cut_Vlist_length)
                            ,matrix_list2_length,cut_Vlist_length);
            free(cut_Vlist_length);
/// todo: verifier qu'il faut bien un +1 a cut_list_length
            for (int pos = 1; pos < cut_list_length+1; ++pos) {
                size_t *cut_Vlist_length;
                /// todo: test si *matrix_list2_length bien est utilisé pour l'array après l'appel de concatenate
                Image matrix_list2[matrix_list2_length]=
                        concatenate(matrix_list2,vertical(M,matrix_list[pos],1,s,cut_Vlist_length)
                                ,matrix_list2_length,cut_Vlist_length);
                free(cut_Vlist_length);
            }
            *cut_list_length=*matrix_list2_length;
            free(matrix_list2_length);
            return matrix_list2;
        }
    }
    //free useless pointers todo: à faire en dehors de la fonction!
    //free(cut_list_length);
}


Image[] lines(char *M, Image input,Image img, size_t
        *cut_list_length){
    // Creates the histogram whose variables are the number of white pixels on each row of the matrix
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
    size_t cut_list[*cut_list_length] = thresholding(HL,img.height,s,img.width,cut_list_length);

    // We make a list of cut areas and if this one is empty we will stop either if
    // the horizontal call is negative either immediately if the last horizontal call was negative
    if (cut_list_length==0){
        Image matrix_list[1]={img};
        return matrix_list;
    }
    else{
        ////////////////////////////////////////////////////////////
/// todo:je fais un +begin.h (et .w sur horizontal) sur chaque membre de
/// cut_list car pour le moment on considere que le begin.h(ou .w) commence a
/// 0, ce qui est faux si on est sur des sous-blocks
        for (int i = 0; i < *cut_list_length; ++i) {
            cut_list[i]+=begin.w;
        }
///////////////////////////////////////////////////////////////////////////

        Image matrix_list[cut_list_length+1];
        ///todo:tester si un array se comporte comme une liste python
        cut_matrix(M,img,matrix_list,cut_list,cut_list_length,0);
        return matrix_list;
    }

    //free useless pointers todo: à faire en dehors de la fonction!
    // free(cut_list_length);
}

///todo: fonction seuil words et chars (pour s)
Image[] cols(char *M, size_t s, Image input,Image img, size_t *cut_list_length){
    // Creates the histogram whose variables are the number of white pixels on each row of the matrix
    size_t HC[img.width];
    create_histogram(img.width, HC);
    for (size_t col = img.begin_w; col < img.width; ++col) {
        for (size_t line = img.begin_h; line < img.height; ++line) {
            HC[col] += *(M+line*input.width+col)/255;
        }
    }

    // Handles thresholding using the threshold and the full histogram
    /// todo: test si *cut_list_length bien est utilisé pour l'array après l'appel de thresholding
    size_t cut_list[*cut_list_length] = thresholding(HC,img.width,s,img.height,cut_list_length);

    // We make a list of cut areas and if this one is empty we will stop either if
    // the horizontal call is negative either immediately if the last horizontal call was negative
    if (cut_list_length==0){
        Image matrix_list[1]={img};
        return matrix_list;
    }
    else{
        ////////////////////////////////////////////////////////////
/// todo:je fais un +begin.h (et .w sur horizontal) sur chaque membre de
/// cut_list car pour le moment on considere que le begin.h(ou .w) commence a
/// 0, ce qui est faux si on est sur des sous-blocks
        for (int i = 0; i < *cut_list_length; ++i) {
            cut_list[i]+=begin.h;
        }
///////////////////////////////////////////////////////////////////////////
        Image matrix_list[cut_list_length+1];
        ///todo:tester si un array se comporte comme une liste python
        cut_matrix(M,img,matrix_list,cut_list,cut_list_length,1);
        return matrix_list;
    }
    //free useless pointers todo: à faire en dehors de la fonction!
    // free(cut_list_length);
}