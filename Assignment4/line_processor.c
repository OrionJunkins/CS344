#include "line_processor.h"

void get_input_lines(char* input_buffer) {
    
    ssize_t line_size;
    size_t input_buffer_size = MAX_LINE_SIZE;

    // wait for flag that buffer has been emptied
    // Lock
    line_size = getline(&input_buffer, &input_buffer_size, stdin);
    // Unlock
}

void separate_lines(char* input_buffer, char* output_buffer){

    for (int i=0; i <  MAX_LINE_SIZE; i++) {
        if (input_buffer[i] == '\n'){
            output_buffer[i] = ' ';
        } else {
            output_buffer[i] = input_buffer[i];
        }
        //printf("character being process: %d\n", input_buffer[i]);
        //printf("character being outputted: %d\n", output_buffer[i]);
    }
}

void replace_plusses(char* input_buffer, char* output_buffer){
    int input_index = 0;
    int output_index = 0;

    while(input_index < MAX_LINE_SIZE) {
        if(input_buffer[input_index] == '+' && input_buffer[input_index+1] == '+'){
            output_buffer[output_index] = '^';
            input_index += 2;
            output_index += 1;
        } else {
            output_buffer[output_index] = input_buffer[input_index];
            input_index += 1;
            output_index += 1;
        }
    }
}