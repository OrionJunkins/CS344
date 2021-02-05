if(strcmp("&",pt) == 0){        // token is '&'
            // Grab the next token
            pt = strtok_r(NULL, " ", &save_ptr); 
            printf("%c\n", pt[0]);
            printf("%c\n", pt[1]);
            printf("%c\n", pt[2]);
            printf("%c\n", pt[3]);
            printf("%c\n", pt[4]);

            if(pt == NULL){ 
                // '&' is the end of the command        
                // Set the command as a background if BG commands are enabled
                if (BACKGROUND_ENABLED){
                    command->background = true;
                }
            } else { 
                // '&' is not the end => just another argument       
                // Copy both '&' and the token following it the arguments list
                strcpy(command->arguments[command->arg_count], "&");            
                command->arg_count += 1;
                strcpy(command->arguments[command->arg_count], pt);            
                command->arg_count += 1;
            }
        } else 