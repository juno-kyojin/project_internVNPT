#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "capwap.h"  // Chứa các khai báo hàm, struct cần thiết

// Giả sử chúng ta đã có hàm create_wtp_descriptor_element() từ ví dụ trước
extern CAPWAPMessageElement* create_wtp_descriptor_element(void);

// Hàm xây dựng Discovery Request chỉ với WTP Descriptor
size_t build_discovery_request_with_only_wtp_descriptor(uint8_t *buffer, size_t buffer_size) {
    size_t offset = 0;

    // Tạo Preamble
    CAPWAPPreamble preamble = {CAPWAP_VERSION,0};
    offset += capwap_serialize_preamble(&preamble, buffer+offset);

    // Tạo CAPWAP Header (giả lập đơn giản)
    CAPWAPHeader header = {0};
    header.version = CAPWAP_VERSION;
    header.type = 0; // Control
    header.hlen = 3; // Giả định preamble+header=12 bytes
    header.rid = 1;
    header.wbid = CAPWAP_WBID_IEEE_802_11; 
    offset += capwap_serialize_header(&header, buffer+offset);

    // Tạo Control Header
    CAPWAPControlHeader ch = {0};
    ch.message_type = htonl(1); // Discovery Request
    ch.seq_num=0;
    ch.message_length=0; // sẽ cập nhật sau
    ch.flags=0;
    size_t ch_offset=offset;
    offset += capwap_serialize_control_header(&ch, buffer+offset);

    // Tạo WTP Descriptor Element
    CAPWAPMessageElement *wtp_desc_el = create_wtp_descriptor_element();
    if(!wtp_desc_el) {
        fprintf(stderr,"Không thể tạo WTP Descriptor Element\n");
        return 0;
    }

    size_t elements_start = offset;

    // Serialize WTP Descriptor Element
    offset += capwap_serialize_message_element(wtp_desc_el, buffer+offset);

    size_t elements_length = offset - elements_start;

    // Cập nhật message_length
    ch.message_length = htons((uint16_t)elements_length);
    memcpy(buffer+ch_offset, &ch, sizeof(ch));

    free(wtp_desc_el->value);
    free(wtp_desc_el);

    return offset;
}

int main() {
    uint8_t buffer[CAPWAP_MAX_PACKET_SIZE];
    size_t pkt_len = build_discovery_request_with_only_wtp_descriptor(buffer, sizeof(buffer));
    if(pkt_len == 0) {
        fprintf(stderr,"Xây dựng gói Discovery Request thất bại\n");
        return 1;
    }

    // Gửi gói tin qua UDP
    send_udp_packet(buffer, pkt_len, "127.0.0.1", 5246);

    printf("Đã gửi Discovery Request chỉ với WTP Descriptor. Mở Wireshark (udp.port==5246) để xem.\n");
    return 0;
}
