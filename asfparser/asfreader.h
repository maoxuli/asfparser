#ifndef	__ASFREADER_H__
#define	__ASFREADER_H__

#include "asfobject.h"
#include "asfpacket.h"
#include <string>
#include <fstream>

class AsfReader
{
public:
	AsfReader(const std::string& name);
	~AsfReader();
    
    bool Initialize();
    
    // Head objects
    AsfHeadObject* HeadObject();
    
    // Locate a data packet
    bool Locate(uint64_t index);
    
    // Loop to read data packets
    AsfPacket* NextPacket();
    
private:
    std::ifstream fp;
    std::string fileName;
    
    AsfHeadObject* HeadObj;
    uint64_t PacketOffset;
    
	uint64_t PacketCount;
};

#endif//__ASFREADER_H__
