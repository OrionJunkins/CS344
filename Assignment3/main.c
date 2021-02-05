#include "command_processing.c"

#define COMMAND_BUFFER_SIZE 2048

int main(){
    struct sigaction ignore_action = {0}, child_termination_action = {0}, background_mode_toggle_action = {0};

    set_SIGTSTP_parent(&background_mode_toggle_action);
    ignore_action.sa_handler = SIG_IGN; 

    sigaction(SIGINT, &ignore_action, NULL);
    sigaction(SIGTSTP, &background_mode_toggle_action, NULL);

    BG_process_list* active_BG = (BG_process_list*)(malloc(sizeof(BG_process_list)));
    active_BG->first = NULL;


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
            execute(command_buffer, active_BG);
        }
        // Close finished BG processes
        close_finished_bg(active_BG);
    }

    free_process_list(active_BG);

    return 0;
}
