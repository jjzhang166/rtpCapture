#include <iostream>
#include "RTPCaptureSDK.h"

using namespace std;

int main(int argc, char* argv[])
{   
    // init sdk
    RTPCaptureSDK sdk;
    
    // add one session
    int fd0 = sdk.addCaptureRtpSession("movie/danbin.flv",45254, 90000);
    
    // add one session
    int fd1 = sdk.addCaptureRtpSession("movie/fangwei.flv",45255, 90000);
    
    // add one session
    int fd2 = sdk.addCaptureRtpSession("movie/yilian.flv",45256, 60000);
    
    // add one session
    int fd3 = sdk.addCaptureRtpSession("movie/rtsp.flv",45257, 90000);
    
    std::cout << "RTP Session 1 : listen UDP port [45254] , capture file [danbin.flv] , video frequency [90000]" << std::endl;
    std::cout << "RTP Session 2 : listen UDP port [45255] , capture file [fangwei.flv] , video frequency [90000]" << std::endl;
    std::cout << "RTP Session 3 : listen UDP port [45256] , capture file [yilian.flv] , video frequency [60000]" << std::endl;
    std::cout << "RTP Session 4 : listen UDP port [45257] , capture file [rtsp.flv] , video frequency [90000]" << std::endl;
    /*   
    // add one session
    int fd4 = sdk.addCaptureRtpSession("test4.flv",45258, 90000);
    
    // del one session not exit
    sdk.delCaptureRtpSession(9);
    
    // del one session
    sdk.delCaptureRtpSession(fd4);
    */
   	while(1)
   	{
        Poco::Thread::sleep(1000);
   	}
    return 0;
}
