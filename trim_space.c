#include "headers/headers.h"

char *trim_space(char *str) {
    int start = 0;
    int end = strlen(str) - 1;

    // Trim leading spaces
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }

    // Trim trailing spaces
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    // Null-terminate the trimmed string
    str[end + 1] = '\0';

    // Return the trimmed string starting from the new "start" position
    return str + start;
}
