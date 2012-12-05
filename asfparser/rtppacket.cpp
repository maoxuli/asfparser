#include "rtppacket.h"
#include <memory.h>
#include <assert.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

RtpPacket::RtpPacket(uint16_t size)
{
	assert( size > RTP_HEAD_LEN );
	buffer = new uint8_t[ size ];
	bufferSize = size;

	payload = buffer + RTP_HEAD_LEN;
	payloadSize = bufferSize - RTP_HEAD_LEN;
    
	SetVersion( RTP_VERSION_CONST);
	SetPayLoadType( RTP_PAYLOADTYPE_CONST);
	SetSSRC( RTP_SSRC_CONST);
}

RtpPacket::~RtpPacket(void)
{
	if( buffer != NULL ) 
    {
		delete[] buffer;
	}
}

bool RtpPacket::SetPayLoad(const uint8_t * buf, uint32_t size) 
{
	assert ( size == payloadSize );
	memcpy( payload, buf, size );
	return true;
}

uint8_t	RtpPacket::GetVersion() 
{
	return buffer[RTP_VERSION_OFFSET];
}

bool	RtpPacket::SetVersion(uint8_t version) 
{
	buffer[RTP_VERSION_OFFSET] = version;
	return true;
}

uint8_t RtpPacket::GetPayLoadType() 
{
	return buffer[RTP_PAYLOADTYPE_OFFSET];
}

bool RtpPacket::SetPayLoadType(uint8_t payLoadType) 
{
	buffer[RTP_PAYLOADTYPE_OFFSET ] = payLoadType;
	return true;
}

uint16_t RtpPacket::GetSeqNum() 
{
	return ntohs( *(uint16_t*)(buffer + RTP_SEQ_NUM_OFFSET) );
}

bool RtpPacket::SetSeqNum(uint16_t seqNum) 
{
	*(uint16_t*)(buffer + RTP_SEQ_NUM_OFFSET) = htons(seqNum);
	return true;
}

uint32_t RtpPacket::GetTimeStamp() 
{
	return ntohl( *( uint32_t* )(buffer + RTP_TIME_STAMP_OFFSET) );
}

bool RtpPacket::SetTimeStamp(uint32_t timeStamp) 
{
	*(uint32_t*)(buffer + RTP_TIME_STAMP_OFFSET) = htonl(timeStamp);
	return true;
}

uint32_t RtpPacket::GetSSRC()  
{
	return ntohl( *(uint32_t* )(buffer + RTP_SSRC_OFFSET) );
}

bool RtpPacket::SetSSRC(uint32_t ssrc) 
 {
	*(uint32_t*)(buffer + RTP_SSRC_OFFSET) = htonl(ssrc);
	return true;
}

bool RtpPacket::SetFirstFlag()
{
	payload[0] |= 0x80;
	return true;
}
