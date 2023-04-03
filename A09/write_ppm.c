#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
    FILE* filePtr;
    filePtr = fopen(filename, "w+");
    if(filePtr == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(filePtr, "%s\n", "P6");
    fprintf(filePtr, "%s\n", "# Created by Elanor Thomas, CS223 Assignment 08");
    fprintf(filePtr, "%d %d\n", w, h);
    fprintf(filePtr, "%d\n", 255);

    int i, j;
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            fwrite(&pixels[i][j], 3, 1, filePtr);
        }
    }
    fclose(filePtr);
}
