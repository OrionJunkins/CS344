#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "command_processing.c"

#define COMMAND_BUFFER_SIZE 2048

int main(){

    bool shell_is_running = true;
    char command[COMMAND_BUFFER_SIZE];
    

    while(shell_is_running){
        
        get_command(command);


        if(is_exit_command(command)) {
            shell_is_running = false;
        }
        memset(command, '\0', COMMAND_BUFFER_SIZE);
    }

    

}

