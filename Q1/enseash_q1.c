#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define EXIT_STRING "exit"
#define MAX_SIZE 128
#define START_MESSAGE "Welcome to ENSEA Tiny Shell.\nTo leave, type '" EXIT_STRING "'.\n"
#define PROMPT_MESSAGE "enseash % "
#define EXIT_MESSAGE "Bye bye...\n"
#define READ_ERROR_MESSAGE "\nError: Unable to read input.\n"
#define DISPLAY_SHELL_ERROR_MESSAGE "\nError: Unable to write.\n"

// Function to check if input matches EXIT_STRING
int is_EXIT_STRING(char *input)
{	
    if(strncmp(input, EXIT_STRING, strlen(input)) == 0)
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

int main()
{
    char inputUserBuffer[MAX_SIZE];
    int bytesRead;

	// Display the start message only at the beginning
	displayString(START_MESSAGE);
	
    do{
        // Display prompt and handle errors		
		displayString(PROMPT_MESSAGE);

        // Read input and handle errors or EOF
        bytesRead = read(0, inputUserBuffer, sizeof(inputUserBuffer));
        if (bytesRead <= 0)
        {
            perror(READ_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }

        // Null-terminate the input string (remove trailing newline)
        inputUserBuffer[bytesRead - 1] = '\0';

    } while (!is_EXIT_STRING(inputUserBuffer));

	displayString(EXIT_MESSAGE);
	
    exit(EXIT_SUCCESS);
}
