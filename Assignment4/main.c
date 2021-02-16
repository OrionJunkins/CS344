#include "line_processor.c"

int main () {
#if 0 // Use a test.txt as stdin
    int sourceFD = open("test.txt", O_RDONLY);
        if (sourceFD == -1) { 
            printf("cannot open for input\n"); 

        }
        // Redirect stdin to target file
        int result = dup2(sourceFD, 0);
        if (result == -1) { 
            perror("cannot open %s for input\n"); 

        }
#endif

    printf("RUNNING\n");

    pthread_t in, sep, proc, out;
    int lines_processed = 0;

    memset(input_buffer, '\0', MAX_LINE_SIZE);

    pthread_create(&in, NULL, get_input_lines, NULL);

    pthread_create(&sep, NULL, separate_lines, NULL);

    pthread_create(&proc, NULL, process_characters, NULL);

    pthread_create(&out, NULL, output_lines, NULL);

    //replace_plusses(separated_buffer, output_buffer);
    //printf("plusses replaced: %s\n", output_buffer);
    //output_lines(output_buffer);
    pthread_join(in, NULL);
    pthread_join(sep, NULL);
    pthread_join(proc, NULL);
    pthread_join(out, NULL);
    return 0;
}