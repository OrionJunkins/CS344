int user_selection(char* prompt, int min, int max) 
{
    /*
        Print a given prompt and get input from the user repeateadly until input is valid.
        Params:
            char* prompt    prompt which will be printed every time input is requested
            int min         lower bound for input (inclusive)
            int max         upper bound for input (inclusive)
        Returns:
            int user_selection    integer value within the range [min,max]
    */

    // Prompt the user
    printf("%s", prompt);

    // Get input
    ssize_t line_size;
    char* input_buffer;
    size_t input_buffer_size = MAX_INPUT_LENGTH;
    input_buffer = (char*)(calloc(input_buffer_size, sizeof(char)));
    int user_selection = -1;
    line_size = getline(&input_buffer, &input_buffer_size, stdin);
    user_selection = atoi(input_buffer);

    // If input is not in the valid range, continually re prompt until it is
    while(user_selection < min | user_selection > max)
    {
        printf("Invalid choice, please try again\n\n%s", prompt);
        line_size = getline(&input_buffer, &input_buffer_size, stdin);
        user_selection = atoi(input_buffer);
    }

    return user_selection;
    }