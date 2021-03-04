#pragma once
#ifndef MASTER_H
#define MASTER_H // Define this file

#include <ctype.h> // Declared headers for all files 
#include <getopt.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#define INT_MAX 64 // Creation of constants 
#define FORMATTED_TIME_SIZE 50 
#define FORMATTED_TIME_FORMAT "%H:%M:%S"


enum state { idle, want_in, in_cs };

struct SharedMemory {
	size_t total; // Total processes
	enum state flags[20]; // Array of flags for all child processes
	enum state turn; // Index for writing to log file
	int intArray[INT_MAX]; // Creation of shared memory array
	int startingDepth; // Starting depth to be accessed from anywhere
	pid_t pgid; // group pid
};

// Shared memory declarations
int smKey;
int smID;
struct SharedMemory* sm;

// Functions that reside with shared.c
void createFile(char*);
void logOutput(char*, char*, ...);
void allocateSM();
void attachSM();
void releaseSM();
void deleteSM();
void removeSM();
int  depthCounter(int, int);
char* getFormattedTime();

#endif
