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

#define MIN_YEAR 1900 //TODO
#define MAX_YEAR 2021
#define MAX_INPUT_LENGTH 100 
#define MAX_NUM_LANGUAGES 5
#define MAX_LANG_LENGTH 21

typedef struct Movie 
{
    /*
        Basic type/struct to hold the details of a single movie.
    */
    char* title;
    int year;
    float rating;
    char* languages;
} Movie;


typedef struct MovieNode 
{
    /*
        Combines a Movie* with a pointer to another MovieNode to allow Movies to be stored in a linked list
    */
    Movie* movie;
    struct MovieNode* next;
} MovieNode;

typedef struct MovieList 
{
    /*
        Stores a pointer to the first MovieNode of the linked list and the list size
    */
    MovieNode* first;
    int size;
} MovieList;


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
void parse_file(MovieList* movies, char* filename);
/*
Fetch the file given by filepath, open it and parse it line by line.
For every line of data, create a Movie and add it to the MovieList movies.
The file at filepath is assumed to be formatted correctly.
Params:
    MovieList* movies   The existing list to which all movies will be added 
    char* filepath      The specified filepath which will be opened & parsed
*/

void parse_line(Movie* movie, char* input);
/*
    Given a line of input from a csv, parse the title, year, rating and languages from the respective columns.
    Set all fields of movie with the corresponding details.
    Params:
        Movie* movie    The movie object to which all parsed data will be assigned 
        char* input     The input string. Assumed to be in the correct format.
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


// ************************************************************ //
// Directory/File Creation
// ************************************************************ //
void create_all_yearly_files(char* dir_path, MovieList* movies);
/*
    Given a list of movies, create a file for every year in which at least one movie was made.
    The file should contain a list of all movie titles made in that year.
    Params:
        char* dir_path          The directory path to which movies will be saved
        MovieList* movies       The list of movies from which data will be pulled
*/


void create_yearly_file(char* dir_path, int year, MovieList* movies_from_year);
/*
    Create a new file in the form 'YYYY.txt' for all of the movies in the given list/year
    Params:
        char* dir_path                  The directory path to which the new file will be saved 
        int year                        The year for which the file is to be created
        MovieList* movies_from_year     The list of movies from the specified year
*/
