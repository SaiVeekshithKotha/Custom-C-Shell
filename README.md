[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Qiz9msrr)
# mini-project-1-template


# MP 1(Part - A):

* Assuming that all absolute paths are starting with the '/' where as relative don't.
* . => current_directory
* .. => parent_directory
* ~ => home_directory
* '-' => previous_working_directory
* Red ==> error messages.

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
* In hop, i accounting multiple parameters of path.
* I exexcute them in the given sequence, if there are any erroneous ones among them it prints the error msg for that and continues.

### Spec 4:
* I am only encoutering for one path_name not multiple (which i did for hop).
* I am assuming the maximum number of entries in a given directory won't be more than 1000.

### Spec 5:
* i am not storing the command executed by the log execute

### Spec 6:
* if a Backgrounnd processes are executed then first it's pid is printed and later when exited it's pid printed again.
* In foreground I am printing the process name and process time only if it's time_taken to execute is more than 2 sec only(for non-user commands only).
* In background, i am redirecting the stdin, stdout, stderr to '/dev/null' so that even if the bg_process is a interactive type it will not take the control of the shell.
* I am only printing the pid if a background process is exited and also whether it exited normally or was terminated.

### Spec 7:
* Assuming + for foreground.
* I am showing the virtual memory in bits.

### Spec 8:
* I am only considering them if the file/directory name is exactly same as the search_name or have the prefix same as the search_name. 

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

##### Chat-gpt:(Resources)
* https://chatgpt.com/share/8582b9c3-04e2-423a-ad6b-dadc7a837f5b

