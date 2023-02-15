/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: 2/15/2023
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Song{
    char songTitle[128];
    char artist[128];
    int durationMs;
    float danceability;
    float energy;
    float tempo;
    float valence;
};

struct node{
    struct Song nodeSong;
    struct node* next;
};// *head;

void printOneSong(struct Song givenSong){
    int seconds = (givenSong.durationMs/1000) % 60;
    int minutes = (int) ((givenSong.durationMs/1000) / 60);
    printf("%-25s artist: %-25s duration: %d:%-2d danceability: %.1f energy: "
            "%.1f tempo: %-5.1f valence: %-3.1f\n", givenSong.songTitle,
            givenSong.artist, minutes, seconds, givenSong.danceability,
            givenSong.energy, givenSong.tempo, givenSong.valence);
}

void printSongs(struct node* head){
    struct node* varNode = head;
    int counter = 0;
    while(varNode != NULL){
        int seconds = (varNode->nodeSong.durationMs/1000) % 60;
        int minutes = (int) ((varNode->nodeSong.durationMs/1000) / 60);
        printf("%2d) %-25s artist: %-25s duration: %d:%-2d danceability: "
                "%3.3f energy: %3.3f tempo: %-5.3f valence: %-3.3f\n",
                counter, varNode->nodeSong.songTitle,varNode->nodeSong.artist,
                minutes, seconds, varNode->nodeSong.danceability,
                varNode->nodeSong.energy, varNode->nodeSong.tempo,
                varNode->nodeSong.valence);
        counter++;
        varNode = varNode->next;
    }
}

int countSongs(struct node* head){
    struct node* varNode = head;
    int counter = 0;
    while(varNode != NULL){
        counter++;
        varNode = varNode->next;
    }
    return counter;
}

//This method algorithm and most syntax taken from 2/6/2023
//powerpoint slide: Example: Dynamic Linked List (using malloc/free) from Aline Normoyle
struct node* insert_front(struct Song givenSong, struct node** head){
    struct node* newNode = (struct node *) malloc(sizeof(struct node));
    if (newNode == NULL){
        printf("ERROR: Out of space\n");
        exit(1);
    }
    newNode->nodeSong = givenSong;
    newNode->next = *head; 
    *head = newNode;
    return newNode;
}

// struct node* find_node(struct Song givenSong, struct node** head){
//     struct node* varNode = *head;
//     struct node* nextNode = varNode->next;
//     while(nextNode != NULL){
//         if (strcmp(varNode->nodeSong.artist, givenSong.artist) == 0
//                 && strcmp(varNode->nodeSong.songTitle, givenSong.songTitle) == 0){
//             return varNode;
//         }
//         varNode = nextNode;
//         nextNode = varNode->next;
//     }
//     if (strcmp(varNode->nodeSong.artist, givenSong.artist) == 0
//                 && strcmp(varNode->nodeSong.songTitle, givenSong.songTitle) == 0){
//             return varNode;
//         }
//     return NULL;
// }



void remove_node(struct Song givenSong, struct node** head){
    struct node* lastNode = *head;
    struct node* varNode = lastNode->next;
    if (*head != NULL && strcmp(lastNode->nodeSong.artist, givenSong.artist) == 0
            && strcmp(lastNode->nodeSong.songTitle, givenSong.songTitle) == 0){
        *head = varNode;
        free(lastNode);
        return;
    }
    while (varNode != NULL && (strcmp(varNode->nodeSong.artist, givenSong.artist) != 0
            || strcmp(varNode->nodeSong.songTitle, givenSong.songTitle) != 0)){
        lastNode = varNode;
        varNode = lastNode->next;
    }

    if (varNode == NULL){
        return;
    }

    lastNode->next = varNode->next;
    free(varNode);
    return;
}

void most_danceable(struct node** head){
    struct node* varNode = *head;
    float maxDanc = varNode->nodeSong.danceability;
    struct node* maxDancNode = *head;
    while(varNode->next != NULL){
        varNode = varNode->next;
        if (varNode->nodeSong.danceability > maxDanc){
            maxDanc = varNode->nodeSong.danceability;
            maxDancNode = varNode;
        }
    }
    // printf("MaxDan%f", maxDanc);
    printOneSong(maxDancNode->nodeSong);
    remove_node(maxDancNode->nodeSong, head);
    return;
}

int main() {
    struct node* listHead = NULL;
    FILE* filePtr;
    filePtr = fopen("songlist.csv", "r");
    if(filePtr == NULL) {
        perror("Error opening file");
        return(-1);
    }
    char lineRead[300];//each artist and title: max 128, plus commas, other vals
    fgets(lineRead, 300, filePtr);//this is the header line, ignore
    const char s[2] = ",";

    while (fgets(lineRead, 300, filePtr) != NULL){
        struct Song curSong;
        strcpy(curSong.songTitle, strtok(lineRead, s));
        strcpy(curSong.artist, strtok(NULL, s));
        curSong.durationMs = atoi(strtok(NULL, s));
        curSong.danceability = atof(strtok(NULL, s));
        curSong.energy = atof(strtok(NULL, s));
        curSong.tempo = atof(strtok(NULL, s));
        curSong.valence = atof(strtok(NULL, s));
        insert_front(curSong, &listHead);
    }
    printSongs(listHead);

    printf("\n\nDataset contains %d songs.\n\n===============", countSongs(listHead));
    printf("\n\nPress 'd' to show the most danceable song (any other key to quit): ");

    char given;
    scanf(" %c", &given);
    while (given == 'd'){
        if (countSongs(listHead) != 0){
            printf("\nMOST DANCEABLE:\n");
            most_danceable(&listHead);
            printf("\n");
            printSongs(listHead);
        }
        printf("\n\nDataset contains %d songs.\n\n===============", countSongs(listHead));
        printf("\n\nPress 'd' to show the most danceable song (any other key to quit): ");
        scanf(" %c", &given);
    }
    
    fclose(filePtr);
    if (listHead != NULL){
        struct node* toFreeNode = listHead;
        struct node* nextNode = toFreeNode->next;
        while(nextNode != NULL){
            free(toFreeNode);
            toFreeNode = nextNode;
            nextNode = toFreeNode->next;
        }
        free(toFreeNode);
    }
    return 0;
}
