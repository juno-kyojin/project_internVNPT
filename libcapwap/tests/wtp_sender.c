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

#define AC_IP "127.0.0.1"      
#define AC_PORT 5246
#define BUFFER_SIZE CAPWAP_MAX_PACKET_SIZE

// Hàm xử lý sự kiện đọc socket
static void handle_sock_read(int sock, void *eloop_ctx, void *sock_ctx) {
    uint8_t buffer[BUFFER_SIZE];
    ssize_t recv_len;

    // Nhận Discovery Response từ AC
    recv_len = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (recv_len < 0) {
        perror("recvfrom thất bại");
        return;
    }

    printf("Nhận được Discovery Response (%.2zu bytes)\n", recv_len);
}

int main() {
    int sockfd;
    struct sockaddr_in ac_addr;
    uint8_t buffer[BUFFER_SIZE];
    const char *wtp_model = "WTP_Model_ABC";
    const char *wtp_serial = "SN789012";

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

    // Khởi tạo eloop
    eloop_init();

    // Đăng ký socket với eloop
    eloop_register_read_sock(sockfd, handle_sock_read, NULL, NULL);

    // Xây dựng Discovery Request với thông tin WTP
    size_t request_len = capwap_build_discovery_request(buffer, BUFFER_SIZE, wtp_model, wtp_serial);
    if (request_len == 0) {
        fprintf(stderr, "Không thể xây dựng Discovery Request\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Gửi Discovery Request tới AC
    send_udp_packet(sockfd, buffer, request_len, AC_IP, AC_PORT);
    printf("Đã gửi Discovery Request tới %s:%d\n", AC_IP, AC_PORT);

    // Chạy eloop
    eloop_run();

    // Giải phóng eloop
    eloop_destroy();

    close(sockfd);
    return 0;
}