#include "asfpacket.h"
#include <memory.h>
#include <assert.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

AsfPacket::AsfPacket(const uint8_t* buf, uint32_t len)
{
	Buf = buf;
	Len = len;
    
    ErrorFlags = 0;
	
    ErrorDataLen = 0;
    ErrorData = NULL;
	
    LenFlags = 0;
    PropFlags = 0;
	
    PacketLen = 0;
    Sequence = 0;
    PadLen = 0;
    Time = 0;
    Duration = 0;
    
    PayloadSize = 0;
    Payload = NULL;
    
	HasKeyFrame = false ;
	HasMultiple = false;
}

AsfPacket::~AsfPacket(void)
{
    
}

// | ErrorFlags | ErrorData | LenFlags | PropFlags | Length | TimeStamp | Duration | Payload |
// |     1      |   EDLen   |    1     |     1     |  1,2,4 |     4     |     2    |         |

bool AsfPacket::Initialize()
{
    const uint8_t* pBuf;
    pBuf = Buf;

    ErrorFlags = *pBuf;
    pBuf += 1;

    ErrorDataLen = ErrorFlags&0x0f;
    bool isOpaque = ErrorFlags&0x10 ? true : false;
    bool hasErrorData = ErrorFlags&0x80 ? true : false;
    uint16_t errorLenType = ErrorFlags&0x60 ;   //B 0110 0000

    if( (ErrorFlags&0xf0) != (uint8_t)0x80 )   // assert that it has error data and no Opaque data
    {
        return false;
    }

    ErrorData = pBuf;
    pBuf += ErrorDataLen;

    LenFlags = *pBuf;
    pBuf += 1;

    PropFlags  = *pBuf;
    pBuf += 1;

    int a,b;
    a = (LenFlags&0x02)>>1;
    b = (LenFlags&0x04)>>2;
    int seqLen = b*2 + a + a*b;

    a = (LenFlags&0x08)>>3;
    b = (LenFlags&0x10)>>4;
    int padLen = b*2 + a + a*b;

    a = (LenFlags&0x20)>>5;
    b = (LenFlags&0x40)>>6;
    int packetLen = b*2 + a + a*b;

    int lenOffset = 1 + ErrorDataLen + 2;
    switch ( packetLen ) {
            case 0 : PacketLen = 0; break;
            case 1 : PacketLen = *pBuf; pBuf += 1; break;
            case 2 : PacketLen = *(uint16_t*) (pBuf); pBuf += 2;  break;
            case 4 : PacketLen = *(uint32_t*) (pBuf); pBuf += 4; break;
    }
    switch ( seqLen ) {
            case 0 : Sequence = 0; break;
            case 1 : Sequence = *pBuf; pBuf += 1; break;
            case 2 : Sequence = *(uint16_t*) (pBuf); pBuf += 2;  break;
            case 4 : Sequence = *(uint32_t*) (pBuf); pBuf += 4; break;
    }
    switch ( padLen ) {
            case 0 : PadLen = 0; break;
            case 1 : PadLen =  *pBuf; pBuf += 1; break;
            case 2 : PadLen = *(uint16_t*) (pBuf); pBuf += 2;  break;
            case 4 : PadLen = *(uint32_t *) (pBuf); pBuf += 4; break;
    }

    Time = *(uint32_t*)(pBuf );
    pBuf += 4;
    
    Duration =*(uint16_t*)(pBuf );
    pBuf += 2;
    
    Payload = pBuf;
    PayloadSize = Len - (pBuf - Buf) - PadLen; // not include  the pad length;

	ParserPayload();

    return true;
}

AsfPacket* AsfPacket::CreatePacket(const uint8_t* buf, uint32_t size)
{
	AsfPacket* pack = new AsfPacket(buf, size);
    
	if( !pack->Initialize() )
	{
		delete pack;
		return NULL;
	} 
    
    return pack;
}

typedef struct _LengthTypeFlags
{
	unsigned MultiPayloadPresent : 1;
	unsigned SequenceType		 : 2;
	unsigned PadLenType			 : 2;
	unsigned PackLenType		 : 2;
	unsigned ErrorPresent		 : 1;
} LenTypeFlags;

bool AsfPacket::ParserPayload()
{
	LenTypeFlags* pLen = (LenTypeFlags*)&(this->LenFlags);
	if(pLen->MultiPayloadPresent)
	{
		 return ParserMultiplePayload();
	} 
    else 
	{
		return ParserSinglePayload();
	}
}

bool AsfPacket::ParserSinglePayload()
{
	uint8_t streamNumber = this->Payload[0];
	if( streamNumber & 0x80 )
	{
		HasKeyFrame = true;
	} 
	else 
	{
		HasKeyFrame = false;
	}
    
	return true;
}

#pragma pack(push, 1)

typedef struct _PayloadFlags
{
	unsigned char NumOfPayload : 6;
	unsigned char PayloadLengthType : 2;
} PayloadFlags;

typedef struct _SinglePayload
{
	uint8_t StreamNumber;
	uint8_t	MediaObjectNumber;
	uint32_t OffsetIntoMediaObject;
	uint8_t	ReplicatedDataLength;
} SinglePayload;

#pragma pack(pop)

bool AsfPacket::ParserMultiplePayload()
{
	assert(this->PropFlags == 0x5D);

	const uint8_t* pBuf = this->Payload;
	PayloadFlags* pf = (PayloadFlags*) pBuf;
	pBuf += sizeof(PayloadFlags);
	assert(pf->PayloadLengthType == 0x2);

	for(int i = 0; i < pf->NumOfPayload; i++)
	{
		SinglePayload* pSp = (SinglePayload*) pBuf;
		pBuf += sizeof(SinglePayload);
		if( pSp->StreamNumber & 0x80 )
		{
			this->HasKeyFrame = true;
			break;
		}

		pBuf += pSp->ReplicatedDataLength;

		uint16_t payLoadLength = *(uint16_t*)pBuf;
		pBuf += sizeof(uint16_t);

		pBuf += payLoadLength;
	}
    
	if(!HasKeyFrame)
	{
		if(pBuf - Payload != this->PayloadSize)
		{
			return false;
		}
	}
    
	return true;
}
