#ifndef MMM_H_
#define MMM_H_
#define MMM_RUNS 4

// globals (anything here would be shared with all threads) */
// I would declare the pointers to the matrices here (i.e., extern double **A, **B, **C),
// as well as the size of the matrices, etc.

/** struct to hold parameters to partial_product() **/
typedef struct thread_args {
  int tid;    // the given thread id (0, 1, ...)
  int start_row;
  int end_row; 
} thread_args;

extern int NUM_THREADS;
extern int DIMENSION;
extern double **BASE_MATRIX;
extern double **BASE_MATRIX_TRANSPOSE;
extern double **MATRIX_PRODUCT_SEQUENTIAL;
extern double **MATRIX_PRODUCT_PARALLEL;

double **mmm_init(double **matrix, int fillWithRandom);
void mmm_reset(double **matrix);
void mmm_freeup(double **matrix);
void mmm_transpose();
double **mmm_seq();
void *mmm_par(void *args_void);
double mmm_verify(double **matrix_A, double **matrix_B);
void mmm_print(char* mode, double avgTime_seq, double avgTime_par);
void print_matrix(double **matrix);
void funny();

#endif /* MMM_H_ */
