#include "line_processor.h"

void* get_input_lines(void* arg) {
    bool stop_command_recieved = false;
    char line_buffer[MAX_LINE_SIZE];
    int i = 0;
    while(!stop_command_recieved){
        // Get a line to a temporary buffer
        memset(line_buffer, '\0', MAX_LINE_SIZE);
        char* getline_buffer = line_buffer;
        ssize_t line_size;
        size_t buffer_size = MAX_LINE_SIZE;
        line_size = getline(&getline_buffer, &buffer_size, stdin);
        if(strcmp(getline_buffer, STOP_COMMAND) == 0){
            stop_command_recieved = true;
        }
        //printf("%s\n", line_buffer);

        // Lock the input buffer mutex. If it is already locked, wait here
        pthread_mutex_lock(&input_buffer_mutex); 
        // Copy the line gotten to input_buf
        strcat(input_buffer, line_buffer);
        
        // Notify consumer that input_buf has data
        pthread_cond_signal(&input_buffer_has_data); 

        // Release the lock
        pthread_mutex_unlock(&input_buffer_mutex);
        printf("%s\n", line_buffer);
        i++;
    }

    return NULL;
}


/**********************************************************************
 *                          *
 **********************************************************************/

void* separate_lines(void* arg){
    char raw_tmp_buffer[MAX_LINE_SIZE];
    char processed_tmp_buffer[MAX_LINE_SIZE];
    int i = 0;
    while(i < MAX_NUM_LINES){
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
        memset(input_buffer, '\0', MAX_LINE_SIZE * MAX_NUM_LINES);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&input_buffer_mutex);

        printf("SEP LINES GOT: %s", raw_tmp_buffer);
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
        i++;
    }
    return NULL;
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
    int i = 0;
    while(i < MAX_NUM_LINES){
        memset(raw_tmp_buffer, '\0', MAX_LINE_SIZE);
        memset(processed_tmp_buffer, '\0', MAX_LINE_SIZE);
        // Lock buf 1
        pthread_mutex_lock(&separated_buffer_mutex); 

        // Wait for separated buffer to have data
        while(strlen(separated_buffer) == 0){
            pthread_cond_wait(&separated_buffer_has_data, &separated_buffer_mutex); 
        }

        // We now have the lock
        // Copy data to temp buffer
        strcpy(raw_tmp_buffer, separated_buffer);
        
        // Clear separated buffer
        memset(separated_buffer, '\0', MAX_LINE_SIZE * MAX_NUM_LINES);

        // Unlock the buffer for the producer
        pthread_mutex_unlock(&separated_buffer_mutex);

        // Process copied data
        
        replace_plusses(processed_tmp_buffer, raw_tmp_buffer);
        printf("RP LINES GOT: %s\n", processed_tmp_buffer);
        // Lock buf 2 -> will wait and make sure lock is available. RP thread should be ready generally
        /*
        pthread_mutex_lock(&separated_buffer_mutex);

        // copy proccessed into buf 2
        strcat(separated_buffer, processed_tmp_buffer);

        // Notify RP thread that buf 2 has data 
        pthread_cond_signal(&separated_buffer_has_data); 
        // unlock

        pthread_mutex_unlock(&separated_buffer_mutex);
        */
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

void output_lines(char* output_buffer){
    // as soon as the 
}