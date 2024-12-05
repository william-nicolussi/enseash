#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define EXIT_STRING "exit"
#define MAX_SIZE 128
#define START_MESSAGE "Welcome to ENSEA Tiny Shell.\nTo leave, type '" EXIT_STRING "'.\n"
#define PROMPT_MESSAGE "enseash % "
#define EXIT_MESSAGE "Bye bye...\n"
#define COMMAND_NOT_FOUND_MESSAGE "Command not found.\n"
#define READ_ERROR_MESSAGE "\nError: Unable to read input.\n"
#define DISPLAY_SHELL_ERROR_MESSAGE "\nError: Unable to write.\n"
#define FORK_FAILED_MESSAGE "fork failed"
#define COMMAND_FAILED_MESSAGE "Command failed"

// ----- PROTOTYPES -----
int stringsAreEquals(char* str1, char* str2);
void displayString(char* strToWrite);
void execute_sub_command(char* strInputShell);
void readUserInput(char* inputUserBuffer);

// ----- MAIN -----
int main()
{
    char inputUserBuffer[MAX_SIZE];
    displayString(START_MESSAGE);

    do {
		displayString(PROMPT_MESSAGE);
        readUserInput(inputUserBuffer);
        execute_sub_command(inputUserBuffer);
    } while (stringsAreEquals(inputUserBuffer, EXIT_STRING) == 0);

    displayString(EXIT_MESSAGE);
    return 0;
}

// ----- FUNCTIONS -----
void readUserInput(char* inputUserBuffer)
{
    int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);
    
    // If EOF detected (Ctrl+D), then inoutUserBuffer becomes EXIT_STRING
    // If read(...) failed, handle error
    // Otherwise store what user typed
    if (bytesRead == 0)
    {
        strcpy(inputUserBuffer, EXIT_STRING);
        displayString(EXIT_STRING);
        displayString("\n");
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
    if (strncmp(str1, str2, strlen(str1)) == 0) {
        return 1;
    }
    return 0;
}

// Display the message or print perror and exit
void displayString(char* strToWrite)
{
    if (write(STDOUT_FILENO, strToWrite, strlen(strToWrite)) == -1)
    {
        perror(DISPLAY_SHELL_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }
}

// Function to execute the user's input on the shell
void execute_sub_command(char* strInputShell)
{
    // If user typed EXIT_STRING, then do nothing
    if (stringsAreEquals(strInputShell, EXIT_STRING))
    {
        return;
    }

	// Otherwise create a child process to execute the command
    pid_t pid = fork();
    if (pid == -1)
    {
        perror(FORK_FAILED_MESSAGE);
        exit(EXIT_FAILURE);
        
	}
	else if (pid == 0)
	{
        // Child process to execute the user's command
        char *args[] = {strInputShell, NULL};
        execvp(args[0], args);

        // If execvp fails, display COMMAND_NOT_FOUND_MESSAGE and exit child
        perror(COMMAND_NOT_FOUND_MESSAGE);
        exit(EXIT_FAILURE);
    }
    else
    {	
		// Parent process waits for the child
        int status;
        waitpid(pid, &status, 0);

        // If child terminated with an error, notify the user
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            displayString(COMMAND_NOT_FOUND_MESSAGE);
        }
    }
}
