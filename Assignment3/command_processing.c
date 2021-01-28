#include "command_processing.h"

void get_command(char* command){
    /*
        Get a command from stdin and save it to the command parameter
    */
    printf(":");
    ssize_t recieved_command_size;
    char* command_buffer = command;
    size_t command_buffer_size = COMMAND_BUFFER_SIZE;
    
    recieved_command_size = getline(&command_buffer, &command_buffer_size, stdin);
    command[recieved_command_size - 1] = '\0';    
}

bool is_exit_command(char* command) {
    /*
        Given a command, see if is an exit command to decide if the shell should be exited
    */
    if(strcmp(command, "exit") == 0){
        return true;
    } else{
        return false;
    }
}

bool is_runnable(char* command) {
    if (strlen(command) == 0){
        printf("Empty command\n");
        return false;
    } else if (command[0] == '#') {
        printf("Comment\n");
        return false;
    } else {
        return true;
    }
}

void execute(char* command) {
    /*
        Execute the given command
    */
   printf("EXECUTING!!! %s\n", command);
}