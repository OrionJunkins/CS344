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
    /*
        Check if a given command is runnable (not a comment or an empty command)
    */
    if (strlen(command) == 0){
        return false;
    } else if (command[0] == '#') {
        return false;
    } else {
        return true;
    }
}


void execute(char* input_command) {
    /*
        Execute the given command
    */
    Command* command = malloc(sizeof(Command)); 
    command->arg_count = 0;

    parse_command(input_command, command);

    if (is_builtin(command)){
        exec_internal(command);
    } else {
        exec_external(command);
    }

    free(command);
}

void parse_command(char* input_command, Command* command) {
    /*
        Tokenize the input_command into a Command
    */
    char* save_ptr;
    char* pt;
    pt = strtok_r(input_command, " ", &save_ptr);
    strcpy(command->command_name, pt);

    pt = strtok_r(NULL, " ", &save_ptr);

    while(pt != NULL){
        if(strcmp("&",pt) == 0){
            command->background = true;
        } else if(strcmp("<",pt) == 0){
            pt = strtok_r(NULL, " ", &save_ptr);
            strcpy(command->infile, pt);
        } else if(strcmp(">",pt) == 0){
            pt = strtok_r(NULL, " ", &save_ptr);
            strcpy(command->outfile, pt);
        } else if(strcmp("$$",pt) == 0){
            //printf(" $$ \n");
        }else{
            strcpy(command->arguments[command->arg_count], pt);            
            command->arg_count += 1;
        }
        pt = strtok_r(NULL, " ", &save_ptr); 
    }
    //for (int i = 0; i < command->arg_count; i++){
    //    printf("arg %d: %s\n", i, command->arguments[i]);
    //}
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

void exec_internal(Command* command){
    /* 
        Given that a command is internally defined, execute it
    */

}

void exec_external(Command* command){
    /* 
        Given that a command is externally defined, execute it
    */
    char* args[MAX_NUM_ARGS];
    args[0] = command->command_name;
    for (int i = 0; i < command->arg_count; i++){
        args[i+1] = command->arguments[i]; 
    }
    args[command->arg_count + 1] = NULL;
    
    //printf("command: %s\n", command->command_name);
    //for (int i = 0; i < command->arg_count + 2; i++){
    //    printf("exec arg %d: %s\n", i, args[i]);
    //}

    int childStatus;
    // Fork a new process
    pid_t spawnPid = fork();

    switch(spawnPid){
        case -1:
            perror("fork()\n");
            exit(1);
            break;
        case 0:
            set_io_streams(command);
            // In the child process
            // Replace the current program with "/bin/ls"
            execvp(args[0], args);            
            // exec only returns if there is an error
            perror("execvp");
            exit(2);
            break;
        default:
            // In the parent process
            // Wait for child's termination
            spawnPid = waitpid(spawnPid, &childStatus, 0);
        break;
    }     
}

void set_io_streams(Command* command) {
    /*
        Reassign stdin and stdout base on the infile and outfile defined in the given command
        If no infile and/or outfile is defined, stdin and/or stdout will not change respectively
    */

    if (strlen(command->infile) > 0){
        int sourceFD = open(command->infile, O_RDONLY);
        if (sourceFD == -1) { 
            perror("source open()"); 
            exit(1); 
        }
        // Redirect stdin to source file
        int result = dup2(sourceFD, 0);
        if (result == -1) { 
            perror("source dup2()"); 
            exit(2); 
        }
    }
    if (strlen(command->outfile) > 0){
        // Open target file //TODO: PERMISSIONS
        int targetFD = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
        if (targetFD == -1) { 
            perror("target open()"); 
            exit(1); 
        }
        // Redirect stdout to target file
        int result = dup2(targetFD, 1);
        if (result == -1) { 
            perror("target dup2()"); 
            exit(2); 
        }
    }
}
