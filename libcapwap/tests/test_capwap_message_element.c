// test_capwap_message_element.c

#include <stdio.h>
#include <stdlib.h>
#include "/home/tuitachi/project1/libcapwap/include/capwap_message_element.h"

int main() {
    uint8_t buffer[256];
    CAPWAPMessageElement element;

    // Khởi tạo một Message Element (ví dụ: Discovery Type)
    element.type = 20; // Discovery Type
    element.length = 1;
    element.value = malloc(element.length);
    element.value[0] = 0; // Unknown

    // Serialize Message Element
    size_t length = capwap_serialize_message_element(buffer, &element);

    // In ra kết quả
    printf("Serialized CAPWAP Message Element (%zu bytes):\n", length);
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // Giải phóng bộ nhớ
    free(element.value);

    return 0;
}
