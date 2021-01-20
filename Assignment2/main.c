#include <stdio.h>
#include <stdlib.h>
#define MAX_INPUT_LENGTH 100
/*
    Reads directory entries
    Finds a file in the current directory based on user specified criteria
    Reads and processes the data in the chosen file
    Creates a directory
    Creates new files in the newly created directory and writes processed data to these files
*/

#include "file_processing.c"

char* TOP_LEVEL_INPUT_PROMPT = "1. Select file to process\n2. Exit the program\n\nEnter a choice 1 or 2: ";
char* FILE_SELECTION_INPUT_PROMPT = "Which file you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of a file\n\nEnter a choice from 1 to 3: ";

int main()
{   
    // Get top level menu user selection
    int selection = user_selection(TOP_LEVEL_INPUT_PROMPT, 1, 2);

    // Continue to prompt the user until they select 2 ('Exit') at the top level menu
    while(selection != 2){
        // Prompt for file selection method
        selection = user_selection(FILE_SELECTION_INPUT_PROMPT, 1, 3);
        
       // Select a file in the selected manner
        FILE* selected_file;
        select_file(selected_file, selection);

        // Process the selected file
        //process_file(selected_file);

        // Re-prompt with top level options
        selection = user_selection(TOP_LEVEL_INPUT_PROMPT, 1, 2);
    }
    
}