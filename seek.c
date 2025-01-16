#include "headers.h"

void add_match(Match **output, char *path, bool is_dir)
{
    Match *temp = (Match *)malloc(sizeof(Match));
    if (temp == NULL) {
        perror("malloc");
        return ;
    }
    strncpy(temp->path, path, MAX_SIZE);
    temp->path[MAX_SIZE - 1] = '\0';  // Ensure null termination
    temp->is_dir = is_dir;
    temp->next = *output;
    *output = temp;
}

void seek(char *target_directory, char *search_name, bool d, bool f, bool e , Match **matches)
{
    // printf("Seeking %s in %s\n", search_name, target_directory);
    // printf("Coming soon\n");

    DIR *dir = opendir(target_directory);
    if (dir == NULL)
    {
        printf("\033[31mError: Directory does not exist\033[0m\n");
        return;
    }
    struct dirent *entry;
    struct stat file_stat;
    while((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || (entry->d_name[0] == '.')) {
            continue;
        }

        char full_path[MAX_SIZE];
        snprintf(full_path, MAX_SIZE, "%s/%s", target_directory, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }
        if (S_ISDIR(file_stat.st_mode)) {
            if (d || (!d && !f)) {  // Directory matching
                if (strncmp(entry->d_name, search_name , strlen(search_name)) == 0) {
                    add_match(matches, full_path, true);
                }
            }
            // Recursive call to explore subdirectories
            // printf("Exploring %s\n", full_path);
            seek(full_path, search_name, d, f, e, matches);

        } else if (S_ISREG(file_stat.st_mode)) {
            if (f || (!d && !f)) {  // File matching
                // printf("Comparing %s with %s\n", entry->d_name, search_name);
                char filename_copy[MAX_SIZE];
                strncpy(filename_copy, entry->d_name, MAX_SIZE);
                filename_copy[MAX_SIZE - 1] = '\0';  // Ensure null termination

                char *filename_no_ext = strtok(filename_copy, ".");
                if (strncmp(entry->d_name, search_name , strlen(search_name)) == 0 ) {
                    add_match(matches, full_path, false);
                }
            }
        }
    }
    closedir(dir);
    return ;
}
void seeking(char *target_directory, char *search_name, bool d, bool f, bool e , char *prev_PWD, char *home)
{
    Match *matches = NULL;
    if(d && f) // If both are true
    {
        printf("\033[31mError: Cannot use -d and -f together\033[0m\n");
        return ;
    }
    char base_directory[MAX_SIZE];
    strcpy(base_directory , target_directory);
    seek(target_directory, search_name, d, f, e, &matches);
    int count = 0;
    Match *temp = matches;
    while(temp != NULL)
    {
        count += 1;
        temp = temp->next;
    }
    if(count == 0)
    {
        printf("\033[31mNo matches found\033[0m\n");
        return ;
    }
    if(e && count == 1)
    {
        if(matches->is_dir)
        {
            printf("\033[34m%s\033[0m\n", matches->path);
            char * temp = getcwd(NULL, 0);
            int rc = chdir(matches->path);
            strcpy(prev_PWD , temp);
            if(rc != 0)
            {
                printf("\033[31mMissing Permissions\033[0m\n");
            }

        }else{
            printf("\033[32m%s\033[0m\n", matches->path + strlen(base_directory));
            printf("\033[32mThis is %s\033[0m\n", search_name);
            FILE *file = fopen(matches->path, "r");
            if(file == NULL)
            {
                printf("\033[31mMissing Permissions for the task\033[0m\n");
                return ;
            }
            char *line = (char *)malloc(MAX_SIZE * sizeof(char));
            if(line == NULL)
            {
                perror("malloc");
                return ;
            }
            while(fgets(line, MAX_SIZE, file) != NULL)
            {
                printf("%s", line);
            }
            printf("\n");
            fclose(file);
            free(line);
            
        }
        return ;
    }
    else{
        Match *temp = matches;
        while(temp != NULL)
        {
            if(temp->is_dir)
            {
                printf("\033[34m.%s\033[0m\n", (temp->path) + strlen(base_directory));
            }else{
                printf("\033[32m.%s\033[0m\n", (temp->path) + strlen(base_directory));
            }
            temp = temp->next;
        }
    }
    return ;
}


void handle_seek(char **commands , char *prev_PWD, char *home)
{
    bool d = false;
    bool f = false;
    bool e = false;
    char target_directory[MAX_SIZE];
    target_directory[0] = '\0';
    char search_name[MAX_SIZE];
    search_name[0] = '\0';

    bool flag = false;
    for(int i = 1 ; commands[i] != NULL ; i++)
    {
        if(strcmp(commands[i] , "-d") == 0)
        {
            d = true;
        }
        else if(strcmp(commands[i] , "-f") == 0)
        {
            f = true;
        }
        else if(strcmp(commands[i] , "-e") == 0)
        {
            e = true;
        }
        else if(flag == true)
        {
            strcpy(target_directory , commands[i]);
            break ;
        }
        else if(flag == false)
        {
            strcpy(search_name , commands[i]);
            flag = true;
        }
    }
    if(d && f)
    {
        printf("\033[31mError: Cannot use -d and -f together\033[0m\n");
        return ;
    }
    char *curr_pwd = getcwd(NULL, 0);
    if(strlen(target_directory) == 0 || strcmp(target_directory , "") == 0)
    {
        strcpy(target_directory , curr_pwd);
    }
    else if(target_directory[0] == '~')
    {
        if(strlen(target_directory) == 1)
        {
            strcpy(target_directory , home);
        }
        else
        {
            char temp[MAX_SIZE];
            strcpy(temp , home);
            strcat(temp , target_directory + 1);
            strcpy(target_directory , temp);
        }
    }else if(target_directory[0] == '-')
    {
        if(strlen(target_directory) == 1)
        {
            strcpy(target_directory , prev_PWD);
        }
        else
        {
            char temp[MAX_SIZE];
            strcpy(temp , prev_PWD);
            strcat(temp , target_directory + 1);
            strcpy(target_directory , temp);
        }
    }
    else if(target_directory[0] == '.')
    {
        if(strlen(target_directory) == 1)
        {
            strcpy(target_directory , curr_pwd);
        }else if(target_directory[1] != '.')
        {
            char temp[MAX_SIZE];
            strcpy(temp , curr_pwd);
            strcat(temp , target_directory + 1);
            strcpy(target_directory , temp);
        }else if(target_directory[1] == '.')
        {
            char temp[MAX_SIZE];
            int i;
            for(i = strlen(curr_pwd) - 1 ; i >= 0 ; i--)
            {
                if(curr_pwd[i] == '/' && i != strlen(curr_pwd) - 1)
                {
                    strcpy(temp , curr_pwd);
                    temp[i] = '\0';
                    break;
                }
            }
            if(strlen(target_directory) > 2)
            {
                strcat(temp , target_directory + 2);
                strcpy(target_directory , temp);
            }else if(strlen(target_directory) == 2)
            {
                strcpy(target_directory , temp);
            }
        }
    }
    else if(target_directory[0] != '/'){
        char temp[MAX_SIZE];
        strcpy(temp , curr_pwd);
        strcat(temp , "/");
        strcat(temp , target_directory);
        strcpy(target_directory , temp);
    }
    // printf("Seeking %s in %s\n", search_name, target_directory);
    // printf("%d %d %d\n", d , f , e);
    if(f && d)
    {
        printf("\033[31mError: Cannot use -d and -f together\033[0m\n");
        return ;
    }
    seeking(target_directory , search_name , d , f , e , prev_PWD , home);

}
