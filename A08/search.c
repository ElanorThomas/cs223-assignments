#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

int main() {
    int length;
    pid_t pid;
    int searchInt = 0;
    int cfound = 0;
    int pfound = 0;

    FILE* fp = fopen("data.bin", "rb");
    if (fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    
    fread(&length, sizeof(int), 1, fp);
    int* intArr = (int*) malloc(length * sizeof(int));
    // printf("Length:%d|\n", length);
    fread(intArr, sizeof(int), length, fp);

    printf("Enter a value to search: ");
    scanf("%d", &searchInt);
        
    pid = fork();
    if (pid == 0){
        for (int i = 0; i < length/2; i++) {
            if (intArr[i] == searchInt){
                printf("Child found %d at index %d\n", searchInt, i);
                cfound++;
            }
        }
        if (!cfound){
            printf("Not found!\n");
        }
        exit(0);
    } else {
        for (int i = length/2; i < length; i++) {
            if (intArr[i] == searchInt){
                printf("Parent found %d at index %d\n", searchInt, i);
                pfound++;
            }
        }
        if (!pfound){
            printf("Not found!\n");
        }
    }
    printf("More work to do\n");



    fclose(fp);
    free(intArr);
}
