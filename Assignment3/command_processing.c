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
        printf("Empty command\n");
        return false;
    } else if (command[0] == '#') {
        printf("Comment\n");
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

   parse_command(input_command, command);

   //redirect_files(command); TODO

   free(command);
}



void parse_command(char* input_command, Command* command) {
    /*
        Tokenize the input_command into a Command
    */
    printf("EXECUTING!!! %s\n", input_command);
    char* save_ptr;
    char* pt;
    pt = strtok_r(input_command, " ", &save_ptr);
    strcpy(command->command_name, pt);

    pt = strtok_r(NULL, " ", &save_ptr);

    int arg_index = 0;
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
            printf(" $$ \n");
        }else{
            strcpy(command->arguments[arg_index], pt);            
            arg_index += 1;
        }
        pt = strtok_r(NULL, " ", &save_ptr); 
    }
}

