/*
    Header file with all needed library includes for text handling.
    Defines functions for basic file parsing and text checking.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>  
#include <stdbool.h>

bool isValidChar(char c){
    /*
        Check if a given char is valid
    */
    // Convert it to an int and store an ascii representation
    int asciiVal = (int)c;

    // If space, null terminator or asciiVal is in [65, 90] return true
    if (c == ' '){
        return true;
    } else if (c == '\0'){
        return true;
    }else if (65 <= asciiVal && asciiVal <= 90){
        return true;
    } else {
        return false;
    }
}


char* getFileText(char* filename){
    /*
        Given a filename, open the file and process it one char at a time.
        If a char is valid, add it to the output.
        Once the end of the file is reached, return the produced output.
    */
    // Open the file
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        fprintf(stderr, "Error: could not locate '%s'\n", filename);
        exit(1); 
    }
    
    // Get the file length
    struct stat st;
    if(stat(filename, &st) != 0) {
        return 0;
    }
    long fileLength = st.st_size; 
    
    // Create an appropriately sized destination string
    char* dest = (char*)calloc(fileLength, sizeof(char));

    // Create temp variables for the current char
    int cur;
    char curChar;

    // Track the array index as data is copied
    int index = 0;

    // So long as the EOF and/or a newline are not reached, get the next char
    while((cur = fgetc(file)) != EOF && cur != '\n'){
        // cast to char
        curChar = (char)cur;

        // Check that it is valid - copy it or raise an error accordingly
        if(isValidChar(curChar)){
            dest[index] = curChar;
        } else{
            fprintf(stderr, "Error: %s contains bad characters\n", filename);
            exit(1); 
        }
        // Bump the index
        index++;
    }
    return dest;
}


void convertFromAscii(char* input, int* output){
    /*
        Convert from ascii to a numeric representation such that:
            A, B, ..., Z corresponds to 1, 2, ..., 25 respectively
            ' ' (space) corresponds to 26
    */
   // For every char in the input,
    for (int i = 0; i < strlen(input); i++){
        // If it is a space, copy 26
        if(input[i] == ' '){
            output[i] = 26;
        } else {
            // Else subtract 65 from the ascii value to yield an alphabet index relative to 0
            int currentCharacter = (int)input[i];
            output[i] = currentCharacter - 65;
        }
    }
}


char convertToAscii(int number){
    /*
        Convert a char to ascii from a numeric representation such that:
            A, B, ..., Z corresponds to 1, 2, ..., 25 respectively
            ' ' (space) corresponds to 26
    */
    // If the number is 26, return a space
    if(number == 26){
        return ' ';
    }else{
        // return the number plus 65 to yield the corresponding ascii value
        char c = (char)(number + 65);
        return c;
    }
}
