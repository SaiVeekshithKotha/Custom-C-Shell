#include "headers/headers.h"

void pingy(int pid, int signal)
{
    signal = signal % 32;

    // Attempt to send the signal
    if (kill(pid, signal) == -1)
    {
        if (errno == ESRCH)
        {
            // Process does not exist
            printf("No such process found\n");
        }
        else if (errno == EINVAL)
        {
            // Invalid signal number
            printf("Invalid signal number\n");
        }
        else
        {
            // Some other error
            printf("Error: %s\n", strerror(errno));
        }
        return ;
    }

    printf("Signal %d sent to process %d\n", signal, pid);
    return ;
}