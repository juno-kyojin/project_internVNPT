#include "/home/tuitachi/hust082022/lib/libeventloop/inc/eloop.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_control.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_header.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_preamble.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_message_element.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LISTEN_PORT 5246
#define BUFFER_SIZE CAPWAP_MAX_PACKET_SIZE

// Hàm xử lý sự kiện đọc socket
static void handle_sock_read(int sock, void *eloop_ctx, void *sock_ctx) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    uint8_t buffer[BUFFER_SIZE];
    ssize_t recv_len;
    const char *ac_name = "VNPT_AC";
    uint16_t wtp_count = 25;

    // Nhận gói tin Discovery Request từ WTP
    recv_len = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    if (recv_len < 0) {
        perror("recvfrom thất bại");
        return;
    }

    printf("Nhận được Discovery Request từ %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Xây dựng Discovery Response
    size_t response_len = capwap_build_discovery_response(buffer, BUFFER_SIZE, ac_name, wtp_count);
    if (response_len == 0) {
        fprintf(stderr, "Không thể xây dựng Discovery Response\n");
        return;
    }

    // Gửi Discovery Response về WTP
    send_udp_packet(sock, buffer, response_len, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Đã gửi Discovery Response tới %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Tạo socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Không thể tạo socket");
        exit(EXIT_FAILURE);
    }

    // Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(LISTEN_PORT);

    // Bind socket với địa chỉ và cổng
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind socket thất bại");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Khởi tạo eloop
    eloop_init();

    // Đăng ký socket với eloop
    eloop_register_read_sock(sockfd, handle_sock_read, NULL, NULL);

    printf("AC Listener đang chạy trên cổng %d...\n", LISTEN_PORT);

    // Chạy eloop
    eloop_run();

    // Giải phóng eloop
    eloop_destroy();

    close(sockfd);
    return 0;
}