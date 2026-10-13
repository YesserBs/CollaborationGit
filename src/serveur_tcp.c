#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TRUE 1
#define _GNU_SOURCE

struct sigaction action;

int creer_socket(int, int *, struct sockaddr_in *);

void fonction_service(int socket)
{
  char buf[100],c;
  int i;

  while (1) {
    i = 0;
    do {
      read(socket, &c, 1);
      buf[i++] = c;
    } while(c != '\n');
    buf[i-1] = 0;
    
    printf("==>%s\n",buf); 
  }
}

void eliminer_zombie(int isg) {
    printf("terminaison d'un processus de service\n");
    wait(NULL);
}

main(int argc, char *argv[]){
  struct sockaddr_in adresse;
  int lg_adresse;
  int port;
  int socket_ecoute, socket_service;

  if (argc != 2) {
    fprintf(stderr,"Nombre de parametres incorrect\n");
    exit(2);
  }

  /* detachement du terminal */
  
  if (fork()!=0) exit(0);
  setsid();
  printf("serveur de pid %d lance \n", getpid());
  
  action.sa_handler=eliminer_zombie;
  sigaction(SIGCHLD, &action, NULL);

  port = atoi(argv[1]);
  lg_adresse = sizeof(adresse);
 
 /* creation et attachement de la socket d'ecoute */
 
 if ((socket_ecoute=creer_socket(SOCK_STREAM, &port, &adresse))==-1) {
   fprintf(stderr,"Creation socket ecoute impossible\n");
   exit(2);
 }
 printf("Socket d'ecoute sur le port %d\n\n",port);
 /* declaration d'ouverture du service */
 if (listen(socket_ecoute, 10) == -1) {
   perror("listen");
   exit(2);
 }
 /* boucle d'attente de connexion */
 
 while (TRUE) {
   socket_service = accept(socket_ecoute,(struct sockaddr *) &adresse, &lg_adresse);
   /*Reception d'un signal (probablement SIGHLD) */
   if (socket_service == -1 && errno==EINTR) continue;
   
   /* Erreur plus grave */
   if (socket_service == -1){
     perror("accept");
     exit(2);
   }

   /*****************************/   
   if (fork()==0){
     /* il n'utilise pas la socket d'ecoute */
     close(socket_ecoute);

     /* Lancement du service */
     printf("lancement du service\n");
     fonction_service(socket_service);
   }

   /* le serveur principal n'utilise pas socket_service */
   close(socket_service);

   /****************************
    if (thr_create(NULL,0,fonction_service,&socket_service, 0,NULL)) {
      fprintf(stderr, "Erreur creation pthread de service\n");
      close(socket_service);
    }
   ****************************/
   
 }
}
