#include "file_processing.h"

int get_selection(char* prompt, int min, int max) 
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
    char* input_buffer = NULL;
    size_t input_buffer_size = 0;
    //input_buffer = (char*)(calloc(input_buffer_size, sizeof(char)));
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

// ************************************************ //
// SELECT FILE //
// ************************************************ //

char* get_file(int selection)
{
    char* filename = NULL;
    switch (selection)
    {
    case 1:
        // Pick the largest file
        filename = largest_file_in_cur_dir();
        break;
    case 2:
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
    
    off_t largest_file_size = -1;
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct dirent *largest_file;
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
                largest_file = aDir;
            }
        }
    }
    return largest_file->d_name;
}
char* smallest_file_in_cur_dir()
{
    off_t smallest_file_size = LONG_LONG_INT_MAX;
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct dirent *smallest_file;
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
                smallest_file = aDir;
            }
        }
    }
    return smallest_file->d_name;
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

char* prompt_for_name()
{
    printf("Enter the complete file name: ");

    ssize_t line_size;
    size_t input_buffer_size = 0;
    char* filename = NULL; //TODO Repeat this structure elsewhere for getline
    line_size = getline(&filename, &input_buffer_size, stdin);

    filename[strlen(filename)-1] = '\0'; // Remove '\n'
    return filename;
}


// ************************************************ //
// PROCESS FILE //
// ************************************************ //
void process_file(char* selected_file)
{
    MovieList* movies = (MovieList*)malloc(sizeof(MovieList)); 
    if(movies == NULL)
    {
        printf("ERROR: Could not allocate memory\n");
        return;
    }
    movies->first = NULL;
    movies->size = 0;

    //Parse the movies in the file at filepath into a list stored in movies
    parse_file(movies, selected_file); 

    // Process the selected file
    //process_file(selected_file);

    char new_dir_path[30];
    
    int rand_seed_offset = 0;
    do {
        srand (time(NULL) + rand_seed_offset);
        rand_seed_offset += 1;
        get_path(new_dir_path);
        printf("%s", new_dir_path);
    } while(access(new_dir_path, F_OK) == 0);

    mkdir(new_dir_path, 0777);

    create_all_yearly_files(new_dir_path, movies);

    //Free movies and everything it contains
    free_MovieList(movies);
    free(movies);
    return;
}

void get_path(char* dir_path)
{
    int upper_bound = 99999;
    
    int rand_number = rand() % (upper_bound + 1);
    char number[6]; 
    sprintf(number, "%d", rand_number); 

    strcpy(dir_path, "./");
    strcat(dir_path, ONID_ID);
    strcat(dir_path, ".movies.");
    strcat(dir_path, number);
    strcat(dir_path, "/");
}