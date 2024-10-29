// agent_main.c
#include <stdio.h>
#include "/home/tuitachi/hust082022/lib/libeventloop/inc/eloop.h"
#include "/home/tuitachi/project_internVNPT/agent/inc/agent_socket.h"

int main() {
    // Thiết lập socket và địa chỉ server
    if (setup_socket(SERVER_IP, SERVER_PORT) < 0) {
        return -1;
    }

    // Khởi tạo eloop
    if (eloop_init() < 0) {
        fprintf(stderr, "Failed to initialize eloop\n");
        return -1;
    }

    // Đăng ký sự kiện đọc từ stdin (standard input) để nhận lựa chọn từ người dùng
    if (eloop_register_read_sock(STDIN_FILENO, handle_user_input, NULL, NULL) < 0) {
        fprintf(stderr, "Failed to register stdin with eloop\n");
        return -1;
    }

    // Bắt đầu vòng lặp sự kiện
    eloop_run();

    // Dọn dẹp và đóng socket
    eloop_destroy();
    close(sockfd);
    return 0;
}
