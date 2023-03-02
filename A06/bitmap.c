/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Mar 1 2023
 * Description: This file reads a file with hex,
 * converts it into binary, and displays it as
 * an image of black and white pixels
 ---------------------------------------------*/
#include <stdio.h>

int main() {
    unsigned long img;
    FILE* filePtr;
    filePtr = fopen("bitmap3.txt", "r");//deref fileName?
    if(filePtr == NULL) {
        perror("Error opening file");
        return 0;
    }
    fscanf(filePtr, " %lx", &img);
    printf("Image (unsigned long): %lx\n", img);
    int i;
    for (i = 63; i >= 0; i--){
        unsigned long int bitmask = 0x1ul << i;
        // printf("Image (unsigned long): %lx\n", bitmask);
        // short givBit = img & bitmask;
        if (img & bitmask){
            printf("@ ");
        }  else {
            printf("  ");
        }
        if ((i)%8 == 0){
            printf("\n");
        }
    }
    fclose(filePtr);
    
    // todo: your code here
    return 0;
}
