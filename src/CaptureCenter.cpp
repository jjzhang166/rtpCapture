#include "CaptureCenter.h"
#include "RTPDump.h"

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
    s->rtpDump = new RTPDump(s->fileName, s->videoFrequency);
    m_sessionVector.push_back(s);
    m_logger.information("Add Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
                s->SessionID, s->listenVideoPort, s->listenAudioPort, s->fileName, s->videoFrequency);
}

void CaptureCenter::delCaptureRtpSession(RTPSession *s)
{
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    
    std::vector<RTPSession*>::iterator result = find( m_sessionVector.begin( ), m_sessionVector.end( ), s );

    if(result !=m_sessionVector.end())
    {
        m_logger.information("Del Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
                            (*result)->SessionID, (*result)->listenVideoPort, (*result)->listenAudioPort, \
                            (*result)->fileName, (*result)->videoFrequency);
        
        Poco::FastMutex::ScopedLock lock((*result)->bufferMutex);
        while(!(*result)->bufferList.empty())
        {
            RTPSession::buffer_node node = *(*result)->bufferList.begin();
            (*result)->bufferList.erase((*result)->bufferList.begin());
            m_memoryPool.release(node.buffer);
        }
        
        delete (*result)->rtpDump;
        m_sessionVector.erase(result);
    }
    else
    {
        m_logger.information("Del Session sessionid[?] error, can not find it.");
    }
}

void CaptureCenter::run()
{
    bool hasTask = false;
    while(!m_stop)
    {
        {
            hasTask = false;
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
                    
                    hasTask = true;
                    
                    //std::cout << (*it)->bufferList.size() << std::endl;
                    //std::cout << "CaptureCenter pool allocated : " << m_memoryPool.allocated() << std::endl;
                    //std::cout << "CaptureCenter pool available : " << m_memoryPool.available() << std::endl;
                }
            }
        }
        if(!hasTask)
            usleep(100);
    }
}
