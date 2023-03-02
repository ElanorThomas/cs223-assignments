/*----------------------------------------------
 * Author: Elanor Thomas
 * Date: Feb 22 2023
 * Description: this program will take in two words
 * and 'cross' them with each other, or indicate
 * if they have no letters in common
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc > 3) {
        printf("We haven't enabled functionality with more"
            " than 2 words yet and will run the program with"
            " the first two words you entered");
    }

    char word1[32];
    char word2[32];
    strcpy(word1, argv[1]);
    strcpy(word2, argv[2]);
    int len1 = strlen(word1);
    int len2 = strlen(word2);


    char** crossword = (char**)malloc(len1 * sizeof(char*));
    int t;
    for (t = 0; t < len1; t++){
        crossword[t] = (char*)malloc(len2 * sizeof(char));
    }

    int a, b;
    for (a = 0; a < len1; a++){
        for (b = 0; b < len2; b++){
            crossword[a][b] = '.';
        }
    }

    int idx1 = 0, idx2 = 0;
    for (idx1 = 0; idx1 < len1; idx1++){
        for (idx2 = 0; idx2 < len2; idx2++){
            if (word1[idx1] == word2[idx2]){
                break;
            }
        }
        if (word1[idx1] == word2[idx2]){
            break;
        }
    }

    if (idx1 >= len1 || idx2 >= len2){
        printf("No common letter. Please try again.\n");
        return 0;
    }

    for (a = 0; a < len1; a++){
        crossword[a][idx2] = word1[a];
    }

    for (a = 0; a < len2; a++){
        crossword[idx1][a] = word2[a];
    }



    for (a = 0; a < len1; a++){
        for (b = 0; b < len2; b++){
            printf("%c ", crossword[a][b]);
        }
        printf("\n");
    }
    for (t = 0; t < len1; t++){
        free(crossword[t]);
    }
    free(crossword);
    return 0;

    
}
