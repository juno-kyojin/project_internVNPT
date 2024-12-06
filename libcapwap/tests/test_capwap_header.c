// test_capwap_header.c

#include <stdio.h>
#include "/home/tuitachi/project1/libcapwap/include/capwap_header.h"

int main() {
    uint8_t buffer[8];
    CAPWAPHeader header = {0};

    // Khởi tạo các giá trị cho header
    header.version = 0;  // Version 0
    header.type = 0;     // Control Message
    header.hlen = 2;     // Header length in 4-byte words (8 bytes)
    header.rid = 0;      // Radio ID
    header.wbid = 1;     // IEEE 802.11
    header.t = 0;
    header.f = 0;
    header.l = 0;
    header.w = 0;
    header.m = 0;
    header.k = 0;
    header.flags = 0;
    header.frag_id = 0;
    header.frag_offset = 0;
    header.reserved = 0;

    // Serialize header
    size_t length = capwap_serialize_header(buffer, &header);

    // In ra kết quả
    printf("Serialized CAPWAP Header (%zu bytes):\n", length);
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    return 0;
}
