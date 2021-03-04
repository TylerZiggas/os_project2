// Author: Tyler Ziggas
// Date: February 2021
// bin_adder is the core of what the children do, dealing with the flags of solution 4 from our notes, adding the binary tree nodes, and logging the information required.

#include "master.h"

void process(const int, int, int);
void signalHandler(int);

int main (int argc, char *argv[]) { // Main call from execl in master
	signal(SIGTERM, signalHandler); // Set up signals 
	signal(SIGUSR1, signalHandler);
	int i = atoi(argv[1]); // Take arguments from execl and make them into ints
	int depth = atoi(argv[2]);
	int id = atoi (argv[3]);
	attachSM(); // Attach shared memory
	process(id, i, depth); // Do math, flags, and output to file
	return EXIT_SUCCESS;
}

void process(const int id, int i, int depth) {
	int n = sm->total; // Bring the total from shared memory
	//printf("%s: Process %d wants to enter critical section\n", getFormattedTime(), id);
	int depthIncrement = depthCounter(sm->startingDepth, depth); // Figure out the depth increment
	int gap = depthIncrement;
	int secondi = i + gap;
	int firstIndex = sm->intArray[i];	
	int secondIndex = sm->intArray[secondi];
	int newFirstIndex = firstIndex + secondIndex; // Addition of first index and second index
	sm->intArray[i] = newFirstIndex; 
	
	printf("%s Child %d is trying to enter critical section\n", getFormattedTime(), id);
	//printf("%d = %d + %d\n", newFirstIndex, firstIndex, secondIndex);
	// Uncomment above to see math
	int j;
	do { // Solution 4
		sm->flags[id] = want_in; // Set flag
		j = sm->turn;
		
		while (j != id) { // Check to see if we can enter critical section
			j = (sm->flags[j] != idle) ? sm->turn : (j + 1) % n;
		}

		sm->flags[id] = in_cs; // Are now flagged to be in critical section
		
		for (j = 0; j < n; j++) {
			if (j != id && sm->flags[j] == in_cs) {
				break;
			}
		}
	} while (j < n || (sm->turn != id && sm->flags[sm->turn] != idle));
	
	sm->turn = id; // Log turn based on the id of the process
	logOutput("adder_log", "Time:%s PID:%d Index:%d Depth:%d\n", getFormattedTime(), getpid(), id, depth); // Critical Section, putting into file
	printf("%s Child %d is leaving critical section\n", getFormattedTime(), id);
	
	j = (sm->turn + 1) % n;
	while (sm->flags[j] == idle) {
		j = (j + 1) % n;
	}
	
	sm->turn = j;
	sm->flags[id] = idle;
}

void signalHandler(int s) { // Catches signal for ^C or Timeout
	if (s == SIGTERM || s == SIGUSR1) {
		printf("Child exiting...\n");
		exit(EXIT_FAILURE);
	}
}
