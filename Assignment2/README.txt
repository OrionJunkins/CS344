Orion Junkins
junkinso@oregonstate.edu
Assignment 2


Included Files: 
    main.c
        Main program flow. Reads directory entries. 
        Finds a file in the current directory based on user specified criteria.
        Reads and processes the data in the chosen file.
        Creates a directory.
        Creates new files in the newly created directory and writes processed data to these files.

    movie_list.c
        Definitions for all movie handling functions
    
    movie_list.h
        Definitions for all needed structs/typedefs.
        Declarations for all movie_list.c functions

    file_processing.c 
        Definitions for all file handling functions

    file_processing.h 
        Declarations for all




TO BUILD/RUN:
    1) Save all movie data files in the same directory as all program files.

    2) For a file to be handled automatically, it must have the prefix "movies_" and suffix ".csv"

    3) Compile main.c into a desired executable such as "main"
        $ gcc -std=gnu99 -o main main.c

    4) Run the executable and interact with the program
        $./main
        