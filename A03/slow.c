/*----------------------------------------------
 * Author:Elanor Thomas 
 * Date:Feb 8 2023
 * Description
 * This program takes a string and prints it with an inputed number
 * of periods between each character.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main() {
<<<<<<< HEAD
    int pauseLength = 0;
    printf("Pause length:");
    scanf("%d", &pauseLength);
    char buff[32];
    printf("Text:");
    scanf(" %31s", buff);
    int m = 0;//given string length
    while (buff[m] != '\0'){
        m++;
    }
    int newBuffLen = ((pauseLength + 1) * m) + 1;
    char* newBuff = malloc(newBuffLen * sizeof(char));
    int i, s;
    int t = 0;
    for (i = 0; i < strlen(buff); i++){
        newBuff[t] = buff[i];
        for (s = 0; s < pauseLength; s++){
            newBuff[++t] = '.';
=======
	int pauseLength = 0;
    	printf("Pause length:");
    	scanf("%d", &pauseLength);

    	char buff[32];
    	
    	printf("Text:");
    	scanf(" %31s", buff);

        int m = 0;//given string length
        while (buff[m] != '\0'){
         	m++;
>>>>>>> e6be5670a2859793f9ed94f82c1472fd400026cb
        }
        t++;
    }
    newBuff[t] = '\0';
    printf("%s", newBuff);
    free(newBuff);
    return 0;
}
