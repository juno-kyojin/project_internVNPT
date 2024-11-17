// capwap_types.h

#ifndef CAPWAP_TYPES_H
#define CAPWAP_TYPES_H

#include <stdint.h>



/**
 * @brief Enum representing the types of CAPWAP messages.
 */
typedef enum {
    DISCOVERY_REQUEST = 1, /**< Discovery Request message */
    DISCOVERY_RESPONSE = 2, /**< Discovery Response message */
    // Các loại thông điệp khác có thể được thêm vào đây
} CAPWAPMessageType;



/**
 * @brief Macro defining the WTP Board Data element type.
 */
#define CAPWAP_ELEMENT_WTP_BOARD_DATA     1

/**
 * @brief Macro defining the WTP Descriptor element type.
 */
#define CAPWAP_ELEMENT_WTP_DESCRIPTOR     2

/**
 * @brief Macro defining the AC Descriptor element type.
 */
#define CAPWAP_ELEMENT_AC_DESCRIPTOR      3

/**
 * @brief Macro defining the AC Name with Priority element type.
 */
#define CAPWAP_ELEMENT_AC_NAME_PRIORITY   4

#endif // CAPWAP_TYPES_H
