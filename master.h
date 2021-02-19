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

enum state { idle, want_in, in_cs };

#define INT_MAX 64

struct SharedProgramMemory {
	size_t total; /* Total processes to ever be in system. */
	enum state flags[20]; /* Array of all child process' mutual exclusion state. */
	enum state turn; /* Index of process that is writing to log file. */
	int intArray[64]; /* Strings loaded from input. */
	pid_t pgid; /* Child process group ID. */
};

char* programName;

int spmKey;
int smID;
struct SharedProgramMemory* spm;

#define FORMATTED_TIME_SIZE 50
#define FORMATTED_TIME_FORMAT "%H:%M:%S"

void touchFile(char*);
void allocateSPM();
void attachSPM();
void releaseSPM();
void deleteSPM();
void removeSPM();
void logOutput(char*, char*, ...);
char* getFormattedTime();

#endif
