#include "master.h"

void setupTimer(int);
void spawnChild(int);
void signalHandler(int);
void helpMenu();

bool flag = false;

int main (int argc, char *argv[]) {
	programName = argv[0];
	int character, numChild = 20, timeSec = 100, count = 0, items = 0, temp, currentChild;
	FILE* datafile;
	time_t t;

	touchFile("datafile");
	touchFile("adder_log");
	signal(SIGINT, signalHandler);

	while ((character = getopt(argc, argv, "s:t:h")) != -1) {
		switch (character) {
			case 's':
				//printf("test s");
				if (isdigit(*optarg) && atoi(optarg) <= 20 && atoi(optarg) > 1) {
					numChild = atoi(optarg);
				} else {
					errno = 22;
					perror("-s requires an argument");
					return 1;
				}
				continue;
			case 't':
				//printf("test t");
				if (isdigit(*optarg) && atoi(optarg) > 0) {
					timeSec = atoi(optarg);
				} else {
					errno = 22;
					perror("-t requires and argument");
					return 1;
				}
				continue;
			case 'h':
				//printf("test h");
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
	//setupTimer(timeSec);
	while(fscanf(datafile, "%d", &temp) == 1) {
		items++;
	}

	if (items % 2 == 1) {
		items++;
	}
	//int intArray[items];
	//intArray[items] = 0;
	double depthD = 0;
	depthD = log(items)/log(2);
	int depth = (int) ceil(depthD);
	//printf("%d\n", depth);	

	//printf("%d\n", items);

	rewind(datafile);

	while (count <= items && fscanf(datafile, "%d", &sm->intArray[count]) == 1) {
		//printf("%d\n", sm->intArray[count]);
		count++;
	}
	
	fclose(datafile);
	sm->total = numChild;
	int s = 5;
	int i = 0;
	int j = numChild;
	int k = numChild;

	while (depth > 0) {
		i = 0;
		k = numChild;
		while (i < s) {
			spawnChild(i++);
		}
		
		while (k > 0) {
			wait(NULL);
			if (i < j) {
				spawnChild(i++);
			}
			k--;
		}
		depth--;
		printf("%d\n", depth);
	}
	
//	setupTimer(timeSec);

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
/* Spawns a child given an index "i". */
void spawnChild(const int i) {
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
		
		if (i == 0) sm->pgid = getpid();
		setpgid(0, sm->pgid);
		/* Disable flag to continue interrupt handler. */
		flag = false;
		
		/* Log the time this child process is starting. */
		//logOutput("adder_log", "%s: Process %d starting\n", getFormattedTime(), i);
		
		/* Convert integer "i" to string "id". */
		char id[256];	
		sprintf(id, "%d", i);		
		/* Execute child process "palin". */
		execl("./bin_adder", "bin_adder", id, (char*) NULL);
	
		/* Exit successfully. */
		exit(EXIT_SUCCESS);
	}
}

/* Responsible for handling Ctrl+C and timeout signals. */
void signalHandler(int s) {
	/* If flag is set, wait for just a bit so the child process has time to set a PGID. */
	if (flag) sleep(1);
	
	killpg(sm->pgid, s == SIGALRM ? SIGUSR1 : SIGTERM);

	while (wait(NULL) > 0);

	printf("Parent exiting...\n");	
	/* Remove shared memory. */
	removeSM();
//	
//	/* Exit successfully. */
	exit(EXIT_SUCCESS);
}

void helpMenu() {
	printf("This program takes in a numbers of an array of size INT_MAX (specified in master.h) and uses the binary tree algorithm to add the integers in the array.");
	printf("The following are commands after inputting ./master : -s (followed by number of allowed processes between 0 and 20, -t (followed by amount of time process is allowed), -h for help, and lastly the file to open)");
	printf("-s and -t are required to take in an input, they will ignore the input if it is out of bounds or exit the program if it is the wrong type.");
	printf("The minimum requirement to run the program is to simply use ./master *name of file*, as the program has default values for -s and -t.");
}

