The aim of the program enseash_q1.c is to print something on the shell and read the 
input of the user. If the string read is equal to the label EXIT_STRING, the program terminates, 
otherwise it asks for another input.

Hence, it is needed to use a loop, and when the user types the EXIT_STRING, break the loop.

The function is_EXIT_STRING(char* input) returns 1 if the string typed by the user is equal to
EXIT_STRING, otherwise it returns 0.
This function is the trigger signal that it is needed to exit the loop.
A do-while loop was used because the condition is_EXIT_STRING(...) must be verified after 
the input was read, which is at the end of the loop.

From the input of the user, it is necessary to take into account that the last charachter is '\n' 
(user typed enter), therefore, in the vector that stores what the user typed, just replace the '\n\ 
with '\0' (endo of string).

The program also prints a start and an exit message (defined with labels).