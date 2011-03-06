#include "asfreader.h"
#include <assert.h>

AsfReader::AsfReader(const std::string& name)
: fileName(name)
, HeadObj(NULL)
, PacketOffset(0)
, PacketCount(0)
{

}

AsfReader::~AsfReader()
{
	if( HeadObj != NULL )
    {
        delete HeadObj;
    }

	fp.clear();
	fp.close();
}

void printGUID(GUID id)
{
	printf("%0x,%0x,%0x,%0x,%0x,%0x,%0x,%0x,%0x,%0x,%0x\n", 
           id.Data1,id.Data2,id.Data3,id.Data4[0],id.Data4[1],id.Data4[2], 
           id.Data4[3],id.Data4[4],id.Data4[5],id.Data4[6],id.Data4[7]);
}

bool AsfReader::Initialize() 
{	
    // Open file
	fp.open( fileName.c_str(), std::ios::in|std::ios::binary );
	if ( !fp.is_open() )  
    {
        printf("Asf open file failed.\n");
        return false;
    }
    
    // Head Object
    OBJECT obj;
    fp.read( (char*)&obj, sizeof(obj) );
    assert(obj.ObjectID == HEAD_OBJECT_ID);
    if(obj.ObjectID != HEAD_OBJECT_ID) 
    {
        printf("Asf read HEAD_OBJECT failed.\n");
        printGUID(obj.ObjectID);
        printGUID(HEAD_OBJECT_ID);
        return false;
    }
    
    // Load AsfHeadObject
	uint8_t * buf;
	buf = new uint8_t[obj.ObjectSize];
	fp.seekg( 0, std::ios::beg );
	fp.read( (char*)buf, obj.ObjectSize );
    
	HeadObj = new AsfHeadObject( buf, obj.ObjectSize );
    assert(HeadObj != NULL);
	bool ret = HeadObj->Initialize();
	delete[] buf;
	if( !ret ) 
	{
        printf("Asf load head objects failed.\n");
        delete HeadObj;
        HeadObj = NULL;
        return false;
	}
    
    // Data object
    DATA_OBJECT dataObj;
    fp.read( (char*)&dataObj, sizeof( dataObj ) );
    assert( dataObj.ObjectID == DATA_OBJECT_ID );
    if(dataObj.ObjectID != DATA_OBJECT_ID)
    {
        printf("Asf read DATA_OBJECT failed.\n");
        printGUID(dataObj.ObjectID);
        printGUID(DATA_OBJECT_ID);
        return false;
    }
    
    // Offset of reading packets
    PacketOffset  = fp.tellg();
    
    return true;
}

AsfHeadObject* AsfReader::HeadObject()
{
    return HeadObj;
}

bool AsfReader::Locate(uint64_t index)
{
    assert(fp.is_open());
    assert(index >=0 && index < HeadObj->PacketCount);
    
	fp.seekg( PacketOffset + index * HeadObj->PacketSize,  std::ios::beg );
    PacketCount = index;
    
	return 0;
}

AsfPacket* AsfReader::NextPacket()
{
	if( PacketCount >= HeadObj->PacketCount)
	{
		return NULL;
	}
	
	uint8_t* buf = new uint8_t[HeadObj->PacketSize];
	fp.read( (char*)buf, HeadObj->PacketSize );
	size_t readLen = fp.gcount();
	if( readLen != HeadObj->PacketSize ) 
	{
        delete[] buf;
		return  NULL;
	} 
    
	
    PacketCount++;
	AsfPacket* packet = AsfPacket::CreatePacket( buf, HeadObj->PacketSize );
    assert(packet != NULL);
    
    delete[] buf;
    return packet;    
}
