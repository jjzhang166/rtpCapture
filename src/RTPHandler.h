#ifndef RTPHANDLER_H
#define RTPHANDLER_H

#include "Poco/Mutex.h"
#include "Poco/NObserver.h"
#include "Poco/Thread.h"
#include "Poco/MemoryPool.h"
#include "Poco/Logger.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"

class RTPSession;
class RTPHandler
{
public:
	RTPHandler(Poco::Net::DatagramSocket as, Poco::Net::DatagramSocket vs, \
                Poco::Net::SocketReactor& reactor, Poco::MemoryPool&  memoryPool, RTPSession *s);
	
	~RTPHandler();
	
	void onAudioSocketReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf);
    void onVideoSocketReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf);
	
private:	
	Poco::Net::DatagramSocket   m_audioSocket;
    Poco::Net::DatagramSocket   m_videoSocket;
	Poco::Net::SocketReactor&   m_reactor;
    Poco::MemoryPool&           m_memoryPool;
    RTPSession*                 m_session;
    Poco::Logger&               m_logger;
};

#endif
