#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include "includes/compareString.h"
#include "includes/liste.h"
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUFFER_LIMIT 1500
#define EVER ;;
#define random_max 10000
#define MAX_CONN 2


/**
 * 
 * Return socket number
 * 
 * */
int SYN(LISTE *portList, int serv_socket, struct sockaddr* client){
	printf("new SYN\n");

	int s;
	 /*
    * Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       handle_error("socket()");
       exit(1);
   }

   int port;
   struct sockaddr_in serv;
   serv.sin_family      = AF_INET;  /* Server is in Internet Domain */
   serv.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */

   printf("J'ai défini la base de mon serveur pour la nouvelle socket\n");
   do{
   		port = nextUsableInt(*portList);
   		printf("J'ai le prochain port : %d\n", port);
   		serv.sin_port = htons(port); 
   }
   while(bind(s, (struct sockaddr *)&serv, sizeof(serv)) < 0);
   printf("Mon bind s'est bien déroulé, je veux associer mon port à ma socket dans ma liste\n");
   addToList(portList, port);
   linkSocketToPort(portList, s, port);
   linkClientToPort(portList, (struct sockaddr_in *) client, port);


	// Allocates storage
	char *syn_ack = (char*)malloc(16 * sizeof(char));
	sprintf(syn_ack, "SYN_ACK:%d", port);
	if (sendto(serv_socket, syn_ack, (strlen(syn_ack)+1), 0,(struct sockaddr *)client, sizeof(*client)) < 0){
			handle_error("sendto()");
		}

	char message[BUFFER_LIMIT];
	int client_address_size = sizeof(*client);
	   if(recvfrom(serv_socket, message, sizeof(message), 0, (struct sockaddr *) &client,
	            (unsigned int *) &client_address_size) <0)
	   {
	       handle_error("recvfrom()");
	       exit(4);
	   }

	   if(strcmp("ACK", message) == 0){
		   //print s
		   printf("Port de connection : %d\n", s);   
		
	   	return s;
	   }
	   else{
	   	return -1;
	   }
}

int leave(LISTE *portList, int port){
	//disconnect the client from the server


 return 0;
}

void distribute(LISTE lst, int FromSocket, char *msg){
	do{
		if(lst->socket != FromSocket){
			printf("socket : %d, msg : %s\n", lst->socket, msg);
			printf("%s\n",lst->client);
			if (sendto(lst->socket, msg, (strlen(msg)+1), 0,(struct sockaddr *)lst->client, sizeof(lst->client)) < 0){
				handle_error("sendto()");
			}
		}
		lst = lst->suivant;
	}while(lst != NULL);
}

int main(int argc, char *argv[]){

   int s, namelen, client_address_size;
   unsigned short port;
   struct sockaddr_in client, server;
   char buf[BUFFER_LIMIT];
   LISTE portList = (LISTE) NULL;
   if(argc != 2)
   {
      	argv[1] = "3001";
   }
   port = atoi(argv[1]);
   addToList(&portList, port);
   /*
    * Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       handle_error("socket()");
       exit(1);
   }

   /*
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network
    * interface.)
    */
   server.sin_family      = AF_INET;  /* Server is in Internet Domain */
   server.sin_port        = htons(port); 
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */

   if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
       handle_error("bind()");
       exit(2);
   }

   linkSocketToPort(&portList, s, port);
   afficherListe(portList);
   for(EVER){
	   /*
	    * Receive a message on socket s in buf  of maximum size 32
	    * from a client. Because the last two paramters
	    * are not null, the name of the client will be placed into the
	    * client data structure and the size of the client address will
	    * be placed into client_address_size.
	    */
	   client_address_size = sizeof(client);

	   if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client,(unsigned int *) &client_address_size) <0){
	       handle_error("recvfrom()");
	       exit(4);
	   }

	   if(strcmp("SYN", buf) == 0){
	   	int next_sock = SYN(&portList, s, (struct sockaddr *) &client);
	   	afficherListe(portList);
	   }
	   else if(strcmp("LEAVE", buf) == 0){
		   //print je dois leave
		   printf("Je dois leave\n");
		   printf("%d\n",port);
		   leave(&portList, port);
	   }
	   else{
	   	/*
	    * Print the message and the name of the client.
	    * The domain should be the internet domain (AF_INET).
	    * The port is received in network byte order, so we translate it to
	    * host byte order before printing it.
	    * The internet address is received as 32 bits in network byte order
	    * so we use a utility that converts it to a string printed in
	    * dotted decimal format for readability.
	    */

	   
		char msg[BUFFER_LIMIT];
		sprintf(msg,"[%s:%d] : %s \n",
	       inet_ntoa(client.sin_addr),
	       ntohs(client.sin_port),
	       buf
	       );
		   //print the messsage received
	   printf("Message received: %s\n", msg);

		
	   //	distribute(portList, s, msg);
	   }
	   
	}

   /*
    * Deallocate the socket.
    */
   close(s);
   return(0);
}