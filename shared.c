#include "master.h"

/* Creates or clears a file given its path "path". */
void touchFile(char* path) {
	FILE* newFile = fopen(path, "w");
		if (newFile == NULL) { 
			perror("Failed to touch file");
			exit(EXIT_FAILURE);
		}
	fclose(newFile);
}

/* Outputs a formatted string "fmt" to a file given its path "path". */
void logOutput(char* path, char* fmt, ...) {
	FILE* fp = fopen(path, "a+");
	
	if (fp == NULL) {
		perror("Failed to open file for logging output");
		exit(EXIT_FAILURE);
	}
		
	int n = 4096;
	char buf[n];
	va_list args;
	
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	
	fprintf(fp, buf);
	fclose(fp);
}

/* Allocates and clears shared memory. */
void allocateSPM() {
	//logOutput("output.log", "%s: Allocating shared memory\n", getFormattedTime());
	attachSPM();
	releaseSPM();
	attachSPM();
}

/* Attaches to shared memory. */
void attachSPM() {
	spmKey = ftok("Makefile", 'p');
	if ((smID = shmget(spmKey, sizeof(struct SharedProgramMemory), IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
		perror("Failed to allocate shared memory for SPM");
		exit(EXIT_FAILURE);
	} else spm = (struct SharedProgramMemory*) shmat(smID, NULL, 0);
}

/* Releases shared memory. */
void releaseSPM() {
	if (spm != NULL) if (shmdt(spm)) {
		 perror("Failed to release SPM");
		exit(EXIT_FAILURE);
	}
}

/* Deletes shared memory. */
void deleteSPM() {
	if (smID > 0) if (shmctl(smID, IPC_RMID, NULL) < 0) { 
		perror("Failed to delete SPM");
		exit(EXIT_FAILURE);
	}
}

/* Releases and deletes shared memory. */
void removeSPM() {
	//logOutput("output.log", "%s: Removing shared memory\n", getFormattedTime());
	releaseSPM();
	deleteSPM();
}

/* Outputs a formatted string "fmt" to a file given its path "path". */
//void logOutput(char* path, char* fmt, ...) {
//	FILE* fp = fopen(path, "a+");
//	
//	if (fp == NULL) {
//		 perror("Failed to open file for logging output");
//		exit(EXIT_FAILURE);
//	}
//	int n = 4096;
//	char buf[n];
//	va_list args;
//	
//	va_start(args, fmt);
//	vsprintf(buf, fmt, args);
//	va_end(args);
//	
//	fprintf(fp, buf);
//	fclose(fp);
//}

/* Returns the current time on the system. */
char* getFormattedTime() {
	char* formattedTime = malloc(FORMATTED_TIME_SIZE * sizeof(char));
	time_t now = time(NULL);
	strftime(formattedTime, FORMATTED_TIME_SIZE, FORMATTED_TIME_FORMAT, localtime(&now));
	return formattedTime;
}
