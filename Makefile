CC = gcc

PRECIOUS=creer_socket.o

all: serveur_tcp client_tcp

nm: creer_socket.o
	nm creer_socket.o

creer_socket.o: creer_socket.c
	$(CC) -c creer_socket.c

serveur_tcp.o: serveur_tcp.c
	$(CC) -c serveur_tcp.c

client_tcp.o: client_tcp.c
	$(CC) -c client_tcp.c

client_service.o: client_service.c
	$(CC) -c client_service.c

serveur_tcp: serveur_tcp.o creer_socket.o
	$(CC) -o serveur_tcp serveur_tcp.o creer_socket.o

client_tcp: client_tcp.o client_service.o creer_socket.o
	$(CC) -o client_tcp client_tcp.o client_service.o \
                 creer_socket.o

clean:
	/bin/rm -f serveur_tcp.o creer_socket.o client_tcp.o \
                   client_service.o
	/bin/rm -f serveur_tcp client_tcp
