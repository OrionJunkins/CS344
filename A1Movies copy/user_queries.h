#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie_list.h"

void print_initial_prompt();
void respond_to_user(struct MovieList* movies, int selection);
void print_movie_titles(struct MovieList* movies); 
void print_best(struct MovieList * movies);
void print_highest_rated(struct MovieList* movies);// -> header?

struct MovieList* year_movies(struct MovieList* all_movies, int year); //needed?

void query_user(struct MovieList* movies){
  print_initial_prompt();
  
  char* end;
  char line_buffer[5];
  int user_selection = -1;

  while(user_selection != 4){
    //fgets(line_buffer, 5, stdin);
    //line_buffer[strlen(line_buffer) - 1] = 0;
    //user_selection = strtol(line_buffer, &end, 10);
    user_selection = 2;
    respond_to_user(movies, user_selection);
    user_selection = 4;
  }
}


void print_initial_prompt(){
  printf("1. Show movies released in the specified year\n2. Show highest rated movie for each year\n3. Show the title and year of release of all movies in a specific language\n4. Exit from the program\nEnter a choice from 1 to 4: ");
}
int year = 2012;
void respond_to_user(struct MovieList* movies, int selection){
  switch(selection){
    case 1: 
      print_movie_titles(year_movies(movies, year));
      break;
    case 2:
      print_best(movies);
      break;
    case 3:
      break;
    case 4:
      break;
    default:
    break;

  }
}
void print_movie_titles(struct MovieList* movies){
  struct MovieNode * node = movies->first;
  while(node != NULL){
    printf("%s\n", node->movie->title);
    node = node->next;
  }
  //return 0; //success
}

struct MovieList* year_movies(struct MovieList* all_movies, int year){
  struct MovieList* filtered_movies = malloc(sizeof(struct MovieList)); 
  filtered_movies->first = NULL;

  struct MovieNode * node = all_movies->first;
  while(node != NULL){
    if(node->movie->year == year){
      add_movie(filtered_movies, node->movie);
    }
    node = node->next;
  }
  return filtered_movies;
}

void print_best(struct MovieList * movies){
  for(int i = 1990; i <= 2021; i++){
    print_highest_rated(year_movies(movies, i));
  }
}

void print_highest_rated(struct MovieList* movies){
  struct Movie* highest_rated = (struct Movie*)(calloc(1,sizeof(struct Movie*)));
  highest_rated->rating = -1;

  struct MovieNode * node = movies->first;
  while(node != NULL){
    if(node->movie->rating > highest_rated->rating){
      highest_rated = node->movie;
    }
    node = node->next;
  }
  if(highest_rated->title != NULL){   //TODO: Verify that this appropriately handles Null input
    printf("%d %.1f %s\n", highest_rated->year, highest_rated->rating, highest_rated->title);
  }  
}

