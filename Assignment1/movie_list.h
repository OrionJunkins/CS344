/*
Orion Junkins
Assignment 1: Movies
Stores a list of movies and gives tools for 
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MIN_YEAR 1900
#define MAX_YEAR 2021
#define MAX_INPUT_LENGTH 100 

typedef struct Movie {
/*
Basic to hold the details of a single movie.
*/
char* title;
int year;
float rating;
char languages[5][21];
} Movie;

/*
Combines a Movie* with a pointer to another MovieNode to allow movies
to be stored as a linked list
*/
typedef struct MovieNode {
  Movie* movie;
  struct MovieNode* next;
} MovieNode;

typedef struct MovieList {
/*
  Stores a pointer to the first MovieNode of the linked list and the
  list size
*/
  MovieNode* first;
  int size;
} MovieList;


void add_movie(MovieList* list, Movie* new_movie);

void free_MovieList(MovieList* list);

/*
Fetch the file at the given path, open it and parse it line by line.
For every line of data, create a Movie and add it to the specified 
MovieList
*/
void parse_file(MovieList* movies, char* filepath);

void output_sucess(char* filepath, int movies_parsed);
/*
  Given a formatted line of input from a csv, parse the title,
  year, rating and languages from the respective columns.
  Return a Movie* with all of the corresponding details.
  */
void parse_line(Movie* movie, char* input);
/*
  Given input in the form of '[lang1;lang2;lang3]' 
  store each language in the languages array of movie
  */
void assign_languages(Movie* movie, char* input);


/*
Prompt the user and respond to their queries
*/
void query_user(MovieList* movies);





void print_initial_prompt();
/*
Standard query options list for user
*/

/*
Given a list of movies and a user's selected query, print the correct information
*/
void respond_to_user(MovieList* movies, int selection);

int prompt_for_year();

/*
Given a list of movies, print all titles to stdout, one per line
*/
void print_movie_titles(MovieList* movies, int year);

/*
  Given a list of movies and a year, return a list of all movies made in the
  given year
*/
MovieList* years_movies(MovieList* all_movies, int year);

/*
  Given a list of movies, print the highest rated for every year
*/
void print_best_each_year(MovieList * movies);

/*
  Helper for print_best_each_year()
  Given a list of movies, print the details of the highest rated
*/
void print_best(MovieList* movies);

void print_for_language(MovieList* movies);

void print_all_in_language(char* language, MovieList* movies);

