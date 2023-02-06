#include <stdio.h>
#include <string.h>

struct Song{
	char songTitle[32];
	char artist[32];
    	int durationMinutes;
    	int durationSeconds;
    	float danceability;
};

void printSongs(int songArraySize, struct Song givenSongs[]){
    	int i;
    	for (i = 0; i < songArraySize; i++){
        	printf("%d) %-20s artist: %-20s duration: %d:%d danceability: %.1f\n", i, givenSongs[i].songTitle, givenSongs[i].artist, givenSongs[i].durationMinutes, givenSongs[i].durationSeconds, givenSongs[i].danceability);
    	}
}

int main() {
    	struct Song song1 = {"Jackie Onassis", "Sammie Rae & The Friends", 4, 52, 0.1};
    	struct Song song2 = {"Hypotheticals", "Lake Street Dive", 3, 50, 0.2};
    	struct Song song3 = {"My Ego Dies At The End", "Jensen McRae", 4, 03, .2};
    	struct Song songArr[3] = {song1, song2, song3};
    	int songNumChoice;
    	char attributeChoice[15];//= {}?
    
    	printf("Welcome to Ellie's struct song list.\n");
    	printSongs(3, songArr);
    	printf("\n=======================\nEnter a song id to edit: ");
    	scanf("%d", &songNumChoice);
    	if (songNumChoice >= 3){
        	printf("Invalid ID. Please try again.");
        	return 0;
    	}
    	char c = getchar();
    	printf("Which attribute do you wish to edit? [artist, title, duration, danceability]:");
    	// scanf("%[^\n]%*c", &attributeChoice);//I tried to use this but I could  not get it to work ahhhh
   	 scanf(" %s", &attributeChoice);
   	 // printf("%s", attributeChoice);
   	if (strcmp(attributeChoice, "artist") == 0){
        	printf("\nWhat would you like the new artist to be? ");
        	char newArtist[32];
        	scanf(" %s", &newArtist);
        	strcpy(songArr[songNumChoice].artist, newArtist);
        	// printSongs(3, songArr);
    	} else if (strcmp(attributeChoice, "title") == 0){
        	printf("What would you like the new title to be? ");
       	 	char newTitle[32];
        	scanf(" %s", &newTitle);
        	strcpy(songArr[songNumChoice].songTitle, newTitle);

    	} else if (strcmp(attributeChoice, "danceability") == 0){
        	printf("What would you like the new danceability value to be? ");
        	float newDance;
        	scanf("%f", &newDance);
        	songArr[songNumChoice].danceability = newDance;
    	} else if (strcmp(attributeChoice, "duration") == 0){
        	printf("Would you like to edit minutes(input m) or seconds(input s)? ");
        	char a = getchar();
        	char c = getchar();
        	if (c == 'm'){
            		printf("What would you like the new minutes value to be? ");
            		int newMinVal;
            		scanf("%d", &newMinVal);
            		songArr[songNumChoice].durationMinutes = newMinVal;
        	}
        	if (c == 's'){
            		printf("What would you like the new seconds value to be? ");
            		int newSecVal;
            		scanf("%d", &newSecVal);
            		songArr[songNumChoice].durationSeconds = newSecVal;
        	}
    	}
    	printSongs(3, songArr);
    	// printf("%d", songNumChoice);
    	return 0;
}
