#ifndef RTPCAPTURESDK_H
#define RTPCAPTURESDK_H

#include "CaptureCenter.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Thread.h"
#include "Poco/MemoryPool.h"
#include "Poco/Logger.h"

class RTPCaptureSDK
{
public:
	RTPCaptureSDK();
		/// Creates the RTPCaptureSDK.

	~RTPCaptureSDK();
		/// Destroys the RTPCaptureSDK.
        
    int addCaptureRtpSession(std::string fileName, int listenPort, int videoFrequency = 90000);
    void delCaptureRtpSession(int session);

private:
    Poco::MemoryPool            m_memoryPool;
	Poco::Net::DatagramSocket   m_socket;
    Poco::Net::SocketReactor    m_reactor;
    Poco::Thread                m_thread;
    CaptureCenter               m_captureCenter;
    std::vector<RTPSession*>    m_sessionVector;
    Poco::FastMutex             m_sessionVectorMutex;
    Poco::Logger&               m_logger;
};


#endif // UDPEchoServer_INCLUDED
