#include <stdio.h>
#include <time.h>
#include "/home/tuitachi/hust082022/lib/libeventloop/inc/eloop.h"
#include "/home/tuitachi/project_internVNPT/agent/inc/agent_socket.h"

char text_filename[100];  // Biến lưu tên file text

void generate_text_filename() {
    // Lấy thời gian hiện tại
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Tạo tên file với định dạng "dd-MM-yyyy_HH-mm-ss.txt"
    snprintf(text_filename, sizeof(text_filename), "%02d-%02d-%04d_%02d-%02d-%02d.txt",
             t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
             t->tm_hour, t->tm_min, t->tm_sec);
}

int main() {
    // Tạo tên file text khi khởi động
    generate_text_filename();

    // Thiết lập socket và địa chỉ server
    if (setup_socket(SERVER_IP, SERVER_PORT) < 0) {
        return -1;
    }

    // Gửi tên file text tới server
    if (sendto(sockfd, text_filename, strlen(text_filename), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to send text filename");
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
