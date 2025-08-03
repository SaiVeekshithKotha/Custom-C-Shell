#include "headers/headers.h"

char *getInput_line(char * line)
{
    line = (char *)malloc(sizeof(char) * MAX_SIZE);
    if (line == NULL)
    {
        printf("\033[031mError in allocating memory\033[0m\n");
        return NULL;
    }

    size_t size = MAX_SIZE;
    ssize_t len = getline(&line, &size, stdin);

    if (len == -1)
    {
        // Handle EOF (Ctrl + D)
        if (feof(stdin))
        {
            // If EOF is encountered, clean up and exit
            free(line);
            kill_all_background_processes();
            printf("\nExiting shell...\n");
            exit(0);
        }
        else
        {
            // If there is an error with getline, handle it appropriately
            // perror("getline");
            printf("\033[031mError in reading input\033[0m\n");
            free(line);
            return NULL;
        }
    }

    // Remove the newline character at the end, if present
    if (len > 0 && line[len - 1] == '\n')
    {
        line[len - 1] = '\0';
    }

    return line;
}

char ** splitInput_line(char *line) {
    char **tokens = (char **)malloc(sizeof(char *) * ( MAX_SIZE ));
    if(tokens == NULL)
    {
        printf("\033[031mError in allocating memory\033[0m\n");
        return NULL;
    }
    char *token = strtok(line, ";");
    int i = 0;
    while(token != NULL) {
        tokens[i] = trim_space(token);
        i++;
        token = strtok(NULL, ";");
    }
    tokens[i] = NULL;
    return tokens;
}

char **get_backround_commands(char *commands , int *num_commands)
{
    int j = 0;
    for(;j < strlen(commands) ; j++)
    {
        if(commands[j] == ' ' || commands[j] == '\t' || commands[j] == '\r')
        {
            continue ;
        }else if(commands[j] == '&')
        {
            continue ;
        }
        else{
            break ;
        }
    }
    int i =0;
    bool flag = false ;
    for(i = strlen(commands) - 1 ; i >= 0 ; i--)
    {
        if(commands[i] == ' ' || commands[i] == '\t' || commands[i] == '\r')
        {
            continue ;
        }else if(commands[i] == '&')
        {
            flag = true ;
            continue ;
        }
        else{
            commands[i + 1] = '\0' ;
            break ;
        }
    }

    char **tokens = (char **)malloc(sizeof(char*) * (MAX_SIZE));
    if(tokens == NULL)
    {
        printf("\033[031mError in allocating memory\033[0m\n");
        return NULL;
    }
    char *token = strtok(commands + j, "&");
    i = 0;
    while(token != NULL)
    {
        tokens[i] = trim_space(token);
        
        i += 1 ;
        token = strtok(NULL, "&");
    }
    tokens[i] = NULL ;
    
    *num_commands = i;
    *num_commands += flag ;
    return tokens ;
}

char **split_command(char *command) // Parses based on spaces
{
    char **segs = (char **)malloc(MAX_SIZE * sizeof(char *));
    if (!segs)
    {
        printf("\033[031mError in allocating memory\033[0m\n");
        return NULL;
    }

    for (int i = 0; i < MAX_SIZE; i++)
    {
        segs[i] = (char *)malloc(MAX_SIZE * sizeof(char));
        if (!segs[i])
        {
            // perror("malloc");
            printf("\033[031mError in allocating memory\033[0m\n");
            return NULL;
        }
        segs[i][0] = '\0'; // Initialize each segment with '\0'
    }

    // Breaking the command into segments based on spaces, tabs, and carriage returns
    int i = 0, j = 0;
    int segment_len = 0;
    while (command[i] != '\0')
    {
        if (command[i] == ' ' || command[i] == '\t' || command[i] == '\r')
        {
            if (segment_len > 0)
            {
                segs[j][segment_len] = '\0';
                segment_len = 0;
                j++;
            }
        }
        else
        {
            if (j < MAX_SIZE && segment_len < MAX_SIZE - 1)
            {
                segs[j][segment_len++] = command[i];
            }
        }
        i++;
    }

    // Ensure the last segment is null-terminated if there's any content
    if (segment_len > 0)
    {
        segs[j][segment_len] = '\0';
    }

    segs[j + 1] = NULL; // Ensure the final element is NULL to indicate the end of the array

    return segs;
}


void preprocess_input(char *line)
{
    char temp[MAX_SIZE];
    char *p = temp;
    char *line2 = malloc(MAX_SIZE);
    char *line3 = line2;
    strcpy(line2, line);
    int k = 0;
    while (*line2)
    {
        if (*line2 == '<' || *line2 == '|')
        {
            *p++ = ' ';
            *p++ = *line2++;
            *p++ = ' ';
            k += 3;
        }
        else if (*line2 == '>' && *(line2 + 1) == '>')
        {
            *p++ = ' ';
            *p++ = *line2++;
            *p++ = *line2++;
            *p++ = ' ';
            k += 4;
        }
        else if (*line2 == '>')
        {
            *p++ = ' ';
            *p++ = *line2++;
            *p++ = ' ';
            k += 3;
        }
        else
        {
            *p++ = *line2++;
            k += 1;
        }
    }
    // *p = '\0';
    // Copy processed string back to the original line
    strcpy(line, temp);
    line[k] = '\0';
    free(line3);
}