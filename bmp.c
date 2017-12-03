/*
    C program to read from and write to a bmp image file.
*/

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

// method to write data to the bmp file
void write_bmp(unsigned char* data, int width, int height){
    struct bmp_id id;
    id.magic1 = 0x42;
    id.magic2 = 0x4D;

    struct bmp_header header;
    header.file_size = width*height+54 + 2;
    header.pixel_offset = 1078;

    struct bmp_dib_header dib_header;
    dib_header.header_size = 40;
    dib_header.width = width;
    dib_header.height = height;
    dib_header.num_planes = 1;
    dib_header.bit_pr_pixel = 8;
    dib_header.compress_type = 0;
    dib_header.data_size = width*height;
    dib_header.hres = 0;
    dib_header.vres = 0;
    dib_header.num_colors = 256;
    dib_header.num_imp_colors = 0;

    char padding[2];

    unsigned char* color_table = (unsigned char*)malloc(1024);
    for(int c= 0; c < 256; c++){
        color_table[c*4] = (unsigned char) c;
        color_table[c*4+1] = (unsigned char) c;
        color_table[c*4+2] = (unsigned char) c;
        color_table[c*4+3] = 0;
    }

    // writing header and image data into the bmp image file
    FILE* fp = fopen("out.bmp", "w+");
    fwrite((void*)&id, 1, 2, fp);
    fwrite((void*)&header, 1, 12, fp);
    fwrite((void*)&dib_header, 1, 40, fp);
    fwrite((void*)color_table, 1, 1024, fp);
    fwrite((void*)data, 1, width*height, fp);
    fwrite((void*)&padding,1,2,fp);
    fclose(fp);
}

//method to read data from a bmp image file
unsigned char* read_bmp(char* filename, int* rwidth, int* rheight){

    FILE* fp = fopen(filename, "rb");

    int width, height, offset;

    // read height, width and offset from the file
    fseek(fp, 18, SEEK_SET);
    fread(&width, 4, 1, fp);
    fseek(fp, 22, SEEK_SET);
    fread(&height, 4, 1, fp);
    fseek(fp, 10, SEEK_SET);
    fread(&offset, 4, 1, fp);

    // read data from the file
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*height*width);

    fseek(fp, offset, SEEK_SET);
    
    // ignore the padding
    fread(data, sizeof(unsigned char), height*width, fp);

    fclose(fp);

    *rwidth = width;
    *rheight = height;
    return data;
}

