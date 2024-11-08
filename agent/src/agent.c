#include "/home/tuitachi/new_project/lib_management/lib_capwap/include/capwap.h"
#include "/home/tuitachi/new_project/lib_management/lib_eloop/inc/eloop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct {
    int sock;
    struct sockaddr_in server_addr;
} AgentData;

// Hàm để gửi tin nhắn CAPWAP từ agent đến server
void send_capwap_message(int sock, struct sockaddr_in *server_addr, CAPWAPMessageType msg_type) {
    CAPWAPMessage msg;
    capwap_create_message(&msg, msg_type, NULL, 0);

    printf("Đang gửi tin nhắn loại: %d đến server...\n", msg_type);
    if (capwap_udp_send(sock, &msg, server_addr) < 0) {
        perror("Lỗi khi gửi tin nhắn");
    } else {
        printf("Đã gửi tin nhắn loại: %d đến server\n", msg_type);
    }
    capwap_free_message(&msg);
}

// Gửi Keep-Alive định kỳ
void agent_periodic_keep_alive(void *eloop_data, void *user_ctx) {
    AgentData *agent_data = (AgentData *)user_ctx;
    send_capwap_message(agent_data->sock, &agent_data->server_addr, KEEP_ALIVE);
    printf("Đã gửi tin nhắn Keep-Alive định kỳ\n");

    // Đăng ký lại để tiếp tục gửi Keep-Alive sau 5 giây
    eloop_register_timeout(5, 0, agent_periodic_keep_alive, eloop_data, user_ctx);
}

int main() {
    int sock;
    AgentData agent_data;

    capwap_udp_init(&sock, 0);
    agent_data.sock = sock;
    agent_data.server_addr.sin_family = AF_INET;
    agent_data.server_addr.sin_port = htons(5246);
    inet_pton(AF_INET, "127.0.0.1", &agent_data.server_addr.sin_addr);


    if (eloop_init() < 0) {
        fprintf(stderr, "Khởi tạo eloop thất bại\n");
        return -1;
    }

    // 1. Bắt đầu quá trình Discovery
    send_capwap_message(agent_data.sock, &agent_data.server_addr, DISCOVERY_REQUEST);
    // Đợi và nhận DISCOVERY_RESPONSE từ server

    // 2. Gửi yêu cầu Join
    send_capwap_message(agent_data.sock, &agent_data.server_addr, JOIN_REQUEST);
    // Đợi và nhận JOIN_RESPONSE

    // 3. Yêu cầu Configuration Status
    send_capwap_message(agent_data.sock, &agent_data.server_addr, CONFIGURATION_STATUS_REQUEST);
    // Đợi và nhận CONFIGURATION_STATUS_RESPONSE

    // 4. Bắt đầu gửi Keep-Alive định kỳ
    eloop_register_timeout(5, 0, agent_periodic_keep_alive, NULL, &agent_data);
    printf("Đã bắt đầu gửi Keep-Alive định kỳ mỗi 5 giây\n");

    // 5. Yêu cầu thông tin đường truyền dữ liệu
    send_capwap_message(agent_data.sock, &agent_data.server_addr, DATA_PATH_REQUEST);
    // Đợi và nhận DATA_PATH_RESPONSE

    // Chạy vòng lặp sự kiện
    eloop_run();

    // Giải phóng tài nguyên sau khi hoàn tất
    capwap_udp_close(sock);
    eloop_destroy();
    return 0;
}
