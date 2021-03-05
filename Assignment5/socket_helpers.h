#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <sys/stat.h> //stat()
#include <netdb.h>      // gethostbyname()
#include <stdbool.h>
#define MAX_DIGITS 11
// Error function used for reporting issues TODO, REMOVE uses
void error(const char *msg) { 
    perror(msg); 
    exit(0); 
} 


void sendAll(int connectionSocket, char * buf, size_t length) {
    ssize_t charsWritten;
    char* bufPtr = buf;
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

void recieveAll(int connectionSocket, char* buf, size_t length){
    size_t uncopiedLength = length;
    int charsRead;
    char* bufPtr = buf;
    while (uncopiedLength > 0){
        charsRead = recv(connectionSocket, bufPtr, uncopiedLength, 0); 
        if (charsRead < 0){
            error("ERROR reading from socket");
            exit(2); //TODO
        }
        bufPtr += charsRead;
        uncopiedLength -= charsRead;
        
    }
    buf[length] = '\0';
    
}