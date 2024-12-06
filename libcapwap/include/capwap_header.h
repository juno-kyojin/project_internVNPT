#ifndef CAPWAP_HEADER_H
#define CAPWAP_HEADER_H

#include <stdint.h>
#include <stdio.h>

// CAPWAP Header Struct without bit-fields
typedef struct {
    uint8_t version;
    uint8_t type;
    uint8_t hlen;
    uint8_t rid;
    uint8_t wbid;
    uint8_t t;
    uint8_t f;
    uint8_t l;
    uint8_t w;
    uint8_t m;
    uint8_t k;
    uint8_t flags;
    uint16_t frag_id;
    uint16_t frag_offset_reserved;
} CAPWAPHeader;

// Hàm serialize và deserialize cho CAPWAPHeader
size_t capwap_serialize_header(const CAPWAPHeader *header, uint8_t *buffer);
int capwap_deserialize_header(const uint8_t *buffer, CAPWAPHeader *header);

#endif // CAPWAP_HEADER_H