#ifndef RTPDUMP_H
#define RTPDUMP_H

#include "srs_librtmp.h"
#include "Poco/Logger.h"

class RTPDump
{
private:
    typedef struct rtp_header
    {
        u_short cc : 4;         // CSRC count
        u_short x : 1;          // header extension flag
        u_short p : 1;          // padding flag
        u_short version : 2;    // protocol version
        u_short pt : 7;         // payload type
        u_short m : 1;          // marker bit

        u_short seq;            // sequence number
        u_int ts;               // timestamp
        u_int ssrc;             // synchronization source
    }rtp_header;

public:
    RTPDump(std::string& fileName);
    ~RTPDump();
    void videoHandler(char* h264_buf, u_int32_t h264_len, u_int time, bool marker);
    void audioHandler(char* audio_buf, u_int32_t audio_len, u_int time, bool marker);
    void rtpHandler(char* buf, int len);
private:
    int                 m_is_video_begin;
    srs_flv_t           m_flv;
    int                 is_sps_pps_ok;
    Poco::Logger&       m_logger;
};

#endif
