#include "line_processor.c"

int main () {
    bool stop_command_recieved = false;

    

    pthread_t p, c;
    int lines_processed = 0;
    while(!stop_command_recieved && lines_processed < MAX_NUMBER_LINES) 
    {
        memset(input_buffer, '\0', MAX_LINE_SIZE);

        pthread_create(&p, NULL, get_input_lines, NULL);

        pthread_create(&c, NULL, separate_lines, NULL);

        //printf("lines separated: %s\n", separated_buffer);
        //replace_plusses(separated_buffer, output_buffer);
        //printf("plusses replaced: %s\n", output_buffer);
        //output_lines(output_buffer);
        pthread_join(p, NULL);
        pthread_join(c, NULL);
        lines_processed++;
    }
    return 0;
}