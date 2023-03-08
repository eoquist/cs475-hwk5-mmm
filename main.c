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
	
	// move these to mmm_h ? global declarations??? needed for multithreading?
	double clockstart, clockend;
	char *mode = argv[1];

	// Sequential
	if (strcmp(argv[1], "S") == 0){ 
		double clockavg = 0;
		DIMENSION = atoi(argv[2]); // !!! ensure not negative 
		BASE_MATRIX = mmm_init(BASE_MATRIX, 1); 
		MATRIX_PRODUCT_SEQUENTIAL = mmm_init(MATRIX_PRODUCT_SEQUENTIAL, 0); 

		// determines average run time in seconds
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); 

			MATRIX_PRODUCT_SEQUENTIAL = mmm_seq();

			clockend = rtclock(); 
			clockavg += clockend - clockstart;
		}
		clockavg = clockavg / (double)MMM_RUNS;

		// print and free product matrix
		print_matrix(MATRIX_PRODUCT_SEQUENTIAL); // !!!

		mmm_print(mode, clockavg, 0.0);
		mmm_freeup(BASE_MATRIX);
		mmm_freeup(MATRIX_PRODUCT_SEQUENTIAL);
	} 
	// Parallel
	else if (strcmp(argv[1], "P") == 0){ 
		if (argc < 4){printf("Error: parallel mode requires [num threads]\n");exit(0);}

		double clockavg_seq = 0, clockavg_par = 0;
		NUM_THREADS = atoi(argv[2]); // !!! ensure not negative 
		DIMENSION = atoi(argv[3]); // !!! ensure not negative 
		BASE_MATRIX = mmm_init(BASE_MATRIX, 1); 
		MATRIX_PRODUCT_SEQUENTIAL = mmm_init(MATRIX_PRODUCT_SEQUENTIAL, 0); 
		MATRIX_PRODUCT_PARALLEL = mmm_init(MATRIX_PRODUCT_PARALLEL, 0); 
		
		// determines average run time in seconds
		for(int i = 0; i < MMM_RUNS; i++){
			clockstart = rtclock(); 
			MATRIX_PRODUCT_SEQUENTIAL = mmm_seq();
			clockend = rtclock(); 
			clockavg_seq += clockend - clockstart;
		}
		for(int i = 0; i < 1; i++){ // MMM_RUNS
			clockstart = rtclock(); 

			// prepare thread arguments
			thread_args *args = (thread_args*) malloc(NUM_THREADS * sizeof(thread_args));

			int partition_size = DIMENSION / NUM_THREADS;
  			int remainder = DIMENSION % NUM_THREADS;
			
			// definitely janky start/end here and in mmm_par but we don't think about that :)
			for (int i = 0; i < NUM_THREADS; i++) {
				args[i].tid = i;
				args[i].start_row = i * (DIMENSION / NUM_THREADS);
				args[i].end_row = args[i].start_row  + partition_size;
				if (i == NUM_THREADS - 1) {
					args[i].start_row += remainder;
    			}
				printf("thread %d [%d,%d]\n", i, args[i].start_row, args[i].end_row);
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
			
			// clean up dynamically allocated memory
			free(threads);
			threads = NULL;

			free(args);
			args = NULL;

			clockend = rtclock(); 
			clockavg_par += clockend - clockstart;
		}

		// printf("BASE_MATRIX\n");
		// print_matrix(BASE_MATRIX); // !!!
		// printf("====================\n");

		printf("MATRIX_PRODUCT_SEQUENTIAL\n");
		print_matrix(MATRIX_PRODUCT_SEQUENTIAL); // !!!
		printf("====================\n");

		printf("MATRIX_PRODUCT_PARALLEL\n");
		print_matrix(MATRIX_PRODUCT_PARALLEL); // !!!
		printf("====================\n");

		clockavg_seq = clockavg_seq / (double)MMM_RUNS;
		clockavg_par = clockavg_par / (double)MMM_RUNS;

		// print stats then compare
		mmm_print(mode, clockavg_seq, clockavg_par);
		double error = mmm_verify(MATRIX_PRODUCT_SEQUENTIAL, MATRIX_PRODUCT_PARALLEL); 
		printf("error is %f\n", error);

		// free the product matrices
		mmm_freeup(BASE_MATRIX);
		mmm_freeup(MATRIX_PRODUCT_SEQUENTIAL);
		mmm_freeup(MATRIX_PRODUCT_PARALLEL);

	} 
	else {
		printf("Error: mode must be either S (sequential) or P (parallel)\n");
	}

	// funny();
	return 0;
}
