# File Type: makefile
# Author: Steven Camacho
# Student ID: 6128597
# Assignment: Program #6 - Processes and Shared Memory

# used to identify what files are desired to be deleted
objects = MultiProcessMain.o ParentFunctions.o ChildFunctions.o

# Compiler
CC = gcc

# create custom exe file name
EXECUTE = -o

# compile code
COMPILE = -c

# user-defined header file
HEADER = MultiProcessHeader.h

# all target used to both build executable and delete object 
# files afterwards
all: build clean

# Once prerequisites are met, create excecutable file MainServer
# to run the program.
build: MultiProcessMain.o ParentFunctions.o ChildFunctions.o
	 $(CC) $(objects) $(EXECUTE) MainServer

# Create object code file for MultiProcessMain source file
MultiProcessMain.o: $(HEADER)
	$(CC) $(COMPILE) MultiProcessMain.c

# Create object code file for ParentFunctions source file
ParentFunctions.o: $(HEADER)
	$(CC) $(COMPILE) ParentFunctions.c

# Create object code file for ChildFunctions source file
ChildFunctions.o: $(HEADER)
	$(CC) $(COMPILE) ChildFunctions.c

# delete all specified object files
# can be performed both before and after build
clean: 
	rm -f $(objects)
