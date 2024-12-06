#ifndef CAPWAP_H
#define CAPWAP_H

#include "capwap_header.h"
#include "capwap_control.h"
#include "capwap_message_element.h"
#include "capwap_preamble.h"


// Common definitions and functions

#define CAPWAP_VERSION 1
#define CAPWAP_WBID_IEEE_802_11 1
#define CAPWAP_MAX_PACKET_SIZE 1500  // Typically the maximum packet size is 1500 bytes

// Function to build Discovery Request
size_t capwap_build_discovery_request(uint8_t *buffer, size_t buffer_size, const char *wtp_model, const char *wtp_serial);
size_t capwap_build_discovery_response(uint8_t *buffer, size_t buffer_size, const char *ac_name, uint16_t wtp_count);
// Function to send CAPWAP packet via UDP
void send_udp_packet(int sockfd, const uint8_t *buffer, size_t buffer_size, const char *dst_ip, uint16_t dst_port);

#endif // CAPWAP_H