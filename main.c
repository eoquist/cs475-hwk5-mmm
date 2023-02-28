#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[])
{
	if (argc <= 2){
		printf("Usage: ./mmmSol <mode> [num threads] <size>");
		exit(0);
	}

	char *mode = "X";
	int numThreads = 0;
	int dimension = 0;
	double **matrix_S;
	double **matrix_P;

	// start: stuff I want to clock
	double clockstart, clockend;
	clockstart = rtclock();

	if (strcmp(argv[0], "S") == 0){ // Sequential
		// Setup
		strcpy(mode,argv[0]);
		dimension = atoi(argv[1]);
		mmm_init(&matrix_S, dimension); // i hate this

	} else if (strcmp(argv[0], "P") == 0){ // Parallel
		if (argc < 4){
			printf("Error: parallel mode requires [num threads]\n");
			exit(0);
		}

		// Setup
		strcpy(mode,argv[0]);
		numThreads = atoi(argv[1]);
		dimension = atoi(argv[2]);
		mmm_init(&matrix_S, dimension); // i hate this
		mmm_init(&matrix_P, dimension); // i hate this so much

	} else {
		printf("Error: mode must be either S (sequential) or P (parallel)");
		exit(0);
	}

	// end: stuff I want to clock
	clockend = rtclock();
	// ========
	// mode: sequential
	// thread count: 1
	// size: 0
	// ========
	printf("Time taken: %.6f sec\n", (clockend - clockstart));

	// compare the matrices and then free them
	// mmm_verify();
	// mmm_freeup(matrix_S);
	// mmm_freeup(matrix_P);
	// exit(0);

	return 0;
}
