/*
Orion Junkins
Assignment 1: Movies
junkinso@oregonstate.edu
Handles file parsing and user interaction.
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
  if(new_node == NULL) {
    printf("Error allocating memory\n");
  }
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
      printf("Error allocating memory\n");
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
  char* languages = (char*)(calloc(MAX_NUM_LANGUAGES * MAX_LANG_LENGTH, sizeof(char)));
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

  while(index < MAX_NUM_LANGUAGES){
    movie->languages[index][0] = '\0'; 
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
  int user_selection = 0;

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

      MovieList* filtered_movies = (MovieList*)(malloc(sizeof(MovieList))); 
      if(filtered_movies == NULL){
        printf("Error allocating memory\n");
      }
      filtered_movies->first = NULL;
      filtered_movies->size = 0;

      years_movies(filtered_movies, movies, year);    

      // Print the filtered list 
      print_movie_titles(filtered_movies, year);

      free_MovieList(filtered_movies);
      free(filtered_movies);
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
  size_t input_buffer_size = MAX_INPUT_LENGTH;
  input_buffer = (char*)(calloc(input_buffer_size, sizeof(char)));

  //Get input
  line_size = getline(&input_buffer, &input_buffer_size, stdin);

  // Cast to an int saved in year
  year = atoi(input_buffer);

  // Free the input_buffer
  free(input_buffer);

  return year;
}



void print_movie_titles(MovieList* movies, int year){
  /*
    Given a list of movies, print all titles to stdout, one per line
    Params:
      MovieList* movies   MovieList of 
      int year            The year of the movies in MovieList
  */

 // Enssure there are movies to print
 if (movies->first == NULL){
    printf("No data about movies released in the year %d\n", year);
  } else {
    // Iterate all of movies printing the title for each
    MovieNode * node = movies->first;
    while(node != NULL){
      printf("%s\n", node->movie->title);
      node = node->next;
    }
  }
}

void print_best_each_year(MovieList * movies){
  /*
    Helper for print_best_each_year().
    Given a list of movies, print the details of the highest rated
    Params:
      MovieList* movies   The MovieList from which the best will be printed
  */

  // For every year, generate a temporary MovieList of movies from that year, print the best movie, and free the temporary list
  int i;
  for(i = MIN_YEAR; i <= MAX_YEAR; i++){
    // Allocate a temp MovieList
    MovieList* movies_from_year = (MovieList*)(malloc(sizeof(MovieList))); 
    if(movies_from_year == NULL){
      printf("Error allocating memory\n");
    }
    movies_from_year->first = NULL;

    // Add every movie in movies from year i to movies_from_year
    years_movies(movies_from_year, movies, i);

    // Print the best movie from year i
    if(movies_from_year->first != NULL){
      print_best(movies_from_year); 
    }

    // Free the temp MovieList
    free_MovieList(movies_from_year);
    free(movies_from_year);
  }
}

void print_best(MovieList* movies){
  /*
    Helper for print_best_each_year()
    Given a list of movies, print the details of the highest rated
    Params:
      MovieList* movies   The list of movies from which the single best will be printed
  */

  // Crete a MovieNode to traverse movies
  MovieNode* node = movies->first;

  // Create a Movie* to hold the highest rated found
  Movie* highest_rated = node->movie; 
  
  // For every movie, compare the rating to the rating of the highest rated seen so far. 
  // Update highest_rated if a higher rated movie is found
  while(node != NULL){
    if(node->movie->rating > highest_rated->rating){
      highest_rated = node->movie;
    }
    node = node->next;
  }

  // Print the highest rated movie found so long as at least one movie was found
  if(highest_rated->title != NULL){
    printf("%d %.1f %s\n", highest_rated->year, highest_rated->rating, highest_rated->title);
  }  
}

void print_for_language(MovieList* movies) {
  /*
    Prompt the user for a language and print every movie in movies which was produced in that language
    Params:
      MovieList* movies   The MovieList from which matches will be printed
  */

  // Prompt the user for a language and store the inpput 
  printf("Enter the language for which you want to see movies: ");
  char* language;
  ssize_t line_size;
  size_t input_buffer_size = MAX_LANG_LENGTH;
  language = (char*)(calloc(input_buffer_size, sizeof(char)));
  line_size = getline(&language, &input_buffer_size, stdin);
  language[strlen(language)-1] = '\0'; // Ensure the line ends with a null terminator

  // Print every movie in movies which was produced in language
  print_all_in_language(language, movies);
  free(language);
}

void print_all_in_language(char* language, MovieList* movies){
  /*
    Given a language, print every movie in movies which was produced in that language.
    Params:
      char* language      Specified language 
      MovieList* movies   List of movies which is being searched
  */
  int match_found = 0;

  // Iterate through every movie
  MovieNode * node = movies->first;
  while(node != NULL){
    // For every language stored in each movie, check against language for a match
    int i;
    for (i = 0; i < MAX_NUM_LANGUAGES; i++){
      // If a match is found, update match_found and print the year and title
      if(node->movie->languages[i][0] != '\0'){
        if (strcmp(node->movie->languages[i], language) == 0) {
          match_found = 1;
          printf("%d %s\n", node->movie->year, node->movie->title);
        }
      }
    } 
    node = node->next;
  } 

  // If no matches were found throughout the entire list, inform the user
  if (match_found == 0) {
      printf("No data about movies released in %s\n", language);
  }
}
