#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_LIMIT 1500
#define EVER ;;

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

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

   while(strcmp("STOP\n",buf) != 0){
		memset((char*)buf, 0, sizeof(buf)); // permet de bien avoir une adresse vide et pas se taper ce qu'on avait en mémoire avant
		printf("Texte à envoyer ? STOP pour terminer\n");
		//scanf("%s", message);
		fgets(buf, sizeof(buf), stdin);  // read string
		/* Send the message in buf to the server */
		if (sendto(s, buf, (strlen(buf)+1), 0,(struct sockaddr *)&server, sizeof(server)) < 0){
		       handle_error("sendto()");
		       exit(2);
		}
		printf("Message sent\n");

	}
   

   /* Deallocate the socket */
   close(s);
   return(0);
}