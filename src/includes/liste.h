#ifndef list_h_
#define list_h_
#include "liste_type.h"
#include <sys/socket.h>

void afficherListe(LISTE);	
int inList(LISTE *pliste, int val);
int nextUsableInt(LISTE pliste);
void removeFromList(LISTE *list, int port);
int viderListe(LISTE *pliste1);
void addToList(LISTE *pliste, int val);	
void setSocketFromPort(LISTE *pliste, int socket, int port);
int getPortFromSocket(LISTE *pliste, int socket);
int getSocketFromPort(LISTE *pliste, int port);
int setPidFromSocket(LISTE *pliste, int socket, int pid);
int getPidFromSocket(LISTE *pliste, int socket);
int getPortFromPID(LISTE *pliste, int pid);
#endif