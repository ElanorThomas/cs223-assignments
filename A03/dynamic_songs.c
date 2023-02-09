/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: 2/8/2023
 * Description This program reads and prints an array of song structs from a csv
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

void printSongs(int songArraySize, struct Song givenSongs[]){
    int i;
   	for (i = 0; i < songArraySize; i++){
        int seconds = (givenSongs[i].durationMs/1000) % 60;
        int minutes = (int) ((givenSongs[i].durationMs/1000) / 60);
       	printf("%2d) %-25s artist: %-25s duration: %d:%-2d danceability: %.1f energy: %.1f tempo: %-5.1f valence: %-3.1f\n", i, givenSongs[i].songTitle, givenSongs[i].artist, minutes, seconds, givenSongs[i].danceability, givenSongs[i].energy, givenSongs[i].tempo, givenSongs[i].valence);
    }
}

int main() {
        struct Song* songArr = malloc(20 * sizeof(struct Song));
        FILE* filePtr;
        filePtr = fopen("songlist.csv", "r");
        if(filePtr == NULL) {
            perror("Error opening file");
            return(-1);
        }

        char lineRead[300];//each artist and title is at max 128, plus extra for commas and each other value
        fgets(lineRead, 300, filePtr);
        const char s[2] = ",";
    
        int numSongs = atoi(strtok(lineRead, s));
        fgets(lineRead, 300, filePtr);//this is the header line, ignore

        int i;
        for (i = 0; i < numSongs; i++){
            fgets(lineRead, 300, filePtr);
            char *curTit = strtok(lineRead, s);
            char *curArt = strtok(NULL, s);
            struct Song curSong = {"placeholder", "placeholder", atoi(strtok(NULL, s)), atof(strtok(NULL, s)), atof(strtok(NULL, s)), atof(strtok(NULL, s)), atof(strtok(NULL, s))};
            strcpy(curSong.songTitle, curTit);
            strcpy(curSong.artist, curArt);
            // int curDur = atoi(strtok(NULL, s));
            // float curDan = atof(strtok(NULL, s));
            // float curEner = atof(strtok(NULL, s));
            // float curTem = atof(strtok(NULL, s));
            // float curVal = atof(strtok(NULL, s));
            // struct Song curSong = {"placeholder", "placeholder", curDur, curEner, curTem, curVal};
            
            songArr[i] = curSong;
        }
    	printSongs(18, songArr);
    	fclose(filePtr);
    	free(songArr);
    	return 0;
}
