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
#include <signal.h>

#define AC_IP "127.0.0.1"      
#define AC_PORT 5246
#define BUFFER_SIZE CAPWAP_MAX_PACKET_SIZE

// Function to handle socket read events
static void handle_sock_read(int sock, void *eloop_ctx, void *sock_ctx) {
    uint8_t buffer[BUFFER_SIZE];
    ssize_t recv_len;

    // Receive Discovery Response from AC
    recv_len = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (recv_len < 0) {
        perror("recvfrom failed");
        return;
    }

    printf("Received Discovery Response (%.2zu bytes)\n", recv_len);
}

// Hàm xử lý tín hiệu
static void handle_signal(int sig) {
    if (sig == SIGINT) {
        eloop_terminate();
    }
}

int main() {
    int sockfd;
    struct sockaddr_in ac_addr;
    uint8_t buffer[BUFFER_SIZE];
    const char *wtp_model = "WTP_Model_ABC";
    const char *wtp_serial = "SN789012";

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&ac_addr, 0, sizeof(ac_addr));
    ac_addr.sin_family = AF_INET;
    ac_addr.sin_port = htons(AC_PORT);
    if (inet_aton(AC_IP, &ac_addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Initialize eloop
    eloop_init();

    // Đăng ký signal handler
    signal(SIGINT, handle_signal);

    // Register socket with eloop
    eloop_register_read_sock(sockfd, handle_sock_read, NULL, NULL);

    // Build Discovery Request with WTP information
    size_t request_len = capwap_build_discovery_request(buffer, BUFFER_SIZE, wtp_model, wtp_serial);
    if (request_len == 0) {
        fprintf(stderr, "Cannot build Discovery Request\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send Discovery Request to AC
    send_udp_packet(sockfd, buffer, request_len, AC_IP, AC_PORT);
    printf("Sent Discovery Request to %s:%d\n", AC_IP, AC_PORT);

    // Free memory allocated by capwap_build_discovery_request if needed
    eloop_run();

    // Destroy eloop
    eloop_destroy();

    close(sockfd);
    return 0;
}