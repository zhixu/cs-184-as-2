#include "film.h"
#include "math.h"
#include <stdlib.h>
#include <png.h>
#include <vector>

Film::Film (int w, int h) {
    /*
     * colors array is indexed by colors[y][x] (not [x][y])
     * where x=0,y=0 is lower left corner
     * and x=width,y=height is top right corner
     */
    width = w;
    height = h;

    colors.resize(height);
    int x, y;
    for(y=0; y<height; y++){
        colors[y].resize(width, NULL);

        for(x=0; x<width; x++){
           colors[y][x] = new Color(0,0,0);
        }
    }
}

void Film::commit(int x, int y, Color color){
    
    x = floor(x + 0.5);
    y = floor(y + 0.5);
    
    *colors[y][x] += color;
}

void Film::write(char* filename){
    FILE *fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    fp = fopen(filename, "wb");
    if(fp == NULL){
        fprintf(stderr, "Could not open file %s for writing\n", filename);
        goto finalize;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL){
        fprintf(stderr, "Could not allocate write struct\n");
        goto finalize;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL){
        fprintf(stderr, "Could not allocate info struct\n");
        goto finalize;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error during png creation\n");
        goto finalize;
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    row = (png_bytep) malloc(3 * width * sizeof(png_byte));
    int x, y;
    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            
            float r = colors[y][x]->r * 255.0;
            float g = colors[y][x]->g * 255.0;
            float b = colors[y][x]->b * 255.0;
            
            row[(3 * x) + 0] = r;
            row[(3 * x) + 1] = g;
            row[(3 * x) + 2] = b;

        }
        png_write_row(png_ptr, row);
    }

    finalize:
    if (fp != NULL) {
        fclose(fp);
    }
    if(info_ptr != NULL){
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    }
    if(png_ptr != NULL) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    }
    if(row != NULL){
        free(row);
    }
}
