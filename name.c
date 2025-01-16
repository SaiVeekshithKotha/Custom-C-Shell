#include "headers.h"

char * getUsername( char * username)
{
    username = (char *)malloc(MAX_SIZE) ;
    if(username == NULL)
    {
        perror("malloc");
        return NULL;
    }
    username = getenv("USER") ;
    return username ;
}

char * getHostname(char * hostname)
{
    hostname = (char *)malloc(MAX_SIZE) ;
    if(hostname == NULL)
    {
        perror("malloc");
        return NULL;
    }
    int rc = gethostname(hostname , MAX_SIZE) ;
    if (rc != 0)
    {
        perror("gethostname") ;
        return NULL ;
    }
    // for(int i = 0 ; i < strlen(hostname) ; i++)
    // {
    //     if(hostname[i] == '.')
    //     {
    //         hostname[i] = '\0' ;
    //         break ;
    //     }
    // }
    return hostname ;
}

char * getPWD( char * PWD)
{
    PWD = (char *)malloc(MAX_SIZE) ;
    if(PWD == NULL)
    {
        perror("malloc");
        return NULL;
    }
    getcwd(PWD, MAX_SIZE) ;
    return PWD ;
}

char * correctPath(char *home) {
    char *temp = getcwd(NULL, 0);
    if (temp == NULL) {
        // Handle error if getcwd fails
        return NULL;
    }

    int home_len = strlen(home);

    // Check if the current working directory starts with the home path
    if (strncmp(temp, home, home_len) == 0) {
        // Allocate memory for the new path
        char *temp2 = (char *)malloc(strlen(temp) - home_len + 2); // +2 for '~' and '\0'
        if (temp2 == NULL) {
            // Handle error if malloc fails
            free(temp);
            return NULL;
        }

        // Replace the home path with '~'
        temp2[0] = '~';
        strcpy(temp2 + 1, temp + home_len);

        free(temp);
        return temp2;
    }

    // If no replacement is needed, return the original path
    return temp;
}

