#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_BUFFER_SIZE 50000
char * STOP_COMMAND = "STOP\n";
char * NEWLINE_STOP_COMMAND = "\nSTOP\n";


//Todo: alter size
char input_buffer[MAX_BUFFER_SIZE];        // get_input_lines > input_buffer > separate_lines
pthread_mutex_t input_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t input_buffer_has_data = PTHREAD_COND_INITIALIZER;

char separated_buffer[MAX_BUFFER_SIZE];    // separate_lines > separated_buffer > replace_plusses
pthread_mutex_t separated_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t separated_buffer_has_data = PTHREAD_COND_INITIALIZER;



char output_buffer[MAX_BUFFER_SIZE];       // replace_plusses > output_buffer > replace_plusses
pthread_mutex_t output_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t output_buffer_has_data = PTHREAD_COND_INITIALIZER;




void replace_newlines(char* output_buffer, char* input_buffer);
void replace_plusses(char* output_buffer, char* input_buffer);
bool check_stop_conditions(char* buffer_contents);

char* pop_stop_suffix_if_present (char *input){
    if(strlen(input) == 5){
        if(strcmp(input, STOP_COMMAND) == 0){
            input[0] = '\0';
            return STOP_COMMAND;
        }
    } else if(strstr(input, NEWLINE_STOP_COMMAND)){
            char* stop_com_loc = strstr(input, NEWLINE_STOP_COMMAND);
            stop_com_loc[0] = '\0';
            return NEWLINE_STOP_COMMAND;
    } 
    return NULL;
}