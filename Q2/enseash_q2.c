#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define PROMPT "Welcome to ENSEA Tiny Shell.\nTo leave, press 'exit'.\nenseash %\n"
#define EXIT_STRING "exit"
#define MAX_HISTORY 10 // Maximum number of commands to store in history

// Function to check if input matches "exit"
int is_exit_command(char *input) {
    return (strncmp(input, EXIT_STRING, strlen(EXIT_STRING)) == 0);
}

// Function to display command history
void display_history(char history[][128], int count) {
    for (int i = 0; i < count; i++) {
        write(STDOUT_FILENO, history[i], strlen(history[i]));
        write(STDOUT_FILENO, "\n", 1);
    }
}

int main() {
    char buffer[128];               // Input buffer for commands
    char history[MAX_HISTORY][128]; // Array to store command history
    int historyCount = 0;           // Number of commands stored in history
    int bytesRead;
    int typedExit = 0;

    do {
        // Display the shell prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Read user input
        bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            _exit(1); // Exit on read error
        }

        // Null-terminate the input to handle it as a proper string
        buffer[bytesRead - 1] = '\0';

        // Check if the user entered "exit"
        if (is_exit_command(buffer)) {
            typedExit = 1;
        } else if (strcmp(buffer, "history") == 0) {
            // Handle the "history" command to display past commands
            display_history(history, historyCount);
        } else {
            // Add the command to history
            if (historyCount < MAX_HISTORY) {
                strncpy(history[historyCount], buffer, sizeof(buffer));
                historyCount++;
            } else {
                // If history is full, shift entries to make room for the new one
                for (int i = 1; i < MAX_HISTORY; i++) {
                    strncpy(history[i - 1], history[i], sizeof(history[i]));
                }
                strncpy(history[MAX_HISTORY - 1], buffer, sizeof(buffer));
            }

            // Measure the time taken for command execution
            struct timeval start, end;
            pid_t pid;

            // Get the start time
            gettimeofday(&start, NULL);

            pid = fork(); // Create a child process
            if (pid == -1) {
                // Fork failed
                write(STDOUT_FILENO, "Fork failed\n", 12);
                continue;
            }

            if (pid == 0) {
                // Child process: execute the command
                execlp(buffer, buffer, NULL);

                // If exec fails, print an error message and exit
                write(STDOUT_FILENO, "Unknown command\n", 16);
                _exit(1); // Exit child process on error
            } else {
                // Parent process: wait for the child to finish
                wait(NULL);

                // Get the end time
                gettimeofday(&end, NULL);

                // Calculate and display the elapsed time
                long elapsedTime = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                char elapsedMsg[64];
                snprintf(elapsedMsg, sizeof(elapsedMsg), "Execution time: %ld ms\n", elapsedTime);
                write(STDOUT_FILENO, elapsedMsg, strlen(elapsedMsg));
            }
        }
    } while (!typedExit);

    return 0;
}
