#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "matrix.h"
#include "mpimatrix.h"
#include "usec.h"

static int g_a_rows = 3;
static int g_a_cols = 2;
static int g_b_cols = 3;
static int g_print_level = 0;
static int g_alg = 0;

static void proc_args(int argc, char **argv)
{
    int opt;
    while((opt = getopt(argc, argv, "ha:r:c:b:p:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                printf("%s -a <alg> -r <rows> -c <cols> -b <b cols> "
                        "-p <print level>\n", argv[0]);
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
            case 'p':
                g_print_level = atoi(optarg);
                break;
        }
    }
}

int main(int argc, char ** argv)
{
    MPI_Init(NULL, NULL);

    proc_args(argc, argv);

    //if (matrix A_rows is a multiple of processes)
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(g_a_rows % world_size == 0)
    {
        //matrix A can evenly be scattered
        int a_cols = g_a_cols;
        int a_rows = g_a_rows;
        int b_cols = g_b_cols;
        int b_rows = g_a_rows;
        int c_rows = a_rows;
        int c_cols = b_cols;

        typedef double a_type[a_cols];
        typedef double b_type[b_cols];
        typedef double c_type[c_cols];

        a_type *a = (a_type*)malloc(sizeof(double)*a_cols*a_rows);
        b_type *b = (b_type*)malloc(sizeof(double)*b_cols*b_rows);
        c_type *c = (c_type*)malloc(sizeof(double)*c_cols*c_rows);

        if(rank == 0)
        {
            double value = 0;
            for(int ii = 0; ii < a_rows; ii++)
            {
                for(int jj = 0; jj < a_cols; jj++)
                {
                    a[ii][jj] = value++;
                }
            }
        
            for(int ii = 0; ii < b_rows; ii++)
            {
                for(int jj = 0; jj < b_cols; jj++)
                {
                    b[ii][jj] = value++;
                }
            }
        
            //conditinally print the matracies
            if(g_print_level > 1)
            {
                printf("Matrix A\n");
                Matrix_Print(stdout, &a[0][0], a_rows, a_cols, "%7.2f ");
                printf("\n\nMatrix B\n");
                Matrix_Print(stdout, &b[0][0], b_rows, b_cols, "%7.2f ");
                printf("\n");

            }
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
        int64_t start = 0;
        if(rank == 0)
        {
            start = usecs();
        }

        //Do the actual work
        switch(g_alg)
        {
            case 0:
                MPI_Matrix_Multiply(&a[0][0], &b[0][0], &c[0][0], a_cols, a_rows, b_cols);
                break;
            case 1:
                if(rank == 0)
                {
                    Matrix_Multiply(&a[0][0], &b[0][0], &c[0][0], a_cols, a_rows, b_cols);
                }
                break;
        }
        
        int64_t stop;
        if(rank == 0)
        {
            stop = usecs();
            printf("Time for matrix multiply %1.6f\n", ((stop - start) / 1000000.0));
        }

        if(rank == 0 && g_print_level > 0)
        {
            printf("Matrix C\n");
            Matrix_Print(stdout, &c[0][0], c_rows, c_cols, "%7.2f ");
            printf("\n");
        }

        free(a);
        free(b);
        free(c);
    }
    //else if not multiple
    else
    {
        if(rank == 0)
        {
            printf("ERROR: Matrix A Rows must be a multiple of %d\n", world_size);
        }
    }

    //End
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
