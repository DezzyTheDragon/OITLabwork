//**************************************************
// Various matrix multiply and support routines
//
// Author: Phil Howard
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "matrix.h"

//****************************************
// Multiply two matrices c = a * b
// Use pointer arithmetic
void Matrix_Multiply(double *a, double *b, double *c, 
                    int a_cols, int a_rows, int b_cols)
{
    double temp_value;
    double *a_value, *b_value, *c_value;

    c_value = c;

    for (int arow=0; arow<a_rows; arow++)
    {
        for (int bcol=0; bcol<b_cols; bcol++)
        {
            a_value = a + arow*a_cols;
            b_value = b + bcol;
            temp_value = 0;

            for (int acol=0; acol<a_cols; acol++)
            {
                temp_value += *a_value * (*b_value);
                //printf("%d %d %d %f %f %f\n", arow, bcol, acol,
                //        temp_value, *a_value, *b_value);
                a_value++;
                b_value += b_cols;
            }

            *c_value = temp_value;
            c_value++;
        }
    }
}
//****************************************
// Multiply two matrices c = a * b
// Use Array Indexing
void Matrix_Multiply_Array(double *a, double *b, double *c, 
                    int a_cols, int a_rows, int b_cols)
{
    double temp_value;
    typedef double a_type[a_cols];
    typedef double b_type[b_cols];
    typedef double c_type[b_cols];
    a_type *a_value;
    b_type *b_value;
    c_type *c_value;

    a_value = (a_type *)a;
    b_value = (b_type *)b;
    c_value = (c_type *)c;

    for (int arow=0; arow<a_rows; arow++)
    {
        for (int bcol=0; bcol<b_cols; bcol++)
        {
            temp_value = 0;

            for (int acol=0; acol<a_cols; acol++)
            {
                temp_value += a_value[arow][acol] * b_value[acol][bcol];
            }

            c_value[arow][bcol] = temp_value;
        }
    }
}

//****************************************
// print a matrix to output
void Matrix_Print(FILE *output, double *mat, int rows, int cols,
                  const char *format)
{
    double *value = mat;

    for (int ii=0; ii<rows; ii++)
    {
        for (int jj=0; jj<cols; jj++)
        {
            fprintf(output, format, *value);
            value++;
        }
        fprintf(output, "\n");
    }
}
