#pragma once

#include <stdint.h>
#include <hal/CAN.h>
#include "CCSDS/CCSDS.h"
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <cstring>
#include <cstdio>
#include <thread>
#include <iostream>

void athing();
void doit();

class CANstreamer{
    public:
        void CANStreamer();
        void run();
        void readCommand();
    private:     
        uint8_t bytebuffer[14];
        bool isStreaming;
        uint32_t canStreamHandle;
        int32_t canStreamStatus;
        HAL_CANStreamMessage messages[50];
        uint16_t CCSDS_count = 0;
        int sockfd;
        struct sockaddr_in servaddr, cliaddr, recaddr; 
};