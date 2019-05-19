#pragma once

#include <stdint.h>
#include <hal/CAN.h>
#include "../../../../CCSDS/src/main/include/CCSDS.h"
#include "../../../../ports.h"
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <cstring>
#include <cstdio>
#include <thread>
#include <iostream>
#include <errno.h>

int main();
unsigned char init();
void run();
void readCommand();   

enum canstreamer_dfi {
    NOOP = 0,
    ACK = 1,
    NACK = 2,
    CAN_FRAME = 3,
    COMMAND = 4,
};

uint8_t bytebuffer[30];
bool isStreaming = true;
uint32_t canStreamHandle;
int32_t canStreamStatus;
HAL_CANStreamMessage messages[50];
int sockfd = 0;
struct sockaddr_in servaddr, cliaddr, recaddr; 
bool initGood = false;