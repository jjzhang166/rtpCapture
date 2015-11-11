#include "RTPDump.h"
#include <iostream>
#include "Poco/Net/SocketDefs.h"

RTPDump::RTPDump(std::string& fileName, int videoFrequency) : 
    m_is_video_begin(0),
    m_flv(NULL),
    is_sps_pps_ok(0),
    audioLastTime(0),
    base_audio_time(0),
    videoLastTime(0),
    base_video_time(0),
    m_length(0),
    m_nal_length(0),
    m_cameraType(DEFAULT),
    m_outputFileName(fileName),
    m_logger(Poco::Logger::get("RTPDump"))
{
    m_videoFrequency = videoFrequency / 1000;
}

bool RTPDump::initFLVHeader(void)
{
    m_flv = srs_flv_open_write(m_outputFileName.c_str());
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
        return false;
    }
    return true;
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
    if(len < 12)    // rtp must more than 12 bytes
    {
        std::cout << "not a rtp packet." << std::endl;
        return;
    }

    // Serialization rtp
    rtp_header *rh = (rtp_header *)((u_char*)buf);
    char* h264_buf = buf + 12;
    u_int32_t h264_len = len - 12;

    if(rh->version != 2)  // rtp version must equal 2 , RFC1889
    {
        return;
    }

    if(rh->pt == 0 || rh->pt == 8) // rtp payload type is PCMU(0) PCMA(8)
    {        
        if(m_is_video_begin)
            audioHandler(h264_buf, h264_len, poco_ntoh_32(rh->ts), rh->m, rh->pt == 0 ? U_LAW : A_LAW);
        return;
    }
    else if(rh->pt == 96) // DynamicRTP-Type-96 rtsp camera
    {         
        // detect camera type
        if(m_cameraType == DEFAULT)
        {
            if(h264_buf[0] == 0|| h264_buf[1] == 0 || h264_buf[2] == 1)
            {
                m_psOutfile.open(m_outputFileName.c_str(),std::ofstream::binary);
                m_cameraType = PS_CAMERA;
            }
            else if((h264_buf[0] == 0x67) || (h264_buf[0] == 0x68) ||((h264_buf[0] & 0x1f) == 28))
            {
                m_cameraType = RTSP_CAMERA;
            }
            else
            {
                m_psOutfile.open(m_outputFileName.c_str(),std::ofstream::binary);
                m_cameraType = PS_CAMERA;
            }
        }
        
        if(m_cameraType == PS_CAMERA)
        {
            m_psOutfile.write (h264_buf, h264_len);
            return;
        }
    }
    else if(rh->pt == 98)
    {
    }
    else
    {
        return;
    }
    
    if(m_flv == NULL)
    {
        if(!initFLVHeader())
            return;
    }
    
    if((h264_buf[0] & 0x1f) == 28)   // FU-A
    {
        if(h264_buf[1] & 0x80) // NAL Start
        {
            m_nal_data[0] = ((h264_buf[0] & 0xe0) | (h264_buf[1] & 0x1f));
            m_nal_length = 1;
        }
        memcpy(m_nal_data + m_nal_length, h264_buf + 2, h264_len - 2);
        m_nal_length += (h264_len - 2);
        
        if(h264_buf[1] & 0x40) // NAL End
        {
            videoHandler(m_nal_data, m_nal_length, poco_ntoh_32(rh->ts), rh->m);
        }                
    }
    else
    {
        videoHandler(h264_buf, h264_len, poco_ntoh_32(rh->ts), rh->m);
    }
}

void RTPDump::audioHandler(char* audio_buf, u_int32_t audio_len, int time, bool marker, AudioType type)
{
    u_int32_t timestamp;
    if (base_audio_time == 0)
    {
        base_audio_time = time;
        audioLastTime = time;
    }

    float time_diff = (float)(time - audioLastTime)/8;
    if(time_diff > 1000) // 1 second
    {
        m_logger.warning("Session which FileName[%s] its audio package timestamp[%d], last package timestamp[%d] , diff time is[%d] millisecond.", 
                            m_outputFileName, time, audioLastTime, (int)time_diff);
        base_audio_time = base_audio_time + (time - audioLastTime);
    }
    timestamp = (time - base_audio_time) / 8;
    audioLastTime = time;

    if (timestamp > 1)
    {
        char *audio_header = audio_buf -1;
        if(type == A_LAW)
            *audio_header = 0x72;
        else if(type == U_LAW)
            *audio_header = 0x82;

        if (srs_flv_write_tag(m_flv, SRS_RTMP_TYPE_AUDIO, timestamp, audio_header, audio_len + 1) != 0) 
        {
            //srs_human_trace("dump rtmp packet failed.");
            //TODO
        }
    }
}

void RTPDump::videoHandler(char* h264_buf, u_int32_t h264_len, int time, bool marker)
{
    u_int32_t timestamp;

    if (base_video_time == 0)
    {
        base_video_time = time;
        videoLastTime = time;
    }

    float time_diff = (float)(time - videoLastTime) / m_videoFrequency;
    if(time_diff > 1000)    // 1 second
    {
        m_logger.warning("Session which FileName[%s] its video package timestamp[%d], last package timestamp[%d] , diff time is[%d] millisecond.", 
                            m_outputFileName, time, videoLastTime, (int)time_diff);
        base_video_time = base_video_time + (time - videoLastTime);
    }
    timestamp = (time - base_video_time) / m_videoFrequency;
    videoLastTime = time;
    //std::cout << "time_diff = " << time_diff  << "  time = " << time << "  timestamp = " << timestamp << std::endl;
    
    if(is_sps_pps_ok != 2)
    {
        if((h264_buf[0] & 0x1f) == 7)
        {
            memcpy(sps, h264_buf, h264_len);
            sps_size = h264_len;
            is_sps_pps_ok++;
        }

        if((h264_buf[0] & 0x1f) == 8)
        {
            is_sps_pps_ok++;
            pps_size = h264_len > 4 ? 4 : h264_len;
            memcpy(pps, h264_buf, pps_size);
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
        if((h264_buf[0] & 0x1f) == 5|| (h264_buf[0] & 0x1f) == 1)
        {
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
                if((h264_buf[0] & 0x1f) == 5)
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
    m_is_video_begin = 1;
}

