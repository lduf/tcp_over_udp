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
		(*pliste)->socket = -1;
		(*pliste)->client = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
		(*pliste)->suivant = NULL;
		return;
	}
	if((*pliste)->port > val){
		LISTE newListe = (LISTE)malloc(sizeof(LISTE));
		newListe->port = val;
		newListe->socket = -1;
		newListe->client = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
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
     fprintf(stdout,"|%d : %d",visitor->socket, visitor->port);
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

void linkSocketToPort(LISTE *pliste, int socket, int port){
	if((*pliste )== NULL){
		return;
	}
	if((*pliste)->port == port){
		(*pliste)->socket = socket;
		return ;
	}
	return linkSocketToPort(&((*pliste)->suivant), socket, port);
}

void linkClientToPort(LISTE *pliste, struct sockaddr_in *client, int port){
	if((*pliste) == NULL){
		return;
	}
	if((*pliste)->port == port){
		(*pliste)->client = client;
		return ;
	}
	return linkClientToPort(&((*pliste)->suivant),  client, port);
}


/**
 * @brief Get the port from the socket number
 * 
 * @param LISTE *pliste : the list which contains all informations
 * @param int socket : the port to which we want to get the socket
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
