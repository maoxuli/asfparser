#ifndef	__ASFOBJECT_H__
#define	__ASFOBJECT_H__

#include <stdint.h>
#include <wchar.h>

// ASF object is identified with GUID

typedef struct {
	uint32_t    Data1;
	uint16_t	Data2;
	uint16_t	Data3;
	uint8_t     Data4[8];
} GUID;

inline bool operator==(const GUID& guidOne, const GUID& guidTwo)
{
	return (
			(guidOne.Data1 == guidTwo.Data1) &&
			(guidOne.Data2 == guidTwo.Data2) &&
			(guidOne.Data3 == guidTwo.Data3) &&
			(guidOne.Data4[0] == guidTwo.Data4[0]) &&
			(guidOne.Data4[1] == guidTwo.Data4[1]) &&
			(guidOne.Data4[2] == guidTwo.Data4[2]) &&
			(guidOne.Data4[3] == guidTwo.Data4[3]) &&
			(guidOne.Data4[4] == guidTwo.Data4[4]) &&
			(guidOne.Data4[5] == guidTwo.Data4[5]) &&
			(guidOne.Data4[6] == guidTwo.Data4[6]) &&
			(guidOne.Data4[7] == guidTwo.Data4[7]) );
}

inline bool operator!=(const GUID& guidOne, const GUID& guidTwo)
{
	return (
			(guidOne.Data1 != guidTwo.Data1) ||
			(guidOne.Data2 != guidTwo.Data2) ||
			(guidOne.Data3 != guidTwo.Data3) ||
			(guidOne.Data4[0] != guidTwo.Data4[0]) ||
			(guidOne.Data4[1] != guidTwo.Data4[1]) ||
			(guidOne.Data4[2] != guidTwo.Data4[2]) ||
			(guidOne.Data4[3] != guidTwo.Data4[3]) ||
			(guidOne.Data4[4] != guidTwo.Data4[4]) ||
			(guidOne.Data4[5] != guidTwo.Data4[5]) ||
			(guidOne.Data4[6] != guidTwo.Data4[6]) ||
			(guidOne.Data4[7] != guidTwo.Data4[7]) );
}

// GUIDs of ASF objects

const GUID HEAD_OBJECT_ID={
	0x75B22630, 0x668E, 0x11CF, {0xa6, 0xd9, 0x00, 0xAA, 0x00, 0x62, 0xCE ,0x6C}
};
const GUID EX_HEAD_OBJECT_ID={
	0x5FBF03B5, 0xA92E, 0x11CF, {0x8E, 0xE3, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65}
};
//5FBF03B5-A92E-11CF-8EE3-00C00C205365
const GUID DATA_OBJECT_ID={
	0x75B22636, 0x668E, 0x11CF, {0xa6, 0xd9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C}
};
const GUID FILE_PROPERTY_ID={
	0x8CABDCA1, 0xA947, 0x11CF, {0x8E, 0xE4, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65}
};
//8CABDCA1-A947-11CF-8EE4-00C00C205365
const GUID STREAM_PROPERTY_ID={
	0xB7DC0791, 0xA9B7, 0x11CF, {0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65}
};
//B7DC0791-A9B7-11CF-8EE6-00C00C205365
const GUID EX_STREAM_PROPERTY_ID={
	0x14E6A5CB, 0xC672, 0x4332, {0x83, 0x99, 0xA9, 0x69, 0x52, 0x06, 0x5B, 0x5A}
};
//14E6A5CB-C672-4332-8399-A96952065B5A
const GUID CONTENT_DESCRIPTION_ID={
	0x75B22633, 0x668E, 0x11CF, {0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C}
};
//75B22633-668E-11CF-A6D9-00AA0062CE6C
const GUID ASF_AUDIO_MEDIA={
	0xF8699E40, 0x5B4D, 0x11CF, {0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B}
};
//F8699E40-5B4D-11CF-A8FD-00805F5C442B
const GUID ASF_VIDEO_MEDIA={
	0xBC19EFC0, 0x5B4D, 0x11CF, {0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B}
};

// ASF head objects structures

#pragma pack(push, 1)	
	
typedef struct _object 
{
	GUID        ObjectID;
	uint64_t    ObjectSize;
} OBJECT;

typedef struct _headObject
{
	GUID        Object_ID;   
	uint64_t    Object_Size;   
	uint32_t    Number_of_Header_Objects; 
	uint8_t     Reserved1; 
	uint8_t     Reserved2; 	
} HEAD_OBJECT;

typedef struct _dataObject
{
	GUID        ObjectID;
	uint64_t	ObjectSize;
	GUID        FileID;
	uint64_t	TotalDataPackets;
	uint16_t	Reserved;
} DATA_OBJECT;

typedef struct _fileObject
{
	GUID		Object_ID;   
	uint64_t	Object_Size;   
	GUID		File_ID;   
	uint64_t	File_Size;   
	uint64_t	Creation_Date;   
	uint64_t	Data_Packets_Count;   
	uint64_t	Play_Duration;   
	uint64_t	Send_Duration;   
	uint64_t	Preroll;   
	uint32_t	Flags;   
	/*Broadcast Flag	1 (LSB)	      
	 Seekable Flag		1	              
	 Reserved			30*/
	uint32_t	Minimum_Data_Packet_Size; 
	uint32_t	Maximum_Data_Packet_Size; 
	uint32_t	Maximum_Bitrate; 
} FILE_OBJECT;

typedef struct _streamProperty
{
	GUID		Object_ID;   
	uint64_t	Object_Size;   
	GUID		Stream_Type; 
	GUID		Error_Correction_Type; 
	uint64_t	Time_Offset; 
	uint32_t	Type_Specific_Data_Length;
	uint32_t	Error_Correction_Data_Length;
	uint16_t	Flags; 
	/*Stream_Number				7 (LSB) 
	 Reserved					8                  
	 EncryptedContent Flag		1*/     
	uint32_t    Reserved;			 
	//BYTE      Type_Specific_Data[0];
	//BYTE      Error_Correction_Data[0];
} STREAM_PROPERTY;

typedef struct _contentDescription
{
	GUID        Object_ID;  
	uint64_t    Object_Size;  
	uint16_t    Title_Length;  
	uint16_t    Author_Length;  
	uint16_t    Copyright_Length;  
	uint16_t    Description_Length;  
	uint16_t    Rating_Length;  
	/*WCHAR     Title[0];  
	WCHAR*      Author;  
	WCHAR*      Copyright;  
	WCHAR*      Description;  
	WCHAR*      Rating;*/
} CONTENT_DESCRIPTION;

typedef struct _exHeadObject
{
	GUID        Object_ID;  
	uint64_t	Object_Size;  
	GUID		Reserved_Field_1;  
	uint16_t    Reserved_Field_2;  
	uint32_t    Header_Extension_Data_Size;  
} EX_HEAD_OBJECT;

typedef struct _exStreamProperty
{
	GUID        Object_ID;  
	uint64_t    Object_Size;  
	uint64_t    Start_Time;
	uint64_t    End_Time;         
	uint32_t    Data_Bitrate;         
	uint32_t    Buffer_Size;         
	uint32_t    Initial_Buffer_Fullness;         
	uint32_t    Alternate_Data_Bitrate;         
	uint32_t    Alternate_Buffer_Size;         
	uint32_t    Alternate_Initial_Buffer_Fullness;         
	uint32_t    Maximum_Object_Size;         
	uint32_t    Flags;         
	/*Reliable Flag               1 (LSB);  
	 Seekable Flag                1;  
	 No Cleanpoints Flag          1;  
	 Resend Live Cleanpoints Flag 1; 
	 Reserved Flags               28;  */
	uint16_t    Stream_Number;         
	uint16_t    Stream_Language_ID_Index;         
	uint64_t    Average_Time_Per_Frame;         
	uint16_t    Stream_Name_Count;         
	uint16_t    Payload_Extension_System_Count;         
	/*See below Stream Names;         
	 See below Payload Extension Systems;         
	 See below Stream Properties Object;  */       
} EX_STREAM_PROPERTY;

#pragma pack(pop)

// A wrapper of general ASF head informations

class AsfHeadObject
{
public:
	AsfHeadObject(const uint8_t* buf, uint16_t len);
	virtual ~AsfHeadObject(void);
    
	bool        Initialize();
	
    // From content description
	wchar_t*	Title;  
	wchar_t*    Author;  
	wchar_t*    Copyright;  
	wchar_t*    Description;  
	wchar_t*    Rating; 
	
    // From stream property and ex stream property
	uint16_t	AudioNum;
	uint16_t	VideoNum;
	uint32_t	AudioBitrate;
	uint32_t	VideoBitrate;
	
    // From file property
	uint32_t	Start_Time;
	uint32_t	End_Time;
	uint32_t	Duration;
	uint32_t    Bitrate;
	uint32_t	PacketSize;
	uint64_t	PacketCount;
	
private:
	const uint8_t*	Buf;
	uint16_t		Len;
	
	HEAD_OBJECT*        pHead;
	FILE_OBJECT*        pFile;
	STREAM_PROPERTY*	pAudioStream;
	STREAM_PROPERTY*	pVideoStream;
	CONTENT_DESCRIPTION* pContent;
	
	EX_HEAD_OBJECT *	pExHead;
	EX_STREAM_PROPERTY* pExAudioStream;
	EX_STREAM_PROPERTY* pExVideoStream;
};

#endif//__ASFOBJECT_H__
