The program enseash_q2.c is an evolution of enseash_q1.c; besides the EXIT_STRING, 
the program recognise FORTUNE_STRING_INPUT and DATE_STRING_INPUT and executes 
the corresponding functions.

From the previous program, the do-while structure was mantained. The function that 
triggers the exit was modified into the function stringsAreEquals(char* str1, char* str2). 
The function stringsAreEquals(...) is used also to trigger the two functions from 
the two commands FORTUNE_STRING_INPUT and DATE_STRING_INPUT.

In the case of FORTUNE_STRING_INPUT is typed, the function execute_fortune() is executed and 
it prints FORTUNE_MESSAGE on the shell.

In the case of DATE_STRING_INPUT is typed, the function execute_date() is executed.
A child is generated using fork(), then, the child process execute "date" on the shell and 
prints the output (child has the same behaviour as writing "date" on the shell).
This function manages also errors of fork().