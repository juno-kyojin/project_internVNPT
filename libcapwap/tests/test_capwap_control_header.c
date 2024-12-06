// test_capwap_control_header.c

#include <stdio.h>
#include "/home/tuitachi/project1/libcapwap/include/capwap_control.h"

int main() {
    uint8_t buffer[8];
    CAPWAPControlHeader control_header = {0};

    // Khởi tạo các giá trị cho control header
    control_header.message_type = 1; // Discovery Request
    control_header.seq_num = 1;
    control_header.message_length = 0; // Sẽ cập nhật sau
    control_header.flags = 0;

    // Serialize control header
    size_t length = capwap_serialize_control_header(buffer, &control_header);

    // In ra kết quả
    printf("Serialized CAPWAP Control Header (%zu bytes):\n", length);
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    return 0;
}
