// capwap_preamble.c

#include "/home/tuitachi/project1/libcapwap/include/capwap_preamble.h"

// Function to serialize CAPWAPPreamble into buffer
size_t capwap_serialize_preamble(const CAPWAPPreamble *preamble, uint8_t *buffer) {
    buffer[0] = (preamble->version << 4) | (preamble->type & 0x0F);
    return 1; // Number of bytes serialized
}

// Function to deserialize CAPWAPPreamble from buffer
int capwap_deserialize_preamble(const uint8_t *buffer, CAPWAPPreamble *preamble) {
    preamble->version = (buffer[0] >> 4) & 0x0F;
    preamble->type = buffer[0] & 0x0F;
    return 0; // Success
}