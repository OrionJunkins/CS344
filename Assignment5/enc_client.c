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
    int connectionSocket, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    char* plainText; 
    char* key;
    // Check usage & args
    if (argc < 3) { 
        fprintf(stderr,"USAGE: %s plaintext key port \n", argv[0]);
        exit(0); 
    } 
    getFileText(plainText, argv[1]);
    getFileText(key, argv[1]);


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

    // Send client verification - 1 byte
    char EncVerification[1] = "E";

    charsWritten = send(connectionSocket, EncVerification, strlen(EncVerification), 0); 
    if (charsWritten < 0){
        error("CLIENT: ERROR writing to socket");
    }



    // Recieve verification response from server
    charsRead = recv(connectionSocket, EncVerification, 1, 0); 
    if (charsRead < 0){
        error("ERROR reading from socket");
    }
    if(EncVerification[0] != 'E'){
        error("Invalid Client"); // Send failure notice to terminate
        exit(2); //TODO check exit code
    }


    // Send size as unsigned short



    // recive confirmation - 1 byte
    
    // Send plaintext
    // Recieve plaintextRecieved confirm

    // Send Key
    

    // Recieve ciphertext

    /*
    // Send message to server
    // Write to the server
    charsWritten = send(connectionSocket, buffer, strlen(buffer), 0); 
    if (charsWritten < 0){
        error("CLIENT: ERROR writing to socket");
    }
    if (charsWritten < strlen(buffer)){
        printf("CLIENT: WARNING: Not all data written to socket!\n");
    }

    // Get return message from server
    // Clear out the buffer again for reuse
    memset(buffer, '\0', sizeof(buffer));
    // Read data from the socket, leaving \0 at end
    charsRead = recv(connectionSocket, buffer, sizeof(buffer) - 1, 0); 
    if (charsRead < 0){
        error("CLIENT: ERROR reading from socket");
    }
    printf("CLIENT: I received this from the server: \"%s\"\n", buffer);

    char key[256] = "EFGGG";

    charsWritten = send(connectionSocket, key, strlen(key), 0); 
    if (charsWritten < 0){
        error("CLIENT: ERROR writing to socket");
    }
    */
    // Close the socket
    close(connectionSocket); 
    return 0;
}



void getFileText(char* dest, char* filename){
    long fileLength;

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
    char cur;
    while((cur = fgetc(file)) != EOF){
        if(isValidChar(cur)){
            dest[index] = cur;
        } else{
            perror("Bad character\n");
            exit(1); //TODO check exit command
        }
        index ++;
    }
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