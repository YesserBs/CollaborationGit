#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>

void handler_sigpipe(int sig) {
  printf("signal SIGPIPE recu\n");
  exit(2);
}

void print_msg(char *buf) {
  printf("-->buf = %s\n", buf);
}

void client_service(int sock, int argc, char *argv[]) {
  int rep;
  char buf[100];

  while(1) {
    scanf("%s",buf);
    strcat(buf, "\n");
    print_msg(buf);
    write(sock, buf, strlen(buf));
  }
}
