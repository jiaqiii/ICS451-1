CC = gcc

all: server.c client.c
	$(CC) -c server.c 
	$(CC) -c client.c
	$(CC) server.o -o server
	$(CC) client.o -o client

server: server.c
	$(CC) -c server.c
	$(CC) server.o -o server

client: client.c
	$(CC) -c client.c
	$(CC) client.o -o client

clean: 
	rm -rf server client *.o
