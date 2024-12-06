// capwap_preamble.c

#include "/home/tuitachi/project1/libcapwap/include/capwap_preamble.h"

// Hàm serialize CAPWAPPreamble vào buffer
size_t capwap_serialize_preamble(const CAPWAPPreamble *preamble, uint8_t *buffer) {
    buffer[0] = (preamble->version << 4) | (preamble->type & 0x0F);
    return 1; // Số byte đã serialize
}

// Hàm deserialize CAPWAPPreamble từ buffer
int capwap_deserialize_preamble(const uint8_t *buffer, CAPWAPPreamble *preamble) {
    preamble->version = (buffer[0] >> 4) & 0x0F;
    preamble->type = buffer[0] & 0x0F;
    return 0; // Thành công
}