#ifndef RTPDUMP_H
#define RTPDUMP_H

#include "Poco/Logger.h"
#include "srs_librtmp.h"

class RTPDump
{
private:
    typedef struct rtp_header
    {
        u_int16_t cc : 4;         // CSRC count
        u_int16_t x : 1;          // header extension flag
        u_int16_t p : 1;          // padding flag
        u_int16_t version : 2;    // protocol version
        u_int16_t pt : 7;         // payload type
        u_int16_t m : 1;          // marker bit

        u_int16_t seq;            // sequence number
        u_int32_t ts;               // timestamp
        u_int32_t ssrc;             // synchronization source
    }rtp_header;

public:
    RTPDump(std::string& fileName, int videoFrequency);
    ~RTPDump();
    void videoHandler(char* h264_buf, u_int32_t h264_len, int time, bool marker);
    void audioHandler(char* audio_buf, u_int32_t audio_len, int time, bool marker);
    void rtpHandler(char* buf, int len);
private:
    int                 m_is_video_begin;
    srs_flv_t           m_flv;
    int                 is_sps_pps_ok;
    
    // audio
    u_int32_t audioLastTimestamp;
    int audioLastTime;
    int base_audio_time;
    
    // video
    u_int32_t videoLastTimestamp;
    int videoLastTime;
    int base_video_time;
    
    char sps[128];
    int sps_size;
    char pps[128];
    int pps_size;
    char video_data[512*1024];
    unsigned int m_length;
    
    int m_videoFrequency;
    
    Poco::Logger&       m_logger;
};

#endif
