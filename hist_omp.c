/*
    serial program for histogram equalisation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "bmp.h"

#define color_depth 255

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: %s image num_threads\n", argv[0]);
        exit(-1);
    }

    omp_set_num_threads(atoi(argv[2]));

    // read from the input bmp image file
    int image_width, image_height;
    unsigned char* image = read_bmp(argv[1], &image_width, &image_height);

    const int image_size = image_width * image_height;
    unsigned char* output_image = malloc(sizeof(unsigned char) * image_size);

    // create a histogram for the pixel values
    int* histogram = (int*)calloc(sizeof(int), color_depth);
    float* transfer_function = (float*)calloc(sizeof(float), color_depth);

    double start = omp_get_wtime();

#pragma omp parallel
{
    int private_histogram[color_depth] = {0};

    #pragma omp for
    for(int i = 0; i < image_size; i++) {
        private_histogram[image[i]]++;
    }

    #pragma omp critical
    {
        for(int i = 0; i < color_depth; i++) {
            histogram[i] += private_histogram[i];
        }
    }
}

    // finding the normalised values using cumulative mass function
    for(int i = 0; i < color_depth; i++){
        int sum = 0;
        for(int j = 0; j <= i; j++) {
            sum += histogram[j];
        }

        transfer_function[i] = ((float) color_depth * sum) / (image_size);
    }

    #pragma omp parallel for
    for(int i = 0; i < image_size; i++){
        output_image[i] = transfer_function[image[i]];
    }

	double end = omp_get_wtime();

    // write data to output bmp image file
    write_bmp(output_image, image_width, image_height);

    printf("Total: %f\n", end-start);
}
