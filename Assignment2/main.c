#include <stdio.h>
#include <stdlib.h>
#define MAX_INPUT_LENGTH 100

#include "file_processing.c"

char* TOP_LEVEL_INPUT_PROMPT = "1. Select file to process\n2. Exit the program\n\nEnter a choice 1 or 2: ";
char* FILE_SELECTION_INPUT_PROMPT = "Which file you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of a file\n\nEnter a choice from 1 to 3: ";

int main()
{
    int selection = user_selection(TOP_LEVEL_INPUT_PROMPT, 1, 2);
    printf("\nInitial user_sel: %d\n", selection);

    while(selection != 2){
        printf("\n2nd user_sel: %d\n", selection);
        printf("\n3rd user_sel: %d\n", selection);
    }
    
}