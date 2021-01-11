/*
Orion Junkins
Assignment 1: Movies
Given a line of movie information, parse the line into specific attributes
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "movie_list.h"


void assign_languages(struct Movie* movie, char* input);

void parse_line(struct Movie* movie, char* input){
  /*
  Given a formatted line of input from a csv, parse the title,
  year, rating and languages from the respective columns.
  Return a struct Movie* with all of the corresponding details.
  */
  if(movie == NULL){
    printf("Error allocating memory");
  }

  //Parse and set the title
  char* pt;
  pt = strtok(input, ",");
  movie->title = (char*)(calloc(strlen(pt) + 1, sizeof(char)));
  strcpy(movie->title, pt);
  
  //Parse and set the year
  pt=strtok(NULL, ",");
  movie->year = atoi(pt);
  
  //Parse and store the languages string
  pt=strtok(NULL, ",");
  char* languages = (char*)(calloc(sizeof(char), strlen(pt)));
  strcpy(languages, pt);
  
  //Parse and store the rating
  pt=strtok(NULL, ",");
  movie->rating = atof(pt);

  //Parse from the languages string and store in movie
  assign_languages(movie, languages);
  free(languages);
}


void assign_languages(struct Movie* movie, char* input){
  /*
  Given input in the form of '[lang1;lang2;lang3]' 
  store each language in the languages array of movie
  */

  //Remove leading '[' and trailing ']'
  input += 1;
  input[strlen(input)-1] = '\0';

  //Parse and store languages
  int index = 0;
  char* pt = strtok (input,";");
  while (pt != NULL) {
      strcpy(movie->languages[index], pt);
      pt = strtok(NULL, ";");
      index += 1;
  }
}
