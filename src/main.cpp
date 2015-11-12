#include <iostream>
#include <unistd.h>
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
    /*
    // del one session not exit
    sdk.delCaptureRtpSession(9);
    
    // del one session
    sdk.delCaptureRtpSession(fd4);
    */
   	while(1)
   	{
        sleep(1);
   	}
    return 0;
}
