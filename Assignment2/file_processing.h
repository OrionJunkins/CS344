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
char* prompt_for_name();
char* largest_file_in_cur_dir();
bool filename_matches_format(char* filename);
char* smallest_file_in_cur_dir();
