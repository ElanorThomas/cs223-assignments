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
	int pauseLength = 0;
    	printf("Pause length:");
    	scanf("%d", &pauseLength);

    	char buff[32];
    	char* newBuff = malloc(128 * sizeof(char));
    	printf("Text:");
    	scanf(" %31s", buff);

   	int i, s;
    	int t = 0;
    	for (i = 0; i < strlen(buff); i++){
       		newBuff[t] = buff[i];
        	for (s = 0; s < pauseLength; s++){
            		newBuff[++t] = '.';
        	}
        	t++;
    	}
    	newBuff[t] = '\0';
    	printf("%s", newBuff);
	free(newBuff);
    	return 0;
}
