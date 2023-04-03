#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct mandelData {
    int threadID;

    int sizeVar;
    float xminVar;
    float xmaxVar;
    float yminVar;
    float ymaxVar;
    int maxIterationsVar; 

    int startIdxI;
    int endIdxI;
    int startIdxJ;
    int endIdxJ;

    struct ppm_pixel* colorPaletteVar;
    struct ppm_pixel** mandelImageVar;
};

void *mandelThread(void *mandelDataObj){
    struct mandelData *mdo = (struct mandelData *) mandelDataObj;
    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d).\n", mdo->threadID, mdo->startIdxI, mdo->endIdxI, mdo->startIdxJ, mdo->endIdxJ);

    int i, j;
    for (i = mdo->startIdxI; i < mdo->endIdxI; i++) {//for each row
        for (j = mdo->startIdxJ; j < mdo->endIdxJ; j++) {//for each column
            float xfrac = (float)i / (float) mdo->sizeVar;
            float yfrac = (float)j / (float) mdo->sizeVar;
            float x0 = mdo->xminVar + xfrac * (mdo->xmaxVar - mdo->xminVar);
            float y0 = mdo->yminVar + yfrac * (mdo->ymaxVar - mdo->yminVar);

            float x = 0;
            float y = 0;
            int iter = 0;
            while (iter < mdo->maxIterationsVar && x*x + y*y < 4){
                float xtmp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtmp;
                iter++;
            }

            if (iter < mdo->maxIterationsVar){
                mdo->mandelImageVar[j][i].red = mdo->colorPaletteVar[iter].red;
                mdo->mandelImageVar[j][i].green = mdo->colorPaletteVar[iter].green;
                mdo->mandelImageVar[j][i].blue = mdo->colorPaletteVar[iter].blue;
            } else {
                mdo->mandelImageVar[j][i].red = 0;
                mdo->mandelImageVar[j][i].green = 0;
                mdo->mandelImageVar[j][i].blue = 0;

            }
        }
    }
    return NULL;
}


int main(int argc, char* argv[]) {
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int maxIterations = 1000;
    int numProcesses = 4;

    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
              "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
        }
    }
    printf("Generating mandelbrot with size %dx%d\n", size, size);
    printf("  Num processes = %d\n", numProcesses);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    // todo: your code here
    // generate pallet
    srand(time(0)); 
    struct ppm_pixel* colorPalette = (struct ppm_pixel*)malloc(maxIterations * sizeof(struct ppm_pixel));

    //PALETTE OPTION 1: RANDOM
    // for (int a = 0; a < maxIterations; a++){
    //     colorPalette[a].red = rand() % 255;
    //     colorPalette[a].green = rand() % 255;
    //     colorPalette[a].blue = rand() % 255;
    // }

    // PALETTE OPTION 2: JITTERED COLORS
    int baseRed = rand() % 255;
    int baseGreen = rand() % 255;
    int baseBlue = rand() % 255;
    for (int a = 0; a < maxIterations; a++){
        colorPalette[a].red = baseRed + rand() % 100 - 50;
        colorPalette[a].green = baseGreen + rand() % 100 - 50;
        colorPalette[a].blue = baseBlue + rand() % 100 - 50;
    }


    //____________ ALLOCATE IMAGE ARRAY
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


    //TIMING STARTS HERE
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);


    struct mandelData mdoArr[4];

    for (int s = 0; s < 4; s++){
        mdoArr[s].sizeVar = size;
        mdoArr[s].xmaxVar = xmax;
        mdoArr[s].xminVar = xmin;
        mdoArr[s].yminVar = ymin;
        mdoArr[s].ymaxVar = ymax;
        mdoArr[s].maxIterationsVar = maxIterations;

        mdoArr[s].colorPaletteVar = colorPalette;
        mdoArr[s].mandelImageVar = mandelImage;
    }

    mdoArr[0].startIdxI = 0;
    mdoArr[0].endIdxI = size / 2;
    mdoArr[0].startIdxJ = 0;
    mdoArr[0].endIdxJ = size / 2;

    mdoArr[1].startIdxI = 0;
    mdoArr[1].endIdxI = size / 2;
    mdoArr[1].startIdxJ = size / 2;
    mdoArr[1].endIdxJ = size;

    mdoArr[2].startIdxI = size / 2;
    mdoArr[2].startIdxJ = 0;
    mdoArr[2].endIdxI = size;
    mdoArr[2].endIdxJ = size / 2;

    mdoArr[3].startIdxI = size / 2;
    mdoArr[3].startIdxJ = size / 2;
    mdoArr[3].endIdxI = size;
    mdoArr[3].endIdxJ = size;
    
    pthread_t threadArr[4];
    for (int threadCount = 0; threadCount < 4; threadCount++){
        mdoArr[threadCount].threadID = threadCount + 1;
        pthread_create(&threadArr[threadCount], NULL, mandelThread, &mdoArr[threadCount]);
    }

    for (int t = 0; t < 4; t++){
        pthread_join(threadArr[t], NULL);
        printf("Thread %d finished.\n", t + 1);
    }

    // int a, b;
    // for (a = 0; a < 5; a++){
    //     for (b = 0; b < 5; b++){
    //         printf("<%d, %d, %d>, ", mdoArr[0].mandelImageVar[a][b].red, mdoArr[0].mandelImageVar[a][b].green, mdoArr[0].mandelImageVar[a][b].blue);
    //     }
    // }

    // printf("\nDIFONE\n");

    // for (a = 0; a < 5; a++){
    //     for (b = 0; b < 5; b++){
    //         printf("<%d, %d, %d>, ", mdoArr[1].mandelImageVar[a][b].red, mdoArr[1].mandelImageVar[a][b].green, mdoArr[1].mandelImageVar[a][b].blue);
    //     }
    // }



    char newFileName[256];
    snprintf(newFileName, 50, "mandelbrot-mThreaded-<%d>-<%ld>.ppm", size, time(0));
    write_ppm_2d(newFileName, mdoArr[0].mandelImageVar, size, size);

    //TIMING ENDS HERE
    //since the image has now been generated and written
    gettimeofday(&tend, NULL);
    double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Computed mandelbrot set (%dx%d) in %g seconds.\n", size, size, timer);

    printf("Writing file: %s\n", newFileName);

    free(colorPalette);
    int t;
    for (t = 0; t < size; t++){
        free(mandelImage[t]);
    }
    free(mandelImage);
}
