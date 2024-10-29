// agent_socket.c
#include <stdio.h>
#include <string.h>
#include "agent_socket.h"

int sockfd;
struct sockaddr_in server_addr;

int setup_socket(const char *server_ip, int server_port) {
    // Tạo socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Thiết lập địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        close(sockfd);
        return -1;
    }

    return 0;
}

void send_ip(void *eloop_ctx, void *user_ctx) {
    const char *ip_message = "IP: 192.168.1.100";
    if (sendto(sockfd, ip_message, strlen(ip_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Sendto failed");
    } else {
        printf("Agent: IP sent to server\n");
    }
}

void send_default_msg(void *eloop_ctx, void *user_ctx) {
    const char *default_message = "Hello from Agent";
    if (sendto(sockfd, default_message, strlen(default_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Sendto failed");
    } else {
        printf("Agent: Default message sent to server\n");
    }
}

void send_host_info(void *eloop_ctx, void *user_ctx) {
    const char *host_message = "Host: Agent-Host";
    if (sendto(sockfd, host_message, strlen(host_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Sendto failed");
    } else {
        printf("Agent: Host info sent to server\n");
    }
}

void send_custom_msg(void *eloop_ctx, void *user_ctx) {
    char custom_message[BUFFER_SIZE];
    printf("Enter your custom message: ");
    fgets(custom_message, BUFFER_SIZE, stdin);

    // Remove trailing newline
    custom_message[strcspn(custom_message, "\n")] = 0;

    if (sendto(sockfd, custom_message, strlen(custom_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Sendto failed");
    } else {
        printf("Agent: Custom message sent to server\n");
    }
}

void handle_user_input(int sock, void *eloop_ctx, void *user_ctx) {
    int choice;
    printf("\nChoose an option to send:\n");
    printf("1. Send IP\n");
    printf("2. Send default message\n");
    printf("3. Send host information\n");
    printf("4. Send custom message\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Clear the newline character left in the input buffer
    getchar();

    switch (choice) {
        case 1:
            send_ip(NULL, NULL);
            break;
        case 2:
            send_default_msg(NULL, NULL);
            break;
        case 3:
            send_host_info(NULL, NULL);
            break;
        case 4:
            send_custom_msg(NULL, NULL);
            break;
        case 5:
            printf("Exiting...\n");
            eloop_terminate();
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
}
