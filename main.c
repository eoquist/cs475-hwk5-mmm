#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[])
{
	if (argc <= 2){printf("Usage: ./mmm <mode> [num threads] <size>\n");exit(0);}
	
	// move these to mmm_h ? global declarations??? needed for multithreading?
	double clockstart, clockend;
	char *mode = argv[1];
	int numThreads = 0;
	int dimension = 0;
	double **matrix_S;
	double **matrix_P;

	// Sequential
	if (strcmp(argv[1], "S") == 0){ 
		double clockavg = 0;
		dimension = atoi(argv[2]); // !!! ensure not negative 
		matrix_S = mmm_init(matrix_S, dimension, 1); 
		double **matrix_product_S = mmm_init(matrix_product_S, dimension, 0); 

		// Ugly code that determines average run time in seconds
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); 
			mmm_seq(matrix_S, dimension);
			clockend = rtclock(); 
			clockavg += clockend - clockstart;
		}
		clockavg = clockavg / (double)MMM_RUNS;

		// print and free product matrix
		mmm_print(mode, numThreads, dimension, clockavg, 0.0);
		mmm_freeup(matrix_product_S,dimension);
	} 
	// Parallel
	else if (strcmp(argv[1], "P") == 0){ 
		if (argc < 4){printf("Error: parallel mode requires [num threads]\n");exit(0);}

		double clockavg_seq = 0, clockavg_par = 0;
		numThreads = atoi(argv[2]); // !!! ensure not negative 
		dimension = atoi(argv[3]); // !!! ensure not negative 
		matrix_S = mmm_init(matrix_S, dimension, 1); 
		matrix_P = mmm_init(matrix_P, dimension, 1); 
		double **matrix_product_S = mmm_init(matrix_product_S, dimension, 0); 
		double **matrix_product_P = mmm_init(matrix_product_P, dimension, 0); 
		
		// Ugly code that determines average run time in seconds
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); 
			mmm_seq(matrix_S, dimension);
			clockend = rtclock(); 
			clockavg_seq += clockend - clockstart;
		}
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); 
			// EVERYTHING IMPORTANT
			// remember it returns void args
			// mmm_par();

			// double ** partial_product = (double**) malloc(numThreads * sizeof(double*));

			//  // prepare thread arguments
			// thread_args *args = (thread_args*) malloc(NUM_THREADS * sizeof(thread_args));
			// for (int i = 0; i < NUM_THREADS; i++) {
			// 	args[i].tid = i;
			// 	args[i].begin = i * N / NUM_THREADS + 1;
			// 	args[i].end = (i + 1) * N / NUM_THREADS;
			// }

			// // allocate space to hold threads
			// pthread_t *threads = (pthread_t*) malloc(NUM_THREADS * sizeof(pthread_t));
			// for (int i = 0; i < NUM_THREADS; i++) {
			// 	pthread_create(&threads[i], NULL, partialSum, &args[i]);
			// }

			clockend = rtclock(); 
			clockavg_par += clockend - clockstart;
		}

		clockavg_seq = clockavg_seq / (double)MMM_RUNS;
		clockavg_par = clockavg_par / (double)MMM_RUNS;

		// print stats then compare
		mmm_print(mode, numThreads, dimension, clockavg_seq, clockavg_par);
		mmm_verify(matrix_S,matrix_P,dimension); 

		// free the product matrices
		mmm_freeup(matrix_product_S, dimension);
		mmm_freeup(matrix_product_P, dimension);

	} 
	else {
		printf("Error: mode must be either S (sequential) or P (parallel)\n");
	}

	if(strcmp(mode,"S") == 0){mmm_freeup(matrix_S, dimension);}
	else if(strcmp(mode,"P") == 0){mmm_freeup(matrix_S, dimension);mmm_freeup(matrix_P, dimension);}
	funny();
	return 0;
}
