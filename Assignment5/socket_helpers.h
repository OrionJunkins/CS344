/*
    Header file with all needed library includes for socket communication.
    Also defines functions for complete package sending/recieveing and basic file parsing.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

// System wide constants
#define MAX_DIGITS 11 // Max length of a string representation of an int
char* HOSTNAME = "localhost"; // Standard system wide host

void setupClientAddressStruct(struct sockaddr_in* address, int portNumber){
    /*
        Set up the address struct
    */
    // Clear out the given address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);

    // Get the DNS entry for this host name
    struct hostent* hostInfo = gethostbyname(HOSTNAME); 
    if (hostInfo == NULL) { 
        fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
        exit(0); 
    }
    // Copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char*) &address->sin_addr.s_addr, 
            hostInfo->h_addr_list[0],
            hostInfo->h_length);
}


void setupServerAddressStruct(struct sockaddr_in* address, int portNumber){
    /*
        Set up the address struct for a server socket
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


void sendAll(int connectionSocket, char * buf, size_t length) {
    /*
        Send length chars from buf to the given connection socket. 
        Assumes buf has at least length chars.
    */
    // Track the chars written
    ssize_t charsWritten;

    // Maintain a pointer within buf
    char* bufPtr = buf;

    // So long as there is length remianing
    while(length > 0){
        // Send as much as possible from bufPtr onward
        charsWritten = send(connectionSocket, bufPtr, length, 0); 
        if (charsWritten < 0){
            fprintf(stderr, "Error: could write to socket\n");
            exit(2);
        }

        // Increment the bufPtr beyond what has been sent
        bufPtr += charsWritten;

        // Decrement the length based on what has been sent
        length -= charsWritten;
    }
}


void recieveAll(int connectionSocket, char* buf, size_t length){
    // Track the chars left to copy
    size_t uncopiedLength = length;

    // # of chars read each call to recb
    int charsRead;

    // Internal pointer within buf
    char* bufPtr = buf;

    // So long as some of the length has not been copied
    while (uncopiedLength > 0){
        // Copy as much as possible to bufPtr
        charsRead = recv(connectionSocket, bufPtr, uncopiedLength, 0); 
        if (charsRead < 0){
            fprintf(stderr, "Error: could read from socket\n");
            exit(2);
        }
        // Increment bufPtr
        bufPtr += charsRead;

        //Decrement uncopied length
        uncopiedLength -= charsRead;
    }
    // Add null terminator
    buf[length] = '\0';
}