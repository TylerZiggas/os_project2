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
	int character, optargCount, maxChild = 20, timeSec = 100, count = 0, items = 0, temp; // Intiailize necessary variables
	FILE* datafile;
	time_t t;
	bool allDigit = true;
	touchFile("datafile"); // This is the file we are looking for as we are randomly generating it to that
	touchFile("adder_log");
	signal(SIGINT, signalHandler);

	while ((character = getopt(argc, argv, "s:t:h")) != -1) { // Set up command line parsing
		switch (character) { 
			case 's': // Change the maxChildren at a time
				allDigit = true;
				for (optargCount = 0; optargCount < strlen(optarg); optargCount++) { // Check if entire optarg is digit
					if (!isdigit(optarg[optargCount])) {
						allDigit = false;
						break;
					}
				}		
				if (allDigit) { // After that, check if it meets requirements to change max
					if ((atoi(optarg) <= 20) && (atoi(optarg) > 1)) {
						maxChild = atoi(optarg);
					}
				} else {
					errno = 22;
					perror("-s requires an argument");
					helpMenu();
					return EXIT_FAILURE;
				}
				continue;
			case 't': // Change the max allowed time
				allDigit = true;
				for (optargCount = 0; optargCount < strlen(optarg); optargCount++) { // Check if optarg is digit
					if(!isdigit(optarg[optargCount])) {
						allDigit = false;
						break;
					}
				}
				if (allDigit) { // See if we meet requirements to change timer
					if (atoi(optarg) > 0) {
						timeSec = atoi(optarg);
					}
				} else {
					errno = 22;
					perror("-t requires and argument");
					helpMenu();
					return EXIT_FAILURE;
				}
				continue;
			case 'h': // Show a help menu if confused
				helpMenu();
				continue;
			default: 
				printf("Input not valid\n");
				helpMenu();
				continue;
		}
	}
	if (optind < argc) { // Check next argument once getopt finishes
		datafile = fopen(argv[optind], "r+"); // Open the datafile
		if (!datafile) { // If it does not exist
			printf("Problem with the datafile\n");
			return 1;
		} else { // Create the random numbers in the file otherwise
			int randCount, random;
			srand((unsigned) time(&t));
			for (randCount = 0; randCount < INT_MAX; randCount++) { // Creation of random ints from [0, 256)
				random =(rand() % (255 + 1));
				fprintf(datafile, "%d\n", random);
			}
			rewind(datafile);
		}
	}
	

	allocateSM(); // Allocate memory for the whole program
	setupTimer(timeSec); // Set up the timer

	while(fscanf(datafile, "%d", &temp) == 1) { // Count number of items
		items++;
	}

	if (items % 2 == 1) { // If it is not divisible by 2, we want to add 1
		items++;
	}

	double depthD = 0; // Calculation of the depth
	depthD = log(items)/log(2);
	int depth = (int) ceil(depthD);
	sm->startingDepth = depth;
	rewind(datafile);

	while (count <= items && fscanf(datafile, "%d", &sm->intArray[count]) == 1) { // Store contents of file into an array
		count++;
	}
	
	fclose(datafile); // Close the datafile
	sm->total = maxChild;
	int index = 0;
	int i;
	int childCounter = 0;	

	while (depth > 0) { // Going through loop for every depth
		int depthIncrement = depthCounter(sm->startingDepth, depth); // Figure out new depth increment
		i = index;

		while (i < items) { // Go through until we are passed the number of items
			if (childCounter < maxChild) { // Spawn children based on max allowed
				spawnChild(childCounter++, i, depth);
				i = i + depthIncrement + depthIncrement;
			} else { // Else wait for children to finish and keep making more
				while(wait(NULL) > 0);
				childCounter = 0;
			}
		}
		while(wait(NULL) > 0); // Wait for all to end before going to next depth
		depth--;
		index = 0;
		childCounter = 0;
	}
	removeSM(); // Removing the shared memory once children are done
	return EXIT_SUCCESS;
}

void setupTimer(const int t) { // Creation of the timer
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = signalHandler;

	if (sigaction(SIGALRM, &action, NULL) != 0) { // In case of failed signal creation
		perror("Failed to set signal action for timer");
		exit(EXIT_FAILURE);	
	}

	struct itimerval timer;
	timer.it_value.tv_sec = t;
	timer.it_value.tv_usec = t;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	
	if (setitimer(ITIMER_REAL, &timer, NULL) != 0) { // In case of failed creation of the timer itself
		perror("Failed to set timer");
		exit(EXIT_FAILURE);
	}
}

void spawnChild(int childCounter, int i, int depth) { // Creation of children
	pid_t pid = fork(); // Fork child process

	if (pid == -1) { // Check to see if child process was created
		perror("Failed to create a child process for bin_adder");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) { // If it was created successfully
		flag = true;
		
		if (childCounter == 0) { // Set first as the group pid
			sm->pgid = getpid();
		}

		setpgid(0, sm->pgid);
		flag = false;
		
		char id[256], bufferi[3], bufferd[2];	// Creation of id through childcounter, and casting i and depth into chars
		sprintf(id, "%d", childCounter);	
		sprintf(bufferi, "%d", i);
		sprintf(bufferd, "%d", depth);
		execl("./bin_adder", "bin_adder", bufferi, bufferd, id, (char*) NULL); // Sending all information to bin_adder
		exit(EXIT_SUCCESS);
	}
}

void signalHandler(int s) { // Signal handler for master
	if (flag) { // In case flag is currently working
		sleep(1);
	}

	killpg(sm->pgid, s == SIGALRM ? SIGUSR1 : SIGTERM);

	while (wait(NULL) > 0); // Wait for all child processes to finish

	printf("Parent exiting...\n");	
	removeSM(); // Deallocate and destroy shared memory
	exit(EXIT_SUCCESS);
}

void helpMenu() { // Help menu
	printf("This program takes in a numbers of an array of size INT_MAX (specified in master.h) and uses the binary tree algorithm to add the integers in the array.\n");
	printf("The following are commands after inputting ./master : -s (followed by number of allowed processes between 0 and 20\n");
	printf("-t (followed by amount of time process is allowed), -h for help, and lastly the file to open\n");
	printf("-s and -t are required to take in an input, they will ignore the input if it is out of bounds or exit the program if it is the wrong type.\n");
	printf("The minimum requirement to run the program is to simply use ./master *name of file*, as the program has default values for -s and -t.\n");
}

