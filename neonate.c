#include "headers/headers.h"

void print_latest_pid()
{
    char path[MAX_SIZE];
    path[0] = '\0';
    snprintf(path, MAX_SIZE, "/proc/loadavg");

    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("\033[031mError in opening the /proc/loadavg file\033[0m");
        return;
    }
    char line[MAX_SIZE];
    line[0] = '\0';
    for (int i = 0; i < 5; i++)
        fscanf(fptr, "%s", line);
    printf("%s\n\n", line);
    // There are 5 things in porc/loadavg and last thing is the latest pid.
    fclose(fptr);
    return ;
}

int keyboardhit()
{
    struct timeval tv = {0, 0}; // No timeout
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void neonate(char **commands)
{
    if (commands[0] == NULL || commands[1] == NULL || strcmp(commands[1], "-n") != 0)
    {
        printf("\033[031mError in the arguments for neonate.\033[0m\n");
        return;
    }
    int time_arg;
    if(commands[2] == NULL)
    {
        time_arg = 0;
    }else{
        time_arg = atoi(commands[2]);
    }
    if (time_arg < 0)
    {
        printf("\033[031mInvalid time argument.\033[0m\n");
        return;
    }

    time_t prev_time;
    time(&prev_time);
    int keep_running = 0;
    activate_rawmode();
    while (!keep_running)
    {
        time_t now;
        time(&now);
        if (now - prev_time >= time_arg)
        {
            print_latest_pid();
            fflush(stdout);
        }
        if (keyboardhit())
        {
            int ch;
            ch = getchar();
            if (ch != EOF || ch != -1)
            {
                char c = (char)ch;
                if (c == 'x')
                {
                    keep_running = 1;
                }
            }
        }
        prev_time = now;
    }
    deactivate_rawmode();
    return;
}
