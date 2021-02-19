#include "master.h"

void setupTimer(int);
void spawnChild(int);
void signalHandler(int);
void fillFile();
void helpMenu();

bool flag = false;

int main (int argc, char *argv[]) {
	programName = argv[0];
	int character, numChild = 20, timeSec = 100, count = 0, items = 0, temp, currentChild;
	FILE* datafile;
	//touchFile("datafile");
	touchFile("output.log");
	//signal(SIGINT, signalHandler);

	while ((character = getopt(argc, argv, "s:t:h")) != -1) {
		switch (character) {
			case 's':
				printf("test s");
				if (isdigit(*optarg) && atoi(optarg) <= 20 && atoi(optarg) > 1) {
					numChild = atoi(optarg);
				} else {
					errno = 22;
					perror("-s");
					return 1;
				}
				continue;
			case 't':
				printf("test t");
				if (isdigit(*optarg) && atoi(optarg) > 0) {
					timeSec = atoi(optarg);
				} else {
					errno = 22;
					perror("-t");
					return 1;
				}
				continue;
			case 'h':
				printf("test h");
				helpMenu();
				continue;
			default: 
				printf("default");
				continue;
		}
	}
	if (optind < argc) {
		datafile = fopen(argv[optind], "r");
		if (!datafile) {
			printf("Problem with the datafile.");
			return 1;
		} //else {
		//	fillFile(*datafile);
		//}
	}
	

	allocateSPM();

	//int loadInts(datafile);

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
	printf("%d\n", depth);	

	printf("%d\n", items);

	rewind(datafile);

	while (count <= items && fscanf(datafile, "%d", &spm->intArray[count]) == 1) {
		printf("%d\n", spm->intArray[count]);
		count++;
	}
	
	fclose(datafile);
	spm->total = numChild;
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
}
	
//	setupTimer(timeSec);

	//while (count <= items && fscanf(datafile, "%d", &intArray[count]) == 1) {
	//	count++;
	//}

	
	//fclose(datafile); 

	removeSPM();
	return 0;
}

//void fillFile(FILE* datafile) {
//	int i = 0;
//	for (i < 0; i < 64; i++) {
//		fprintf(datafile, "%d", (rand() % (255 - 0 + 1)));
//	}
//}

/* Sets up timer for timeout functionality. */
//void setupTimer(const int t) {
//	struct sigaction action;
//	memset(&action, 0, sizeof(action));
//	action.sa_handler = signalHandler;
//	if (sigaction(SIGALRM, &action, NULL) != 0) crash("Failed to set signal action for timer");
//	
//	struct itimerval timer;
//	timer.it_value.tv_sec = t;
//	timer.it_value.tv_usec = t;
//	
//	timer.it_interval.tv_sec = 0;
//	timer.it_interval.tv_usec = 0;
//	
//	if (setitimer(ITIMER_REAL, &timer, NULL) != 0) crash("Failed to set timer");
//}

//int loadInts(char* datafile) {
//	FILE* intList = fopen(datafile, "r");
//}

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
		
		if (i == 0) spm->pgid = getpid();
		setpgid(0, spm->pgid);
		
		/* Disable flag to continue interrupt handler. */
		flag = false;
		
		/* Log the time this child process is starting. */
		//logOutput("output.log", "%s: Process %d starting\n", getFormattedTime(), i);
		
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
//void signalHandler(int s) {
//	/* If flag is set, wait for just a bit so the child process has time to set a PGID. */
//	if (flag) sleep(1);
	
//	/* Initialize a message. */
//	char message[4096];
//	strfcpy(message, "%s: Exiting due to %s signal\n", getFormattedTime(), s == SIGALRM ? "timeout" : "interrupt");
//	
//	/* Output that message. */
//	fprintf(stderr, message);
//	logOutput("output.log", message);
//	
//	/* Send kill signals to all child processes using appropriate signal. */
//	killpg(spm->pgid, s == SIGALRM ? SIGUSR1 : SIGTERM);
//	
//	/* To avoid having zombie processes, wait for all the children to exit. */
//	while (wait(NULL) > 0);
//	
//	/* Remove shared memory. */
//	removeSPM();
//	
//	/* Exit successfully. */
//	exit(EXIT_SUCCESS);
//}

void helpMenu() {

}

