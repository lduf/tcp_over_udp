#include <stdio.h>
#include <stdlib.h>
#include "includes/server.h"
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUFFER_LIMIT 1500
#define EVER ;;
#define random_max 10000
#define MAX_CONN 2
#define LEGAL_PATH_REGEX "^FILE:(.+)\\/([^\\/]+)$"
#define SEGMENT_SIZE 536

/**
 * @param LISTE *portList : liste des ports
 * @param int serv_socket : port du serveur
 * @param sockaddr sockaddr* client : info client client
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
   memset(&serv, 0, sizeof(serv));
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
   setSocketFromPort(portList, s, port);

	// Allocates storage
	char *syn_ack = (char*)malloc(16 * sizeof(char));
	sprintf(syn_ack, "SYN_ACK:%d", port);
	if (sendto(serv_socket, syn_ack, (strlen(syn_ack)+1), 0,(struct sockaddr *)client, sizeof(*client)) < 0){
			handle_error("sendto()");
		}

	char message[BUFFER_LIMIT];
	int client_address_size = sizeof(*client);
	   if(recvfrom(serv_socket, message, sizeof(message), 0, (struct sockaddr *) &client,(unsigned int *) &client_address_size) <0)
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

/**
 * @brief This function is used to close the socket, and end the connection
 * 
 * @param portList 
 * @param PID 
 * @return int 
 */
int leave(LISTE *portList, int PID){
	//disconnect the client from the server
		int port = getPortFromPID(portList, PID);
		// get the socket from the port
		int s = getSocketFromPort(portList, port);
		printf("J'ai la socket %d \n", s);
		printf("Je dois remove le port %d \n", port);
		//remove the port from the list
		removeFromList(portList, port);
		//affichage de la liste
		afficherListe(*portList);
		//close the socket
		printf("Je ferme la socket %d \n", s);
		close(s);
		return 0;
}

/**
 * @brief This function returns the content of the given file
 * 
 * @param char *fileName : the name of the file
 * 
 * @return char* : the content of the file
 */
char* getFileContent(char *fileName){
	if (fileName[strlen(fileName)  - 1] == '\n') fileName[strlen(fileName)  - 1] = '\0';
	FILE *file = fopen(fileName, "r");
	if(file == NULL){
		return NULL;
	}
	char *content = malloc(sizeof(char) * getFileSize(fileName));
	char c;
	while((c = fgetc(file)) != EOF){
		sprintf(content, "%s%c",content, c);
	}
	fclose(file);
	return content;
}

/**
 * @brief This function return the number of characters in the given file
 * 
 * 	@param char *fileName : the name of the file
 *  @return int : the number of characters in the file
 */
int getFileSize(char *fileName){
	if (fileName[strlen(fileName)  - 1] == '\n') fileName[strlen(fileName) - 1] = '\0';
	FILE *file = fopen(fileName, "r");
	if(file == NULL){
		handle_error("fopen()");
	}
	int size = 0;
	char c;
	while((c = fgetc(file)) != EOF){
		size++;
	}
	fclose(file);
	return size;
}

/**
 * @brief This function is used to send the file to the client
 * 
 * @param int s : the socket of the client
 * @param char *fileName : the name of the file
 * @param sockaddr *client : the client's address
 * @param int client_address_size : the size of the client's address
 * @return int : the number of characters sent
 */
int sendFile(int s, char *fileName, struct sockaddr *client, int client_address_size){
	char *fileContent = getFileContent(fileName);
	printf("J'ai le contenu du fichier : %s\n", fileContent);
	int bytesSent = 0;
	char *message = malloc(sizeof(char) * SEGMENT_SIZE);
	int size =  getFileSize(fileName);
	printf("Taille du fichier %d, soit %d\n",size, size/SEGMENT_SIZE);
	printf("Je rentre dans le while\n");
	int packet = 0;
	while(bytesSent < size){
		memset(message, 0, sizeof(*message));
		while (bytesSent < (packet+1)*SEGMENT_SIZE && bytesSent < size){
			sprintf(message, "%s%c", message, fileContent[bytesSent]);
			bytesSent +=1 ;
		}
		printf("J'ai envoyé le paquet n° : %d : %d bytes\n\n %s\n", packet, bytesSent, message);
		packet ++;
		
	}
	printf("While done \n");
	/*
	int n = sendto(s, fileContent, strlen(fileContent), 0, client, client_address_size);
	if(n < 0){
		handle_error("sendto()");
	}
	*/
	return 0;
}

/**
 * @brief this function is used to handle client's request/messages
 * 
 * @param int socket : the socket number 
 * @return void 
 */
void handle_client(LISTE *portList,int socket){
	int client_address_size;
   	unsigned short port;
   	struct sockaddr_in client;
   	char buf[BUFFER_LIMIT];

	 for(EVER){
	   /*
	    * Receive a message on socket s in buf  of maximum size 32
	    * from a client. Because the last two paramters
	    * are not null, the name of the client will be placed into the
	    * client data structure and the size of the client address will
	    * be placed into client_address_size.
	    */
	   client_address_size = sizeof(client);

	   if(recvfrom(socket, buf, sizeof(buf), 0, (struct sockaddr *) &client,(unsigned int *) &client_address_size) <0){
	       handle_error("recvfrom()");
	   }
	   if(strcmp("LEAVE", buf) == 0){
		   printf("Déconnection demandée");
		   break;
	   }
	   else if(compareString(buf,LEGAL_PATH_REGEX) == 1){
	   		char *fileName = malloc(sizeof(char) * strlen(buf));
			strtok(buf, ":");
			fileName = strtok(NULL, ":");

			printf("The client send a filepath : %s\n", fileName);
			sendFile(socket, fileName, (struct sockaddr *) &client, client_address_size);
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

	   //get the client connection port
		getsockname(socket, (struct sockaddr *) &client, (unsigned int *) &client_address_size);
		
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
}


/**
 * @brief This function create a new child process, associate the pid to the socket, calls the client handler function and wait for the child process to finish, then returns the pid, so the parent process can kill it
 * 
 * @param LISTE *portList : liste des ports
 * @param int used_socket : socket
 * 
 *@return int pid : pid of the child process 
 */
int child(LISTE *portList, int used_socket){
	pid_t pid = fork();
	if(pid == 0){
		printf("Je suis dans le fils\n");
		setPidFromSocket(portList, used_socket, getpid());
		handle_client(portList, used_socket);
		return getpid();
	}
	else if(pid == -1){
		handle_error("fork()");
	}
	return -1;
}

/**
 * @brief This function is the main function : create the default socket which handle the client SYN/ and call all the required function to process data
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]){

   int s, namelen, client_address_size;
   unsigned short port;
   struct sockaddr_in client, server;
   char buf[BUFFER_LIMIT];
   LISTE portList = (LISTE) NULL;
   LISTE *portListPtr = &portList;
   portListPtr = mmap(NULL, sizeof *portListPtr, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   
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
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
       handle_error("socket()");
       exit(1);
   }
	memset(&server, 0, sizeof(server));	

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
   
   setSocketFromPort(&portList, s, port);
   setPidFromSocket(&portList, s, getpid());
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

	// On reçoit une demande de connection d'un client sur la socket de connection
	   if(strcmp("SYN", buf) == 0){
	   	int next_sock = SYN(&portList, s, (struct sockaddr *) &client);
	   	afficherListe(portList);
		   //create child process
		   int child_pid = child(&portList, next_sock);
		   if(child_pid != -1){
		   		printf("Pid du fils : %d\n", child_pid);
				printf("Pid du pere : %d\n", getpid());
				leave(&portList, child_pid);
				//kill(child_pid, SIGKILL);
				afficherListe(portList);
				//session est terminée
				printf("Session du client PID %d terminée\n", child_pid);
				
		   }


	   }
	}

   /*
    * Deallocate the socket.
    */
   close(s);
   return(0);
}

