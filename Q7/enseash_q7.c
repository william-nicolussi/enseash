#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>

#define EXIT_STRING "exit"
#define MAX_SIZE 128
#define START_MESSAGE "Welcome to ENSEA Tiny Shell.\nTo leave, type '" EXIT_STRING "'.\n"
#define PROMPT_MESSAGE "enseash % "
#define EXIT_MESSAGE "Bye bye...\n"
#define COMMAND_NOT_FOUND_MESSAGE "Command not found.\n"
#define READ_ERROR_MESSAGE "Error: Unable to read input.\n"
#define OPEN_INPUT_FILE_ERROR_MESSAGE "Error opening input file"
#define OPEN_OUTPUT_FILE_ERROR_MESSAGE "Error opening output file"
#define DISPLAY_SHELL_ERROR_MESSAGE "\nError: Unable to write.\n"
#define FORK_FAILED_MESSAGE "fork failed"
#define STRING_SHOW_EXIT_SUB_FUNCTION "[exit:%d | %.3f ms] %s"
#define STRING_SHOW_SIGN_SUB_FUNCTION "[sign:%d | %.3f ms] %s"
#define STRING_SHOW_UNKNOWN_SUB_FUNCTION "[unknown | %.3f ms] %s"
#define FILE_PERMISSIONS 0644 /* rw-r--r--: Owner (read/write), Group (read-only), Others (read-only) */

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
        displayString(PROMPT_MESSAGE);
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
	// If EOF detected (Ctrl+D), then inputUserBuffer becomes EXIT_STRING
    // If read(...) failed, handle error
    // Otherwise store what user typed
    int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);
    if (bytesRead == 0)
    {
        strcpy(inputUserBuffer, EXIT_STRING);
    }
    else if (bytesRead == -1)
    {
        perror(READ_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }
    else
    {
        inputUserBuffer[bytesRead - 1] = '\0'; // change '\n' with '\0'
    }
}

// Function to check if the two strings are the same
int stringsAreEquals(char* str1, char* str2)
{
    if (strncmp(str1, str2, strlen(str1)) == 0)
    {
        return 1;
    }
    return 0;
}

// Function to display the message or print perror and exit
void displayString(char* strToWrite)
{
    if (write(STDOUT_FILENO, strToWrite, strlen(strToWrite)) == -1)
    {
        perror(DISPLAY_SHELL_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }
}

// Function to execute the user's input on the shell
void executeCommand(char* input)
{
    if (stringsAreEquals(input, EXIT_STRING))
    {
        return;
    }

    // Start measuring time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Parse the command using strtok
    char* args[MAX_SIZE / 2 + 1]; // Array to hold command and its arguments
    char* token = strtok(input, " ");
    int argCount = 0;
    char* inputFile = NULL;
    char* outputFile = NULL;
    int redirectInput = 0, redirectOutput = 0;
    while (token != NULL)
    {
        if (strcmp(token, "<") == 0)
        {
            redirectInput = 1;
            token = strtok(NULL, " ");
            inputFile = token;
        }
        else if (strcmp(token, ">") == 0)
        {
            redirectOutput = 1;
            token = strtok(NULL, " ");
            outputFile = token;
        }
        else
        {
            args[argCount++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[argCount] = NULL;

    pid_t pid = fork();
    if (pid == -1)
    {
        perror(FORK_FAILED_MESSAGE);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process to execute the command
        // Restore default behavior for SIGINT in the child process
        signal(SIGINT, SIG_DFL);
        if (redirectInput && inputFile)
        {
            int fdInput = open(inputFile, O_RDONLY);
            if (fdInput == -1)
            {
                perror(OPEN_INPUT_FILE_ERROR_MESSAGE);
                exit(EXIT_FAILURE);
            }
            dup2(fdInput, STDIN_FILENO);
            close(fdInput);
        }

        if (redirectOutput && outputFile)
        {
            int fdOutput = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSIONS);
            if (fdOutput == -1)
            {
                perror(OPEN_OUTPUT_FILE_ERROR_MESSAGE);
                exit(EXIT_FAILURE);
            }
            dup2(fdOutput, STDOUT_FILENO);
            close(fdOutput);
        }

        // If execvp fails, display error and exit child
        execvp(args[0], args);
        perror(COMMAND_NOT_FOUND_MESSAGE);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process waits for the child
        int status;
        waitpid(pid, &status, 0);

        // End measuring time
        gettimeofday(&end, NULL);
        double elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; // Seconds to milliseconds
        elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0;     // Microseconds to milliseconds

        // Prepare and display the prompt with exit status and time
        char buffer[MAX_SIZE];
        if (WIFEXITED(status))
        {
            snprintf(buffer, sizeof(buffer), STRING_SHOW_EXIT_SUB_FUNCTION, WEXITSTATUS(status), elapsedTime, PROMPT_MESSAGE);
        }
        else if (WIFSIGNALED(status))
        {
            snprintf(buffer, sizeof(buffer), STRING_SHOW_SIGN_SUB_FUNCTION, WTERMSIG(status), elapsedTime, PROMPT_MESSAGE);
        }
        else
        {
            snprintf(buffer, sizeof(buffer), STRING_SHOW_UNKNOWN_SUB_FUNCTION, elapsedTime, PROMPT_MESSAGE);
        }
        displayString(buffer);
    }
}
