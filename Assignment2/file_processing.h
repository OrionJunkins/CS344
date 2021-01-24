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
#define MAX_FILENAME_LENGTH 100
#define PREFIX "movies_"
#define SUFFIX ".csv"
#define LONG_LONG_INT_MAX 9223372036854775807
#define ONID_ID "junkinso"
#define MAX_DIR_NAME_LENGTH 100
char* prompt_for_name();
char* largest_file_in_cur_dir();
bool filename_matches_format(char* filename);
char* smallest_file_in_cur_dir();
void get_path(char* dir_path);
