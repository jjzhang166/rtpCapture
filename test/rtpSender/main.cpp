#include <iostream>
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"

#include "pcap/pcap.h"

using namespace std;

enum IPProtcol
{
    ICMP = 1,
    TCP = 6,
    UDP = 17
};

/* 4 bytes IP address */
typedef struct ip_address
{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header
{
    u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
    u_char  tos;            // Type of service 
    u_short tlen;           // Total length 
    u_short identification; // Identification
    u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
    u_char  ttl;            // Time to live
    u_char  proto;          // Protocol
    u_short crc;            // Header checksum
    ip_address  saddr;      // Source address
    ip_address  daddr;      // Destination address
    u_int   op_pad;         // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
    u_short sport;          // Source port
    u_short dport;          // Destination port
    u_short len;            // Datagram length
    u_short crc;            // Checksum
}udp_header;

/* RTP header */
typedef struct rtp_header
{
#if 0
    u_short version : 2;   /* protocol version */
    u_short p : 1;         /* padding flag */
    u_short x : 1;         /* header extension flag */
    u_short cc : 4;        /* CSRC count */
    u_short m : 1;         /* marker bit */
    u_short pt : 7;        /* payload type */
#else
    u_short cc : 4;        /* CSRC count */
    u_short x : 1;         /* header extension flag */
    u_short p : 1;         /* padding flag */
    u_short version : 2;   /* protocol version */
    u_short pt : 7;        /* payload type */
    u_short m : 1;         /* marker bit */
#endif

    u_short seq;      /* sequence number */
    u_int ts;               /* timestamp */
    u_int ssrc;             /* synchronization source */
}rtp_header;

Poco::Net::DatagramSocket m_ss;

void dispatcher_handler(u_char *temp1,const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    u_int i = 0;
    struct tm *ltime;
    char timestr[16];
    ip_header *ih;
    udp_header *uh;
    rtp_header *rh;
    u_int ip_len;
    u_short sport, dport;
    time_t local_tv_sec;
    
    int L2Count = 0;
    for(; L2Count < 16 ; L2Count++)
    {
        if(pkt_data[L2Count] == 0x08 && pkt_data[L2Count + 1] == 0x00)
        {
            break;
        }
    }

    /* retireve the position of the ip header */
    //ih = (ip_header *)(pkt_data + 14); //length of ethernet header 14Byte of L2
    ih = (ip_header *)(pkt_data + (L2Count + 2)); //length of ethernet header 14Byte of L2

    //printf("proto : %d\n",ih->proto);
    if (ih->proto == UDP)
    {
        static int sendCount = 0;
        //if(++sendCount > 10)
        {
            //usleep(1000);
            sendCount = 0;
        }

        /* retireve the position of the udp header */
        ip_len = (ih->ver_ihl & 0xf) * 4;
        uh = (udp_header *)((u_char*)ih + ip_len);

        int ret = 0;
        char *rtp = ((char*)ih + ip_len + 8);
        int rtp_len = ntohs(uh->len) - 8;
        rh = (rtp_header *)((u_char*)ih + ip_len + 8);
        //  视频
        {
            if (ntohs(uh->sport) == 4130 && ntohs(uh->dport) == 5002)   // DANBIN
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45250));
            if (ntohs(uh->sport) == 10014 && ntohs(uh->dport) == 5002)  // FANGWEI
            //if (ntohs(uh->sport) == 10198 && ntohs(uh->dport) == 45252)  // FANGWEI u-law
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45252));
            //if (ntohs(uh->sport) == 11782 && ntohs(uh->dport) == 5002)  // YILIAN
            if (ntohs(uh->sport) == 11788 && ntohs(uh->dport) == 45254)  // YILIAN
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45254));
            if (ntohs(uh->sport) == 8216 && ntohs(uh->dport) == 55456)  // RTSP
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45256));
            if (ntohs(uh->sport) == 15060 && ntohs(uh->dport) == 31000)  // PS
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45258));
            if (ntohs(uh->sport) == 5006 && ntohs(uh->dport) == 45252)  // CHAOLIU
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45260));
            if (ntohs(uh->sport) == 50170 && ntohs(uh->dport) == 45252)  // XINGWANG
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45262));
			//printf("video seq = %d, udp_len=%d \n", ntohs(rh->seq), rtp_len);
        }

        // 音频
        {                
            if (ntohs(uh->sport) == 13274 && ntohs(uh->dport) == 5000)  // DANBIN
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45251));
            if (ntohs(uh->sport) == 10012 && ntohs(uh->dport) == 5000)  // FANGWEI
            //if (ntohs(uh->sport) == 10196 && ntohs(uh->dport) == 45253)  // FANGWEI u-law
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45253));
            //if (ntohs(uh->sport) == 11780 && ntohs(uh->dport) == 5000)  // YILIAN
            if (ntohs(uh->sport) == 11786 && ntohs(uh->dport) == 45255)  // YILIAN
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45255));
            if (ntohs(uh->sport) == 5004 && ntohs(uh->dport) == 45253)  // CHAOLIU
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45261));
            if (ntohs(uh->sport) == 50168 && ntohs(uh->dport) == 45253)  // XINGWANG
                m_ss.sendTo(rtp, rtp_len, Poco::Net::SocketAddress("localhost", 45263));
			//printf("audio seq = %d, udp_len=%d \n", ntohs(rh->seq), rtp_len);
		}
    }
    return;
}

int main(int argc, char* argv[])
{
    const int fileCount = 7;
	pcap_t *fp[fileCount];
    char* fileName[] = {"capFile/danbin.pcap", "capFile/fangwei.pcap", "capFile/yilian_1.pcap", \
                        "capFile/rtsp.pcap", "capFile/ps.pcap", "capFile/chaoliu.pcap", \
                        "capFile/xingwang.pcap"};
	char errbuf[PCAP_ERRBUF_SIZE];
    
    for(int i = 0; i < fileCount; i++ )
    {
        if ((fp[i] = pcap_open_offline(fileName[i], errbuf)) == NULL)
        {
            cout << "Unable to open the file : " << fileName[i] << endl;
            return -1;
        }
    }
    
    for(int i = 0; i < fileCount; i++ )
    {
        if(i == (1 - 1))
        {         
            cout << "send : " << fileName[i] << endl;
            // read and dispatch packets until EOF is reached 
            pcap_loop(fp[i], 0, &dispatcher_handler, NULL);
        }
        pcap_close(fp[i]);
        
    }

    cout << "finish . " << endl;
    return 0;
}
