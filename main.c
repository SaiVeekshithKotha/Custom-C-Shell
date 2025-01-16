#include "headers.h"

 pid_t background_pids[MAX_SIZE];
 int num_background_pids = 0;

int main()
{
    char *username;
    char *hostname;
    char *PWD;
    char *home_temp = getenv("PWD");
    char *line;
    char **pieces;
    char **command_segs;
    char *prev_PWD = getenv("PWD");
    char *fg_out = (char*)malloc(MAX_SIZE);
    if(fg_out == NULL)
    {
       perror("malloc");
       return 1;
    }
    fg_out[0] = '\0';
    num_background_pids = 0;
    // The background processes are stored in the array background_pids

    // I am copying the home directory to a new variable home so that I can free the home_temp variable and i can recall the function getenv("PWD") to get the current directory without in any way affecting the home directory variable
    char *home = (char *)malloc(MAX_SIZE);
    if(home == NULL)
    {
       perror("malloc");
       return 1;
    }
    strcpy(home, home_temp);
    FILE *fptr1 = fopen("log.txt", "r");
    if(fptr1 == NULL)
    {
        fptr1 = fopen("log.txt", "w");
        fclose(fptr1);
    }else{
        fclose(fptr1);
    }
    char path_to_log[MAX_SIZE] ;
    strcpy(path_to_log , getenv("PWD"));
    strcat(path_to_log , "/log.txt");
    // free(home_temp) ;

    while (1)
    {
        username = getUsername(username);
        hostname = getHostname(hostname);
        // I am considering the home directory to be the directiory from where the shell is started i.e, the PWD
        PWD = correctPath(home);

        printf("<%s@%s:%s %s>", username, hostname, PWD, (fg_out == NULL) ? "" : fg_out);
        fg_out[0] = '\0';
        // Prints the details of the user and the current directory either relative to the home directory or the absolute path if it is outside the home directory

        line = getInput_line(line);
        // Takes the input from the user
        execute_shell(home, prev_PWD, username, line, path_to_log, fg_out);
        signal(SIGCHLD, sigchild_handler); // Signal handler for handling the background processes when they exit
        
    }
    free(home);
    free(fg_out);

    return EXIT_SUCCESS;
}
