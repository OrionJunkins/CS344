#include "socket_helpers.h"

void processConnection(struct sockaddr_in clientAddress, int connectionSocket);

void encryptText(char* plaintext, char* key, char* ciphertextDest); 
void convertFromAscii(char* input, int* output);

void sendAll(int connectionSocket, char* buf, size_t length);
void recieveAll(int connectionSocket, char * buf, size_t length);

char convertToAscii(int number);