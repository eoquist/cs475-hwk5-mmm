#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[])
{
	double clockstart, clockend;
	clockstart = rtclock(); // start clocking
	// start: stuff I want to clock

	if (argc == 0)
	{
		printf("die\n");
		exit(0);
	}
	char mode = argv[1];
	int numThreads;
	int dimension;
	// if first arg is S --> make sure theres only one whole number
	// if the first arg is P --> next arg is number of threads, then the arg after that is the matrix dimension

	// dynamically allocate matrix
	// malloc a size N array of pointers to ints
	int **matrix;
	matrix = (int **)malloc(sizeof(int *) * dimension);
	for (int i = 0; i < N; i++)
	{
		matrix[i] = (int *)malloc(sizeof(int) * dimension;)
	}

	// free
	for (int i = 0; i < N; i++)
	{
		free(matrix[i]);
		matrix[i] = NULL; // remove dangling pointer
	}
	free(matrix);
	matrix = NULL;

	// end: stuff I want to clock
	clockend = rtclock(); // stop clocking
	printf("Time taken: %.6f sec\n", (clockend - clockstart));

	return 0;
}
