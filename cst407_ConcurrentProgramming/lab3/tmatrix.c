#include "tmatrix.h"
#include <stdio.h>

pthread_mutex_t matrixC_lock;
int matrixC_row = 0;
int matrixC_col = 0;

void *Threaded_Matrix_Scheduler(void *data)
{
    //matrix c col = b_col
    //matrix c rows = a_rows
    matrixData *matrix = (matrixData *)data;
    int running = 1;
    while(running)
    {
        pthread_mutex_lock(&matrixC_lock);
        int assigned_row = matrixC_row;
        int assigned_col = matrixC_col;
        matrixC_col++;
        if(matrixC_col >= matrix->b_col)
        {
            matrixC_row++;
            //printf("row %d\n", matrixC_row);
            if(matrixC_row >= matrix->a_rows)
            {
                //Matrix is calculated
                running = 0;
            }
            else
            {
                matrixC_col = 0;
            }
        }
        pthread_mutex_unlock(&matrixC_lock);
        //printf("Solving for cell %d, %d\n", assigned_row, assigned_col);
        Threaded_Matrix_Multiply_Cell(matrix->matrixA, matrix->matrixB, matrix->matrixC,
                matrix->a_cols, matrix->b_col, assigned_row, assigned_col);
        
    }

    return NULL;
}

void Threaded_Matrix_Multiply_Cell(double *a, double *b, double *c, 
        int a_cols, int b_cols, int dest_row, int dest_col)
{
    //Convert the incomming values into something that can be worked with
    typedef double a_type[a_cols];
    typedef double b_type[b_cols];
    typedef double c_type[b_cols];
    a_type *a_value;
    b_type *b_value;
    c_type *c_value;
    a_value = (a_type *)a;
    b_value = (b_type *)b;
    c_value = (c_type *)c;
    //Multiply each of the corisponding cells in A and B and add together
    double temp_value = 0;
    for(int i = 0; i < a_cols; i++)
    {
        //printf("Cell (%d, %d): doing (%d, %d) * (%d, %d)\n", dest_row, dest_col,
        //        dest_row, i, i, dest_row);
        temp_value += a_value[dest_row][i] * b_value[i][dest_col];
    }
    c_value[dest_row][dest_col] = temp_value;
}
