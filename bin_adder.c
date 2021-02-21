#include "master.h"

void process(const int, int, int);
void signalHandler(int);

int main (int argc, char *argv[]) {
	programName=argv[0];
	signal(SIGTERM, signalHandler);
	signal(SIGUSR1, signalHandler);
	int i = atoi(argv[1]);
	int depth = atoi(argv[2]);
	int id = atoi (argv[3]);
	attachSM();
	process(id, i, depth);
	return EXIT_SUCCESS;
}

void process(const int id, int i, int depth) {
	int n = sm->total;	
	//printf("%s: Process %d wants to enter critical section\n", getFormattedTime(), id);
	//sleep(1);
	int depthIncrement = depthCounter(sm->startingDepth, depth);
	int gap = depthIncrement;
	int secondi = i + gap;
	int firstIndex = sm->intArray[i];	
	int secondIndex = sm->intArray[secondi];
	int newFirstIndex = firstIndex + secondIndex;
	sm->intArray[i] = newFirstIndex;
	
	printf("%d = %d + %d\n", newFirstIndex, firstIndex, secondIndex);

	int j;
	do {
		sm->flags[id] = want_in;
		j = sm->turn;
		
		while (j != id) { 
			j = (sm->flags[j] != idle) ? sm->turn : (j + 1) % n;
		}

		sm->flags[id] = in_cs;
		
		for (j = 0; j < n; j++) {
			if (j != id && sm->flags[j] == in_cs) {
				break;
			}
		}
	} while (j < n || (sm->turn != id && sm->flags[sm->turn] != idle));
	
	sm->turn = id;
	
	/* Enter critical section */
	
	/* Output that process is in critical section. */
	//fprintf(stderr, "%s: Process %d in critical section\n", getFormattedTime(), i);
	
	logOutput("adder_log", "Time:%s PID:%d Index:%d Depth:%d\n", getFormattedTime(), getpid(), i, depth);
	//logOutput("Time:%s Exiting Critical Section, this sum is %d\n", getFormattedTime(), sm->intArray[i]);
	/* Output that process is exiting critical section. */
	//fprintf(stderr, "%s: Process %d exiting critical section\n", getFormattedTime(), i);
	
	/* Exit critical section */
	
	j = (sm->turn + 1) % n;
	while (sm->flags[j] == idle) {
		j = (j + 1) % n;
	}
	
	sm->turn = j;
	sm->flags[id] = idle;
}

void signalHandler(int s) {
	if (s == SIGTERM || s == SIGUSR1) {
		printf("Child exiting...\n");
		exit(EXIT_FAILURE);
	}
}
