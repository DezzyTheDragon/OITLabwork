CFLAGS = -g -O0 -Wall

C_OBJS = encode.o \
	 socket.o \
	 util.o \
	 addr2str.o \

S_OBJS = encode.o \
	 socket.o \
	 util.o \
	 addr2str.o \
	 handlerequest.o \
	 list.o \

all: client server

clean:
	rm -f client
	rm -f server
	rm -f *.o

.c.o:
	gcc $(CFLAGS) -c $? -o $@

client: client.c $(C_OBJS)
	gcc $(CFLAGS) client.c -o client $(C_OBJS)

server: server.c $(S_OBJS)
	gcc $(CFLAGS) server.c -o server $(S_OBJS)
