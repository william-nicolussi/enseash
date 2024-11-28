#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define START "Welcome to ENSEA Tiny Shell.\nTo leave, press 'exit'.\n"
#define PROMPT "enseash %"
#define EXIT_MESSAGE "Bye bye...\n"
#define EXIT_CODE "exit"
#define MAX_SIZE 128

#define EXIT_STRING "exit"

// Maximum number of commands to store in history
#define MAX_HISTORY 10

int main()
{
      char command[MAX_SIZE];
      int pid,status; 
      write(STDOUT_FILENO,START,strlen(START));
      
      while(1){
            write(STDOUT_FILENO,PROMPT,strlen(PROMPT));     //writing prompt
            read(STDIN_FILENO,command,MAX_SIZE);            //reading command
            command[strlen(command)-1]='\0';          //We need to remove the last character
            pid=fork();                         
            if(pid !=0){
                  wait(&status);
                  if(strcmp(command,EXIT_CODE)==0){
                  }
            }
            else if(pid==0){
                  execlp(command,command,(char*)NULL);      //executing command (no arguments)
            }
      }
}
