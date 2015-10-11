#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include "Poco/Mutex.h"

class RTPHandler;
class RTPDump;
class RTPSession
{
public:
    typedef struct buffer_node
    {
        void* buffer;
        int   len;
    }buffer_node;
public:
    RTPSession()
    {
        if(++SessionCounter < 0)
            SessionCounter = 0;
        SessionID = SessionCounter;
    }
    int         SessionID;          // session id
    int         videoFrequency;     // default is 90000
    int         listenPort;         // listen ip is 0.0.0.0
    std::string fileName;           // record rtp stream file name

    RTPDump*                    rtpDump;
    RTPHandler*                 handler;
    Poco::FastMutex             bufferMutex;
    std::vector<buffer_node>    bufferList;
private:
    static int                  SessionCounter;
};

class session_finder 
{ 
public: 
    session_finder(const int id):m_sessionID(id){} 
    bool operator ()(const std::vector<RTPSession*>::value_type &value)
    {
        return value->SessionID == m_sessionID;
    }
private: 
    int m_sessionID;                     
}; 

#endif
