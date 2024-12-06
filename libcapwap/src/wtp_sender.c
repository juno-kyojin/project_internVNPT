#include "capwap.h"
#include "capwap_control.h"
#include "capwap_header.h"
#include "capwap_preamble.h"
#include "capwap_message_element.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define AC_IP "127.0.0.1"      
#define AC_PORT 5246
#define BUFFER_SIZE CAPWAP_MAX_PACKET_SIZE

int main() {
    int sockfd;
    struct sockaddr_in ac_addr;
    socklen_t addr_len = sizeof(ac_addr);
    uint8_t buffer[BUFFER_SIZE];
    ssize_t recv_len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Không thể tạo socket");
        exit(EXIT_FAILURE);
    }

    memset(&ac_addr, 0, sizeof(ac_addr));
    ac_addr.sin_family = AF_INET;
    ac_addr.sin_port = htons(AC_PORT);
    if (inet_aton(AC_IP, &ac_addr.sin_addr) == 0) {
        fprintf(stderr, "Địa chỉ IP không hợp lệ\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Build Discovery Request
    size_t request_len = capwap_build_discovery_request(buffer, BUFFER_SIZE);
    if (request_len == 0) {
        fprintf(stderr, "Không thể xây dựng Discovery Request\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send Discovery Request to AC
    send_udp_packet(sockfd, buffer, request_len, AC_IP, AC_PORT);
    printf("Đã gửi Discovery Request tới %s:%d\n", AC_IP, AC_PORT);

    // Recv Discovery Response from AC
    recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (recv_len < 0) {
        perror("recvfrom thất bại");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Nhận được Discovery Response (%.2zu bytes)\n", recv_len);
    close(sockfd);
    return 0;
}