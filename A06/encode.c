/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Mar 1 2023
 * Description: This code enables the user to encode
 * a secret message  in the LSBs of a ppm file
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: encode <file.ppm>\n");
        return 0;
    }

    char newFileName[1024];
    int b;
    for (b = 0; b < strlen(argv[1]) - 4; b++){
        newFileName[b] = argv[1][b];
    }
    newFileName[b] = '\0';
    strcat(newFileName, "-encoded.ppm");

    int w, h;
    struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
    printf("Reading file %s\n", argv[1]);
    printf("Image size: %d x %d\n", w, h);
    int maxChars = ((w * h) * 3) / 8;
    printf("Max number of characters in this image: %d\n", maxChars);

    char buff[1024];
    printf("Enter a message: ");
    scanf(" %s", buff);//instead of %31s - ALSO i believe this contains the null character

    
    printf("Writing file %s\n", newFileName);
    
    int len = strlen(buff);
    // printf("YEE%d", len);
    short* bits = (short*) malloc((len + 1) * 8 * sizeof(short));
    int s;
    for (s = 8; s > 0; s--){
        bits[((len + 1) * 8) - s] = 0;
    }
    
    int count  = 0;
    
    //This is to make the array of bits, called bits, to represent the message given
    int i;
    for (i = 0; i < len; i++){
        int givChar = buff[i];
        int j;
        for (j = 7; j >= 0; j--){
            int givBit = (int)(givChar / (1 << j));
            givChar -= (givBit * (1 << j));
            bits[count] = givBit;
            count++;
        }
    }

    int t;
    int bitCount = 0;
    int totalBits = (len + 1) * 8;

    for (t = 0; t < w * h; t++){//this is for looping through the pixels
    // for (t = 0; t < len * 8; t++){
        //doing len * 8 instead of w * h should have the same effect as only writing the number of bits necessary
        if (bitCount < totalBits){
            if (bits[bitCount] == 0){//if the value is 0, clear the last bit
                pixels[t].red &= ~(0x1ul);
            } else { //otherwise, set the last bit
                pixels[t].red |= 0x1ul;
            }
            bitCount++;
        } else {
            break;
        }

        if (bitCount < totalBits){
            if (bits[bitCount] == 0){
                pixels[t].green &= ~(0x1ul);
            } else {
                pixels[t].green |= 0x1ul;
            }
            bitCount++;
        } else {
            break;
        }

        if (bitCount < totalBits){
            if (bits[bitCount] == 0){
                pixels[t].blue &= ~(0x1ul);
            } else {
                pixels[t].blue |= 0x1ul;
            }
            bitCount++;
        } else {
            break;
        }
        
    }

    for (int a = 0; a < (len + 1) * 8; a++){
        printf("%d, ", bits[a]);
    }

    // for (i = 0; i < w*h; i++) {
    //     if (!(i % h)){
    //         printf("\n");
    //     }
    //     printf("( %d, %d, %d )", pixels[i].red, pixels[i].green, pixels[i].blue);
    // }

    write_ppm(newFileName, pixels, w, h);

    free(bits);
    free(pixels);
    return 0;
}

