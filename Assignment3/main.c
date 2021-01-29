#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "command_processing.c"

#define COMMAND_BUFFER_SIZE 2048

int main(){

    // Shell will continue to prompt the user so long as this remains true
    bool shell_is_running = true;

    // Each command recieved will be held here
    char command_buffer[COMMAND_BUFFER_SIZE];
    
    while(shell_is_running){
        
        // Get a new command from stdin
        get_command(command_buffer);

        if(is_exit_command(command_buffer)) {
            shell_is_running = false;
        } else if (is_runnable(command_buffer)){
            execute(command_buffer);
        }
        
        memset(command_buffer, '\0', COMMAND_BUFFER_SIZE);
        
    }

    return 0;
}

