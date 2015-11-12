#include "RTPHandler.h"
#include "Header.h"

RTPHandler::RTPHandler(Poco::Net::DatagramSocket as, Poco::Net::DatagramSocket vs, \
                        Poco::Net::SocketReactor& reactor, Poco::MemoryPool&  memoryPool, RTPSession *s):
    m_audioSocket(as),
    m_videoSocket(vs),
    m_reactor(reactor),
    m_memoryPool(memoryPool),
    m_session(s),
    m_logger(Poco::Logger::get("RTPHandler"))
{
    m_reactor.addEventHandler(m_audioSocket, Poco::NObserver<RTPHandler, Poco::Net::ReadableNotification>(*this, &RTPHandler::onAudioSocketReadable));
    m_reactor.addEventHandler(m_videoSocket, Poco::NObserver<RTPHandler, Poco::Net::ReadableNotification>(*this, &RTPHandler::onVideoSocketReadable));
}

RTPHandler::~RTPHandler()
{
    m_reactor.removeEventHandler(m_audioSocket, Poco::NObserver<RTPHandler, Poco::Net::ReadableNotification>(*this, &RTPHandler::onAudioSocketReadable));
    m_reactor.removeEventHandler(m_videoSocket, Poco::NObserver<RTPHandler, Poco::Net::ReadableNotification>(*this, &RTPHandler::onVideoSocketReadable));
    
    m_audioSocket.close();
    m_videoSocket.close();
    m_logger.information("Del Session sessionid[%d] success.",m_session->SessionID);
}

void RTPHandler::onAudioSocketReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf)
{
    if (m_audioSocket.available())
    {
        void *buffer = m_memoryPool.get();
        int n = m_audioSocket.receiveBytes(buffer, m_memoryPool.blockSize());
          
        RTPSession::buffer_node node;
        node.buffer = buffer;
        node.len = n;
        
        Poco::FastMutex::ScopedLock lock(m_session->bufferMutex);
        m_session->bufferList.push_back(node);
    }
}

void RTPHandler::onVideoSocketReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf)
{
    if (m_videoSocket.available())
    {
        void *buffer = m_memoryPool.get();
        int n = m_videoSocket.receiveBytes(buffer, m_memoryPool.blockSize());
        
        RTPSession::buffer_node node;
        node.buffer = buffer;
        node.len = n;
        
        Poco::FastMutex::ScopedLock lock(m_session->bufferMutex);
        m_session->bufferList.push_back(node);
    }
}
