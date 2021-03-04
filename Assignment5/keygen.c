#include "keygen.h"
int main(int argc, char* argv[]){

    if(argc < 2){
        fprintf(stderr, "Invalid Input to Keygen");
    }

    int keyLen = atoi(argv[1]);
    char key[keyLen];
    srand(time(0));

    for(int i = 0; i < keyLen; i++){
        char c = generateRandChar();
        key[i] = c;
    }
    
    write(STDOUT_FILENO, key, strlen(key));  
    return 0;
}

char generateRandChar(){
    int randNumber = rand();
    int charIndex = randNumber % 27;
    char c = mapToAscii(charIndex);
    return c;
}

char mapToAscii(int number){
    if(number == 26){
        return ' ';
    }
    else{
        char c = (char)(number + 65);
        return c;
    }
}