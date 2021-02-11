#include "line_processor.h"

void* get_input_lines(void* arg) {
    
    char tmp_buffer[MAX_LINE_SIZE];
    while(1){
        // Get a line to a temporary buffer
        memset(tmp_buffer, '\0', MAX_LINE_SIZE);
        char* getline_buffer = tmp_buffer;
        ssize_t line_size;
        size_t buffer_size = MAX_LINE_SIZE;
        line_size = getline(&getline_buffer, &buffer_size, stdin);
        // Lock the input buffer mutex. If it is already locked, wait here
        pthread_mutex_lock(&input_buffer_mutex); 

        // Copy the line gotten to input_buf
        strcpy(input_buffer, tmp_buffer);
        
        // Notify consumer that input_buf has data
        pthread_cond_signal(&input_buffer_has_data); 

        // Wait until consumer empties the buffer
        pthread_cond_wait(&input_buffer_is_clear, &input_buffer_mutex); 

        // Release the lock
        pthread_mutex_unlock(&input_buffer_mutex);
    }

    return NULL;
}


/**********************************************************************
 *                          *
 **********************************************************************/

void* separate_lines(void* arg){
    char raw_tmp_buffer[MAX_LINE_SIZE];
    char processed_tmp_buffer[MAX_LINE_SIZE];

    while(1){
    memset(raw_tmp_buffer, '\0', MAX_LINE_SIZE);
    memset(processed_tmp_buffer, '\0', MAX_LINE_SIZE);
    // Lock buf 1
    pthread_mutex_lock(&input_buffer_mutex); 

    // Wait for input_buf to have data
    pthread_cond_wait(&input_buffer_has_data, &input_buffer_mutex); 

    // We now have the lock
    // Copy data to temp buffer
    strcpy(raw_tmp_buffer, input_buffer);
    
    // Signal the producer that the buffer is clear
    pthread_cond_signal(&input_buffer_is_clear);

    // Unlock the buffer for the producer
    pthread_mutex_unlock(&input_buffer_mutex);

    printf("SEP LINES GOT: %s", raw_tmp_buffer);
    // Process copied data
    //replace_newlines(raw_tmp_buffer)
    // Lock buf 2 -> will wait and make sure lock is available. RP thread should be ready generally
    // copy proccessed into buf 2
    // Notify RP thread that buf 2 has data 
    // unlock
    }
}


void replace_newlines(char* input_buffer, char* output_buffer){

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

void output_lines(char* output_buffer){
    // as soon as the 
}