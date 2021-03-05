#include "socket_helpers.h"

char* HOSTNAME = "localhost";

bool isValidChar(char c);
char* getFileText(char* filename);
void setupAddressStruct(struct sockaddr_in* address, 
                            int portNumber, 
                            char* hostname);
