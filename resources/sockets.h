#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

typedef void (*server_handler)(int, char *, int);
typedef void (*connection_handler)(int);
// void server_handler(int socket, char *client_message, int data_length)

char client_message[16777216] = "";

int rwsocket(char *address, int portnum, char *senddata, char *towrite, int amount) {
		int socket_desc;
		struct sockaddr_in server;
		char *message;
	  char *server_reply = (char *) malloc(amount);

		socket_desc = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_desc == -1) {
				printf("Error while creating socket. Error code: \"%d\"\n", errno);
	    	return 1;
		}

		server.sin_addr.s_addr = inet_addr(address);
		server.sin_family = AF_INET;
		server.sin_port = htons(portnum);
		puts("Server socket created");

		if (connect(socket_desc, (struct sockaddr *) & server, sizeof(server)) < 0) {
				printf("Error while connecting to remote server. Error code: \"%d\" ", errno);
				if (errno == 111) {
						printf("(ERR_CONNECTION_REFUSED)");
				}
				if (errno == 101) {
						printf("(ERR_NETWORK_UNREACHABLE)");
				}
				printf("\n");
				return 1;
		}
		puts("Server socket connected");

		if (send(socket_desc, senddata, strlen(senddata), 0) < 0) {
				printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
				return 1;
		}
		puts("Sent message to server");

		if (recv(socket_desc, server_reply, amount, 0) < 0) {
				printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
		  	return 1;
		}
		strcpy(towrite, server_reply);
		close(socket_desc);
		return 0;
}

int *scanforopenports(char *address, char *buffer, int maximum) {
		int socket_desc;
		struct sockaddr_in server;
		int i;
		maximum = maximum + 1;
		int max = 65536;

		socket_desc = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_desc == -1) {
				printf("Error while creating socket. Error code: \"%d\"\n", errno);
				return NULL;
		}
		server.sin_addr.s_addr = inet_addr(address);
		server.sin_family = AF_INET;
		if (65536 > maximum) {
				max = maximum;
		}
		for (i = 1; i < max; i ++) {
			printf("Scanning port %d...\n", i);
			server.sin_port = htons(i);
			printf("Added sin_port to server struct\n");
			if (connect(socket_desc, (struct sockaddr *) & server, sizeof(server)) < 0) {
					buffer[i] = 0;
			} else {
					buffer[i] = 1;
					close(socket_desc);
					socket_desc = socket(AF_INET, SOCK_STREAM, 0);
					if (socket_desc == -1) {
							printf("Error while creating new socket. Error code: \"%d\"\n", errno);
							return NULL;
					}
			}
			printf("Finished connect test.\n");
		}
		close(socket_desc);
}

int srvsocket(int port, int maxreadsize, int maxmessagesize, server_handler func, connection_handler onconnect, connection_handler ondisconnect) {
		int socket_desc, new_socket, c;
		int read_size;
		char *message;
		struct sockaddr_in server, client;

		socket_desc = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_desc == -1) {
			printf("Could not create socket. Error code: \"%d\"\n", errno);
			return 1;
		}

		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(port);

		int optval = 1;
		setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
		optval = 1;
		setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

		if (bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
			printf("Binding of socket to server failed. Error code: \"%d\"\n", errno);
			close(socket_desc);
			return 2;
		}

		listen(socket_desc, 3);

		puts("Waiting for incoming connections...");
		c = sizeof(struct sockaddr_in);
		while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
			onconnect(new_socket);
/*
			message = "Connection recieved\n";
			write(new_socket, message, strlen(message));*/
			while((read_size = recv(new_socket, client_message, maxreadsize, 0)) > 0) {
				func(new_socket, client_message, read_size);
			}
			ondisconnect(new_socket);
		}

		if (new_socket < 0) {
			printf("Could not accept connection. Error code: \"%d\"\n", errno);
			close(socket_desc);
			return 3;
		}

		close(socket_desc);
		close(new_socket);
		puts("Exiting...");
		return 0;
}

int lookupIP(char *hostname, char *buffer) {
		char ip[128];
		struct hostent *he;
		struct in_addr **addr_list;
		int i;

		if ((he = gethostbyname(hostname)) == NULL) {
				printf("Error while using gethostbyname.\n");
				return 1;
		}

		addr_list = (struct in_addr **) he->h_addr_list;

		for (i = 0; addr_list[i] != NULL; i ++) {
				strcpy(ip, inet_ntoa(*addr_list[i]));
		}

		strcpy(buffer, ip);
		return 0;
}
