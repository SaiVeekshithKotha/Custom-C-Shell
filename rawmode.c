#include "headers/headers.h"

struct termios orig_termios;

void deactivate_rawmode()
{
    // Restore original attributes
    if (tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios) == -1)
    {
        // perror("tcsetattr");
        printf("Error restoring terminal attributes: %s\n", strerror(errno));
    }
}

void activate_rawmode()
{
    // Get current terminal attributes
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
        // perror("tcgetattr");
        printf("Error getting terminal attributes: %s\n", strerror(errno));
        return;
    }

    // Register a function to restore terminal settings when the program exits
    atexit(deactivate_rawmode);

    // Create a copy of the original attributes
    struct termios raw = orig_termios;

    // Modify the terminal attributes for raw mode
    raw.c_lflag &= ~(ICANON | ECHO);
    
    // Set the new attributes
    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1)
    {
        // perror("tcsetattr");
        printf("Error enabling raw mode: %s\n", strerror(errno));
        return ;
    }
}