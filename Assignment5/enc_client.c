#include "enc_client.h"
/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

// Error function used for reporting issues
void error(const char *msg) { 
    perror(msg); 
    exit(0); 
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



int main(int argc, char *argv[]) {
    // Check usage & args
    if (argc < 3) { 
        fprintf(stderr,"USAGE: %s plaintext key port \n", argv[0]);
        exit(0); 
    } 

    // Socket setup
    int connectionSocket, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;

    // Get plaintext and Key files    
    char* plaintext = getFileText(argv[1]);
    char* key = getFileText(argv[2]);

    // Get sizes of each
    int fileSize = strlen(plaintext);
    int keySize = strlen(key);

    // Verify that lengths match
    if(fileSize != keySize) {
        perror("File and Key must be the same size");
        exit(1); // TODO Check error conditions
    }

    // Create a socket
    connectionSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (connectionSocket < 0){
        error("CLIENT: ERROR opening socket");
    }

    // Set up the server address struct
    setupAddressStruct(&serverAddress, atoi(argv[3]), HOSTNAME);

    // Connect to server
    if (connect(connectionSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        error("CLIENT: ERROR connecting");
    }

    // Send client verification 
    char* EncVerification = "E";
    sendAll(connectionSocket, EncVerification, strlen(EncVerification)); 

    // Recieve verification response from server
    charsRead = recv(connectionSocket, EncVerification, 1, 0); 
    if(EncVerification[0] != 'E'){
        error("Invalid Client"); // Send failure notice to terminate
        exit(2); //TODO check exit code
    }

    // Send size as unsigned long
    char sizeString[MAX_DIGITS]; // Long enough for max unsigned int size
    memset(sizeString, '\0', MAX_DIGITS);
    sprintf(sizeString, "%d", keySize);
    sendAll(connectionSocket, sizeString, MAX_DIGITS);
    
    // Send plaintext
    sendAll(connectionSocket, plaintext, strlen(plaintext));
    printf("ptxt sent\n");
    // Send Key
    sendAll(connectionSocket, key, strlen(plaintext));
    printf("key sent\n");

    // Recieve ciphertext

    // Close the socket
    close(connectionSocket); 
    return 0;
}



char* getFileText(char* filename){
    long fileLength;
    char* dest;

    FILE* file = fopen(filename, "r");

    if (file == NULL){
        perror("Error opening specified file\n");
        exit(1); //TODO check exit command
    }
    fseek(file, 0L , SEEK_END);
    fileLength = ftell(file);
    rewind(file);
    dest = calloc(1, fileLength + 1);
    if(dest == NULL){
        perror("Error Allocating memory\n");
        exit(1); //TODO check exit command
    }
    int index = 0;
    char cur = fgetc(file);
    for(int i = 0; i < fileLength; i++){ 
        if(isValidChar(cur)){
            dest[index] = cur;
        } else{
            perror("Bad character\n");
            exit(1); //TODO check exit command
        }
        index++;
        cur = fgetc(file);
    }
    return dest;    
}

bool isValidChar(char c){
    int asciiVal = (int)c;
    if (c == ' '){
        return true;
    } else if (65 <= asciiVal && asciiVal <= 90){
        return true;
    } else {
        return false;
    }
}


void sendAll(int connectionSocket, const void * buf, size_t length) {
    ssize_t charsWritten;
    const char* bufPtr = buf;
    while(length > 0){
        charsWritten = send(connectionSocket, bufPtr, length, 0); 
        if (charsWritten < 0){
            error("CLIENT: ERROR writing to socket");
            exit(2); //TODO
        }
        bufPtr += charsWritten;
        length -= charsWritten;
    }
}