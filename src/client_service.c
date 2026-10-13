#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>

// Ceci est une autre modification
void handler_sigpipe(int sig);
void client_service(int sock, int argc, char *argv[]);

void handler_sigpipe(int sig) {
  printf("signal SIGPIPE recu\n");
  exit(2);
}

void client_service(int sock, int argc, char *argv[]) {
  int rep;
  char buf[100];

  while(1) {
    scanf("%s",buf);
    strcat(buf, "\n");
    printf("-->buf = %s\n", buf);
    write(sock, buf, strlen(buf));
  }
}
