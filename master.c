#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

int main (int argc, char *argv[]) {
	int character, numChild = 20, timeSec = 100;
	while ((character = getopt(argc, argv, "s:t:h")) != -1) {
		switch (character) {
			case 's':
				printf("test s");
				if (isdigit(*optarg)) {
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
	return 0;
}

int helpMenu() {

}
