#include "enc_server.h"
// Error function used for reporting issues
void error(const char *msg) {
  perror(msg);
  exit(1);
} 

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

    //Fork
    //In parent:
        // Close connectionSocket
    //In child :
        // Close listen socket
        // Recieve validation & size
        // If invalid:
            // Exit with Error
            // Terminate Process
            //otherwise,
        // Send confirmation to client
        // Recieve data
        // encrypt
        // Send encrypted
        // Close
    //int wait_result = waitpid(node_ptr->PID, &wstatus, WNOHANG); Close BG processes
    }
    // Close the listening socket
    close(listenSocket); 
    return 0;
}

void processConnection(struct sockaddr_in clientAddress, int connectionSocket) {
    printf("SERVER: Connected to client running at host %d port %d\n", 
                            ntohs(clientAddress.sin_addr.s_addr),
                            ntohs(clientAddress.sin_port));
    int charsRead;
    // Recv verification
    char EncVerification[1];
    charsRead = recv(connectionSocket, EncVerification, 1, 0); 
    if (charsRead < 0){
    error("ERROR reading from socket");
    }

    if(EncVerification[0] != 'E'){
        error("Invalid Client"); // Send failure notice to terminate
        exit(2); //TODO check exit code
    }

    // respond w/verification
    charsRead = send(connectionSocket, EncVerification, strlen(EncVerification), 0); 
    if (charsRead < 0){
        error("ERROR writing to socket");
    }
    
    
    
    // recieve size as unsigned short


    // Confirm that size has been recieved

    
    
    // Recieve plaintext
    
    // Send encrypted text

    
    
    
    
    
    
    
    /*
    int textLength = 256;
    char plainText[textLength]; //Change size based on given in verification handshake
    char key[textLength]; 
    int charsRead;
    // Get the message from the client and display it
    memset(plainText, '\0', textLength);
    
    // Read the client's message from the socket
    charsRead = recv(connectionSocket, plainText, textLength-1, 0); 
    if (charsRead < 0){
    error("ERROR reading from socket");
    }
    printf("SERVER: I received this PLAINTEXT from the client: \"%s\"\n", plainText);
    charsRead = recv(connectionSocket, key, textLength-1, 0); 
    if (charsRead < 0){
    error("ERROR reading from socket");
    }
    printf("SERVER: I received this KEY from the client: \"%s\"\n", key);
    

    // Send a Success message back to the client
    charsRead = send(connectionSocket, 
                    "I am the server, and I got your message", 39, 0); 
    if (charsRead < 0){
    error("ERROR writing to socket");
    }

    char encryptedText[textLength];
    encryptText(plainText, key, encryptedText);
    */
    // Close the connection socket for this client
    close(connectionSocket); 
}






void encryptText(char* plainText, char* key, char* encryptedText){
    int plainTextNumeric[strlen(plainText)];
    convertFromAscii(plainText, plainTextNumeric);
    for (int i = 0; i < strlen(plainText); i++){
        printf("PT: %c\n", plainText[i]);
        printf("NUM: %c\n\n", plainTextNumeric[i]);
    }
}

void convertFromAscii(char* input, int* output){
    for (int i = 0; i < strlen(input); i++){
        if(input[i] == ' '){
            output[i] = 26;
        } else {
            int currentCharacter = (int)input[i];
            output[i] = currentCharacter - 65;
        }
        printf("NUM: %c\n\n", output[i]);
    }
}