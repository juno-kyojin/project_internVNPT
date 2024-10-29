#include <stdio.h>
#include <string.h>
#include "log.h"  
#include "msg_handle.h"

void handle_message(const char *msg) {
    // Kiểm tra và xử lý từng loại tin nhắn
    if (strstr(msg, "IP:") == msg) {
        printf("Handling IP message\n");
        write_log(MSG_INFO, "Received IP message: %s", msg);
    } else if (strstr(msg, "Hello") == msg) {
        printf("Handling default message\n");
        write_log(MSG_INFO, "Received default message: %s", msg);
    } else if (strstr(msg, "Host:") == msg) {
        printf("Handling host information message\n");
        write_log(MSG_INFO, "Received host info message: %s", msg);
    } else {
        printf("Handling custom message\n");
        write_log(MSG_INFO, "Received custom message: %s", msg);
    }
}
