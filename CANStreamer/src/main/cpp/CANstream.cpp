#include "CANstream.h"

void foo(){
  while(true){
    std::cout << "Woo!\n";
    std::this_thread::sleep_for (std::chrono::milliseconds(1000));
  }
}

int main(){
  std::thread streamer;
  std::thread listener;
  if(init() == 0){
    std::cout << "Init successful.\n";
    streamer = std::thread(run);
    listener = std::thread(readCommand);
    streamer.join();
    listener.join();
    std::thread bar(foo);
    bar.join();
  }
  return 0;
}

unsigned char init(){
    std::cout << "CAN streamer is starting up...\n";
    HAL_CAN_OpenStreamSession(&canStreamHandle, 0, 0, 100, &canStreamStatus);
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        std::cout << "socket creation failed\n"; 
        return 1;
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    //My port information 
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(CANSTREAMER_PORT); 

    //Destination port information 
    cliaddr.sin_family    = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    cliaddr.sin_port = htons(INTPORT); 
      
    // Bind
    errno = 0;
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){  
      std::cout << "Failed to bind to local socket; exit code " << errno << '\n';
        return 1; 
    }
    std::cout << "Initialization successful.\n";
    return 0;
}


void run(){
  uint32_t messagesRead=0;
  uint8_t i;
  uint8_t j;
  ccsds_frc_header_t headerInfo;
  std::cout<< "Starting telem sender...\n";
  if(isStreaming){
    std::cout << "Stream on...\n";
  }
  headerInfo.ccsds_header.isCommand = false;
  headerInfo.ccsds_header.hasSecondaryHeader = true;
  headerInfo.ccsds_header.APID = CANSTREAMER_APID;
  headerInfo.ccsds_header.seq_type = UNSEGMENTED;
  headerInfo.ccsds_header.seq_count = 0;
  headerInfo.DFI_type = CAN_FRAME;
  headerInfo.DFI_version = 0;
  while(true){
    HAL_CAN_ReadStreamSession (canStreamHandle, messages, 100, &messagesRead, &canStreamStatus);
    if(messagesRead > 0){
      for(i=0; i<messagesRead; i++){
        //Process CAN messages
        uint32_t itime;
        uint32_t ftime;
        make_timestamp(&itime, &ftime);
        headerInfo.integer_time = itime;
        headerInfo.fractional_time = ftime;
        headerInfo.ccsds_header.length = ((messages[i].dataSize+4)+10)-1;
        serialize_CCSDS_FRC_header(headerInfo, bytebuffer);
        headerInfo.ccsds_header.seq_count++;
        
        bytebuffer[16] = (messages[i].messageID >> 24);
        bytebuffer[17] = (messages[i].messageID >> 16);
        bytebuffer[18] = (messages[i].messageID >> 8);
        bytebuffer[19] = (messages[i].messageID);
        if(isStreaming){
          memcpy(bytebuffer+20, messages[i].data, messages[i].dataSize);
          sendto(sockfd, bytebuffer, 20+messages[i].dataSize, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr)); 
          //for(j = 0; j < 20+messages[i].dataSize; j++){
          //  printf("%02X", bytebuffer[j]);
         // }
         // printf("\n");
        }
      }
      //printf("%d\n", messagesRead);
    }
    else{
      std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
  }
}

void readCommand(){
  uint8_t i;
  char buffer[20];
  unsigned int len = 0;
  int n = 0;
  ccsds_frc_header_t header;
  std::cout<< "Starting command listener...\n";
  memset(buffer, 0, 20);
  while(true){
    n = recvfrom(sockfd, buffer, 20, MSG_WAITALL, ( struct sockaddr *) &recaddr, &len); 
    if(n>0){
    for(i=0; i<20; i++){
      printf("%x", buffer[i]);
    }
    printf("\n");
    }

    parse_CCSDS_FRC_header(&header, (uint8_t*)buffer);
    if(header.ccsds_header.isCommand){
      if(header.ccsds_header.APID==CANSTREAMER_APID){
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
}