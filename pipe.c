#include "headers/headers.h"

int check_pipe(char *command)
{
    int len = strlen(command);
    for (int i = 0; i < len; i += 1)
    {
        if (command[i] == '|')
            return 1;
    }
    return 0;
}

void shift_command_segments2(char **command_segs, int start)
{
    int j;
    for (j = start; command_segs[j] != NULL; j++)
    {
        command_segs[j - 2] = command_segs[j];
    }
    command_segs[j - 2] = NULL;
}
// void execute_command_in_pipe(char **segments, int num_of_commands)
void execute_pipe_commands_in_pipe(char **segments, int num_of_commands, char *home, char *prev_PWD, char *username, char *path_to_log, char *fg_out, bool bg_flag, bool *log)
{
    int pipefd[2];
    int fd_in = 0;
    // printf("Hi");
    if (bg_flag)
    {
        pid_t pid2 = fork();
        if (pid2 == 0)
        {
            setpgid(0, 0);
            for (int i = 0; i < num_of_commands - 1; i += 1)
            {
                if (pipe(pipefd) == -1)
                {
                    perror("pipe failed");
                    exit(EXIT_FAILURE);
                }

                if (fd_in != 0)
                {
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                }
                if (i < num_of_commands - 1)
                {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                }
                char **command_segs = split_command(segments[i]);
                int fd_output = -1, fd_input = -1;
                char *input_file = NULL, *output_file = NULL;
                bool append = false;

                // Traverse through command_segs to find redirection operators
                for (int i = 0; command_segs[i] != NULL; i++)
                {
                    if (strcmp(command_segs[i], ">") == 0 || strcmp(command_segs[i], ">>") == 0)
                    {
                        if (command_segs[i + 1] == NULL)
                        {
                            printf("\033[031mError: No output file specified\033[0m\n");
                            exit(EXIT_FAILURE);
                        }
                        output_file = command_segs[i + 1];
                        append = (strcmp(command_segs[i], ">>") == 0);
                        shift_command_segments2(command_segs, i + 2);
                        i -= 1;
                    }
                    else if (strcmp(command_segs[i], "<") == 0)
                    {
                        if (command_segs[i + 1] == NULL)
                        {
                            printf("\033[031mError: No input file specified\033[0m\n");
                            exit(EXIT_FAILURE);
                        }
                        input_file = command_segs[i + 1];
                        shift_command_segments2(command_segs, i + 2);
                        i -= 1;
                    }
                }
                // Handle input redirection first
                if (input_file != NULL)
                {
                    fd_input = open(input_file, O_RDONLY);
                    if (fd_input < 0)
                    {
                        printf("\033[031mError: Unable to open input file: %s\033[0m\n", input_file);
                        return;
                    }
                    // Redirect stdin to the file
                    dup2(fd_input, STDIN_FILENO);
                    close(fd_input);
                }

                // Handle output redirection
                if (output_file != NULL)
                {
                    int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
                    fd_output = open(output_file, flags, 0644);
                    if (fd_output < 0)
                    {
                        printf("\033[031mError: Unable to open output file: %s\033[0m\n", output_file);
                        return;
                    }
                    // Redirect stdout to the file
                    dup2(fd_output, STDOUT_FILENO);
                    close(fd_output);
                }
                pid_t pid3 = fork();
                if(pid3 == 0)
                {
                    execvp(command_segs[0], command_segs);
                    printf("\033[31mError: Failed to execute command\033[0m\n");
                    exit(EXIT_FAILURE);
                }
                else if (pid3 > 0)
                {
                    wait(NULL);
                    if (i < num_of_commands - 1)
                    {
                        close(pipefd[1]);
                    }
                    fd_in = pipefd[0];
                }
                else
                {
                    printf("\033[31mError: Fork failed\033[0m\n");
                    return;
                }
            }
        }
        else if (pid2 > 0)
        {
            add_background_pid3(pid2, segments);
            return;
        }
        else
        {
            printf("\033[31mError: Fork failed\033[0m\n");
            return;
        }
    }
    for (int i = 0; i < num_of_commands; i++)
    {
        if (i < num_of_commands - 1)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }

        int pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        { // Child process
            if (fd_in != 0)
            {
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            if (i < num_of_commands - 1)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            // close(pipefd[0]); // Close unused read end in child

            char **command_segs = split_command(segments[i]);
            int fd_output = -1, fd_input = -1;
            char *input_file = NULL, *output_file = NULL;
            bool append = false;

            // Traverse through command_segs to find redirection operators
            for (int i = 0; command_segs[i] != NULL; i++)
            {
                if (strcmp(command_segs[i], ">") == 0 || strcmp(command_segs[i], ">>") == 0)
                {
                    if (command_segs[i + 1] == NULL)
                    {
                        printf("\033[031mError: No output file specified\033[0m\n");
                        exit(EXIT_FAILURE);
                    }
                    output_file = command_segs[i + 1];
                    append = (strcmp(command_segs[i], ">>") == 0);
                    shift_command_segments2(command_segs, i + 2);
                    i -= 1;
                }
                else if (strcmp(command_segs[i], "<") == 0)
                {
                    if (command_segs[i + 1] == NULL)
                    {
                        printf("\033[031mError: No input file specified\033[0m\n");
                        exit(EXIT_FAILURE);
                    }
                    input_file = command_segs[i + 1];
                    shift_command_segments2(command_segs, i + 2);
                    i -= 1;
                }
            }

            // Handle input redirection first
            if (input_file != NULL)
            {
                fd_input = open(input_file, O_RDONLY);
                if (fd_input < 0)
                {
                    printf("\033[031mError: Unable to open input file: %s\033[0m\n", input_file);
                    return;
                }
                // Redirect stdin to the file
                dup2(fd_input, STDIN_FILENO);
                close(fd_input);
            }

            // Handle output redirection
            if (output_file != NULL)
            {
                int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
                fd_output = open(output_file, flags, 0644);
                if (fd_output < 0)
                {
                    printf("\033[031mError: Unable to open output file: %s\033[0m\n", output_file);
                    return;
                }
                // Redirect stdout to the file
                dup2(fd_output, STDOUT_FILENO);
                close(fd_output);
            }
            execute_pipe_shell_fg(home, prev_PWD, username, command_segs, path_to_log, fg_out, log);
            // execvp(command_segs[0], command_segs);
            exit(EXIT_SUCCESS);
        }
        else
        {               // Parent process
            wait(NULL); // Wait for child to finish

            // close(pipefd[1]);  // Close write end in parent
            if (i < num_of_commands - 1)
            {
                close(pipefd[1]);
            }
            fd_in = pipefd[0]; // Move read end to input_fd for the next command
        }
    }

    if (fd_in != 0)
    {
        close(fd_in); // Close the last pipe's read-end
    }
}

char **split_pipe_commands(char *line)
{
    char **tokens = (char **)malloc(sizeof(char *) * (MAX_SIZE));
    if (tokens == NULL)
    {
        printf("\033[031mError in allocating memory\033[0m\n");
        return NULL;
    }
    char *token = strtok(line, "|");
    int i = 0;
    while (token != NULL)
    {
        tokens[i] = trim_space(token);
        if (strcmp(tokens[i], "") == 0)
        {
            printf("\033[31mInvalid use of pipe\033[0m\n");
            free(tokens);
            return NULL;
        }
        i++;
        token = strtok(NULL, "|");
    }
    tokens[i] = NULL;
    return tokens;
}

// void handle_pipe(char *input)
void handle_pipe(char *home, char *prev_PWD, char *username, char *input, char *path_to_log, char *fg_out, bool bg_flag, bool *log)
{
    if (input == NULL || strlen(input) == 0)
    {
        printf("\033[31mEmpty input provided\033[0m\n");
        return;
    }
    int num_pipes = 0;

    for (int i = 0; i < strlen(input); i += 1)
    {
        if (input[i] == '|')
            num_pipes += 1;
    }

    char **pipe_segments = split_pipe_commands(input); // Split commands by '|'
    if (pipe_segments == NULL)
    {
        return;
    }
    if (pipe_segments[0] == NULL)
    {
        printf("\033[31mInvalid use of pipe\033[0m\n");
        free(pipe_segments);
        return;
    }

    int num_comm = 0;
    while (pipe_segments[num_comm] != NULL)
    {
        num_comm++;
    }

    if ((num_pipes + 1) != num_comm)
    {
        printf("\033[31mInvalid use of pipe\033[0m\n");
        return;
    }
    // Check for invalid use of pipe
    for (int i = 0; i <= num_pipes; i++)
    {
        if (strcmp(pipe_segments[i], "") == 0 || pipe_segments[i] == NULL)
        {
            printf("\033[31mInvalid use of pipe\033[0m\n");
            return;
        }
        if(strncmp(pipe_segments[i], "log" , 3) == 0)
        {
            *log = true;
        }
        if (strncmp(pipe_segments[i], "hop", 3) == 0)
        {
            char *dummy = strdup(pipe_segments[i]);
            char **dummy_segs = split_command(dummy);
            int rc = 0;
            int length = 0;
            for (int i = 0; dummy_segs[i] != NULL; i++)
            {
                length++;
            }
            if (dummy_segs[1] == NULL)
            {
                rc = changing_dir(NULL, prev_PWD, home);
            }
            else
            {
                // the segs array has the command and the arguments as its elements making it easier to pass the arguments to the function changing_dir

                for (int k = 1; k < length; k++)
                {
                    rc = changing_dir(dummy_segs[k], prev_PWD, home);
                }
            }
        }
    }

    int backup_stdin = dup(STDIN_FILENO);
    int backup_stdout = dup(STDOUT_FILENO);
    // printf("Hello");
    execute_pipe_commands_in_pipe(pipe_segments, num_comm, home, prev_PWD, username, path_to_log, fg_out, bg_flag, log);
    dup2(backup_stdin, STDIN_FILENO);
    dup2(backup_stdout, STDOUT_FILENO);
    close(backup_stdin);
    close(backup_stdout);

    free(pipe_segments);
}