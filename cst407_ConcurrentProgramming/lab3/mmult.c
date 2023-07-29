//********************************************
// Matrix multiply test harness
//
// Author: Phil Howard

#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "matrix.h"     //Used for the matrix print function
#include "tmatrix.h"
#include "usec.h"

#ifndef _POSIX_C_SOURCE
#error "No POSIX C source"
#endif

#if _POSIX_C_SOURCE < 2
#error "old POSIX"
#endif

static int g_a_rows = 3;
static int g_a_cols = 2;
static int g_b_cols = 3;
static int g_n_threads = 1;
static int g_print_level = 0;
static int g_alg = 0;

//**************************************************************
// Handle command line args. Store results in file scope variables.
// The meaning of the algorithm parameter is left up to the student
static void proc_args(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "ha:r:c:b:t:p:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf("%s -a <alg> -r <rows> -c <cols> -b <b cols> "
                       "-t <threads> -p <print level>\n", argv[0]);
                exit(1);
                break;
            case 'a':
                g_alg = atoi(optarg);
                break;
            case 'r':
                g_a_rows = atoi(optarg);
                break;
            case 'c':
                g_a_cols = atoi(optarg);
                break;
            case 'b':
                g_b_cols = atoi(optarg);
                break;
            case 't':
                g_n_threads = atoi(optarg);
                break;
            case 'p':
                g_print_level = atoi(optarg);
                break;
        }
    }
}

//*************************************************************
// Main function for matrix multiply test harness
int main(int argc, char **argv)
{
    int a_cols, a_rows;
    int b_cols, b_rows;
    int c_cols, c_rows;

    proc_args(argc, argv);

    a_cols = g_a_cols;
    a_rows = g_a_rows;

    b_cols = g_b_cols;
    b_rows = a_cols;

    c_rows = a_rows;
    c_cols = b_cols;

    // Note: new versions of C allow the declaration of arrays with non-static
    // sizes (as shown in the following lines). However, allocating such large
    // arrays on the stack may cause the program to run out of stack space 
    // (which it does for "interesting" size matrices. This is why I not using
    // this simple approach
    //
    // double a[a_rows][a_cols];
    // double b[a_rows][b_cols];
    // double c[c_rows][c_cols];

    // Note (continued): The following code is not as clean as the code above,
    // but it works, and the data is not on the stack. It is also more friendly
    // to old-school compilers that don't like non-static sized arrays.
    typedef double a_type[a_cols];
    typedef double b_type[b_cols];
    typedef double c_type[c_cols];

    a_type *a = (a_type*)malloc(sizeof(double)*a_cols*a_rows);
    b_type *b = (b_type*)malloc(sizeof(double)*b_cols*b_rows);
    c_type *c = (c_type*)malloc(sizeof(double)*c_cols*c_rows);

    // Initialize the A matrix
    double value = 0;
    for (int ii=0; ii<a_rows; ii++)
    {
        for (int jj=0; jj<a_cols; jj++)
        {
            a[ii][jj] = value++;
        }
    }

    // Initialize the B matrix
    for (int ii=0; ii<b_rows; ii++)
    {
        for (int jj=0; jj<b_cols; jj++)
        {
            b[ii][jj] = value++;
        }
    }

    // conditionally print the source matrixes
    if (g_print_level > 1) 
    {
        //printf("No matrix print implemented\n");
        Matrix_Print(stdout, &a[0][0], a_rows, a_cols, "%7.2f ");
        printf("\n");

        Matrix_Print(stdout, &b[0][0], b_rows, b_cols, "%7.2f ");
        printf("\n");
    }

    // Note: if you add/change algorithms, you will have to update this 
    // switch statement.
    /*
    switch (g_alg)
    {
        case 0:
            printf("Matrix Multiply\n");
            Matrix_Multiply(&a[0][0],&b[0][0],&c[0][0], 
                            a_cols, a_rows, b_cols);
            break;
        case 1:
            printf("Array Indexing\n");
            Matrix_Multiply_Array(&a[0][0],&b[0][0],&c[0][0], 
                            a_cols, a_rows, b_cols);
            break;
        default:
            printf("Unknown algirithm\n");
            break;
    }
    */

    //TODO: spin up threads here and give the Threaded_Matrix_Scheduler
    
    pthread_t threads[g_n_threads];
    matrixData data;
    data.matrixA = &a[0][0];
    data.matrixB = &b[0][0];
    data.matrixC = &c[0][0];
    data.a_cols = a_cols;
    data.a_rows = a_rows;
    data.b_col = b_cols;
    int64_t start = 0;
    int64_t stop = 0;
    switch(g_alg)
    {
        case 0:
            for(int t = 0; t < g_n_threads; t++)
            {
                pthread_create(&threads[t], NULL, Threaded_Matrix_Scheduler, (void *)&data);
            }

            //join all the threads
            for(int j = 0; j < g_n_threads; j++)
            {
                pthread_join(threads[j], NULL);
            }
            break;
        case 1:
            start = usecs();
            Matrix_Multiply(&a[0][0], &b[0][0], &c[0][0], a_cols, a_rows, b_cols);
            stop = usecs();
            printf("Time for single threaded matrix multiply %1.6f\n", 
                    (stop - start) / 1000000.0);
            break;
    }
    

    /*
    //TEST CODE
    //Known correct
    Matrix_Multiply(&a[0][0], &b[0][0], &c[0][0], a_cols, a_rows, b_cols);
    printf("Phils Matrix solver solution\n");
    Matrix_Print(stdout, &c[0][0], c_rows, c_cols, "%7.2f");
    //My Matrix solve code
    Threaded_Matrix_Scheduler(&a[0][0], &b[0][0], &c[0][0], a_cols, a_rows, b_cols);
    printf("My Matrix solver solution\n");
    Matrix_Print(stdout, &c[0][0], c_rows, c_cols, "%7.2f");
    */

    // conditionally print the answer
    if (g_print_level > 0) 
    {
        //printf("No matrix print implemented\n");
        Matrix_Print(stdout, &c[0][0], c_rows, c_cols, "%7.2f ");
    }

    return 0;
}


