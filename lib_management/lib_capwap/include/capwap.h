#ifndef CAPWAP_H
#define CAPWAP_H

#include <stdint.h>
#include <netinet/in.h>


// CAPWAP Header Structure
typedef struct {
    uint8_t version_type;      // Version and Type
    uint8_t hlen_rid;          // Header Length and Radio ID
    uint8_t wbid;              // Wireless Binding ID
    uint8_t flags;             // Control Flags
    uint16_t message_length;   // Total Message Length
    uint16_t msg_type;         // Message Type - ADD THIS LINE HERE
    uint16_t fragment_id;      // Fragment ID
    uint16_t fragment_offset;  // Fragment Offset
} CAPWAPHeader;

// CAPWAP Message Types
typedef enum {
    DISCOVERY_REQUEST = 1,
    DISCOVERY_RESPONSE,
    JOIN_REQUEST,
    JOIN_RESPONSE,
    CONFIGURATION_STATUS_REQUEST,
    CONFIGURATION_STATUS_RESPONSE,
    KEEP_ALIVE,
    DATA_PATH_REQUEST,
    DATA_PATH_RESPONSE,
    CONFIGURATION_UPDATE
} CAPWAPMessageType;

// CAPWAP Message Element Types
typedef enum {
    ELEMENT_SSID = 1,
    ELEMENT_CHANNEL,
    ELEMENT_TX_POWER,
    ELEMENT_ENCRYPTION,
    ELEMENT_HEARTBEAT_INTERVAL,
    ELEMENT_CPU_USAGE,
    ELEMENT_MEMORY_USAGE,
    ELEMENT_NETWORK_TRAFFIC,
    ELEMENT_CLIENT_INFO,    // For client connection details (MAC, IP, state)
    ELEMENT_ALERT           // For warnings like overload or hardware failure
} CAPWAPMessageElementType;

// CAPWAP Message Element Structure
typedef struct {
    uint16_t element_type;    // Type of message element
    uint16_t element_length;  // Length of element data
    uint8_t *element_data;    // Pointer to element data
} CAPWAPMessageElement;

//  CAPWAP Message Structure
// typedef struct {
//     CAPWAPHeader header;          // CAPWAP Header
//     CAPWAPMessageType msg_type;   // Message Type
//     uint8_t *payload;             // Pointer to payload data (contains Message Elements)
//     uint16_t payload_len;         // Length of payload data
// } CAPWAPMessage;

typedef struct {
    CAPWAPHeader header;          // CAPWAP Header chứa msg_type
    uint8_t *payload;             // Pointer to payload data (contains Message Elements)
    uint16_t payload_len;         // Length of payload data
} CAPWAPMessage;

// CAPWAP Session ID
typedef struct {
    uint8_t session_id[16]; // 128-bit session identifier
} CAPWAPSessionID;

// Function Declarations

// CAPWAP session initialization
int capwap_init_session(CAPWAPSessionID *session);

// CAPWAP message creation and release
int capwap_create_message(CAPWAPMessage *msg, CAPWAPMessageType type, const CAPWAPMessageElement *elements, uint16_t num_elements);
void capwap_free_message(CAPWAPMessage *msg);

// CAPWAP protocol handlers
int capwap_handle_discovery(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server);
int capwap_handle_join(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server);
int capwap_handle_configuration(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server);
int capwap_handle_keep_alive(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server);
int capwap_handle_data_path(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server);
int capwap_handle_config_update(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr);

// CAPWAP UDP Transport functions
int capwap_udp_init(int *sock, int port);
int capwap_udp_send(int sock, const CAPWAPMessage *msg, struct sockaddr_in *dest_addr);
int capwap_udp_receive(int sock, CAPWAPMessage *msg, struct sockaddr_in *src_addr);
void capwap_udp_close(int sock);

#endif // CAPWAP_H