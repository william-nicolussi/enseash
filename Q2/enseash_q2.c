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

//----- PROTOTYPES -----
int stringsAreEquals(char* str1, char* str2);
void displayString(char* strToWrite);
void execute_sub_command(char* strInputShell);
void readUserInput(char* inputUserBuffer);

//----- MAIN -----
int main()
{
    char inputUserBuffer[MAX_SIZE];

	displayString(START_MESSAGE);

    do{
		displayString(PROMPT_MESSAGE);
		
        readUserInput(inputUserBuffer);
        
		execute_sub_command(inputUserBuffer);

    } while (stringsAreEquals(inputUserBuffer, EXIT_STRING) == 0);

	displayString(EXIT_MESSAGE);
		
    return 0;
}

void readUserInput(char* inputUserBuffer)
{
	int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);
	if (bytesRead <= 0)
	{
		perror(READ_ERROR_MESSAGE);
		exit(EXIT_FAILURE);
	}
	inputUserBuffer[bytesRead - 1] = '\0';
}

// Function to check if the two stings are the same
int stringsAreEquals(char* str1, char* str2)
{	
    if(strncmp(str1, str2, strlen(str1))==0)
    {
		return 1;
	}
    return 0;
}

// Display the message or print perror and exit
void displayString(char* strToWrite)
{
	if(write(STDOUT_FILENO, strToWrite, strlen(strToWrite))==-1)
	{
		perror(DISPLAY_SHELL_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
	}
}

// Function to execute the user's input on the shell 
void execute_sub_command(char* strInputShell)
{
    // If user typed EXIT_STRING, then do nothing
    if (stringsAreEquals(strInputShell, EXIT_STRING)) {
        return;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror(FORK_FAILED_MESSAGE);
        exit(EXIT_FAILURE);
    } else if (pid == 0)
    {
        // Child process to execute system's date command
        char *args[] = {strInputShell, NULL};
        execvp(args[0], args); 
        perror(COMMAND_FAILED_MESSAGE);
        exit(EXIT_FAILURE);
            } else {
        // Parent process waits for the child
        wait(NULL);
    }
}
