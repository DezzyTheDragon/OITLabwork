#pragma once
//*****************************************************
// Define various matrix multiply routines
//
// Author: Phil Howard
//
#include <stdio.h>

// Multiply c = a*b
// The data for the matrices is in sequential row-major order
// Pointer arithmetic is used to fetch appropriate values for the multiplication
void Matrix_Multiply(double *a, double *b, double *c, 
                    int a_cols, int a_rows, int b_cols);

// Multiply c = a*b
// The data for the matrices is in sequential row-major order
// The function declares types so the matrix multiply is done using
// array notation
void Matrix_Multiply_Array(double *a, double *b, double *c, 
                    int a_cols, int a_rows, int b_cols);

// Print the specified matrix to the specified FILE using 'format' to format
// each element.
// The data for the matrices is in sequential column-major order
void Matrix_Print(FILE *output, double *mat, int rows, int cols, 
                  const char *format);

