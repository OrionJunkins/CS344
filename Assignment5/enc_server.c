#include "enc_server.h"
// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}

int main(int argc, char *argv[]){
  int connectionSocket;
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);

  // Check usage & args
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    exit(1);
  } 
  
  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    error("ERROR opening socket");
  }

  // Set up the address struct for the server socket
  setupAddressStruct(&serverAddress, atoi(argv[1]));

  // Associate the socket to the port
  if (bind(listenSocket, 
          (struct sockaddr *)&serverAddress, 
          sizeof(serverAddress)) < 0){
    error("ERROR on binding");
  }

  // Start listening for connetions. Allow up to 5 connections to queue up
  listen(listenSocket, 5); 
  
  // Accept a connection, blocking if one is not available until one connects
    while(1){
        // Accept the connection request which creates a connection socket
        connectionSocket = accept(listenSocket, 
                (struct sockaddr *)&clientAddress, 
                &sizeOfClientInfo); 
        if (connectionSocket < 0){
            error("ERROR on accept");
        }

        // Fork a new process
        pid_t spawnPid = fork();
        switch(spawnPid){
            case -1:
                perror("fork() failure\n");
                exit(1);
                break;
            case 0:
                // In the child
                close(listenSocket);
                processConnection(clientAddress, connectionSocket);
                exit(0);
            default:
                // In the parent
                close(connectionSocket);
        }

    //int wait_result = waitpid(node_ptr->PID, &wstatus, WNOHANG); Close BG processes?
    }
    // Close the listening socket
    close(listenSocket); 
    return 0;
}

void processConnection(struct sockaddr_in clientAddress, int connectionSocket) {
    printf("SERVER: Connected to client running at host %d port %d\n", 
                            ntohs(clientAddress.sin_addr.s_addr),
                            ntohs(clientAddress.sin_port));
    // Recv verification
    char encVerification[2];
    recieveAll(connectionSocket, encVerification, 1);    
    if(encVerification[0] != 'E'){
        // respond w/ failure
        char* verificationResponse = "F";
        sendAll(connectionSocket, verificationResponse, strlen(verificationResponse)); 
        error("Invalid Client"); 
        exit(2); //TODO check exit code
    }

    // respond w/verification
    char* verificationResponse = "V";
    sendAll(connectionSocket, verificationResponse, strlen(verificationResponse)); 
    
    // recieve size
    char keySizeString[MAX_DIGITS+1]; // Enough for text and '\0'
    recieveAll(connectionSocket,keySizeString, MAX_DIGITS);
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
    
    // Send encrypted text
    sendAll(connectionSocket, ciphertext, keySize);

    // Close the connection socket for this client
    close(connectionSocket); 
}

void encryptText(char* plaintext, char* key, char* ciphertextDest){
    int size = strlen(plaintext);
    int plaintextNumeric[strlen(plaintext)];
    int keyNumeric[strlen(key)];
    convertFromAscii(plaintext, plaintextNumeric);
    convertFromAscii(key, keyNumeric);
    
    char ciphertext[strlen(plaintext)];
    int cipherCharNumeric;
    char cipherChar;

    for (int i = 0; i < size; i++){
        cipherCharNumeric = (keyNumeric[i] + plaintextNumeric[i]) % 27;
        cipherChar = convertToAscii(cipherCharNumeric);
        ciphertext[i] = cipherChar;
    }
    ciphertext[size] = '\0';
    
    strcpy(ciphertextDest, ciphertext);
    
}

void convertFromAscii(char* input, int* output){
    for (int i = 0; i < strlen(input); i++){
        if(input[i] == ' '){
            output[i] = 26;
        } else {
            int currentCharacter = (int)input[i];
            output[i] = currentCharacter - 65;
        }
    }
}

char convertToAscii(int number){
    if(number == 26){
        return ' ';
    }
    else{
        char c = (char)(number + 65);
        return c;
    }
}