/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Mar 1 2023
 * Description: This file allows the programmer to
 * read a given ppm file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
    FILE* filePtr;
    filePtr = fopen(filename, "r");
    if(filePtr == NULL) {
        perror("Error opening file");
        return NULL;
    }
    char lineRead[1024];

    if (!fgets(lineRead, sizeof(lineRead), filePtr)) {//filetype or image format
        perror(filename);
        exit(1);
    }

    if (lineRead[0] != 'P' || lineRead[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    fgets(lineRead, 1024, filePtr);

    fscanf(filePtr, " %d%*c", w);//width and height
    fscanf(filePtr, " %d%*c", h);

    fgets(lineRead, 60, filePtr);//im assuming this is 255

    struct ppm_pixel* image = (struct ppm_pixel*)malloc((*w)* (*h) * sizeof(struct ppm_pixel));
    if(image == NULL){
        printf("Error. Allocation was unsuccessful. \n");
        return NULL;
    }
    int i;
    for (i = 0; i < (*w) * (*h); i++) {
        fread(&image[i], 3, 1, filePtr);
    }

    fclose(filePtr);
    return image;
}


struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
    FILE* filePtr;
    filePtr = fopen(filename, "r");
    if(filePtr == NULL) {
        perror("Error opening file");
        return NULL;
    }
    char lineRead[1024];

    if (!fgets(lineRead, sizeof(lineRead), filePtr)) {//filetype or image format
        perror(filename);
        exit(1);
    }

    if (lineRead[0] != 'P' || lineRead[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    fgets(lineRead, 1024, filePtr);


    fscanf(filePtr, " %d%*c", w);//width and height
    fscanf(filePtr, " %d%*c", h);

    fgets(lineRead, 60, filePtr);//im assuming this is 255

    struct ppm_pixel** image = (struct ppm_pixel**)malloc(*w * sizeof(struct ppm_pixel*));
    if(image == NULL){
        printf("Error. Allocation was unsuccessful. \n");
        return NULL;
    }
    //_____________
    int i;
    for (i = 0; i < *w; i++){
        image[i] = (struct ppm_pixel*)malloc(*h * sizeof(struct ppm_pixel));
        if(image[i] == NULL){
            printf("Error. Allocation was unsuccessful. \n");
            return NULL;
        }
    }


    int j;
    for (i = 0; i < *w; i++) {
        for (j = 0; j < *h; j++) {
            fread(&image[i][j], 3, 1, filePtr);
        }
    }
    fclose(filePtr);
    return image;
    return NULL;
}
