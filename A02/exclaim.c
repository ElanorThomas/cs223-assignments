#include <stdio.h>
#include <string.h>

static char* exclaim(char* str);

int main() {
 	char buff[32];
	while (strcmp(buff, "STOP") != 0){
		scanf("%s", &buff);
		exclaim(buff);
		printf("%s\n", buff);
	}
	return 0;
}

static char* exclaim(char* str){
	char options[4] = {'@', '!', '#', '*'};
	int i;
	for (i = 0; i < strlen(str); i++){
		if (str[i] > 96 && str[i] <  123){
			str[i] = options[rand() % 4]; 
		}
	}
	return str;
}
