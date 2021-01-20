#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define PREFIX "movies"
#define SUFFIX ".csv"

void prompt_for_name(char* filename);

int user_selection(char* prompt, int min, int max) 
{
    /*
        Print a given prompt and get input from the user repeateadly until input is valid.
        Params:
            char* prompt    prompt which will be printed every time input is requested
            int min         lower bound for input (inclusive)
            int max         upper bound for input (inclusive)
        Returns:
            int user_selection    integer value within the range [min,max]
    */

    // Prompt the user
    printf("%s", prompt);

    // Get input
    ssize_t line_size;
    char* input_buffer;
    size_t input_buffer_size = MAX_INPUT_LENGTH;
    input_buffer = (char*)(calloc(input_buffer_size, sizeof(char)));
    int user_selection = -1;
    line_size = getline(&input_buffer, &input_buffer_size, stdin);
    user_selection = atoi(input_buffer);

    // If input is not in the valid range, continually re prompt until it is
    while(user_selection < min | user_selection > max)
    {
        printf("Invalid choice, please try again\n\n%s", prompt);
        line_size = getline(&input_buffer, &input_buffer_size, stdin);
        user_selection = atoi(input_buffer);
    }
    free (input_buffer);
    return user_selection;
}

// ************************************************ //
// SELECT FILE //
// ************************************************ //

void select_file(FILE* selected_file, int selection)
{
    char* filename = (char*) (calloc(MAX_FILENAME_LENGTH, sizeof(char)));
    switch (selection)
    {
    case 1:
        // Pick the largest file
        largest_file_in_cur_dir(filename);
        break;
    case 2:
        // 
        break;
    case 3:
        // Specify the name of a file 
        prompt_for_name(filename);        
        break;
    default:
        printf("Error: invalid selection");
        break;
    }
    printf("\n\nFILENAME: %s\n\n", filename);

    //OPEN(filename)
    free(filename);
}

void largest_file_in_cur_dir(filename)
{
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;

    // Go through all the entries
    while((aDir = readdir(currDir)) != NULL)
    {
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strcmp(str + strlen(aDir->d_name) - (1 + strlen(SUFFIX)), SUFFIX) == 0) //Extract to bool function
        {
            printf("/n%s", aDir->d_name);
        }
    }
}
void prompt_for_name(char* filename)
{
    printf("Enter the complete file name: ");

    ssize_t line_size;
    size_t input_buffer_size = MAX_FILENAME_LENGTH;
    line_size = getline(&filename, &input_buffer_size, stdin);
}


// ************************************************ //
// PROCESS FILE //
// ************************************************ //
void process_file(selected_file)
{
    return;
}


