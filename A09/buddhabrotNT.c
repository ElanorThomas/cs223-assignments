#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

static pthread_barrier_t barrier;

struct buddhabrotData {
    int threadID;
    int maxCount;

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

    int** countsVar;
    int** mandelSetMembershipVar;

    // struct ppm_pixel* colorPaletteVar;
    struct ppm_pixel** buddhabrotImageVar;
};

void * buddhabrotThread(void* bbDataObj){
    struct buddhabrotData *bbdo = (struct buddhabrotData *) bbDataObj;
    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d).\n", bbdo->threadID, bbdo->startIdxI, bbdo->endIdxI, bbdo->startIdxJ, bbdo->endIdxJ);

    // perform step 1
    int i, j;
    for (i = bbdo->startIdxI; i < bbdo->endIdxI; i++) {//for each row
        for (j = bbdo->startIdxJ; j < bbdo->endIdxJ; j++) {//for each column
            float xfrac = (float)i / (float) bbdo->sizeVar;
            float yfrac = (float)j / (float) bbdo->sizeVar;
            float x0 = bbdo->xminVar + xfrac * (bbdo->xmaxVar - bbdo->xminVar);
            float y0 = bbdo->yminVar + yfrac * (bbdo->ymaxVar - bbdo->yminVar);

            float x = 0;
            float y = 0;
            int iter = 0;
            while (iter < bbdo->maxIterationsVar && x*x + y*y < 4){
                float xtmp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtmp;
                iter++;
            }

            if (iter < bbdo->maxIterationsVar){//escaped
                bbdo->mandelSetMembershipVar[i][j] = 0;//false
            } else {
                bbdo->mandelSetMembershipVar[i][j] = 1;//true
            }
        }
    }

    // perform step 2
    for (i = bbdo->startIdxI; i < bbdo->endIdxI; i++) {//for each row
        for (j = bbdo->startIdxJ; j < bbdo->endIdxJ; j++) {//for each column
            if (bbdo->mandelSetMembershipVar[i][j]){//if it belongs to the mandelbrot set
                continue;
            }

            float xfrac = (float)i / (float) bbdo->sizeVar;
            float yfrac = (float)j / (float) bbdo->sizeVar;
            float x0 = bbdo->xminVar + xfrac * (bbdo->xmaxVar - bbdo->xminVar);
            float y0 = bbdo->yminVar + yfrac * (bbdo->ymaxVar - bbdo->yminVar);

            float x = 0;
            float y = 0;
            while (x*x + y*y < 4){
                float xtmp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtmp;
            

                int yrow = round(bbdo->sizeVar * (y - bbdo->yminVar) / (bbdo->ymaxVar - bbdo->yminVar));
                int xcol = round(bbdo->sizeVar * (x - bbdo->xminVar) / (bbdo->xmaxVar - bbdo->xminVar));

                if (yrow < 0 || yrow >= bbdo->sizeVar){ continue; }
                if (xcol < 0 || xcol >= bbdo->sizeVar) { continue; }
                pthread_mutex_t mutex;
                pthread_mutex_lock(&mutex);
                bbdo->countsVar[yrow][xcol]++;
                if (bbdo->countsVar[yrow][xcol] > bbdo->maxCount){
                    bbdo->maxCount = bbdo->countsVar[yrow][xcol];
                }
                pthread_mutex_unlock(&mutex);
            }
        }
    }


    // use a thread barrier to wait for all threads to finish steps 1 and 2
    pthread_barrier_wait(&barrier);

    // perform step 3
    float gamma = 0.681;
    float factor = 1.0 / gamma;
    for (i = bbdo->startIdxI; i < bbdo->endIdxI; i++) {//for each row
        for (j = bbdo->startIdxJ; j < bbdo->endIdxJ; j++) {//for each column
            float value = 0;
            if (bbdo->countsVar[i][j] > 0){
                value = log(bbdo->countsVar[i][j]) / log(bbdo->maxCount);
                value = pow(value, factor);
            }
            // printf("Val: %f, ", value);
            bbdo->buddhabrotImageVar[i][j].red = value * 255;
            bbdo->buddhabrotImageVar[i][j].green = value * 255;
            bbdo->buddhabrotImageVar[i][j].blue = value * 255;
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
    // float xmin = -2.0;
    // float xmax = 1.0;
    // float ymin = -1.12;
    // float ymax = 1.12;

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
    printf("Generating buddhabrot with size %dx%d\n", size, size);
    printf("  Num processes = %d\n", numProcesses);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    // todo: your code here
    // compute image

    //____________ ALLOCATE BUDDHABROT IMAGE ARRAY
    struct ppm_pixel** buddhabrotImage = (struct ppm_pixel**)malloc(size * sizeof(struct ppm_pixel*));
    if(buddhabrotImage == NULL){
        printf("Error. Allocation was unsuccessful. \n");
        return -1;
    }

    for (int  s = 0; s < size; s++){
        buddhabrotImage[s] = (struct ppm_pixel*)malloc(size * sizeof(struct ppm_pixel));
        if(buddhabrotImage[s] == NULL){
            printf("Error. Allocation was unsuccessful. \n");
            return -1;
        }
    }

    //____________ ALLOCATE COUNTS & mandelSetMembership ARRAY
    int** counts = (int**) malloc(size * sizeof(int*));
    int** mandelSetMembership = (int**) malloc(size * sizeof(int*));
    
    if(counts == NULL){
        printf("Error. Allocation was unsuccessful. \n");
        return -1;
    }
    if(mandelSetMembership == NULL){
        printf("Error. Allocation was unsuccessful. \n");
        return -1;
    }

    for (int s = 0; s < size; s++){
        counts[s] = (int*)malloc(size * sizeof(int));
        mandelSetMembership[s] = (int*)malloc(size * sizeof(int));
        if(counts[s] == NULL){
            printf("Error. Allocation was unsuccessful. \n");
            return -1;
        }
        if(mandelSetMembership[s] == NULL){
            printf("Error. Allocation was unsuccessful. \n");
            return -1;
        }
    }

    //NOW WE SET ALL ELEMENTS OF COUNT EQUAL TO 0
    for (int g = 0; g < size; g++){
        for (int h = 0; h < size; h++){
            counts[g][h] = 0;
        }
    }


    //TIMING STARTS HERE
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);


    struct buddhabrotData bbdoArr[4];

    for (int s = 0; s < 4; s++){
        bbdoArr[s].threadID = s + 1;
        bbdoArr[s].maxCount = 0;
        bbdoArr[s].sizeVar = size;
        bbdoArr[s].xmaxVar = xmax;
        bbdoArr[s].xminVar = xmin;
        bbdoArr[s].yminVar = ymin;
        bbdoArr[s].ymaxVar = ymax;
        bbdoArr[s].maxIterationsVar = maxIterations;

        bbdoArr[s].countsVar = counts;
        bbdoArr[s].mandelSetMembershipVar = mandelSetMembership;
        bbdoArr[s].buddhabrotImageVar = buddhabrotImage;
    }

    bbdoArr[0].startIdxI = 0;
    bbdoArr[0].endIdxI = size / 2;
    bbdoArr[0].startIdxJ = 0;
    bbdoArr[0].endIdxJ = size / 2;

    bbdoArr[1].startIdxI = 0;
    bbdoArr[1].endIdxI = size / 2;
    bbdoArr[1].startIdxJ = size / 2;
    bbdoArr[1].endIdxJ = size;

    bbdoArr[2].startIdxI = size / 2;
    bbdoArr[2].startIdxJ = 0;
    bbdoArr[2].endIdxI = size;
    bbdoArr[2].endIdxJ = size / 2;

    bbdoArr[3].startIdxI = size / 2;
    bbdoArr[3].startIdxJ = size / 2;
    bbdoArr[3].endIdxI = size;
    bbdoArr[3].endIdxJ = size;

    pthread_barrier_init(&barrier, NULL, 4);
    
    pthread_t threadArr[4];
    for (int threadCount = 0; threadCount < 4; threadCount++){
        pthread_create(&threadArr[threadCount], NULL, buddhabrotThread, &bbdoArr[threadCount]);
    }

    for (int t = 0; t < 4; t++){
        pthread_join(threadArr[t], NULL);
        printf("Thread %d finished.\n", t + 1);
    }

    char newFileName[256];
    snprintf(newFileName, 50, "buddhabrot-<%d>-<%ld>.ppm", size, time(0));
    write_ppm_2d(newFileName, bbdoArr[0].buddhabrotImageVar, size, size);

    //TIMING ENDS HERE
    //since the image has now been generated and written
    gettimeofday(&tend, NULL);
    double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Computed buddhabrot set (%dx%d) in %g seconds.\n", size, size, timer);

    printf("Writing file: %s\n", newFileName);


    int t;
    for (t = 0; t < size; t++){
        free(buddhabrotImage[t]);
        free(counts[t]);
        free(mandelSetMembership[t]);
    }
    free(buddhabrotImage);
    free(counts);
    free(mandelSetMembership);
}
