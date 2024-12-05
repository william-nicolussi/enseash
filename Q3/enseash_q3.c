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

#define CTRL_D_PRESSED_MESSAGE "As you pressed ctrl + D, the program exits"

//----- PROTOTYPES -----
int stringsAreEquals(char* str1, char* str2);
void displayShell(char* strToWrite);
void execute_fork(char* strInputShell);

//----- MAIN -----
int main()
{
	int programHasToStop = 0;
    char inputUserBuffer[MAX_SIZE];

	displayShell(START_MESSAGE); 
    do{
		displayShell(PROMPT_MESSAGE);
        int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);
        
        // If read(...) above gets an error, stop the execution
        // If EOF was read, set the variable to exit and go to the end
        // Otherwise set inputUserBuffer
        if (bytesRead == -1)
        {
            perror(READ_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
        else if (bytesRead == 0)
        {
			programHasToStop = 1;
			displayShell(EXIT_STRING);
			displayShell("\n");
			continue;
		}
		else
		{
			inputUserBuffer[bytesRead - 1] = '\0';
		}
        
		if (stringsAreEquals(inputUserBuffer, EXIT_STRING))
		{
			programHasToStop = 1;
		}

		execute_fork(inputUserBuffer);
		
    } while (!programHasToStop);

	displayShell(EXIT_MESSAGE);
	
    return 0;
}

//----- OTHER FUNCTIONS -----

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
void displayShell(char* strToWrite)
{
	if(write(STDOUT_FILENO, strToWrite, strlen(strToWrite))==-1)
	{
		perror(DISPLAY_SHELL_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
	}
}

// Function to execute the users' input on the shell 
void execute_fork(char* strInputShell)
{
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
    } else 
    {
        // Parent process waits for his child
        wait(NULL);
    }
}
