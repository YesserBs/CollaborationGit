#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

static struct sockaddr_in adresse;
int creer_socket(int type, int *ptr_port, struct sockaddr_in *ptr_adresse){
  
  int desc; /* descripteur de la socket */
  int longueur=sizeof(struct sockaddr_in); /* taille de l'adresse */

  if ((desc=socket(AF_INET, type, 0))==-1) {
    perror("Creation de socket impossible");
    return -1;
  }
/* Preparation de l'adresse d'attachement */
  adresse.sin_family = AF_INET;
  adresse.sin_addr.s_addr = htonl(INADDR_ANY);
  adresse.sin_port = htons(*ptr_port); /* numero de port en format reseau */
  if (bind(desc, (struct sockaddr *)&adresse, longueur) == -1) {
    perror("Attachement de la socket impossible");
    close(desc); /* attachemnt impossible => fermeture de la socket */
    return(-1);
  }
  /* Recuperation de l'adresse effective d'attachement */
  if (ptr_adresse != NULL)
    getsockname(desc, (struct sockaddr *)ptr_adresse, & longueur);
  return desc;
}

