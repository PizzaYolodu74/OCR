//
// Created by Paul-alexandre
//

#include "Usual_Functions.h"
#include <stdio.h>
//##############################################################################
//#                       Usual fonctions for the cutting                      #
//##############################################################################


// Cut the matrix according to a height i or a width j the cutting function
// preserve the reading direction because in occidental language, we read
// top to bottom and left to right as the cutting function work


void cut_matrix(char* M,size_t i=0,size_t j=0,size_t length,size_t width){ //verifier si size_t i = 0 fonctionne pour les tests
    L=malloc(2*sizeof(char));
    size_t sizeM = sizeof(M)/sizeof(M[0]);
    if (i){
        // Return two matrix with i lines and len(M)-i lines
        // from one matrix
        char M0[i];
        char M1[sizeM-i];
        for (size_t line = 0; line < i; ++line) {
            M0[line]=M[line]
        }
        for (size_t line = i; line < sizeM; ++line) {
            M1[sizeM - line] = M[sizeM - line];
        }
        L[0]=M0;
        L[1]=M1;
    }
    else{
        if (j){
            // Return two matrix with j columns and len(M[0])-j
            // columns from one matrix
            size_t sizeM_0 = sizeof(M)/sizeof(M[0]);
            char M2[size_M][j];
            char M3[sizeM][sizeM_0-j];
            for (size_t line = 0; line < sizeM; ++line) {
                for (size_t col = 0; col < j; ++col) {
                    M2[line]=M[line][col];
                }
            }
            for (size_t line = 0; line < sizeM; ++line) {
                for (size_t col = j; col < sizeM_0; ++col) {
                    M3[line]=M[line][col];
                }
            }
            L[0]=M2;
            L[1]=M3;
        }
        else{
            if (i == 0 || j == 0 || i == sizeM || j == sizeM){
                return M;
            }
        }
    }
    return L;
}

// Create an empty histogram (initialized at 0)
int[] create_histogram(size_t nb,int H[]){
    for (size_t i = 0; i < nb; ++i) {
        H[i]=0;
    }
    return H;
}


// Using the threshold and the length of the matrix,
// return a list of row(or columns) where we will need to cut

int* thresholding(int H[],int s, size_t l){
    // Return where we need to cut in function of the threshold
    int n = 0;
    int *L;
    size_t index=0;
    size_t sizeH=sizeof(H)/sizeof(H[0]);
    for (size_t i = 0; i < sizeH ; ++i) {
        if (H[i] == l){
            n+=1;
        }
        else{
            if (n>s){
                L[index]=(i-(n/2));
                index++;
            }
            n=0;
        }
    }
    return L;
}
/* Je skip cette partie pour le moment

char* resize(char *M,size_t left,size_t right,size_t l,size_t left0,size_t right0,size_t l0){
    char newM[right-left];
    for (int i = left; i < right; ++i) {
        newM[i-left]=M[i];
    }
}

// Remove vertical white borders from the matrix by using an histogram
// and searching white column at the begin or the ending of the matrix
int wipe_vertical(char *M){
    size_t l=sizeof(M)/sizeof(M[0]);
    size_t l0=sizeof(M[0])/sizeof(M[0][0]);
    int HV[lo];
    create_histogram(lo,HV);
    for (int col = 0; col < l0; ++col) {
        for (int line = 0; line < l; ++line) {
            HV[col] += (M[line][col]/255)
        }
    }
    i=0;
    l0max = l0;
    //Search white column at the beginning
    while (i < l0max && HV[i] == l){
        i ++;
        l0 --;

    }
}

// Remove horizontal white borders from the matrix by using an histogram
// and searching white row at the begin or the ending of the matrix
int wipe_horizontal(char *M){
    size_t l=sizeof(M)/sizeof(M[0]);
    size_t l0=sizeof(M[0])/sizeof(M[0][0]);
}


// Remove white borders from the matrix
void wipe_white_borders(char *M){
    wipe_vertical(M);
    wipe_horizontal(M);
}


*/

char* square_reshaped(char *M){
    size_t l=sizeof(M)/sizeof(M[0]);
    size_t l0=sizeof(M[0])/sizeof(M[0][0]);

    if (l>l0){
        char newM[l][l0];
        for (int i = 0; i < l; ++i) {
            for (int j = 0; j < l0; ++j) {
                newM[i][j]=M[i][j];
            }
            for (int j = l0; j < l; ++j) {
                newM[i][j]=255;
            }
        }
    }
    else{
        if (l<l0){
            char newM[l][l0];
            for (int i = 0; i < l; ++i) {
                for (int j = 0; j < l0; ++j) {
                    newM[i][j]=M[i][j];
                }
            }
            for (int i = l; i < l0; ++i) {
                for (int j = 0; j < l0; ++j) {
                    newM[i][j]=255;
                }
            }
        }
        else{
            return M;
        }
    }
    return newM;
}

char* adjust_matrix_size(char *M){
    size_t l=sizeof(M)/sizeof(M[0]);
    size_t l0=sizeof(M[0])/sizeof(M[0][0]);
    if (l>l0){

    }
    if (l<l0){

    }


}


















