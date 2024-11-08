#include "/home/tuitachi/new_project/lib_management/lib_capwap/include/capwap.h"
#include "/home/tuitachi/new_project/lib_management/lib_eloop/inc/eloop.h"
#include <stdio.h>
#include <arpa/inet.h>

// Cấu trúc dữ liệu cho server
typedef struct {
    int sock;
    struct sockaddr_in agent_addr;
} ServerData;

// Callback để xử lý tin nhắn CAPWAP từ agent
void server_receive_callback(int sock, void *eloop_data, void *user_ctx) {
    ServerData *server_data = (ServerData *)user_ctx;
    CAPWAPMessage msg;

    // Nhận tin nhắn từ agent
    if (capwap_udp_receive(sock, &msg, &server_data->agent_addr) > 0) {
        printf("Đã nhận tin nhắn loại: %d\n", msg.header.msg_type);

        // Xử lý từng loại tin nhắn theo CAPWAP
        switch (msg.header.msg_type) {
            case DISCOVERY_REQUEST:
                printf("Nhận DISCOVERY_REQUEST từ agent\n");
                capwap_handle_discovery(&msg, sock, &server_data->agent_addr, 1);  
                break;

            case JOIN_REQUEST:
                printf("Nhận JOIN_REQUEST từ agent\n");
                capwap_handle_join(&msg, sock, &server_data->agent_addr, 1);  
                break;

            case CONFIGURATION_STATUS_REQUEST:
                printf("Nhận CONFIGURATION_STATUS_REQUEST từ agent\n");
                capwap_handle_configuration(&msg, sock, &server_data->agent_addr, 1);  
                break;

            case KEEP_ALIVE:
                printf("Nhận KEEP_ALIVE từ agent\n");
                // Xử lý tin nhắn KEEP_ALIVE nếu cần
                break;

            case DATA_PATH_REQUEST:
                printf("Nhận DATA_PATH_REQUEST từ agent\n");
                capwap_handle_data_path(&msg, sock, &server_data->agent_addr, 1);  
                break;

            case CONFIGURATION_UPDATE:
                printf("Nhận CONFIGURATION_UPDATE từ agent\n");
                capwap_handle_config_update(&msg, sock, &server_data->agent_addr);
                break;

            default:
                printf("Tin nhắn không xác định: %d\n", msg.header.msg_type);
                break;
        }

        // Giải phóng tài nguyên cho tin nhắn đã nhận
        capwap_free_message(&msg);
    }
}

int main() {
    int sock;
    ServerData server_data;

    // Khởi tạo socket và gán địa chỉ cho server
    if (capwap_udp_init(&sock, 5246) < 0) {
        fprintf(stderr, "Không thể khởi tạo UDP socket\n");
        return -1;
    }
    server_data.sock = sock;

    // Thiết lập sin_family cho agent_addr
    server_data.agent_addr.sin_family = AF_INET;

    // Khởi tạo eloop
    if (eloop_init() < 0) {
        fprintf(stderr, "Không thể khởi tạo eloop\n");
        return -1;
    }

    // Đăng ký hàm callback để nhận tin nhắn từ agent
    eloop_register_read_sock(sock, server_receive_callback, NULL, &server_data);

    printf("Server đang lắng nghe trên cổng 5246...\n");

    // Chạy vòng lặp sự kiện
    eloop_run();

    // Giải phóng tài nguyên sau khi hoàn tất
    capwap_udp_close(sock);
    eloop_destroy();

    return 0;
}
