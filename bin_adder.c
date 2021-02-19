//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>
#include "master.h"

void process(const int);
void signalHandler(int);

int main (int argc, char **argv) {
	programName=argv[0];
	signal(SIGTERM, signalHandler);
	signal(SIGUSR1, signalHandler);

	attachSPM();
	int id = atoi(argv[1]);
	int i = 0;
	for (i = 0; i < 1; i++) {
		printf("%d\n", spm->intArray[i]);
	}
	process(id);
//	printf("hello\n"); //, this is child %s", argv[1]);
	return EXIT_SUCCESS;
}

void process(const int i) {
	/* Get the total number of workers ever to be in system. */
	int n = spm->total;
	
	/* Output that process wants to enter critical section. */
//	fprintf(stderr, "%s: Process %d wants to enter critical section\n", getFormattedTime(), i);
	
	int j;
	do {
		spm->flags[i] = want_in;
		j = spm->turn;
		
		while (j != i)
			j = (spm->flags[j] != idle) ? spm->turn : (j + 1) % n;
		
		spm->flags[i] = in_cs;
		
		for (j = 0; j < n; j++)
			if (j != i && spm->flags[j] == in_cs) break;
	} while (j < n || (spm->turn != i && spm->flags[spm->turn] != idle));
	
	spm->turn = i;
	
	/* Enter critical section */
	
	/* Output that process is in critical section. */
	//fprintf(stderr, "%s: Process %d in critical section\n", getFormattedTime(), i);
	
	/* Sleep for at most 2 seconds. */
	//sleep(rand() % (CS_SLEEP_MAX - CS_SLEEP_MIN + 1) + CS_SLEEP_MIN);
	
	/* Log time, PID, index, and string to appropriate output file. */
	logOutput("adder_log", "%s %d %d\n", getFormattedTime(), getpid(), i);
	/* Output that process is exiting critical section. */
	//fprintf(stderr, "%s: Process %d exiting critical section\n", getFormattedTime(), i);
	
	/* Exit critical section */
	
	j = (spm->turn + 1) % n;
	while (spm->flags[j] == idle)
		j = (j + 1) % n;
	
	spm->turn = j;
	spm->flags[i] = idle;
	
	/* Enter remainder section */
	/* Exit remainder section */
}

/* Responsible for handling Ctrl+C and timeout signals. */
void signalHandler(int s) {
	if (s == SIGTERM || s == SIGUSR1) {
		/* Initialize a message. */
		//char message[4096];
		//strfcpy(message, "%s: Process %d exiting due to %s signal\n", getFormattedTime(), id, s == SIGUSR1 ? "timeout" : "interrupt");
		
		/* Output that message. */
		//fprintf(stderr, message);
		//logOutput("output.log", message);
		
		/* Exit abnormally. */
		exit(EXIT_FAILURE);
	}
}
