#include "headers/headers.h"

void shift_command_segments(char **command_segs, int start) {
    int j;
    for ( j = start; command_segs[j] != NULL; j++) {
        command_segs[j - 2] = command_segs[j];
    }
    command_segs[j - 2] = NULL;
}

int handle_redirection(char **command_segs)
{
    int fd_out = -1, fd_in = -1;
    char *input_file = NULL, *output_file = NULL;
    bool append = false;

    // Traverse through command_segs to find redirection operators
    for (int i = 0; command_segs[i] != NULL; i++)
    {
        if (strcmp(command_segs[i], ">") == 0 || strcmp(command_segs[i], ">>") == 0)
        {
            if (command_segs[i + 1] == NULL) {
                printf("\033[031mError: No output file specified\033[0m\n");
                return 1;
            }
            output_file = command_segs[i + 1];
            append = (strcmp(command_segs[i], ">>") == 0);
            shift_command_segments(command_segs, i + 2);
            i -= 1;
        }
        else if (strcmp(command_segs[i], "<") == 0)
        {
            if (command_segs[i + 1] == NULL) {
                printf("\033[031mError: No input file specified\033[0m\n");
                return 1;
            }
            input_file = command_segs[i + 1];
            shift_command_segments(command_segs, i + 2);
            i -= 1;
        }
    }
    // if(input_file != NULL) printf("%s",input_file) ;
    // if(output_file != NULL) printf("%s", output_file) ;

    // Handle input redirection first
    if (input_file != NULL)
    {
        fd_in = open(input_file, O_RDONLY);
        if (fd_in < 0)
        {
            printf("\033[031mError: Unable to open input file: %s\033[0m\n", input_file);
            return 1;
        }
        // Redirect stdin to the file
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    // Handle output redirection
    if (output_file != NULL)
    {
        int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
        fd_out = open(output_file, flags, 0644);
        if (fd_out < 0)
        {
            printf("\033[031mError: Unable to open output file: %s\033[0m\n", output_file);
            return 1;
        }
        // Redirect stdout to the file
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }

    return 0;
}
