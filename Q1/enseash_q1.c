#include <unistd.h>
#include <string.h>

#define EXIT_STRING "exit"
#define MAX_SIZE 128
#define START_MESSAGE "Welcome to ENSEA Tiny Shell.\nTo leave, type '" EXIT_STRING "'.\n"
#define PROMPT_MESSAGE "enseash % "
#define EXIT_MESSAGE "Bye bye...\n"

// Function to check if input matches EXIT_STRING
int is_EXIT_STRING(char *input)
{	
    if(strncmp(input, EXIT_STRING, strlen(EXIT_STRING)) == 0)
    {
		return 1;
	}
    return 0;
}

int main()
{
    char inputUserBuffer[MAX_SIZE];
    int bytesRead;

	// Display the start message only at the beginning
	write(STDOUT_FILENO, START_MESSAGE, strlen(START_MESSAGE));
	
    do{
        // Display prompt
        write(STDOUT_FILENO, PROMPT_MESSAGE, strlen(PROMPT_MESSAGE));

        // Read input
        bytesRead = read(0, inputUserBuffer, sizeof(inputUserBuffer));

        // Null-terminate the input string (remove trailing newline)
        inputUserBuffer[bytesRead - 1] = '\0';

    } while (!is_EXIT_STRING(inputUserBuffer));

	write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
	
    return 0;
}
