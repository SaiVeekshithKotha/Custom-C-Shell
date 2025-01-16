#include "headers.h"

#define MAX_SIZE 1024

// Function to compare two strings for qsort
int compare(const void *a, const void *b) {
    struct dirent *entryA = *(struct dirent **)a;
    struct dirent *entryB = *(struct dirent **)b;
    return strcmp(entryA->d_name, entryB->d_name);
}

void revealing(char *path_to_directory, bool l, bool a) {
    // Open the directory
    DIR *dir = opendir(path_to_directory);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Collect entries
    struct dirent **entries = NULL;
    size_t count = 0;
    struct dirent *entry;
    long long total_blocks = 0; // Initialize total blocks

    while ((entry = readdir(dir)) != NULL) {
        if (!a && entry->d_name[0] == '.') {
            continue; // Skip hidden files if 'a' flag is not set
        }
        entries = realloc(entries, sizeof(struct dirent *) * (count + 1));
        if (entries == NULL) {
            perror("realloc");
            closedir(dir);
            return;
        }
        entries[count++] = entry;

        char file_path[MAX_SIZE];
        strcpy(file_path, path_to_directory);
        strcat(file_path, "/");
        strcat(file_path, entry->d_name);

        struct stat file_stat;
        if (stat(file_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        total_blocks += file_stat.st_blocks; // Add blocks to total
    }
    if(l)
    {
        printf("total %lld\n", total_blocks);
    }

    // Sort entries
    qsort(entries, count, sizeof(struct dirent *), compare);

    // Print entries
    for (size_t i = 0; i < count; i++) {
        char file_path[MAX_SIZE];
        strcpy(file_path, path_to_directory);
        strcat(file_path, "/");
        strcat(file_path, entries[i]->d_name);

        struct stat file_stat;
        if (stat(file_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        if (l) {
            // Detailed listing
            char str[1024];
            snprintf(str, sizeof(str), "%c%c%c%c%c%c%c%c%c%c %u %s %s %lld %s",
                S_ISDIR(file_stat.st_mode) ? 'd' : '-',
                file_stat.st_mode & S_IRUSR ? 'r' : '-',
                file_stat.st_mode & S_IWUSR ? 'w' : '-',
                file_stat.st_mode & S_IXUSR ? 'x' : '-',
                file_stat.st_mode & S_IRGRP ? 'r' : '-',
                file_stat.st_mode & S_IWGRP ? 'w' : '-',
                file_stat.st_mode & S_IXGRP ? 'x' : '-',
                file_stat.st_mode & S_IROTH ? 'r' : '-',
                file_stat.st_mode & S_IWOTH ? 'w' : '-',
                file_stat.st_mode & S_IXOTH ? 'x' : '-',
                file_stat.st_nlink,
                getpwuid(file_stat.st_uid)->pw_name,
                getgrgid(file_stat.st_gid)->gr_name,
                (long long)file_stat.st_size,
                entries[i]->d_name);

            // Colorized output
            if (S_ISDIR(file_stat.st_mode)) {
                printf("\033[34m%s\033[0m\n", str);
            } else if (file_stat.st_mode & S_IXUSR) {
                printf("\033[32m%s\033[0m\n", str);
            } else {
                printf("\033[37m%s\033[0m\n", str);
            }
        } else {
            // Basic listing
            if (S_ISDIR(file_stat.st_mode)) {
                printf("\033[34m%s\033[0m\n", entries[i]->d_name);
            } else if (file_stat.st_mode & S_IXUSR) {
                printf("\033[32m%s\033[0m\n", entries[i]->d_name);
            } else {
                printf("\033[37m%s\033[0m\n", entries[i]->d_name);
            }
        }
    }

    free(entries);
    closedir(dir);
}

// The rest of your code remains the same


void handle_reveal(char **commands, char *prev_pwd, char *home)
{
    bool l = false;
    bool a = false;
    char path[MAX_SIZE];
    path[0] = '\0';
    int i = 1;
    while (commands[i] != NULL)
    {

        if (commands[i][0] != '-')
        {
            // Checking for path and terminating the loop
            // path = commands[i];
            strcpy(path, commands[i]);
            break;
            // only taking in one parameter as path
        }
        else if (strcmp(commands[i], "-l") == 0)
        {
            // Checking for single flag '-l'
            l = true;
        }
        else if (strcmp(commands[i], "-a") == 0)
        {
            // Checking for single flag '-a'
            a = true;
        }
        else if ((commands[i][0] == '-') && strlen(path) == 0 && strlen(commands[i]) == 1)
        {
            // Checking for single flag '-
            strcpy(path, prev_pwd);
            break;
            // only taking in one parameter as path
        }
        else if ((commands[i][0] == '-') && strlen(path) == 0 && strlen(commands[i]) > 1)
        {
            // Checking for multiple flags and path
            int length = strlen(commands[i]);
            int j = 1;
            bool flag = false;

            int countA = 0;
            int countL = 0;
            // printf("Length : %d\n" , length);
            // printf("Commands[i] : %s\n" , commands[i]);
            for (j = 1; j < length; j++)
            {
                if (commands[i][j] == 'a')
                {
                    countA++;
                }
                else if (commands[i][j] == 'l')
                {
                    countL++;
                }
                else
                {
                    flag = true;
                    break;
                }
                // printf("%c\n" , commands[i][j]);
            }
            if (flag == false && countA + countL == length - 1)
            {
                if (countA > 0)
                {
                    a = true;
                }
                if (countL > 0)
                {
                    l = true;
                }
            }
            else
            {
                strcpy(path , commands[i]);
            }
        }
        i++;
    }

    char *curr_pwd = getcwd(NULL, 0);
    if (curr_pwd == NULL)
    {
        perror("getcwd");
        return;
    }

    if (strlen(path) == 0 || strcmp(path, "") == 0)
    {
        // If path is not given
        strcpy(path, home);
    }
    else if (path[0] == '~')
    {
        // If path is '~'
        if (strlen(path) == 1)
            strcpy(path, home);
        else
        {
            char *temp = (char*)malloc(MAX_SIZE);
            if(temp == NULL)
            {
                perror("malloc");
                return ;
            }
            strcpy(temp, home);
            strcat(temp, path + 1);
            strcpy(path, temp);
            free(temp);
        }
    }
    else if (path[0] == path[1] && path[0] == '.')
    {
        // If path is '..'
        char *temp = (char *)malloc(MAX_SIZE);
        if(temp == NULL)
        {
            perror("malloc");
            return ;
        }

        strcpy(temp, curr_pwd);
        int len = strlen(temp);
        for (int i = len - 1; i >= 0; i--)
        {
            if (temp[i] == '/' && i != len - 1)
            {
                temp[i] = '\0';
                break;
            }
        }
        if (strlen(path) == 2)
            strcpy(path, temp);
        else
        {
            strcat(temp, path + 2);
            strcpy(path, temp);
        }
        free(temp);
    }
    else if (path[0] == '.')
    {
        // If path is '.' (stay in the same directory)
        if (strlen(path) == 1)
            strcpy(path, curr_pwd);
        else
        {
            char *temp = (char *)malloc(MAX_SIZE);
            if(temp == NULL)
            {
                perror("malloc");
                return ;
            }
            strcpy(temp, curr_pwd);
            strcat(temp, path + 1);
            strcpy(path, temp);
            free(temp);
        }
    }
    else if (path[0] == '-') // Check this part again
    {
        // If path is '-'
        if (strlen(path) == 1)
            strcpy(path, prev_pwd);
        else
        {
            char *temp = (char *)malloc(MAX_SIZE);
            if(temp == NULL)
            {
                perror("malloc");
                return ;
            }
            strcpy(temp, prev_pwd);
            strcat(temp, path + 1);
            strcpy(path, temp);
            free(temp);
        }
    }
    else if (path[0] != '/')
    {
        // Convert relative path to absolute path
        char *temp = (char *)malloc(MAX_SIZE);
        if(temp == NULL)
        {
            perror("malloc");
            return ;
        }
        strcpy(temp, curr_pwd);
        strcat(temp, "/");
        strcat(temp, path);
        strcpy(path, temp);
        free(temp);
    }

    // Reveal the directory
    printf("Revealing the directory\n");
    printf("Path : %s\n", path);

    revealing(path, l, a);
    return;
}
