//	GS-CS	1.00.90	JPN	-	Completed
#include "stdafx.h"
#include "TNotice.h"
#include "user.h"
#include "giocp.h"
#include "..\common\winutil.h"

void TNotice::MakeNoticeMsg(void * lpNotice, BYTE btType, char * szNoticeMsg)
{
	PMSG_NOTICE * pNotice = (PMSG_NOTICE *)lpNotice;
	pNotice->type  = btType;

#ifdef GS_UNICODE
	unsigned char szTempMsg[4096]={0};
	memset( pNotice->szNoticeMessage, 0, sizeof(pNotice->szNoticeMessage));
	MultiByteToWideChar( 1258, 0, (char*)szNoticeMsg,  strlen((char*)szNoticeMsg), (unsigned short*)szTempMsg, sizeof(szTempMsg) );
	memcpy(pNotice->szNoticeMessage, szTempMsg, sizeof(pNotice->szNoticeMessage));
	pNotice->szNoticeMessage[254]=0;
	pNotice->szNoticeMessage[255]=0;
	wsprintf(pNotice->Notice, szNoticeMsg);
	PHeadSetB(&pNotice->PacketHeader, 0x0D, wcslen((unsigned short *)pNotice->szNoticeMessage)*2+ 0x12);
#else
	wsprintf(pNotice->Notice, szNoticeMsg);
	PHeadSetB((LPBYTE)pNotice, 0x0D, strlen(pNotice->Notice) + sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + 1 );
#endif
}


void TNotice::MakeNoticeMsgEx(void * lpNotice, BYTE btType, char * szNoticeMsg, ...)
{
	PMSG_NOTICE * pNotice = (PMSG_NOTICE *)lpNotice;
	pNotice->type = btType;

#ifdef GS_UNICODE
	unsigned char szTempMsg[4096]={0};
#endif


	va_list pArguments;

#ifdef GS_UNICODE
	memset( pNotice->szNoticeMessage, 0, sizeof(pNotice->szNoticeMessage));
#endif


	va_start(pArguments, szNoticeMsg);

#ifdef GS_UNICODE
	vsprintf((char*)szTempMsg, (char*)szNoticeMsg, pArguments);
#else
	vsprintf((char*)pNotice->Notice, (char*)szNoticeMsg, pArguments);
#endif

	va_end(pArguments);


#ifdef GS_UNICODE
	MultiByteToWideChar(1258, 0, (char*)szTempMsg,  strlen((char*)szTempMsg), (unsigned short*)pNotice->szNoticeMessage, sizeof(pNotice->szNoticeMessage) );
	pNotice->szNoticeMessage[254]=0;
	pNotice->szNoticeMessage[255]=0;
	PHeadSetB(&pNotice->PacketHeader, 0x0D, wcslen((unsigned short *)pNotice->szNoticeMessage)*2+ 0x12);
#else
	PHeadSetB((LPBYTE)pNotice, 0x0D, strlen(pNotice->Notice)  + sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + 1);
#endif

}




#ifdef GS_UNICODE
void TNotice::MakeNoticeMsgUnicode(void * lpNotice, BYTE btType, unsigned char * szNoticeMsg, int iSize)
{
	NOTICE * pNotice = (NOTICE *)lpNotice;

	pNotice->btNoticeType = btType;
	
	if ( iSize > sizeof(pNotice->szNoticeMessage ) )
	{
		iSize = sizeof( pNotice->szNoticeMessage );
	}

	PHeadSetB( &pNotice->PacketHeader, 0x0D, iSize + 16 );
	memcpy( (char*)pNotice->szNoticeMessage, szNoticeMsg, iSize);
	szNoticeMsg[iSize-2]=0;
	szNoticeMsg[iSize-1]=0;
}
#endif




void TNotice::SetNoticeProperty(void * lpNotice, BYTE btType, DWORD dwColor, BYTE btCount, WORD wDelay, BYTE btSpeed)
{
	PMSG_NOTICE * pNotice =(PMSG_NOTICE *) lpNotice;

	pNotice->type = btType;
	pNotice->dwColor = dwColor;
	pNotice->btCount = btCount;
	pNotice->wDelay = wDelay;
	pNotice->btSpeed = btSpeed;
}

void TNotice::SendNoticeToAllUser(void * lpNotice)
{
	PMSG_NOTICE * pNotice = (PMSG_NOTICE *)lpNotice;

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				DataSend(n, (unsigned char*)pNotice  , pNotice->h.size  );
			}
		}
	}
}

void TNotice::SendNoticeToUser(int aIndex, void * lpNotice)
{
	PMSG_NOTICE * pNotice = (PMSG_NOTICE *)lpNotice;
	DataSend(aIndex, (unsigned char*)pNotice, pNotice->h.size  );
}



void TNotice::AllSendServerMsg(LPSTR chatmsg)
{
	PMSG_NOTICE pNotice;
	
	MakeNoticeMsg((TNotice *)&pNotice, 0,  chatmsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				DataSend(n, (unsigned char*)&pNotice , pNotice.h.size );
			}
		}
	}
}

void TNotice::GCServerMsgStringSend(LPSTR szMsg, int aIndex, BYTE type)
{
	PMSG_NOTICE pNotice;
	
	MakeNoticeMsg(&pNotice, type, szMsg);
	DataSend(aIndex, (unsigned char*)&pNotice, pNotice.h.size);
}



TNotice::TNotice(BYTE btType)
{
	this->m_Notice.btCount = 1;
	this->m_Notice.btSpeed = 20;
	this->m_Notice.dwColor = _ARGB(255, 255, 200, 80 );
	this->m_Notice.wDelay = 0;
	this->m_Notice.type = btType;
}







void TNotice::SendToAllUser(LPSTR szMsg, ...)
{
#ifdef GS_UNICODE
	unsigned char szTempMsg[4096]={0};
#endif

	va_list pArguments;

#ifdef GS_UNICODE
	memset(this->m_Notice.Notice , 0, sizeof(this->m_Notice.Notice ));
#endif


	va_start(pArguments, szMsg);

#ifdef GS_UNICODE
	vsprintf((char*)szTempMsg, (char*)szMsg, pArguments);
#else
	vsprintf(this->m_Notice.Notice, (char*)szMsg, pArguments);
#endif

	va_end(pArguments);

#ifdef GS_UNICODE
	MultiByteToWideChar( 1258, 0, (char*)szTempMsg,  strlen((char*)szTempMsg), (unsigned short*)this->m_Notice.Notice, sizeof(this->m_Notice.Notice) );
	this->m_Notice.Notice[254] = 0;
	this->m_Notice.Notice[255] =0;
	PHeadSetB( &this->m_pNotice.PacketHeader, 0x0D, wcslen((unsigned short*)this->m_Notice.Notice )*2 + 0x12);
#else
	PHeadSetB((LPBYTE)&this->m_Notice, 0x0D, strlen(this->m_Notice.Notice)  + sizeof(PMSG_NOTICE) - sizeof(this->m_Notice.Notice) + 1);
#endif

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				DataSend(n, (LPBYTE)&this->m_Notice , this->m_Notice.h.size );
			}
		}
	}
}

void TNotice::SendToUser(int aIndex, LPSTR szMsg, ...)
{
#ifdef GS_UNICODE
	unsigned char szTempMsg[4096]={0};
#endif

	va_list pArguments;

#ifdef GS_UNICODE
	memset(this->m_pNotice.szNoticeMessage , 0, sizeof(this->m_pNotice.szNoticeMessage ));
#endif


	va_start(pArguments, szMsg);

#ifdef GS_UNICODE
	vsprintf((char*)szTempMsg, (char*)szMsg, pArguments);
#else
	vsprintf(this->m_Notice.Notice, (char*)szMsg, pArguments);
#endif

	va_end(pArguments);

#ifdef GS_UNICODE
	MultiByteToWideChar( 1258, 0, (char*)szTempMsg,  strlen((char*)szTempMsg), (unsigned short*)this->m_pNotice.szNoticeMessage, sizeof(this->m_pNotice.szNoticeMessage) );
	this->m_pNotice.szNoticeMessage[254]=0;
	this->m_pNotice.szNoticeMessage[255]=0;
	PHeadSetB( &this->m_pNotice.PacketHeader, 0x0D, wcslen((unsigned short*)this->m_pNotice.szNoticeMessage )*2+0x12);
#else
	PHeadSetB((LPBYTE)&this->m_Notice, 0x0D, strlen(this->m_Notice.Notice)  + sizeof(PMSG_NOTICE) - sizeof(this->m_Notice.Notice) + 1);
#endif

	DataSend(aIndex, (LPBYTE)&this->m_Notice , this->m_Notice.h.size );

}