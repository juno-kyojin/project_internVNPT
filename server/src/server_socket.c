#include <stdio.h>
#include <string.h>
#include "server_socket.h"
#include "msg_handle.h"

int setup_server_socket(int port) {
    // Tạo socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Thiết lập địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Gắn socket với địa chỉ và cổng
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }

    return 0;
}

void receive_messages() {
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(client_addr);

    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        // Null-terminate the received message
        buffer[n] = '\0';
        printf("Received message: %s\n", buffer);

        // Xử lý tin nhắn
        handle_message(buffer);
    }
}
