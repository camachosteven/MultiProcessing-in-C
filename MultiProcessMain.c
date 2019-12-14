/*=============================================================================
|   Source code:  MultiProcessMain.c
|           Author:  Steven Camacho
|     Student ID:  6128597
|    Assignment:  Program #6 - Processes and Shared Memory
|
|            Course:  COP 4338 (Advanced Programming)
|           Section:  U03
|        Instructor:  William Feild
|        Due Date:  3 December 2019, at the beginning of class
|
|	I hereby certify that this collective work is my own
|	and none of it is the work of any other person or entity.
|	______________________________________ [Signature]
|
|        Language:  C
|  Compile/Run:
|
|	- The program requires command line arguments from user. For the input,
|	  the user must enter between 1 and 7 arguments. Each argument must also
|	  be an integer between 0 and 9, inclusive. No duplicates are allowed
|	  within arguments. If the arguments do not meet these requirements,
|	  the program will output the error and gracefully terminate.
|	- All of the source code files, header files, and makefile must be in 
|	  the SAME directory. Since the makefile is named 'makefile' and not 
|	  some other filename, make sure there are no other makefiles in the
|	  directory. If the makefile does not work, alternate compilation 
|	  instructions are shown below so that the program can still be run. 
|
|	  ('all' target will compile, create executable, and then clean)
|
| 	  make all
|     ./MainServer 1 9 2 8 3
|
|	  ************* Alternate compilation instructions ******************
|	  If the makefile does not compile the code and create the executable
|	  file, type in the following: 
|
|	  gcc MultiProcessMain.c ParentFunctions.c ChildFunctions.c 
|		  MultiProcessHeader.h -o MainServer
|	  
|	  This will also create the executable file named 'MainServer'; after 
|	  compilation, execute as such: 
|
|	  ./MainServer 1 9 2 8 3
|
|  +--------------------------------------------------------------------------
|
|  Description:  This program gets and attaches shared memory, stores an array
|				 of integers in the shared memory, forks multiple child
|				 processes so that each process will modify a single array
|				 element in that shared memory by doubling the element and
|				 multiplying it by the child's private ID; finally, it outputs
|				 the final state of shared memory, detaches and removes it,
|				 then terminates. The size of the array and values of the
|				 elements for the shared memory is determined by the command
|				 line arguments.
|
|        Input:  The user must input between 1 and 7 arguments. Each argument
|				 must be a unique integer between 0 and 9, inclusive, with no
|				 duplicates occurring.
|				 ************* Examples of valid arguments *****************
|					./MainServer 3 1 7
|					./MainServer 9 5 8 2 1 4
|			     ************* Examples of invalid arguments ***************
|					./MainServer
|					./MainServer 3 3 7
|					./MainServer fail 9 2
|					./MainServer 56782
|
|       Output:  As each process (parent or child), performs a step/function,
|				 it outputs a statement describing the action its executing.
|				 The processes do so without a buffer so that the display will
|				 show how each process executed its steps, with a prefix
|				 identifying which process did what. For the children, their
|				 prefixes will have n amount of spaces based on their private
|				 ID. For example, Child #1 has 2 spaces, Child #2 has 4
|				 spaces, Child #3 has 6 spaces, etc.. Therefore, the parent
|				 prefix will have 0 spaces before it.
|				 ***************** Output Disclaimer ***********************
|			   - The output on the FIU SCIS computer lab Unix system has the
|				 child processes interleaved with one another along with the 
|				 parent as expected. However, for output on the Ocelot SSH,
|				 each child process outputs all at once before going to the 
|				 next child process.
|
|  Procedure:
|			  * All processes output trace statements as steps are executed *
|			  1. Command line arguments are checked to determine their validity
|				 and will terminate if not valid.
|			  2. Shared memory is requested using IPC_PRIVATE as key and will
|				 terminate if request was unsuccessful.
|			  3. Shared memory is attached to parent address space and will
|				 terminate if attachment was unsuccessful.
|			  4. Shared memory is filled with the command line arguments as an
|				 array.
|			  5. Parent displays the initial state of the shared memory.
|			  6. Parent begins forking child processes and will terminate if
|				 it is unable to fork all of the children. The child processes
|				 will return no error back to the main which will terminate
|				 immediately. The parent process will return a certain value
|				 that will allow it to continue instead of terminate.
|			  7. Each child will display the shared memory at the moment it was
|				 access. The child with then modify the array element
|				 determined by their private ID. They will double that element,
|				 then multiply it by the private ID. Lastly, the child will
|				 once again display the shared memory at the moment it was
|				 accessed, then terminate.
|			  8. Parent waits for all children and stores their pids and exit
|				 codes in the order of their termination.
|			  9. Once all child processes have finished, parent displays their
|				 process IDs and their exit codes.
|			 10. Parent displays the final state of the shared memory.
|			 11. Parent detaches, removes shared memory, and terminates.
|
|   Required Features Not Included: All required features are included.
|
|   Known Bugs: None; the program operates correctly.
|  *===========================================================================*/

#include "MultiProcessHeader.h"

int main(int argc, char * argv[]) {
	if (!validateCommandLine(argc, argv)) { return ERROR; }

	Process * proIDList = NULL; // store processID, code of child processes
	int * argsList = NULL; // pointer to the shared memory
	int forkReturn = 0; // used to determine result of forking children
	int memoryID = 0; // store shared memory so it can be attached and removed

	if (requestSharedMemory(--argc, &memoryID)) { return ERROR; }
	if (attachSharedMemory(memoryID, &argsList)) { return ERROR; }

	fillSharedMemory(argc, argv, argsList);
	displayParentSharedMemory("initial", argc, argsList);

	// pass shared memory pointer to child process since key is IPC_PRIVATE
	forkReturn = forkingChildProcesses(argc, argsList);

	// if function above returns in error, forking of all children failed
	if (forkReturn == ERROR) { return ERROR; }

	// if function returns 0, this means child process finished and must exit
	if (forkReturn == NO_ERROR) { return NO_ERROR; }

	// function returns error if malloc required to store the pids and exit
	// codes of the children returned NULL
	if (waitingOnChildProcesses(argc, &proIDList)) { return ERROR; }

	displayAllChildProcessesInfo(argc, proIDList);
	displayParentSharedMemory("final", argc, argsList);

	freeProcessIDMemory(&proIDList);
	detachSharedMemory(&argsList);
	removeSharedMemory(memoryID);
	endProgram();

	return NO_ERROR;
} // end main function