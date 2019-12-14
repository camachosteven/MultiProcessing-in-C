This C multiprocessing program was my final project in FIU's systems programming course.

We were instructed to receive between 1-7 unique numbers ranging between 0 and 9 from the command line. 
Once command line input was validated, the parent process had to request and attach shared memory
for all processes to access and to store all input from command line. The number of processes spawn 
is equivalent to the number of arguments. Each child process has assigned one element to modify within the
list of args and then return back to the parent process. After all child processes terminated, parent process
displayed final state of shared memory, clean up all memory allocated during the process, and terminated. 
Throughout the entirety of the program, a trace statement is displayed without buffer so user know which 
processses is doing what. 
