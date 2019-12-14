/*=============================================================================
|   Header File:  MultiProcessHeader.h
|           Author:  Steven Camacho
|     Student ID:  6128597
|    Assignment:  Program #6 - Processes and Shared Memory
|  *==========================================================================*/

#include <stdio.h>      // required for I/O
#include <stdlib.h>     // required for malloc, free
#include <string.h>     // required for string formatting
#include <ctype.h>      // required for digit verification

#include <sys/types.h>  // these next five header files are
#include <sys/ipc.h>    // required for system variables 
#include <sys/shm.h>    // (key_t, pid_t), system calls 
#include <sys/wait.h>   // (fork(), wait()) and shared memory
#include <unistd.h>     // (shmget(), shmat(), shmdt(), shmctl())

enum ERRORS { NO_ERROR, ERROR };  // required for error checking
enum BOOLEAN { FALSE, TRUE };     // required for true/false values

#define ASCII_ZERO '0'		// used to convert char into int
#define CHILD_ID_SIZE 100   // array size for child ID
#define CMD_ARG_ERROR -1    // error in parsing cmd arg into int
#define DOUBLE 2            // used to double shared memory element
#define EMPTY_STRING ""		// initialize empty buffer
#define MAX_CMD_ARGS 8      // max number of args allowed
#define MAX_CMD_VALUE 9     // max value for each arg
#define MIN_CMD_ARGS 2      // min number of args allowed
#define MIN_CMD_VALUE 0     // min value for each arg
#define NULL_CHAR '\0'		// terminating character for string
#define SHMAT_ERROR -1		// shared attachment failed
#define SPACE_SIZE 50       // array size for child prefixes
#define STD_OUT 1           // used for unbuffered output to screen
#define PARENT_CONTINUE 2   // parent will continue after forking
#define PARENT_FLAG 0666    // flag when requesting shared memory
#define POWER_TEN 10		// multiply a number times 10

struct process {            // each child process will return
	pid_t pid;                // its pid and exit code to parent,
	int status;               // struct used to group both values
};

typedef struct process Process; // used for readability

// used ONLY when variable is shared memory array
// not every time variable is of type int *
typedef int * ShmList;


// PARENT FUNCTION PROTOTYPES 

// functions to validate the command line args
int validateCommandLine(int, char **);
int checkArgCount(int);
int getCmdArg(char *);
int isArgInt(int, char *);
int isWithinRange(int);
int isUnique(char **);

// functions to acquire shared memory for parent/child processes
int requestSharedMemory(int, int *);
int attachSharedMemory(int, ShmList *);
void fillSharedMemory(int, char **, ShmList);
void displayParentSharedMemory(char *, int, ShmList);

// functions to fork and wait for child processes
int forkingChildProcesses(int, ShmList);
int waitingOnChildProcesses(int, Process **);
void displayAllChildProcessesInfo(int, Process *);

// functions to cleanly terminate parent process
void freeProcessIDMemory(Process **);
void detachSharedMemory(ShmList *);
void removeSharedMemory(int);
void endProgram();


// CHILD FUNCTION PROTOTYPES

// functions for all child processes to execute
void ChildProcess(int, int, ShmList);
void createChildPrefix(char[], int);
void displayChildSharedMemory(char[], int, ShmList);
void updateMemory(char[], ShmList, int);