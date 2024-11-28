#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h> // For debugging purposes only (if needed during testing)

#define EXIT_STRING "exit"
#define MAX_SIZE 128
#define START_MESSAGE "Welcome to ENSEA Tiny Shell.\nTo leave, type '" EXIT_STRING "'.\n"
#define PROMPT_MESSAGE "enseash % "
#define EXIT_MESSAGE "Bye bye...\n"
#define COMMAND_NOT_FOUND_MESSAGE "Command not found.\n"

// Function to check if input matches EXIT_STRING
int is_EXIT_STRING(char *input)
{	
    if(strncmp(input, EXIT_STRING, strlen(EXIT_STRING)) == 0)
    {
		return 1;
	}
    return 0;
}

int main()
{
    char inputUserBuffer[MAX_SIZE];
    int pid, status;        // For managing process IDs and status

    // Display the startup message
    write(STDOUT_FILENO, START_MESSAGE, strlen(START_MESSAGE));

    do{
        // Display the prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Read user input
        int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);

        // Null-terminate the input string (remove newline)
        inputUserBuffer[bytesRead - 1] = '\0';

        // Fork a new process to handle the command
        pid = fork();
        if (pid == -1) {
            write(STDOUT_FILENO, "Error: Unable to fork.\n", 23);
            continue; // Skip iteration on error
        }

        if (pid == 0) {
            // Child process: execute the command
            execlp(inputUserBuffer, inputUserBuffer, (char *)NULL);

            write(STDOUT_FILENO, "Error: Command not found.\n", 26);
            _exit(1); // Use _exit to terminate child process immediately
        } else {
            // Parent process: wait for the child to complete
            waitpid(pid, &status, 0);
        }
    } while (!is_EXIT_STRING(inputUserBuffer));

	write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
	
    return 0;
}
