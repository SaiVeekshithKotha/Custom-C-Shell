#include "headers.h"

extern pid_t background_pids[MAX_SIZE];
extern int num_background_pids;

void add_background_pid(pid_t pid)
{
    if (num_background_pids < MAX_SIZE)
    {
        background_pids[num_background_pids++] = pid;
    }
}

void remove_background_pid(pid_t pid)
{
    for (int i = 0; i < num_background_pids; i++)
    {
        if (background_pids[i] == pid)
        {
            // Shift remaining elements left
            for (int j = i; j < num_background_pids - 1; j++)
            {
                background_pids[j] = background_pids[j + 1];
            }
            num_background_pids--;
            return;
        }
    }
}
void sigchild_handler(int signum)
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
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
            if (WIFEXITED(status))
            {
                printf("\033[036mBackground Process with PID %d exited normally.\033[0m\n", pid, WEXITSTATUS(status));
                remove_background_pid(pid);
            }
            else if (WIFSIGNALED(status))
            {
                printf("\033[036mBackground Process with PID %d terminated.\033[0m\n", pid, WTERMSIG(status));
                remove_background_pid(pid);
            }
        }
    }
}

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
        int fd = open("/dev/null", O_RDWR);
        if (fd < 0)
        {
            printf("\033[31mError opening /dev/null\033[0m\n");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);  // Redirect stdin to /dev/null
        dup2(fd, STDOUT_FILENO); // Redirect stdout to /dev/null
        dup2(fd, STDERR_FILENO); // Redirect stderr to /dev/null

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
        printf("Background Process with PID %d\n", rc);
        add_background_pid(rc);
    }
}


void sys_comm_fg(char **commands)
{
    int pid = fork();
    if (pid < 0)
    {
        printf("\033[31mError in forking\033[0m\n");
        return ;
    }
    else if (pid == 0)
    {   // Child Process
        if(strcmp(commands[0] , "echo") == 0)
        {
            int i = 1;
            while(commands[i] != NULL)
            {
                if(commands[i][0] == '\"')
                {
                    commands[i]++;
                }
                if(commands[i][strlen(commands[i]) - 1] == '\"')
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
    {   // Parent Process
        
        // Wait for foreground process to complete so that the control remains with the shell
        int stat;
        waitpid(pid, &stat, 0);

        if (WIFEXITED(stat) && WEXITSTATUS(stat) != 0)
        {
            return ; // Command execution failed; return NULL
        }

        if(strcmp(commands[0] , "sleep") == 0)
        {
            printf("Sleep for %s seconds\n", commands[1]);
        }
        return ;
    }
    return ;
}
