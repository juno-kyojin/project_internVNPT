// capwap_functions.h

#ifndef CAPWAP_FUNCTIONS_H
#define CAPWAP_FUNCTIONS_H

#include "capwap_structs.h"
#include <stddef.h>

// **Session Management**

/**
 * @brief Initializes a CAPWAP session by generating a random session ID.
 * 
 * @param session_id Pointer to CAPWAPSessionID structure to store the generated session ID.
 * @return int 0 on success, -1 on failure.
 */
int capwap_init_session(CAPWAPSessionID *session_id);

// **Message Handling**

/**
 * @brief Creates a Discovery Request message.
 * 
 * @param msg Pointer to CAPWAPMessage structure to initialize.
 * @return int 0 on success, -1 on failure.
 */
int capwap_create_discovery_request(CAPWAPMessage *msg);

/**
 * @brief Creates a Discovery Response message with AC Descriptor and AC Name with Priority.
 * 
 * @param msg Pointer to CAPWAPMessage structure to populate.
 * @param ac_desc Pointer to ACDescriptor structure containing AC information.
 * @param ac_name_priority Pointer to ACNameWithPriority structure containing AC name and priority.
 * @return int 0 on success, -1 on failure.
 */
int capwap_create_discovery_response(CAPWAPMessage *msg, const ACDescriptor *ac_desc, const ACNameWithPriority *ac_name_priority);

/**
 * @brief Updates the message_length of a CAPWAP message based on its elements.
 * 
 * @param msg Pointer to CAPWAPMessage structure to update.
 * @return int 0 on success, -1 on failure.
 */
int capwap_update_message_length(CAPWAPMessage *msg);

/**
 * @brief Serializes a CAPWAP message into a buffer.
 * 
 * @param msg Pointer to CAPWAPMessage structure.
 * @param buffer Buffer to serialize the message into.
 * @param buffer_size Size of the buffer.
 * @return int Number of bytes written to the buffer, or -1 on failure.
 */
int capwap_serialize_message(const CAPWAPMessage *msg, uint8_t *buffer, size_t buffer_size);

/**
 * @brief Deserializes a buffer into a CAPWAP message structure.
 * 
 * @param msg Pointer to CAPWAPMessage structure to populate.
 * @param buffer Buffer containing the serialized message.
 * @param buffer_size Size of the buffer.
 * @return int 0 on success, -1 on failure.
 */
int capwap_deserialize_message(CAPWAPMessage *msg, const uint8_t *buffer, size_t buffer_size);

/**
 * @brief Frees all memory associated with a CAPWAP message.
 * 
 * @param msg Pointer to CAPWAPMessage structure to free.
 */
void capwap_free_message(CAPWAPMessage *msg);

/**
 * @brief Prints the contents of a CAPWAP message in a human-readable format.
 * 
 * @param msg Pointer to CAPWAPMessage structure to print.
 */
void print_capwap_message(const CAPWAPMessage *msg);

#endif // CAPWAP_FUNCTIONS_H
