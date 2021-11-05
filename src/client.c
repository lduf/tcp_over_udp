#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "includes/compareString.h"

#define BUFFER_LIMIT 1500
#define EVER ;;

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int conn(int s, struct sockaddr_in server){
	int port_ack = -1;
	/**
 * 
 * 
 * CONNECTION PROCESS
 * 
 **/

	printf("Initialisation de la connection ... \n");
	char buf[BUFFER_LIMIT];
   strcpy(buf,"SYN");
   if (sendto(s, buf, (strlen(buf)+1), 0,(struct sockaddr *)&server, sizeof(server)) < 0){
			handle_error("sendto()");
		}
	printf("SYN sent \n");
	int server_addr_len = sizeof(server);
	if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &server,
	            (unsigned int *) &server_addr_len) <0)
	   {
	       handle_error("recvfrom()");
	       exit(4);
	   }
	if(compareString(buf, "^SYN_ACK:[0-9]+$") == 1){
		strtok(buf, ":");
		port_ack = atoi(strtok(NULL, ":"));
		printf("SYNACK and new port number received\n");
		strcpy(buf,"ACK");
	   if (sendto(s, buf, (strlen(buf)+1), 0,(struct sockaddr *)&server, sizeof(server)) < 0){
				handle_error("sendto()");
		}
		printf("ACK sent\n");
	}else{
		printf("%s", buf);
		//handle_error("Error during SYN");
	}
	return port_ack;
}



int main(int argc, char *argv[]){

   int s;
   unsigned short port;
   struct sockaddr_in server;
   char buf[BUFFER_LIMIT];

   /* argv[1] is internet address of server argv[2] is port of server.
    * Convert the port from ascii to integer and then from host byte
    * order to network byte order.
    */
   if(argc != 3)
   {
      	argv[1] = "127.0.0.1";
		argv[2] = "3001";
   }
   port = htons(atoi(argv[2]));


   /* Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       handle_error("socket()");
       exit(1);
   }

   /* Set up the server name */
   server.sin_family      = AF_INET;            /* Internet Domain    */
   server.sin_port        = port;               /* Server Port        */
   server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */

   int port_ack = conn(s, (struct sockaddr_in) server);
   printf("Je dois me connecter au port %d\n", port_ack);

		/**
		 * 
		 * CONNECTED
		 * 
		 * */ 

   while(strcmp("STOP\n",buf) != 0){
		memset((char*)buf, 0, sizeof(buf)); // permet de bien avoir une adresse vide et pas se taper ce qu'on avait en mémoire avant
		printf("Texte à envoyer ? STOP pour terminer\n");
		//scanf("%s", message);
		fgets(buf, sizeof(buf), stdin);  // read string
		/* Send the message in buf to the server */
		//compare if the message is STOP
		if(strcmp("STOP\n",buf) == 0){
			//assing the message to the buffer
			strcpy(buf,"LEAVE");
			if (sendto(s, buf, (strlen(buf)+1), 0,(struct sockaddr *)&server, sizeof(server)) < 0){
				handle_error("sendto()");
			}
			printf("Disconnection sent\n");
			break;
		}
		if (sendto(s, buf, (strlen(buf)+1), 0,(struct sockaddr *)&server, sizeof(server)) < 0){
			handle_error("sendto()");
		}
		printf("Message sent\n");

	}
   

   /* Deallocate the socket */
   close(s);
   return(0);
}