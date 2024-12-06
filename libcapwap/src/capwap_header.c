#include "capwap_header.h"
#include <string.h>
#include <arpa/inet.h>

// Function to serialize CAPWAPHeader into buffer
size_t capwap_serialize_header(const CAPWAPHeader *header, uint8_t *buffer) {
    size_t offset = 0;

    buffer[offset++] = (header->version << 4) | ((header->type & 0x01) << 3) | (header->hlen & 0x07);
    buffer[offset++] = (header->rid << 3) | (header->wbid & 0x07);
    buffer[offset++] = ((header->t & 0x01) << 7) | ((header->f & 0x01) << 6) |
                       ((header->l & 0x01) << 5) | ((header->w & 0x01) << 4) |
                       ((header->m & 0x01) << 3) | ((header->k & 0x01) << 2) |
                       (header->flags & 0x03);

    uint16_t frag_id_net = htons(header->frag_id);
    memcpy(buffer + offset, &frag_id_net, sizeof(frag_id_net));
    offset += sizeof(frag_id_net);

    uint16_t frag_offset_reserved_net = htons(header->frag_offset_reserved);
    memcpy(buffer + offset, &frag_offset_reserved_net, sizeof(frag_offset_reserved_net));
    offset += sizeof(frag_offset_reserved_net);

    return offset;
}

// Hàm deserialize CAPWAPHeader từ buffer
int capwap_deserialize_header(const uint8_t *buffer, CAPWAPHeader *header) {
    size_t offset = 0;

    header->version = (buffer[offset] >> 4) & 0x0F;
    header->type = (buffer[offset] >> 3) & 0x01;
    header->hlen = buffer[offset++] & 0x07;

    header->rid = (buffer[offset] >> 3) & 0x1F;
    header->wbid = buffer[offset++] & 0x07;

    header->t = (buffer[offset] >> 7) & 0x01;
    header->f = (buffer[offset] >> 6) & 0x01;
    header->l = (buffer[offset] >> 5) & 0x01;
    header->w = (buffer[offset] >> 4) & 0x01;
    header->m = (buffer[offset] >> 3) & 0x01;
    header->k = (buffer[offset] >> 2) & 0x01;
    header->flags = buffer[offset++] & 0x03;

    header->frag_id = ntohs(*(uint16_t *)(buffer + offset));
    offset += sizeof(header->frag_id);

    header->frag_offset_reserved = ntohs(*(uint16_t *)(buffer + offset));
    offset += sizeof(header->frag_offset_reserved);

    return offset;
}