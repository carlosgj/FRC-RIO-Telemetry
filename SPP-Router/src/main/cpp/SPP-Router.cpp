#include "SPP-Router.h"

/* The purpose of this process is, essentially, SPP-over-UDP multiplexing. 
Outbound: a telemetry-sending process sends SPP-over-UDP packets to 127.0.0.1:INTPORT. 
    This process then sends them from 0.0.0.0:EXTPORT to DESTADDRESS:DESTPORT. 
Inbound: another host sends SPP-over-UDP packets to 0.0.0.0:EXTPORT. 
    This process parses the APID field of the encapsulated SPP packet. 
    It then sends the SPP-over-UDP packet from 127.0.0.1:INTPORT to 127.0.0.1:<proc>PORT, based on the APID. 
*/

int makeUDPsock(in_addr_t address, uint16_t port){
    int sockfd;
    struct sockaddr_in addr;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
            perror("Socket creation failed"); 
            exit(EXIT_FAILURE); 
    } 
    memset(&addr, 0, sizeof(addr));
    addr.sin_family    = AF_INET;
    addr.sin_addr.s_addr = address; 
    addr.sin_port = htons(port);
    if ( bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){  
        exit(EXIT_FAILURE); 
    }
    return sockfd;
}

void setup(){
    int_sock = makeUDPsock(inet_addr("127.0.0.1"), INTPORT);
    ext_sock = makeUDPsock(inet_addr("10.6.96.2"), EXTPORT);

    //Set up destination address
    memset(&destaddr, 0, sizeof(destaddr));
    destaddr.sin_family    = AF_INET;
    destaddr.sin_addr.s_addr = inet_addr(DESTADDRESS); 
    destaddr.sin_port = htons(DESTPORT);
}

void run_inbound(){
    uint16_t n;
    socklen_t addrlen;
    in_addr_t recaddr;
    char buffer[RECV_BUFFER_LEN]; 
    while(true){
        //Receive inbound commands
        n = recvfrom(ext_sock, buffer, RECV_BUFFER_LEN, MSG_WAITALL, ( struct sockaddr *) &recaddr, &addrlen);
        if(n > 0){
            //Handle inbound packet
        }
    }
}

void run_outbound(){
    uint16_t n;
    socklen_t addrlen;
    in_addr_t recaddr;
    char buffer[RECV_BUFFER_LEN]; 
    while(true){
        //Receive outbound telemetry
        n = recvfrom(int_sock, buffer, RECV_BUFFER_LEN, MSG_WAITALL, ( struct sockaddr *) &recaddr, &addrlen);
        if(n > 0){
            sendto(ext_sock, buffer, n, MSG_CONFIRM, (const struct sockaddr *) &destaddr, sizeof(destaddr));
        }
    }
}

int main(){
    setup();
    std::thread inbound(run_inbound);
    std::thread outbound(run_outbound);
    inbound.join();
    outbound.join();
}