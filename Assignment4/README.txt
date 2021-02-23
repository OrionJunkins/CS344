Assignment 4
Multi-threaded Producer Consumer Pipeline
Orion Junkins
junkinso@oregonstate.edu

TO BUILD:
    $gcc -pthread -std=gnu99 -g -o line_processor main.c

TO RUN: 
    For stdin usage:
    $./line_processor

    Alternatively, route in any text file:
    $./line_processor < file.txt