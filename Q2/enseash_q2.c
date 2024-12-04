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
#define WRITE_ERROR_MESSAGE "\nError: Unable to write.\n"

#define FORTUNE_MESSAGE "Today is what happened to yesterday.\n"
#define FORTUNE_STRING_INPUT "fortune"
#define DATE_STRING_INPUT "date"
#define FORK_FAILED_MESSAGE "fork failed"
#define COMMAND_FAILED_MESSAGE "Command failed"
#define DATE_COMMAND_ON_SHELL "date"

//----- PROTOTYPES -----
int stringsAreEquals(char* str1, char* str2);
void writeOrExit(char* strToWrite, char* strErrorMessage);
void execute_fortune();
void execute_date();

//----- MAIN -----
int main()
{
    char inputUserBuffer[MAX_SIZE];

	writeOrExit(START_MESSAGE, WRITE_ERROR_MESSAGE);

    do{
		writeOrExit(PROMPT_MESSAGE, WRITE_ERROR_MESSAGE);
        int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);
        if (bytesRead <= 0)
        {
            perror(READ_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
        inputUserBuffer[bytesRead - 1] = '\0';
        
        // If string correspond to a command, execute the corresponding function
        // otherwise write the COMMAND_NOT_FOUND_MESSAGE
		if(stringsAreEquals(inputUserBuffer, FORTUNE_STRING_INPUT))
		{
			execute_fortune();
		}
		else if (stringsAreEquals(inputUserBuffer, DATE_STRING_INPUT))
		{
			execute_date();
		}
		else
		{
			writeOrExit(COMMAND_NOT_FOUND_MESSAGE, WRITE_ERROR_MESSAGE);
		}

    } while (!stringsAreEquals(inputUserBuffer, EXIT_STRING));

	writeOrExit(EXIT_MESSAGE, WRITE_ERROR_MESSAGE);
	
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
void writeOrExit(char* strToWrite, char* strErrorMessage)
{
	if(write(STDOUT_FILENO, strToWrite, strlen(strToWrite))==-1)
	{
		perror(strErrorMessage);
        exit(EXIT_FAILURE);
	}
}

// Function to execute if user enters FORTUNE_STRING_INPUT
void execute_fortune()
{
    if(write(STDOUT_FILENO, FORTUNE_MESSAGE, strlen(FORTUNE_MESSAGE))==-1)
    {
		perror(WRITE_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
	}
}

// Function to execute if user enters DATE_STRING_INPUT
void execute_date()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror(FORK_FAILED_MESSAGE);
        exit(EXIT_FAILURE);
    } else if (pid == 0)
    {
        // Child process to execute system's date command
        char *args[] = {DATE_COMMAND_ON_SHELL, NULL};
        execvp(args[0], args);
        perror(COMMAND_FAILED_MESSAGE);
        exit(EXIT_FAILURE);
    } else 
    {
        // Parent process waits for his child
        wait(NULL);
    }
}
