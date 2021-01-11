#ifndef movie_list
#define movie_list
/*
Orion Junkins
Assignment 1: Movies
Stores a list of movies and gives tools for 
*/
#include <stdlib.h>

struct Movie{
  /*
  Basic struct to hold the details of a single movie.
  */
  char* title;
  int year;
  float rating;
  char languages[5][20];
};


struct MovieNode{
  struct Movie* movie;
  struct MovieNode* next;
};

struct MovieList{
  struct MovieNode* first;
};

void add_movie(struct MovieList* list, struct Movie* new_movie){
  struct MovieNode* new_node = (struct MovieNode*)(malloc(sizeof(struct MovieNode)));
  new_node->movie = new_movie;
  new_node->next = list->first;
  list->first = new_node;
}


//needed?
void add_node(struct MovieList* list, struct MovieNode* new_node){
  new_node->next = list->first;
  list->first = new_node;
}

#endif