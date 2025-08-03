#include "headers/headers.h"
void sigchild_handler(int signum)
{
    int status;
    pid_t pid;

    // Reap all children that have terminated
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        // printf("Signal handler: Child with PID %d terminated\n", pid);

        // Check if the PID is in the list of background PIDs

        int is_background = 0;
        for (int i = 0; i < num_background_pids; i++)
        {
            if (background_pids[i] == pid)
            {
                is_background = 1;
                break;
            }
        }

        if (is_background)
        {
            char *name = malloc(sizeof(char) * MAX_SIZE);
            if (name == NULL)
            {
                printf("\033[031mError in allocating memory.\033[0m\n");
                return;
            }

            // Ensure get_name_bg returns valid string
            strcpy(name, get_name_bg(pid));

            if (strcmp(name, "") == 0)
            {
                printf("\033[31mError in retrieving command name\033[0m\n");
                free(name);
                return;
            }

            if (WIFEXITED(status))
            {
                printf("[%d] \033[036mBackground Process with PID %d exited normally.\033[0m", num_background_pids, pid);
                remove_background_pid(pid);
                printf("\t\033[034m%s\033[0m\n", name);
            }
            else if (WIFSIGNALED(status))
            {
                printf("[%d] \033[036mBackground Process with PID %d terminated.\033[0m\n", num_background_pids, pid);
                remove_background_pid(pid);
                printf("\t\033[034m%s\033[0m\n", name);
            }
            free(name);
        }
    }
}

void setup_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = sigchild_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    // The SA_NOCLDSTOP flag basically makes the kernel send a SIGCHLD signal only when a child terminates not when something else happens 

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        // perror("sigaction");
        printf("\033[31mError in setting up signal handler\033[0m\n");
        exit(EXIT_FAILURE);
    }
}

void kill_all_background_processes()
{
    for (int i = 0; i < num_background_pids; i++)
    {
        if (background_pids[i] > 0)
        {
            signal(SIGTSTP, handle_sigtstp);
            // Try to terminate the process gracefully
            printf("Killing background process with PID %d\n", background_pids[i]);
            kill(background_pids[i], SIGTERM); // Send SIGTERM to each background process

            // Optionally: Wait a short time to give them a chance to terminate
            usleep(100000); // 100ms

            // If the process didn't terminate, send SIGKILL to force termination
            if (kill(background_pids[i], 0) == 0) // Check if the process is still running
            {
                kill(background_pids[i], SIGKILL); // Forcefully kill the process
                printf("Forcefully killed background process with PID %d\n", background_pids[i]);
            }
        }
    }
    num_background_pids = 0; // Reset the background process count
}

void handle_sigint(int sigum)
{
    if (foreground_pid != -1)
    {
        kill(foreground_pid, SIGINT);
    }
    printf("\n");
    return;
}

void handle_sigtstp(int sigum)
{
    if (foreground_pid != -1)
    {
        // Send SIGTSTP to the foreground process

        kill(foreground_pid, SIGTSTP);

        setpgid(foreground_pid, 0);
        // Add process to the background list
        add_background_pid2(foreground_pid, foreground_command);
        // Print status
        printf("\nForeground process with PID %d has been stopped and moved to background.\n", foreground_pid);

        // Reset foreground_pid
        foreground_pid = -1;
    }
}