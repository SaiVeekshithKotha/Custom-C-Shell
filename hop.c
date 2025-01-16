#include "headers.h"

int changing_dir(char *path, char *prev_pwd,  char *home) {

    char *new_path = (char *)malloc(1000);
    int rc = 0;
    if (!new_path) {
        perror("malloc");
        return -1;
    }
    // curr_pwd is to get current PWD
    char *curr_pwd = getcwd(NULL, 0);
    if (!curr_pwd) {
        perror("getcwd");
        free(new_path);
        return -1 ;
    }

    if (path == NULL || strcmp(path, "") == 0 ) {
        rc = chdir(home);
        
    }else if(path[0] == '~'){
        // If path is '~' (go to home directory) or go directory in the home directory
       if(strlen(path) == 1){
           rc = chdir(home);
       }else{
            char *temp = (char *)malloc(MAX_SIZE);
           if(temp == NULL)
            {
                perror("malloc");
                return -1;
            }
            if (!temp) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            strcpy(temp, home);
            strcat(temp, path + 1);
            rc = chdir(temp);
            free(temp);
        }
    }else if(path[0] == '.'){
        // If path is '.' (stay in the same directory)
        if(strlen(path) == 1) rc = chdir(curr_pwd);
        else{
            char * temp = (char*)malloc(MAX_SIZE);
            if(temp == NULL)
            {
                perror("malloc");
                return -1;
            }
            strcpy(temp , curr_pwd);
            strcat(temp , path + 1);
            rc = chdir(temp);
            free(temp);
        }
        if(path[1] == '.')
        {
            strcpy(new_path, curr_pwd);
            int len = strlen(new_path);
            for (int i = len - 1; i >= 0; i--) {
                if (new_path[i] == '/' && i != len - 1) {
                    new_path[i] = '\0';
                    break;
                }
            }
            if(strlen(new_path) == 0) {
                new_path[0] = '/';
                new_path[1] = '\0';
            }
            if(strlen(path) == 2)
            {
                rc = chdir(new_path);
            }else{
                strcat(new_path , path + 2);
                rc = chdir(new_path);
            }
        }  
    } else if (path[0] == '-') {
        // If path is '-' (go to previous directory)

        if(strlen(path) == 1) rc = chdir(prev_pwd);
        else{
            char *temp = (char*)malloc(MAX_SIZE);
            if(temp == NULL)
            {
                perror("malloc");
                return -1;
            }
            strcpy(temp , prev_pwd);
            strcat(temp , path + 1);
            rc = chdir(temp);
            free(temp);
        }
        
    } else {
        // Convert relative path to absolute path
        char * a = (char *)malloc(1000);
        if (!a) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        if(path[0] != '/'){ 
            strcpy(a, curr_pwd);
            strcat(a, "/");
            strcat(a, path);
        } else {
            strcpy(a, path);
        }
        free(a) ;
        rc = chdir(path);
       
    }

    strcpy(prev_pwd, curr_pwd);

    free(new_path);
    free(curr_pwd);
    return rc;
}
