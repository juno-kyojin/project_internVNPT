// agent_socket.h
#ifndef AGENT_SOCKET_H
#define AGENT_SOCKET_H

#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"

extern int sockfd;
extern struct sockaddr_in server_addr;

void send_ip(void *eloop_ctx, void *user_ctx);
void send_default_msg(void *eloop_ctx, void *user_ctx);
void send_host_info(void *eloop_ctx, void *user_ctx);
void send_custom_msg(void *eloop_ctx, void *user_ctx);
void handle_user_input(int sock, void *eloop_ctx, void *user_ctx);

int setup_socket(const char *server_ip, int server_port);

#endif // AGENT_SOCKET_H
