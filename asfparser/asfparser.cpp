#include "AsfParser.h"
#include <assert.h>

AsfParser::AsfParser(const std::string& name) 
: fileName(name)
{
	reader = NULL;
}

AsfParser::~AsfParser()
{
	if(reader != NULL)
    {
		delete reader;
    }
}

bool AsfParser::Initialize()
{
    // Asf Reader
    if(reader == NULL)
    {
        reader = new AsfReader(fileName);
    }
    
    assert(reader != NULL);
    if(!reader->Initialize())
    {
        return false;
    }
    
    return true;
}

uint64_t AsfParser::GetPacketCount()
{
    if(reader == NULL)
    {
        return 0;
    }
    
    return reader->HeadObject()->PacketCount;
}

bool AsfParser::LocatePacket(uint64_t index)
{
    if(reader == NULL)
    {
        return false;
    }
    
    return reader->Locate(index);
}

RtpPacket* AsfParser::NextRtpPacket()
{
    if(reader == NULL)
    {
        return NULL;
    }
    
    AsfPacket* packet = reader->NextPacket();
    if(packet == NULL)
    {
        return NULL;
    }
    
    return MakeRtpPacket(packet);
}

RtpPacket* AsfParser::MakeRtpPacket(AsfPacket* packet) 
{
	assert(packet != NULL);
    
	if( packet->PadLen == 0 ) 
    {
		uint16_t rtpSize = RtpPacket::RTP_HEAD_LEN + 4 + packet->PacketLen; 
		RtpPacket* rtpPacket = new RtpPacket( rtpSize );
		uint8_t* p = rtpPacket->payload;
        
		*(uint16_t*) p = htons(0x4000);
		p += 2;
        
		*(uint16_t*) p = htons(rtpPacket->payloadSize);
		p += 2;
        
		memcpy( p, packet->Buf, packet->PacketLen );
		assert ( packet->PacketLen + 4 == rtpPacket->payloadSize );
		rtpPacket->SetTimeStamp( packet->Time );
		return rtpPacket;
	}
	else 
    {
		uint16_t rtpSize = RtpPacket::RTP_HEAD_LEN + 4 + 7 + 6 + packet->PayloadSize ; 
		RtpPacket* rtpPacket = new RtpPacket( rtpSize );
		uint8_t* buf = new uint8_t [ rtpPacket->payloadSize ];
		uint8_t* p = buf;
        
		*(uint16_t*) p = htons(0x4000);
		p += 2;
        
		*(uint16_t*) p = htons(rtpPacket->payloadSize);
		p += 2;
        
		(*p) = packet->ErrorFlags;
		p += 1;
		for(int i = 0; i < packet->ErrorDataLen; i++) 
        {
			(*p) = packet->ErrorData[i];
			p += 1;
		}
        
		(*p) = 0x40;
		if( (packet->LenFlags&0x01) != 0 ) 
        {
			(*p) |= 0x01;
		}
		p += 1;
        
		(*p) = packet->PropFlags;
		p += 1;
        
		*(uint16_t*)p =  rtpPacket->payloadSize - 4 ;
		p += 2;
        
		*(uint16_t*) p = packet->Time;
		p += 4;
        
		*(uint16_t*) p = packet->Duration;
		p += 2;
        
		memcpy( p, packet->Payload, packet->PayloadSize );
		p += packet->PayloadSize;
		
		assert( p - buf == rtpPacket->payloadSize );
        
        
		rtpPacket->SetPayLoad( buf, rtpPacket->payloadSize );
		rtpPacket->SetTimeStamp( packet->Time );
		delete buf;
		return rtpPacket;
	}
}
