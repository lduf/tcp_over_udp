#ifndef list_h_
#define list_h_
#include "liste_type.h"

void afficherListe(LISTE);	
int inList(LISTE *pliste, int val);
int nextUsableInt(LISTE pliste);
void removeItem(LISTE *list, int i);
int viderListe(LISTE *pliste1);
void addToList(LISTE *pliste, int val);	
void linkSocketToPort(LISTE *pliste, int socket, int port);
void linkClientToPort(LISTE *pliste, struct sockaddr_in *client, int port);
#endif