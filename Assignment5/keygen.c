/*
    Generates a randomized key of the given length.
    Key is composed of capital letters and spaces
*/
#include "keygen.h"

int main(int argc, char* argv[]){
    // Check usage
    if(argc < 2){
        fprintf(stderr,"USAGE: %s keySize\n", argv[0]);
        exit(0); 
    }

    // Get the key length as an int
    int keyLen = atoi(argv[1]);

    // Create an array to hold the key
    char key[keyLen+1]; // Enough for text and '\0'

    // Seed rand generator
    srand(time(0));

    // Up to key length,
    for(int i = 0; i < keyLen; i++){
        // Generate a random valid character
        char c = generateRandChar();

        // Assign it to the key
        key[i] = c;
    }

    // Add a null terminator
    key[keyLen] = '\0';
    
    // Send the key to stdout with a newline attached
    printf("%s\n", key);
    return 0;
}

char generateRandChar(){
    /*
        Generate a random valid char( A-Z or a space )
    */
    // Get random number
    int randNumber = rand();

    // Put it in the range [0, 26]
    int charIndex = randNumber % 27;

    // Convert it to the appropriate char
    char c = mapToAscii(charIndex);
    return c;
}

char mapToAscii(int number){
    /*
        Convert a number in the range [0,26] to a character
    */
    // If the number is 26, return a space
    if(number == 26){
        return ' ';
    }else{
        // Otherwise add 65 and cast to a char to yield the appropriate capital letter
        char c = (char)(number + 65);
        return c;
    }
}
