#include "command_processing.h"

/*****************************************************
 *                Command fetching                   *
 *****************************************************/
void get_command(char* command){
    /*
        Get a command from stdin and save it to the command parameter
    */
    // Print initial shell prompt
    printf(":");
    // Get a line from the user
    ssize_t recieved_command_size;
    char* command_buffer = command;
    size_t command_buffer_size = COMMAND_BUFFER_SIZE;
    recieved_command_size = getline(&command_buffer, &command_buffer_size, stdin);

    // Replace trailing '\n' from getline() with '\0'
    command[recieved_command_size - 1] = '\0';    
}

void expand_variables(char * command) {
    /*
        Search the entire command for occurences of $$ and replace them with the current PID
        Modified string is copied back to command *, that is the original parameter is modified
    */
    // Get a string representation of the current PID
    unsigned int pidLength = sizeof(pid_t);
    char PID[pidLength];
    sprintf(PID, "%d", getpid());

    // Create a new empty array with its own index
    char expanded_command[COMMAND_BUFFER_SIZE];
    int expanded_index = 0;

    // Iterate over the original command
    for(int orig_index = 0; orig_index < COMMAND_BUFFER_SIZE; orig_index++){
        // If "$$" is found, copy the PID
        if (command[orig_index] == '$' && command[orig_index + 1] == '$'){
            for (int i = 0; i < strlen(PID); i++){
                expanded_command[expanded_index + i] = PID[i];
            }
            // Update both indexes
            orig_index += 1;
            expanded_index += strlen(PID);

        // Otherwise, copy the characters exactly and proceed
        } else {
            expanded_command[expanded_index] = command[orig_index];
            expanded_index += 1;
        } 
    }
    // Copy the new command back to the original source (modify it)
    strcpy(command, expanded_command);
}

bool is_runnable(char* command) { 
    /*
        Check if a given command is runnable (not a comment or an empty command)
    */
    // Check for empty line
    if (strlen(command) == 0){
        return false;
    // Check for comment
    } else if (command[0] == '#') {
        return false;
    // Otherwise, command is runnable
    } else {
        return true;
    }
}


/*****************************************************
 *               Command Handling                    *
 *****************************************************/
void execute(char* input_command, BG_process_list* active_BG) { 
    /*
        Process and execute the given command
    */
    // Generate a completely blank Command
    Command* command = new_empty_Command();

    // Parse the input_command string into the Command structure
    parse_command(input_command, command);

    // Decide if the command a builtin or not and execute accordingly
    if (is_builtin(command)){
        exec_internal(command, active_BG);
    } else {
        exec_external(command, active_BG);
    }
    
    // Free the malloc'd command
    free(command);
}

Command* new_empty_Command(){
    /*
        Create a new completely empty Command structure with all fields default initialized
    */
    // Malloc new Command
    Command* command = (Command*)(malloc(sizeof(Command))); 

    // Initialize all fields
    memset(command->arguments, '\0', sizeof(char) * MAX_NUM_ARGS*COMMAND_BUFFER_SIZE );
    command->arg_count = 0;
    strcpy(command->infile, "");
    strcpy(command->outfile, "");
    command->background = false;

    // Return new empty command
    return command;
}

void parse_command(char* input_command, Command* command) {
    /*
        Tokenize the input_command string into a Command structure
    */
    // Setup for strtok_r
    char* save_ptr;
    char* pt;

    // Copy the name of the command
    pt = strtok_r(input_command, " ", &save_ptr);
    strcpy(command->command_name, pt);

    // Continue parsing and updating command fields until NULL is found
    pt = strtok_r(NULL, " ", &save_ptr);
    while(pt != NULL){
        if(strcmp("<",pt) == 0){         // Input redirection
            pt = strtok_r(NULL, " ", &save_ptr);
            strcpy(command->infile, pt);
        } else if(strcmp(">",pt) == 0){         // Output redirection
            pt = strtok_r(NULL, " ", &save_ptr);
            strcpy(command->outfile, pt);
        } else{         // Just another argument to be added to the list
            strcpy(command->arguments[command->arg_count], pt);            
            command->arg_count += 1;
        }
        // Move to the next 
        pt = strtok_r(NULL, " ", &save_ptr); 
    }

    if(strcmp(command->arguments[command->arg_count - 1], "&") == 0){
        if (BACKGROUND_ENABLED){
            command->background = true;
        }
        strcpy(command->arguments[command->arg_count - 1], "\0");
        command->arg_count -= 1;
    }
}

bool is_builtin(Command* command){
    /*
        If the given command has a name which is included in the array of builtins, return true, else false
    */
    for (int i = 0; i < NUM_BUILTINS; i++){
        if(strcmp(BUILTIN_COMMANDS[i], command->command_name) == 0){
            return true;
        }
    }
    return false;
}



/*****************************************************
 *          Internal Command Execution               *
 *****************************************************/

void exec_internal(Command* command, BG_process_list* active_BG){
    /* 
        Given that a command is internally defined, execute it
    */
    // Check which internal command was given and handle accordingly
    if (strcmp(command->command_name, "cd") == 0){
        cd(command->arguments[0]);
    } else if (strcmp(command->command_name, "status") == 0){
        status();
    } else if (strcmp(command->command_name, "exit") == 0){
        // Free all malloc'd resources and exit
        free_process_list(active_BG);
        free(active_BG);
        free(command);
        exit(0);
    } else {
        printf("Unknown internal command");
        exit(1);
    }
}

void status(){
    /* 
        Based on the global WSTATUS variable, print the most recent program exit value or termination signal
    */
    if(WIFEXITED(WSTATUS)){
      printf("exit value %d\n", WEXITSTATUS(WSTATUS));
    } else{
      printf("terminated by signal %d\n", WTERMSIG(WSTATUS));
    }
}

void cd(char* given_path) {
    /* 
        Change directory to the specified path or default to the directory in HOME
    */
    // Create a new blank dir_path string
    char dir_path[COMMAND_BUFFER_SIZE];
    memset( dir_path, '\0', sizeof(char)*COMMAND_BUFFER_SIZE );

    // If the arg is blank, get the environment variable for HOME and copy it to dir_path
    if(strcmp(given_path, "") == 0){
        strcpy(dir_path, getenv("HOME"));

    // Otherwise copy the give path
    } else {
        strcpy(dir_path, given_path);
    }

    // Change the current directory
    chdir(dir_path);
}



/*****************************************************
 *          External Command Execution               *
 *****************************************************/

void exec_external(Command* command, BG_process_list* active_BG){
    /* 
        Given that a command is externally defined, execute it
    */
    // Produce an argument array for an exec function from command->arguments
    char* args[MAX_NUM_ARGS];
    args[0] = command->command_name;
    for (int i = 0; i < command->arg_count; i++){
        args[i+1] = command->arguments[i]; 
    }
    args[command->arg_count + 1] = NULL;

    // Set sig handlers to be used inside of the child
    struct sigaction default_action = {0}; //, ignore_action = {0}
    default_action.sa_handler = SIG_DFL;
    //ignore_action.sa_handler = SIG_IGN;  TODO - delete if tests are being passed

    // Mask SIGTSTP while foreground process is active
    sigset_t ignore_while_fg_active = {0};
    sigaddset(&ignore_while_fg_active, SIGTSTP);
    sigprocmask(SIG_BLOCK, &ignore_while_fg_active, NULL);

    // Fork a new process
    pid_t spawnPid = fork();
    switch(spawnPid){
        case -1:
            perror("fork() failure\n");
            exit(1);
            break;
        case 0:
            // In the child process
            if(!command->background){ 
                // Foreground process, so set SIGINT to default termination handler
                sigaction(SIGINT, &default_action, NULL);
            }

            // Regardless of FG/BG, set SIGTSTP to be ignored.  TODO?
            //sigaction(SIGTSTP, &ignore_action, NULL);
            
            // Update IO streams based on command specification
            set_input_stream(command);
            set_output_stream(command);

            // Execute the command
            execvp(args[0], args);   

            // Print an error and exit if the command could not be executed
            printf("%s: no such file or directory\n", command->command_name);        
            exit(1);
            break;
        default:
            // In the parent process
            // If command is a background process, print the child PID and add the process to the active_BG list
            if (command->background){
                printf("background pid is %d\n", spawnPid);
                add_process(active_BG, spawnPid);
            } else {
                // It is a foreground process, so wait for the child to terminate
                spawnPid = waitpid(spawnPid, &WSTATUS, 0);
                if (spawnPid > 0 && WIFSIGNALED(WSTATUS)) {
                    printf("terminated by signal %d\n", WTERMSIG(WSTATUS));
                }
                
            }
            // Unblock SIGTSTP
            sigprocmask(SIG_UNBLOCK, &ignore_while_fg_active, NULL);
            break;
    }  
}

void set_input_stream(Command* command) {
    /*
        Reassign stdin bases on the infile defined in the given command
        If no infile is defined, stdin will either:
            remain the same if command is a foreground process
            be set to /dev/null if command is a background process
    */

    // If an infile is present, open it and redirect stdin
    if (strlen(command->infile) > 0){
        // Open target file
        int sourceFD = open(command->infile, O_RDONLY);
        if (sourceFD == -1) { 
            printf("cannot open %s for input\n", command->infile); 
            exit(1); 
        }
        // Redirect stdin to target file
        int result = dup2(sourceFD, 0);
        if (result == -1) { 
            perror("cannot open %s for input\n"); 
            exit(1); 
        }
    } else if (command->background){
        // No infile is present, set stdin to /dev/null
        int devNull = open("/dev/null", O_RDONLY);
        int result = dup2(devNull, 1);
    }
}

void set_output_stream(Command* command) {
    /*
        Reassign stdout bases on the outfile defined in the given command
        If no outfile is defined, stdout will either:
            remain the same if command is a foreground process
            be set to /dev/null if command is a background process
    */

    // If an outfile is present, open it and redirect stdin
    if (strlen(command->outfile) > 0){
        // Open target file //TODO: PERMISSIONS
        int targetFD = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
        if (targetFD == -1) { 
            printf("cannot open %s for output\n", command->outfile); 
            exit(1); 
        }
        // Redirect stdout to target file
        int result = dup2(targetFD, 1);
        if (result == -1) { 
            perror("cannot open %s for input\n"); 
            exit(1); 
        }
    } else if (command->background) {
        // No infile is present, set stdin to /dev/null
        int devNull = open("/dev/null", O_RDONLY);
        int result = dup2(devNull, 1);
    }
}


/*****************************************************
 *                  Signal Handling                  *
 *****************************************************/

void set_SIGTSTP_parent(struct sigaction* action){
    /*
        Set the appropriate functionality for the given sigaction
    */
    action->sa_handler = parent_SIGTSTP_handler;
    action->sa_flags = SA_RESTART;
    sigfillset(&action->sa_mask);
}

void parent_SIGTSTP_handler (int num) {
    /*
        Toggle background/foreground only mode and print a message giving the new status
    */
    if(BACKGROUND_ENABLED){
        write(STDOUT_FILENO, "Entering foreground-only mode (& is now ignored)\n", 50);
        BACKGROUND_ENABLED = false;
    } else {
        write(STDOUT_FILENO, "Exiting foreground-only mode\n", 30);
        BACKGROUND_ENABLED = true;
    }
}


/*****************************************************
 *            Active Process Management              *
 *****************************************************/
void add_process(BG_process_list* active_BG, pid_t PID){
    /*
        Given an existing list, add a new PID to it
    */
    // Create a new process_node with the given PID
    process_node* new_node = (process_node*)(malloc(sizeof(process_node)));
    new_node->PID = PID;

    // Place the new node at the front of the list
    new_node->next = active_BG->first;
    active_BG->first = new_node;
}

void free_process_list(BG_process_list* processes){
    /*
        Free all memory allocated to any nodes of an existing process list
    */
    process_node* cur_node = processes->first;
    process_node* tmp;
    while(cur_node != NULL){
        tmp = cur_node;
        cur_node = cur_node->next;
        free(tmp);
    }
}

//TODO currently only bg processes are in the list, so there is no notification for fg child death -> resolved?
void close_finished_bg(BG_process_list* active_BG){
    /*
        Close all finished BG processes and print the status of each
    */
    // Create a ptr to the first node in the list and iterate the entire list
    process_node* node_ptr;
    node_ptr = active_BG->first;
    int wstatus = 0;
    while(node_ptr != NULL){
        // Wait for the current PID to see if it is complete
        int wait_result = waitpid(node_ptr->PID, &wstatus, WNOHANG);

        // If it is completed, print the exit status
        if (wait_result > 0){
            printf("background pid %d is done: ", node_ptr->PID);
            if(WIFEXITED(wstatus)){
                printf("exit value %d\n",  WEXITSTATUS(wstatus));
            } else{
                printf("terminated by signal %d\n", WTERMSIG(wstatus));
            }
        }
        node_ptr = node_ptr->next;
    }
}