#include "RTPCapture.h"
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
    int sockfd,len;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[2048];
    int sessionId = 0;
    
    //create a UDP socket
    if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
    {
        printf("create socket error.\n");
        return 1;
    }
    
    // zero out the structure
    bzero(&addr, sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(45250);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    
    //bind socket to port
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("bind socket error.\n");
    }
    
    sessionId = addCaptureRtpSession("movie/danbin.flv", 90000);
    
    //keep listening for data
    while(1)
    {
        //try to receive some data, this is a blocking call
        //len = recv(sockfd, buffer, sizeof(buffer), 0);
        len = recvfrom(sockfd, buffer, sizeof(buffer), 0 , (struct sockaddr *)&addr ,&addr_len);  
        
        // call sdk to capture this packet
        onRTPData(sessionId, buffer, len);
    }
    return 0;
}

