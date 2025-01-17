/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Mar 1 2023
 * Description: This file allows the programmer to
 * write to a ppm file of their choice
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE* filePtr;
    filePtr = fopen(filename, "w+");
    if(filePtr == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(filePtr, "%s\n", "P6");
    fprintf(filePtr, "%s\n", "# Created by GIMP version 2.10.24 PNM plug-in");
    fprintf(filePtr, "%d %d\n", w, h);
    fprintf(filePtr, "%d\n", 255);

    int i;
    for (i = 0; i < w * h; i++) {
        // printf("( %d, %d, %d )", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
        fwrite(&pixels[i], 3, 1, filePtr);
    }

    fclose(filePtr);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
    FILE* filePtr;
    filePtr = fopen(filename, "w+");
    if(filePtr == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(filePtr, "%s\n", "P6");
    fprintf(filePtr, "%s\n", "# Created by GIMP version 2.10.24 PNM plug-in");
    fprintf(filePtr, "%d %d\n", w, h);
    fprintf(filePtr, "%d\n", 255);

    int i, j;
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            // fread(&image[i][j], sizeof(struct ppm_pixel), 1, filePtr);
            // printf("%c", pixels[i][j].red, );
            printf("( %d, %d, %d )", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);

            fwrite(&pixels[i][j], 3, 1, filePtr);

        }
        // printf("\n");
    }

    fclose(filePtr);
}

