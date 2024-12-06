#ifndef CAPWAP_MESSAGE_ELEMENT_H
#define CAPWAP_MESSAGE_ELEMENT_H

#include <stdint.h>
#include <stdlib.h>

// Message Element Types required for Discovery Request according to RFC5415:
#define CAPWAP_ELEMENT_TYPE_DISCOVERY_TYPE        20   
#define CAPWAP_ELEMENT_TYPE_WTP_BOARD_DATA        38  
#define CAPWAP_ELEMENT_TYPE_WTP_DESCRIPTOR        39   
#define CAPWAP_ELEMENT_TYPE_WTP_FRAME_TUNNEL_MODE 41 
#define CAPWAP_ELEMENT_TYPE_WTP_MAC_TYPE          44   
#define CAPWAP_ELEMENT_TYPE_WTP_RADIO_INFORMATION 1048 
#define CAPWAP_ELEMENT_TYPE_VENDOR_SPECIFIC       37   

typedef struct {
    uint16_t type;
    uint16_t length;
    uint8_t *value;
} CAPWAPMessageElement;

size_t capwap_serialize_message_element(const CAPWAPMessageElement *element, uint8_t *buffer);
size_t capwap_deserialize_message_element(const uint8_t *buffer, CAPWAPMessageElement *element);

// Discovery Type (1 byte), for example: 2 = DHCP
CAPWAPMessageElement* create_discovery_type_element(uint8_t discovery_type);

// WTP Board Data (Vendor ID=1, Model="ModelX", Serial="SN1234")
CAPWAPMessageElement* create_wtp_board_data_element(const char *model, const char *serial);

// WTP Descriptor
CAPWAPMessageElement* create_wtp_descriptor_element(void);

// WTP Frame Tunnel Mode (1 byte), assume=0
CAPWAPMessageElement* create_wtp_frame_tunnel_mode_element(uint8_t mode);

// WTP MAC Type (1 byte), 0=Local MAC
CAPWAPMessageElement* create_wtp_mac_type_element(uint8_t mac_type);

// WTP Radio Information (5 bytes), simple: Radio ID=1, 4 bytes 0
CAPWAPMessageElement* create_wtp_radio_info_element(void);

// Vendor-specific 
CAPWAPMessageElement* create_vendor_specific_element(uint32_t enterprise_number, const uint8_t *data, uint16_t data_length);

CAPWAPMessageElement* create_ac_descriptor_element(void);
CAPWAPMessageElement* create_ac_name_element(const char *ac_name);
CAPWAPMessageElement* create_capwap_control_ipv4_address_element(uint16_t wtp_count);

#endif // CAPWAP_MESSAGE_ELEMENT_H
