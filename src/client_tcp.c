#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int creer_socket(int, int *, struct sockaddr_in *);
void client_service(int, int, char **);
struct sockaddr_in adresse_serveur, adresse_client;

main(int argc, char *argv[]){
  int port;
  int socket_client;
  struct hostent *hp; /* pour l'adresse de la machine distante */

  if (argc<3) {
    fprintf(stderr, "erreur sur le nombre de parametres\n");
    exit(2);}
  /* Recherche de l'adresse Internet de la machine */
  
  if ((hp=gethostbyname(argv[1])) == NULL) {
    fprintf(stderr, "machine %s inconnue\n", argv[1]);
    exit(2);}
  
  /* creation et attachement de la socket du client (port quelconque) */
  
  port = 0;
  if ((socket_client=creer_socket(SOCK_STREAM, &port, &adresse_client))==-1){
    fprintf(stderr, "Creation socket du client impossible\n");
    exit(2);}
  printf("Client sur le port %d\n", ntohs(adresse_client.sin_port));
  /* Preparation de l'adresse du serveur */
  adresse_serveur.sin_family=AF_INET;
  adresse_serveur.sin_port=htons(atoi(argv[2]));
  memcpy(&adresse_serveur.sin_addr.s_addr, hp->h_addr, hp->h_length);
  
  /* demande de connexion au serveur */
  
  if (connect(socket_client, (struct sockaddr *)&adresse_serveur, sizeof(adresse_serveur))==-1){
    perror("connect");
    exit(2);}
  printf("connexion acceptee\n");
  client_service(socket_client, argc-3, argv+3);
}

