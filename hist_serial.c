/*
    serial program for histogram equalisation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

int main(int argc, char** argv){

    if(argc != 4){
        printf("Usage: %s image width height\n", argv[0]);
        exit(-1);
    }

    const int image_width = atoi(argv[2]);
    const int image_height = atoi(argv[3]);
    const int image_size = image_width * image_height;
    const int color_depth = 255;

    // read from the input bmp image file
    unsigned char* image = read_bmp(argv[1]);
    unsigned char* output_image = malloc(sizeof(unsigned char) * image_size);

    // create a histogram for the pixel values
    int* histogram = (int*)calloc(sizeof(int), color_depth);
    for(int i = 0; i < image_size; i++){
        histogram[image[i]]++;
    }

    // finding the normalised values using cumulative mass function
    float* transfer_function = (float*)calloc(sizeof(float), color_depth);
    for(int i = 0; i < color_depth; i++){
        int sum = 0;
        for(int j = 0; j <= i; j++) {
            sum += histogram[j];
        }

        transfer_function[i] = ((float) color_depth * sum) / (image_size);
    }

    for(int i = 0; i < image_size; i++){
        output_image[i] = transfer_function[image[i]];
    }

    // write data to output bmp image file
    write_bmp(output_image, image_width, image_height);
}
