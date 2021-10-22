all : server client

server : server.o
	gcc server.o -o server

client : client.o
	gcc client.o -o client

serveur.o : serveur.c
	gcc -c -g -Wall serveur.c -o serveur.o

client.o : client.c
	gcc -c -g -Wall client.c -o client.o

clean:
	rm -f serveur client *.o