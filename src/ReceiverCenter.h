#ifndef RECEIVERCENTER_H
#define RECEIVERCENTER_H

#include "Header.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Thread.h"
#include "Poco/MemoryPool.h"
#include "Poco/Logger.h"

class ReceiverCenter
{
public:
	ReceiverCenter();
		/// Creates the ReceiverCenter.

	~ReceiverCenter();
		/// Destroys the ReceiverCenter.
        
    void addCaptureRtpSession(RTPSession *s);
    void delCaptureRtpSession(RTPSession *s);
    
    void onRTPData(int session, char* data, int len);
public:
    Poco::MemoryPool            m_memoryPool;
private:
    Poco::Net::SocketReactor    m_reactor;
    Poco::Thread                m_thread;
    std::map<int,RTPSession*>   m_sessionMap;
    Poco::FastMutex             m_sessionMapMutex;
    Poco::Logger&               m_logger;
};


#endif //
