#include "ReceiverCenter.h"
#include "RTPHandler.h"

#include "Poco/FileChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

ReceiverCenter::ReceiverCenter():
    m_memoryPool(1500, 10, 20000),
    m_logger(Poco::Logger::get("ReceiverCenter"))
{    
    m_thread.setName("T1");
    m_logger.information("memory poll blockSize = %d", (int)m_memoryPool.blockSize());
    m_thread.start(m_reactor);
}

ReceiverCenter::~ReceiverCenter()
{
}

void ReceiverCenter::onRTPData(int session, char* data, int len)
{
    Poco::FastMutex::ScopedLock lock(m_sessionMapMutex);
    
    std::map<int,RTPSession*>::iterator r = m_sessionMap.find(session);
    
    if(r != m_sessionMap.end())
    {
        if(len < m_memoryPool.blockSize())
        {
            Poco::Logger::get("RTPCaptureSDK").information("Session [%d] data len[%d] is overflow.", session, len);
            return;
        }
        void *buffer = m_memoryPool.get();
          
        RTPSession::buffer_node node;
        node.buffer = buffer;
        node.len = len;
        
        Poco::FastMutex::ScopedLock lock(r->second->bufferMutex);
        r->second->bufferList.push_back(node);
    }
    else
    {
        Poco::Logger::get("RTPCaptureSDK").information("Session [%d] not found when onRTPData call.", session);
    }
}

void ReceiverCenter::addCaptureRtpSession(RTPSession *s)
{
    // TODO 如果端口、文件名已经存在异常处理
    
    Poco::FastMutex::ScopedLock lock(m_sessionMapMutex);
    s->rtpHandler = NULL;
    if(s->listenVideoPort != 0 && s->listenAudioPort != 0)
    {
        Poco::Net::DatagramSocket videoSocket;
        videoSocket.bind(Poco::Net::SocketAddress("0.0.0.0", s->listenVideoPort), true);
        
        Poco::Net::DatagramSocket audioSocket;
        audioSocket.bind(Poco::Net::SocketAddress("0.0.0.0", s->listenAudioPort), true);
        
        RTPHandler * h = new RTPHandler(audioSocket, videoSocket, m_reactor, m_memoryPool, s);
        s->rtpHandler = h;
    }
    
    m_sessionMap[s->SessionID] = s;
    
    m_logger.information("Add Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
    s->SessionID, s->listenVideoPort, s->listenAudioPort, s->fileName, s->videoFrequency);
}
void ReceiverCenter::delCaptureRtpSession(RTPSession *s)
{
    Poco::FastMutex::ScopedLock lock(m_sessionMapMutex);
    
    std::map<int,RTPSession*>::iterator r = m_sessionMap.find(s->SessionID);
    
    if(r !=m_sessionMap.end())
    {
        m_logger.information("Del Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
        r->second->SessionID, r->second->listenVideoPort, r->second->listenAudioPort, r->second->fileName, r->second->videoFrequency);
        
        if(s->rtpHandler != NULL)
        {
            delete r->second->rtpHandler;
        }
        m_sessionMap.erase(r);
    }
    else
    {
        m_logger.information("Del Session sessionid[?] error, can not find it.");
    }
}
