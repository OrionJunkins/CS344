#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 
#define COMMAND_BUFFER_SIZE 2048
#define MAX_NUM_ARGS 512
bool is_exit_command(char* command);

typedef struct Command {
    char command_name[COMMAND_BUFFER_SIZE];
    char arguments[MAX_NUM_ARGS][COMMAND_BUFFER_SIZE];
    char infile[COMMAND_BUFFER_SIZE];
    char outfile[COMMAND_BUFFER_SIZE];
    bool background;
    } Command; 

void parse_command(char* input_command, Command* command);



