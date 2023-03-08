#ifndef MMM_H_
#define MMM_H_
#define MMM_RUNS 3

// globals (anything here would be shared with all threads) */
// I would declare the pointers to the matrices here (i.e., extern double **A, **B, **C),
// as well as the size of the matrices, etc.

/** struct to hold parameters to partialSum() **/
typedef struct thread_args {
  int tid;    // the given thread id (0, 1, ...)
  long begin; // where to begin summation
  long end;   // where to end summation
} thread_args;

double** mmm_init(double **matrix, int dimension, int fillWithRandom);
void mmm_reset(double **matrix, int dimension);
void mmm_freeup(double **matrix, int dimension);
void mmm_seq(double **matrix, int dimension);
void *mmm_par(void *args);
double mmm_verify(double **matrix_A, double **matrix_B, int dimension);
void mmm_print(char* mode, int numThreads, int dimension, double avgTime_seq, double avgTime_par);
void funny();

#endif /* MMM_H_ */
