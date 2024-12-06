#ifndef CAPWAP_H
#define CAPWAP_H

#include "capwap_header.h"
#include "capwap_control.h"
#include "capwap_message_element.h"
#include "capwap_preamble.h"


// Các định nghĩa và hàm chung

#define CAPWAP_VERSION 1
#define CAPWAP_WBID_IEEE_802_11 1
#define CAPWAP_MAX_PACKET_SIZE 1500  // Thông thường kích thước gói tối đa là 1500 bytes

// Hàm xây dựng Discovery Request
size_t capwap_build_discovery_request(uint8_t *buffer, size_t buffer_size);
size_t capwap_build_discovery_response(uint8_t *buffer, size_t buffer_size);
// Hàm gửi gói CAPWAP qua UDP
void send_udp_packet(int sockfd, const uint8_t *buffer, size_t buffer_size, const char *dst_ip, uint16_t dst_port);

#endif // CAPWAP_H