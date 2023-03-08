#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "mmm.h"
// funny
#include <unistd.h>

/**
 * Allocate and initialize the matrices on the heap. Populate
 * the input matrices with random integers from 0 to 99
 * @param matrix pointer to a 2D array
 * @param dimension integer width of a square matrix
 */
double **mmm_init(double **matrix, int dimension, int fillWithRandom)
{
	matrix = (double **)calloc(dimension, sizeof(double *));
	for (int i = 0; i < dimension; i++)
	{
		matrix[i] = (double *)calloc(dimension, sizeof(double));
		if(fillWithRandom == 1){
			for (int j = 0; j < dimension; j++)
			{
				matrix[i][j] = (double)(rand() % 100);
			}
		}
	}
	return matrix;
}

/**
 * Reset a given matrix to zeroes
 * @param matrix pointer to a 2D arrayF
 * @param dimension integer width of a square matrix
 */
void mmm_reset(double **matrix, int dimension)
{
	printf("mmm_reset started\n");
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; i < dimension; j++)
		{
			matrix[i][j] = 0;
		}
	}
	printf("mmm_reset ended\n");
}

/**
 * Free up memory allocated to all matrices
 * @param matrix pointer to a 2D array
 * @param dimension integer width of a square matrix
 */
void mmm_freeup(double **matrix, int dimension)
{
	for (int i = 0; i < dimension; i++)
	{
		free(matrix[i]);
		matrix[i] = NULL;
	}
	free(matrix);
	matrix = NULL;
}

/**
 * Sequential MMM
 */
void mmm_seq(double **matrix, int dimension)
{
	double **matrix_product  = mmm_init(matrix_product, dimension, 0);
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            double sum = 0.0;
            for (int k = 0; k < dimension; k++) {
				// k = col of 1st, row of 2nd
                sum += matrix[i][k] * matrix[k][j]; 
            }
            matrix_product[i][j] = sum;
        }
    }
	// decide whether mmm_seq returns a product or not -- or a clock time?
	mmm_freeup(matrix_product,dimension); // freeing it up in the meantime though
}

/**
 * Parallel MMM
 */
void *mmm_par(void *args)
{
	printf("mmm_par started\n");
	// TODO - code to perform parallel MMM



	printf("mmm_par ended\n");
	return args;
}

/**
 * Verifies the correctness between the matrix product generated by
 * the sequential run and the parallel run using fabs().
 * @param matrix_A pointer to a 2D array
 * @param matrix_B pointer to a 2D array
 * @return the largest error between two corresponding elements
 * in the result matrices
 */
double mmm_verify(double **matrix_A, double **matrix_B, int dimension)
{
	double error = 0.0;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            error += fabs(matrix_A[i][j] - matrix_B[i][j]);
        }
    }
    return error;
	
}

/**
 * Final print before program termination. This should print all relevant stats for the user.
*/
void mmm_print(char* mode, int numThreads, int dimension, double avgTime_seq, double avgTime_par){
	// this is super cool and not redundant at all :)
	char *mode_fullname; 
	if(strcmp(mode,"S")==0){
		mode_fullname = malloc(strlen("sequential") + 1);
		strcpy(mode_fullname,"sequential");
	}
	else if(strcmp(mode,"P")==0){
		mode_fullname = malloc(strlen("parallel") + 1);
		strcpy(mode_fullname,"parallel");
	}

	printf("========\n");
	printf("mode: %s\n", mode_fullname);
	printf("thread count: %d\n", numThreads);
	printf("size: %d\n", dimension);
	printf("========\n");
	printf("Sequential Time (avg of %d runs): %.6f sec\n", MMM_RUNS, avgTime_seq);
	if(strcmp(mode,"P")==0){
		printf("Parallel Time (avg of %d runs): %.6f sec", MMM_RUNS, avgTime_par);
		double speedup = avgTime_seq / avgTime_par;
		printf("Speedup: %.6f sec", speedup);
	}
	free(mode_fullname);
	mode_fullname = NULL;
}

/**
 * Assumed that the badapple repo is saved in the same directory as 
 * the one holding 'cs475-hwk5-mmm'
 * @author of repo being used https://github.com/leafyao8621/badapple
*/
void funny(){
	// ask david about running make within a make file?
	chdir("../badapple");
	printf("======================================\n");
	printf("Hope you enjoy!");
	system("make -s");
	sleep(3);
	
	chdir("../cs475-hwk5-mmm");
	int ret_val;
	ret_val = system("PULSE_SERVER=localhost mpg123 mystery.mp3 &");
	if(0 != ret_val){perror("There was an error");}
	
	sleep(3);
	chdir("../badapple");
	system("./main");
}