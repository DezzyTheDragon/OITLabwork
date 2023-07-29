#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include "matrix.h"

void MPI_Matrix_Multiply(double *a, double *b, double *c, 
        int a_cols, int a_rows, int b_cols)
{
    //Get data ready
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int rowsAssigned = a_rows / world_size;
    //int offset = rank * rowsAssigned;
    int dataCount = a_cols * (a_rows / world_size);
    int cDataCount = b_cols * (a_rows / world_size);

    double temp_value;
    typedef double a_type[a_cols];
    typedef double b_type[b_cols];
    typedef double c_type[b_cols];
    a_type *a_value;
    b_type *b_value;

    a_type *a_buffer = malloc(sizeof(double) * dataCount);
    c_type *c_buffer = malloc(sizeof(double) * cDataCount);

    a_value = (a_type *)a;
    b_value = (b_type *)b;

    /*
    //DEBUG: check the matrix is correct
    if(rank == 0)
    {
        c_type *c_single = (c_type*)malloc(sizeof(double)*a_rows*b_cols);
        Matrix_Multiply(a, b, &c_single[0][0], a_cols, a_rows, b_cols);
        printf("Correct Matrix C\n");
        Matrix_Print(stdout, &c_single[0][0], a_rows, b_cols, "%7.2f ");
        printf("\n");
        free(c_single);
    }
    //===================================
    */

    //Distribute data to nodes
    MPI_Scatter((void *)&a_value[0][0], dataCount, MPI_DOUBLE, (void *)a_buffer, dataCount,
            MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast((void *)&b_value[0][0], b_cols * a_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /*
    //DEBUG: Check distributed data is correct
    printf("Rank %d Matrix A\n", rank);
    Matrix_Print(stdout, &a_buffer[0][0], rowsAssigned, a_cols, "%7.2f ");
    printf("\nRank %d Matrix B\n", rank);
    Matrix_Print(stdout, &b_value[0][0], a_rows, b_cols, "%7.2f ");
    printf("\n");
    //==============================
    */

    //Do the heckin matrix multiply
    for(int r = 0; r < rowsAssigned; r++)
    {
        for(int bcol = 0; bcol < b_cols; bcol++)
        {
            temp_value = 0;
            for(int acol = 0; acol < a_cols; acol++)
            {
                temp_value += a_buffer[r][acol] * b_value[acol][bcol];
            }
            c_buffer[r][bcol] = temp_value;
        }
    }

    /*
    //DEBUG: Check the result of a given slice
    printf("Rank %d Matrix C Slice\n", rank);
    Matrix_Print(stdout, &c_buffer[0][0], rowsAssigned, b_cols, "%7.2f ");
    printf("\n");
    //========================================
    */

    //Collect the results
    MPI_Gather((void *)&c_buffer[0][0], cDataCount, MPI_DOUBLE, (void *)c, 
            cDataCount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /*
    if(rank == 0)
    {
        printf("Result\n");
        Matrix_Print(stdout, c, a_rows, b_cols, "%7.2f ");
        printf("\n");
    }
    */

    free(a_buffer);
    free(c_buffer);
}
