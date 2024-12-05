# Tiny Shell Project ENSEA

Overview:
The project is sessions 1&2 of the lab of the course "System programming" at 
ENSEA university.
The aim of this project is to code a simple shell.
The lab consists in seven jobs, each one corresponds to a folder.

Q1:
Display a welcome message and read user input.

Q2:
Execute of the entered command (fork()) and return to the prompt.

Q3:
Exit the shell if user's input is EXIT_STRING or EOF.

Q4:
Display the return code or signal of the previous command in the prompt.

Q5:
Measure the time of command execution (clock_gettime).

Q6:
Execute command with arguments.

Q7:
Redirect to stdin or stdout if user uses '<' and '>'.

How to compile and execute:
In every folder (one per question), there is a Makefile.
It is possible to compile the *.c file by typing "make".
It is possible to execute the binary file by typing "make exec".