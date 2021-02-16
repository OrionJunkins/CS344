#define MAX_LINE_SIZE 10000 // TODO is this tmp buffer size as well?
#define MAX_NUM_LINES 50
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


/// Only included for dup2 input testing
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
char * STOP_COMMAND = "STOP\n";



//Todo: alter size
char input_buffer[MAX_LINE_SIZE*MAX_NUM_LINES];        // get_input_lines > input_buffer > separate_lines
pthread_mutex_t input_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t input_buffer_has_data = PTHREAD_COND_INITIALIZER;

char separated_buffer[MAX_LINE_SIZE*MAX_NUM_LINES];    // separate_lines > separated_buffer > replace_plusses
pthread_mutex_t separated_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t separated_buffer_has_data = PTHREAD_COND_INITIALIZER;


char output_buffer[MAX_LINE_SIZE];       // replace_plusses > output_buffer > replace_plusses
pthread_mutex_t output_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t output_buffer_has_data = PTHREAD_COND_INITIALIZER;




void replace_newlines(char* output_buffer, char* input_buffer);
void replace_plusses(char* output_buffer, char* input_buffer);