#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct searchData {
    int threadID;
    int valFound;

    int startIdx;
    int endIdx;

    int searchVal;
    int* searchArr;
};

void *threadSearch(void *searchDataObj){
    struct searchData *sdo = (struct searchData *) searchDataObj;
    for (int i = sdo->startIdx; i < sdo->endIdx; i++ ){
        if (sdo->searchArr[i] == sdo->searchVal){
            printf("Thread %d found %d at index %d.\n", sdo->threadID, sdo->searchVal, i);
            sdo->valFound = 1;
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: %s <NumThreads>\n", argv[0]);
        exit(0);
    }

    int length;
    int givenSearchInt = 0;
    int numThreads = atoi(argv[1]);

    FILE* fp = fopen("data.bin", "rb");
    if (fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    
    fread(&length, sizeof(int), 1, fp);
    int* intArr = (int*) malloc(length * sizeof(int));
    fread(intArr, sizeof(int), length, fp);

    printf("Enter a value to search: ");
    scanf(" %d", &givenSearchInt);
    

    struct searchData sd1;
    sd1.valFound = 0;
    sd1.searchVal = givenSearchInt;
    sd1.searchArr = intArr;


    for (int i = 1; i <= numThreads; i++){
        pthread_t ptid;
        sd1.threadID = i;
        sd1.startIdx = (length/numThreads) * (sd1.threadID - 1);
        sd1.endIdx = sd1.startIdx + (length/numThreads);
        pthread_create(&ptid, NULL, threadSearch, &sd1);
        pthread_join(ptid, NULL);
    }
    if (!sd1.valFound){
        printf("Not found.\n");
    }

    
    //do i need to detach or exit the threads? or is join enough

    fclose(fp);
    free(intArr);
    return 0;
}