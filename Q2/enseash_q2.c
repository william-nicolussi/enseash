#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h> // For debugging purposes only (if needed during testing)

#define START "Welcome to ENSEA Tiny Shell.\nTo leave, press 'exit'.\n"
#define PROMPT "enseash %"
#define EXIT_MESSAGE "Bye bye...\n"
#define EXIT_CODE "exit"
#define MAX_SIZE 128

int main() {
    char command[MAX_SIZE]; // Buffer to store user input
    int pid, status;        // For managing process IDs and status

    // Display the startup message
    write(STDOUT_FILENO, START, strlen(START));

    while (1) {
        // Display the prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Read user input
        ssize_t bytesRead = read(STDIN_FILENO, command, MAX_SIZE - 1);
        if (bytesRead <= 0) {
            write(STDOUT_FILENO, "\nError reading input.\n", 22);
            continue;
        }

        // Null-terminate the input string (remove newline)
        command[bytesRead - 1] = '\0';

        // Check if the user wants to exit
        if (strcmp(command, EXIT_CODE) == 0) {
            write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
            break; // Exit the shell loop
        }

        // Fork a new process to handle the command
        pid = fork();
        if (pid == -1) {
            write(STDOUT_FILENO, "Error: Unable to fork.\n", 23);
            continue; // Skip iteration on error
        }

        if (pid == 0) {
            // Child process: execute the command
            execlp(command, command, (char *)NULL);

            write(STDOUT_FILENO, "Error: Command not found.\n", 26);
            _exit(1); // Use _exit to terminate child process immediately
        } else {
            // Parent process: wait for the child to complete
            waitpid(pid, &status, 0);

            // Optionally, you can display the exit status of the command here
        }
    }

    return 0; // Exit successfully
}
