#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include <stdbool.h>
#define MAX_DIGITS 11
char* HOSTNAME = "localhost";

bool isValidChar(char c);
//void getFileText(char* dest, char* filename);
char* getFileText(char* filename);

void sendAll(int socket, const void * buf, size_t length);