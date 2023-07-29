CC = g++
CPPFLAGS = -Wall -g -pthread -std=c++11
OBJS = tsList.o \
	usec.o \

all: main

main: main.cpp $(OBJS)
	g++ $(CPPFLAGS) main.cpp $(OBJS) -o main -pthread

helmain: helmain.cpp $(OBJS) 
	g++ $(CPPFLAGS) helmain.cpp $(OBJS) -o helmain -pthread

clean:
	rm -f main
	rm -f helmain
	rm -r *.o
