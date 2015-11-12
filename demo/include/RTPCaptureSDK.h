#ifndef RTPCAPTURESDK_H
#define RTPCAPTURESDK_H
#include <vector>

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
        
    int addCaptureRtpSession(std::string fileName, int listenVideoPort, int listenAudioPort, int videoFrequency = 90000);
    void delCaptureRtpSession(int session);

private:
    ReceiverCenter              *m_receiverCenter;
    CaptureCenter               *m_captureCenter;
    std::vector<RTPSession*>     m_sessionVector;
};


#endif // 
