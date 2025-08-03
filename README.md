<!-- [![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Qiz9msrr) -->
<!-- # mini-project-1-template -->


# MP 1(Part - A):

* Assuming that all absolute paths are starting with the '/' where as relative don't.
* . => current_directory
* .. => parent_directory
* ~ => home_directory
* '-' => previous_working_directory as an absolute address
* Most of the error messages are in *_red_*.

### Spec 1:
* Assuming that the directory in which shell starts is the home directory.

### Spec 2:
* I am initially assuming the prev_pwd to home directory.
* If the entered command is erroneous i am printing "Error in executing the command or incorrect command".
* In this shell, i am giveing the precedence arder like ; then & i.e, i will be considering that ; differentiates two commands irrespective of what they are.
* If there is a input such as ==> a ; & then i am not considering the command a to be run in bg since i am splitting the line into two commands where i encounter an ';' .
* I am printing the absolute path after the use of correct command hop.
* I am assuming the maximum input length to be 4096.

### Spec 3:
```
    hop <path> <path> ...
```
* The given path can be relative of absolute paths.
* In hop, I am accounting for multiple parameters of path.
* I exexcute them in the given sequence, if there are any erroneous ones among them it prints the error msg for that and continues.

### Spec 4:
```
    reveal <flags> <path>
```
* I am only encoutering for one path_name not multiple (which i did for hop).
* I am assuming the maximum number of entries in a given directory won't be more than 1000.
* Assuming that no name of the file/folder are not of format starting with '-' and contains only "l" and "a".
* * Directories --> blue | executable --> green 

### Spec 5:
* To print the latest 15 commands (latest at the bottom):
```
    log
```
* To clear the history :
```
    log purge
```
* To execute the nth recent command (n < 15):
```
    log execute <n>
```
* i am storing the command executed by the log execute.

### Spec 6:
* if a Backgrounnd processes are executed then first it's pid is printed and later when exited it's pid printed again.
* I am assuming that the max number of background processes to be atmost 20.
* In foreground I am printing the process name and process time only if it's time_taken to execute is more than 2 sec only(for non-user commands only).
* I am only printing the pid if a background process is exited and also whether it exited normally or was terminated.
* I am printing it like ==> sleep: 5 ; sleep 3;
* Maximum number of bg_processes are 4096.
* In background if an erroneous command 

### Spec 7:
* To get the details of given pid (if no pid is given then the details of the current shell is given):
```
    proclore <pid>
```
* Assuming + for foreground.
* I am showing the virtual memory in bits.

### Spec 8:
* To find/seek the file in the given directory:
```
    seek <flags> <file/folder> <target_directory>
```
* I am only considering them if the file/directory name is exactly same as the search_name or have the prefix same as the search_name. 
* I am assuming that there are no files/directories with names such as -d, -e, -f.

### Spec 9:
* alias format in .myshrc:
    ``` alias <alias_name> = <actual_command>```
* I am assuming the max number of aliases to be 20.
* If the aliases are changed/modified while the shell is already running then the shell won't know the changes, to make it apply to the shell, you need to restart the shell(i.e., close the current shell and open a new shell).
* If the actual command is related to log, then it is not stored in the history(i.e., log.txt).

### Spec 10:
* The shell doesn't handle multi redirections
* Redirection operands include ">", ">>", "<".

### Spec 11:
* My piping doesn't handle user commands in piping in background.

### Spec 12:

### Spec 13:
* To get the process details:
```
    activities
```
* It mainly gives the details of the processes spawned by our shell only .
* Output format :
```
    [pid] - [Command_name] -State
```
* State is either running or stopped, so i considered that every state other than stopped state is running.

### Spec 14:
* To send a signal to a process:
```
    ping <pid> <signal(int)>
```
* Assuming number of signals would be 32 only.

### Spec 15:
* for making a process foreground:
```
    fg <pid>
```
* for making a procsee in backround from a stopped state to running in background itself:
```
    bg <pid>
```

### Spec 16:
* To get the latest creates pid :
```
    neonate -n <time_arg>
```
* Press 'x' key to exit this command.


### Spec 17:
* Command Format:
    ```
    iMan <command_name>
    ```
* iMan gets the man pages from the internet and displays it.
* iMan expects only one argument, if more are given except the first one everything else is ignored.

## Compilation of shell:
* To compile:
```
make
```
* To run:
```
./vksh
```
* To clean:

``` 
make clean
```

## Chat-gpt:(Resources)
* https://chatgpt.com/share/8582b9c3-04e2-423a-ad6b-dadc7a837f5b
* https://chatgpt.com/share/66e57c2f-6508-8006-9ab2-b74019d1113d

