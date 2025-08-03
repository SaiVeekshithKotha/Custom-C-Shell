#include "headers/headers.h"

void fg(int pid)
{
    // Check if the PID is in the background process list
    int is_background = 0;
    int index = -1;

    for (int i = 0; i < num_background_pids; i++)
    {
        if (background_pids[i] == pid)
        {
            is_background = 1;
            index = i;
            break;
        }
    }

    if (!is_background)
    {
        printf("No such process found\n");
        return;
    }
    pid_t tgid = tcgetpgrp(STDIN_FILENO);

    if (tgid == -1)
    {
        // perror("tcgetpgrp");
        printf("\033[031mzerror: tcgetpgrp ");
        return ;
    }

    setpgid(pid, tgid);
    // Resume the stopped background process
    if (kill(pid, SIGCONT) == -1)
    {
        // perror("Failed to continue the process");
        printf("\033[031mFailed to continue the process\033[0m\n");
        return;
    }

    // Remove from background list and print status
    // First store this detail and update the foreground
    foreground_pid = pid ;
    int k= 0 ;
    for(k = 0 ; k < num_background_pids ; k +=1)
    {
        if(background_pids[k] == pid)
        {
            strcpy(foreground_command , names_of_bg[k]);
        }
    }
    remove_background_pid(pid);
    printf("Process [%d] brought to foreground\n", pid);

    // Wait for the process to finish
    int status;
    waitpid(pid, &status, WUNTRACED);
    foreground_pid = -1;
    // Handle the process status
    if (WIFEXITED(status))
    {
        printf("Process [%d] exited with status %d\n", pid, WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("Process [%d] terminated by signal %d\n", pid, WTERMSIG(status));
    }
    else if (WIFSTOPPED(status))
    {
        printf("Process [%d] stopped by signal %d\n", pid, WSTOPSIG(status));
    }
    printf("\033[034mCmd - %s", foreground_command);
    foreground_command[0] = '\0';
    printf("\033[0m\n");
}


void bg(int pid)
{
    // Check if the PID is in the background process list and is stopped
    int is_background = 0;
    // int is_stopped = 0;

    for (int i = 0; i < num_background_pids; i++)
    {
        if (background_pids[i] == pid)
        {
            is_background = 1;

            // Check if the process is stopped
            break;
        }
    }

    if (!is_background)
    {
        printf("No such process found\n");
        return;
    }

    // if (!is_stopped)
    // {
    //     printf("Process [%d] is not stopped\n", pid);
    //     return;
    // }

    // Resume the stopped background process
    if (kill(pid, SIGCONT) == -1)
    {
        printf("\033[031m");
        printf("Failed to continue the process");
        printf("\033[0m\n");
        return;
    }

    // Print status
    printf("Process [%d] resumed in the background\n", pid);
}
