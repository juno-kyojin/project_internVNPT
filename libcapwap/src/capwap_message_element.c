#include "/home/tuitachi/project_internVNPT/libcapwap/include/capwap_message_element.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

size_t capwap_serialize_message_element(const CAPWAPMessageElement *element, uint8_t *buffer) {
    // Convert type and length to network byte order
    uint16_t t = htons(element->type);
    uint16_t l = htons(element->length);

    // Write type (2 bytes)
    memcpy(buffer, &t, 2);
    // Write length (2 bytes)
    memcpy(buffer+2, &l, 2);
    // Write value (length bytes)
    memcpy(buffer+4, element->value, element->length);

    return 4 + element->length;
}
size_t capwap_deserialize_message_element(const uint8_t *buffer, CAPWAPMessageElement *element) {
    return 0;
}

// Discovery Type (type=20, length=1, value=2=DHCP)
CAPWAPMessageElement* create_discovery_type_element(uint8_t discovery_type) {
    CAPWAPMessageElement *el = malloc(sizeof(*el));
    if (!el) return NULL;
    el->type = 20; // Type for Discovery Type
    el->length = 1;
    el->value = malloc(1);
    if (!el->value) {
        free(el);
        return NULL;
    }
    el->value[0] = discovery_type;
    return el;
}

// Function to create Board Data Sub-Element
size_t create_board_data_sub_element(uint16_t type, const uint8_t *data, uint16_t data_length, uint8_t *buffer) {
    uint16_t type_net = htons(type);
    uint16_t length_net = htons(data_length);

    memcpy(buffer, &type_net, 2);
    memcpy(buffer + 2, &length_net, 2);
    memcpy(buffer + 4, data, data_length);

    return 4 + data_length;
}

// WTP Board Data (Type 38)
CAPWAPMessageElement* create_wtp_board_data_element(const char *model, const char *serial) {
    uint32_t vendor_id = htonl(1); // Vendor Identifier, not 0

    // Create Board Data Sub-Elements
    uint8_t sub_elements[1024]; // Maximum size of 1024 bytes
    size_t offset = 0;

    // WTP Model Number Sub-Element
    offset += create_board_data_sub_element(0, (const uint8_t *)model, strlen(model), sub_elements + offset);

    // WTP Serial Number Sub-Element
    offset += create_board_data_sub_element(1, (const uint8_t *)serial, strlen(serial), sub_elements + offset);

    // Total length: Vendor Identifier (4 bytes) + Sub-Elements
    size_t total_length = 4 + offset;

    // Create Message Element
    CAPWAPMessageElement *el = malloc(sizeof(*el));
    if (!el) return NULL;

    el->type = 38; // WTP Board Data
    el->length = total_length;
    el->value = malloc(total_length);
    if (!el->value) {
        free(el);
        return NULL;
    }

    uint8_t *p = el->value;

    // Vendor Identifier
    memcpy(p, &vendor_id, 4); p += 4;

    // Board Data Sub-Elements
    memcpy(p, sub_elements, offset);

    return el;
}

// WTP Descriptor (type=39):
// max=1,radios=1,num_encrypt=1
// enc_sub={0x01,0x00,0x00}
// HW "HW1.0", SW "SW1.0", BT "BT1.0"
CAPWAPMessageElement* create_wtp_descriptor_element(void) {
    CAPWAPMessageElement *el = malloc(sizeof(*el));
    if (!el) return NULL;

    el->type = 39; // WTP Descriptor
    // Assume data is "HW1.0" (5 bytes), "SW1.0" (5 bytes), "BT1.0" (5 bytes)
    // WTP Descriptor data is more complex, this is a simple example:
    const char *hw="HW1.0";
    const char *sw="SW1.0";
    const char *bt="BT1.0";
    // Assume we concatenate these 3 strings into value
    // length = 5+5+5 = 15 bytes
    el->length = 15;
    el->value = malloc(el->length);
    if (!el->value) {
        free(el);
        return NULL;
    }
    memcpy(el->value, hw, 5);
    memcpy(el->value+5, sw, 5);
    memcpy(el->value+10, bt, 5);

    return el;
}


// WTP Frame Tunnel Mode (41), length=1, val=0
CAPWAPMessageElement* create_wtp_frame_tunnel_mode_element(uint8_t mode) {
    CAPWAPMessageElement* el=malloc(sizeof(*el));
    el->type=41;
    el->length=1;
    el->value=malloc(1);
    el->value[0]=mode;
    return el;
}

// WTP MAC Type(44), length=1, val=0
CAPWAPMessageElement* create_wtp_mac_type_element(uint8_t mac_type) {
    CAPWAPMessageElement* el=malloc(sizeof(*el));
    el->type=44;
    el->length=1;
    el->value=malloc(1);
    el->value[0]=mac_type;
    return el;
}

// WTP Radio Information(1048), length=5: [0x01][0x00,0x00,0x00,0x00]
CAPWAPMessageElement* create_wtp_radio_info_element(void) {
    CAPWAPMessageElement* el=malloc(sizeof(*el));
    el->type=1048;
    el->length=5;
    el->value=malloc(5);
    el->value[0]=1;
    memset(el->value+1,0,4);
    return el;
}

CAPWAPMessageElement* create_ac_descriptor_element(void) {
    // Simulate data for AC Descriptor
    uint16_t stations = htons(50); // Assume 50 stations
    uint16_t limit = htons(100); // Limit 100 stations
    uint16_t active_wtps = htons(20); // 20 active WTPs
    uint16_t max_wtps = htons(50); // Supports up to 50 WTPs
    uint8_t security = 0x22; // Supports X.509
    uint8_t r_mac_field = 1; // Supports Radio MAC Address
    uint8_t reserved1 = 0;
    uint8_t dtls_policy = 0x03; // Supports DTLS and clear text

    // Create Message Element
    size_t data_length = 12; // Data length
    CAPWAPMessageElement *el = malloc(sizeof(*el));
    if (!el) return NULL;
    el->type = 1; // AC Descriptor
    el->length = data_length;
    el->value = malloc(data_length);
    if (!el->value) {
        free(el);
        return NULL;
    }
    // Copy data into value
    memcpy(el->value, &stations, 2);
    memcpy(el->value + 2, &limit, 2);
    memcpy(el->value + 4, &active_wtps, 2);
    memcpy(el->value + 6, &max_wtps, 2);
    el->value[8] = security;
    el->value[9] = r_mac_field;
    el->value[10] = reserved1;
    el->value[11] = dtls_policy;
    return el;
}

// AC Name (Type 4)
CAPWAPMessageElement* create_ac_name_element(const char *ac_name) {
    // Create Message Element
    CAPWAPMessageElement *el = malloc(sizeof(*el));
    if (!el) return NULL;
    el->type = 4; // AC Name
    el->length = strlen(ac_name);
    el->value = malloc(el->length);
    if (!el->value) {
        free(el);
        return NULL;
    }
    strcpy((char *)el->value, ac_name);
    return el;
}

// CAPWAP Control IPv4 Address (Type 10)
CAPWAPMessageElement* create_capwap_control_ipv4_address_element(uint16_t wtp_count) {
    // Simulate data for CAPWAP Control IPv4 Address
    uint32_t ipv4_address = inet_addr("192.168.1.100"); // Replace with actual IP address
    uint16_t wtp_count_net = htons(wtp_count);

    // Create Message Element
    size_t data_length = 6; // Data length
    CAPWAPMessageElement *el = malloc(sizeof(*el));
    if (!el) return NULL;
    el->type = 10; // CAPWAP Control IPv4 Address
    el->length = data_length;
    el->value = malloc(data_length);
    if (!el->value) {
        free(el);
        return NULL;
    }
    // Copy data into value
    memcpy(el->value, &ipv4_address, 4);
    memcpy(el->value + 4, &wtp_count_net, 2);

    return el;
}
