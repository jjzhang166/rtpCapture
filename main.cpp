#include <iostream>
#include "UDPServer.h"

#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/FileChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

using namespace std;

int main(int argc, char* argv[])
{
    Poco::FormattingChannel* Fchannel = new Poco::FormattingChannel(new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%T]:[%q] %s: %t"));
    Fchannel->setChannel(new Poco::FileChannel("sample.log"));
    Fchannel->getChannel()->setProperty("rotation", "1 M");
    Poco::Logger::root().setChannel(Fchannel);
    Poco::Logger& logger = Poco::Logger::get("main"); // inherits root channel
    
    // init sdk
    UDPServer udpServer;
    logger.information("init sdk ...");
    
    // add one session
    int fd1 = udpServer.addCaptureRtpSession("test1.flv",45255);
    
    // add one session
    int fd2 = udpServer.addCaptureRtpSession("test2.flv",45256);
    
    // del one session
    udpServer.delCaptureRtpSession(9);
    
    
    // del one session
    udpServer.delCaptureRtpSession(fd2);
    
    logger.information("go go go");
    
   	while(1)
   	{
   		sleep(1);
   	}
    return 0;
}
