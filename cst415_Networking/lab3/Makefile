CFLAGS = -g -O0 -Wall -lpthread

C_OBJS = getport.o \
	 utils.o \
	 encode.o \
	 readline.o \
	 addr2str.o \

S_OBJS = getport.o \
	 utils.o \
	 encode.o \
	 addr2str.o \
	 readline.o \
	 timed_accept.o \

all: client server

clean:
	rm -f client
	rm -f server

.c.o:
	gcc $(CFLAGS) -c $? -o $@

client: client.c $(C_OBJS)
	gcc $(CFLAGS) client.c -o client $(C_OBJS)
	mkdir dump

server: server.c $(S_OBJS)
	gcc $(CFLAGS) server.c -o server $(S_OBJS)
	
