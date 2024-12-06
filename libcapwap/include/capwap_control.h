#ifndef CAPWAP_CONTROL_H
#define CAPWAP_CONTROL_H
#include <stdio.h>
#include <stdint.h>

// CAPWAP Control Header Struct
typedef struct {
    uint32_t message_type;    // Message Type (4 bytes)
    uint8_t seq_num;          // Sequence Number (1 byte)
    uint16_t message_length;  // Message Length (2 bytes)
    uint8_t flags;            // Flags (1 byte)
} CAPWAPControlHeader;

// Functions to serialize and deserialize CAPWAPControlHeader
size_t capwap_serialize_control_header(const CAPWAPControlHeader *control_header, uint8_t *buffer);
int capwap_deserialize_control_header(const uint8_t *buffer, CAPWAPControlHeader *control_header);

#endif // CAPWAP_CONTROL_H