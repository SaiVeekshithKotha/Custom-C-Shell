#include "headers.h"

void proclore(char **commands)
{
    pid_t pid;
    char path_pid[MAX_SIZE];
    if (commands[1] == NULL)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(commands[1]);
    }
    printf("PID: %d\n", pid);

    // Check if the process exists
    if (kill(pid, 0) < 0)
    {
        if (errno == ESRCH)
        {
            printf("\033[031mNo such process\033[0m\n");
        }
        else if (errno == EPERM)
        {
            printf("\033[031mOperation not permitted\033[0m\n");
        }
        return;
    }

    /* For Linux */
    char path_mem[MAX_SIZE];
    sprintf(path_mem, "/proc/%d/status", pid);
    char input[MAX_SIZE];
    sprintf(input , "%d" , pid);
    // char process_status[MAX_SIZE];
    char process_memory[MAX_SIZE];

    FILE *fptr1 = fopen(path_mem, "r");
    if (fptr1 == NULL)
    {
        printf("\033[031mError in opening the file\033[0m\n");
        return;
    }
    while(fgets(input,MAX_SIZE,fptr1)!=NULL){
        // if(!strncmp(input,"State",5)){
        //     strcpy(process_status,input);
        // }
        if(!strncmp(input,"VmSize:",7)){
            strcpy(process_memory,input);
        }
    }
    fclose(fptr1);
    // printf("Virtual Memory: %s",process_memory);

    sprintf(path_pid, "/proc/%d/stat", pid);
    FILE *fptr = fopen(path_pid, "r");
    if (fptr == NULL)
    {
        printf("\033[031mError in opening the file\033[0m\n");
        return;
    }

    int ppid,pgid,sid,tty_nr,tpgid;
    char comm;
    char state;
    fscanf(fptr,"%d %s %c %d %d %d %d %d",&pid,&comm,&state,&ppid,&pgid,&sid,&tty_nr,&tpgid);
    if(pgid == tpgid){
        printf("state: %c+\n",state);
    }else{
        printf("state: %c\n",state);
    }
    printf("Process Group ID %d\n", pgid);

    fclose(fptr);
    printf("Virtual Memory: %d\n", process_memory);

    char executable_path[MAX_SIZE];
    sprintf(path_pid, "/proc/%d/exe", pid);
    readlink(path_pid, executable_path, MAX_SIZE);
    printf("Executable Path: %s\n", executable_path);


}








