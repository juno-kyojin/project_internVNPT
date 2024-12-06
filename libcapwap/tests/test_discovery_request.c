#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "capwap.h"

int main(){
    uint8_t buffer[CAPWAP_MAX_PACKET_SIZE];
    size_t pkt_len=capwap_build_discovery_response(buffer,sizeof(buffer));
    if(pkt_len==0){
        fprintf(stderr,"Fail to build discovery request\n");
        return 1;
    }

    // Gửi gói tin
    send_udp_packet(buffer,pkt_len,"127.0.0.1",5246);

    printf("Đã gửi Discovery Request.\n");
    return 0;
}
