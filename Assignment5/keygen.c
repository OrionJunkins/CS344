#include "keygen.h"
int main(int argc, char* argv[]){

    if(argc < 2){
        fprintf(stderr, "Invalid Input to Keygen");
    }

    int key_len = atoi(argv[1]);
    char key[key_len + 1];
    srand(time(0));

    for(int i = 0; i < key_len; i++){
        char c = generate_rand_char();
        key[i] = c;
    }
    
    strcat(key, "\n");
    printf("%s", key);    
    return 0;
}

char generate_rand_char(){
    int rand_number = rand();
    int char_index = rand_number % 27;
    char c = map_to_ascii(char_index);
    return c;
}

char map_to_ascii(int number){
    if(number == 26){
        return ' ';
    }
    else{
        char c = (char)(number + 65);
        return c;
    }
}