#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

//static enum state{idle, want_in, in_cs};
//static int turn;
//static int n;
//static enum state flag[20];
//static int sum;

int main (int argc, char *argv[]) {
	extern int n;
	int character, shmid, numChild = 20, timeSec = 100, count = 0, items = 0, temp, currentChild = 0, i, j, flagSet, status;
	FILE* datafile;
	char *shm, *ptr;
	key_t key;

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
		}
	}
	
	if ((key = ftok("./", 5393)) == -1) {
		perror("key");
		return 1;	
	}

	while(fscanf(datafile, "%d", &temp) == 1) {
		items++;
	}

	if (items % 2 == 1) {
		items++;
	}	

	int intArray[items];
	intArray[items] = 0;
	printf("%d\n", items);

	rewind(datafile);
	shmid = shmget(key, items*2, IPC_CREAT | 0600);	
	if (shmid < 0) {
		perror("shmget");
		return 1;
	}
	
	shm = shmat(shmid, NULL, 0);
	if (shm == (char *) -1) {
		perror("shmat");
		return 1;
	}

	while (count <= items && fscanf(datafile, "%d", &intArray[count]) == 1) {
		count++;
	}

	


	char ok = 's';
	for (;currentChild <= numChild; currentChild++) {
		fork();
		execl("./bin_adder", "bin_adder", (char *) NULL);
	}

//	do {
//		do {
//			flag[flagSet] = want_in;
//			j = turn;
//			while (j != 1 ) 
//				j = (flag[j] != idle ? turn : (j + 1) % n);
//			flag[j] = in_cs;
//			for (j = 0; j < numChild; j++)
//				if(( j != flagSet ) && (flag[j] == in_cs))
//					break;
//		} while (( j < n ) || ( turn != flagSet && flag[turn] != idle));
//
//		turn = flagSet;
//				
		// critical section
//		
//		j = (turn + 1) % n;
//		while (flag[j] == idle) 
//			j = (j + 1) % n;
//
//		turn = j;
//		flag[flagSet] = idle;
//	
//	} while (1);
	
	fclose(datafile); 
	if (shmdt(shm) == -1) {
		perror("Failed to detach");
		return 1;
	}
	
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("Failed to remove");
		return 1;
	}

	return 0;
}

int helpMenu() {

}
