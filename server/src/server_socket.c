#include <stdio.h>
#include <string.h>
#include "server_socket.h"
#include "msg_handle.h"
#include "database.h"

char current_text_filename[100];  // Variable to store the current file name

// Function to set up server socket
int setup_server_socket(int port) {
    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }

    return 0;
}

// Function to receive messages from the Agent
void receive_messages() {
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(client_addr);

    while (1) {
        // Receive data from Agent
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        // Null-terminate the received message
        buffer[n] = '\0';

        // Check if this is the text filename (assuming it's the first message from the Agent)
        if (strstr(buffer, ".txt")) {
            // Save the new file name to current_text_filename
            strncpy(current_text_filename, buffer, sizeof(current_text_filename) - 1);
            current_text_filename[sizeof(current_text_filename) - 1] = '\0';

            // Validate that the filename is not empty
            if (strlen(current_text_filename) == 0) {
                fprintf(stderr, "Received an empty filename. Skipping file creation.\n");
                continue;
            }

            // Create a new text file
            printf("Received filename: %s\n", current_text_filename);  // Print the received filename
            create_text_file(current_text_filename);
            printf("New text file created: %s\n", current_text_filename);
        } else {
            // Validate that the filename is not empty before writing
            if (strlen(current_text_filename) == 0) {
                fprintf(stderr, "No valid filename set. Unable to write data.\n");
                continue;
            }

            // Save the received data to the current text file
            printf("Received data: %s\n", buffer);
            write_to_text_file(current_text_filename, buffer);
        }
    }
}
