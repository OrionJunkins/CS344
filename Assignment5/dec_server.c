/*
    Decryption Server. Given a port number 
        1) Connect to any valid decryption clients which connect
        2) Recieve filesize, cipertext and key
        3) Decrypt into plaintext
        4) Send plaintext back to client
*/
#include "dec_server.h"

int main(int argc, char *argv[]){
    // Setup for connection socket
    int connectionSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t sizeOfClientInfo = sizeof(clientAddress);

    // Check usage & args
    if (argc < 2) { 
        fprintf(stderr, "USAGE: %s port\n", argv[0]); 
        exit(1);
    } 
    
    // Create the socket that will listen for connections
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        fprintf(stderr, "Error: could not create a network socket\n");
        exit(2);
    }

    // Set up the address struct for the server socket
    setupServerAddressStruct(&serverAddress, atoi(argv[1]));

    // Associate the socket to the port
    if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        fprintf(stderr, "Error: could not bind network socket\n");
        exit(2);
    }

    // Start listening for connetions. Allow up to 5 connections to queue up
    listen(listenSocket, 5); 
    
    // Accept a connection, blocking if one is not available until one connects
    while(1){
        // Accept the connection request which creates a connection socket
        connectionSocket = accept(listenSocket,(struct sockaddr *)&clientAddress, &sizeOfClientInfo); 
        if (connectionSocket < 0){
            fprintf(stderr, "Error: could not establish a connection\n");
            exit(2);
        }

        // Fork a new process to handle the connection
        pid_t spawnPid = fork();
        switch(spawnPid){
            case -1:
                fprintf(stderr, "Error: fork() failure\n");
                exit(2);
                break;
            case 0:
                // In the child, close the listen socket
                close(listenSocket);

                // Process the connection
                processConnection(clientAddress, connectionSocket);

                // Exit/terminate child process once complete
                exit(0);
                break;

            default:
                // In the parent, close the connection socket and proceed
                close(connectionSocket);
                break;
        }
    }
    // Close the listening socket
    close(listenSocket); 
    return 0;
}


void processConnection(struct sockaddr_in clientAddress, int connectionSocket) {
    // Recieve verification of identity from the client
    char encVerification[2]; // Enough for verification char and '\0'
    recieveAll(connectionSocket, encVerification, 1);   

    // Check validity of verification 
    if(encVerification[0] != 'D'){
        // If invalid, respond with a failure flag of "F"
        char* verificationResponse = "F";
        sendAll(connectionSocket, verificationResponse, strlen(verificationResponse)); 

        // Connection is invalid, terminate the process
        exit(2); 
    }

    // Else, the client is valid so respond with a verification char of 'V'
    char* verificationResponse = "V";
    sendAll(connectionSocket, verificationResponse, strlen(verificationResponse)); 
    
    // Recieve a string containing the file size to expect for recieving key/platintext
    char keySizeString[MAX_DIGITS+1]; // Enough for text and '\0'
    recieveAll(connectionSocket,keySizeString, MAX_DIGITS);

    // Convert string to int
    int keySize = atoi(keySizeString);

    // Recieve ciphertext
    char ciphertext[keySize+1]; // Enough for text and '\0'
    recieveAll(connectionSocket, ciphertext, keySize);

    // Recieve key
    char key[keySize+1]; // Enough for text and '\0'
    recieveAll(connectionSocket, key, keySize);

    // Generate cipertext
    char plaintext[keySize+1]; // Enough for text and '\0'
    decryptText(ciphertext, key, plaintext);
    
    // Send plaintext back to client
    sendAll(connectionSocket, plaintext, keySize);

    // Close the connection socket
    close(connectionSocket); 
}


void decryptText(char* ciphertext, char* key, char* plaintextDest){
    /*
        Perform a basic encryption by adding ciphertext to key modulo 27
        Copy the plaintext to the given plaintextDest
    */
    // Store the size of the ciphertext
    int size = strlen(ciphertext);

    // Convert the given ciphertext and key from ascii into integers in the range [0...26]
    int ciphertextNumeric[size];
    int keyNumeric[size];
    convertFromAscii(ciphertext, ciphertextNumeric);
    convertFromAscii(key, keyNumeric);
    
    // Create a modifiable plaintext array
    char plaintext[size];

    // Temps to hold each char during processing
    int plaintextCharNumeric;
    char plaintextChar;

    // For every index up to the size,
    for (int i = 0; i < size; i++){
        // Get the numeric value of the plaintext char to be copied
        plaintextCharNumeric = (ciphertextNumeric[i] - keyNumeric[i]) % 27;
        if(plaintextCharNumeric < 0){
            plaintextCharNumeric += 27;
        }
        // Convert it to ascii
        plaintextChar = convertToAscii(plaintextCharNumeric);
        // Copy it to the array
        plaintext[i] = plaintextChar;
    }
    // Append the array with terminator
    plaintext[size] = '\0';
    
    // Copy to the given destination
    strcpy(plaintextDest, plaintext);  
}
