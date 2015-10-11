#ifndef CAPTURECENTER_H
#define CAPTURECENTER_H

#include <iostream>
#include "Header.h"
#include "RTPDump.h"
#include "Poco/Logger.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/MemoryPool.h"

class CaptureCenter: public Poco::Runnable
{
public:
	CaptureCenter(Poco::MemoryPool& memoryPool);
		/// Creates the CaptureCenter.

	~CaptureCenter();
		/// Destroys the CaptureCenter.
        
    void addCaptureRtpSession(RTPSession *s);
    void delCaptureRtpSession(RTPSession *s);
        
	void run();
		/// Does the work.
private:
	Poco::Thread                m_thread;
	bool                        m_stop;
    Poco::MemoryPool&           m_memoryPool;
    std::vector<RTPSession*>    m_sessionVector;
    Poco::FastMutex             m_sessionVectorMutex;
    Poco::Logger&               m_logger;
};

#endif
