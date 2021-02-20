#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Define maxes for buffer sizes
#define MAX_BUFFER_SIZE 50000 // Entire possible buffer (all lines)
#define MAX_LINE_SIZE 1000    // Max length of a single line

// Store two possible valid stop commands
char * STOP_COMMAND = "STOP\n";             // Case of an isolated stop line
char * NEWLINE_STOP_COMMAND = "\nSTOP\n";   // Case of a stop line appended onto a longer string of input


// Input buffer, mutex and cond for communication between input and separator threads
char input_buffer[MAX_BUFFER_SIZE];       
pthread_mutex_t input_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t input_buffer_has_data = PTHREAD_COND_INITIALIZER;

// Input buffer, mutex and cond for communication between separator and plus sign threads
char separated_buffer[MAX_BUFFER_SIZE];   
pthread_mutex_t separated_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t separated_buffer_has_data = PTHREAD_COND_INITIALIZER;

// Input buffer, mutex and cond for communication between plus sign and output threads
char output_buffer[MAX_BUFFER_SIZE]; 
pthread_mutex_t output_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t output_buffer_has_data = PTHREAD_COND_INITIALIZER;


// Declare all relevant functions for each thread 
/**********************************************************************
 *                         Input Thread                               *
 **********************************************************************/
void* input_thread(void* arg);
    /*
        Pull all data from stdin as it become available
        Store the resulting data in input_buffer
    */

void get_next_line(char* destination);
    /*
        Input Thread helper to pull single lines from stdin
        Clears the given destination and populates it using getline()
    */



/**********************************************************************
 *                    Line Separator Thread                           *
 **********************************************************************/
void* line_separator_thread(void* arg);
    /*
        Pull all data from input_buffer as it become available
        Replace any newline character with a single space
        Store the resulting data in separated_buffer
    */

void replace_newlines(char* output_buffer, char* input_buffer);
    /*
        Copy input to output but replace all newline characters with a single space
    */



/**********************************************************************
 *                       Plus Sign Thread                             *
 *********************************************************************/
void* plus_signal_thread(void* arg);
    /*
        Pull all data from separated_buffer as it become available
        Replace any occurence of "++" with a single "^"
        Store the resulting data in output_buffer
    */

void replace_plusses(char* output_buffer, char* input_buffer);
    /*
        Go through the given input, and copy it to the given output
        For any occurence of "++" replace both '+' chars with a single '^' char and proceed
    */



/**********************************************************************
 *                      Output Data to STDOUT                         *
 **********************************************************************/
void* output_thread(void* arg);
    /*
        Pull from output_buffer
        Output 80 chars at a time to STDOUT as they become available
    */



/**********************************************************************
 *     STOP detection for Separator, Plus Sign and Output Threads     *
 **********************************************************************/
char* pop_stop_suffix_if_present (char *input);
    /*
        Check for either case of stop command and pop/return it if found
    */
