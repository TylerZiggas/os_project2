//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>
#include "master.h"

void process(const int);
void signalHandler(int);

int main(int argc, char *argv[])
{
	programName = argv[0];
	//int test = atoi(argv[1]);
	printf("running bin_adder! ");
	signal(SIGTERM, signalHandler);
	signal(SIGUSR1, signalHandler);
	printf("\n%s\n %s\n", argv[1], argv[2]);
	attachSM();
	//int id = atoi(argv[1]);
	int i = 0;
	for (i = 0; i < 1; i++)
	{
		sleep(1);
		//printf("whats this %d\n", sm->intArray[i]);
	}
	//process(id);
	return EXIT_SUCCESS;
}

void process(const int i)
{
	/* Get the total number of workers ever to be in system. */
	int n = sm->total;
	/* Output that process wants to enter critical section. */
	//	fprintf(stderr, "%s: Process %d wants to enter critical section\n", getFormattedTime(), i);

	int j;
	do
	{
		sm->flags[i] = want_in;
		j = sm->turn;

		while (j != i)
		{
			j = (sm->flags[j] != idle) ? sm->turn : (j + 1) % n;
		}

		sm->flags[i] = in_cs;

		for (j = 0; j < n; j++)
		{
			if (j != i && sm->flags[j] == in_cs)
			{
				break;
			}
		}
	} while (j < n || (sm->turn != i && sm->flags[sm->turn] != idle));

	sm->turn = i;

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

	j = (sm->turn + 1) % n;
	while (sm->flags[j] == idle)
	{
		j = (j + 1) % n;
	}

	sm->turn = j;
	sm->flags[i] = idle;
}

/* Responsible for handling Ctrl+C and timeout signals. */
void signalHandler(int s)
{
	if (s == SIGTERM || s == SIGUSR1)
	{
		printf("Child exiting...\n");
		exit(EXIT_FAILURE);
	}
}
