#ifndef RTPCAPTURE_HPP
#define RTPCAPTURE_HPP

#ifdef __cplusplus
extern "C"{
#endif

void onRTPData(int session, char* data, int len);

int addCaptureRtpSession(char* fileName, int videoFrequency);

void delCaptureRtpSession(int session);

#ifdef __cplusplus
}
#endif

#endif
