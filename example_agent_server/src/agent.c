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

// **Define Config of Agent**
typedef struct {
    WTPBoardData wtp_board_data;
    WTPDescriptor wtp_descriptor;
} AgentConfig;

/**
 * @brief Initialize the configuration of the Agent.
 * 
 * @param config Pointer to the AgentConfig structure to initialize.
 */
void initialize_agent_config(AgentConfig *config) {
    if (config == NULL) return;

    // Configure WTP Board Data
    strncpy(config->wtp_board_data.board_type, "TypeA", sizeof(config->wtp_board_data.board_type) - 1);
    config->wtp_board_data.board_type[sizeof(config->wtp_board_data.board_type) - 1] = '\0';
    strncpy(config->wtp_board_data.board_version, "v1.2.3", sizeof(config->wtp_board_data.board_version) - 1);
    config->wtp_board_data.board_version[sizeof(config->wtp_board_data.board_version) - 1] = '\0';

    // Configure WTP Descriptor
    strncpy(config->wtp_descriptor.wtp_name, "WTP-001", sizeof(config->wtp_descriptor.wtp_name) - 1);
    config->wtp_descriptor.wtp_name[sizeof(config->wtp_descriptor.wtp_name) - 1] = '\0';
    strncpy(config->wtp_descriptor.wtp_model, "ModelX", sizeof(config->wtp_descriptor.wtp_model) - 1);
    config->wtp_descriptor.wtp_model[sizeof(config->wtp_descriptor.wtp_model) - 1] = '\0';
}

/**
 * @brief Callback to handle Discovery Response from Server.
 * 
 * @param sock UDP socket.
 * @param eloop_ctx Related eloop data (can be NULL).
 * @param user_ctx User data (AgentConfig).
 */
void on_agent_receive(int sock, void *eloop_ctx, void *user_ctx) {
    (void)eloop_ctx; // Prevent unused parameter warning
    AgentConfig *config = (AgentConfig *)user_ctx;
    uint8_t recv_buffer[2048];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    ssize_t recv_len = recvfrom(sock, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&from_addr, &from_len);
    if (recv_len < 0) {
        perror("Agent: Unable to receive data");
        return;
    }

    printf("Agent: Received %zd bytes from Server\n", recv_len);

    // Print received data in hexadecimal format
    printf("Agent: Data received: ");
    for (ssize_t i = 0; i < recv_len; i++) {
        printf("%02X ", recv_buffer[i]);
    }
    printf("\n");

    // Deserialize Discovery Response
    CAPWAPMessage discovery_response;
    if (capwap_deserialize_message(&discovery_response, recv_buffer, recv_len) != 0) {
        fprintf(stderr, "Agent: Unable to deserialize Discovery Response\n");
        return;
    }

    // Check message type
    if (discovery_response.header.msg_type != DISCOVERY_RESPONSE) {
        fprintf(stderr, "Agent: Received message is not a Discovery Response\n");
        capwap_free_message(&discovery_response);
        return;
    }

    printf("Agent: Successfully received Discovery Response\n");

    // Print the content of the message
    print_capwap_message(&discovery_response);

    // Free memory
    capwap_free_message(&discovery_response);

    // After processing, terminate the loop if not needed
    eloop_terminate();
}

int main() {
    int sock;
    struct sockaddr_in server_addr, local_addr;
    AgentConfig config;

    // Initialize Agent configuration
    initialize_agent_config(&config);

    printf("Agent: Initializing UDP socket...\n");
    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Agent: Unable to create socket");
        return EXIT_FAILURE;
    }

    // Configure Agent address (can be automatic)
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(0); // Let the system choose the port

    printf("Agent: Bind socket...\n");
    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("Agent: Unable to bind socket");
        close(sock);
        return EXIT_FAILURE;
    }

    // Configure Server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5246); // Default port for CAPWAP Control Messages
    if (inet_aton("127.0.0.1", &server_addr.sin_addr) == 0) { // Use localhost for example
        fprintf(stderr, "Agent: Invalid Server address\n");
        close(sock);
        return EXIT_FAILURE;
    }

    // Initialize CAPWAP session
    CAPWAPSessionID session_id;
    if (capwap_init_session(&session_id) != 0) {
        fprintf(stderr, "Agent: Unable to initialize CAPWAP session\n");
        close(sock);
        return EXIT_FAILURE;
    }
    printf("Agent: Initializing CAPWAP session...\n");
    printf("Agent: CAPWAP session initialized\n");

    // Create and send Discovery Request
    CAPWAPMessage discovery_request;
    if (capwap_create_discovery_request(&discovery_request) != 0) {
        fprintf(stderr, "Agent: Unable to create Discovery Request\n");
        close(sock);
        return EXIT_FAILURE;
    }

    // Add WTP Board Data and WTP Descriptor to Discovery Request
    if (capwap_add_wtp_board_data_element(&discovery_request, &config.wtp_board_data) != 0) {
        fprintf(stderr, "Agent: Unable to add WTP Board Data to Discovery Request\n");
        capwap_free_message(&discovery_request);
        close(sock);
        return EXIT_FAILURE;
    }

    if (capwap_add_wtp_descriptor_element(&discovery_request, &config.wtp_descriptor) != 0) {
        fprintf(stderr, "Agent: Unable to add WTP Descriptor to Discovery Request\n");
        capwap_free_message(&discovery_request);
        close(sock);
        return EXIT_FAILURE;
    }

    // Update message_length after adding message elements
    if (capwap_update_message_length(&discovery_request) != 0) {
        fprintf(stderr, "Agent: Unable to update message_length for Discovery Request\n");
        capwap_free_message(&discovery_request);
        close(sock);
        return EXIT_FAILURE;
    }

    // Serialize Discovery Request
    uint8_t send_buffer[2048];
    int send_length = capwap_serialize_message(&discovery_request, send_buffer, sizeof(send_buffer));
    if (send_length < 0) {
        fprintf(stderr, "Agent: Error serializing Discovery Request\n");
        capwap_free_message(&discovery_request);
        close(sock);
        return EXIT_FAILURE;
    }

    // Send Discovery Request to Server
    printf("Agent: Sending Discovery Request to Server...\n");
    ssize_t sent_bytes = sendto(sock, send_buffer, send_length, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (sent_bytes < 0) {
        perror("Agent: Unable to send Discovery Request");
        capwap_free_message(&discovery_request);
        close(sock);
        return EXIT_FAILURE;
    }
    printf("capwap_send_discovery_request: Discovery Request sent successfully\n");

    capwap_free_message(&discovery_request);

    // **Start integrating eloop**

    // Initialize eloop
    if (eloop_init() != 0) {
        fprintf(stderr, "Agent: Unable to initialize eloop\n");
        close(sock);
        return EXIT_FAILURE;
    }

    // Register read event for UDP socket with eloop
    int ret = eloop_register_read_sock(sock, on_agent_receive, NULL, &config);
    if (ret != 0) {
        fprintf(stderr, "Agent: Unable to register read event with eloop\n");
        eloop_destroy();
        close(sock);
        return EXIT_FAILURE;
    }

    // Run event loop
    printf("Agent: Waiting for Discovery Response from Server...\n");
    eloop_run(); // Do not compare with 0 because eloop_run() returns void

    // **End integrating eloop**

    // Free resources after the loop ends
    eloop_unregister_read_sock(sock);
    eloop_destroy();
    close(sock);

    printf("Agent: Program terminated\n");

    return EXIT_SUCCESS;
}
