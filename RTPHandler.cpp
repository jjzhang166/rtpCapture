#include "RTPHandler.h"
#include "Header.h"

RTPHandler::RTPHandler(Poco::Net::DatagramSocket socket, Poco::Net::SocketReactor& reactor, Poco::MemoryPool&  memoryPool, RTPSession *s):
    m_socket(socket),
    m_reactor(reactor),
    m_memoryPool(memoryPool),
    m_session(s),
    m_logger(Poco::Logger::get("RTPHandler"))
{
    m_reactor.addEventHandler(m_socket, Poco::NObserver<RTPHandler, Poco::Net::ReadableNotification>(*this, &RTPHandler::onSocketReadable));
}

RTPHandler::~RTPHandler()
{
    m_reactor.removeEventHandler(m_socket, Poco::NObserver<RTPHandler, Poco::Net::ReadableNotification>(*this, &RTPHandler::onSocketReadable));
}

void RTPHandler::onSocketReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf)
{
    if (m_socket.available())
    {
        void *buffer = m_memoryPool.get();
        int n = m_socket.receiveBytes(buffer, m_memoryPool.blockSize());
        
        Poco::FastMutex::ScopedLock lock(m_session->bufferMutex);
        RTPSession::buffer_node node;
        node.buffer = buffer;
        node.len = n;
        m_session->bufferList.push_back(node);
        
        m_logger.information("receive : %d", n);
        //std::cout << "receive : " << n << std::endl;
        //std::cout << "pool allocated : " << m_memoryPool.allocated() << std::endl;
        //std::cout << "pool available : " << m_memoryPool.available() << std::endl;
        //std::cout << "m_bufferList size : " << m_session->bufferList.size() << std::endl;
    }
}
