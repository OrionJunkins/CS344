#define MAX_LINE_SIZE 1000
#define MAX_NUMBER_LINES 50
#include <stdio.h>
#include <stdbool.h>


int main () {
    bool stop_command_recieved = false;

    //Todo: alter size
    char input_buffer[MAX_LINE_SIZE];        // get_input_lines > input_buffer > seperate_lines
    char seperated_buffer[MAX_LINE_SIZE];    // seperate_lines > seperated_buffer > replace_plusses
    char output_buffer[MAX_LINE_SIZE];       // replace_plusses > output_buffer > replace_plusses


    int lines_processed = 0;
    while(!stop_command_recieved && lines_processed < MAX_NUMBER_LINES) 
    {
        // get_input_lines
        // seperate_lines
        // replace_plusses
        // output_lines

        printf("line printing\n");
        lines_processed++;
    }
    return 0;
}