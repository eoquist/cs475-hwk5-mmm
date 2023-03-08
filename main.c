#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int NUM_THREADS = 0;
int DIMENSION = 0;
double **BASE_MATRIX;
double **BASE_MATRIX_TRANSPOSE;
double **MATRIX_PRODUCT_SEQUENTIAL;
double **MATRIX_PRODUCT_PARALLEL;

int main(int argc, char *argv[])
{
	if (argc <= 2){printf("Usage: ./mmm <mode> [num threads] <size>\n");exit(0);}
	
	double clockstart, clockend;
	char *mode = argv[1];

	// Sequential
	if (strcmp(argv[1], "S") == 0){ 
		double clockavg = 0;
		DIMENSION = abs(atoi(argv[2])); // im lazy and dont want to deal with negatives
		BASE_MATRIX = mmm_init(BASE_MATRIX, 1); 
		MATRIX_PRODUCT_SEQUENTIAL = mmm_init(MATRIX_PRODUCT_SEQUENTIAL, 0); 

		// determines average run time in seconds
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); 

			MATRIX_PRODUCT_SEQUENTIAL = mmm_seq();

			clockend = rtclock(); 
			if(i != 0){
				clockavg += clockend - clockstart;
			}
		}
		clockavg = clockavg / (double)MMM_RUNS;

		// print and free product matrix
		mmm_print(mode, clockavg, 0.0);
		mmm_freeup(BASE_MATRIX);
		mmm_freeup(MATRIX_PRODUCT_SEQUENTIAL);
	} 
	// Parallel
	else if (strcmp(argv[1], "P") == 0){ 
		if (argc < 4){printf("Error: parallel mode requires [num threads]\n");exit(0);}

		double clockavg_seq = 0, clockavg_par = 0;
		NUM_THREADS = abs(atoi(argv[2])); 
		DIMENSION = abs(atoi(argv[3])); // abs(olutely)
		BASE_MATRIX = mmm_init(BASE_MATRIX, 1); 
		MATRIX_PRODUCT_SEQUENTIAL = mmm_init(MATRIX_PRODUCT_SEQUENTIAL, 0); 
		MATRIX_PRODUCT_PARALLEL = mmm_init(MATRIX_PRODUCT_PARALLEL, 0); 
		
		// determines average run time in seconds
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); 
			MATRIX_PRODUCT_SEQUENTIAL = mmm_seq();
			clockend = rtclock(); 

			if(i != 0){
				clockavg_seq += clockend - clockstart;
			}
		}
		for(int i = 0; i < MMM_RUNS; i++){ 
			clockstart = rtclock(); 

			// prepare thread arguments
			thread_args *args = (thread_args*) malloc(NUM_THREADS * sizeof(thread_args));
			int partition_size = DIMENSION / NUM_THREADS;

			for (int i = 0; i < NUM_THREADS; i++) {
				args[i].tid = i;
				args[i].start_row = i * (DIMENSION / NUM_THREADS);
				args[i].end_row = args[i].start_row  + partition_size;
				if (i == NUM_THREADS - 1) {
					args[i].end_row = DIMENSION - 1;
    			}
			}

			// allocate space to hold threads
			pthread_t *threads = (pthread_t*) malloc(NUM_THREADS * sizeof(pthread_t));
			for (int i = 0; i < NUM_THREADS; i++) {
				pthread_create(&threads[i], NULL, mmm_par, &args[i]);
			}

			// wait for threads to finish
			for (int i = 0; i < NUM_THREADS; i++) {
				pthread_join(threads[i], NULL);
			}
			
			// clean up
			free(threads);
			threads = NULL;
			free(args);
			args = NULL;

			clockend = rtclock(); 
			if(i != 0){
				clockavg_par += clockend - clockstart;
			}
		}
		clockavg_seq = clockavg_seq / (double)(MMM_RUNS-1);
		clockavg_par = clockavg_par / (double)(MMM_RUNS-1);

		// print stats then compare
		mmm_print(mode, clockavg_seq, clockavg_par);
		double error = mmm_verify(MATRIX_PRODUCT_SEQUENTIAL, MATRIX_PRODUCT_PARALLEL); 
		printf("Verifying... largest error between parallel and sequential matrix: %f\n", error);

		// free the product matrices
		mmm_freeup(BASE_MATRIX);
		mmm_freeup(MATRIX_PRODUCT_SEQUENTIAL);
		mmm_freeup(MATRIX_PRODUCT_PARALLEL);

	} 
	else {
		printf("Error: mode must be either S (sequential) or P (parallel)\n");
	}

	funny();
	return 0;
}
