all : server client

server : server.o compareString.o liste.o
	gcc server.o compareString.o liste.o -o server

client : client.o compareString.o
	gcc client.o compareString.o -o client

compareString.o : compareString.c includes/compareString.h
	gcc -c -g -Wall compareString.c -I /includes -o compareString.o

liste.o : liste.c includes/liste_type.h includes/liste.h
	gcc -c -g -Wall liste.c -I /includes -o liste.o
serveur.o : serveur.c includes/serveur.h
	gcc -c -g -Wall seveur.c -I /includes -o serveur.o

client.o : client.c
	gcc -c -g -Wall client.c -o client.o

clean:
	rm -f serveur client *.o
