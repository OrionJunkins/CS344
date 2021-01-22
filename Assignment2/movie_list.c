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
void parse_file(MovieList* movies, char* filename) {
  /*
    Fetch the file given by filepath, open it and parse it line by line.
    For every line of data, create a Movie and add it to the MovieList movies.
    The file at filepath is assumed to be formatted correctly.
    Params:
      MovieList* movies   The existing list to which all movies will be added 
      char* filepath      The specified filepath which will be opened & parsed
  */
 
  // Open the specified file and check validity
    FILE* fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("Error reading %s", filename);
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
    while(line_size >=0)
    {
        Movie* movie = (Movie*)(malloc(sizeof(Movie))); 
        if(movie == NULL){
        printf("Error allocating memory\n");
            return;
        }
        parse_line(movie, line_contents);

        add_movie(movies, movie);
        
        line_size = getline(&line_contents, &line_buffer_size, fp);        
    }

    // Close the file
    fclose(fp);
    
    // Output a statement declaring success
    output_sucess(filename, movies->size);
}

void output_sucess(char* filepath, int movies_parsed)
{
    /*
        Given a filepath name and a number of movies parsed print a success statement
        Params:
        char* filepath      Path of movie parsed 
        int movies_parsed   Number of movies parsed
    */
    printf("Processed file %s and parsed data for %d movies\n", filepath, movies_parsed);
}

void parse_line(Movie* movie, char* input)
{
    /*
        //TODO Check input for valid format
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
    movie->languages = (char*)(calloc(strlen(pt) + 1, sizeof(char))); 
    strcpy(movie->languages, pt);
    
    // Continue parsing and set the rating of movie
    pt=strtok(NULL, ",");
    movie->rating = atof(pt);

}

void years_movies(MovieList* filtered_movies, MovieList* all_movies, int year)
{
    /*
        Given a list of movies and a year, return a MovieList of all movies made in the given year
        Params:
        MovieList* all_movies   Primary MovieList which will be searched for matches
        int year                Year for which matches will be generated
    */

    MovieNode * node = all_movies->first;

    // Iterate all_movies checking the year of each node's movie against year, 
    // Copy that movie to filtered_movies if a match is found
    while(node != NULL)
    {
        if(node->movie->year == year)
        {
            Movie * matching_movie = (Movie*)(malloc(sizeof(Movie)));
            copy_Movie(matching_movie, node->movie);
            add_movie(filtered_movies, matching_movie);
        }

        node = node->next;
    }
}

void copy_Movie(Movie* dest, Movie* src)
{
    /*
        Given an empty Movie dest, set (and allocate if needed) all of its' fields with the values held in src
        Params:
        Movie* dest     Destination; Empty Movie
        Movie* src      Source; Movie to be duplicated
    */

    // Allocate and copy a title based on the length of src->title
    dest->title = (char*)(calloc(strlen(src->title) + 1, sizeof(char))); 
    strcpy(dest->title, src->title);

    // Set year
    dest->year = src->year;
    
    // For every language array, copy from src to dest
    dest->languages = (char*)(calloc(strlen(src->languages) + 1, sizeof(char))); 
    strcpy(dest->languages, src->languages);

    // Set rating
    dest->rating = src->rating;
}


// ************************************************************ //
// Directory/File Creation
// ************************************************************ //
void create_yearly_files(char* dir_path, MovieList* movies)
{
    /*
        Given a list of movies, create a file for every year in which at least one movie was made.
        The file should contain a list of all movie titles made in that year.
    */
    int i;
    for(i = MIN_YEAR; i <= MAX_YEAR; i++)
    {
        // Allocate a temp MovieList
        MovieList* movies_from_year = (MovieList*)(malloc(sizeof(MovieList))); 
        if(movies_from_year == NULL)
        {
             printf("Error allocating memory\n");
        }
        movies_from_year->first = NULL;

        // Add every movie in movies from year i to movies_from_year
        years_movies(movies_from_year, movies, i);

        char year[5];  //TODO Remove if possible
        sprintf(year, "%d", i); 
        char* filepath = calloc(20, sizeof(char));
        strcat(filepath, dir_path);
        strcat(filepath, year);
        strcat(filepath, ".txt");
        
        int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0600); //TODO CHECK PERMISSIONS
        if (fd == -1)
        {
            printf("open() failed on \"%s\"\n", filepath);
            perror("Error");
            exit(1);
	    }

        MovieNode * node = movies_from_year->first;
        while(node != NULL)
        {
            char new_line[1 + strlen(node->movie->title)];
            strcpy(new_line, node->movie->title);
            strcat(new_line, "\n");
            write(fd, new_line, strlen(new_line));
            node=node->next;
        }        

        free_MovieList(movies_from_year);
        free(movies_from_year);
    }
}
