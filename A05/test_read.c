/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Feb 22 2023
 * Description test file for read_ppm.c
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
    int w, h;
    struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

    // todo: print out the grid of pixels
    int i, j;
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            printf("( %d, %d, %d )", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
        }
        printf("\n");
    }

    for (i = 0; i < h; i++) {
        free(pixels[i]);
    }
    free(pixels);
    return 0;
}

