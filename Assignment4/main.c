#include "line_processor.c"

int main () {
    bool stop_command_recieved = false;

    //Todo: alter size
    char input_buffer[MAX_LINE_SIZE];        // get_input_lines > input_buffer > separate_lines
    char separated_buffer[MAX_LINE_SIZE];    // separate_lines > separated_buffer > replace_plusses
    char output_buffer[MAX_LINE_SIZE];       // replace_plusses > output_buffer > replace_plusses


    int lines_processed = 0;
    while(!stop_command_recieved && lines_processed < MAX_NUMBER_LINES) 
    {
        memset(input_buffer, '\0', MAX_LINE_SIZE);
        get_input_lines(input_buffer);
        printf("input gotten: %s\n", input_buffer);
        separate_lines(input_buffer, separated_buffer);
        printf("lines separated: %s\n", separated_buffer);
        replace_plusses(separated_buffer, output_buffer);
        printf("plusses replaced: %s\n", output_buffer);
        // output_lines

        lines_processed++;
    }
    return 0;
}