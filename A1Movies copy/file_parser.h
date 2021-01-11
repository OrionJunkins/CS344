/*
Orion Junkins
Assignment 1: Movies
Contains tools for taking a char* file path and processing each line in the 
respective file
*/
#include <stdio.h>
#include "movie_list.h"
#include "line_parser.h"

void parse_file(struct MovieList* movies, char* filepath) 
{

  //Open the specified file and check validity
  FILE* fp = fopen(filepath, "r");
  if(fp == NULL)
  {
    printf("Error reading %s", filepath);
  }

  //Setup for getline()
  char* line_contents = NULL;
  size_t line_buffer_size = 0;
  ssize_t line_size;

  //Get the first line
  line_size = getline(&line_contents, &line_buffer_size, fp);

  //TODO: Check that first line is valid "﻿Title,Year,Languages,Rating Value\r\n"

  //Define a new movie list with first = NULL
  
  movies->first = NULL;
  //So long as nonempty lines exist, get next line, parse it, and add it
  //to movie_list
  line_size = getline(&line_contents, &line_buffer_size, fp);
  while(line_size >=0) 
  {
    struct Movie* movie = (struct Movie*)(malloc(sizeof(struct Movie))); 
    parse_line(movie, line_contents);
    add_movie(movies, movie);
    line_size = getline(&line_contents, &line_buffer_size, fp);
  }

  //Free memory and close the file
  free(line_contents);
  line_contents = NULL;
  fclose(fp);
}