/*
Orion Junkins
junkinso@oregonstate.edu
Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "movie_list.c"

#define MAX_INPUT_LENGTH 100
#define MAX_FILENAME_LENGTH 200
#define MAX_DIR_NAME_LENGTH 100
#define LONG_LONG_INT_MAX 9223372036854775807
#define PREFIX "movies_"
#define SUFFIX ".csv"
#define ONID_ID "junkinso"

// ************************************************ //
// SELECT FILE //
// ************************************************ //
int get_selection(char* prompt, int min, int max);
/*
    Print a given prompt and get input from the user repeateadly until input is valid
    Params:
        char* prompt    prompt which will be printed every time input is requested
        int min         lower bound for input (inclusive)
        int max         upper bound for input (inclusive)
    Returns:
        int user_selection    integer value within the range [min,max]
*/


void get_file(char* filename, int selection);
/*
    Given a choice selection made by the user, find the name of the desired file in the current directory
    Params:
        char* filename:     exact filename which corresponds to the users selection
        int selection:      user choice in the range [1,3]    
*/


void largest_file_in_cur_dir(char* filename);
/*
    Check every file in the current direcory. Return the filename of the largest file with the specified format
    Params:
        char* filename        The filename of the largest file matching the format
*/

void smallest_file_in_cur_dir(char* filename);
/*
    Check every file in the current direcory. Return the filename of the smallest file with the specified format
    Params:
        char* filename        The filename of the smallest file matching the format
*/

bool filename_matches_format(char* filename);
/*
    Check to see if a given filename has the specified suffix and prefix 
    Params:
        char* filename      filename which will be checked
    Returns:    
        bool        True if filename matches format, false otherwise
*/
  

void prompt_for_name(char* filename);
/*
    Prompt the user for a filename
    Params:
            char* filename      output location for user entered filename
*/



// ************************************************ //
// PROCESS FILE //
// ************************************************ //
void process_file(char* selected_file);
/*
    Given a filename, process it into a movie list, create a new directory, and store a list of titles in a new txt file for each year
    Params:
        char* selected_file         filename which will be opened/processed. Assumed to be valid
*/


void get_path(char* output_dir_path);
/*
    Generate a new filpath in the form ./ONID_ID.movies.#/ where # is a randomly generated number up to 99999
    Params:
        char* output_dir_path       Output variable to which the new filepath will be assigned
*/

  