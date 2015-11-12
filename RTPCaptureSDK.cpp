#include "ReceiverCenter.h"
#include "CaptureCenter.h"
#include "RTPCaptureSDK.h"

#include "Poco/FileChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

RTPCaptureSDK::RTPCaptureSDK(std::string logName)
{
    Poco::FormattingChannel* Fchannel = new Poco::FormattingChannel(
                        new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%T]:[%q] %s: %t"),
                        new Poco::FileChannel(logName));
    Fchannel->getChannel()->setProperty("rotation", "10 M");
    Poco::Logger::root().setChannel(Fchannel);
    
    m_receiverCenter = new ReceiverCenter();
    m_captureCenter = new CaptureCenter(m_receiverCenter->m_memoryPool);
}

RTPCaptureSDK::~RTPCaptureSDK()
{
}

int RTPCaptureSDK::addCaptureRtpSession(std::string fileName, int listenVideoPort, int listenAudioPort, int videoFrequency)
{
    // TODO 如果端口、文件名已经存在异常处理
    
    RTPSession  *s = new RTPSession;
    s->fileName = fileName;
    s->listenVideoPort  = listenVideoPort;
    s->listenAudioPort = listenAudioPort;
    s->videoFrequency = videoFrequency;
    
    m_receiverCenter->addCaptureRtpSession(s);
    m_captureCenter->addCaptureRtpSession(s);
    m_sessionVector.push_back(s);
    
    Poco::Logger::get("RTPCaptureSDK").information("Add Session sessionid[%d] videoPort[%d] audioPort[%d] fileName[%s] videoFrequency[%d] success.",\
    s->SessionID, s->listenVideoPort, s->listenAudioPort, s->fileName, s->videoFrequency);
    
    return s->SessionID;
}
void RTPCaptureSDK::delCaptureRtpSession(int session)
{    
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
