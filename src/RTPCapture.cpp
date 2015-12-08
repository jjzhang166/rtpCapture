#include "RTPCapture.h"
#include <string>
#include "RTPCaptureSDK.h"

RTPCaptureSDK *sdk = NULL;

#ifdef __cplusplus
extern "C"{
#endif

void onRTPData(int session, char* data, int len)
{
    if(sdk != NULL)
    {
        sdk->onRTPData(session, data, len);
    }
}

int addCaptureRtpSession(char* fileName, int videoFrequency)
{
    if(sdk == NULL)
    {
        sdk = new RTPCaptureSDK(std::string("log/sample.log"));
    }

    return sdk->addCaptureRtpSession(fileName, 90000);
}

void delCaptureRtpSession(int session)
{
    if(sdk != NULL)
    {
        sdk->delCaptureRtpSession(session);
    }
}

#ifdef __cplusplus
}
#endif

