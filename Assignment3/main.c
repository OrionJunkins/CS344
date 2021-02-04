#include "command_processing.c"

#define COMMAND_BUFFER_SIZE 2048

int main(){
    set_SIGINT_parent();
    set_SIGTSTP_parent(); 
    
    // Each command recieved will be held here
    char command_buffer[COMMAND_BUFFER_SIZE];
    
    // Loop until an exit command is executed
    while(1){
        
        // Get a new command from stdin
        memset(command_buffer, '\0', COMMAND_BUFFER_SIZE);
        get_command(command_buffer);

        // Expand variables
        expand_variables(command_buffer);

        // If the command is runnable, execute it
        if (is_runnable(command_buffer)){
            execute(command_buffer);
        }
    }

    return 0;
}
