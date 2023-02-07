#include <stdio.h>
#include <string.h>

int main() {
        char buff[32];
        scanf("%[^\n]%*c", buff);
        char acronym[1024] = {};
        int i;
        int letterCount = 0;
        for (i = 0; i < strlen(buff); i++){
		if (buff[i] > 64 && buff[i] < 91){
                        acronym[letterCount] = buff[i];
                        letterCount++; 
                }
        }
       	acronym[letterCount] = '\0';
        printf("Your acronym is: %s\n", acronym);
        return 0;
}
