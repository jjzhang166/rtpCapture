#include "UDPServer.h"
#include "RTPHandler.h"
#include <algorithm>

UDPServer::UDPServer(): 
    m_memoryPool(1500, 10, 20000),
    m_captureCenter(m_memoryPool),
    m_logger(Poco::Logger::get("UDPServer"))
{
    m_thread.setName("T1");
    m_logger.information("memory poll blockSize = %d", (int)m_memoryPool.blockSize());
    m_thread.start(m_reactor);
}

UDPServer::~UDPServer()
{
}

int UDPServer::addCaptureRtpSession(std::string fileName, int listenPort, int videoFrequency)
{
    // TODO 如果端口、文件名已经存在异常处理
    
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    
    RTPSession  *s = new RTPSession;
    s->fileName = fileName;
    s->listenPort  = listenPort;
    s->videoFrequency = videoFrequency;
    
    Poco::Net::DatagramSocket socket;
    socket.bind(Poco::Net::SocketAddress("0.0.0.0", listenPort), true);
    RTPHandler * h = new RTPHandler(socket, m_reactor, m_memoryPool, s);
    
    s->handler = h;
    
    m_captureCenter.addCaptureRtpSession(s);
    
    m_sessionVector.push_back(s);
    
    m_logger.information("Add Session sessionid[%d] listenPort[%d] fileName[%s] videoFrequency[%d] success.",\
    s->SessionID, s->listenPort, s->fileName, s->videoFrequency);
    
    return s->SessionID;
}
void UDPServer::delCaptureRtpSession(int session)
{
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    
    std::vector<RTPSession*>::iterator result = find_if( m_sessionVector.begin(), m_sessionVector.end(), session_finder(session));
    
    if(result !=m_sessionVector.end())
    {
        m_logger.information("Del Session sessionid[%d] listenPort[%d] fileName[%s] videoFrequency[%d] success.",\
        (*result)->SessionID, (*result)->listenPort, (*result)->fileName, (*result)->videoFrequency);
        
        m_sessionVector.erase(result);
    }
    else
    {
        m_logger.information("Del Session sessionid[%d] error, can not find it.", session);
    }
}
