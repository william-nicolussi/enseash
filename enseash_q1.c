#include <unistd.h>
#include <string.h>

#define PROMPT "Welcome to ENSEA Tiny Shell.\nTo leave, press 'exit'.\nenseash %\n"
#define EXIT_STRING "exit"

// Function to check if input matches "exit"
int is_exit_command(char *input)
{
    // Ensure exact match, considering newline removal
    return (strncmp(input, EXIT_STRING, strlen(EXIT_STRING)) == 0);
}

int main()
{
    char buffer[128]; // Buffer to store user input
    int bytesRead;
    int typedExit = 0; //variable that becomes 1 if user types EXIT_STRING

    do{
        // Display prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Read input
        //bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        bytesRead = read(0, buffer, sizeof(buffer) - 1);

        // Null-terminate the input string (remove trailing newline)
        buffer[bytesRead - 1] = '\0';

        // Check if the command is "exit"
        if (is_exit_command(buffer))
        {
            typedExit = 1;
        }
    } while (!typedExit);

    return 0;
}
