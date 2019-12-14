/*=============================================================================
|   Source code:  ChildFunctions.c
|           Author:  Steven Camacho
|     Student ID:  6128597
|    Assignment:  Program #6 - Processes and Shared Memory
|  +--------------------------------------------------------------------------
|
|  Description:  The following functions are for the child processes that are
|				 forked by the parent. The main child process function begins
|				 and displays the shared memory at the moment it was accessed.
|				 The private ID is also displayed. The principal part of this
|				 function is that it modifies the element determined by the
|				 private ID by doubling it and multiplying it by the private
|				 ID. Finally, it once again displays the shared memory and
|				 exits.
|
|        Input:  The "main" function for the child process takes in the number
|				 of arguments, its privateID, and the shared memory list.
|
|       Output:  Just like the parent process, each child process outputs a
|				 statement describing the action its executing. Each process
|				 has n amount of spaces based on their private
|				 ID. For example, Child #1 has 2 spaces, Child #2 has 4
|				 spaces, Child #3 has 6 spaces, etc..
|
|  Procedure:
|			  * All processes output trace statements as steps are executed *
|			  1. Once the ChildProcess function is called, it creates the
|				 prefix for that process by adding the necessary amount
|				 of spaces and Child ID for the trace statements.
|             2. The shared memory is displayed along with the private ID.
|			  3. The function then updates the memory by doubling the array
|				 element corresponding to the private ID and then multiplying
|				 it by the ID. For example, Child #2 would modify the second
|				 element in the array by doubling it and multiplying by 2.
|				 Child #5, however would modify the fifth element in the array
|				 by doubling it and multiplying by 5 instead.
|			  4. Lastly, the child process displays the shared memory again
|				 and gracefully terminates.
|
|   Required Features Not Included: All required features are included.
|
|   Known Bugs: None; the program operates correctly.
|  *===========================================================================*/

#include "MultiProcessHeader.h"

/*---------------------------- ChildProcess ----------------------------
		|  Function ChildProcess (argCount, privateID, sharedList)
		|
		|  Purpose: This function is executed only by the child process, not
		|			the parent. The child's prefix is created by adding spaces
		|			and Child #- (  Child #1) It outputs that the process has
		|			begun and displays the shared memory at that moment and
		|			its private	ID. The functions calls another function that
		|			will update memory. Finally the shared memory is displayed
		|			once again and the process terminates.
		|
		|  @param  number of arguments
		|  @param  private ID
		|  @param  shared memory list
		|
		|  @return  none
		*-------------------------------------------------------------------*/
void ChildProcess(int argCount, int privateID, ShmList sharedList) {
	char processBuffer[BUFSIZ] = EMPTY_STRING;
	char childIdentifier[CHILD_ID_SIZE] = EMPTY_STRING;

	// create child prefix so that it's not repeated, more efficient
	createChildPrefix(childIdentifier, privateID);
	sprintf(processBuffer, "%s%s\n", childIdentifier, "starting");
	write(STD_OUT, processBuffer, strlen(processBuffer));

	displayChildSharedMemory(childIdentifier, argCount, sharedList);
	sprintf(processBuffer, "%s%s%d\n", childIdentifier, "displaying private"
		" ID - ",
		privateID);
	write(STD_OUT, processBuffer, strlen(processBuffer));

	updateMemory(childIdentifier, sharedList, privateID);

	displayChildSharedMemory(childIdentifier, argCount, sharedList);

	sprintf(processBuffer, "%s%s\n", childIdentifier, "finished");
	write(STD_OUT, processBuffer, strlen(processBuffer));
} // end ChildProcess function


/*-------------------------- createChildPrefix --------------------------
		|  Function createChildPrefix (childPrefix, childID)
		|
		|  Purpose: This function first outputs the actions its about to
		|			execute. The child's prefix is created in the following
		|			steps: The number of spaces the prefix has is equal to
		|			private ID times 2. So Child #1's prefix has 2 spaces,
		|			Child #2's prefix has 4 spaces, etc.. Then "Child #" is
		|			added along with the private ID.
		|
		|  @param  prefix array
		|  @param  child private ID
		|
		|  @return none
		*-------------------------------------------------------------------*/
void createChildPrefix(char childPrefix[], int childID) {
	char spaces[SPACE_SIZE] = EMPTY_STRING;

	int spaceCount = 0;
	for (; spaceCount < childID; spaceCount++) {
		strcat(spaces, "  ");
	} // end for loop

	sprintf(childPrefix, "%s%s%d%s", spaces, "Child #", childID, ": ");
} // end createChildPrefix function

/*----------------------- displayChildSharedMemory --------------------------
		|  Function displayChildSharedMemory (prefix, numArgs, listOfArgs)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. It then displays the shared memory. To output
		|			the memory each element is placed into a buffer; once all
		|			elements have been retrieved, then the memory is outputted.
		|			This prevents other trace statements from being displayed
		|			in the middle of the array.
		|
		|  @param  child prefix string
		|  @param  number of arguments
		|  @param  shared memory list
		|
		|  @return none
		*-------------------------------------------------------------------*/
void displayChildSharedMemory(char prefix[], int numArgs, ShmList listOfArgs) {
	char displayBuffer[BUFSIZ] = EMPTY_STRING;
	char displaySecondBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(displayBuffer, "%s%s", prefix, "displaying shared memory ");

	int shmIndex = 0;
	for (; shmIndex < numArgs; shmIndex++) {
		sprintf(displaySecondBuffer, "%d%s", listOfArgs[shmIndex], " ");
		strcat(displayBuffer, displaySecondBuffer);
	} // end for loop
	strcat(displayBuffer, "\n");

	write(STD_OUT, displayBuffer, strlen(displayBuffer));
} // end displayChildSharedMemory function

/*----------------------- updateMemory --------------------------
		|  Function updateMemory (mode, argCount, sharedList)
		|
		|  Purpose: This function first outputs the action its about to
		|			execute. Then the element to be modified is given by the
		|			privateID. For child #1, the element it needs to modify
		|			first element, or the element at index 0. For child #2,
		|			it's the second element, or element at index 1.
		|
		|  @param  child prefix string
		|  @param  shared memory list
		|  @param  private ID number
		|
		|  @return none
		*-------------------------------------------------------------------*/
void updateMemory(char prefix[], ShmList memory, int privateID) {
	char updateBuffer[BUFSIZ] = EMPTY_STRING;

	sprintf(updateBuffer, "%s%s\n", prefix, "updating shared memory");
	write(STD_OUT, updateBuffer, strlen(updateBuffer));

	int individIndex = privateID - 1;
	memory[individIndex] = memory[individIndex] * DOUBLE * privateID;
} // end updateMemory function