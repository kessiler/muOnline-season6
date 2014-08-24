#include "StdAfx.h"
#include "MuMsg.h"

int nMsg=0;

int fnMsg(void)
{
	return 42;
}

CMsg::CMsg()
{
	memset(&this->szDefaultMsg  , 0, sizeof(this->szDefaultMsg ) );
	this->Msghead = NULL;
	strcpy(this->szDefaultMsg , "Msg error");
}

CMsg::~CMsg()
{
	this->lMsgFree();
}

void  CMsg::XorBuffer(char* buff, int len)
{
	if (len <=0)
	{
		return;
	}

	for(int iCounter=0;iCounter<len;iCounter++)
	{
		buff[iCounter] = buff[iCounter]^0xCA;
	}
}

void CMsg::DataFileLoadVer01(FILE* filename)
{
	unsigned char szTemp[256];
	short Index;
	unsigned short Len;

	int Max=this->LoadHeader.count;

	if (Max <= 0)
	{
		MessageBox(NULL, "읽는중 데이터의개수 에러 입니다."	, "Error", MB_OK|MB_APPLMODAL); // Modification error data while reading
		return;
	}

	while ( Max-- )
	{
		memset(&szTemp, 0, sizeof(szTemp) );

		fread(&Index , 2, 1, filename);
		fread(&Len , 2, 1, filename);
		fread(szTemp , Len , 1, filename);
		szTemp[Len]=0;

		this->XorBuffer((char*)szTemp , Len );
		this->lMsgListAdd( Index, (unsigned char*) szTemp);
	} 
}

void CMsg::LoadWTF(LPSTR filename)
{
	FILE* WTFFile;
	char szBufferError[128];
	
	WTFFile =fopen(filename, "rb");	//ok
	
	if (WTFFile == 0 )
	{
		wsprintf(szBufferError, "Could not open %s.", filename);	
		MessageBox(NULL, szBufferError, "Error", MB_OK|MB_APPLMODAL);
		return;
	}

	if (this->lMsgListInit() != 0)
	{
		fread(&this->LoadHeader, 28, 1, WTFFile);

		if (this->LoadHeader.headcode != 0xCC)
		{
			MessageBox(NULL, "TextCode Type Wrong.", "Error", MB_OK|MB_APPLMODAL);
		}
		else if ( (this->LoadHeader.version -1) != 0)
		{
			MessageBox(NULL, "지원하지 않는 버젼 데이터 입니다.", "Error", MB_OK|MB_APPLMODAL);
		}
		else
		{
			DataFileLoadVer01(WTFFile);
		}
		fclose(WTFFile);
	}
}

int CMsg::lMsgListInit()
{
	LPMSG_STRUCT msg = new MSG_STRUCT;

	if ( msg == NULL )
	{
		MessageBox(NULL, "Memory Allocation Error (MuMsg)", "Error", MB_OK);
		return 0;
	}
	msg->next  =NULL;
	msg->number  =0;
	msg->msg = NULL;
	memset(this->MsgIndex, 0, sizeof(this->MsgIndex));
	return 1;
}

void CMsg::lMsgFree()
{
	for ( int n = 0; n < 32768; n++ )
	{
		if ( this->MsgIndex[n] != 0 )
		{
			delete [] this->MsgIndex[n]->msg ;
			delete this->MsgIndex[n];
		}
	}
}

LPMSG_STRUCT  CMsg::lMsgListNew()
{
	return new MSG_STRUCT;
}

int CMsg::lMsgListAdd(int index, UCHAR* smsg)
{
	int MsgLen=strlen((char*)smsg);
	char* pPointer;
	
	if ( MsgLen > 0 )
	{
		this->MsgIndex[index]=this->lMsgListNew();
	
		if (this->MsgIndex[index] == 0)
		{
			MessageBox(NULL, "Memory Allocation Error #2(MuMsg)", "Error", MB_OK);
			return 0;
		}

		pPointer = new char[MsgLen+1];
		
		if ( pPointer  != 0 )
		{
			this->MsgIndex[index]->number=index;
		
			
			if ( index >= 0 || index < 32767 )
			{
				this->MsgIndex[index]->msg = (unsigned char*)pPointer;
				strcpy((char*)this->MsgIndex[index]->msg , (char*)smsg);
				return 1;
			}
			else
			{
				MessageBox(NULL, "Message Index Table Make Error", "Error", MB_OK);
				return 0;
			}

		}
	}


	return 1;
}

void CMsg::lMsgListPrint()
{
	for ( int n = 0; n<32768 ; n++)
	{
		if ( this->MsgIndex[n] != 0 )
		{
			MessageBox(NULL, (char*)this->MsgIndex[n]->msg, "Message", MB_OK);
		}
	}
}

LPSTR CMsg::Get(int index)
{
	if ( index >= 0 && index < 32768 )
	{
		if ( this->MsgIndex[index] == 0 )
		{
			return this->szDefaultMsg;
		}
			
		if ( this->MsgIndex[index]->msg == 0 )
		{
			return this->szDefaultMsg;
		}

		if ( *this->MsgIndex[index]->msg == 0 )
		{
			return this->szDefaultMsg;
		}

		return (char*)this->MsgIndex[index]->msg;
	}
	return (char*)this->szDefaultMsg;
		
}


