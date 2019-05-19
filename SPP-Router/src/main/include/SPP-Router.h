#pragma once

#include <stdint.h>
//#include "CCSDS/CCSDS.h"
#include "../../../../ports.h"
#include "../../../../CCSDS/src/main/include/CCSDS.h"
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <cstring>
#include <cstdio>
#include <thread>
#include <iostream>

#define RECV_BUFFER_LEN 200 //Does it really need to be this big?
int int_sock;
int ext_sock;
struct sockaddr_in intaddr, extaddr, destaddr;

int makeUDPsock(in_addr_t address, uint16_t port);
void setup();
void run_inbound();
void run_outbound();
void run();
int main();