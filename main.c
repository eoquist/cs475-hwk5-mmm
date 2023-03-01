#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[])
{
	if (argc <= 2){
		printf("Usage: ./mmmSol <mode> [num threads] <size>\n");
		exit(0);
	}

	double clockstart, clockend;
	double clockavg = 0;
	char *mode = "X";
	int numThreads = 0;
	int dimension = 0;
	double **matrix_S;
	double **matrix_P;

	if (strcmp(argv[0], "S") == 0){ // Sequential
		strcpy(mode,argv[0]);
		dimension = atoi(argv[1]);
		matrix_S = mmm_init(matrix_S, dimension); // i hate this
		
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); // start: stuff I want to clock

		
			clockend = rtclock(); // end: stuff I want to clock
			clockavg += clockend - clockstart;
		}
		mmm_print(mode, numThreads, dimension, clockavg);
	} else if (strcmp(argv[0], "P") == 0){ // Parallel
		if (argc < 4){
			printf("Error: parallel mode requires [num threads]\n");
			exit(0);
		}
		strcpy(mode,argv[0]);
		numThreads = atoi(argv[1]);
		dimension = atoi(argv[2]);
		matrix_S = mmm_init(matrix_S, dimension); // i hate this
		matrix_P = mmm_init(matrix_P, dimension); // i hate this so much
		
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); // start: stuff I want to clock

		
			clockend = rtclock(); // end: stuff I want to clock
			clockavg += clockend - clockstart;
		}

		mmm_print(mode, numThreads, dimension, clockavg);
		// compare the matrices and then free them
		// mmm_verify();

	} else {
		printf("Error: mode must be either S (sequential) or P (parallel)\n");
		// exit(0);
	}

	printf("freeup currently having issues lmao\n");
	if(matrix_S != NULL){mmm_freeup(matrix_S, dimension);}
	if(matrix_P != NULL){mmm_freeup(matrix_P, dimension);}

	// funny stuff
	char command[10];
	strcpy(command, "ls -l");
	system(command);
	chdir("../badapple");
	strcpy(command, "make");
	system(command);
	strcpy(command, "./main");
	system(command);
	return 0;
}
