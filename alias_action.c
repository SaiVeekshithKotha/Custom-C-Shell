#include "headers/headers.h"

void alias_loading(char *home) {
    char myshrc_path[MAX_SIZE];
    snprintf(myshrc_path, MAX_SIZE, "%s/.myshrc", home);

    FILE *fptr = fopen(myshrc_path, "r");
    if (fptr == NULL) {
        printf("\033[31mError in opening file %s\033[0m\n", myshrc_path);
        return;
    }

    // Allocate buffer for reading lines
    char *line = (char *)malloc(MAX_SIZE);
    if (line == NULL) {
        printf("\033[31mError in allocating memory for line\033[0m\n");
        fclose(fptr);
        return;
    }

    // Read through the file line by line
    while (fgets(line, MAX_SIZE, fptr) != NULL) {
        char *prefix = "alias ";
        // Check if the line starts with "alias"
        if (strncmp(line, prefix, strlen(prefix)) == 0) {
            char *alias_line = line + strlen(prefix);

            // Parse alias word and alias command
            char *alias_word = trim_space(strtok(alias_line, "="));
            char *alias_command = trim_space(strtok(NULL, "'\n"));

            if (alias_command && alias_word) {
                Aliases[alias_count].alias_name = strdup(alias_word);
                Aliases[alias_count].command = strdup(alias_command);
                alias_count += 1;
            }
        }
    }

    // Free allocated memory and close file
    free(line);
    fclose(fptr);
}

char * check_alias_command(char * commands)
{
    for(int i = 0 ; i < alias_count ; i+=1)
    {
        if(strcmp(Aliases[i].alias_name , commands) == 0)
        {
            return Aliases[i].command ;
        }
    }
    return NULL ;
}