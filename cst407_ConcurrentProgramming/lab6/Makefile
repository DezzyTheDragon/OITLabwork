#**************************************************
# Makefile for CST407 Threading
# CST 415 Lab 1
#
# Author: Philip Howard
# Email:  phil.howard@oit.edu
# Date:   Sept 22, 2016
#
CC = gcc
CFLAGS = -g -O0 -Wall -pthread -std=c14
CPPFLAGS = -g -O3 -Wall -std=c++17
LFLAGS = -pthread

OBJS = 

all: locktest

clean:
	rm -f threader
	rm -f locktest
	rm -f *.o

.c.o:
	gcc $(CFLAGS) -c $? -o $@

.cpp.o:
	g++ $(CPPFLAGS) -c $? -o $@

locktest: locktest.o $(OBJS)
	g++ locktest.o -o locktest $(OBJS) $(LFLAGS)

