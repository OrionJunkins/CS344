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
        exit(1); // TODO Check error conditions
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
    setupAddressStruct(&serverAddress, port, HOSTNAME);

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
        error("Invalid Client"); // Send failure notice to terminate
        exit(2); //TODO check exit code
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
    printf("%s", ciphertext);

    // Close the socket
    close(connectionSocket); 
    return 0;
}


// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                            int portNumber, 
                            char* hostname){
    
    // Clear out the address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);

    // Get the DNS entry for this host name
    struct hostent* hostInfo = gethostbyname(hostname); 
    if (hostInfo == NULL) { 
        fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
        exit(0); 
    }
    // Copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char*) &address->sin_addr.s_addr, 
            hostInfo->h_addr_list[0],
            hostInfo->h_length);
}


char* getFileText(char* filename){
    /*
        Given a filename, open the file, and process it one char at a time.
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

    int cur;
    char curChar;
    int index = 0;
    while((cur = fgetc(file)) != EOF){
        curChar = (char)cur;
        if(isValidChar(curChar)){
            dest[index] = curChar;
        } else{
            fprintf(stderr, "Error: input contains bad characters");
            exit(1); 
        }
        index++;
    }
    return dest;
}


bool isValidChar(char c){
    int asciiVal = (int)c;
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


