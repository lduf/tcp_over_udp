#ifndef listeType_h_
#define listeType_h_

struct model_elem {
  int port ;
  int socket ;
  int pid;
  struct model_elem* suivant;
};

typedef struct model_elem ELEMLISTE;

typedef ELEMLISTE *LISTE; 
 
 #endif