#include "headers/headers.h"

int compare1(const void *a, const void *b)
{
    pinfo *P1 = (struct pinfo *)a;
    pinfo *P2 = (struct pinfo *)b;
    return strcmp(P1->name, P2->name);
}

void activities()
{
    int k = num_background_pids;

    // Check if there are any background processes
    if (k <= 0)
    {
        printf("No background processes found.\n");
        return;
    }

    // Allocate memory for process info structures
    pinfo **proc = (pinfo **)malloc(sizeof(pinfo *) * k);
    if (proc == NULL)
    {
        printf("\033[031mError in allocating memory\033[0m\n");
        return;
    }

    for (int i = 0; i < k; i += 1)
    {
        proc[i] = (pinfo *)malloc(sizeof(pinfo));
        if (proc[i] == NULL)
        {
            printf("\033[031mError in allocating memory\033[0m\n");
            // Free already allocated memory
            for (int j = 0; j < i; j++)
                free(proc[j]);
            free(proc);
            return;
        }

        proc[i]->name = names_of_bg[i];  // No need to strdup or allocate memory
        proc[i]->pid = background_pids[i];
    }

    // Sort the process info by name
    qsort(proc, k, sizeof(pinfo *), compare1);

    // Allocate memory for the file path
    char *path = (char *)malloc(sizeof(char) * MAX_SIZE);
    if (path == NULL)
    {
        printf("\033[031mError in allocating memory\033[0m\n");
        for (int i = 0; i < k; i++)
            free(proc[i]);
        free(proc);
        return;
    }

    // Loop through each process and get its status
    for (int i = 0; i < k; i += 1)
    {
        snprintf(path, MAX_SIZE, "/proc/%d/status", proc[i]->pid);
        FILE *fptr = fopen(path, "r");
        if (fptr == NULL)
        {
            printf("\033[031mError in retrieving process details for PID: %d\033[0m\n", proc[i]->pid);
            continue;  // Continue to the next process if this one fails
        }

        char *line = (char *)malloc(sizeof(char) * MAX_SIZE);
        if (line == NULL)
        {
            printf("\033[031mError in allocating memory for line buffer\033[0m\n");
            fclose(fptr);
            continue;  // Handle memory error, but continue processing other processes
        }

        char *status = (char *)malloc(sizeof(char) * MAX_SIZE);
        if (status == NULL)
        {
            printf("\033[031mError in allocating memory for status\033[0m\n");
            free(line);
            fclose(fptr);
            continue;
        }

        while (fgets(line, MAX_SIZE, fptr) != NULL)
        {
            // Scan the process state and check if it's 'T' (stopped)
            if (sscanf(line, "State: %19s", status) == 1)
            {
                if (strcmp(status, "T") == 0)
                {
                    printf("%d : %s - Stopped\n", proc[i]->pid, proc[i]->name);
                }
                else
                {
                    printf("%d : %s - Running\n", proc[i]->pid, proc[i]->name);
                }
                break;  // Break out once we find the state
            }
        }

        // Free the allocated memory for this loop
        free(line);
        free(status);
        fclose(fptr);
    }
    printf(":Hello");

    // Free all allocated memory
    free(path);
    for (int i = 0; i < k; i++)
    {
        free(proc[i]);  // Free the process struct, but not proc[i]->name
    }
    free(proc);

    return;
}
