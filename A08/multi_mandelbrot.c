#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

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

    

    //TODO: MY WORK HERE
    srand(time(0)); 
    
    
    int shmidCP;
    shmidCP = shmget(IPC_PRIVATE, maxIterations * sizeof(struct ppm_pixel), 0644 | IPC_CREAT);
    if (shmidCP == -1) {
        perror("Error: cannot initialize shared memory\n");
        exit(1);
    }

    
    struct ppm_pixel* colorPalette = shmat(shmidCP, NULL, 0);
    if (colorPalette == (void*) -1) {
        perror("Error: cannot access shared memory\n");
        exit(1);
    }

    // PALETTE
    int baseRed = rand() % 255;
    int baseGreen = rand() % 255;
    int baseBlue = rand() % 255;
    int a;
    for (a = 0; a < maxIterations; a++){
        colorPalette[a].red = baseRed + rand() % 100 - 50;
        colorPalette[a].green = baseGreen + rand() % 100 - 50;
        colorPalette[a].blue = baseBlue + rand() % 100 - 50;
    }


    //DECLARE ARRAYS________________________________________
    int shmidTA;
    shmidTA = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel*) * size, 0644 | IPC_CREAT);
    if (shmidTA == -1) {
        perror("Error: cannot initialize shared memory\n");
        exit(1);
    }

    struct ppm_pixel** mandelImage = shmat(shmidTA, NULL, 0);
    if (mandelImage == (void*) -1) {
        perror("Error: cannot access shared memory\n");
        exit(1);
    }

    int shmidSubA;
    int* shmidSubArrayIDs = (int*) malloc(size * sizeof(int));
    for (int s = 0; s < size; s++){
        shmidSubA = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size, 0644 | IPC_CREAT);
        shmidSubArrayIDs[s] = shmidSubA;
        if (shmidSubA == -1) {
            perror("Error: cannot initialize shared memory\n");
            exit(1);
        }

        mandelImage[s] = shmat(shmidSubA, NULL, 0);
        if (mandelImage[s] == (void*) -1) {
            perror("Error: cannot access shared memory\n");
            exit(1);
        }
    }
    //DONE DECLARING ARRAYS________________________________________

    // TIMING STARTS HERE
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    for (int forkCount = 0; forkCount < numProcesses; forkCount++){
        int pid = fork();
        if (pid == 0){
            int startIdxI = 0, startIdxJ = 0, endIdxI = size, endIdxJ = size;
            if(forkCount == 0){
                endIdxI = size / 2;
                endIdxJ = size / 2;
            } else if (forkCount == 1) {
                startIdxJ = size / 2;
                endIdxI = size / 2;
            } else if (forkCount == 2) {
                startIdxI = size / 2;
                endIdxJ = size / 2;
            } else if (forkCount == 3) {
                startIdxI = size / 2;
                startIdxJ = size / 2;
            }
            printf("Sub-image block: cols (%d, %d) to rows (%d,%d)\n", startIdxI, endIdxI, startIdxJ, endIdxJ);
            int i, j;
            for (i = startIdxI; i < endIdxI; i++) {//for each row
                for (j = startIdxJ; j < endIdxJ; j++) {//for each column
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
            exit(0);
        } else {
            printf("Launched child process: %d\n", pid);
        }

    }
    
    for (int i = 0; i < numProcesses; i++) {
        int status;
        int pid = wait(&status);
        printf("Child process complete: %d\n", pid);
    }

    char newFileName[256];
    snprintf(newFileName, 50, "multi-mandelbrot-<%d>-<%ld>.ppm", size, time(0));
    write_ppm_2d(newFileName, mandelImage, size, size);

    // TIMING ENDS HERE
    // since the image has now been generated and written
    gettimeofday(&tend, NULL);
    double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Computed mandelbrot set (%dx%d) in %g seconds.\n", size, size, timer);




    //FREE COLOR PALETTE
    if (shmdt(colorPalette) == -1) {
        perror("Error: cannot detatch from shared memory 00\n");
        exit(1);
    }
    if (shmctl(shmidCP, IPC_RMID, 0) == -1) {
        perror("Error: cannot remove shared memory 00\n");
        exit(1);
    }

    //FREE SUBARRAYS
    for (int s = 0; s < size; s++){
        if (shmdt(mandelImage[s]) == -1) {
            perror("Error: cannot detatch from shared memory 11\n");
            exit(1);
        }
        if (shmctl(shmidSubArrayIDs[s], IPC_RMID, 0) == -1) {
            perror("Error: cannot remove shared memory 11\n");
            exit(1);
        }
    }
    //FREE SUBARRAY ID ARRAY
    free(shmidSubArrayIDs);

    //FREE TOP LEVEL ARRAY/IMAGE
    if (shmdt(mandelImage) == -1) {
            perror("Error: cannot detatch from shared memory 22\n");
            exit(1);
        }
    
    if (shmctl(shmidTA, IPC_RMID, 0) == -1) {
        perror("Error: cannot remove shared memory 22\n");
        exit(1);
    }

}
