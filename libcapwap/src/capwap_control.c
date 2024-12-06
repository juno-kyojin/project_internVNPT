#include "/home/tuitachi/project1/libcapwap/include/capwap_control.h"
#include <string.h>
#include <arpa/inet.h>


// Hàm serialize CAPWAPControlHeader vào buffer
size_t capwap_serialize_control_header(const CAPWAPControlHeader *control_header, uint8_t *buffer) {
    size_t offset = 0;
    uint32_t mt_net = htonl(control_header->message_type);
    memcpy(buffer+offset, &mt_net, 4); offset+=4;
    buffer[offset++] = control_header->seq_num;
    uint16_t ml_net = htons(control_header->message_length);
    memcpy(buffer+offset, &ml_net, 2); offset+=2;
    buffer[offset++] = control_header->flags;
    return offset;
}

// Hàm deserialize CAPWAPControlHeader từ buffer
int capwap_deserialize_control_header(const uint8_t *buffer, CAPWAPControlHeader *control_header) {
    size_t offset = 0;

    // Deserialize message_type (4 bytes)
    control_header->message_type = ntohl(*(uint32_t *)(buffer + offset));
    offset += 4;

    // Deserialize seq_num (1 byte)
    control_header->seq_num = buffer[offset++];

    // Deserialize message_length (2 bytes)
    control_header->message_length = ntohs(*(uint16_t *)(buffer + offset));
    offset += 2;

    // Deserialize flags (1 byte)
    control_header->flags = buffer[offset++];

    return 0; // Thành công
}