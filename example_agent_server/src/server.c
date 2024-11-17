#include "/home/tuitachi/project_provip/libcapwap/inc/capwap_structs.h"
#include "/home/tuitachi/project_provip/libcapwap/inc/capwap_functions.h"
#include "/home/tuitachi/project_provip/libcapwap/inc/capwap_elements.h"
#include "/home/tuitachi/project_provip/libcapwap/inc/capwap_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "eloop.h"
// **Define Config of Server**
typedef struct {
    ACDescriptor ac_desc;
    ACNameWithPriority ac_name_priority;
} ServerConfig;

/**
 * @brief Initialize the configuration of the Server.
 * 
 * @param config Pointer to the ServerConfig structure to initialize.
 */
void initialize_server_config(ServerConfig *config) {
    if (config == NULL) return;

    // Configure AC Descriptor
    config->ac_desc.stations = 150;       // Current number of stations
    config->ac_desc.limit = 300;          // Station limit
    config->ac_desc.active_wtps = 15;     // Number of active WTPs
    config->ac_desc.max_wtps = 25;        // WTP limit
    config->ac_desc.security = 0x06;      // Security setting
    config->ac_desc.r_mac_field = 1;      // R-MAC Field
    config->ac_desc.reserved1 = 0;        // Reserved

    // Configure AC Name with Priority
    config->ac_name_priority.priority = 1; // Priority 1 - Primary AC

    // Check if ac_name is a pointer, allocate memory
    // Check in capwap.h
    // Assume ac_name is a pointer
    config->ac_name_priority.ac_name = malloc(32); // Allocate 32 bytes
    if (config->ac_name_priority.ac_name == NULL) {
        fprintf(stderr, "Server: Unable to allocate memory for ac_name\n");
        exit(EXIT_FAILURE);
    }

    strncpy(config->ac_name_priority.ac_name, "PrimaryAC", 31);
    config->ac_name_priority.ac_name[31] = '\0';
}

/**
 * @brief Callback to handle Discovery Request from Agent.
 * 
 * @param sock UDP socket.
 * @param eloop_ctx Related eloop data (can be NULL).
 * @param user_ctx User data (ServerConfig).
 */
void on_server_receive(int sock, void *eloop_ctx, void *user_ctx) {
    (void)eloop_ctx; // Prevent unused parameter warning
    ServerConfig *config = (ServerConfig *)user_ctx;
    uint8_t recv_buffer[2048];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    ssize_t recv_len = recvfrom(sock, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&from_addr, &from_len);
    if (recv_len < 0) {
        perror("Server: Unable to receive data");
        return;
    }

    printf("Server: Received %zd bytes from Agent\n", recv_len);

    // Print received data in hexadecimal format
    printf("Server: Data received: ");
    for (ssize_t i = 0; i < recv_len; i++) {
        printf("%02X ", recv_buffer[i]);
    }
    printf("\n");

    // Deserialize Discovery Request
    CAPWAPMessage discovery_request;
    memset(&discovery_request, 0, sizeof(CAPWAPMessage)); // Initialize message

    if (capwap_deserialize_message(&discovery_request, recv_buffer, recv_len) != 0) {
        fprintf(stderr, "Server: Unable to deserialize Discovery Request\n");
        return;
    }

    printf("Server: Successfully deserialized Discovery Request\n");

    // Check message type
    printf("Server: Checking message type: %u\n", discovery_request.header.msg_type);
    if (discovery_request.header.msg_type != DISCOVERY_REQUEST) {
        fprintf(stderr, "Server: Received message is not a Discovery Request\n");
        capwap_free_message(&discovery_request);
        return;
    }

    printf("Server: Processing Discovery Request\n");

    // Extract WTP Board Data and WTP Descriptor from Discovery Request
    WTPBoardData received_board_data;
    WTPDescriptor received_descriptor;

    for (uint16_t i = 0; i < discovery_request.num_elements; i++) {
        if (discovery_request.elements[i].element_type == CAPWAP_ELEMENT_WTP_BOARD_DATA) {
            if (discovery_request.elements[i].element_length != (sizeof(received_board_data.board_type) + sizeof(received_board_data.board_version))) {
                printf("Server: WTP Board Data has invalid length\n");
                continue;
            }

            memcpy(received_board_data.board_type, discovery_request.elements[i].element_data, sizeof(received_board_data.board_type));
            received_board_data.board_type[sizeof(received_board_data.board_type) - 1] = '\0';

            memcpy(received_board_data.board_version, discovery_request.elements[i].element_data + sizeof(received_board_data.board_type), sizeof(received_board_data.board_version));
            received_board_data.board_version[sizeof(received_board_data.board_version) - 1] = '\0';

            printf("Server: WTP Board Data:\n");
            printf("  Board Type: %s\n", received_board_data.board_type);
            printf("  Board Version: %s\n", received_board_data.board_version);
        }
        else if (discovery_request.elements[i].element_type == CAPWAP_ELEMENT_WTP_DESCRIPTOR) {
            if (discovery_request.elements[i].element_length != (sizeof(received_descriptor.wtp_name) + sizeof(received_descriptor.wtp_model))) {
                printf("Server: WTP Descriptor has invalid length\n");
                continue;
            }

            memcpy(received_descriptor.wtp_name, discovery_request.elements[i].element_data, sizeof(received_descriptor.wtp_name));
            received_descriptor.wtp_name[sizeof(received_descriptor.wtp_name) - 1] = '\0';

            memcpy(received_descriptor.wtp_model, discovery_request.elements[i].element_data + sizeof(received_descriptor.wtp_name), sizeof(received_descriptor.wtp_model));
            received_descriptor.wtp_model[sizeof(received_descriptor.wtp_model) - 1] = '\0';

            printf("Server: WTP Descriptor:\n");
            printf("  WTP Name: %s\n", received_descriptor.wtp_name);
            printf("  WTP Model: %s\n", received_descriptor.wtp_model);
        }
        // Can add handling for other message elements if needed
        else {
            // Unknown message element, just print type and data
            printf("Server: Unrecognized message element:\n");
            printf("  Element Type: %u\n", discovery_request.elements[i].element_type);
            printf("  Element Length: %u bytes\n", discovery_request.elements[i].element_length);
            printf("  Element Data: ");
            for (uint16_t k = 0; k < discovery_request.elements[i].element_length; k++) {
                printf("%02X ", discovery_request.elements[i].element_data[k]);
            }
            printf("\n");
        }
    }

    // Create Discovery Response
    CAPWAPMessage discovery_response;
    memset(&discovery_response, 0, sizeof(CAPWAPMessage)); // Initialize message

    if (capwap_create_discovery_response(&discovery_response, &config->ac_desc, &config->ac_name_priority) != 0) {
        fprintf(stderr, "Server: Unable to create Discovery Response\n");
        capwap_free_message(&discovery_request);
        return;
    }

    // Serialize Discovery Response
    uint8_t send_buffer[2048];
    int send_length = capwap_serialize_message(&discovery_response, send_buffer, sizeof(send_buffer));
    if (send_length < 0) {
        fprintf(stderr, "Server: Error serializing Discovery Response\n");
        capwap_free_message(&discovery_request);
        capwap_free_message(&discovery_response);
        return;
    }

    // Send Discovery Response to Agent
    ssize_t sent_bytes = sendto(sock, send_buffer, send_length, 0, (struct sockaddr *)&from_addr, from_len);
    if (sent_bytes < 0) {
        perror("Server: Unable to send Discovery Response");
        capwap_free_message(&discovery_request);
        capwap_free_message(&discovery_response);
        return;
    }

    printf("Server: Successfully sent Discovery Response\n");

    // Free memory
    capwap_free_message(&discovery_request);
    capwap_free_message(&discovery_response);
}

/**
 * @brief Callback to stop the event loop when receiving SIGINT (Ctrl+C).
 * 
 * @param sig Signal number.
 * @param user_ctx User data (not used in this case).
 */
void on_signal(int sig, void *user_ctx) {
    (void)sig;        // Prevent unused parameter warning
    (void)user_ctx;   // Prevent unused parameter warning
    printf("Server: Received stop signal, terminating program...\n");
    eloop_terminate(); // Stop the event loop
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    ServerConfig config;

    // Initialize Server configuration
    initialize_server_config(&config);

    printf("Server: Initializing UDP socket...\n");
    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Server: Unable to create socket");
        // Free allocated memory for ac_name if needed
        free(config.ac_name_priority.ac_name);
        return EXIT_FAILURE;
    }

    // Configure Server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    server_addr.sin_port = htons(5246);        // Default port for CAPWAP Control Messages

    printf("Server: Bind socket to port %d...\n", ntohs(server_addr.sin_port));
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Server: Unable to bind socket");
        close(sock);
        free(config.ac_name_priority.ac_name);
        return EXIT_FAILURE;
    }

    printf("Server: Listening on port %d\n", ntohs(server_addr.sin_port));

    // **Start integrating eloop**

    // Initialize eloop
    if (eloop_init() != 0) {
        fprintf(stderr, "Server: Unable to initialize eloop\n");
        close(sock);
        free(config.ac_name_priority.ac_name);
        return EXIT_FAILURE;
    }

    // Register read event for UDP socket with eloop
    int ret = eloop_register_read_sock(sock, on_server_receive, NULL, &config);
    if (ret != 0) {
        fprintf(stderr, "Server: Unable to register read event with eloop\n");
        eloop_destroy();
        close(sock);
        free(config.ac_name_priority.ac_name);
        return EXIT_FAILURE;
    }

    // Register SIGINT signal handler with eloop
    ret = eloop_register_signal(SIGINT, on_signal, NULL);
    if (ret != 0) {
        fprintf(stderr, "Server: Unable to register SIGINT signal handler with eloop\n");
        eloop_unregister_read_sock(sock);
        eloop_destroy();
        close(sock);
        free(config.ac_name_priority.ac_name);
        return EXIT_FAILURE;
    }

    // Run event loop
    printf("Server: Listening and processing Discovery Requests...\n");
    eloop_run(); 

    // **End integrating eloop**

    // Free resources after the loop ends
    eloop_unregister_read_sock(sock);
    eloop_destroy();
    close(sock);

    // Free allocated memory for ac_name
    free(config.ac_name_priority.ac_name);

    printf("Server: Program terminated\n");

    return EXIT_SUCCESS;
}

