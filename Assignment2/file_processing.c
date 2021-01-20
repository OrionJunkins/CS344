#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_INPUT_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define PREFIX "movies"
#define SUFFIX ".csv"
#define LONG_LONG_INT_MAX 9223372036854775807
void prompt_for_name(char* filename);
void largest_file_in_cur_dir(char* filename);
bool filename_matches_format(char* filename);
void smallest_file_in_cur_dir(char* filename);

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
        smallest_file_in_cur_dir(filename);
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

void largest_file_in_cur_dir(char* filename)
{
    off_t largest_file_size = -1;
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;

    // Go through all the entries
    while((aDir = readdir(currDir)) != NULL)
    {        
        if(filename_matches_format(aDir->d_name))
        {
            stat(aDir->d_name, &dirStat);
            if(dirStat.st_size > largest_file_size)
            {
                largest_file_size = dirStat.st_size;
                strcpy(filename, aDir->d_name);
            }
        }
    }
}

void smallest_file_in_cur_dir(char* filename)
{
    off_t smallest_file_size = LONG_LONG_INT_MAX;
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;

    // Go through all the entries
    while((aDir = readdir(currDir)) != NULL)
    {        
        if(filename_matches_format(aDir->d_name))
        {
            stat(aDir->d_name, &dirStat);
            if(dirStat.st_size < smallest_file_size)
            {
                smallest_file_size = dirStat.st_size;
                strcpy(filename, aDir->d_name);
            }
        }
    }
}

bool filename_matches_format(char* filename)
{
    if (strncmp(PREFIX, filename, strlen(PREFIX)) == 0)
    {
        if(strcmp(filename + strlen(filename) - strlen(SUFFIX), SUFFIX) == 0)
        {
            return true;
        }
    }
    return false;
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

