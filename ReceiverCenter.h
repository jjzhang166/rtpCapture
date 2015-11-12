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
public:
    Poco::MemoryPool            m_memoryPool;
private:
    Poco::Net::SocketReactor    m_reactor;
    Poco::Thread                m_thread;
    std::vector<RTPSession*>    m_sessionVector;
    Poco::FastMutex             m_sessionVectorMutex;
    Poco::Logger&               m_logger;
};


#endif //
