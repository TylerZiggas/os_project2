#pragma once
#ifndef MASTER_H
#define MASTER_H

#include <ctype.h>
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

#define INT_MAX 32
#define FORMATTED_TIME_SIZE 50
#define FORMATTED_TIME_FORMAT "%H:%M:%S"


enum state { idle, want_in, in_cs };

struct SharedMemory {
	size_t total; /* Total processes to ever be in system. */
	enum state flags[20]; /* Array of all child process' mutual exclusion state. */
	enum state turn; /* Index of process that is writing to log file. */
	int intArray[INT_MAX]; /* Strings loaded from input. */
	int startingDepth;
	pid_t pgid; /* Child process group ID. */
};

char* programName;
int smKey;
int smID;
struct SharedMemory* sm;

void touchFile(char*);
void logOutput(char*, char*, ...);
void allocateSM();
void attachSM();
void releaseSM();
void deleteSM();
void removeSM();
//void updateIncrement();
int  depthCounter(int, int);
char* getFormattedTime();

#endif
