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

void ReceiverCenter::addCaptureRtpSession(RTPSession *s)
{
    // TODO 如果端口、文件名已经存在异常处理
    
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);    
    {
        Poco::Net::DatagramSocket videoSocket;
        videoSocket.bind(Poco::Net::SocketAddress("0.0.0.0", s->listenVideoPort), true);
        
        Poco::Net::DatagramSocket audioSocket;
        audioSocket.bind(Poco::Net::SocketAddress("0.0.0.0", s->listenAudioPort), true);
        
        RTPHandler * h = new RTPHandler(audioSocket, videoSocket, m_reactor, m_memoryPool, s);
        s->rtpHandler = h;
    }
    
    m_sessionVector.push_back(s);
    
    m_logger.information("Add Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
    s->SessionID, s->listenVideoPort, s->listenAudioPort, s->fileName, s->videoFrequency);
}
void ReceiverCenter::delCaptureRtpSession(RTPSession *s)
{
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    
    std::vector<RTPSession*>::iterator result = find( m_sessionVector.begin( ), m_sessionVector.end( ), s );
    
    if(result !=m_sessionVector.end())
    {
        m_logger.information("Del Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
        (*result)->SessionID, (*result)->listenVideoPort, (*result)->listenAudioPort, (*result)->fileName, (*result)->videoFrequency);
        
        delete (*result)->rtpHandler;
        m_sessionVector.erase(result);
    }
    else
    {
        m_logger.information("Del Session sessionid[?] error, can not find it.");
    }
}
