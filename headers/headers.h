#ifndef HEADERS_H
#define HEADERS_H

#define MAX_SIZE 4096
#define MAX_BG_PROCESSES 4096
#define BUFFER_SIZE 4096
#define MAX_ALIASES 20

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"

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

#include <termios.h>
#include <sys/select.h>

#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// Structs for storing background processes
typedef struct Match
{
    char path[MAX_SIZE];
    bool is_dir;
    struct Match *next;
} Match;

typedef struct Alias
{
    char *alias_name;
    char *command;
} Alias;

typedef struct pinfo
{
    char *name;
    int pid;
} pinfo;

extern pid_t background_pids[MAX_BG_PROCESSES];
extern int num_background_pids;
extern char names_of_bg[MAX_BG_PROCESSES][MAX_SIZE];
extern Alias Aliases[MAX_ALIASES];
extern int foreground_pid;
extern char foreground_command[MAX_SIZE] ;
extern int alias_count;
// extern int saved_stdout;
// extern int saved_stdin;

// hop
int changing_dir(char *command, char *prev_pwd, char *home);
// input
char *getInput_line(char *line);
char **splitInput_line(char *line);
char **get_backround_commands(char *commands, int *num_commands);
char **split_command(char *command);
void preprocess_input(char *line);
// name
char *getUsername(char *username);
char *getHostname(char *hostname);
char *getPWD(char *PWD);
char *correctPath(char *PWD);
// reveal
void handle_reveal(char **commands, char *prev_pwd, char *home);
void revealing(char *path_to_directory, bool l, bool a);
// trim_space
char *trim_space(char *str);
// log
void log_print(char *path_to_log);
void log_store(char *path_to_log, char *string);
void log_purge(char *path_to_log);
char *log_execute_com(char *path_to_log, int n);
// proclore
void proclore(char **commands);
// Seek
void handle_seek(char **commands, char *prev_PWD, char *home);
// Sys Comm fg and bg
void sys_comm_bg(char **commands);
void sys_comm_fg(char **commands);
void add_background_pid(pid_t pid, char **commands);
void add_background_pid2(int pid, char *commands);
void add_background_pid3(pid_t pid, char **commands);
void remove_background_pid(pid_t pid);
char *get_name_bg(pid_t pid);
// Execute Shell
void execute_shell(char *home, char *prev_PWD, char *username, char *line, char *path_to_log, char *fg_out);
// Alias
void alias_loading(char *home);
char *check_alias_command(char *commands);
// iMan
void getting_man_pages(char **commands);
// Redirection
int handle_redirection(char **command_segs);
// Activities
void activities();
// FG_BG
void fg(int pid);
void bg(int pid);
// Signals
void setup_signal_handler();
void sigchild_handler(int signum);
void kill_all_background_processes();
void handle_sigint(int sigum);
void handle_sigtstp(int sigum);
// Ping
void pingy(int pid, int signal);
// Pipe
int check_pipe(char *command) ;
void handle_pipe(char *home, char *prev_PWD, char *username, char *input, char *path_to_log, char *fg_out, bool bg_flag, bool *log);
char **split_pipe_commands(char *line);
void execute_pipe_shell_fg(char *home, char *prev_PWD, char *username, char **command_segs, char *path_to_log, char *fg_out , bool *log);
// void execute_pipe_shell_bg(char *home, char *prev_PWD, char *username, char **command_segs, char *path_to_log, char *fg_out , bool *log);
// Rawmode &b cooked Mode
void deactivate_rawmode();
void activate_rawmode();
// Neonate
void neonate(char **commaands);

#endif
