#ifndef	__RTPPACKET_H__
#define	__RTPPACKET_H__

#include <stdint.h>

class RtpPacket
{
public:
    /*
     *  RTP Header
     *	.. | PayLoad Type | Seq Num | TimeStamp | ..
     *  .. | 8 Bit        | 16 Bit  | 32 Bit    | ..
     */
    enum
    {
        RTP_VERSION_OFFSET	    =	0,
        RTP_PAYLOADTYPE_OFFSET	=	1,
        RTP_SEQ_NUM_OFFSET		=	2,
        RTP_TIME_STAMP_OFFSET	=	4,
        RTP_SSRC_OFFSET			=	8,

        RTP_HEAD_LEN			=	12,
        RTP_PAYLOAD_OFFSET      =   12,

        RTP_VERSION_CONST	    =	0x80,	
        RTP_PAYLOADTYPE_CONST   =	0xe0,
        RTP_SSRC_CONST			=	3202321672
    };

    uint8_t*  buffer;
    uint16_t  bufferSize;

    uint8_t*  payload;
    uint16_t  payloadSize;

    uint16_t  trackID;
    bool	  isKeyFrame;	

public:
    RtpPacket(uint16_t size);
    ~RtpPacket(void);
    
    bool SetFirstFlag();
    bool SetPayLoad(const uint8_t* buf,uint32_t size);

	uint8_t	GetVersion();
	bool	SetVersion( uint8_t );
    
	uint8_t	GetPayLoadType();
	bool	SetPayLoadType( uint8_t );
    
	uint16_t GetSeqNum();
	bool	 SetSeqNum( uint16_t );
    
	uint32_t GetTimeStamp();
	bool	 SetTimeStamp( uint32_t );
    
	uint32_t GetSSRC() ;
	bool     SetSSRC( uint32_t );
};

#endif
