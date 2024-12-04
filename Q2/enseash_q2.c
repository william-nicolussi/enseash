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

#define FORTUNE_MESSAGE "Today is what happened to yesterday.\n"
#define FORTUNE_STRING_INPUT "fortune"
#define DATE_STRING_INPUT "date"

// Function to check if the two stings are the same
int stringsAreEquals(char* str1, char* str2)
{	
    if(strncmp(str1, str2, strlen(str1))==0 && strncmp(str1, str2, strlen(str2))==0)
    {
		return 1;
	}
    return 0;
}

// Function to execute if user enters FORTUNE_STRING_INPUT
void execute_fortune()
{
    write(STDOUT_FILENO, FORTUNE_MESSAGE, strlen(FORTUNE_MESSAGE));
}

// Function to execute if user enters DATE_STRING_INPUT
void execute_date()
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process to execute system's date command
        char *args[] = {"date", NULL};
        execvp(args[0], args);
        perror("Command failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process waits for his child
        wait(NULL);
    }
}

int main()
{
    char inputUserBuffer[MAX_SIZE];

    write(STDOUT_FILENO, START_MESSAGE, strlen(START_MESSAGE));

    do{
        write(STDOUT_FILENO, PROMPT_MESSAGE, strlen(PROMPT_MESSAGE));
        int bytesRead = read(STDIN_FILENO, inputUserBuffer, MAX_SIZE - 1);
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
			write(STDOUT_FILENO, COMMAND_NOT_FOUND_MESSAGE, strlen(COMMAND_NOT_FOUND_MESSAGE));
		}

    } while (!stringsAreEquals(inputUserBuffer, EXIT_STRING));

	write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
	
    return 0;
}
