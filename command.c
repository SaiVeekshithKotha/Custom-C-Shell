#include "headers/headers.h"

void execute_shell(char *home, char *prev_PWD, char *username, char *line, char *path_to_log, char *fg_out)
{
    char another_line[MAX_SIZE];
    snprintf(another_line, MAX_SIZE, "%s", line);

    preprocess_input(line); // Adding spaces before and after the redirection operands for simplicity.
    // printf("%s",line);
    char **pieces = splitInput_line(line);
    // Splits the input line into pieces separated by ';'

    bool log = false;
    for (int i = 0; pieces[i] != NULL; i++)
    {
        if (strcmp(pieces[i], "") == 0)
            continue;

        char **bg_fg = (char **)malloc(sizeof(char *) * MAX_SIZE);
        if (bg_fg == NULL)
        {
            printf("\033[31mError: Memory allocation failed\033[0m\n");
            return;
        }
        int number_of_bg_fg = 0;
        bg_fg = get_backround_commands(pieces[i], &number_of_bg_fg);

        // Splits the pieces into background and foreground commands
        for (int j = 0; bg_fg[j] != NULL; j += 1)
        {
            bool bg_flag = (j < number_of_bg_fg - 1);

            if(check_pipe(bg_fg[j]) == 1)
            {
                // handle_pipe(bg_fg[j]);
                // printf("%s \n", bg_fg[j]);
                // printf("%d", bg_flag);

                handle_pipe(home, prev_PWD, username, bg_fg[j], path_to_log, fg_out, bg_flag, &log);
                continue;
            }

            char **command_segs = split_command(bg_fg[j]); // Splits the command into command and arguments and NULL at the end.

            // Handles the redirection operators
            // if(check)
            int saved_stdin = dup(STDIN_FILENO);
            int saved_stdout = dup(STDOUT_FILENO);

            int a = handle_redirection(command_segs);
            if (a == 1 || command_segs[0] == NULL)
            {
                // Restore file descriptors if redirection fails
                dup2(saved_stdout, STDOUT_FILENO);
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                continue; // Skip this command
            }


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
                    dup2(saved_stdout, STDOUT_FILENO); // Restore original stdout
                    dup2(saved_stdin, STDIN_FILENO);   // Restore original stdin
                    close(saved_stdout);
                    close(saved_stdin);
                    continue;
                }
                else
                {
                    command_segs = split_command(alias_equivalent_command);
                }
            }

            if (bg_flag == true)
            {
                // BackGround Process
                sys_comm_bg(command_segs);
            }
            else
            {
                // Foreground Process
                time_t start, end;
                time(&start);
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
                else if(strcmp(command_segs[0] , "neonate") == 0)
                {
                    neonate(command_segs) ;
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
                    log = true;
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
                            strcpy(line, log_execute_com(path_to_log, n));
                            execute_shell(home, prev_PWD, username, line, path_to_log, fg_out);
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
                // ##########################  COMMAND EXIT##########################
                else if (strcmp(command_segs[0], "exit") == 0)
                {
                    // Free command_segs
                    for (int k = 0; k < MAX_SIZE && command_segs[k] != NULL; k++)
                    {
                        free(command_segs[k]);
                    }
                    free(command_segs);
                    // Free pieces
                    free(pieces);
                    free(bg_fg);
                    free(home);
                    free(fg_out);
                    if (log == false)
                    {
                        log_store(path_to_log, another_line);
                    }
                    printf("\033[32mExiting Shell\033[0m\n");
                    time(&end);
                    kill_all_background_processes();
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    sys_comm_fg(command_segs);
                }
                time(&end);
                double time_taken = difftime(end, start);
                if (time_taken > 2)
                {
                    char *output = (char *)malloc(MAX_SIZE * sizeof(char));
                    snprintf(output, MAX_SIZE, "%s : %d", command_segs[0], (int)time_taken);
                    if (fg_out[0] == '\0')
                    {
                        strcpy(fg_out, output);
                    }
                    else
                    {
                        strcat(fg_out, ";");
                        strcat(fg_out, output);
                    }
                    free(output);
                }
            }
            // redirection termination.
            dup2(saved_stdout, STDOUT_FILENO); // Restore original stdout
            dup2(saved_stdin, STDIN_FILENO);   // Restore original stdin
            close(saved_stdin);
            close(saved_stdout);
            for (int k = 0; k < MAX_SIZE && command_segs[k] != NULL; k++)
            {
                free(command_segs[k]);
            }
            free(command_segs);
        }
        free(bg_fg);
    }
    free(pieces);
    if (log == false)
    {
        log_store(path_to_log, another_line);
    }
    // free(line);
    return;
}
