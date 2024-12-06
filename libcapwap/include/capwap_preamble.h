#ifndef CAPWAP_PREAMBLE_H
#define CAPWAP_PREAMBLE_H

#include <stdint.h>
#include <stdio.h>
// CAPWAP Preamble Struct
typedef struct {
    uint8_t version : 4; // Version (4 bits)
    uint8_t type : 4;    // Payload Type (4 bits), = 0 for Control messages
} CAPWAPPreamble;

// Functions to serialize and deserialize CAPWAPPreamble
size_t capwap_serialize_preamble(const CAPWAPPreamble *preamble, uint8_t *buffer);
int capwap_deserialize_preamble(const uint8_t *buffer, CAPWAPPreamble *preamble);

#endif // CAPWAP_PREAMBLE_H