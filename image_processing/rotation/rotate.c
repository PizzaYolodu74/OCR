//
// Created by Paul-alexandre
//

#include "rotate.h"
#include "bilinear_interpolation.h"



// Rotate the picture using bilinear interpolation taking an angle in degree.
SDL_Surface* rotate (SDL_Surface* img, int height, int width,int degree_angle) {
    // Center of the image.
    int height_center = floor(height / 2);
    int width_center = floor(width / 2);

    // Loop through each pixel of the new image, select the new vertical
    // and horizontal positions, and interpolate the image to make the change.
    SDL_Surface *output_img;
    if (height > width){
        output_img = SDL_CreateRGBSurface(0, height, height, 32, 0, 0, 0, 0);
    }
    else{
        output_img = SDL_CreateRGBSurface(0, width, width, 32, 0, 0, 0, 0);
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Convert degree angle to radian angle.
            // Take the new position of the pixel.
            double angle = degree_angle * (double) acos(-1) / 180;//acos(-1)=Pi
            float vertical_position = (float) cos(angle) *
                    (i - height_center) + sin(angle) * (j - width_center)
                                      + height_center;
            float horizontal_position = (float) -sin(angle) *
                    (i - height_center) + cos(angle) * (j - width_center)
                                        + width_center;

            // Take the four locations and pixels that we want to interpolate
            // from the input image
            int top = floor(vertical_position);
            int bottom = top + 1;
            int left = floor(horizontal_position);
            int right = left + 1;

            Uint32 pixel = get_pixel(img, i, j);
            Uint8 r, g ,b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            // If one of the location are invalide, skip the
            // interpolation, otherwise. Otherwise, interpolate the pixel.
            // The resulting pixel is set in the SDL_Surface
            if (top >= 0 && bottom < height && left >= 0 && right < width ) {
                float interpolated = bilinearly_interpolate
                (top, bottom,left, right, horizontal_position,
                vertical_position,img);

                // Set rgb value of the new pixel in the output img
                if (fmod(interpolated,1.0)> 0.5 )
                    r=floor(interpolated)+1;
                else
                    r=floor(interpolated);
                g=r;
                b=r;

                pixel = SDL_MapRGB(output_img->format, r, g, b);
                put_pixel(output_img,i,j,pixel);
            }
        }
    }
    return output_img;
}
