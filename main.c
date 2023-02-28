#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[])
{
	double clockstart, clockend;
	clockstart = rtclock();

	// start: stuff I want to clock
	int* numThreads = (int *)calloc(1, sizeof(int *));
	int* dimension = (int *)calloc(1, sizeof(int *));
	double **matrix;

	switch (argc)
	{
	case 3: // 2 params -- Sequential
		printf("first param: .%s.\n", argv[0]);
		printf("second param: .%s.\n", argv[1]);
		// if (argv[0]){
		// 	printf("Error: mode must be either S (sequential) or P (parallel)");
		// }
		break;
	case 4: // 3 params -- Parallel
		break;
	default: // 0 params, 1 param, or too many params
		printf("Usage: ./mmmSol <mode> [num threads] <size>");
		exit(0);
		break;
	}

	// dynamically allocate matrix
	//
	//
	//
	//
	//
	//

	// free
	for (int i = 0; i < dimension; i++)
	{
		free(matrix[i]);
		matrix[i] = NULL;
	}
	free(matrix);
	matrix = NULL;

	// end: stuff I want to clock
	clockend = rtclock(); // stop clocking
	printf("Time taken: %.6f sec\n", (clockend - clockstart));

	return 0;
}
