#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

extern int sockfd;
extern struct sockaddr_in server_addr, client_addr;

int setup_server_socket(int port);
void receive_messages();

#endif // SERVER_SOCKET_H
