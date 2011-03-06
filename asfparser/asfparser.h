#ifndef __ASFPARSER_H__
#define	__ASFPARSER_H__

#include "asfreader.h"
#include "rtppacket.h"
#include <string>

class AsfParser
{	
public:
	AsfParser(const std::string& name);
	virtual ~AsfParser();
	
    bool        Initialize();
	
    uint64_t    GetPacketCount();
    
    bool        LocatePacket(uint64_t index);
    RtpPacket*  NextRtpPacket();
	
private:
    std::string fileName;
	AsfReader*  reader;
    
    RtpPacket* MakeRtpPacket(AsfPacket* packet);
};

#endif//__ASFMEDIAPARSER_H__
