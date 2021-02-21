// Author: Tyler Ziggas
// Date: February 2021
// This program takes in -s, -t, or -h with -s requiring an integer between 2 and 20, -t requiring an integer above 0, and -h showing a help screen
// After any combination of those needed, you simply "datafile" as that file will automatically be filled with numbers
// Those numbers in the file are put into an array and are added until finally index 0 of the array has the sum of all integers in the file.

#include "master.h"

void setupTimer(int);
void spawnChild(int, int, int);
void signalHandler(int);
void helpMenu();

bool flag = false;

int main (int argc, char *argv[]) {
	programName = argv[0];
	int character, maxChild = 20, timeSec = 100, count = 0, items = 0, temp;
	FILE* datafile;
	time_t t;
	touchFile("datafile");
	touchFile("adder_log");
	signal(SIGINT, signalHandler);

	while ((character = getopt(argc, argv, "s:t:h")) != -1) {
		switch (character) {
			case 's':
				if (isdigit(*optarg) && atoi(optarg) <= 20 && atoi(optarg) > 1) {
					maxChild = atoi(optarg);
				} else {
					errno = 22;
					perror("-s requires an argument");
					return 1;
				}
				continue;
			case 't':
				if (isdigit(*optarg) && atoi(optarg) > 0) {
					timeSec = atoi(optarg);
				} else {
					errno = 22;
					perror("-t requires and argument");
					return 1;
				}
				continue;
			case 'h':
				helpMenu();
				continue;
			default: 
				printf("default");
				continue;
		}
	}
	if (optind < argc) {
		datafile = fopen(argv[optind], "r+");
		if (!datafile) {
			printf("Problem with the datafile.");
			return 1;
		}  {
			int randCount, random;
			srand((unsigned) time(&t));
			for (randCount = 0; randCount < INT_MAX; randCount++) {
				random =(rand() % (255 + 1));
				fprintf(datafile, "%d\n", random);
			}
			rewind(datafile);
		}
	}
	

	allocateSM();
	setupTimer(timeSec);
	while(fscanf(datafile, "%d", &temp) == 1) {
		items++;
	}
	if (items % 2 == 1) {
		items++;
	}
	double depthD = 0;
	depthD = log(items)/log(2);
	int depth = (int) ceil(depthD);
	sm->startingDepth = depth;
	//printf("%d\n", depth);	

	//printf("%d\n", items);

	rewind(datafile);

	while (count <= items && fscanf(datafile, "%d", &sm->intArray[count]) == 1) {
		//printf("%d\n", sm->intArray[count]);
		count++;
	}
	
	fclose(datafile);
	sm->total = maxChild;
	//int s = items/2;
	int index = 0;
	int i;
	int childCounter = 0;	

	while (depth > 0) {
		int depthIncrement = depthCounter(sm->startingDepth, depth);
		i = index;
		//while (childCounter < numChild) {
		//	spawnChild(childCounter++, i, depth);
		//	i = i + depthIncrement + depthIncrement;
		//}

		while (i < items) {
			wait(NULL);
			if (childCounter < maxChild) {
				spawnChild(childCounter++, i, depth);
				//printf("%d\n", childCounter);
				i = i + depthIncrement + depthIncrement;
			} else {
				//wait(NULL);
				//childCounter--;
				continue;
			}
		}
		depth--;
		//printf("%d\n", depth);
		index = 0;
		childCounter = 0;
	}
	removeSM();
	return 0;
}

void setupTimer(const int t) {
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = signalHandler;
	if (sigaction(SIGALRM, &action, NULL) != 0) {
		perror("Failed to set signal action for timer");
		exit(EXIT_FAILURE);	
	}
	struct itimerval timer;
	timer.it_value.tv_sec = t;
	timer.it_value.tv_usec = t;
	
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	
	if (setitimer(ITIMER_REAL, &timer, NULL) != 0) {
		perror("Failed to set timer");
		exit(EXIT_FAILURE);
	}
}

void spawnChild(int childCounter, int i, int depth) {
	/* Fork the current process. */
	pid_t pid = fork();
	
	/* Check if fork'ing failed. */
	if (pid == -1) {
		perror("Failed to create a child process for bin_adder");
		exit(EXIT_FAILURE);
	}

	/* Check if is child process. */
	if (pid == 0) {
		/* Enable flag to slow interrupt handler. */
		flag = true;
		
		if (childCounter == 0) sm->pgid = getpid();
		setpgid(0, sm->pgid);
		/* Disable flag to continue interrupt handler. */
		flag = false;
		
		/* Log the time this child process is starting. */
		//logOutput("adder_log", "%s: Process %d starting\n", getFormattedTime(), i);
		
		/* Convert integer "i" to string "id". */
		char id[256];	
		sprintf(id, "%d", childCounter);	
		//printf("child here, %d, %d\n", i, depth);
		char bufferi[3], bufferd[2];
		sprintf(bufferi, "%d", i);
		sprintf(bufferd, "%d", depth);
		execl("./bin_adder", "bin_adder", bufferi, bufferd, id, (char*) NULL);
		exit(EXIT_SUCCESS);
	}
}

void signalHandler(int s) {
	/* If flag is set, wait for just a bit so the child process has time to set a PGID. */
	if (flag) sleep(1);
	
	killpg(sm->pgid, s == SIGALRM ? SIGUSR1 : SIGTERM);

	while (wait(NULL) > 0);

	printf("Parent exiting...\n");	
	removeSM();
	exit(EXIT_SUCCESS);
}

void helpMenu() {
	printf("This program takes in a numbers of an array of size INT_MAX (specified in master.h) and uses the binary tree algorithm to add the integers in the array.");
	printf("The following are commands after inputting ./master : -s (followed by number of allowed processes between 0 and 20, -t (followed by amount of time process is allowed), -h for help, and lastly the file to open)");
	printf("-s and -t are required to take in an input, they will ignore the input if it is out of bounds or exit the program if it is the wrong type.");
	printf("The minimum requirement to run the program is to simply use ./master *name of file*, as the program has default values for -s and -t.");
}

