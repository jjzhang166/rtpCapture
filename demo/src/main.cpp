#include <iostream>
#include <fstream>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include "RTPCaptureSDK.h"

using namespace std;

struct rtp_session
{
    std::string outputFileName;
    int listenVideoPort;
    int listenAudioPort;
    int videoFrequency;
};

int main(int argc, char* argv[])
{
#if 0
    rtp_session sessions[] = {{"movie/danbin.flv",45250, 45251, 90000}, {"movie/fangwei.flv",45252, 45253, 90000},\
                              {"movie/yilian.flv",45254, 45255, 60000}, {"movie/rtsp.flv",45256, 45257, 90000},\
                              {"movie/ps.vob",45258, 45259, 90000}, {"movie/chaoliu.flv",45260, 45261, 90000}, \
                              {"movie/xingwang.flv",45262, 45263, 90000}};

    const int size = sizeof(sessions) / sizeof(struct rtp_session);
    int fd[size];
    
    // init sdk
    RTPCaptureSDK sdk("log/sample.log");
    
    for(int i = 0; i < size; i++)
    {
        // add one session
        fd[i] = sdk.addCaptureRtpSession(sessions[i].outputFileName,sessions[i].listenVideoPort, \
                                           sessions[i].listenAudioPort, sessions[i].videoFrequency);
        
        std::cout << "Session " << i << " :video port [" <<sessions[i].listenVideoPort \
                  << "] audio port[" << sessions[i].listenAudioPort << "] output file[" \
                  << sessions[i].outputFileName << "] video frequency [" << sessions[i].videoFrequency \
                  << "]" << std::endl;
    }
    
    // del one session not exit
    sdk.delCaptureRtpSession(9);
    
    // del one session
    //sdk.delCaptureRtpSession(fd[4]);

   	while(1)
   	{
        usleep(1000);
   	}
#else
    int sockfd,len;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[2048];
    int sessionId = 0;
    
    //create a UDP socket
    if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "create socket error." << std::endl;
        return 1;
    }
    
    // zero out the structure
    bzero ( &addr, sizeof(addr) );
    addr.sin_family=AF_INET;
    addr.sin_port=htons(45250);
    addr.sin_addr.s_addr=htonl(INADDR_ANY) ;
    
    //bind socket to port
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "bind socket error." << std::endl;
    }

    // init sdk
    RTPCaptureSDK sdk("log/sample.log");
    
    sessionId = sdk.addCaptureRtpSession("movie/danbin.flv", 90000);
    
    //keep listening for data
    while(1)
    {
        //try to receive some data, this is a blocking call
        //len = recv(sockfd, buffer, sizeof(buffer), 0);
        len = recvfrom(sockfd, buffer, sizeof(buffer), 0 , (struct sockaddr *)&addr ,&addr_len);  
        
        // call sdk to capture this packet
        sdk.onRTPData(sessionId, buffer, len);
    }
#endif
    return 0;
}
