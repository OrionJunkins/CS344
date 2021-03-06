/*
    Trivial header file for enc_server.c All documentation appears in .c file.
*/
#include "socket_helpers.h"
#include "text_helpers.h"
void processConnection(struct sockaddr_in clientAddress, int connectionSocket);
void encryptText(char* plaintext, char* key, char* ciphertextDest); 
