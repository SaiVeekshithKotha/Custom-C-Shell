#include "headers/headers.h"

void log_purge(char * path_to_log)
{
    FILE *fptr = fopen(path_to_log, "w");
    if (fptr == NULL)
    {
        printf("\033[31mError: Unable to open log file\033[0m\n");
    }
    fclose(fptr);
}

char* log_execute_com(char *path_to_log, int n)
{
    FILE *fptr = fopen(path_to_log, "r");
    if (fptr == NULL)
    {
        printf("\033[31mError: No such command in log\033[0m\n");
        return NULL;
    }

    char *string = (char *)malloc(MAX_SIZE);
    if (string == NULL)
    {
        printf("\033[31mError: Memory allocation failed\033[0m\n");
        fclose(fptr);
        return NULL;
    }

    int line = 0;
    while (fgets(string, MAX_SIZE, fptr) != NULL)
    {
        if (strcmp(string, "\n") != 0)  // Skip empty lines
        {
            line++;
        }
    }

    if (line < n || n <= 0)
    {
        printf("\033[31mError: No such command in log\033[0m\n");
        free(string);
        fclose(fptr);
        return NULL;
    }

    // Calculate the target line
    int target_line = line - n + 1;

    // Rewind file to start reading again
    rewind(fptr);
    line = 0;
    while (fgets(string, MAX_SIZE, fptr) != NULL)
    {
        if (strcmp(string, "\n") != 0)  // Skip empty lines
        {
            line++;
        }
        if (line == target_line)
        {
            break;
        }
    }
    fclose(fptr);

    // Remove the newline character at the end, if present
    int len = strlen(string);
    if (len > 0 && string[len - 1] == '\n')
    {
        string[len - 1] = '\0';
    }

    return string;
}


void log_store(char * path_to_log ,char *string)
{
    if(strcmp(string , "") == 0 || strcmp(string , "\n") == 0 || string == NULL) return ;
    for(int i = strlen(string) - 1 ; i >= 0 ; i--)
    {
        if(string[i] == '\n')
        {
            continue;
        }else{
            string[i + 1] = '\0';
            break;
        }
    }
    char string1[MAX_SIZE] ;
    int line = 0 ;
    FILE *fptr = fopen(path_to_log, "r");
    if (fptr == NULL)
    {
        fptr = fopen(path_to_log, "w");
        if (fptr == NULL)
        {
            printf("\033[031m Error: Unable to open log file\033[0m\n");
        }
        fprintf(fptr, "%s\n", string);
        fclose(fptr);
    }else{
        char history[15][MAX_SIZE] ;
        while(fgets(string1, MAX_SIZE, fptr) != NULL)
        {
            snprintf(history[line], MAX_SIZE, "%s", string1);
            line+= 1 ;
        }
        fclose(fptr);
        for(int i = strlen(string1) - 1 ; i >= 0 ; i--)
        {
            if(string1[i] == '\n')
            {
                continue;
            }else{
                string1[i + 1] = '\0';
                break;
            }
        }
        if(strcmp(string1 ,string ) == 0)
        {
            return ;
        }
        if(line == 15)
        {
            FILE *fptr = fopen(path_to_log, "w");
            if (fptr == NULL)
            {
                printf("\033[031m Error: Unable to open log file\033[0m\n");
            }
            for(int i = 1 ; i < 15 ; i++)
            {
                fprintf(fptr, "%s", history[i]);
            }
            fprintf(fptr, "%s\n", string);
            fclose(fptr);
        }else{
            FILE *fptr1 = fopen(path_to_log, "a");
            if (fptr1 == NULL)
            {
                printf("\033[031m Error: Unable to open file\033[0m");
            }
            fprintf(fptr1, "%s\n", string);
            fclose(fptr1);
        }
        return ;
    }
}

void log_print(char *path_to_log)
{
    int line = 0;
    FILE *fptr = fopen(path_to_log, "r");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    char string[MAX_SIZE] ;
    while(fgets(string, MAX_SIZE, fptr) != NULL)
    {
        printf("%s", string);
        line += 1;
    }
    if(line == 0)
    {
        printf("\n");
    }
    fclose(fptr);
}
