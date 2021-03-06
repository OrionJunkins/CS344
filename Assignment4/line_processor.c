#include "line_processor.h"

/**********************************************************************
 *                         Input Thread                               *
 **********************************************************************/
void* input_thread(void* arg) {
    /*
        Pull all data from stdin as it become available
        Store the resulting data in input_buffer
    */
    // Temporary line buffer
    char line_buffer[MAX_LINE_SIZE]; 

    // Set up a conditional loop. Bool goes to true once "STOP\n" is recieved
    bool input_thread_stopped = false; 
    while(!input_thread_stopped){
        // Capture the next line from STDIN into line_buffer
        get_next_line(line_buffer);

        // Check for a stop command and update the loop conditional
        if(strcmp(line_buffer, STOP_COMMAND) == 0){
            input_thread_stopped = true;
        }     

        // Lock the input buffer mutex. If it is already locked, wait here
        pthread_mutex_lock(&input_buffer_mutex); 

        // Concat the line onto whatever is already in input_buffer
        strcat(input_buffer, line_buffer);
        
        // Notify consumer that the input buffer has data
        pthread_cond_signal(&input_buffer_has_data); 

        // Release the lock on input_buffer
        pthread_mutex_unlock(&input_buffer_mutex);
    }
    return NULL;
}

void get_next_line(char* destination){
    /*
        Input Thread helper to pull single lines from stdin
        Clears the given destination and populates it using getline()
    */
        memset(destination, '\0', MAX_LINE_SIZE);
        ssize_t line_size;
        size_t buffer_size = MAX_LINE_SIZE;
        line_size = getline(&destination, &buffer_size, stdin);
}



/**********************************************************************
 *                    Line Separator Thread                           *
 **********************************************************************/
void* line_separator_thread(void* arg){
    /*
        Pull all data from input_buffer as it become available
        Replace any newline character with a single space
        Store the resulting data in separated_buffer
    */
    // Set up two internal buffers to handle data while processing
    char raw_tmp_buffer[MAX_BUFFER_SIZE];
    char processed_tmp_buffer[MAX_BUFFER_SIZE];

    // Set up a conditional loop. Bool goes to true once "STOP\n" is recieved
    bool separation_thread_stopped = false;
    while(!separation_thread_stopped){
        // Clear both internal buffers
        memset(raw_tmp_buffer, '\0', MAX_BUFFER_SIZE);
        memset(processed_tmp_buffer, '\0', MAX_BUFFER_SIZE);
        
        // Lock the input buffer
        pthread_mutex_lock(&input_buffer_mutex); 

        // Wait for the input buffer to have data
        while(strlen(input_buffer) == 0){
            pthread_cond_wait(&input_buffer_has_data, &input_buffer_mutex); 
        }

        // Copy data from the input buffer into the raw temporary buffer
        strcpy(raw_tmp_buffer, input_buffer);
        
        // Clear input buffer
        memset(input_buffer, '\0', MAX_BUFFER_SIZE);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&input_buffer_mutex);

        // Pop off the stop command and anything that follows if one is present
        char* stop_suffix = pop_stop_suffix_if_present(raw_tmp_buffer);

        // Process the data stored in raw tmp, and store it in processed tmp
        replace_newlines(processed_tmp_buffer, raw_tmp_buffer);

        // If a stop command was found, append it back onto the end and modify the loop conditional
        if (stop_suffix != NULL){
            separation_thread_stopped = true;
            strcat(processed_tmp_buffer, stop_suffix);
        }

        // Lock the separated buffer
        pthread_mutex_lock(&separated_buffer_mutex);

        // Copy all processed data into the separated buffer
        strcat(separated_buffer, processed_tmp_buffer);

        // Notify the plus signal thread that the separated buffer has data
        pthread_cond_signal(&separated_buffer_has_data); 
        
        // Unlock the mutex on separated buffer
        pthread_mutex_unlock(&separated_buffer_mutex);
    }
    return NULL;
}


void replace_newlines(char* output_buffer, char* input_buffer){
    /*
        Copy input to output but replace all newline characters with a single space
    */
    // Iterate through the entire input buffer
    for (int i=0; i <  MAX_BUFFER_SIZE; i++) {
        if (input_buffer[i] == '\n'){
            // If a newline is found, copy a space into output
            output_buffer[i] = ' ';
        } else {
            // Otherwise just copy directly
            output_buffer[i] = input_buffer[i];
        }
    }
}



/**********************************************************************
 *                       Plus Sign Thread                             *
 *********************************************************************/
void* plus_signal_thread(void* arg){
    /*
        Pull all data from separated_buffer as it become available
        Replace any occurence of "++" with a single "^"
        Store the resulting data in output_buffer
    */
    /// Set up two internal buffers to handle data while processing
    char raw_tmp_buffer[MAX_BUFFER_SIZE];
    char processed_tmp_buffer[MAX_BUFFER_SIZE];

    // Set up a conditional loop. Bool goes to true once "STOP\n" is recieved
    bool plus_signal_thread_stopped = false;
    while(!plus_signal_thread_stopped){
        // Clear both internal buffers
        memset(raw_tmp_buffer, '\0', MAX_BUFFER_SIZE);
        memset(processed_tmp_buffer, '\0', MAX_BUFFER_SIZE);

        // Lock the separated buffer
        pthread_mutex_lock(&separated_buffer_mutex); 

        // Wait for separated buffer to have data
        while(strlen(separated_buffer) == 0){
            pthread_cond_wait(&separated_buffer_has_data, &separated_buffer_mutex); 
        }

        // Copy data from the separated buffer to the raw temp buffer
        strcpy(raw_tmp_buffer, separated_buffer);
        
        // Clear separated buffer
        memset(separated_buffer, '\0', MAX_BUFFER_SIZE);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&separated_buffer_mutex);

        // Pop off the stop command and anything that follows if one is present
        char* stop_suffix = pop_stop_suffix_if_present(raw_tmp_buffer);

        // Process copied data
        replace_plusses(processed_tmp_buffer, raw_tmp_buffer);

        // If a stop command was found, append it back onto the end and modify the loop conditional
        if (stop_suffix != NULL){
            plus_signal_thread_stopped = true;
            strcat(processed_tmp_buffer, stop_suffix);
        }
        
        // Lock the output buffer mutex
        pthread_mutex_lock(&output_buffer_mutex);

        // copy proccessed data into output buffer
        strcat(output_buffer, processed_tmp_buffer);

        // Notify output thread that output buffer has data 
        pthread_cond_signal(&output_buffer_has_data); 

        // Unlock the mutex
        pthread_mutex_unlock(&output_buffer_mutex);
    }
    return NULL;
}


void replace_plusses(char* output_buffer, char* input_buffer){
    /*
        Go through the given input, and copy it to the given output
        For any occurence of "++" replace both '+' chars with a single '^' char and proceed
    */
    // Track an index in both
    int input_index = 0;
    int output_index = 0;

    // So long as the input is not fully processed
    while(input_index < MAX_BUFFER_SIZE) {
        // So long as we are not on the last char, check if a "++" is present
        if(input_index < MAX_BUFFER_SIZE - 1 && input_buffer[input_index] == '+' && input_buffer[input_index+1] == '+'){
            // Copy only a single '^' if so
            output_buffer[output_index] = '^';
            input_index += 2;
            output_index += 1;
        } else {
            // Otherwisae copy directly
            output_buffer[output_index] = input_buffer[input_index];
            input_index += 1;
            output_index += 1;
        }
    }
}



/**********************************************************************
 *                      Output Data to STDOUT                         *
 **********************************************************************/
void* output_thread(void* arg){
    /*
        Pull from output_buffer
        Output 80 chars at a time to STDOUT as they become available
    */
    // Set up a temporary buffer to store copies of the entire output buffer
    char tmp_buffer[MAX_BUFFER_SIZE];    
    
    // Set up a line of size 80 for outputting individual lines
    char line_buffer[80];

    // Maintain an index into line_buffer to preserve place across multiple pulls from output_buffer
    int line_index = 0;
    
    // Set up a conditional loop. Bool goes to true once "STOP\n" is recieved
    bool output_thread_stopped = false;
    while(!output_thread_stopped){
        // Clear the tmp_bffer
        memset(tmp_buffer, '\0', MAX_BUFFER_SIZE);

        // Lock output buffer
        pthread_mutex_lock(&output_buffer_mutex); 

        // Wait for output buffer to have data
        while(strlen(output_buffer) == 0){
            pthread_cond_wait(&output_buffer_has_data, &output_buffer_mutex); 
        }

        // Copy data from the output_buffer to temp buffer
        strcpy(tmp_buffer, output_buffer);
        
        // Clear output buffer
        memset(output_buffer, '\0', MAX_BUFFER_SIZE);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&output_buffer_mutex);
  
        // Pop a stop command off the end if present
        char* stop_suffix = pop_stop_suffix_if_present(tmp_buffer);

        // Toggle the loop condition if stop command is recieved
        if (stop_suffix != NULL){
            output_thread_stopped = true;
        }

        // Print 80 chars at a time whenever possible
        for(int i = 0; i < strlen(tmp_buffer); i++){
            // Copy the current char
            line_buffer[line_index] = tmp_buffer[i];
            if (line_index >= 79){
                // If a line length of 80 has been reached, print the line
                write(STDOUT_FILENO, line_buffer, 80);
                write(STDOUT_FILENO, "\n", 1);

                // Clear line_buffer and reset its index to the start
                memset(line_buffer, '\0', 80);
                line_index = 0;
            } else {
                // Increment the line buffer index
                line_index++;
            }
        }
    }
    return NULL;
}



/**********************************************************************
 *     STOP detection for Separator, Plus Sign and Output Threads     *
 **********************************************************************/
char* pop_stop_suffix_if_present (char *input){
    /*
        Check for either case of stop command and pop/return it if found
    */
    if(strlen(input) == 5){
        // If the entire buffer is a stop command, pop and return it
        if(strcmp(input, STOP_COMMAND) == 0){
            input[0] = '\0';
            return STOP_COMMAND;
        }
    } else if(strstr(input, NEWLINE_STOP_COMMAND)){
            // If a stop command appears in the line, pop and return it
            char* stop_com_loc = strstr(input, NEWLINE_STOP_COMMAND);
            stop_com_loc[0] = '\0';
            return NEWLINE_STOP_COMMAND;
    } 
    // No stop command found, return NULL
    return NULL;
}
