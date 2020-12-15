//
// Created by Paul-alexandre
//


#include "Usual_Functions.h"
#include "Cutting.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <err.h>


Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}


void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *) p = pixel;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *) p = pixel;
            break;
    }
}

Uint8* FromBMP_to_Pointers(char* path,Image *output){
    SDL_Surface* img = SDL_LoadBMP(path);
    Uint8 r, g ,b;
    Uint32 pixel;
    output->begin_h=0;
    output->begin_w=0;
    output->height=(size_t)img->h;
    output->width=(size_t)img->w;
    Uint8* M = (Uint8 *) malloc(output->height*output->width*sizeof(Uint8));
    for (size_t height = 0; height < output->height; ++height) {
        for (size_t width = 0; width < output->width; ++width) {
            pixel = get_pixel(img, width, height);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            *(M+height*(output->width)+width)=r;
        }
    }
    SDL_FreeSurface(img);
    return M;
}

void FromPointers_to_BMP(Uint8* M,Image *img,char* path){
    SDL_Surface *output_img =
            SDL_CreateRGBSurface(0, img->height, img->width, 32, 0, 0, 0, 0);
    Uint8 r, g ,b;
    Uint32 pixel;
    for (size_t height = 0; height < img->height; ++height) {
        for (size_t width = 0; width < img->width; ++width) {
            r=*(M+height*img->width+width);
            g=r;
            b=r;
            pixel = SDL_MapRGB(output_img->format, r, g, b);
            put_pixel(output_img,width,height,pixel);
        }
    }
    SDL_FreeSurface(output_img);
    SDL_SaveBMP(output_img,path);
}

Uint8* FromImage_to_Pointer(Uint8* M,Image input,Image img){
    Uint8 *new_M=(Uint8*)malloc(img.width*img.height*sizeof(Uint8));
    for (size_t height = img.begin_h; height < img.height+img.begin_h;
    ++height) {
        for (size_t width = img.begin_w; width < img.width+img.width;width++){
            *(new_M+height*img.width+width)=
                    *(M+(img.begin_h+height)*input.width+img.begin_w+width);
        }
    }
    return new_M;
}

int main ( int argc , char * argv[] ) {

    if (argc!=2){
        errx(1,"invalid argument: type the path of the image you want to "
                 "cut");

    }

    // Open a file in order to write in it.
    FILE * fp;
    fp = fopen ("results/result.txt","w");

    // Init the Image struct, open the picture we test and convert it in
    // one-line matrix representation, modify the Image too.
    Image img;
    Uint8* M=FromBMP_to_Pointers(argv[1],&img);

    // Init the variable that represent the length of the return pointer in
    // lines() and cols() function.
    size_t *cut_list_length=(size_t*)malloc(sizeof(size_t));
    size_t *line_length=(size_t*)malloc(sizeof(size_t));
    size_t *word_length=(size_t*)malloc(sizeof(size_t));
    *cut_list_length=0;

    wipe_white_borders(M,img,&img);
    // Call the lines() function that cut the input image in multiple lines.
    Image *new_lines=lines(M,img,img,cut_list_length);

    // For each line.
    for (size_t j = 0;j<*cut_list_length;j++){
        *line_length=0;
        // Call the cols() on line_matrix to cut in word_matrix.
        wipe_white_borders(M,img,&(*(new_lines+j)));
        Image *new_words=cols(M,1,img,*(new_lines+j),line_length);

        // For each word.
        for (size_t k = 0;k<*line_length;k++) {
            *word_length=0;
            // Call the cols() on word_matrix to cut in char_matrix.
            wipe_white_borders(M,img,&(*(new_words+k)));
            Image *new_chars=cols(M,0,img,*(new_words+k),word_length);

            // For each char.
            for (int l = 0; l < *word_length; ++l) {
                wipe_white_borders(M,img,&(*(new_chars+l)));

                //transform Image struct into size_t pointer from 16*16 size
                Uint8 *M_chars0 = FromImage_to_Pointer(M,img,*(new_chars+l));
                Uint8 *M_chars;
                if ((*(new_chars+l)).height>(*(new_chars+l)).width) {
                    Uint8 *M_chars = square_reshaped(M, img, (*(new_chars+l)),
                    M_chars0, 1);
                }
                else{
                    if ((*(new_chars+l)).height < (*(new_chars+l)).width){
                        Uint8 *M_chars = square_reshaped(M, img,
                        (*(new_chars+l)),M_chars0, 0);
                    }
                }
                /// Adjust_matrix(M,img,M_chars,(*(new_chars+l)));

                // Call the neural network on char_matrix and return a char.
                ///char str=neural_network(M_chars);

                // Write the char on the return file.
                ///fprintf (fp,"%c",str);

                // Free useless pointer from fromImage_to_Pointer function.
                free(M_chars);
                free(M_chars0);
            }
            // Print an space on file in order to separate words (not done on
            // last word of the line).
            if (k!=*word_length-1)
                fprintf (fp," ");
            // Free useless pointer from last cols() for char function.

            free(new_chars);

        }
        // Print a new_line char in order to separate lines.
        fprintf (fp,"\n");
        // Free useless pointer from last cols() for word function.
        free(new_words);
    }
    // Close the file.
    fclose (fp);

    // Free all remaining pointers.
    free(M);
    free(cut_list_length);
    free(line_length);
    free(word_length);
    free(new_lines);
    return 0 ;
}
