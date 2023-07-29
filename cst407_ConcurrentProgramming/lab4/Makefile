OBJS = matrix.o \
       mpimatrix.o \
       usec.o

all: mpimmult

.c.o:
	mpicc -c -g -O3 -Wall $? -o $@

mpimmult: mpimmult.o $(OBJS)
	mpicc -g -O3 -Wall mpimmult.o -o ~/mpi_prog/mpimmult $(OBJS)

clean:
	rm ~/mpi_prog/mpimmult
	rm *.o
