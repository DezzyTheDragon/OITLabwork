CFLAGS = -g -O0 -Wall -pthread

OBJS = addr2str.o \
       encode.o \
       getport.o \
       utils.o \
       connect.o \
       timed_accept.o \
       readline.o \

all: server

clean:
	rm -f server

.c.o:
	gcc $(CFLAGS) -c $? -o $@

server: server.c $(OBJS)
	gcc $(CFLAGS) server.c -o server $(OBJS)
