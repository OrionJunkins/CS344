/*
    Encryption client. Given ciphertext, a key, and a port: 
        1) Connect to an encryption server at the given port if possible
        2) Send plaintext and key
        3) Recieve ciphertext back
        4) Output ciphertext
*/
#include "enc_client.h"

int main(int argc, char *argv[]) {
    // Store values from argv
    char* plaintextFile = argv[1];
    char* keyFile = argv[2];
    int port = atoi(argv[3]);
    
    // Check usage & args
    if (argc < 3) { 
        fprintf(stderr,"USAGE: %s plaintext key port \n", argv[0]);
        exit(0); 
    } 

    // Socket setup
    int connectionSocket, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;

    // Get plaintext and key text from argv filenames to char*
    char* plaintext = getFileText(plaintextFile);
    char* key = getFileText(keyFile);

    // Get sizes of each
    int plaintextSize = strlen(plaintext);
    int keySize = strlen(key);

    // Verify that lengths match
    if(keySize < plaintextSize) {
        fprintf(stderr, "Error: key '%s' is too short\n", keyFile);
        exit(1); 
    } else if (keySize > plaintextSize) {
        key[plaintextSize] = '\0';
        keySize = strlen(key);
    }

    // Create a socket
    connectionSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (connectionSocket < 0){
        fprintf(stderr, "Error: could not create a network socket\n");
        exit(2);
    }

    // Set up the server address struct
    setupClientAddressStruct(&serverAddress, port);

    // Connect to server
    if (connect(connectionSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        fprintf(stderr, "Error: could not contact enc_server on port %d\n", port);
        exit(2);
    }

    // Send client identity verification 
    char* encVerification = "E";
    sendAll(connectionSocket, encVerification, strlen(encVerification)); 

    // Recieve verification response from server
    char verificationResponse[1];
    recieveAll(connectionSocket, verificationResponse, 1); 
    if(verificationResponse[0] != 'V'){
        fprintf(stderr, "Error: could not contact enc_server on port %d\n", port);
        exit(2);
    }

    // Send key/plaintext size as unsigned long
    char sizeString[MAX_DIGITS]; // Max string length needed to hold largest possible length
    memset(sizeString, '\0', MAX_DIGITS);
    sprintf(sizeString, "%d", keySize);
    sendAll(connectionSocket, sizeString, MAX_DIGITS);
    
    // Send plaintext
    sendAll(connectionSocket, plaintext, keySize);
    
    // Send Key
    sendAll(connectionSocket, key, keySize);

    // Recieve ciphertext
    char ciphertext[keySize + 1]; // Enough for text and '\0'
    recieveAll(connectionSocket, ciphertext, keySize);

    // Output the ciphertext
    printf("%s\n", ciphertext);

    // Close the socket
    close(connectionSocket); 
    return 0;
}
