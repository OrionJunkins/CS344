/*
Orion Junkins
junkinso@oregonstate.edu
Assignment 2
*/

#include "file_processing.h"

// ************************************************ //
// SELECT FILE //
// ************************************************ //
int get_selection(char* prompt, int min, int max) 
{
    /*
        Print a given prompt and get input from the user repeateadly until input is valid
        Params:
            char* prompt    prompt which will be printed every time input is requested
            int min         lower bound for input (inclusive)
            int max         upper bound for input (inclusive)
        Returns:
            int user_selection    integer value within the range [min,max]
    */

    // Prompt the user
    printf("%s", prompt);

    // Get an integer input from stdin
    ssize_t line_size;
    char* input_buffer = NULL;
    size_t input_buffer_size = 0;
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

    return user_selection;
}

char* get_file(int selection)
{
    /*
        Given a choice selection made by the user, find the name of the desired file in the current directory
        Params:
            int selection:      user choice in the range [1,3]
        returns:
            char* filename:     exact filename which corresponds to the users selection
    */
    char* filename = NULL;
    switch (selection)
    {
    case 1:
        // Pick the largest file
        filename = largest_file_in_cur_dir();
        break;
    case 2:
        // Pick the smallest file
        filename = smallest_file_in_cur_dir();
        break;
    case 3:
        // Specify the name of a file 
        filename = prompt_for_name();        
        break;
    default:
        printf("Error: invalid selection");
        break;
    }
    return filename;
}

char* largest_file_in_cur_dir()
{
    /*
        Check every file in the current direcory. Return the filename of the largest file with the specified format
        Returns:
            char* d_name        The filename of the largest file matching the format
    */

    // Setup to store file/size info
    off_t largest_file_size = -1;
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct dirent *largest_file;
    struct stat dirStat;

    // So long as there are more directory entries, check the next to see if it matches the format and is bigger than the current largest
    while((aDir = readdir(currDir)) != NULL)
    {
        if(filename_matches_format(aDir->d_name))
        {
            stat(aDir->d_name, &dirStat);
            if(dirStat.st_size > largest_file_size)
            {
                largest_file_size = dirStat.st_size;
                largest_file = aDir;
            }
        }
    }
    return largest_file->d_name;
}

char* smallest_file_in_cur_dir()
{
    /*
        Check every file in the current direcory. Return the filename of the smallest file with the specified format
        Returns:
            char* d_name        The filename of the smallest file matching the format
    */

    // Setup to store file/size info
    off_t smallest_file_size = LONG_LONG_INT_MAX;
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct dirent *smallest_file;
    struct stat dirStat;

    // So long as there are more directory entries, check the next to see if it matches the format and is smaller than the current largest
    while((aDir = readdir(currDir)) != NULL)
    {        
        if(filename_matches_format(aDir->d_name))
        {
            stat(aDir->d_name, &dirStat);
            if(dirStat.st_size < smallest_file_size)
            {
                smallest_file_size = dirStat.st_size;
                smallest_file = aDir;
            }
        }
    }
    return smallest_file->d_name;
}

bool filename_matches_format(char* filename)
{
    /*
        Check to see if a given filename has the specified suffix and prefix 
        Params:
            char* filename      filename which will be checked
        Returns:    
            bool        True if filename matches format, false otherwise
    */
    if (strncmp(PREFIX, filename, strlen(PREFIX)) == 0)
    {
        if(strcmp(filename + strlen(filename) - strlen(SUFFIX), SUFFIX) == 0)
        {
            return true;
        }
    }
    return false;
}

char* prompt_for_name()
{
    /*
        Prompt the user for a filename
        Returns:
            char* filename      filename exactly as entered by the user
    */
    printf("Enter the complete file name: ");

    ssize_t line_size;
    size_t input_buffer_size = 0;
    char* filename = NULL;
    line_size = getline(&filename, &input_buffer_size, stdin);

    filename[strlen(filename)-1] = '\0'; // Remove '\n'

    return filename;
}


// ************************************************ //
// PROCESS FILE //
// ************************************************ //
void process_file(char* selected_file)
{
    /*
        Given a filename, process it into a movie list, create a new directory, and store a list of titles in a new txt file for each year
        Params:
            char* selected_file         filename which will be opened/processed. Assumed to be valid
    */

    // Create a new MovieList to store data from file
    MovieList* movies = (MovieList*)malloc(sizeof(MovieList)); 
    if(movies == NULL)
    {
        printf("ERROR: Could not allocate memory\n");
        perror("Error");
        exit(1);
        return;
    }
    movies->first = NULL;
    movies->size = 0;

    //Parse the movies in the file at filepath into movies
    parse_file(movies, selected_file); 

    // Generate a new directory name and ensure it does not already exist
    char new_dir_path[MAX_DIR_NAME_LENGTH];
    int rand_seed_offset = 0;
    do 
    {
        srand (time(NULL) + rand_seed_offset);
        get_path(new_dir_path);
        rand_seed_offset += 1;
    } while(access(new_dir_path, F_OK) == 0);

    // Make a new directory with the given path
    mkdir(new_dir_path, 0750);

    // Isolate the name of the dir for printing
    char* dir_name = new_dir_path;
    dir_name += 2;
    dir_name[strlen(dir_name)-1] = '\0';

    printf("Created directory with the name %s\n\n", dir_name);

    // Create a file for every year represented in movies with all titles from that year
    create_all_yearly_files(new_dir_path, movies);

    //Free movies and everything it contains
    free_MovieList(movies);
    free(movies);
}

void get_path(char* output_dir_path)
{
    /*
        Generate a new filpath in the form ./ONID_ID.movies.#/ where # is a randomly generated number up to 99999
        Params:
            char* output_dir_path       Output variable to which the new filepath will be assigned
    */

    // Generate a random integer and convert it to a string
    int upper_bound = 99999;
    int rand_number = rand() % (upper_bound + 1);
    char number[6]; 
    sprintf(number, "%d", rand_number); 
    
    // Copy/concatenate all sections of the tile to output_dir_path
    strcpy(output_dir_path, "./");
    strcat(output_dir_path, ONID_ID);
    strcat(output_dir_path, ".movies.");
    strcat(output_dir_path, number);
    strcat(output_dir_path, "/");
}