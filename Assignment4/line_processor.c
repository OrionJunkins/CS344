#include "line_processor.h"

void* get_input_lines(void* arg) {
    char line_buffer[MAX_LINE_SIZE];
    while(!input_thread_stopped){
        // Get a line to a temporary buffer
        memset(line_buffer, '\0', MAX_LINE_SIZE);
        char* getline_buffer = line_buffer;
        ssize_t line_size;
        size_t buffer_size = MAX_LINE_SIZE;
        line_size = getline(&getline_buffer, &buffer_size, stdin);
        if(strcmp(getline_buffer, STOP_COMMAND) == 0){
            printf("YAY");
            input_thread_stopped = true;
        }
        

        // Lock the input buffer mutex. If it is already locked, wait here
        pthread_mutex_lock(&input_buffer_mutex); 
        printf("%s\n", line_buffer);
        // Copy the line gotten to input_buf
        strcat(input_buffer, line_buffer);
        
        // Notify consumer that input_buf has data
        pthread_cond_signal(&input_buffer_has_data); 

        // Release the lock
        pthread_mutex_unlock(&input_buffer_mutex);
    }
    input_thread_stopped = true;
    return NULL;
}


/**********************************************************************
 *                          *
 **********************************************************************/

void* separate_lines(void* arg){
    bool stop_command_received = false;
    char raw_tmp_buffer[MAX_LINE_SIZE];
    char processed_tmp_buffer[MAX_LINE_SIZE];
    while(!stop_command_received){
        memset(raw_tmp_buffer, '\0', MAX_LINE_SIZE);
        memset(processed_tmp_buffer, '\0', MAX_LINE_SIZE);
        // Lock buf 1
        pthread_mutex_lock(&input_buffer_mutex); 

        // Wait for input_buf to have data
        while(strlen(input_buffer) == 0){
            pthread_cond_wait(&input_buffer_has_data, &input_buffer_mutex); 
        }

        // We now have the lock
        // Copy data to temp buffer
        strcpy(raw_tmp_buffer, input_buffer);
        
        // Clear input buffer
        memset(input_buffer, '\0', MAX_LINE_SIZE);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&input_buffer_mutex);

        
        // Process copied data
        replace_newlines(processed_tmp_buffer, raw_tmp_buffer);

        // Lock buf 2 -> will wait and make sure lock is available. RP thread should be ready generally
        pthread_mutex_lock(&separated_buffer_mutex);

        // copy proccessed into buf 2
        strcat(separated_buffer, processed_tmp_buffer);

        // Notify RP thread that buf 2 has data 
        pthread_cond_signal(&separated_buffer_has_data); 
        // unlock

        pthread_mutex_unlock(&separated_buffer_mutex);
        printf("SEP LINES GOT: %s\n", processed_tmp_buffer);
        stop_command_received = check_stop_conditions(processed_tmp_buffer);
    }
    printf("THREAD 2 EXITTED\n");
    return NULL;
}

bool check_stop_conditions(char* buffer_contents){
    int len = strlen(buffer_contents);
    if (len >= 5){
        char* end_of_contents = buffer_contents + len - 5;
        char* substring;
        substring = strstr(end_of_contents, "STOP");
        if(substring != NULL                // "STOP\0" is the end of the string
            && strlen(input_buffer) == 0    // Input buffer is empty
            && input_thread_stopped){        // Input thread has already stopped
        
            return true;
        }
    }
    return false;
}


void replace_newlines(char* output_buffer, char* input_buffer){

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





/**********************************************************************
 *                          *
 **********************************************************************/
void* process_characters(void* arg){
    char raw_tmp_buffer[MAX_LINE_SIZE];
    char processed_tmp_buffer[MAX_LINE_SIZE];
    int i = 0; // TODO make infinite?
    while(i < MAX_NUM_LINES){
        memset(raw_tmp_buffer, '\0', MAX_LINE_SIZE);
        memset(processed_tmp_buffer, '\0', MAX_LINE_SIZE);
        // Lock separated buffer
        pthread_mutex_lock(&separated_buffer_mutex); 

        // Wait for separated buffer to have data
        while(strlen(separated_buffer) == 0){
            pthread_cond_wait(&separated_buffer_has_data, &separated_buffer_mutex); 
        }

        // We now have the lock
        // Copy data to temp buffer
        strcpy(raw_tmp_buffer, separated_buffer);
        
        // Clear separated buffer
        memset(separated_buffer, '\0', MAX_LINE_SIZE);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&separated_buffer_mutex);

        // Process copied data
        
        replace_plusses(processed_tmp_buffer, raw_tmp_buffer);
        //printf("RP LINES GOT: %s\n", processed_tmp_buffer);
        // Lock output buffer-> will wait and make sure lock is available. RP thread should be ready generally
        
        pthread_mutex_lock(&output_buffer_mutex);

        // copy proccessed data into output buffer
        strcat(output_buffer, processed_tmp_buffer);

        // Notify output thread that output buffer has data 
        pthread_cond_signal(&output_buffer_has_data); 
        // unlock

        pthread_mutex_unlock(&output_buffer_mutex);
        i++;
    }
    return NULL;
}


void replace_plusses(char* output_buffer, char* input_buffer){
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

void* output_lines(void* arg){
    char tmp_buffer[MAX_LINE_SIZE];    
    char line_buffer[80];
    int line_index = 0;

    int lines_processed = 0; // TODO make infinite?
    while(lines_processed < MAX_NUM_LINES){
        memset(tmp_buffer, '\0', MAX_LINE_SIZE);

        // Lock output buffer
        pthread_mutex_lock(&output_buffer_mutex); 

        // Wait for output buffer to have data
        while(strlen(output_buffer) == 0){
            pthread_cond_wait(&output_buffer_has_data, &output_buffer_mutex); 
        }

        // We now have the lock
        // Copy data to temp buffer
        strcpy(tmp_buffer, output_buffer);
        
        // Clear output buffer
        memset(output_buffer, '\0', MAX_LINE_SIZE);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&output_buffer_mutex);

        //printf("OUTPUT LINES GOT: %s\n", tmp_buffer);
        
        ////////////

        int i = 0;
        while(i < strlen(tmp_buffer)){
            line_buffer[line_index] = tmp_buffer[i];
            if (line_index >= 79){
                //printf("line_index %d\n", line_index);
                write(STDOUT_FILENO, line_buffer, 80);
                //printf("line outputed: %s\n", line_buffer);
                memset(line_buffer, '\0', 80);
                
                line_index = 0;
            } else {
                line_index++;
            }
            i++;
        }
        //printf("Leftovers in line buffer: %s\n", line_buffer);

        lines_processed++;
    }
    return NULL;
}