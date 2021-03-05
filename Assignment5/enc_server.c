#include "enc_server.h"

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
    setupAddressStruct(&serverAddress, atoi(argv[1]));

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


void setupAddressStruct(struct sockaddr_in* address, int portNumber){
    /*
        Set up the address struct for the server socket
    */
    // Clear out the address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);
    // Allow a client at any address to connect to this server
    address->sin_addr.s_addr = INADDR_ANY;
}


void processConnection(struct sockaddr_in clientAddress, int connectionSocket) {
    // Recieve verification of identity from the client
    char encVerification[2]; // Enough for verification char and '\0'
    recieveAll(connectionSocket, encVerification, 1);   

    // Check validity of verification 
    if(encVerification[0] != 'E'){
        // If invalid, respond with a failure flag of "F"
        char* verificationResponse = "F";
        sendAll(connectionSocket, verificationResponse, strlen(verificationResponse)); 

        // Raise an error and terminate the process
        fprintf(stderr, "Error: Invalid Client\n"); 
        exit(0); //TODO check exit code
    }

    // Else, the client is valid so respond with a verification char of 'V'
    char* verificationResponse = "V";
    sendAll(connectionSocket, verificationResponse, strlen(verificationResponse)); 
    
    // Recieve a string containing the file size to expect for recieving key/platintext
    char keySizeString[MAX_DIGITS+1]; // Enough for text and '\0'
    recieveAll(connectionSocket,keySizeString, MAX_DIGITS);

    // Convert string to int
    int keySize = atoi(keySizeString);

    // Recieve plaintext
    char plaintext[keySize+1]; // Enough for text and '\0'
    recieveAll(connectionSocket, plaintext, keySize);

    // Recieve key
    char key[keySize+1]; // Enough for text and '\0'
    recieveAll(connectionSocket, key, keySize);

    // Generate cipertext
    char ciphertext[keySize+1]; // Enough for text and '\0'
    encryptText(plaintext, key, ciphertext);
    
    // Send ciphertext back to client
    sendAll(connectionSocket, ciphertext, keySize);

    // Close the connection socket
    close(connectionSocket); 
}

void encryptText(char* plaintext, char* key, char* ciphertextDest){
    /*
        Perform a basic encryption by adding plaintext to key modulo 27
        Copy the ciphertext to the given ciphertextDest
    */
    // Store the size of the plaintext
    int size = strlen(plaintext);

    // Convert the given plaintext and key from ascii into integers in the range [0...26]
    int plaintextNumeric[size];
    int keyNumeric[size];
    convertFromAscii(plaintext, plaintextNumeric);
    convertFromAscii(key, keyNumeric);
    
    // Create a modifiable ciphertext array
    char ciphertext[size];

    // Temps to hold each char during processing
    int cipherCharNumeric;
    char cipherChar;

    // For every index up to the size,
    for (int i = 0; i < size; i++){
        // Get the numeric value of the ciphertext char to be copied
        cipherCharNumeric = (keyNumeric[i] + plaintextNumeric[i]) % 27;

        // Convert it to ascii
        cipherChar = convertToAscii(cipherCharNumeric);

        // Copy it to the array
        ciphertext[i] = cipherChar;
    }
    // Append the array with terminator
    ciphertext[size] = '\0';
    
    // Copy to the given destination
    strcpy(ciphertextDest, ciphertext);  
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
