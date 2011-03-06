#ifndef	__ASFPACKET_H__
#define	__ASFPACKET_H__

#include <stdint.h>

class AsfPacket
{	
public:
    AsfPacket(const uint8_t* buf, uint32_t len);
    ~AsfPacket(void);
	
    bool Initialize();
    
	static AsfPacket* CreatePacket(const uint8_t* buf, uint32_t size);
	
private:
	bool ParserPayload();
	bool ParserSinglePayload();
	bool ParserMultiplePayload();
	
public:
    const uint8_t*  Buf;
    uint32_t        Len;
	
    uint8_t     ErrorFlags;
	uint8_t     ErrorDataLen;
    const uint8_t*  ErrorData;
    
    uint8_t     LenFlags; // Length of seq, pad, packet
    uint8_t     PropFlags;
	
    uint32_t	PacketLen;
    uint32_t	Sequence;
    uint32_t	PadLen;
    uint32_t	Time;
    uint32_t	Duration;
    
    uint32_t        PayloadSize;
    const uint8_t*  Payload;
    
	bool	HasKeyFrame;
	bool    HasMultiple;
};

#endif
