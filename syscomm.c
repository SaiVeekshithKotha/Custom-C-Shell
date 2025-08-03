#include "headers/headers.h"
void add_background_pid(pid_t pid, char **commands)
{
    if (num_background_pids < MAX_BG_PROCESSES)
    {
        background_pids[num_background_pids] = pid;
        names_of_bg[num_background_pids][0] = '\0'; // Initialize name to an empty string

        int i = 0;
        while (commands[i] != NULL)
        {
            if (i == 0)
            {
                strcpy(names_of_bg[num_background_pids], commands[i]);
            }
            else
            {
                strcat(names_of_bg[num_background_pids], " ");
                strcat(names_of_bg[num_background_pids], commands[i]);
            }
            i++;
        }
        num_background_pids++; // Increment after successful addition
    }
    else
    {
        printf("\033[031mToo many background processes.\033[0m\n");
    }
}

void add_background_pid3(pid_t pid, char **commands)
{
    if (num_background_pids < MAX_BG_PROCESSES)
    {
        background_pids[num_background_pids] = pid;
        names_of_bg[num_background_pids][0] = '\0'; // Initialize name to an empty string

        int i = 0;
        while (commands[i] != NULL)
        {
            if (i == 0)
            {
                strcpy(names_of_bg[num_background_pids], commands[i]);
            }
            else
            {
                strcat(names_of_bg[num_background_pids], " | ");
                strcat(names_of_bg[num_background_pids], commands[i]);
            }
            i++;
        }
        num_background_pids++; // Increment after successful addition
    }
    else
    {
        printf("\033[031mToo many background processes.\033[0m\n");
    }
}

void add_background_pid2(int pid, char *commands)
{
    if (num_background_pids < MAX_BG_PROCESSES)
    {
        background_pids[num_background_pids] = pid;
        names_of_bg[num_background_pids][0] = '\0'; // Initialize name to an empty string

        strcpy(names_of_bg[num_background_pids], commands);
        num_background_pids += 1;
    }
}

void remove_background_pid(pid_t pid)
{
    int i = 0;
    for (i = 0; i < num_background_pids; i++)
    {
        if (background_pids[i] == pid)
        {
            // Shift remaining elements left
            for (int j = i; j < num_background_pids - 1; j++)
            {
                background_pids[j] = background_pids[j + 1];
            }
            for (int j = i; j < num_background_pids - 1; j += 1)
            {
                strcpy(names_of_bg[j], names_of_bg[j + 1]);
            }
            background_pids[num_background_pids] = -1;
            strcpy(names_of_bg[num_background_pids--], "\0");
        }
    }
}
char *get_name_bg(pid_t pid)
{
    for (int i = 0; i < num_background_pids; i += 1)
    {
        if (background_pids[i] == pid)
        {
            return names_of_bg[i];
        }
    }
    return "";
}

// Signal handler to suspend interactive background processes
// void handle_interactive_signal(int sig) {
//     if (sig == SIGTTIN || sig == SIGTTOU) {
//         // Suspend the process with SIGTSTP if it tries to access terminal
//         printf("\033[31mProcess %d tried to access terminal, suspending...\033[0m\n", getpid());
//         kill(getpid(), SIGTSTP);
//     }
// }

void sys_comm_bg(char **commands)
{

    int rc = fork();
    if (rc < 0)
    {
        printf("\033[31mError in forking\033[0m\n");
        return;
    }
    else if (rc == 0)
    {
        // Child Process
        
        
        setpgid(0, 0); // Set the child process into a new process group

        if (strcmp(commands[0], "echo") == 0)
        {
            int i = 1;
            while (commands[i] != NULL)
            {
                if (commands[i][0] == '\"')
                {
                    commands[i]++;
                }
                if (commands[i][strlen(commands[i]) - 1] == '\"')
                {
                    commands[i][strlen(commands[i]) - 1] = '\0';
                }
                i += 1;
            }
        }

        int r = execvp(commands[0], commands);
        if (r < 0)
        {
            printf("\033[31mError in executing the command: %s\033[0m\n", commands[0]);
            exit(EXIT_FAILURE); // Exit with failure status if command fails
        }
    }
    else
    {
        // Parent Process

        // If there was any redirection , restore it.
        dup2(1, STDOUT_FILENO);
        dup2(0, STDIN_FILENO);
        printf("\033[033mBackground Process with PID %d\033[0m\n", rc);
        add_background_pid(rc, commands);
        
    }
}

void sys_comm_fg(char **commands)
{
    int pid = fork();
    if (pid < 0)
    {
        printf("\033[31mError in forking\033[0m\n");
        return;
    }
    else if (pid == 0)
    { // Child Process
        if (strcmp(commands[0], "echo") == 0)
        {
            int i = 1;
            while (commands[i] != NULL)
            {
                if (commands[i][0] == '\"')
                {
                    commands[i]++;
                }
                if (commands[i][strlen(commands[i]) - 1] == '\"')
                {
                    commands[i][strlen(commands[i]) - 1] = '\0';
                }
                i += 1;
            }
        }

        int rc = execvp(commands[0], commands);
        if (rc < 0)
        {
            printf("\033[31mError in executing the command or incorrect command\033[0m\n");
            _exit(EXIT_FAILURE); // If the command is incorrect, exit the child process
        }
    }
    else
    { // Parent Process
        foreground_pid = pid;
        int i = 0;
        while (commands[i] != NULL)
        {
            if (i == 0)
            {
                strcpy(foreground_command, commands[i]);
            }
            else
            {
                strcat(foreground_command, " ");
                strcat(foreground_command, commands[i]);
            }
            i++;
        }
        // Wait for foreground process to complete so that the control remains with the shell
        int stat;

        waitpid(pid, &stat, WUNTRACED);

        foreground_pid = -1;
        if (WIFEXITED(stat) && WEXITSTATUS(stat) != 0)
        {
            return; // Command execution failed; return NULL
        }
        if(WIFSTOPPED(stat))
        {
            printf("%d -pid %s stopped and sent to background\n",pid , foreground_command);
        }
        if (strcmp(commands[0], "sleep") == 0)
        {
            printf("Sleep for %s seconds\n", commands[1]);
        }
        foreground_command[0] = '\0' ;
    }
    return;
}
