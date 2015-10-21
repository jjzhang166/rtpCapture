#include "CaptureCenter.h"

int RTPSession::SessionCounter=0;

CaptureCenter::CaptureCenter(Poco::MemoryPool& memoryPool):
	m_thread("CaptureCenter"),
	m_stop(false),
    m_memoryPool(memoryPool),
    m_logger(Poco::Logger::get("CaptureCenter"))
{
    m_thread.setName("T2");
	m_thread.start(*this);
}

CaptureCenter::~CaptureCenter()
{
	m_stop = true;
	m_thread.join();
}

void CaptureCenter::addCaptureRtpSession(RTPSession *s)
{
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    m_logger.information("addCaptureRtpSession...");
    s->rtpDump = new RTPDump(s->fileName, s->videoFrequency);
    m_sessionVector.push_back(s);
}

void CaptureCenter::delCaptureRtpSession(RTPSession *s)
{
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    
    std::vector<RTPSession*>::iterator result = find( m_sessionVector.begin( ), m_sessionVector.end( ), s );
    m_sessionVector.erase(result);
}

void CaptureCenter::run()
{
    while(!m_stop)
    {
        Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
        for(std::vector<RTPSession*>::iterator it = m_sessionVector.begin(); it !=m_sessionVector.end(); ++it)
        {
            Poco::FastMutex::ScopedLock lock((*it)->bufferMutex);
            while(!(*it)->bufferList.empty())
            {
                RTPSession::buffer_node node = *(*it)->bufferList.begin();
                (*it)->bufferList.erase((*it)->bufferList.begin());
                (*it)->rtpDump->rtpHandler((char*)(node.buffer), node.len);
                m_memoryPool.release(node.buffer);
                
                //std::cout << (*it)->bufferList.size() << std::endl;
                //std::cout << "CaptureCenter pool allocated : " << m_memoryPool.allocated() << std::endl;
                //std::cout << "CaptureCenter pool available : " << m_memoryPool.available() << std::endl;
            }
        }
        //usleep(10);
    }
}
