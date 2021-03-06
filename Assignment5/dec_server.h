/*
    Trivial header file for dec_server.c All documentation appears in .c file.
*/
#include "socket_helpers.h"
#include "text_helpers.h"
void processConnection(struct sockaddr_in clientAddress, int connectionSocket);
void decryptText(char* ciphertext, char* key, char* plaintextDest);
