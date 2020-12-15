//
// Created by Paul-alexandre
//

#include "Usual_Functions.h"


//##############################################################################
//#                       Usual functions for the cutting                      #
//##############################################################################


// Cut the matrix according cut_list depending of length and the test if the
// cut need to be done on vertical or horizontal axis. The function modify the
// matrix_list that is filled with cut matrix.
// The cutting function preserve the reading direction because in occidental
// language, we read from top to bottom and from left to right as the cutting
// function work.
void cut_matrix(Image img,Image *matrix_list,size_t *cut_list
        ,size_t length,Uint8 vertical){
    if (length==0){
        *matrix_list=img;
        return;
    }
    if (vertical==1){

        // Allow to have length+1 matrix from one matrix
        // Create new_img struct that are cut matrix.
        size_t pos = img.begin_h;
        for (size_t posi = 0; posi < length; ++posi) {
            size_t height = *(cut_list+posi)-pos;
            Image new_img = {
                    pos,            // begin_h
                    img.begin_w,    // begin_w
                    height,         // height
                    img.width       // width
            };
            (*(matrix_list+posi)).width=new_img.width;
            (*(matrix_list+posi)).height=new_img.height;
            (*(matrix_list+posi)).begin_w=new_img.begin_w;
            (*(matrix_list+posi)).begin_h=new_img.begin_h;
            pos=*(cut_list+posi);   // move the origin to the next position
        }
        Image new_img = {
                pos,                  // begin_h
                img.begin_w,          // begin_w
                img.height+img.begin_h-pos,// height
                img.width             // width
        };
        (*(matrix_list+length)).width=new_img.width;
        (*(matrix_list+length)).height=new_img.height;
        (*(matrix_list+length)).begin_w=new_img.begin_w;
        (*(matrix_list+length)).begin_h=new_img.begin_h;
    }
    else{
        // Allow to have length+1 matrix from one matrix
        // Create new_img struct that are cut matrix.
        size_t pos = img.begin_w;
        for (size_t posi = 0; posi < length; ++posi) {
            size_t width = *(cut_list+posi)-pos;
            Image new_img = {
                    img.begin_h,    // begin_h
                    pos,            // begin_w
                    img.height,     // height
                    width           // width
            };
            (*(matrix_list+posi)).width=new_img.width;
            (*(matrix_list+posi)).height=new_img.height;
            (*(matrix_list+posi)).begin_w=new_img.begin_w;
            (*(matrix_list+posi)).begin_h=new_img.begin_h;
            pos=*(cut_list+posi);   // move the origin to the next position
        }
        Image new_img = {
                img.begin_h,              // begin_h
                pos,                      // begin_w
                img.height,               // height
                img.width+img.begin_w-pos // width
        };
        (*(matrix_list+length)).width=new_img.width;
        (*(matrix_list+length)).height=new_img.height;
        (*(matrix_list+length)).begin_w=new_img.begin_w;
        (*(matrix_list+length)).begin_h=new_img.begin_h;

    }
}


// Using the threshold and the length of the matrix,
// return a list of rows or columns where we will need to cut.

size_t* thresholding(size_t *H,size_t Hlength,size_t s,
                      size_t length,size_t *cut_list_length){
    size_t *cut_list=malloc(Hlength/s*sizeof(size_t));
    size_t n=0;
    for (size_t index = 0; index < Hlength ; ++index) {
        if (*(H+index) == length){
            // If the index position in the histogram is equal to
            // length, it means that it is a white
            // row or column in the picture, so the counter "n" is
            // incremented by 1.
            n++;
        }
        else{
            if (n>s){
                // If this white part of the histogram is higher than
                // the threshold 's'
                // add the middle of the place where we need to cut in
                // the cut_list
                *(cut_list+*cut_list_length)=(index-(n/2));
                *cut_list_length+=1;
            }
            n=0;
        }
    }
    // Adjust the cut_list matrix to be of length *cut_list_length
    size_t *new_cut_list=(size_t*) malloc(*cut_list_length * sizeof(size_t));
    for(size_t i=0;i<*cut_list_length;++i){
        *(new_cut_list+i)=*(cut_list+i);
    }
    free(cut_list);
    return new_cut_list;
}


// Delete all useless white borders of the given image.
// The Image struct img is a pointer because it is modified in the function.
// It could be better to call it directly from a Cutting.c function with an
// existing Histogram.
void wipe_white_borders(Uint8 *M,Image input,Image *img){
    // Vertical run
    size_t* HV=calloc(img->width,sizeof(size_t));
    for (size_t col = img->begin_w; col < img->width+img->begin_w; ++col) {
        for (size_t line = img->begin_h;
        line < img->height+img->begin_h; ++line) {
            *(HV+col-img->begin_w) += *(M+line*input.width+col)/255;
        }
    }
    size_t width_max=img->width+img->begin_w;

    while (img->begin_w<width_max && *(HV+img->begin_w)==img->height){
        img->begin_w++;
        img->width--;
    }
    if (img->begin_w==width_max)
        return;

    while (width_max>img->begin_w && *(HV+width_max-1)==img->height){
        img->width--;
        width_max--;
    }

    // Horizontal run
    size_t* HH=calloc(img->height,sizeof(size_t));
    for (size_t line = img->begin_h; line < img->height+img->begin_h; ++line) {
        for (size_t col = img->begin_w; col < img->width+img->begin_w; ++col) {
            *(HH+line-img->begin_h) += *(M+line*input.width+col)/255;
        }
    }
    size_t height_max=img->height+img->begin_h;

    while (img->begin_h<height_max && *(HH+img->begin_h)==img->width){
        img->begin_h++;
        img->height--;
    }
    if (img->begin_h==height_max)

        return;

    while (height_max>img->begin_h && *(HH+height_max-1)==img->width){
        img->height--;
        height_max--;
    }

}


// Reshape the given matrix in order to put it in a square matrix.
// It help resize it to the standard 16x16 matrix that is used in neural
// network.
Uint8* square_reshaped(Uint8 *M,Image input, Image img, Uint8
        *new_M, Uint8 vertical){
    if (vertical==1){
        // Add the half of the needed white lines before the given matrix
        // and the other half after.
        Uint8* newM=malloc(img.height*img.height*sizeof(Uint8));
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
        // Add the half of the needed white lines before the given matrix
        // and the other half after.
        Uint8* newM=malloc(img.width*img.width*sizeof(Uint8));
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

// Concatenate two pointers of Image struct, the length of the concatenate
// matrix is stored in M1_length.
void concatenate(Image *M1,Image *M2,size_t *M1_length,size_t *M2_length){
    M1 = (Image *) realloc(M1, (*M1_length+*M2_length) * sizeof
    (Image));


    size_t i=0;
    size_t size_1=*M1_length;
    while(i<*M2_length){
        *(M1+size_1+i)=*(M2+i);
        i++;
    }
    *M1_length+=i;
}

/*
 //doesn't work for width or height >16
Uint8* adjust_matrix_size(Uint8 *M,Image input, Image img, Uint8
*new_M, Uint8 vertical){
    if (img.height<16){
        if (img.width<16){
            if (img.height>img.width){
                square_M = (Uint8 *) malloc(img.height*img.height * sizeof
                        (Uint8));
                square_reshaped(M,input,img,square_M,1);
                img.width=img.height;
            }
            if (img.height<img.width) {
                square_M = (Uint8 *) malloc(img.width * img.width * sizeof
                        (Uint8));
                square_reshaped(M, input, img, square_M, 0);
                img.height = img.width;
            }
            if (img.height==img.width){
                Uint8 *square_M = M;
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
