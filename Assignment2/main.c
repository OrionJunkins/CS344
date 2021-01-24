/*
    Orion Junkins
    junkinso@oregonstate.edu
    Assignment 2

    Reads directory entries
    Finds a file in the current directory based on user specified criteria
    Reads and processes the data in the chosen file
    Creates a directory
    Creates new files in the newly created directory and writes processed data to these files
*/
#include <stdio.h>
#include <stdlib.h>
#define MAX_INPUT_LENGTH 100
#include "file_processing.c"

char* TOP_LEVEL_INPUT_PROMPT = "1. Select file to process\n2. Exit the program\n\nEnter a choice 1 or 2: ";
char* FILE_SELECTION_INPUT_PROMPT = "Which file you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of a file\n\nEnter a choice from 1 to 3: ";

int main()
{   
    // Get top level menu user selection
    int user_selection = get_selection(TOP_LEVEL_INPUT_PROMPT, 1, 2);

    // Continue to prompt the user until they select 2 ('Exit') at the top level menu
    while(user_selection != 2)
    {
        // Prompt for file selection method
        user_selection = get_selection(FILE_SELECTION_INPUT_PROMPT, 1, 3);

        // Select a file in the selected manner
        char* selected_file = get_file(user_selection);

        // If the selected file is not valid, re-prompt the user
        while( access( selected_file, F_OK ) != 0 ) 
        {
            printf("The file %s was not found. Try again\n\n", selected_file);
            user_selection = get_selection(FILE_SELECTION_INPUT_PROMPT, 1, 3);
            selected_file = get_file(user_selection);
        }   
        
        // Process the selected file
        process_file(selected_file);

        // Re-prompt with top level options
        user_selection = get_selection(TOP_LEVEL_INPUT_PROMPT, 1, 2);
    }
}
