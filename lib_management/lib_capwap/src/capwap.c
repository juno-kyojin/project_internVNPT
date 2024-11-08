#include "/home/tuitachi/new_project/lib_management/lib_capwap/include/capwap.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Initialize CAPWAP session ID
int capwap_init_session(CAPWAPSessionID *session) {
    if (!session) return -1;
    srand(time(NULL));
    for (int i = 0; i < 16; i++) {
        session->session_id[i] = rand() % 256;
    }
    return 0;
}

int capwap_create_message(CAPWAPMessage *msg, CAPWAPMessageType type, const CAPWAPMessageElement *elements, uint16_t num_elements) {
    if (!msg || (num_elements > 0 && !elements)) return -1;

    // Initialize message header
    msg->header.version_type = 0;         // Set version
    msg->header.msg_type = type;          // Set message type inside header
    msg->header.message_length = 0;       // Initialize message length

    // Calculate total payload length
    uint16_t total_length = 0;
    for (uint16_t i = 0; i < num_elements; i++) {
        total_length += sizeof(elements[i].element_type) + sizeof(elements[i].element_length) + elements[i].element_length;
    }

    // Set payload length
    msg->payload_len = total_length;

    // Allocate memory for payload if there's any element
    if (total_length > 0) {
        msg->payload = (uint8_t *)malloc(total_length);
        if (!msg->payload) return -1;  // Memory allocation failed
    } else {
        msg->payload = NULL;  // No elements, no payload
    }

    // Fill payload with message elements if any
    uint8_t *payload_ptr = msg->payload;
    for (uint16_t i = 0; i < num_elements && payload_ptr; i++) {
        memcpy(payload_ptr, &elements[i].element_type, sizeof(elements[i].element_type));
        payload_ptr += sizeof(elements[i].element_type);

        memcpy(payload_ptr, &elements[i].element_length, sizeof(elements[i].element_length));
        payload_ptr += sizeof(elements[i].element_length);

        memcpy(payload_ptr, elements[i].element_data, elements[i].element_length);
        payload_ptr += elements[i].element_length;
    }

    // Set total message length in header
    msg->header.message_length = sizeof(CAPWAPHeader) + msg->payload_len;

    return 0;
}


// Free CAPWAP message
void capwap_free_message(CAPWAPMessage *msg) {
    if (msg && msg->payload) {
        free(msg->payload);
        msg->payload = NULL; // Đảm bảo không gọi free() hai lần
    }
}


int capwap_handle_discovery(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server) {
    if (is_server) {
        // Server xử lý DISCOVERY_REQUEST và phản hồi DISCOVERY_RESPONSE
        printf("Server: Handling Discovery Request\n");
        CAPWAPMessage response_msg;
        capwap_create_message(&response_msg, DISCOVERY_RESPONSE, NULL, 0);
        capwap_udp_send(sock, &response_msg, src_addr);
        printf("Server: Sent Discovery Response\n");
        capwap_free_message(&response_msg);
    } else {
        // Agent gửi DISCOVERY_REQUEST tới Server
        printf("Agent: Sending Discovery Request\n");
        CAPWAPMessage request_msg;
        capwap_create_message(&request_msg, DISCOVERY_REQUEST, NULL, 0);
        capwap_udp_send(sock, &request_msg, src_addr);
        printf("Agent: Sent Discovery Request\n");
        capwap_free_message(&request_msg);
    }
    return 0;
}

int capwap_handle_join(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server) {
    if (is_server) {
        // Server xử lý JOIN_REQUEST và phản hồi JOIN_RESPONSE
        printf("Server: Handling Join Request\n");
        CAPWAPMessage response_msg;
        capwap_create_message(&response_msg, JOIN_RESPONSE, NULL, 0);
        capwap_udp_send(sock, &response_msg, src_addr);
        printf("Server: Sent Join Response\n");
        capwap_free_message(&response_msg);
    } else {
        // Agent gửi JOIN_REQUEST tới Server
        printf("Agent: Sending Join Request\n");
        CAPWAPMessage request_msg;
        capwap_create_message(&request_msg, JOIN_REQUEST, NULL, 0);
        capwap_udp_send(sock, &request_msg, src_addr);
        printf("Agent: Sent Join Request\n");
        capwap_free_message(&request_msg);
    }
    return 0;
}

int capwap_handle_configuration(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server) {
    if (is_server) {
        // Server xử lý CONFIGURATION_STATUS_REQUEST và phản hồi CONFIGURATION_STATUS_RESPONSE
        printf("Server: Handling Configuration Status Request\n");
        CAPWAPMessageElement config_elements[2];
        
        // Ví dụ cấu hình phần tử: SSID
        config_elements[0].element_type = ELEMENT_SSID;
        config_elements[0].element_length = 6;
        config_elements[0].element_data = (uint8_t *)"MySSID";

        // Phần tử cấu hình: Channel
        config_elements[1].element_type = ELEMENT_CHANNEL;
        config_elements[1].element_length = 1;
        config_elements[1].element_data = (uint8_t *)"\x06"; // Channel 6

        CAPWAPMessage response_msg;
        capwap_create_message(&response_msg, CONFIGURATION_STATUS_RESPONSE, config_elements, 2);
        capwap_udp_send(sock, &response_msg, src_addr);
        printf("Server: Sent Configuration Status Response\n");
        capwap_free_message(&response_msg);
    } else {
        // Agent gửi CONFIGURATION_STATUS_REQUEST tới Server
        printf("Agent: Sending Configuration Status Request\n");
        CAPWAPMessage request_msg;
        capwap_create_message(&request_msg, CONFIGURATION_STATUS_REQUEST, NULL, 0);
        capwap_udp_send(sock, &request_msg, src_addr);
        printf("Agent: Sent Configuration Status Request\n");
        capwap_free_message(&request_msg);
    }
    return 0;
}


int capwap_handle_keep_alive(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server) {
    if (is_server) {
        // Server nhận KEEP_ALIVE từ Agent
        printf("Server: Handling Keep-Alive/Heartbeat\n");
        CAPWAPMessage response_msg;
        capwap_create_message(&response_msg, KEEP_ALIVE, NULL, 0);
        capwap_udp_send(sock, &response_msg, src_addr);
        printf("Server: Sent Keep-Alive Response\n");
        capwap_free_message(&response_msg);
    } else {
        // Agent gửi KEEP_ALIVE tới Server
        printf("Agent: Sending Keep-Alive/Heartbeat\n");
        CAPWAPMessage request_msg;
        capwap_create_message(&request_msg, KEEP_ALIVE, NULL, 0);
        capwap_udp_send(sock, &request_msg, src_addr);
        printf("Agent: Sent Keep-Alive\n");
        capwap_free_message(&request_msg);
    }
    return 0;
}


int capwap_handle_data_path(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr, int is_server) {
    if (is_server) {
        // Server xử lý DATA_PATH_REQUEST và phản hồi DATA_PATH_RESPONSE
        printf("Server: Handling Data Path Request\n");
        CAPWAPMessage response_msg;
        capwap_create_message(&response_msg, DATA_PATH_RESPONSE, NULL, 0);
        capwap_udp_send(sock, &response_msg, src_addr);
        printf("Server: Sent Data Path Response\n");
        capwap_free_message(&response_msg);
    } else {
        // Agent gửi DATA_PATH_REQUEST tới Server
        printf("Agent: Sending Data Path Request\n");
        CAPWAPMessage request_msg;
        capwap_create_message(&request_msg, DATA_PATH_REQUEST, NULL, 0);
        capwap_udp_send(sock, &request_msg, src_addr);
        printf("Agent: Sent Data Path Request\n");
        capwap_free_message(&request_msg);
    }
    return 0;
}


int capwap_handle_config_update(CAPWAPMessage *msg, int sock, struct sockaddr_in *src_addr) {
    printf("Handling Configuration Update\n");

    // Biến con trỏ tới payload để duyệt qua các Message Elements
    uint8_t *payload_ptr = msg->payload;
    uint16_t remaining_len = msg->payload_len;

    while (remaining_len > sizeof(CAPWAPMessageElementType) + sizeof(uint16_t)) {
        // Đọc loại và chiều dài của từng Message Element
        uint16_t element_type = *(uint16_t *)payload_ptr;
        payload_ptr += sizeof(uint16_t);

        uint16_t element_length = *(uint16_t *)payload_ptr;
        payload_ptr += sizeof(uint16_t);

        // Kiểm tra còn đủ dữ liệu để đọc không
        if (element_length > remaining_len) {
            printf("Error: Payload length mismatch\n");
            return -1;
        }

        // Xử lý từng Message Element dựa trên loại
        switch (element_type) {
            case ELEMENT_SSID:
                // Cập nhật SSID
                printf("Updating SSID to: %.*s\n", element_length, (char *)payload_ptr);
                // Thêm mã cập nhật SSID vào đây
                break;

            case ELEMENT_CHANNEL:
                // Cập nhật kênh
                printf("Updating Channel to: %d\n", *payload_ptr);
                // Thêm mã cập nhật kênh vào đây
                break;

            case ELEMENT_TX_POWER:
                // Cập nhật công suất phát
                printf("Updating Tx Power to: %d dBm\n", *payload_ptr);
                // Thêm mã cập nhật công suất phát vào đây
                break;

            case ELEMENT_ENCRYPTION:
                // Cập nhật mã hóa
                printf("Updating Encryption to: %.*s\n", element_length, (char *)payload_ptr);
                // Thêm mã cập nhật mã hóa vào đây
                break;

            default:
                printf("Unknown Configuration Element Type: %d\n", element_type);
                break;
        }

        // Chuyển sang Message Element tiếp theo
        payload_ptr += element_length;
        remaining_len -= (sizeof(uint16_t) * 2 + element_length);
    }

    return 0;
}

// Initialize UDP transport for CAPWAP
int capwap_udp_init(int *sock, int port) {
    struct sockaddr_in addr;
    *sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sock < 0) {
        perror("Failed to create socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(*sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Failed to bind socket");
        close(*sock);
        return -1;
    }
    return 0;
}


int capwap_udp_send(int sock, const CAPWAPMessage *msg, struct sockaddr_in *dest_addr) {
    size_t total_len = sizeof(CAPWAPHeader) + msg->payload_len;
    uint8_t *buffer = malloc(total_len);

    if (!buffer) return -1;

    // Copy CAPWAP header và payload vào buffer
    memcpy(buffer, &msg->header, sizeof(CAPWAPHeader));
    if (msg->payload_len > 0) {
        memcpy(buffer + sizeof(CAPWAPHeader), msg->payload, msg->payload_len);
    }

    dest_addr->sin_family = AF_INET; // Đảm bảo `sin_family` là `AF_INET`

    int result = sendto(sock, buffer, total_len, 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr));
    free(buffer);

    if (result < 0) {
        perror("sendto failed");  // Chi tiết lỗi khi gửi thất bại
    } else {
        printf("Sent %zu bytes to %s:%d\n", total_len, inet_ntoa(dest_addr->sin_addr), ntohs(dest_addr->sin_port));
    }
    return result;
}


int capwap_udp_receive(int sock, CAPWAPMessage *msg, struct sockaddr_in *src_addr) {
    uint8_t buffer[1024]; // Bộ đệm đủ lớn để chứa gói tin
    socklen_t addr_len = sizeof(*src_addr);

    // Nhận dữ liệu
    ssize_t len = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)src_addr, &addr_len);
    if (len < sizeof(CAPWAPHeader)) {
        printf("Received packet too small\n");
        return -1;
    }

    // Copy header từ buffer vào msg->header
    memcpy(&msg->header, buffer, sizeof(CAPWAPHeader));

    // Sử dụng msg_type từ header
    // msg->msg_type = msg->header.msg_type;
    printf("Debug: Received msg_type: %d\n", msg->header.msg_type);

    // Copy payload nếu có
    msg->payload_len = len - sizeof(CAPWAPHeader);
    if (msg->payload_len > 0) {
        msg->payload = malloc(msg->payload_len);
        if (!msg->payload) {
            printf("Memory allocation failed\n");
            return -1;
        }
        memcpy(msg->payload, buffer + sizeof(CAPWAPHeader), msg->payload_len);
    } else {
        msg->payload = NULL;
    }

    return msg->payload_len;
}


// Close UDP connection
void capwap_udp_close(int sock) {
    close(sock);
}
