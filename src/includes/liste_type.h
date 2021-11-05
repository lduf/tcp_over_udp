#ifndef listeType_h_
#define listeType_h_
#include <sys/socket.h>

struct model_elem {
  int port ;
  int socket ;
  struct sockaddr_in* client;
  struct model_elem* suivant;
};

typedef struct model_elem ELEMLISTE;

typedef ELEMLISTE *LISTE; 
 
 #endif