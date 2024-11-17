// // capwap.h

// #ifndef CAPWAP_H
// #define CAPWAP_H

// #include <stdint.h>
// #include <netinet/in.h>

// // **Loại Thông Điệp CAPWAP**
// typedef enum {
//     DISCOVERY_REQUEST = 1,
//     DISCOVERY_RESPONSE = 2,
//     // Các loại thông điệp khác có thể được thêm vào đây
// } CAPWAPMessageType;

// // **Loại Phần Tử Tin Nhắn CAPWAP**
// #define CAPWAP_ELEMENT_WTP_BOARD_DATA     1
// #define CAPWAP_ELEMENT_WTP_DESCRIPTOR     2
// #define CAPWAP_ELEMENT_AC_DESCRIPTOR      3
// #define CAPWAP_ELEMENT_AC_NAME_PRIORITY   4

// // **Cấu Trúc Header của CAPWAP**
// typedef struct {
//     uint8_t version:4;
//     uint8_t type:4; // 0: Control Message, 1: Data Message
//     uint8_t flags;
//     uint16_t message_length;
//     uint16_t msg_type;
// } CAPWAPHeader;

// // **Cấu Trúc Phần Tử Tin Nhắn của CAPWAP**
// typedef struct {
//     uint16_t element_type;
//     uint16_t element_length;
//     uint8_t *element_data;
// } CAPWAPMessageElement;

// // **Cấu Trúc Toàn Bộ Thông Điệp CAPWAP**
// typedef struct {
//     CAPWAPHeader header;
//     CAPWAPMessageElement *elements;
//     uint16_t num_elements;
// } CAPWAPMessage;

// // **Cấu Trúc AC Descriptor**
// typedef struct {
//     uint32_t stations;
//     uint32_t limit;
//     uint32_t active_wtps;
//     uint32_t max_wtps;
//     uint8_t security;
//     uint8_t r_mac_field;
//     uint8_t reserved1;
// } ACDescriptor;

// // **Cấu Trúc AC Name with Priority**
// typedef struct {
//     uint8_t priority;
//     char *ac_name;
// } ACNameWithPriority;

// // **Cấu Trúc WTP Board Data**
// typedef struct {
//     char board_type[16];
//     char board_version[16];
// } WTPBoardData;

// // **Cấu Trúc WTP Descriptor**
// typedef struct {
//     char wtp_name[32];
//     char wtp_model[32];
// } WTPDescriptor;

// // **Cấu Trúc Session ID**
// typedef struct {
//     uint8_t session_id[16]; // 128-bit Session ID
// } CAPWAPSessionID;

// // **Giao Diện Hàm**

// /* Session Management */
// int capwap_init_session(CAPWAPSessionID *session_id);

// /* Message Handling */
// int capwap_create_discovery_request(CAPWAPMessage *msg);
// int capwap_create_discovery_response(CAPWAPMessage *msg, const ACDescriptor *ac_desc, const ACNameWithPriority *ac_name_priority);
// int capwap_update_message_length(CAPWAPMessage *msg); // Thêm dòng này
// int capwap_serialize_message(const CAPWAPMessage *msg, uint8_t *buffer, size_t buffer_size);
// int capwap_deserialize_message(CAPWAPMessage *msg, const uint8_t *buffer, size_t buffer_size);
// void capwap_free_message(CAPWAPMessage *msg);
// void print_capwap_message(const CAPWAPMessage *msg);

// /* Element Creation */
// int capwap_add_wtp_board_data_element(CAPWAPMessage *msg, const WTPBoardData *wtp_board_data);
// int capwap_add_wtp_descriptor_element(CAPWAPMessage *msg, const WTPDescriptor *wtp_descriptor);
// int capwap_add_ac_descriptor_element(CAPWAPMessage *msg, const ACDescriptor *ac_desc);
// int capwap_add_ac_name_priority_element(CAPWAPMessage *msg, const ACNameWithPriority *ac_name_priority);

// #endif // CAPWAP_H
// capwap.h

#ifndef CAPWAP_H
#define CAPWAP_H

#include "capwap_types.h"
#include "capwap_structs.h"
#include "capwap_functions.h"
#include "capwap_elements.h"

#endif // CAPWAP_H
