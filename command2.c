#include "headers/headers.h"

void execute_pipe_shell_fg(char *home, char *prev_PWD, char *username, char **command_segs, char *path_to_log, char *fg_out , bool *log)
{
    int command_count = 0;
    while (command_segs[command_count] != 0)
    {
        command_count += 1;
    }

    // Check if it's a alias command
    char *alias_equivalent_command = check_alias_command(command_segs[0]);
    if (alias_equivalent_command != NULL)
    {
        if (command_count > 1)
        {
            printf("\033[031mError in the command, too many arguments.\033[0m\n");
            // redirection termination.
            // continue;
            return;
        }
        else
        {
            command_segs = split_command(alias_equivalent_command);
        }
    }

    // Foreground Process
    // ########################### FG #############################
    if (strcmp(command_segs[0], "fg") == 0)
    {
        if (command_count > 2)
        {
            printf("\033[031mToo many args.\033[0m\n");
        }
        else
        {
            fg(atoi(command_segs[1]));
        }
    }
    // ######################### BG ###############################
    else if (strcmp(command_segs[0], "bg") == 0)
    {
        if (command_count > 2)
        {
            printf("\033[031mToo many args.\033[0m\n");
        }
        else
        {
            bg(atoi(command_segs[1]));
        }
    }
    // ########################## Neonate ############################
    else if (strcmp(command_segs[0], "neonate") == 0)
    {
        neonate(command_segs);
    }
    // ########################## PING ################################
    else if (strcmp(command_segs[0], "ping") == 0)
    {
        if (command_count != 3)
        {
            printf("\033[031mIncorrect number of args.\033[0m\n");
        }
        else
        {
            pingy(atoi(command_segs[1]), atoi(command_segs[2]));
        }
    }
    // ########################## ACTIVITIES ###########################
    else if (strcmp(command_segs[0], "activities") == 0)
    {
        activities();
    }
    // ########################## HOP COMMAND ##########################
    else if (strcmp(command_segs[0], "hop") == 0)
    {
        int rc = 0;
        int length = 0;
        for (int i = 0; command_segs[i] != NULL; i++)
        {
            length++;
        }
        if (command_segs[1] == NULL)
        {
            rc = changing_dir(NULL, prev_PWD, home);
            if (rc != 0)
            {
                printf("\033[31mError in changing directory\033[0m\n");
            }
            else
            {
                printf("\033[32mDirectory changed successfully to %s\033[0m\n", getcwd(NULL, 0));
            }
        }
        else
        {
            // the segs array has the command and the arguments as its elements making it easier to pass the arguments to the function changing_dir

            for (int k = 1; k < length; k++)
            {
                rc = changing_dir(command_segs[k], prev_PWD, home);
                if (rc != 0)
                {
                    printf("\033[31mError in changing directory\033[0m\n");
                }
                if (rc == 0)
                {
                    printf("\033[32mDirectory changed successfully to %s\033[0m\n", getcwd(NULL, 0));
                }
                // break;
            }
        }
    }
    // ##########################  COMMAND REVEAL##########################
    else if (strcmp(command_segs[0], "reveal") == 0)
    {
        handle_reveal(command_segs, prev_PWD, home);
    }
    // ##########################   PROCLORE   ##########################
    else if (strcmp(command_segs[0], "proclore") == 0)
    {
        proclore(command_segs);
    }
    // ##########################   LOG   ##########################
    else if (strcmp(command_segs[0], "log") == 0)
    {
        (*log) = true;
        if (command_segs[1] == NULL)
        {
            log_print(path_to_log);
        }
        else if (strcmp(command_segs[1], "purge") == 0)
        {
            log_purge(path_to_log);
        }
        else if (strcmp(command_segs[1], "execute") == 0)
        {
            int n = atoi(command_segs[2]);

            if (n == 0 || n > 15 || n < 0)
            {
                printf("\033[31mError: Invalid command number\033[0m\n");
            }
            else if (log_execute_com(path_to_log, n) == NULL)
            {
            }
            else
            {
                char *line = (char *)malloc(MAX_SIZE * sizeof(char));
                strcpy(line, log_execute_com(path_to_log, n));
                execute_shell(home, prev_PWD, username, line, path_to_log, fg_out);
                free(line);
                // I am also storing the rerunning command in the log file
            }
        }
    }
    // ########################## SEEK #################################
    else if (strcmp(command_segs[0], "seek") == 0)
    {
        handle_seek(command_segs, prev_PWD, home);
    }
    // ######################## iMan ####################################
    else if (strcmp(command_segs[0], "iMan") == 0)
    {
        getting_man_pages(command_segs);
    }

    else
    {
        // 
        int pid = fork();
        if (pid == 0)
        {
            execvp(command_segs[0], command_segs);
            printf("\033[31mError in executing command\033[0m\n");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, WUNTRACED);
        }
        else
        {
            printf("\033[31mError in forking\033[0m\n");
            return;
        }
    }

    return;
}

// void execute_pipe_shell_bg(char *home, char *prev_PWD, char *username, char **command_segs, char *path_to_log, char *fg_out , bool *log){
//     int pid = fork();

// }
