/*
Orion Junkins
Assignment 1: Movies
Program main: handle command line input, parse the file and query the user.
Depends on movie_list.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie_list.h"

int main (int argc, char* argv[])
{
  //Check validity of command line argument
  if(argc != 2)
  {
    printf("Error: Specify a single CSV file\n");
    return 1;
  }
  char* filepath = argv[1];

  //Create an empty MovieList movies
  struct MovieList* movies = (struct MovieList*)malloc(sizeof(struct MovieList)); 
  if(movies == NULL){
    printf("ERROR: Could not allocate memory\n");
    return 1;
  }
  movies->first = NULL;

  //Parse the movies in the file at filepath into a list stored in movies
  parse_file(movies, filepath); 

  //With the list produced, allow the user to interact with and query the list
  query_user(movies);

  //Free movies and everything it contains
  free_MovieList(movies);
  free(movies); 

  return 0;
}

