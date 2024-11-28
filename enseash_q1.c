#include <unistd.h>
#include <string.h>

#define PROMPT "Welcome to ENSEA Tiny Shell.\nTo leave, press 'exit'.\nenseash %\n"
#define EXIT_STRING "exit"

int main()
{
	char buffer[128];  // Buffer to store input
	int bytesRead;
	
	int typedExit = 0;	//variable that becomes 1 if user types EXIT_STRING
	
	do
	{
		write(STDOUT_FILENO,PROMPT,strlen(PROMPT));
	
		bytesRead = read(0, buffer, sizeof(buffer) - 1);

		// Null-terminate the string (since `read` doesn't do this)
		buffer[bytesRead] = '\0';

		//equivalent of printf("You entered: %s", buffer);
		write(STDOUT_FILENO,buffer,strlen(buffer));
		
		char lenEX = strlen(EXIT_STRING);
		write(STDOUT_FILENO, lenEX, 1);
		
		char lenB = strlen(buffer);
		write(STDOUT_FILENO, lenB, 1);
		
		//compare string charachters and string lenght
		if(strncmp(buffer, EXIT_STRING, strlen(EXIT_STRING)) == 0 && strlen(buffer)==strlen(EXIT_STRING)+1)
		{
			write(STDOUT_FILENO,"exit typed", 10);
			typedExit = 1;
		}
	} while(typedExit == 0);
	
	return 0;
}
