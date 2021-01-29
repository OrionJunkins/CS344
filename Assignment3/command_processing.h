#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define COMMAND_BUFFER_SIZE 2048
#define MAX_NUM_ARGS 512
bool is_exit_command(char* command);

typedef struct Command {
    char command[COMMAND_BUFFER_SIZE];
    char* arguments[MAX_NUM_ARGS];
    char* infile;
    char* outfile;
    bool background;
} Command;