#include <stdio.h>
#include "server_socket.h"
#include "log.h"
#include "database.h"

int sockfd;
struct sockaddr_in server_addr, client_addr;

int main() {
    // Set up server socket
    if (setup_server_socket(SERVER_PORT) < 0) {
        fprintf(stderr, "Failed to setup server socket\n");
        return -1;
    }

    // Initialize logging system (if needed)
    init_log("server.log");

    printf("Server is running and waiting for messages...\n");

    // Start receiving messages from clients (agents)
    receive_messages();

    // Clean up
    close_log();
    close(sockfd);

    return 0;
}
