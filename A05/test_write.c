/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Feb 22 2023
 * Description: test file for write_ppm.c
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    //____TEST FOR 2D WRITE___________________________

    // int w, h;
    // struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);
    // write_ppm_2d("test.ppm", pixels, w, h);

    // int w2, h2;
    // struct ppm_pixel** pixels2 = read_ppm_2d("test.ppm", &w2, &h2);


    // // test writing the file to test.ppm, reload it, and print the contents
    // int i, j;
    // for (i = 0; i < w; i++) {
    //     for (j = 0; j < h; j++) {
    //         printf("( %d, %d, %d )", pixels2[i][j].red, pixels2[i][j].green, pixels2[i][j].blue);
    //     }
    //     printf("\n");
    // }

    // for (i = 0; i < h; i++) {
    //     free(pixels2[i]);
    // }
    // free(pixels2);
    // return 0;



    //TEST FOR 1D WRITE________________________
    int w, h;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
    write_ppm("test.ppm", pixels, w, h);

    int w2, h2;
    struct ppm_pixel* pixels2 = read_ppm("test.ppm", &w2, &h2);

    int i;
    for (i = 0; i < w * h; i++) {
        if (!(i % h)){
            printf("\n");
        }
        printf("( %d, %d, %d )", pixels2[i].red, pixels2[i].green, pixels2[i].blue);
    }

    free(pixels2);
    return 0;
}
