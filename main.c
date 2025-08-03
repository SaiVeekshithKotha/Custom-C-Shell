#include "headers/headers.h"

pid_t background_pids[MAX_BG_PROCESSES];
int num_background_pids = 0;
char names_of_bg[MAX_BG_PROCESSES][MAX_SIZE];
Alias Aliases[MAX_ALIASES];
int alias_count = 0;
// int saved_stdout = 1; // Save original stdout
// int saved_stdin = 0;   // Save original stdin
int foreground_pid = -1; // For storing the foreground pid
char foreground_command[MAX_SIZE];


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
    char *fg_out = (char *)malloc(MAX_SIZE);
    if (fg_out == NULL)
    {
        perror("malloc");
        return 1;
    }
    fg_out[0] = '\0';
    num_background_pids = 0;
    foreground_command[0] = '\0' ;
    // The background processes are stored in the array background_pids

    // I am copying the home directory to a new variable home so that I can free the home_temp variable and i can recall the function getenv("PWD") to get the current directory without in any way affecting the home directory variable
    char *home = (char *)malloc(MAX_SIZE);
    if (home == NULL)
    {
        perror("malloc");
        return 1;
    }
    strcpy(home, home_temp);
    FILE *fptr1 = fopen("log.txt", "r");
    if (fptr1 == NULL)
    {
        fptr1 = fopen("log.txt", "w");
        fclose(fptr1);
    }
    else
    {
        fclose(fptr1);
    }
    char path_to_log[MAX_SIZE];
    strcpy(path_to_log, getenv("PWD"));
    strcat(path_to_log, "/log.txt");
    // free(home_temp) ;

    // Loading the Aliases
    alias_loading(home);

    // Setuping signal handlers
    setup_signal_handler();
    signal(SIGINT , handle_sigint) ;
    signal(SIGTSTP, handle_sigtstp);
    // signal(SIGCHLD, sigchild_handler); // Signal handler for handling the background processes when they exit

    while (1)
    {
        username = getUsername(username);
        hostname = getHostname(hostname);
        // I am considering the home directory to be the directiory from where the shell is started i.e, the PWD
        PWD = correctPath(home);

        printf("<\033[036m%s\033[034m@%s:\033[035m%s \033[032m%s\033[0m>", username, hostname, PWD, (fg_out == NULL) ? "" : fg_out);
        fg_out[0] = '\0';
        // Prints the details of the user and the current directory either relative to the home directory or the absolute path if it is outside the home directory

        line = getInput_line(line);
        // Takes the input from the user
        execute_shell(home, prev_PWD, username, line, path_to_log, fg_out);
        free(line);
    }
    free(home);
    free(fg_out);

    return EXIT_SUCCESS;
}
