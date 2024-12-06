// tests/test_capwap_full.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "capwap.h"
#include "capwap_preamble.h"

int main() {
    uint8_t buffer[1024];
    size_t length = capwap_build_discovery_request(buffer, sizeof(buffer));

    if (length == 0) {
        fprintf(stderr, "Failed to build Discovery Request\n");
        return -1;
    }

    // Thêm đoạn mã in buffer để kiểm tra
    printf("Serialized Buffer (%zu bytes):\n", length);
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n\n");

    // Khởi tạo offset
    size_t offset = 0;

    // Deserialize CAPWAP Preamble
    CAPWAPPreamble preamble = {0};
    offset += capwap_deserialize_preamble(buffer + offset, &preamble);
    printf("CAPWAP Preamble:\n");
    printf("  Version: %u\n", preamble.version);
    printf("  Type: %u\n\n", preamble.type);

    // Deserialize CAPWAP Header
    CAPWAPHeader header = {0};
    offset += capwap_deserialize_header(buffer + offset, &header);
    printf("CAPWAP Header:\n");
    printf("  Version: %u\n", header.version);
    printf("  Type: %u\n", header.type);
    printf("  HLEN: %u\n", header.hlen);
    printf("  RID: %u\n", header.rid);
    printf("  WBID: %u\n", header.wbid);
    printf("  T: %u\n", header.t);
    printf("  F: %u\n", header.f);
    printf("  L: %u\n", header.l);
    printf("  W: %u\n", header.w);
    printf("  M: %u\n", header.m);
    printf("  K: %u\n", header.k);
    printf("  Flags: %u\n", header.flags);
    printf("  Frag ID: %u\n", header.frag_id);
    printf("  Frag Offset: %u\n", header.frag_offset);
    printf("  Reserved: %u\n\n", header.reserved);

    // Deserialize Control Header
    CAPWAPControlHeader control_header = {0};
    offset += capwap_deserialize_control_header(buffer + offset, &control_header);
    printf("Control Header:\n");
    printf("  Message Type: %u\n", control_header.message_type);
    printf("  Sequence Number: %u\n", control_header.seq_num);
    printf("  Message Length: %u\n", control_header.message_length);
    printf("  Flags: %u\n\n", control_header.flags);

    // Deserialize Message Elements
    size_t message_elements_length = control_header.message_length;
    size_t processed_length = 0;

    printf("Message Elements:\n");
    while (processed_length < message_elements_length) {
        CAPWAPMessageElement element = {0};
        size_t element_length = capwap_deserialize_message_element(buffer + offset + processed_length, &element);

        if (element_length == 0) {
            fprintf(stderr, "Failed to deserialize Message Element\n");
            break;
        }

        printf("  Type: %u\n", element.type);
        printf("  Length: %u\n", element.length);
        printf("  Value: ");
        for (int i = 0; i < element.length; i++) {
            printf("%02X ", element.value[i]);
        }
        printf("\n");

        free(element.value);
        processed_length += element_length;
    }

    return 0;
}