#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server_socket.h"
#include "msg_handle.h"
#include "log.h"

int sockfd;
struct sockaddr_in server_addr, client_addr;

int main() {
    // Thiết lập socket server
    if (setup_server_socket(SERVER_PORT) < 0) {
        fprintf(stderr, "Failed to setup server socket\n");
        return -1;
    }

    // Khởi tạo hệ thống log
    init_log("server.log");

    printf("Server is running and waiting for messages...\n");

    // Bắt đầu nhận tin nhắn từ các client (agents)
    receive_messages();

    // Dọn dẹp
    close_log();
    close(sockfd);

    return 0;
}
