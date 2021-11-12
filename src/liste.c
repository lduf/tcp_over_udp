#include <stdio.h>
#include <stdlib.h>

#include "includes/liste.h"


/*!
************************************************
* \brief ajouter un portent dans une liste triee
* \param pliste adresse d'une liste 
* \param val un entier a ajouter à la liste triee
* \warning On passe un pointeur sur une liste (donc un pointeur
*   sur un pointeur sur un portent)
* \return Modifie La liste, insère l'élément inséré
**************************************************/    
void addToList(LISTE *pliste, int val)	
{ 
	if(*pliste == NULL){
		(*pliste) = (LISTE)malloc(sizeof(LISTE));
		(*pliste)->port = val;
		(*pliste)->socket = (int)malloc(sizeof(int));
		(*pliste)->pid = (int)malloc(sizeof(int));
		(*pliste)->socket = 0;
		(*pliste)->pid = 0;
		(*pliste)->suivant = NULL;
		return;
	}
	if((*pliste)->port > val){
		LISTE newListe = (LISTE)malloc(sizeof(LISTE));
		newListe->port = val;
		newListe->socket = (int)malloc(sizeof(int));
		(*pliste)->pid = (int)malloc(sizeof(int));
		(*pliste)->socket = 0;
		(*pliste)->pid = 0;
		newListe->suivant = (*pliste);

		(*pliste) = newListe;
		return;
	}
	addToList((&(*pliste)->suivant), val);
}


/*!
************************************************
* \brief vérfier si un élément est présent dans la liste
* \param pliste adresse d'une liste 
* \param val le port sur lequel on opère la vérification
* \warning On passe un pointeur sur une liste (donc un pointeur
*   sur un pointeur sur un portent)
* \return 1 in list / 0 not in list
**************************************************/    
int inList(LISTE *pliste, int val)	
{ 
	if(*pliste == NULL){
		return 0;
	}
	if((*pliste)->port == val){
		return 1;
	}
	return inList((&(*pliste)->suivant), val);
}


int nextUsableInt(LISTE liste){
	if(liste->suivant == NULL){
		return liste->port +1;
	}
	if(liste->port + 1 == liste->suivant->port){
		return nextUsableInt(liste->suivant);
	}
	return liste->port +1;
}
/*!
************************************************
* \brief affiche une liste d'entier
* \param liste une liste d'entier
**************************************************/    
void afficherListe(LISTE liste)	
{
  ELEMLISTE *visitor;
 
 visitor=liste;
 while(visitor!=0)
   {
     fprintf(stdout,"|%d : %d / %d",visitor->socket, visitor->port, visitor->pid);
     visitor=visitor->suivant;
   }
 fprintf(stdout,"|\n");
}	


/**
 * @brief Supprime un élément de la liste à partir de son port
 * 
 * @param list 
 * @param port 
 * @return * Remove*** 
 *
 * */
void removeFromList(LISTE *list, int port)
{
    while ((*list) != NULL)
    {
        if ((*list)->port == port)
        {
            LISTE tmp = *list;
            *list = (*list)->suivant;
            free(tmp);
            return ;
        }
        else
        {
            list = &((*list)->suivant);
        }
    }
}

int viderListe(LISTE *pliste1){
	if(*pliste1 == NULL){
		return 0;
	}
	int res = 1 + viderListe(&(*pliste1)->suivant);
	free(*pliste1);
	return res;
}

/**
 * @brief Set the Socket From Port object
 * 
 * @param pliste 
 * @param socket 
 * @param port 
 */

void setSocketFromPort(LISTE *pliste, int socket, int port){
	if((*pliste )== NULL){
		return;
	}
	if((*pliste)->port == port){
		(*pliste)->socket = socket;
		return ;
	}
	return setSocketFromPort(&((*pliste)->suivant), socket, port);
}


/**
 * @brief Get the port from the socket number
 * 
 * @param LISTE *pliste : the list which contains all informations
 * @param int socket : the socket to which we want to get the port
 * @return int the port number
 */
int getPortFromSocket(LISTE *pliste, int socket){
	if((*pliste) == NULL){
		return -1;
	}
	if((*pliste)->socket == socket){
		return (*pliste)->port;
	}
	return getPortFromSocket(&((*pliste)->suivant), socket);
}

/**
 * @brief Get the socket from the port number
 * 
 * @param LISTE *pliste : the list which contains all informations
 * @param int port : the port to which we want to get the socket
 * @return int the port number
 */
int getSocketFromPort(LISTE *pliste, int port){
	if((*pliste) == NULL){
		printf("LIST NULL\n");
		return -1;
	}
	if((*pliste)->port == port){
		return (*pliste)->socket;
	}
	return getSocketFromPort(&((*pliste)->suivant), port);
}

/**
 * @brief This function will set the pid of the process which is connected to the socket
 * 
 * @param LISTE *pliste : the list which contains all informations
 * @param int socket : the port to which we want to get the socket
 * @param int pid : the pid of the process
 * 
 * @return int -1 if the socket is not found, 0 otherwise
 */
int setPidFromSocket(LISTE *pliste, int socket, int pid){
	if((*pliste) == NULL){
		return -1;
	}
	if((*pliste)->socket == socket){
		(*pliste)->pid = pid;
		return 0;
	}
	return setPidFromSocket(&((*pliste)->suivant), socket, pid);
}

/**
 * @brief This function will get the pid of the process which is connected to the socket
 * 
 * @param LISTE *pliste : the list which contains all informations
 * @param int socket : the port to which we want to get the socket
 * 
 * @return int -1 if the socket is not found, the pid otherwise
 */
int getPidFromSocket(LISTE *pliste, int socket){
	if((*pliste) == NULL){
		return -1;
	}
	if((*pliste)->socket == socket){
		return (*pliste)->pid;
	}
	return getPidFromSocket(&((*pliste)->suivant), socket);
}

/**
 * @brief This function will get the port from the pid
 * 
 * @param LISTE *pliste : the list which contains all informations
 * @param int pid : the pid to which we want to get the port
 * 
 * @return int -1 if the pid is not found, the port otherwise
 */
int getPortFromPID(LISTE *pliste, int pid){
	if((*pliste) == NULL){
		return -1;
	}
	if((*pliste)->pid == pid){
		return (*pliste)->port;
	}
	return getPortFromPID(&((*pliste)->suivant), pid);
}