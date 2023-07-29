#**************************************************
# Makefile for CST407 Matrix Multiply
#
# Author: Philip Howard
#
CC = gcc
CFLAGS = -g -O3 -Wall -pthread -std=c11
CPPFLAGS = -g -O0 -Wall -std=c++11
LFLAGS = -pthread

OBJS = tmatrix.o \
       matrix.o \
       usec.o

all: mmult

clean:
	rm -f mmult
	rm -f *.o
	rm -f out

.c.o:
	gcc $(CFLAGS) -c $? -o $@

.cpp.o:
	g++ $(CPPFLAGS) -c $? -o $@

mmult: mmult.o $(OBJS)
	gcc $(CFLAGS) mmult.o -o mmult $(OBJS) $(LFLAGS)

