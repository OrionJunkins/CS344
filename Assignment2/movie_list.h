/*
Orion Junkins
junkinso@oregonstate.edu
Assignment 2
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define MIN_YEAR 2003 //TODO
#define MAX_YEAR 2021
#define MAX_INPUT_LENGTH 100 
#define MAX_NUM_LANGUAGES 5
#define MAX_LANG_LENGTH 21




typedef struct Movie {
/*
Basic struct to hold the details of a single movie.
*/
char* title;
int year;
float rating;
char* languages;
} Movie;

/*
Combines a Movie* with a pointer to another MovieNode to allow movies to be stored in a linked list
*/
typedef struct MovieNode {
  Movie* movie;
  struct MovieNode* next;
} MovieNode;

typedef struct MovieList {
/*
  Stores a pointer to the first MovieNode of the linked list and the list size
*/
  MovieNode* first;
  int size;
} MovieList;

void create_year_file(int year, MovieList* years_movies);
void create_yearly_file(char* dir_path, int year, MovieList* movies_from_year);

// ************************************************************ //
// MovieList helpers
// ************************************************************ //
void add_movie(MovieList* list, Movie* new_movie);
  /*
    Given an existing MovieList and a specific movie, create a node for 
    the new movie and add it to the head of the list
    Params: 
      MovieList* list    Existing list to which the movie will be added
      Movie* new_movie   New movie which is being added
  */

void free_MovieList(MovieList* list);
  /*
    Free all allocated memory in the list, all its' nodes, and all its' nodes' movies
    Params:
      MovieList* list   The movie list to free
  */



// ************************************************************ //
// File Parsing
// ************************************************************ //
void parse_file(MovieList* movies, char* filepath);
  /*
    Fetch the file given by filepath, open it and parse it line by line.
    For every line of data, create a Movie and add it to the MovieList movies.
    The file at filepath is assumed to be formatted correctly.
    Params:
      MovieList* movies   The existing list to which all movies will be added 
      char* filepath      The specified filepath which will be opened & parsed
  */

void output_sucess(char* filepath, int movies_parsed);
  /*
    Given a filepath name and a number of movies parsed print a success statement
    Params:
      char* filepath      Path of movie parsed 
      int movies_parsed   Number of movies parsed
  */

void parse_line(Movie* movie, char* input);
  /*
    Given a line of input from a csv, parse the title, year, rating and languages from the respective columns.
    Set all fields of movie with the corresponding details.
    Params:
      Movie* movie    The movie object to which all parsed data will be assigned 
      char* input     The input string. Assumed to be in the correct format.
  */

void assign_languages(Movie* movie, char* input);
  /*
    Given a string of input languages, parse the string and store each language in the languages array of the specified movie
    Params:
      Movie* movie    The movie object to which parsed languages will be assigend 
      char* input     The input string in the form '[lang1;lang2;lang3]'
  */



// ************************************************************ //
// User Interaction
// ************************************************************ //
void query_user(MovieList* movies);
  /*
    Prompt the user with a series of options for exploring the provided movies data.
    Respond to their queries.
    Params:
      MovieList* movies   The list of movies which is to be explored by the user
  */

void print_initial_prompt();
  /*
    Print standard query options list for user
  */

void respond_to_user(MovieList* movies, int selection);
  /*
    Given a list of movies and a user's selected query, respond correctly
    Params:
      MovieList* movies   The MovieList of available data
      int selection       The option inputted by the user
  */

int prompt_for_year();
  /*
    In order to return data for a certain year, prompt the user to enter a year 
    Get from stdin, and return an int
  */

void years_movies(MovieList* filtered_movies, MovieList* all_movies, int year);
  /*
    Given a list of movies and a year, return a MovieList of all movies made in the given year
    Params:
      MovieList* all_movies   Primary MovieList which will be searched for matches
      int year                Year for which matches will be generated
  */

 void copy_Movie(Movie* dest, Movie* src);
  /*
    Given an empty Movie dest, set (and allocate if needed) all of its' fields with the values held in src
    Params:
      Movie* dest     Destination; Empty Movie
      Movie* src      Source; Movie to be duplicated
  */

void print_movie_titles(MovieList* movies, int year);
  /*
    Given a list of movies, print all titles to stdout, one per line
    Params:
      MovieList* movies   MovieList of 
      int year            The year of the movies in MovieList
  */

void print_best_each_year(MovieList * movies);
  /*
    Helper for print_best_each_year().
    Given a list of movies, print the details of the highest rated
    Params:
      MovieList* movies   The MovieList from which the best will be printed
  */

void print_best(MovieList* movies);
  /*
    Helper for print_best_each_year()
    Given a list of movies, print the details of the highest rated
    Params:
      MovieList* movies   The list of movies from which the single best will be printed
  */

void print_for_language(MovieList* movies);
  /*
    Prompt the user for a language and print every movie in movies which was produced in that language
    Params:
      MovieList* movies   The MovieList from which matches will be printed
  */

void print_all_in_language(char* language, MovieList* movies);
  /*
    Given a language, print every movie in movies which was produced in that language.
    Params:
      char* language      Specified language 
      MovieList* movies   List of movies which is being searched
  */
