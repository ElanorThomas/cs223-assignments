/*----------------------------------------------
 * Author: Elanor Thomas
* Base code from Aline Normoyle
 * Date: Apr 19 2023
 * Description: This file contains the memstats method,
 * which examines the memory in use and in the freed
 * memory linked list at the time of calling, and
 * prints a number of statistics on the data, such as
 * the total amount of memory allocated and in use,
 * the total number of blocks of memory allocated and
 * in use, and the percent of memory in the buffer
 * that is not being utilized
 ---------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
    int size;
    int used;
    struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
    int totalBlocks = 0;
    int usedBlocks = 0;

    int memoryAllocated = 0;
    int memoryInUse = 0;
    int freedMemory = 0;

    int underutilizedMemory = 0;
    int underUtilSize = 0;
    
    for (int i = 0; i < len; i++){
        if (buffer[i] != NULL){
            struct chunk *givenChunk = (struct chunk*)((struct chunk*)buffer[i] - 1);
            totalBlocks++;
            usedBlocks++;
            memoryInUse += givenChunk->size;
            memoryAllocated += givenChunk->size;
            underutilizedMemory += givenChunk->size - givenChunk->used;
            underUtilSize += givenChunk->size;
        }
    }
    // printf("lower in memstats%d\n", buffer.len);

    

    struct chunk *nextChunk = freelist;
    while (nextChunk != NULL){
        totalBlocks++;
        memoryAllocated += nextChunk->size;
        freedMemory += nextChunk->size;
        // underutilizedMemory += nextChunk->size - nextChunk->used;
        
        nextChunk = nextChunk->next;
    }

    printf("Total blocks: %d Free blocks: %d Used blocks: %d\n", totalBlocks, (totalBlocks - usedBlocks), usedBlocks);
    printf("Total memory allocated: %d Free memory: %d Used memory: %d\n", memoryAllocated, freedMemory, memoryInUse);
    printf("Underutilized memory: %.2f\n", (float) underutilizedMemory / (float) underUtilSize);
}

int main ( int argc, char* argv[]) {

    printf("Starting test..\n");

    srand(100);

    double timer;
    struct timeval tstart, tend;

    gettimeofday(&tstart, NULL);

    void *buffer[BUFFER];
    for (int i = 0; i < BUFFER; i++) {
        buffer[i] = NULL;
    }

    void *init = sbrk(0);
    void *current;
    printf("The initial top of the heap is %p.\n", init);
    for (int j = 0 ; j < ROUNDS; j++) {
        printf("---------------\n%d\n" , j);

        for (int i= 0 ; i < LOOP ; i++) {
            int index = rand() % BUFFER;
            if (buffer[index] != NULL) {
                free(buffer[index]);
                buffer[index] = NULL;
                printf("Freeing index %d\n", index);
            } 
            else {
                size_t size = (size_t) randExp(8, 4000); 
                int *memory = NULL;
                memory = malloc(size);

                if (memory == NULL) {
                    fprintf(stderr, "malloc failed\n");
                    return(1);
                } 
                *memory = 123;
                buffer[index] = memory;
                printf("Allocating %d bytes at index %d\n", (int) size, index);
            }
        }
        extern struct chunk* flist;//to make this compilable without mylloc, remove extern
        current = sbrk(0);
        int allocated = current - init;
        init = current;

        printf("The new top of the heap is %p.\n", current);
        printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
        memstats(flist, buffer, BUFFER);
    }

    for (int i = 0; i < BUFFER; i++) {
        free(buffer[i]); 
    }
    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Time is %g\n", timer);

    return 0 ;
}
