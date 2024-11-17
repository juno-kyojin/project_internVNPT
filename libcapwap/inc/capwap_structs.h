// capwap_structs.h

#ifndef CAPWAP_STRUCTS_H
#define CAPWAP_STRUCTS_H

#include <stdint.h>

// **Cấu Trúc Header của CAPWAP**

/**
 * @brief Structure representing the CAPWAP message header.
 */
typedef struct {
    uint8_t version:4;
    uint8_t type:4; // 0: Control Message, 1: Data Message
    uint8_t flags;
    uint16_t message_length;
    uint16_t msg_type;
} CAPWAPHeader;

// **Cấu Trúc Phần Tử Tin Nhắn của CAPWAP**

/**
 * @brief Structure representing a CAPWAP message element.
 */
typedef struct {
    uint16_t element_type;
    uint16_t element_length;
    uint8_t *element_data;
} CAPWAPMessageElement;

// **Cấu Trúc Toàn Bộ Thông Điệp CAPWAP**

/**
 * @brief Structure representing a complete CAPWAP message.
 */
typedef struct {
    CAPWAPHeader header;
    CAPWAPMessageElement *elements;
    uint16_t num_elements;
} CAPWAPMessage;

// **Cấu Trúc AC Descriptor**

/**
 * @brief Structure representing the AC Descriptor.
 */
typedef struct {
    uint32_t stations;
    uint32_t limit;
    uint32_t active_wtps;
    uint32_t max_wtps;
    uint8_t security;
    uint8_t r_mac_field;
    uint8_t reserved1;
} ACDescriptor;

// **Cấu Trúc AC Name with Priority**

/**
 * @brief Structure representing the AC Name with Priority.
 */
typedef struct {
    uint8_t priority;
    char *ac_name;
} ACNameWithPriority;

// **Cấu Trúc WTP Board Data**

/**
 * @brief Structure representing the WTP Board Data.
 */
typedef struct {
    char board_type[16];
    char board_version[16];
} WTPBoardData;

// **Cấu Trúc WTP Descriptor**

/**
 * @brief Structure representing the WTP Descriptor.
 */
typedef struct {
    char wtp_name[32];
    char wtp_model[32];
} WTPDescriptor;

// **Cấu Trúc Session ID**

/**
 * @brief Structure representing the CAPWAP session ID.
 */
typedef struct {
    uint8_t session_id[16]; // 128-bit Session ID
} CAPWAPSessionID;

#endif // CAPWAP_STRUCTS_H
