#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_control.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/src/capwap_header.c"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_preamble.h"
#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_message_element.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void send_udp_packet(int sockfd, const uint8_t *buffer, size_t buffer_size, const char *dst_ip, uint16_t dst_port) {
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(dst_port);
    dest_addr.sin_addr.s_addr = inet_addr(dst_ip);

    if (sendto(sockfd, buffer, buffer_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto failed");
    } else {
        printf("CAPWAP packet sent successfully!\n");
    }
}

size_t capwap_build_discovery_request(uint8_t *buffer, size_t buffer_size, const char *wtp_model, const char *wtp_serial) {
    size_t offset = 0;

    CAPWAPPreamble preamble = {CAPWAP_VERSION, 0};
    offset += capwap_serialize_preamble(&preamble, buffer + offset);

    CAPWAPHeader header = {0};
    header.version = CAPWAP_VERSION;
    header.type = 0;
    header.hlen = 3;
    header.rid = 1;
    header.wbid = CAPWAP_WBID_IEEE_802_11;
    offset += capwap_serialize_header(&header, buffer + offset);

    // Create control header
    CAPWAPControlHeader ch = {0};
    ch.message_type = 1; // Discovery Request
    ch.seq_num = 0;
    ch.message_length = 0;
    ch.flags = 0;
    size_t ch_offset = offset;
    offset += capwap_serialize_control_header(&ch, buffer + offset);

    // Create Message Elements
    CAPWAPMessageElement* dt_el = create_discovery_type_element(2);
    CAPWAPMessageElement* wb_el = create_wtp_board_data_element(wtp_model, wtp_serial);

    size_t elements_start = offset;

    // Serialize Message Element "Discovery Type"
    offset += capwap_serialize_message_element(dt_el, buffer + offset);
    // Serialize Message Element "WTP Board Data"
    offset += capwap_serialize_message_element(wb_el, buffer + offset);

    size_t elements_len = offset - elements_start;
    ch.message_length = (uint16_t)elements_len;

    // Serialize the control header back into the buffer
    capwap_serialize_control_header(&ch, buffer + ch_offset);

    // Free memory
    free(dt_el->value); free(dt_el);
    free(wb_el->value); free(wb_el);

    return offset;
}

size_t capwap_build_discovery_response(uint8_t *buffer, size_t buffer_size, const char *ac_name, uint16_t wtp_count) {
    size_t offset = 0;

    // Initialize CAPWAPPreamble
    CAPWAPPreamble preamble = {CAPWAP_VERSION, 0}; // Control message
    offset += capwap_serialize_preamble(&preamble, buffer + offset);

    // Initialize CAPWAPHeader
    CAPWAPHeader header = {0};
    header.version = CAPWAP_VERSION;
    header.type = 0; // 802.3 frame
    header.hlen = 3; // Header length (3 words)
    header.rid = 1; // Radio ID
    header.wbid = CAPWAP_WBID_IEEE_802_11;
    offset += capwap_serialize_header(&header, buffer + offset);

    // Create CAPWAPControlHeader
    CAPWAPControlHeader ch = {0};
    ch.message_type = 2; // Discovery Response
    ch.seq_num = 0; // Assume seq_num = 0
    ch.message_length = 0; // Update after adding Message Elements
    ch.flags = 0;
    size_t ch_offset = offset; // Save current offset position
    offset += capwap_serialize_control_header(&ch, buffer + offset);

    // Create Message Elements
    CAPWAPMessageElement *ac_descriptor_el = create_ac_descriptor_element();
    CAPWAPMessageElement *ac_name_el = create_ac_name_element(ac_name);
    CAPWAPMessageElement *wtp_radio_info_el = create_wtp_radio_info_element();
    CAPWAPMessageElement *capwap_control_ipv4_addr_el = create_capwap_control_ipv4_address_element(wtp_count);

    // Save the start position of Message Elements
    size_t elements_start = offset;

    // Serialize Message Elements into buffer
    offset += capwap_serialize_message_element(ac_descriptor_el, buffer + offset);
    offset += capwap_serialize_message_element(ac_name_el, buffer + offset);
    offset += capwap_serialize_message_element(wtp_radio_info_el, buffer + offset);
    offset += capwap_serialize_message_element(capwap_control_ipv4_addr_el, buffer + offset);

    // Update message_length in CAPWAPControlHeader
    size_t elements_len = offset - elements_start;
    ch.message_length = (uint16_t)elements_len;
    capwap_serialize_control_header(&ch, buffer + ch_offset); // Write back CAPWAPControlHeader

    // Free memory
    free(ac_descriptor_el->value);
    free(ac_descriptor_el);
    free(ac_name_el->value);
    free(ac_name_el);
    free(wtp_radio_info_el->value);
    free(wtp_radio_info_el);
    free(capwap_control_ipv4_addr_el->value);
    free(capwap_control_ipv4_addr_el);

    return offset;
}