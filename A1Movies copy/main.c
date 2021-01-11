#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie_list.h"
#include "file_parser.h"
#include "user_queries.h"

int main (int argc, char* argv[])
{
  if(argc < 0)//2
  {
    printf("Specify a CSV file");
    return 1;
  }
  //char* filepath = argv[1];
  char* filepath = "test_movies.csv";

  struct MovieList* movies= (struct MovieList*)malloc(sizeof(struct MovieList)); 

  parse_file(movies, filepath); 

  query_user(movies);

  return 0;
}

