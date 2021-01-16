/*
Orion Junkins
Assignment 1: Handles file parsing and user interaction.
Implements movie_list.h
*/
#include "movie_list.h"

// ************************************************************ //
// MovieList helpers
// ************************************************************ //
void add_movie(MovieList* list, Movie* new_movie){
  /*
  Given an existing MovieList and a specific movie, create a node for 
  the new movie and add it to the head of the list
  Params: 
    MovieList* list    Existing list to which the movie will be added
    Movie* new_movie   New movie which is being added
  */

  // Create a new MovieNode for new_movie and put it at the front of the list
  MovieNode* new_node = (MovieNode*)(malloc(sizeof(MovieNode)));
  new_node->movie = new_movie;
  new_node->next = list->first;
  list->first = new_node;

  // Increment the size
  list->size++;
}

void free_MovieList(MovieList* list){
  /*
  Free all allocated memory in the list, all its' nodes, and all its' nodes' movies
  Params:
    MovieList* list   The movie list to free
  */

  // Temp pointer to move through the linked list
  MovieNode* tmp = list->first;

  // So long as nodes remain in the list, free all allocated memory
  while(list->first != NULL){
    tmp = list->first->next;
    free(list->first->movie->title);
    free(list->first->movie);
    free(list->first);
    list->first = tmp;
  }
}



// ************************************************************ //
// File Parsing
// ************************************************************ //
void parse_file(MovieList* movies, char* filepath) {
  /*
  Fetch the file given by filepath, open it and parse it line by line.
  For every line of data, create a Movie and add it to the MovieList movies.
  The file at filepath is assumed to be formatted correctly.
  Params:
    MovieList* movies   The existing list to which all movies will be added 
    char* filepath      The specified filepath which will be opened & parsed
  */
 
  // Open the specified file and check validity
  FILE* fp = fopen(filepath, "r");
  if(fp == NULL){
    printf("Error reading %s", filepath);
    return;
  }

  // Setup for getline()
  char* line_contents = NULL;
  size_t line_buffer_size = 0;
  ssize_t line_size;

  // Get the first line (column headers). This line is ignored.
  line_size = getline(&line_contents, &line_buffer_size, fp);

  // So long as nonempty lines exist, get next line, parse it into a new Movie, and add it to movies
  line_size = getline(&line_contents, &line_buffer_size, fp);
  while(line_size >=0){
    Movie* movie = (Movie*)(malloc(sizeof(Movie))); 
    if(movie == NULL){
      printf("Error allocating memory");
      return;
    }
    parse_line(movie, line_contents);
    add_movie(movies, movie);
    line_size = getline(&line_contents, &line_buffer_size, fp);
  }

  // Free memory and close the file
  free(line_contents);
  line_contents = NULL;
  fclose(fp);
  
  // Output a statement declaring success
  output_sucess(filepath, movies->size);
}

void output_sucess(char* filepath, int movies_parsed){
  /*
  Given a filepath name and a number of movies parsed print a success statement
  Params:
    char* filepath      Path of movie parsed 
    int movies_parsed   Number of movies parsed
  */
  printf("Processed file %s and parsed data for %d movies\n", filepath, movies_parsed);
}

void parse_line(Movie* movie, char* input){
  /*
  Given a line of input from a csv, parse the title, year, rating and languages from the respective columns.
  Set all fields of movie with the corresponding details.
  Params:
    Movie* movie    The movie object to which all parsed data will be assigned 
    char* input     The input string. Assumed to be in the correct format.
  */

  // Parse input and set the title of movie
  char* pt;
  pt = strtok(input, ",");
  movie->title = (char*)(calloc(strlen(pt) + 1, sizeof(char))); 
  strcpy(movie->title, pt);
  
  // Continue parsing and set the year of movie
  pt=strtok(NULL, ",");
  movie->year = atoi(pt);
  
  // Continue parsing and store the languages strings
  pt=strtok(NULL, ",");
  char* languages = (char*)(calloc(sizeof(char), strlen(pt)));
  strcpy(languages, pt);
  
  // Continue parsing and set the rating of movie
  pt=strtok(NULL, ",");
  movie->rating = atof(pt);

  // Parse the languages string and store an array of languages in movie
  assign_languages(movie, languages);

  // Free languages
  free(languages);
}

void assign_languages(Movie* movie, char* input){
  /*
  Given a string of input languages, parse the string and store each language in the languages array of the specified movie
  Params:
    Movie* movie    The movie object to which parsed languages will be assigend 
    char* input     The input string in the form '[lang1;lang2;lang3]'
  */

  // Remove leading '[' and trailing ']' from input
  input += 1;
  input[strlen(input)-1] = '\0';

  // Parse the string and store languages in movie->languages
  int index = 0;
  char* pt = strtok (input,";");
  while (pt != NULL) {
      strcpy(movie->languages[index], pt);
      pt = strtok(NULL, ";");
      index += 1;
  }
}



// ************************************************************ //
// User Interaction
// ************************************************************ //
void query_user(MovieList* movies){
  /*
  Prompt the user with a series of options for exploring the provided movies data.
  Respond to their queries.
  Params:
    MovieList* movies   The list of movies which is to be explored by the user
  */

  // Setup to get user selection with getline()
  ssize_t line_size;
  char* input_buffer;
  size_t input_buffer_size = MAX_INPUT_LENGTH;
  input_buffer = (char*)(calloc(input_buffer_size, sizeof(char)));
  int user_selection;

  // Repeatedly prompt the user with options, get user input and respond accordingly until the user enters '4' (exit)
  while(user_selection != 4){
    print_initial_prompt();
    line_size = getline(&input_buffer, &input_buffer_size, stdin);
    user_selection = atoi(input_buffer);

    // Only respond if user_selection is a valid choice
    if(user_selection == 1 || user_selection == 2 || user_selection == 3 || user_selection == 4){
      respond_to_user(movies, user_selection);
    } else{
      printf("You entered an incorrect choice. Try again.\n");
    }
  }
  // Free the buffer used by getline()
  free(input_buffer);
}

void print_initial_prompt(){
  /*
  Print standard query options list for user
  */
  printf("\n1. Show movies released in the specified year\n2. Show highest rated movie for each year\n3. Show the title and year of release of all movies in a specific language\n4. Exit from the program\n\nEnter a choice from 1 to 4: ");
}

void respond_to_user(MovieList* movies, int selection){
  /*
  Given a list of movies and a user's selected query, respond correctly
  Params:
    MovieList* movies   The MovieList of available data
    int selection       The option inputted by the user
  */
  switch(selection){
    int year; // For use in case 1
    case 1:   // "Show movies released in the specified year"
      // Get the user to enter a specific year, and produce a list of movies made in that year
      year = prompt_for_year();
      MovieList* filtered_movies = years_movies(movies, year);

      // Print the filtered list 
      print_movie_titles(filtered_movies, year);
      free_MovieList(filtered_movies);
      break;
    case 2:
      // "Show highest rated movie for each year"
      print_best_each_year(movies);
      break;
    case 3:
      // "Show the title and year of release of all movies in a specific language"
      print_for_language(movies);
      break;
    case 4:
    // "Exit from the program"
      break;
    default:
    break;

  }
}

int prompt_for_year(){
  /*
  In order to return data for a certain year, prompt the user to enter a year 
  Get from stdin, and return an int
  */
  int year;
  printf("Enter the year for which you want to see movies: ");

  // Setup for getline()
  ssize_t line_size;
  char* input_buffer;
  size_t input_buffer_size = 50;
  input_buffer = (char*)(calloc(input_buffer_size, sizeof(char)));

  //Get input
  line_size = getline(&input_buffer, &input_buffer_size, stdin);

  // Cast to an int saved in year
  year = atoi(input_buffer);

  // Free the input_buffer
  free(input_buffer);

  return year;
}

MovieList* years_movies(MovieList* all_movies, int year){
  /*
  Given a list of movies and a year, return a MovieList of all movies made in the given year
  Params:
    MovieList* all_movies, int year
  */
  MovieList* filtered_movies = malloc(sizeof(MovieList)); 
  filtered_movies->first = NULL;

  MovieNode * node = all_movies->first;
  while(node != NULL){
    if(node->movie->year == year){
      add_movie(filtered_movies, node->movie);
    }
    node = node->next;
  }
  return filtered_movies;
}

void print_movie_titles(MovieList* movies, int year){
  /*
    Given a list of movies, print all titles to stdout, one per line
  */
 if (movies->first == NULL){
    printf("No data about movies released in the year %d\n", year);
  } else {
    MovieNode * node = movies->first;
    while(node != NULL){
      printf("%s\n", node->movie->title);
      node = node->next;
    }
  }
}



void print_best_each_year(MovieList * movies){
  /*
    Given a list of movies, print the highest rated for every year
  */
  for(int i = MIN_YEAR; i <= MAX_YEAR; i++){
    print_best(years_movies(movies, i));
  }
}

void print_best(MovieList* movies){
  /*
    Helper for print_best_each_year()
    Given a list of movies, print the details of the highest rated
  */
  Movie* highest_rated = (Movie*)(calloc(1,sizeof(Movie*)));
  highest_rated->rating = -1;

  MovieNode * node = movies->first;
  while(node != NULL){
    if(node->movie->rating > highest_rated->rating){
      highest_rated = node->movie;
    }
    node = node->next;
  }
  if(highest_rated->title != NULL){
    printf("%d %.1f %s\n", highest_rated->year, highest_rated->rating, highest_rated->title);
  }  
}

void print_for_language(MovieList* movies) {
  char* language;
  printf("Enter the language for which you want to see movies: ");

  ssize_t line_size;
  size_t input_buffer_size = 21;
  language = (char*)(calloc(input_buffer_size, sizeof(char)));
  line_size = getline(&language, &input_buffer_size, stdin);
  language[strlen(language)-1] = '\0';
  print_all_in_language(language, movies);
  free(language);
}

void print_all_in_language(char* language, MovieList* movies){
  MovieNode * node = movies->first;
  int match_found = 0;
  while(node != NULL){
    for (int i = 0; i < 5; i++){
      if(strcmp(node->movie->languages[i], language) == 0) {
        match_found = 1;
        printf("%d %s\n", node->movie->year, node->movie->title);
      }
    } 
    node = node->next;
  } 
  if (match_found == 0) {
      printf("No data about movies released in %s\n", language);
  }
}