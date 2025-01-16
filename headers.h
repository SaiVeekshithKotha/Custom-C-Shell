#ifndef HEADERS_H
#define HEADERS_H

#define MAX_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

// extern pid_t background_pids[MAX_SIZE];
// extern int num_background_pids;

// Structs
typedef struct Match{
    char path[MAX_SIZE];
    bool is_dir;
    struct Match *next;
} Match;


// hop 
int changing_dir(char * command , char * prev_pwd ,  char * home) ;
// input
char * getInput_line(char * line) ;
char ** splitInput_line(char *line) ;
char **get_backround_commands(char *commands , int *num_commands) ;
char **split_command(char *command) ;
// name
char * getUsername( char * username) ;
char * getHostname(char * hostname) ;
char * getPWD( char * PWD) ;
char * correctPath( char * PWD) ;
//reveal
void handle_reveal(char **commands , char *prev_pwd , char *home);
void revealing(char *path_to_directory ,bool l , bool a );
//trim_space
char * trim_space(char * str) ;
//log
void log_print(char *path_to_log) ;
void log_store(char *path_to_log, char * string);
void log_purge(char *path_to_log) ;
char* log_execute_com(char *path_to_log,int n);
// proclore
void proclore(char **commands) ;
//Seek
void handle_seek(char **commands, char *prev_PWD, char *home);
// Sys Comm fg and bg
void sys_comm_bg(char **commands);
void sys_comm_fg(char **commands);
void sigchild_handler(int signum);
// Execute Shell
void execute_shell(char *home, char *prev_PWD, char *username, char *line, char *path_to_log , char *fg_out);


#endif
