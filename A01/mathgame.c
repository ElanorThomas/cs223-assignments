/***************************************************
 * mathgame.c
 * Author:Elanor Thomas 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Welcome to math game! How many rounds do you want to play?");
    int numQuestions;
    int numCorrect = 0;
    scanf("%d", &numQuestions);
    int i, op1, op2, givenAns;
    for( i = 0 ; i < numQuestions; i++ ) {
        op1 = rand()%10;
        op2 = rand()%10;
        printf("%d + %d = ? ", op1, op2);
        scanf("%d", &givenAns);
        if (givenAns == op1 + op2){
            printf("Correct!\n");
            numCorrect++;
        } else {
            printf("Incorrect ;(\n");
        }
   }
   printf("You answered %d / %d correctly.\n", numCorrect, numQuestions);
   return 0;
}
