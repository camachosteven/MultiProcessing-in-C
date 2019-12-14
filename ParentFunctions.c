/*=============================================================================
|   Source code:  ParentFunctions.c
|           Author:  Steven Camacho
|     Student ID:  6128597
|    Assignment:  Program #6 - Processes and Shared Memory
|  +--------------------------------------------------------------------------
|
|  Description:  These parent functions are called by the main and take in
|				 the server role and the child processes are the cilents.
|				 The command line is validated and shared memory is requested,
|				 attached, and filled with the arguments. Child process are
|				 forked so that each process will modify a single array
|				 element. When all children have terminated, the process IDs
|				 and exit codes are displayed in the order that the children
|				 exited. The shared memory is then detached, removed, and the
|				 program terminates.
|
|        Input:  The user must input between 1 and 7 arguments. Each argument
|				 must be a unique integer between 0 and 9, inclusive, with no
|				 duplicates occurring.
|				 *	Examples of valid arguments *
|					./MainServer 3 1 7
|					./MainServer 9 5 8 2 1 4
|			     * Examples of invalid arguments *
|					./MainServer
|					./MainServer 3 3 7
|					./MainServer fail 9 2
|					./MainServer 56782
|
|       Output:  As each process (parent or child) performs a step/function,
|				 it outputs a statement describing the action its executing.
|				 The processes do so without a buffer so that the display will
|				 show how each process executed its steps, with a prefix
|				 identifying which process did what. The parent
|				 prefix will have 0 spaces before it.
|
|  Procedure:
|			  * All processes output trace statements as steps are executed *
|			  1. The validateCommandLine function checks the arguments by first
|				 checking whether if the number of arguments is valid. The
|				 arguments are then checked to determine whether they are
|				 an integer or not. The range is checked to see if the
|				 arguments fall within the given range [0, 9]. Lastly, the
|				 arguments are all valid if they are unique from one another.
|				 The arguments will NOT be valid if they do not meet all of
|				 these tests.
|			  2. The requestSharedMemory function uses shmget() to get shared
|				 memory and will return an error if request failed.
|			  3. The attachSharedMemory functions uses shmat() to attach
|				 memory to the parent's address space and will return an error
|				 if attachment failed.
|			  4. The fillSharedMemory function initializes the shm array with
|				 all the command line arguments.
|			  5. The displayParentSharedMemory function displays the initial
|				 array.
|			  6. The forkingChildProcesses and waitingOnChildProcesses
|				 functions fork the child processes and wait for them to
|				 complete their tasks. If all children are not forked, then
|				 parent will terminate. As the children terminate, the wait
|				 method returns the pid and stores exit code in parameter var.
|			  7. The displayAllChildProcessesInfo function displays all the
|				 process IDs and exit codes in the order they terminated.
|			  8. The freeProcessIDMemory function deallocates memory to store
|				 all the process IDs.
|			  9. Finally, the parent detaches, removes the shared memory, and
|				 then terminates with no error.
|
|   Required Features Not Included: All required features are included.
|
|   Known Bugs: None; the program operates correctly.
|  *===========================================================================*/

#include "MultiProcessHeader.h"

/*---------------------------- validateCommandLine ----------------------------
		|  Function validateCommandLine (numArgs, listOfArgs)
		|
		|  Purpose: This function validates the command line arguments. First,
		|			if checks the number of arguments is between 1 and 7
		|			inclusive. Then each argument checked to see if they are
		|			indeed an int. Arguments are also verified to see if they
		|			are within range and if they are unique from all other
		|			arguments.
		|
		|  @param  number of arguments
		|  @param  argument values
		|
		|  @return  TRUE if arguments meet all these requirements
		|			FALSE otherwise
		*-------------------------------------------------------------------*/
int validateCommandLine(int numArgs, char ** listOfArgs) {
	char validateBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(validateBuffer, "%s\n", "Parent: starting");
	write(STD_OUT, validateBuffer, strlen(validateBuffer));
	sprintf(validateBuffer, "%s\n", "Parent: validating command line");
	write(STD_OUT, validateBuffer, strlen(validateBuffer));

	if (!checkArgCount(numArgs--)) { return FALSE; }

	int cmdArg = 0;
	int index = 1;

	do {
		cmdArg = getCmdArg(listOfArgs[index]);

		if (!isArgInt(cmdArg, listOfArgs[index])) { return FALSE; }
		if (!isWithinRange(cmdArg)) { return FALSE; }
		if (!isUnique(listOfArgs)) { return FALSE; }

	} while (++index <= numArgs); // end do-while loop

	return TRUE;
} // end validateCommandLine function

/*---------------------------- checkArgCount ----------------------------
		|  Function checkArgCount (argCount)
		|
		|  Purpose: This function is used as command line validation test.
		|			The test is passed if the number of arguments is within
		|			the min and max constants defined in the header file.
		|
		|  @param	number of arguments
		|
		|  @return  TRUE if within range of min and max values
		|			FALSE otherwise
		*-------------------------------------------------------------------*/
int checkArgCount(int argCount) {
	if ((argCount < MIN_CMD_ARGS) || (argCount > MAX_CMD_ARGS)) {
		printf("%s\n", "Parent: Incorrect number of command-line arguments "
			"entered.");
		printf("%s\n", "Please enter between 1 and 7 numbers, each being a "
			"unique integer between 0-9 inclusive. ");
		return FALSE;
	} // end if statement

	return TRUE;
} // end checkArgCount function

/*---------------------------- getCmdArg ----------------------------
		|  Function getCmdArg (argChar)
		|
		|  Purpose: This function takes a string and converts it into an int.
		|			If the string is not an integer, then function will return
		|			a predefined command argument error.
		|
		|  @param  argument string
		|
		|  @return the integer from the string, -1 otherwise
		*-------------------------------------------------------------------*/
int getCmdArg(char * argChar) {
	int number = 0;

	while ((*argChar != NULL_CHAR) && isdigit(*argChar)) {
		number *= POWER_TEN;
		number += *argChar++ - ASCII_ZERO;
	} // end while loop

	if (*argChar == NULL_CHAR) { return number; }

	return CMD_ARG_ERROR;
} // end getCmdArg function

/*---------------------------- isArgInt ----------------------------
		|  Function isArgInt (argument, argPtr)
		|
		|  Purpose: This function is used as command line validation test.
		|			The test is passed if the argument is not the command
		|			argument error.
		|
		|  @param  argument integer
		|  @param  argument string used in case test fails
		|  @return FALSE if argument equals -1, TRUE otherwise
		*-------------------------------------------------------------------*/
int isArgInt(int argument, char * argPtr) {
	if (argument == CMD_ARG_ERROR) {
		printf("%s%s%s\n", "Parent: The following argument is not an "
			"integer: ", argPtr, "\nPlease enter valid command"
			" line arguments.");
		return FALSE;
	} // end if statement

	return TRUE;
} // end isArgInt function

/*---------------------------- isWithinRange ----------------------------
		|  Function isWithinRange (number)
		|
		|  Purpose: This function is used as command line validation test.
		|			The test is passed if the number is between 0 and 9,
		|			inclusive.
		|
		|  @param  argument in int type

		|  @return TRUE if number falls within range, FALSE otherwise
		*-------------------------------------------------------------------*/
int isWithinRange(int number) {
	if ((number > MAX_CMD_VALUE) || (number < MIN_CMD_VALUE)) {
		printf("%s%d%s\n", "Parent: The following argument is outside of "
			"the range of [0-9]: ", number, "\nPlease enter arguments "
			"between 0 and 9 inclusive.");
		return FALSE;
	} // end if statement

	return TRUE;
} // end isWithinRange function

/*---------------------------- isUnique ----------------------------
		|  Function isArgInt (argsList)
		|
		|  Purpose: This function is used as command line validation test.
		|			The test is passed if the argument is different from
		|			all the other arguments before it.
		|
		|  @param  list of all arguments
		|
		|  @return FALSE if argument is equal to any other, TRUE otherwise
		*-------------------------------------------------------------------*/
int isUnique(char ** argList) {
	static int lastIndex = 1;

	int lastArg = getCmdArg(argList[lastIndex]);
	int currentIndex = 1;
	for (; currentIndex < lastIndex; currentIndex++) {
		if (lastArg == getCmdArg(argList[currentIndex])) {
			printf("%s%d%s\n", "Parent: The value '", lastArg,"' occurs more"
				" than once.\nAll command line arguments be distinct from "
				"one another.");
			return FALSE;
		} // end if statement

	} // end for loop

	lastIndex++;

	return TRUE;
} // end isUnique function

/*---------------------------- requestSharedMemory ----------------------------
		|  Function requestSharedMemory (argCount, shmIdPtr)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. It then requests shared memory using shmget() and
		|			requests argCount * sizeof(int) bytes. The function will
		|			return an error value if request was unsuccessful.
		|
		|  @param  number of arguments
		|  @param  pointer to the memory ID variable in main

		|  @return ERROR if shared memory request failed, NO_ERROR otherwise
		*-------------------------------------------------------------------*/
int requestSharedMemory(int argCount, int * shmIdPtr) {
	char requestBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(requestBuffer, "%s\n", "Parent: requesting shared memory");
	write(STD_OUT, requestBuffer, strlen(requestBuffer));

	*shmIdPtr = shmget(IPC_PRIVATE, argCount * sizeof(int),
		IPC_CREAT | PARENT_FLAG);

	if (*shmIdPtr < 0) {
		printf("%s\n", "Parent: shared memory request failed.");
		return ERROR;
	} // end if statement

	return NO_ERROR;
} // end requestSharedMemory function

/*---------------------------- attachSharedMemory ----------------------------
		|  Function attachSharedMemory (shmId, shmListPtr)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. It then attaches shared memory using shmat() and
		|			attaches the shared memory to the current address space.
		|			The function will return an error value if attachment was
		|			unsuccessful.
		|
		|  @param  number of arguments
		|  @param  pointer to the memory ID variable in main

		|  @return ERROR if shared memory request failed, NO_ERROR otherwise
		*-------------------------------------------------------------------*/
int attachSharedMemory(int shmId, ShmList * shmListPtr) {
	char attachBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(attachBuffer, "%s\n", "Parent: attaching shared memory");
	write(STD_OUT, attachBuffer, strlen(attachBuffer));

	*shmListPtr = (int *) shmat(shmId, NULL, 0);

	if (((long) *shmListPtr) == SHMAT_ERROR) {
		printf("%s\n", "Parent: shared memory attachment failed.");
		return ERROR;
	} // end if statement

	return NO_ERROR;
} // end attachSharedMemory function

/*---------------------------- fillSharedMemory ----------------------------
		|  Function fillSharedMemory (argCount, argValues, shmListPtr)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. It then fills the shared array with the command
		|			line arguments. The arguments are converted into an int,
		|			since they have already been validated, before storing
		|			them in the array.
		|
		|  @param  number of arguments
		|  @param  command line argument values
		|  @param  pointer to the shared array
		|
		|  @return none
		*-------------------------------------------------------------------*/
void fillSharedMemory(int argCount, char ** argValues, ShmList shmListPtr) {
	char fillBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(fillBuffer, "%s\n", "Parent: filling shared memory");
	write(STD_OUT, fillBuffer, strlen(fillBuffer));

	int shmIndex = 0;
	for (; shmIndex < argCount; shmIndex++) {
		shmListPtr[shmIndex] = getCmdArg(argValues[shmIndex + 1]);
	} // end for loop

} // end fillSharedMemory function

/*----------------------- displayParentSharedMemory --------------------------
		|  Function displayParentSharedMemory (mode, argCount, sharedList)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. It then displays the shared memory. The only
		|			difference between this version of displaying the memory
		|			and the children's version is that this version also
		|			outputs whether the state of the memory is the initial
		|			before the children to their tasks or the final after
		|			the children have all terminated.
		|
		|  @param  string identifying whether its initial or final
		|  @param  number of arguments
		|  @param  shared memory list
		|
		|  @return none
		*-------------------------------------------------------------------*/
void displayParentSharedMemory(char * mode, int argCount, ShmList sharedList) {
	char displayBuffer[BUFSIZ] = EMPTY_STRING;
	char displaySecondBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(displayBuffer, "%s%s%s", "Parent: displaying ", mode, 
		" shared memory ");

	int shmIndex = 0;
	for (; shmIndex < argCount; shmIndex++) {
		sprintf(displaySecondBuffer, "%d%s", sharedList[shmIndex], " ");
		strcat(displayBuffer, displaySecondBuffer);
	} // end for loop

	strcat(displayBuffer, "\n");
	write(STD_OUT, displayBuffer, strlen(displayBuffer));
} // end displayParentSharedMemory function

/*-------------------------- forkingChildProcesses --------------------------
		|  Function forkingChildProcesses (argCount, sharedList)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. It then forks each and every child process. If
		|			forking was successful, each child is given a function
		|			to execute and will terminate. The parent will return a
		|			value back to the main that will allow it continue with its
		|			process. If parent cannot fork all of the children, then
		|			then the program will terminate with an error code.
		|
		|  @param  number of arguments
		|  @param  pointer to the shared array
		|
		|  @return ERROR if forking all children failed
		|		   NO_ERROR for child to terminate gracefully
		|		   PARENT_CONTINUE for parent to continue after forking
		*-------------------------------------------------------------------*/
int forkingChildProcesses(int argCount, ShmList sharedList) {
	int forkID = 0;
	char forkBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(forkBuffer, "%s\n\n", "Parent: forking (each and every) "
		"child process");
	write(STD_OUT, forkBuffer, strlen(forkBuffer));

	// number of child processes is equal to the number of arguments
	int proCounter = 1;
	for (; proCounter <= argCount; proCounter++) {
		if ((forkID = fork()) < 0) {
			printf("%s%d%s\n", "Parent: forking child #",
				proCounter, "failed.");
			return ERROR;
		} // end 'forkID' if
		else if (forkID == 0) {
			ChildProcess(argCount, proCounter, sharedList);
			return NO_ERROR;
		} // end 'forkID' else if

	} // end for loop

	return PARENT_CONTINUE;
} // end forkingChildProcesses function

/*-------------------------- waitingOnChildProcesses --------------------------
		|  Function waitingOnChildProcesses (argCount, proListPtr)
		|
		|  Purpose: This function first outputs the actions its about to
		|			execute. The parent then allocates memory to store the
		|			process IDs and their exit codes. It then waits for all
		|			for all the children to terminate and stores their values
		|			in the order in which they exit.
		|
		|  @param  number of arguments
		|  @param  pointer to the process ID list
		|
		|  @return ERROR if memory cannot be allocated to store process info
		|		   NO_ERROR otherwise
		*-------------------------------------------------------------------*/
int waitingOnChildProcesses(int argCount, Process ** proListPtr) {
	char waitingBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(waitingBuffer, "%s\n", "Parent: waiting for (each and "
		"every) child");
	write(STD_OUT, waitingBuffer, strlen(waitingBuffer));

	sprintf(waitingBuffer, "%s\n", "Parent: requesting dynamic "
		"memory for process ID list");
	write(STD_OUT, waitingBuffer, strlen(waitingBuffer));

	*proListPtr = malloc(argCount * sizeof(Process));

	if (*proListPtr == NULL) {
		printf("%s\n", "Parent: process ID list malloc failed.");
		return ERROR;
	} // end if statement

	sprintf(waitingBuffer, "%s\n", "Parent: filling process ID list "
		"as children exit");
	write(STD_OUT, waitingBuffer, strlen(waitingBuffer));

	int statusCode = 0;
	int processIndex = 0;
	for (; processIndex < argCount; processIndex++) {
		(*proListPtr)[processIndex].pid = wait(&statusCode);
		(*proListPtr)[processIndex].status = statusCode;
	} // end for loop

	return NO_ERROR;
} // end waitingOnChildProcesses function

/*--------------------- displayAllChildProcessesInfo -------------------------
		|  Function displayAllChildProcessesInfo (argCount, processList)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. The process IDs and their exit codes are then
		|			displayed in the order in which they terminated.
		|
		|  @param  number of arguments
		|  @param  pointer to the process ID list
		|
		|  @return none
		*-------------------------------------------------------------------*/
void displayAllChildProcessesInfo(int argCount, Process * processList) {
	printf("\n%s\n", "Parent: detected (each and every) child completion");
	printf("%s\n", "Parent: displaying all Process IDs and exit codes");
	printf("%s\n", "--------------------------------------------------");

	int processIndex = 0;
	for (; processIndex < argCount; processIndex++) {
		printf("%s%ld%s%d\n", "Parent: Process ",
			(long) processList[processIndex].pid, " exited with code ",
			processList[processIndex].status);
	} // end for loop

	printf("%s\n", "--------------------------------------------------");
} // end displayAllChildProcessesInfo function

/*------------------------- freeProcessIDMemory -----------------------------
		|  Function freeProcessIDMemory (proIDListPtr)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. The process ID list is deallocated since its
		|			use is no longer needed. Remember that memory is allocated
		|			in the waitingOnChildProcesses function.
		|
		|  @param  pointer to the process ID list
		|
		|  @return none
		*-------------------------------------------------------------------*/
void freeProcessIDMemory(Process ** proIDListPtr) {
	printf("%s\n", "Parent: freeing dynamic memory for process ID list");

	void * tempPtr = *proIDListPtr;
	free(tempPtr);
} // end freeProcessIDMemory function

/*--------------------------- detachSharedMemory ----------------------------
		|  Function detachSharedMemory (sharedListPtr)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. The shared memory array is detached from the
		|			parent's address space using shmdt().
		|
		|  @param  pointer to the process ID list
		|
		|  @return none
		*-------------------------------------------------------------------*/
void detachSharedMemory(ShmList * sharedListPtr) {
	printf("%s\n", "Parent: detaching shared memory");

	void * listPtr = (void *) *sharedListPtr;
	shmdt(listPtr);
} // end detachSharedMemory function

/*--------------------------- removeSharedMemory ----------------------------
		|  Function removeSharedMemory (memID)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. The shared memory array is removed using shmctl().
		|
		|  @param  memory ID number
		|
		|  @return none
		*-------------------------------------------------------------------*/
void removeSharedMemory(int memID) {
	printf("%s\n", "Parent: removing shared memory");

	shmctl(memID, IPC_RMID, NULL);
} // end removeSharedMemory function

/*--------------------------- endProgram ----------------------------
		|  Function endProgram ()
		|
		|  Purpose: This function simply tells the user that the program
		|			successfully accomplished its tasks and will terminate.
		|
		|  @param  none
		|
		|  @return none
		*-------------------------------------------------------------------*/
void endProgram() {
	printf("%s\n", "Parent: finished");
} // end endProgram function