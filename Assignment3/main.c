#include "command_processing.c"

#define COMMAND_BUFFER_SIZE 2048

int main(){
    struct sigaction SIGINT_parent_action = {0}, SIGTSTP_parent_action = {0};
    SIGINT_parent_action.sa_handler = parent_SIGINT_handler;
    SIGINT_parent_action.sa_flags = 0;
    sigfillset(&SIGINT_parent_action.sa_mask);
    sigaction(SIGINT, &SIGINT_parent_action, NULL);

    SIGTSTP_parent_action.sa_handler = parent_SIGSTP_handler;
    SIGTSTP_parent_action.sa_flags = 0;
    sigfillset(&SIGTSTP_parent_action.sa_mask);
    sigaction(SIGINT, &SIGTSTP_parent_action, NULL);

    // Each command recieved will be held here
    char command_buffer[COMMAND_BUFFER_SIZE];
    
    // Loop until an exit command is executed
    while(1){
        sigaction(SIGINT, &SIGINT_parent_action, NULL);
        sigaction(SIGTSTP, &SIGTSTP_parent_action, NULL);
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
