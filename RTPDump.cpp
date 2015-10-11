#include "RTPDump.h"
#include <arpa/inet.h>
#include <iostream>

RTPDump::RTPDump(std::string& fileName) : 
    m_is_video_begin(0),
    m_flv(NULL),
    is_sps_pps_ok(0),
    m_logger(Poco::Logger::get("RTPDump"))
{
    m_flv = srs_flv_open_write(fileName.c_str());
    if (m_flv) 
    {
        // flv header
        char header[9];
        // 3bytes, signature, "FLV",
        header[0] = 'F';
        header[1] = 'L';
        header[2] = 'V';
        // 1bytes, version, 0x01,
        header[3] = 0x01;
        // 1bytes, flags, UB[5] 0, UB[1] audio present, UB[1] 0, UB[1] video present.
        header[4] = 0x05; // audio + video.
        // 4bytes, dataoffset
        header[5] = 0x00;
        header[6] = 0x00;
        header[7] = 0x00;
        header[8] = 0x09;
        if (srs_flv_write_header(m_flv, header) != 0) 
        {
            std::cout << "write flv header failed." << std::endl;
        }
    }
    else
    {
        m_flv = NULL;
        std::cout << "open flv file failed." << std::endl;
    }
}

RTPDump::~RTPDump()
{
    if(m_flv)
    {
        srs_flv_close(m_flv);
    }
}

void RTPDump::rtpHandler(char* buf, int len)
{
    if(m_flv ==NULL)
    {
        std::cout << "FLV file open error." << std::endl;
        return;
    }
    if(len < 12)    // rtp must more than 12 bytes
    {
        std::cout << "not a rtp packet." << std::endl;
        return;
    }

    // Serialization rtp
    rtp_header *rh = (rtp_header *)((u_char*)buf);

    if(rh->version != 2)  // rtp version must equal 2 , RFC1889
    {
        return;
    }

    switch(rh->pt)
    {
        case 8 :
        {
            // rtp payload type is PCMA
            if(m_is_video_begin)
                audioHandler(buf + 12, len - 12, ntohl(rh->ts), rh->m);
            break;
        }
        case 98 :
        {
            // rtp payload type is H264
            videoHandler(buf + 12, len - 12, ntohl(rh->ts), rh->m);
            break;
        }
        default :
        {
            break;
        }
    }
}

void RTPDump::audioHandler(char* audio_buf, u_int32_t audio_len, u_int time, bool marker)
{
    static u_int32_t audioLastTimestamp = 0;
    static u_int audioLastTime = 0;
    static u_int base_audio_time = 0;

    u_int32_t timestamp;
    if (base_audio_time == 0)
    {
        base_audio_time = time;
    }

    timestamp = (time - base_audio_time) / 8;
    float time_diff = (float)(time - audioLastTime)/8;

    if(time_diff > 1000)
    {
        timestamp = audioLastTimestamp + 33;
        base_audio_time = time;
    }
    audioLastTimestamp = timestamp;

    if (timestamp > 1)
    {
        char *audio_header = audio_buf -1;
        *audio_header = 0x72;

        if (srs_flv_write_tag(m_flv, SRS_RTMP_TYPE_AUDIO, timestamp, audio_header, audio_len + 1) != 0) 
        {
            //srs_human_trace("dump rtmp packet failed.");
            //TODO
        }
    }
    audioLastTime = time;
}

void RTPDump::videoHandler(char* h264_buf, u_int32_t h264_len, u_int time, bool marker)
{
    u_int32_t timestamp;
    static u_int lastTime = 0;
    static u_int32_t lastTimestamp;
    static u_int base_video_time = 0;

    if (base_video_time == 0)
    {
        base_video_time = time;
    }

    timestamp = (time - base_video_time) / 90;
    float time_diff = (float)(time - lastTime) / 90;

    //std::cout << "size = " << h264_len << std::endl;

    if(time_diff > 1000)
    {
        timestamp = lastTimestamp + 33;
        base_video_time = time;
    }
    lastTimestamp = timestamp;

    if(is_sps_pps_ok != 2)
    {
        static char sps[128];
        static int sps_size;
        static char pps[128];
        static int pps_size;
        if((h264_buf[0] & 0xf) == 7)
        {
            memcpy(sps, h264_buf, h264_len);
            sps_size = h264_len;
            is_sps_pps_ok++;
        }

        if((h264_buf[0] & 0xf) == 8)
        {
            is_sps_pps_ok++;
            memcpy(pps, h264_buf, h264_len);
            pps_size = h264_len;
        }

        if(is_sps_pps_ok == 2)
        {
            if(srs_flv_write_sps_pps_tag(m_flv, timestamp, sps, sps_size, pps, pps_size) != 0)
            {
                srs_human_trace("dump rtmp packet failed.");
            }
        }
    }
    else
    {
        if((h264_buf[0] & 0xf) == 5|| (h264_buf[0] & 0xf) == 1)
        {
            static char video_data[512*1024];
            static unsigned int m_length = 0;

            char *pp = (char*)&h264_len;
            char *p = video_data + 5 + m_length;
            p[0] = pp[3];
            p[1] = pp[2];
            p[2] = pp[1];
            p[3] = pp[0];

            memcpy(p + 4, h264_buf, h264_len);

            m_length += (h264_len + 4);

			if(marker)
            {
                if((h264_buf[0] & 0xf) == 5)
                    memcpy(video_data, "\x17\x01\x00\x00\x00", 5); // AVCVIDEOPACKET
                else
                    memcpy(video_data, "\x27\x01\x00\x00\x00", 5); // AVCVIDEOPACKET
                
                
                if (srs_flv_write_tag(m_flv, SRS_RTMP_TYPE_VIDEO, timestamp, video_data, m_length + 5) != 0) 
				{
					srs_human_trace("dump rtmp packet failed.");
                }

                m_length = 0;
            }
        }
    }
	
    lastTime = time;
    m_is_video_begin = 1;
}

