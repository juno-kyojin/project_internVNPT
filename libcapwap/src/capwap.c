// capwap.c

#include "/home/tuitachi/project_provip/libcapwap/inc/capwap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <openssl/rand.h>

/* Session Management */

/**
 * @brief Generates a random 128-bit session ID using OpenSSL's RAND_bytes.
 * 
 * @param session_id Pointer to CAPWAPSessionID structure to store the generated session ID.
 * @return int 0 on success, -1 on failure.
 */
int capwap_init_session(CAPWAPSessionID *session_id) {
    if (session_id == NULL) {
        return -1;
    }

    if (RAND_bytes(session_id->session_id, sizeof(session_id->session_id)) != 1) {
        return -1;
    }

    return 0;
}

/* Message Handling */

/**
 * @brief Creates a Discovery Request message.
 * 
 * @param msg Pointer to CAPWAPMessage structure to initialize.
 * @return int 0 on success, -1 on failure.
 */
int capwap_create_discovery_request(CAPWAPMessage *msg) {
    if (msg == NULL) return -1;

    // Initialize header
    memset(&msg->header, 0, sizeof(CAPWAPHeader));
    msg->header.version = 1; // Version 1
    msg->header.type = 0;    // Control Message
    msg->header.flags = 0;
    msg->header.msg_type = DISCOVERY_REQUEST;
    msg->num_elements = 0;
    msg->elements = NULL;

    // Calculate message length (no elements initially)
    msg->header.message_length = 0;

    return 0;
}

/**
 * @brief Creates a Discovery Response message with AC Descriptor and AC Name with Priority.
 * 
 * @param msg Pointer to CAPWAPMessage structure to populate.
 * @param ac_desc Pointer to ACDescriptor structure containing AC information.
 * @param ac_name_priority Pointer to ACNameWithPriority structure containing AC name and priority.
 * @return int 0 on success, -1 on failure.
 */
int capwap_create_discovery_response(CAPWAPMessage *msg, const ACDescriptor *ac_desc, const ACNameWithPriority *ac_name_priority) {
    if (msg == NULL || ac_desc == NULL || ac_name_priority == NULL) return -1;

    // Initialize header
    memset(&msg->header, 0, sizeof(CAPWAPHeader));
    msg->header.version = 1; // Version 1
    msg->header.type = 0;    // Control Message
    msg->header.flags = 0;
    msg->header.msg_type = DISCOVERY_RESPONSE;
    msg->num_elements = 0;
    msg->elements = NULL;

    // Add AC Descriptor
    if (capwap_add_ac_descriptor_element(msg, ac_desc) != 0) {
        fprintf(stderr, "capwap_create_discovery_response: Failed to add AC Descriptor element\n");
        capwap_free_message(msg);
        return -1;
    }

    // Add AC Name with Priority
    if (capwap_add_ac_name_priority_element(msg, ac_name_priority) != 0) {
        fprintf(stderr, "capwap_create_discovery_response: Failed to add AC Name with Priority element\n");
        capwap_free_message(msg);
        return -1;
    }

    // Update message_length based on current elements
    if (capwap_update_message_length(msg) != 0) {
        fprintf(stderr, "capwap_create_discovery_response: Failed to update message_length\n");
        capwap_free_message(msg);
        return -1;
    }

    return 0;
}

/**
 * @brief Updates the message_length of a CAPWAP message based on its elements.
 * 
 * @param msg Pointer to CAPWAPMessage structure to update.
 * @return int 0 on success, -1 on failure.
 */
int capwap_update_message_length(CAPWAPMessage *msg) {
    if (msg == NULL) return -1;

    uint16_t total_length = 0;
    for (uint16_t i = 0; i < msg->num_elements; i++) {
        total_length += sizeof(uint16_t) * 2; // element_type + element_length
        total_length += msg->elements[i].element_length;
    }
    msg->header.message_length = total_length;

    return 0;
}

/* Element Creation */

/**
 * @brief Adds a WTP Board Data message element to a CAPWAP message.
 * 
 * @param msg Pointer to CAPWAPMessage structure.
 * @param wtp_board_data Pointer to WTPBoardData structure containing WTP board information.
 * @return int 0 on success, -1 on failure.
 */
int capwap_add_wtp_board_data_element(CAPWAPMessage *msg, const WTPBoardData *wtp_board_data) {
    if (msg == NULL || wtp_board_data == NULL) return -1;

    CAPWAPMessageElement element;
    memset(&element, 0, sizeof(CAPWAPMessageElement));

    element.element_type = CAPWAP_ELEMENT_WTP_BOARD_DATA;

    // Fixed size fields: board_type (16 bytes) + board_version (16 bytes)
    element.element_length = sizeof(wtp_board_data->board_type) + sizeof(wtp_board_data->board_version);

    // Allocate memory for element data
    element.element_data = (uint8_t *)malloc(element.element_length);
    if (element.element_data == NULL) {
        return -1;
    }

    size_t offset = 0;
    memcpy(element.element_data + offset, wtp_board_data->board_type, sizeof(wtp_board_data->board_type));
    offset += sizeof(wtp_board_data->board_type);

    memcpy(element.element_data + offset, wtp_board_data->board_version, sizeof(wtp_board_data->board_version));
    offset += sizeof(wtp_board_data->board_version);

    // Add element to message
    CAPWAPMessageElement *new_elements = realloc(msg->elements, (msg->num_elements + 1) * sizeof(CAPWAPMessageElement));
    if (new_elements == NULL) {
        free(element.element_data);
        return -1;
    }
    msg->elements = new_elements;
    msg->elements[msg->num_elements] = element;
    msg->num_elements += 1;

    // Update message_length after adding the element
    if (capwap_update_message_length(msg) != 0) {
        fprintf(stderr, "capwap_add_wtp_board_data_element: Failed to update message_length\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Adds a WTP Descriptor message element to a CAPWAP message.
 * 
 * @param msg Pointer to CAPWAPMessage structure.
 * @param wtp_descriptor Pointer to WTPDescriptor structure containing WTP descriptor information.
 * @return int 0 on success, -1 on failure.
 */
int capwap_add_wtp_descriptor_element(CAPWAPMessage *msg, const WTPDescriptor *wtp_descriptor) {
    if (msg == NULL || wtp_descriptor == NULL) return -1;

    CAPWAPMessageElement element;
    memset(&element, 0, sizeof(CAPWAPMessageElement));

    element.element_type = CAPWAP_ELEMENT_WTP_DESCRIPTOR;

    // Fixed size fields: wtp_name (32 bytes) + wtp_model (32 bytes)
    element.element_length = sizeof(wtp_descriptor->wtp_name) + sizeof(wtp_descriptor->wtp_model);

    // Allocate memory for element data
    element.element_data = (uint8_t *)malloc(element.element_length);
    if (element.element_data == NULL) {
        return -1;
    }

    size_t offset = 0;
    memcpy(element.element_data + offset, wtp_descriptor->wtp_name, sizeof(wtp_descriptor->wtp_name));
    offset += sizeof(wtp_descriptor->wtp_name);

    memcpy(element.element_data + offset, wtp_descriptor->wtp_model, sizeof(wtp_descriptor->wtp_model));
    offset += sizeof(wtp_descriptor->wtp_model);

    // Add element to message
    CAPWAPMessageElement *new_elements = realloc(msg->elements, (msg->num_elements + 1) * sizeof(CAPWAPMessageElement));
    if (new_elements == NULL) {
        free(element.element_data);
        return -1;
    }
    msg->elements = new_elements;
    msg->elements[msg->num_elements] = element;
    msg->num_elements += 1;

    // Update message_length after adding the element
    if (capwap_update_message_length(msg) != 0) {
        fprintf(stderr, "capwap_add_wtp_descriptor_element: Failed to update message_length\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Adds an AC Descriptor message element to a CAPWAP message.
 * 
 * @param msg Pointer to CAPWAPMessage structure.
 * @param ac_desc Pointer to ACDescriptor structure containing AC information.
 * @return int 0 on success, -1 on failure.
 */
int capwap_add_ac_descriptor_element(CAPWAPMessage *msg, const ACDescriptor *ac_desc) {
    if (msg == NULL || ac_desc == NULL) return -1;

    CAPWAPMessageElement element;
    memset(&element, 0, sizeof(CAPWAPMessageElement));

    element.element_type = CAPWAP_ELEMENT_AC_DESCRIPTOR;

    // Fixed fields: stations, limit, active_wtps, max_wtps, security, r_mac_field, reserved1
    size_t fixed_fields_size = sizeof(uint32_t) * 4 + sizeof(uint8_t) * 3; // 4*4 + 3 = 19 bytes

    // AC Information Sub-elements: Hardware Version and Software Version
    // Each sub-element: vendor_id (4 bytes) + type (2 bytes) + length (2 bytes) + data (7 bytes) = 15 bytes
    size_t ac_info_sub_elements_size = 15 * 2; // 2 sub-elements

    // Total element length
    element.element_length = fixed_fields_size + ac_info_sub_elements_size;

    // Allocate memory for element data
    element.element_data = (uint8_t *)malloc(element.element_length);
    if (element.element_data == NULL) {
        return -1;
    }

    size_t offset = 0;

    // Populate fixed fields
    uint32_t net_stations = htonl(ac_desc->stations);
    memcpy(element.element_data + offset, &net_stations, sizeof(net_stations));
    offset += sizeof(net_stations);

    uint32_t net_limit = htonl(ac_desc->limit);
    memcpy(element.element_data + offset, &net_limit, sizeof(net_limit));
    offset += sizeof(net_limit);

    uint32_t net_active_wtps = htonl(ac_desc->active_wtps);
    memcpy(element.element_data + offset, &net_active_wtps, sizeof(net_active_wtps));
    offset += sizeof(net_active_wtps);

    uint32_t net_max_wtps = htonl(ac_desc->max_wtps);
    memcpy(element.element_data + offset, &net_max_wtps, sizeof(net_max_wtps));
    offset += sizeof(net_max_wtps);

    memcpy(element.element_data + offset, &ac_desc->security, sizeof(ac_desc->security));
    offset += sizeof(ac_desc->security);

    memcpy(element.element_data + offset, &ac_desc->r_mac_field, sizeof(ac_desc->r_mac_field));
    offset += sizeof(ac_desc->r_mac_field);

    memcpy(element.element_data + offset, &ac_desc->reserved1, sizeof(ac_desc->reserved1));
    offset += sizeof(ac_desc->reserved1);

    // AC Information Sub-element 1: Hardware Version
    uint32_t vendor_id = htonl(0x00000000); // Vendor Identifier = 0
    uint16_t hw_type = htons(4);             // Hardware Version Type
    uint16_t hw_length = htons(7);           // Length of Hardware Version Data
    const char hardware_version[7] = {'H','W','1','.','0','.','0'};

    memcpy(element.element_data + offset, &vendor_id, sizeof(vendor_id));
    offset += sizeof(vendor_id);

    memcpy(element.element_data + offset, &hw_type, sizeof(hw_type));
    offset += sizeof(hw_type);

    memcpy(element.element_data + offset, &hw_length, sizeof(hw_length));
    offset += sizeof(hw_length);

    memcpy(element.element_data + offset, hardware_version, sizeof(hardware_version));
    offset += sizeof(hardware_version);

    // AC Information Sub-element 2: Software Version
    uint16_t sw_type = htons(5);             // Software Version Type
    uint16_t sw_length = htons(7);           // Length of Software Version Data
    const char software_version[7] = {'S','W','1','.','0','.','0'};

    memcpy(element.element_data + offset, &vendor_id, sizeof(vendor_id));
    offset += sizeof(vendor_id);

    memcpy(element.element_data + offset, &sw_type, sizeof(sw_type));
    offset += sizeof(sw_type);

    memcpy(element.element_data + offset, &sw_length, sizeof(sw_length));
    offset += sizeof(sw_length);

    memcpy(element.element_data + offset, software_version, sizeof(software_version));
    offset += sizeof(software_version);

    // Debugging: Print AC Information Sub-elements
    printf("Debug: AC Information Sub-element 1 (Hardware Version) Data: %.*s\n", 7, hardware_version);
    printf("Debug: AC Information Sub-element 2 (Software Version) Data: %.*s\n", 7, software_version);

    // Add element to message
    CAPWAPMessageElement *new_elements = realloc(msg->elements, (msg->num_elements + 1) * sizeof(CAPWAPMessageElement));
    if (new_elements == NULL) {
        free(element.element_data);
        return -1;
    }
    msg->elements = new_elements;
    msg->elements[msg->num_elements] = element;
    msg->num_elements += 1;

    // Update message_length after adding the element
    if (capwap_update_message_length(msg) != 0) {
        fprintf(stderr, "capwap_add_ac_descriptor_element: Failed to update message_length\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Adds an AC Name with Priority message element to a CAPWAP message.
 * 
 * @param msg Pointer to CAPWAPMessage structure.
 * @param ac_name_priority Pointer to ACNameWithPriority structure containing AC name and priority.
 * @return int 0 on success, -1 on failure.
 */
int capwap_add_ac_name_priority_element(CAPWAPMessage *msg, const ACNameWithPriority *ac_name_priority) {
    if (msg == NULL || ac_name_priority == NULL || ac_name_priority->ac_name == NULL) return -1;

    CAPWAPMessageElement element;
    memset(&element, 0, sizeof(CAPWAPMessageElement));

    element.element_type = CAPWAP_ELEMENT_AC_NAME_PRIORITY;

    size_t ac_name_length = strlen(ac_name_priority->ac_name);
    element.element_length = sizeof(uint8_t) + ac_name_length; // Priority (1 byte) + AC Name

    element.element_data = (uint8_t *)malloc(element.element_length);
    if (element.element_data == NULL) {
        return -1;
    }

    size_t offset = 0;
    memcpy(element.element_data + offset, &ac_name_priority->priority, sizeof(ac_name_priority->priority));
    offset += sizeof(ac_name_priority->priority);

    memcpy(element.element_data + offset, ac_name_priority->ac_name, ac_name_length);
    offset += ac_name_length;

    // Add element to message
    CAPWAPMessageElement *new_elements = realloc(msg->elements, (msg->num_elements + 1) * sizeof(CAPWAPMessageElement));
    if (new_elements == NULL) {
        free(element.element_data);
        return -1;
    }
    msg->elements = new_elements;
    msg->elements[msg->num_elements] = element;
    msg->num_elements += 1;

    // Update message_length after adding the element
    if (capwap_update_message_length(msg) != 0) {
        fprintf(stderr, "capwap_add_ac_name_priority_element: Failed to update message_length\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Serializes a CAPWAP message into a buffer.
 * 
 * @param msg Pointer to CAPWAPMessage structure.
 * @param buffer Buffer to serialize the message into.
 * @param buffer_size Size of the buffer.
 * @return int Number of bytes written to the buffer, or -1 on failure.
 */
int capwap_serialize_message(const CAPWAPMessage *msg, uint8_t *buffer, size_t buffer_size) {
    if (msg == NULL || buffer == NULL) return -1;

    size_t offset = 0;

    // Serialize Header (6 bytes)
    if (buffer_size < 6) return -1;

    buffer[offset] = ((msg->header.version & 0x0F) << 4) | (msg->header.type & 0x0F);
    offset += 1;

    buffer[offset] = msg->header.flags;
    offset += 1;

    uint16_t net_message_length = htons(msg->header.message_length);
    memcpy(buffer + offset, &net_message_length, sizeof(net_message_length));
    offset += sizeof(net_message_length);

    uint16_t net_msg_type = htons(msg->header.msg_type);
    memcpy(buffer + offset, &net_msg_type, sizeof(net_msg_type));
    offset += sizeof(net_msg_type);

    // Serialize Message Elements
    for (uint16_t i = 0; i < msg->num_elements; i++) {
        const CAPWAPMessageElement *element = &msg->elements[i];

        // Check buffer size
        if (offset + sizeof(uint16_t) * 2 + element->element_length > buffer_size) return -1;

        uint16_t net_element_type = htons(element->element_type);
        memcpy(buffer + offset, &net_element_type, sizeof(net_element_type));
        offset += sizeof(net_element_type);

        uint16_t net_element_length = htons(element->element_length);
        memcpy(buffer + offset, &net_element_length, sizeof(net_element_length));
        offset += sizeof(net_element_length);

        memcpy(buffer + offset, element->element_data, element->element_length);
        offset += element->element_length;
    }

    return offset; // Total bytes written
}

/**
 * @brief Deserializes a buffer into a CAPWAP message structure.
 * 
 * @param msg Pointer to CAPWAPMessage structure to populate.
 * @param buffer Buffer containing the serialized message.
 * @param buffer_size Size of the buffer.
 * @return int 0 on success, -1 on failure.
 */
int capwap_deserialize_message(CAPWAPMessage *msg, const uint8_t *buffer, size_t buffer_size) {
    if (msg == NULL || buffer == NULL) return -1;

    size_t offset = 0;

    if (buffer_size < 6) return -1; // Minimum header size

    // Deserialize Header
    msg->header.version = (buffer[offset] >> 4) & 0x0F;
    msg->header.type = buffer[offset] & 0x0F;
    offset += 1;

    msg->header.flags = buffer[offset];
    offset += 1;

    uint16_t net_message_length;
    memcpy(&net_message_length, buffer + offset, sizeof(net_message_length));
    msg->header.message_length = ntohs(net_message_length);
    offset += sizeof(net_message_length);

    uint16_t net_msg_type;
    memcpy(&net_msg_type, buffer + offset, sizeof(net_msg_type));
    msg->header.msg_type = ntohs(net_msg_type);
    offset += sizeof(net_msg_type);

    // Initialize elements
    msg->elements = NULL;
    msg->num_elements = 0;

    // Deserialize Message Elements
    while (offset + 4 <= buffer_size && offset < 6 + msg->header.message_length) {
        uint16_t net_element_type;
        memcpy(&net_element_type, buffer + offset, sizeof(net_element_type));
        uint16_t element_type = ntohs(net_element_type);
        offset += sizeof(net_element_type);

        uint16_t net_element_length;
        memcpy(&net_element_length, buffer + offset, sizeof(net_element_length));
        uint16_t element_length = ntohs(net_element_length);
        offset += sizeof(net_element_length);

        if (offset + element_length > buffer_size) {
            capwap_free_message(msg);
            return -1;
        }

        // Allocate memory for element data
        uint8_t *element_data = (uint8_t *)malloc(element_length);
        if (element_data == NULL) {
            capwap_free_message(msg);
            return -1;
        }
        memcpy(element_data, buffer + offset, element_length);
        offset += element_length;

        // Add element to message
        CAPWAPMessageElement element;
        element.element_type = element_type;
        element.element_length = element_length;
        element.element_data = element_data;

        CAPWAPMessageElement *new_elements = realloc(msg->elements, (msg->num_elements + 1) * sizeof(CAPWAPMessageElement));
        if (new_elements == NULL) {
            free(element_data);
            capwap_free_message(msg);
            return -1;
        }
        msg->elements = new_elements;
        msg->elements[msg->num_elements] = element;
        msg->num_elements += 1;
    }

    return 0;
}

/**
 * @brief Frees all memory associated with a CAPWAP message.
 * 
 * @param msg Pointer to CAPWAPMessage structure to free.
 */
void capwap_free_message(CAPWAPMessage *msg) {
    if (msg == NULL) return;

    if (msg->elements != NULL) {
        for (uint16_t i = 0; i < msg->num_elements; i++) {
            if (msg->elements[i].element_data != NULL) {
                free(msg->elements[i].element_data);
                msg->elements[i].element_data = NULL;
            }
        }
        free(msg->elements);
        msg->elements = NULL;
    }

    msg->num_elements = 0;
}

/* Utility */

/**
 * @brief Prints the contents of a CAPWAP message in a human-readable format.
 * 
 * @param msg Pointer to CAPWAPMessage structure to print.
 */
void print_capwap_message(const CAPWAPMessage *msg) {
    if (msg == NULL) return;

    printf("CAPWAP Message:\n");
    printf("Version: %u\n", msg->header.version);
    printf("Type: %u (%s)\n", msg->header.type, msg->header.type == 0 ? "Control Message" : "Data Message");
    printf("Flags: 0x%02X\n", msg->header.flags);
    printf("Message Length: %u bytes\n", msg->header.message_length);
    printf("Message Type: %u\n", msg->header.msg_type);
    printf("Number of Message Elements: %u\n", msg->num_elements);

    for (uint16_t i = 0; i < msg->num_elements; i++) {
        printf("  Element %u:\n", i + 1);
        printf("    Element Type: %u\n", msg->elements[i].element_type);
        printf("    Element Length: %u bytes\n", msg->elements[i].element_length);

        if (msg->elements[i].element_type == CAPWAP_ELEMENT_WTP_BOARD_DATA) {
            // Parse WTP Board Data
            if (msg->elements[i].element_length != (sizeof(((WTPBoardData *)0)->board_type) + sizeof(((WTPBoardData *)0)->board_version))) {
                printf("    Invalid WTP Board Data length\n");
                continue;
            }

            WTPBoardData wtp_board_data;
            size_t offset = 0;

            memcpy(wtp_board_data.board_type, msg->elements[i].element_data + offset, sizeof(wtp_board_data.board_type));
            wtp_board_data.board_type[sizeof(wtp_board_data.board_type) - 1] = '\0';
            offset += sizeof(wtp_board_data.board_type);

            memcpy(wtp_board_data.board_version, msg->elements[i].element_data + offset, sizeof(wtp_board_data.board_version));
            wtp_board_data.board_version[sizeof(wtp_board_data.board_version) - 1] = '\0';
            offset += sizeof(wtp_board_data.board_version);

            printf("    WTP Board Data:\n");
            printf("      Board Type: %s\n", wtp_board_data.board_type);
            printf("      Board Version: %s\n", wtp_board_data.board_version);
        }
        else if (msg->elements[i].element_type == CAPWAP_ELEMENT_WTP_DESCRIPTOR) {
            // Parse WTP Descriptor
            if (msg->elements[i].element_length != (sizeof(((WTPDescriptor *)0)->wtp_name) + sizeof(((WTPDescriptor *)0)->wtp_model))) {
                printf("    Invalid WTP Descriptor length\n");
                continue;
            }

            WTPDescriptor wtp_descriptor;
            size_t offset = 0;

            memcpy(wtp_descriptor.wtp_name, msg->elements[i].element_data + offset, sizeof(wtp_descriptor.wtp_name));
            wtp_descriptor.wtp_name[sizeof(wtp_descriptor.wtp_name) - 1] = '\0';
            offset += sizeof(wtp_descriptor.wtp_name);

            memcpy(wtp_descriptor.wtp_model, msg->elements[i].element_data + offset, sizeof(wtp_descriptor.wtp_model));
            wtp_descriptor.wtp_model[sizeof(wtp_descriptor.wtp_model) - 1] = '\0';
            offset += sizeof(wtp_descriptor.wtp_model);

            printf("    WTP Descriptor:\n");
            printf("      WTP Name: %s\n", wtp_descriptor.wtp_name);
            printf("      WTP Model: %s\n", wtp_descriptor.wtp_model);
        }
        else if (msg->elements[i].element_type == CAPWAP_ELEMENT_AC_DESCRIPTOR) {
            // Parse AC Descriptor
            if (msg->elements[i].element_length < 19 + 15 * 2) { // Fixed fields + 2 sub-elements
                printf("    Invalid AC Descriptor length\n");
                continue;
            }

            ACDescriptor ac_desc;
            size_t offset = 0;

            ac_desc.stations = ntohl(*(uint32_t *)(msg->elements[i].element_data + offset));
            offset += sizeof(uint32_t);
            ac_desc.limit = ntohl(*(uint32_t *)(msg->elements[i].element_data + offset));
            offset += sizeof(uint32_t);
            ac_desc.active_wtps = ntohl(*(uint32_t *)(msg->elements[i].element_data + offset));
            offset += sizeof(uint32_t);
            ac_desc.max_wtps = ntohl(*(uint32_t *)(msg->elements[i].element_data + offset));
            offset += sizeof(uint32_t);
            ac_desc.security = *(uint8_t *)(msg->elements[i].element_data + offset);
            offset += sizeof(uint8_t);
            ac_desc.r_mac_field = *(uint8_t *)(msg->elements[i].element_data + offset);
            offset += sizeof(uint8_t);
            ac_desc.reserved1 = *(uint8_t *)(msg->elements[i].element_data + offset);
            offset += sizeof(uint8_t);

            printf("    AC Descriptor:\n");
            printf("      Stations: %u\n", ac_desc.stations);
            printf("      Limit: %u\n", ac_desc.limit);
            printf("      Active WTPs: %u\n", ac_desc.active_wtps);
            printf("      Max WTPs: %u\n", ac_desc.max_wtps);
            printf("      Security: 0x%02X\n", ac_desc.security);
            printf("      R-MAC Field: %u\n", ac_desc.r_mac_field);
            printf("      Reserved1: %u\n", ac_desc.reserved1);

            // AC Information Sub-elements
            for (int j = 0; j < 2; j++) { // Assuming 2 sub-elements
                if (offset + 4 + 2 + 2 > msg->elements[i].element_length) {
                    printf("      Incomplete AC Information Sub-element\n");
                    break;
                }

                uint32_t vendor_id = ntohl(*(uint32_t *)(msg->elements[i].element_data + offset));
                offset += sizeof(uint32_t);
                uint16_t info_type = ntohs(*(uint16_t *)(msg->elements[i].element_data + offset));
                offset += sizeof(uint16_t);
                uint16_t info_length = ntohs(*(uint16_t *)(msg->elements[i].element_data + offset));
                offset += sizeof(uint16_t);

                if (offset + info_length > msg->elements[i].element_length) {
                    printf("      Incomplete AC Information Sub-element Data\n");
                    break;
                }

                char info_data[info_length + 1];
                memcpy(info_data, msg->elements[i].element_data + offset, info_length);
                info_data[info_length] = '\0'; // Null-terminate
                offset += info_length;

                printf("      AC Information Sub-element:\n");
                printf("        Vendor Identifier: 0x%08X\n", vendor_id);
                printf("        AC Information Type: %u\n", info_type);
                printf("        AC Information Length: %u\n", info_length);
                printf("        AC Information Data: %s\n", info_data);
            }
        }
        else if (msg->elements[i].element_type == CAPWAP_ELEMENT_AC_NAME_PRIORITY) {
            // Parse AC Name with Priority
            if (msg->elements[i].element_length < 2) { // Priority (1) + at least 1 byte for AC Name
                printf("    Invalid AC Name with Priority length\n");
                continue;
            }

            uint8_t priority = *(uint8_t *)(msg->elements[i].element_data);
            size_t ac_name_length = msg->elements[i].element_length - sizeof(uint8_t);
            char *ac_name = (char *)malloc(ac_name_length + 1);
            if (ac_name == NULL) {
                printf("    Memory allocation failed for AC Name\n");
                continue;
            }
            memcpy(ac_name, msg->elements[i].element_data + sizeof(uint8_t), ac_name_length);
            ac_name[ac_name_length] = '\0'; // Null-terminate

            printf("    AC Name with Priority:\n");
            printf("      Priority: %u\n", priority);
            printf("      AC Name: %s\n", ac_name);

            free(ac_name);
        }
        else {
            // For other element types, just print raw data
            printf("    Unrecognized Element:\n");
            printf("      Element Type: %u\n", msg->elements[i].element_type);
            printf("      Element Length: %u bytes\n", msg->elements[i].element_length);
            printf("      Element Data: ");
            for (uint16_t k = 0; k < msg->elements[i].element_length; k++) {
                printf("%02X ", msg->elements[i].element_data[k]);
            }
            printf("\n");
        }
    }
}
