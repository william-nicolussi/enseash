#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/time.h>

#define EXIT_STRING "exit"
#define MAX_SIZE 128
#define START_MESSAGE "Welcome to ENSEA Tiny Shell.\nTo leave, type '" EXIT_STRING "'.\n"
#define PROMPT_MESSAGE "enseash % "
#define EXIT_MESSAGE "Bye bye...\n"
#define COMMAND_NOT_FOUND_MESSAGE "Command not found.\n"
#define READ_ERROR_MESSAGE "Error: Unable to read input.\n"
#define FORK_FAILED_MESSAGE "fork failed"

// ----- PROTOTYPES -----
int stringsAreEquals(char* str1, char* str2);
void displayString(char* strToWrite);
void executeCommand(char* input);
void readUserInput(char* inputUserBuffer);

// ----- MAIN -----
int main()
{
    char inputUserBuffer[MAX_SIZE];
    displayString(START_MESSAGE);

    do {
        displayString(PROMPT_MESSAGE);  // Display the prompt here only
        readUserInput(inputUserBuffer);
        executeCommand(inputUserBuffer);
    } while (stringsAreEquals(inputUserBuffer, EXIT_STRING) == 0);

    displayString(EXIT_MESSAGE);
    return 0;
}

// ----- FUNCTIONS -----

// Function to read user input from the shell
void readUserInput(char* inputUserBuffer)
{
    int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);
    if (bytesRead == 0) {
        // EOF detected (Ctrl+D)
        strcpy(inputUserBuffer, EXIT_STRING); // Simulate typing "exit"
    } else if (bytesRead == -1) {
        perror(READ_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    } else {
        inputUserBuffer[bytesRead - 1] = '\0'; // Remove newline
    }
}

// Function to check if the two strings are the same
int stringsAreEquals(char* str1, char* str2)
{
    if (strncmp(str1, str2, strlen(str1)) == 0) {
        return 1;
    }
    return 0;
}

// Function to display the message or print perror and exit
void displayString(char* strToWrite)
{
    if (write(STDOUT_FILENO, strToWrite, strlen(strToWrite)) == -1) {
        perror("Error: Unable to write.");
        exit(EXIT_FAILURE);
    }
}

// Function to execute the user's input on the shell
void executeCommand(char* input)
{
    if (stringsAreEquals(input, EXIT_STRING)) {
        return;
    }

    // Start measuring time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Parse the command using strtok
    char* args[MAX_SIZE / 2 + 1]; // Array to hold command and its arguments
    char* token = strtok(input, " ");
    int argCount = 0;

    while (token != NULL) {
        args[argCount++] = token;
        token = strtok(NULL, " ");
    }
    args[argCount] = NULL; // Null-terminate the arguments array

    pid_t pid = fork();
    if (pid == -1) {
        perror(FORK_FAILED_MESSAGE);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process to execute the command
        execvp(args[0], args);

        // If execvp fails, display error and exit child
        perror(COMMAND_NOT_FOUND_MESSAGE);
        exit(EXIT_FAILURE);
    } else {
        // Parent process waits for the child
        int status;
        waitpid(pid, &status, 0);

        // End measuring time
        gettimeofday(&end, NULL);
        double elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; // Seconds to milliseconds
        elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0;     // Microseconds to milliseconds

        // Prepare and display the status message (WITHOUT PROMPT_MESSAGE)
        char buffer[MAX_SIZE];
        if (WIFEXITED(status)) {
            snprintf(buffer, sizeof(buffer), "[exit:%d | %.3f ms]\n", WEXITSTATUS(status), elapsedTime);
        } else if (WIFSIGNALED(status)) {
            snprintf(buffer, sizeof(buffer), "[sign:%d | %.3f ms]\n", WTERMSIG(status), elapsedTime);
        } else {
            snprintf(buffer, sizeof(buffer), "[unknown | %.3f ms]\n", elapsedTime);
        }
        displayString(buffer);
    }
}