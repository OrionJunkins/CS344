#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#define COMMAND_BUFFER_SIZE 2048
#define MAX_NUM_ARGS 512

#define NUM_BUILTINS 3
const char BUILTIN_COMMANDS[NUM_BUILTINS][20] = {
    "exit",
    "cd",
    "status",
};


typedef struct Command {
    char command_name[COMMAND_BUFFER_SIZE];
    char arguments[MAX_NUM_ARGS][COMMAND_BUFFER_SIZE];
    int arg_count;
    char infile[COMMAND_BUFFER_SIZE];
    char outfile[COMMAND_BUFFER_SIZE];
    bool background;
    } Command; 

void parse_command(char* input_command, Command* command);
bool is_builtin(Command* command);
bool is_exit_command(char* command);
void exec_external(Command* command);
void exec_internal(Command* command);
void set_io_streams(Command* command);


