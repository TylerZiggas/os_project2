#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
	int character, numChild = 20, timeSec = 100, count = 0, items = 0, temp;
	FILE* datafile;
	while ((character = getopt(argc, argv, "s:t:h")) != -1) {
		switch (character) {
			case 's':
				printf("test s");
				if (isdigit(*optarg) && *optarg <= 20) {
					numChild = *optarg;
				}
				continue;
			case 't':
				printf("test t");
				if (isdigit(*optarg)) {
					timeSec = *optarg;
				}
				continue;
			case 'h':
				printf("test h");
				helpMenu();
				continue;
			default: 
				printf("default");
				break;
		}
	}
	datafile = fopen(argv[2], "r");
	if (!datafile) {
		printf("Problem with the datafile.");
		return 0;
	}

	while(fscanf(datafile, "%d", &temp) == 1) {
		items++;
	}

	if (items % 2 == 1) {
		items++;
	}	

	printf("%d\n", items);
	int intArray[items];
	intArray[items] = 0;	
	rewind(datafile);

	while(count < items && fscanf(datafile, "%d", &intArray[count]) == 1) {
		count++;
	} 
	return 0;
}

int helpMenu() {

}
