#ifndef RTPCAPTURESDK_H
#define RTPCAPTURESDK_H
#include <map>

class ReceiverCenter;
class CaptureCenter;
class RTPSession;
class RTPCaptureSDK
{
public:
	RTPCaptureSDK(std::string logName);
		/// Creates the RTPCaptureSDK.

	~RTPCaptureSDK();
		/// Destroys the RTPCaptureSDK.
    
    void onRTPData(int session, char* data, int len);
        /// rtp data with 12 byte header, len is rtp header size plus rtp data size

    int addCaptureRtpSession(std::string fileName, int videoFrequency = 90000);
        /// add rtp capture session just with the fileName, audio and video data should call fun onRTPData(...)
        
    int addCaptureRtpSession(std::string fileName, int listenVideoPort, int listenAudioPort, int videoFrequency = 90000);
        /// add rtp capture session with video port and audio port, SDK will receive rtp data it self.        
        /// return the session id.
    
    void delCaptureRtpSession(int session);
        /// delete session with its session id
    
private:
    ReceiverCenter              *m_receiverCenter;
    CaptureCenter               *m_captureCenter;
    std::map<int,RTPSession*>    m_sessionMap;
};


#endif // 
