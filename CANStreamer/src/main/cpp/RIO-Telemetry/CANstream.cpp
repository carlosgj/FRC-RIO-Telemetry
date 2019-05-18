#include "RIO-Telemetry/CANstream.h"

#define PORT     8080 //Port to bind to

void doit(){
    std::thread first (athing);
    first.detach();
}

void athing(){
    while(true){
    std::cout << "Woo!\n";
    std::this_thread::sleep_for (std::chrono::seconds(1));
    }
}

void CANstreamer::CANStreamer(){
    HAL_CAN_OpenStreamSession(&canStreamHandle, 0, 0, 100, &canStreamStatus);
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    //Server information 
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 

    //Client information 
    cliaddr.sin_family    = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr("10.6.96.5"); 
    cliaddr.sin_port = htons(PORT); 
      
    // Bind
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){  
        exit(EXIT_FAILURE); 
    }       
}


void CANstreamer::run(){
    uint32_t messagesRead=0;
    uint8_t i;
    HAL_CAN_ReadStreamSession (canStreamHandle, messages, 100, &messagesRead, &canStreamStatus);
    for(i=0; i<messagesRead; i++){
        //Process CAN messages
        create_CCSDS_header(false, 0, UNSEGMENTED, CCSDS_count++, messages[i].dataSize+4, bytebuffer);
      bytebuffer[6] = (messages[i].messageID >> 24);
      bytebuffer[7] = (messages[i].messageID >> 16);
      bytebuffer[8] = (messages[i].messageID >> 8);
      bytebuffer[9] = (messages[i].messageID);
      if(isStreaming){
        memcpy(bytebuffer+10, messages[i].data, messages[i].dataSize);
        sendto(sockfd, bytebuffer, 10+messages[i].dataSize,  
          MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr)); 
      }
    }
    printf("%d\n", messagesRead);
}

void CANstreamer::readCommand(){
    uint8_t i;
    char buffer[20];
    memset(buffer, 0, 20);
    unsigned int len = 0;
    int n = 0;
    bool isCommand;
    uint16_t APID;
    uint16_t count;
    uint16_t length;
    
    n = recvfrom(sockfd, buffer, 20, MSG_DONTWAIT, ( struct sockaddr *) &recaddr, &len); 
    if(n>0){
    for(i=0; i<20; i++){
      printf("%x", buffer[i]);
    }
    printf("\n");
    }

    CCSDS_sequence_type seqtype;
    parse_CCSDS_header(&isCommand, &APID, &seqtype, &count, &length, (uint8_t*)buffer);
    if(isCommand){
      if(APID==129){
        //This is a message to CAN streamer
        switch(buffer[6]){
          case 0:
            //TBD
            break;
          case 1:
            //Turn off
            isStreaming = false;
            break;
          case 2:
            //Turn on
            isStreaming = true;
            break;
          default:
            break;
        }
      }
    }
    
}