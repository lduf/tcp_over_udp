#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUFFER_LIMIT 1500
#define EVER ;;
#define random_max 10000


/**
 * 
 * Return socket number
 * 
 * */
int SYN(int serv_socket, struct sockaddr* client){


	srand(time(NULL));   // Initialization, should only be called once.
	int r = rand() % random_max + 3000;      // Returns a pseudo-random integer between 0 and RAND_MAX.
	printf("tentative de SYN de la part du client");
	// Allocates storage
	char *syn_ack = (char*)malloc(15 * sizeof(char));
	sprintf(syn_ack, "SYN_ACK:%d", r);
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
	   	return 0;
	   }
	   else{
	   	return -1;
	   }

	
}
int main(int argc, char *argv[]){

   int s, namelen, client_address_size;
   unsigned short port;
   struct sockaddr_in client, server;
   char buf[BUFFER_LIMIT];

   if(argc != 2)
   {
      	argv[1] = "3001";
   }
   port = htons(atoi(argv[1]));

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
   server.sin_port        = port;         /* Use any available port      */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */

   if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
       handle_error("bind()");
       exit(2);
   }


   for(EVER){
	   /*
	    * Receive a message on socket s in buf  of maximum size 32
	    * from a client. Because the last two paramters
	    * are not null, the name of the client will be placed into the
	    * client data structure and the size of the client address will
	    * be placed into client_address_size.
	    */
	   client_address_size = sizeof(client);

	   if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client,
	            (unsigned int *) &client_address_size) <0)
	   {
	       handle_error("recvfrom()");
	       exit(4);
	   }

	   if(strcmp("SYN", buf) == 0){
	   	SYN(s, (struct sockaddr *) &client);
	   }
	   /*
	    * Print the message and the name of the client.
	    * The domain should be the internet domain (AF_INET).
	    * The port is received in network byte order, so we translate it to
	    * host byte order before printing it.
	    * The internet address is received as 32 bits in network byte order
	    * so we use a utility that converts it to a string printed in
	    * dotted decimal format for readability.
	    */
	   printf("[%s:%d] : %s \n",
	       inet_ntoa(client.sin_addr),
	       ntohs(client.sin_port),
	       buf
	       );
	}

   /*
    * Deallocate the socket.
    */
   close(s);
   return(0);
}