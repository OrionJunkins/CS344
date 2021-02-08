#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#define COMMAND_BUFFER_SIZE 2048 // Max command input size
#define MAX_NUM_ARGS 512 // Max number of function arguments
#define MAX_BG_PROCESSES 128 // Max number of concurrent background processes

int WSTATUS = 0; // Global status to be checked by builtin status()
bool BACKGROUND_ENABLED = true; // Determines if background processes are allowed


// Store all builtin commands
#define NUM_BUILTINS 3
const char BUILTIN_COMMANDS[NUM_BUILTINS][20] = {

    "exit",
    "cd",
    "status",
};

typedef struct Command {
    /*
        Stores all relevant details for a given command
    */
    char command_name[COMMAND_BUFFER_SIZE];
    char arguments[MAX_NUM_ARGS][COMMAND_BUFFER_SIZE];
    int arg_count;
    char infile[COMMAND_BUFFER_SIZE];
    char outfile[COMMAND_BUFFER_SIZE];
    bool background;
    } Command; 

typedef struct process_node{
    /*
        Node for a linked list of process PIDs
    */
    pid_t PID;
    struct process_node *next;
} process_node;

typedef struct BG_process_list{
    /*
        Stores the head to a linked list of process nodes to track active BG PIDs
    */
    struct process_node * first;
} BG_process_list;


/*****************************************************
 *                Command fetching                   *
 *****************************************************/
void get_command(char* command);
    /*
        Get a command from stdin and save it to the command parameter
    */

void expand_variables(char * command);
    /*
        Search the entire command for occurences of $$ and replace them with the current PID
        Modified string is copied back to command *, that is the original parameter is modified
    */

bool is_runnable(char* command);
    /*
        Check if a given command is runnable (not a comment or an empty command)
    */



/*****************************************************
 *               Command Handling                    *
 *****************************************************/
void execute(char* input_command, BG_process_list* active_BG);
    /*
        Process and execute the given command
    */
    // Generate a completely blank Command

Command* new_empty_Command();
    /*
        Create a new completely empty Command structure with all fields default initialized
    */

void parse_command(char* input_command, Command* command);
    /*
        Tokenize the input_command string into a Command structure
    */

bool is_builtin(Command* command);
    /*
        If the given command has a name which is included in the array of builtins, return true, else false
    */



/*****************************************************
 *          Internal Command Execution               *
 *****************************************************/

void exec_internal(Command* command, BG_process_list* active_BG);
    /* 
        Given that a command is internally defined, execute it
    */
    // Check which internal command was given and handle accordingly

void status();
    /* 
        Based on the global WSTATUS variable, print the most recent program exit value or termination signal
    */

void cd(char* given_path);
    /* 
        Change directory to the specified path or default to the directory in HOME
    */



/*****************************************************
 *          External Command Execution               *
 *****************************************************/

void exec_external(Command* command, BG_process_list* active_BG);
    /* 
        Given that a command is externally defined, execute it
    */

void set_input_stream(Command* command);
    /*
        Reassign stdin bases on the infile defined in the given command
        If no infile is defined, stdin will either:
            remain the same if command is a foreground process
            be set to /dev/null if command is a background process
    */

void set_output_stream(Command* command);
    /*
        Reassign stdout bases on the outfile defined in the given command
        If no outfile is defined, stdout will either:
            remain the same if command is a foreground process
            be set to /dev/null if command is a background process
    */



/*****************************************************
 *                  Signal Handling                  *
 *****************************************************/
void set_SIGTSTP_parent(struct sigaction* action);
    /*
        Set the appropriate functionality for the given sigaction
    */

void parent_SIGTSTP_handler (int num);
    /*
        Toggle background/foreground only mode and print a message giving the new status
    */



/*****************************************************
 *            Active Process Management              *
 *****************************************************/
void add_process(BG_process_list* active_BG, pid_t PID);
    /*
        Given an existing list, add a new PID to it
    */

void free_process_list(BG_process_list* processes);
    /*
        Free all memory allocated to any nodes of an existing process list
    */

void close_finished_bg(BG_process_list* active_BG);
    /*
        Close all finished BG processes and print the status of each
    */
   