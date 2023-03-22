/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Mar 21 2023
 * Description: This file contains the main method of
 * a mandelbrot set visualization program. This program
 * writes a new file, with a name including the current time
 * in order to differentiate file names. It also takes in
 * size and minimum and maximum x and y values, should 
 * the use choose to include them
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>

int main(int argc, char* argv[]) {
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int maxIterations = 1000;   
    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
        }
    }
    printf("Generating mandelbrot with size %dx%d\n", size, size);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    srand(time(0)); 
    
    struct ppm_pixel** mandelImage = (struct ppm_pixel**)malloc(size * sizeof(struct ppm_pixel*));
    if(mandelImage == NULL){
        printf("Error. Allocation was unsuccessful. \n");
        return -1;
    }
    int s;
    for (s = 0; s < size; s++){
        mandelImage[s] = (struct ppm_pixel*)malloc(size * sizeof(struct ppm_pixel));
        if(mandelImage[s] == NULL){
            printf("Error. Allocation was unsuccessful. \n");
            return -1;
        }
    }

 
    // todo: your work here
    // generate pallet
    struct ppm_pixel* colorPalette = (struct ppm_pixel*)malloc(maxIterations * sizeof(struct ppm_pixel));

    //PALETTE OPTION 1: RANDOM
    // int a;
    // for (a = 0; a < maxIterations; a++){
    //     colorPalette[a].red = rand() % 255;
    //     colorPalette[a].green = rand() % 255;
    //     colorPalette[a].blue = rand() % 255;
    // }

    // PALETTE OPTION 2: JITTERED COLORS
    int baseRed = rand() % 255;
    int baseGreen = rand() % 255;
    int baseBlue = rand() % 255;
    int a;
    for (a = 0; a < maxIterations; a++){
        colorPalette[a].red = baseRed + rand() % 100 - 50;
        colorPalette[a].green = baseGreen + rand() % 100 - 50;
        colorPalette[a].blue = baseBlue + rand() % 100 - 50;
    }

    //TIMING STARTS HERE
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    int i, j;
    for (i = 0; i < size; i++) {//for each row
        for (j = 0; j < size; j++) {//for each column
            float xfrac = (float)i / (float)size;
            
            float yfrac = (float)j / (float)size;
            float x0 = xmin + xfrac * (xmax - xmin);
            float y0 = ymin + yfrac * (ymax - ymin);

            float x = 0;
            float y = 0;
            int iter = 0;
            while (iter < maxIterations && x*x + y*y < 4){
                float xtmp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtmp;
                iter++;
            }

            if (iter < maxIterations){
                // printf("%d ", iter);//SHOULD THIS BE [j][i] or [i][j]
                mandelImage[j][i].red = colorPalette[iter].red;
                mandelImage[j][i].green = colorPalette[iter].green;
                mandelImage[j][i].blue = colorPalette[iter].blue;
            } else {
                mandelImage[j][i].red = 0;
                mandelImage[j][i].green = 0;
                mandelImage[j][i].blue = 0;

            }
        }
    }
    char newFileName[256];
    snprintf(newFileName, 50, "mandelbrot-<%d>-<%ld>.ppm", size, time(0));
    write_ppm_2d(newFileName, mandelImage, size, size);

    //TIMING ENDS HERE
    //since the image has now been generated and written
    
    gettimeofday(&tend, NULL);
    double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Computed mandelbrot set (%dx%d) in %g seconds.\n", size, size, timer);


    free(colorPalette);
    int t;
    for (t = 0; t < size; t++){
        free(mandelImage[t]);
    }
    free(mandelImage);

}
