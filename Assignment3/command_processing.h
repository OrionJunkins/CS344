#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#define COMMAND_BUFFER_SIZE 2048
#define MAX_NUM_ARGS 512
#define MAX_BG_PROCESSES 128

int WSTATUS = 0; //check that 0 is correct TODO
bool BACKGROUND_ENABLED = true;


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

typedef struct process_node{
    pid_t PID;
    struct process_node *next;
} process_node;

typedef struct BG_process_list{
    struct process_node * first;
} BG_process_list;



void parse_command(char* input_command, Command* command);
bool is_builtin(Command* command);
void exec_external(Command* command, BG_process_list* active_BG);
void exec_internal(Command* command, BG_process_list* active_BG);
void set_output_stream(Command* command);
void set_input_stream(Command* command);
void status();
Command* new_empty_Command();
void cd(char* arg);

void parent_SIGINT_handler(int num);
void parent_SIGTSTP_handler (int num);
void child_SIGSTP_handler (int num);
void set_SIGTSTP_child();
void set_SIGINT_parent(struct sigaction* SIGINT_parent_action);
void set_SIGTSTP_parent(struct sigaction* SIGTSTP_parent_action);
void set_action_to_default(struct sigaction* action);


void free_process_list(BG_process_list* processes);
void set_SIGCHLD(struct sigaction* SIGCHLD_action);
void SIGCHLD_handler(int num);
void add_process(BG_process_list* processes, pid_t PID);