#define MAX_LINE_SIZE 100 // TODO
#define MAX_NUMBER_LINES 50
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

    //Todo: alter size
    char input_buffer[MAX_LINE_SIZE];        // get_input_lines > input_buffer > separate_lines
    pthread_mutex_t input_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t input_buffer_has_data = PTHREAD_COND_INITIALIZER;
    pthread_cond_t input_buffer_is_clear = PTHREAD_COND_INITIALIZER;

    char separated_buffer[MAX_LINE_SIZE];    // separate_lines > separated_buffer > replace_plusses
    pthread_mutex_t separated_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

    
    char output_buffer[MAX_LINE_SIZE];       // replace_plusses > output_buffer > replace_plusses
    pthread_mutex_t output_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
