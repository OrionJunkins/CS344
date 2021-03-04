#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_DIGITS 11

void processConnection(struct sockaddr_in clientAddress, int connectionSocket);

void encryptText(char* plainText, char* key, char* encryptedText);
void convertFromAscii(char* input, int* output);

void sendAll(int connectionSocket, const void * buf, size_t length);
void recieveAll(int connectionSocket, char * buf, size_t length);
