// capwap_elements.h

#ifndef CAPWAP_ELEMENTS_H
#define CAPWAP_ELEMENTS_H

#include "capwap_structs.h"

// **Element Creation**

/**
 * @brief Adds WTP Board Data element to the CAPWAP message.
 * 
 * @param msg Pointer to the CAPWAP message.
 * @param wtp_board_data Pointer to the WTP Board Data.
 * @return int Status code.
 */
int capwap_add_wtp_board_data_element(CAPWAPMessage *msg, const WTPBoardData *wtp_board_data);

/**
 * @brief Adds WTP Descriptor element to the CAPWAP message.
 * 
 * @param msg Pointer to the CAPWAP message.
 * @param wtp_descriptor Pointer to the WTP Descriptor.
 * @return int Status code.
 */
int capwap_add_wtp_descriptor_element(CAPWAPMessage *msg, const WTPDescriptor *wtp_descriptor);

/**
 * @brief Adds AC Descriptor element to the CAPWAP message.
 * 
 * @param msg Pointer to the CAPWAP message.
 * @param ac_desc Pointer to the AC Descriptor.
 * @return int Status code.
 */
int capwap_add_ac_descriptor_element(CAPWAPMessage *msg, const ACDescriptor *ac_desc);

/**
 * @brief Adds AC Name with Priority element to the CAPWAP message.
 * 
 * @param msg Pointer to the CAPWAP message.
 * @param ac_name_priority Pointer to the AC Name with Priority.
 * @return int Status code.
 */
int capwap_add_ac_name_priority_element(CAPWAPMessage *msg, const ACNameWithPriority *ac_name_priority);

#endif // CAPWAP_ELEMENTS_H
