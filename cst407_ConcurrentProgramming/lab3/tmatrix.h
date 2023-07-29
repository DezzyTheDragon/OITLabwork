#pragma once
//#include <atomic>
#include <pthread.h>
//Have each thread calculate just one cell of the final matrix
//Ex:   Thread1(matrixA_row 1, matrixB_col 1, matrixC 1_1)
//      Thread2(matrixA_row 1, matrixB_col 2, matrixC 1_2)
//      etc...


//Maybe atomic global variables to track what needs to be solved in matrix C
//std::atomic<int> matrixC_row = 0;
//std::atomic<int> matrixC_col = 0;

//Using a lock to make sure nothing sneeks in between the check and a modify
//pthread_mutex_t matrixC_lock;
//int matrixC_row = 0;
//int matrixC_col = 0;

typedef struct{
    double *matrixA; 
    double *matrixB; 
    double *matrixC;
    int a_cols;
    int a_rows;
    int b_col;
} matrixData;

// Threaded function that is responsible for figuring out what portion of
// the matrix needs to be solved. Computes left to right top to bottom
// Pass pointers to each matrix and the size data of the matrix
void *Threaded_Matrix_Scheduler(void *data);

// Solves for one cell in matrix c
// Requires matrix A and B to read from, Matrix C to store the answer
// the size of the matracies, the target cell that is being solved
void Threaded_Matrix_Multiply_Cell(double *a, double *b, double *c,
        int a_cols, int b_cols, int dest_row, int dest_col);
