#include "command_processing.c"

#define COMMAND_BUFFER_SIZE 2048

int main(){
    struct sigaction SIGINT_action = {0}, SIGTSTP_action = {0}, SIGCHLD_action = {0};
    set_SIGINT_parent(&SIGINT_action);
    set_SIGTSTP_parent(&SIGTSTP_action); 
    set_SIGCHLD(&SIGCHLD_action);

    sigaction(SIGINT, &SIGINT_action, NULL);
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);
    sigaction(SIGCHLD, &SIGCHLD_action, NULL);


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
