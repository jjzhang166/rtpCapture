#include "RTPCaptureSDK.h"
#include "RTPHandler.h"

#include "Poco/FileChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

RTPCaptureSDK::RTPCaptureSDK(std::string logName): 
    m_memoryPool(1500, 10, 20000),
    m_captureCenter(m_memoryPool)
{
    Poco::FormattingChannel* Fchannel = new Poco::FormattingChannel(
                        new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%T]:[%q] %s: %t"),
                        new Poco::FileChannel(logName));
    Fchannel->getChannel()->setProperty("rotation", "10 M");
    Poco::Logger::root().setChannel(Fchannel);
    
    m_thread.setName("T1");
    Poco::Logger::get("RTPCaptureSDK").information("memory poll blockSize = %d", (int)m_memoryPool.blockSize());
    m_thread.start(m_reactor);
}

RTPCaptureSDK::~RTPCaptureSDK()
{
}

int RTPCaptureSDK::addCaptureRtpSession(std::string fileName, int listenVideoPort, int listenAudioPort, int videoFrequency)
{
    // TODO 如果端口、文件名已经存在异常处理
    
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    
    RTPSession  *s = new RTPSession;
    s->fileName = fileName;
    s->listenVideoPort  = listenVideoPort;
    s->listenAudioPort = listenAudioPort;
    s->videoFrequency = videoFrequency;
    
    {
        Poco::Net::DatagramSocket videoSocket;
        videoSocket.bind(Poco::Net::SocketAddress("0.0.0.0", listenVideoPort), true);
        RTPHandler * h = new RTPHandler(videoSocket, m_reactor, m_memoryPool, s);
        s->videoHandler = h;
    }
    {
        Poco::Net::DatagramSocket audioSocket;
        audioSocket.bind(Poco::Net::SocketAddress("0.0.0.0", listenAudioPort), true);
        RTPHandler * h = new RTPHandler(audioSocket, m_reactor, m_memoryPool, s);
        s->audioHandler = h;
    }
    
    m_captureCenter.addCaptureRtpSession(s);
    
    m_sessionVector.push_back(s);
    
    Poco::Logger::get("RTPCaptureSDK").information("Add Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
    s->SessionID, s->listenVideoPort, s->listenAudioPort, s->fileName, s->videoFrequency);
    
    return s->SessionID;
}
void RTPCaptureSDK::delCaptureRtpSession(int session)
{
    Poco::FastMutex::ScopedLock lock(m_sessionVectorMutex);
    
    std::vector<RTPSession*>::iterator result = find_if( m_sessionVector.begin(), m_sessionVector.end(), session_finder(session));
    
    if(result !=m_sessionVector.end())
    {
        Poco::Logger::get("RTPCaptureSDK").information("Del Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
        (*result)->SessionID, (*result)->listenVideoPort, (*result)->listenAudioPort, (*result)->fileName, (*result)->videoFrequency);
        
        m_sessionVector.erase(result);
    }
    else
    {
        Poco::Logger::get("RTPCaptureSDK").information("Del Session sessionid[%d] error, can not find it.", session);
    }
}
