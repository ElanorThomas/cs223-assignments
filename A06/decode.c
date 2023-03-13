/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Mar 1 2023
 * Description: This code enables the user to check for
 * and read a secret message hidden in the LSBs
 * of a ppm file
 * Note: there was no indication about the behavior you
 * wanted for very large files for which part of them
 * had not been encoded with a message. I figured out
 * that my code would just attempt to read the LSBs
 * as if there was a message. Since you didn't indicate
 * that you wanted different behavior, I decided to leave it.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "read_ppm.h"
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: decode <file.ppm>\n");
        return 0;
    }

    int w, h;
    struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
    printf("Reading file %s\n", argv[1]);
    printf("Image size: %d x %d\n", w, h);
    int maxChars = ((w * h) * 3) / 8;
    printf("Max number of characters in this image: %d\n", maxChars);


    unsigned char* pixelsChar = (unsigned char*) pixels;
    int j, subSum = 0, subCount = 7;

    for (j = 0; j < (w * h * 3); j++){
        if (pixelsChar[j] & 0x1ul){
            subSum += (1 << subCount);
            // subSum += pow(2, subCount);
        }
        subCount--;
        if (subCount < 0){
            if (subSum == 0){
                break;
            }
            printf("%c", subSum);
            subCount = 7;
            subSum = 0;
        }
    }
    printf("\n");
    free(pixels);
    return 0;
}

