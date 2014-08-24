#ifndef TNOTICE_H
#define TNOTICE_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "protocol.h"


__inline DWORD _ARGB(BYTE a, BYTE r, BYTE g, BYTE b){ return a*16777216 + b*65536 + g*256 + r;}; // in tnotice.h : line 8

struct PMSG_NOTICE
{
	PBMSG_HEAD h;
	BYTE type;	// 3
	BYTE btCount;	// 4
	WORD wDelay;	// 6	
	int dwColor;	// 8
	BYTE btSpeed;	// C
	char Notice[256];	// D
	
};


class TNotice
{
public:

	static void MakeNoticeMsg(void * lpNotice, BYTE btType, LPSTR szNoticeMsg);
	static void MakeNoticeMsgEx(void * lpNotice, BYTE btType, LPSTR szNoticeMsg, ...);
	static void SetNoticeProperty(void * lpNotice, BYTE btType, DWORD dwColor, BYTE btCount, WORD wDelay, BYTE btSpeed);
	static void SendNoticeToAllUser(void * lpNotice);
	static void SendNoticeToUser(int aIndex, void * lpNotice);
	static void AllSendServerMsg(LPSTR chatmsg);
	static void GCServerMsgStringSend(LPSTR szMsg, int aIndex, BYTE type);

	TNotice(BYTE btType);

	void __cdecl SendToUser(int aIndex, LPSTR szMsg, ...);
	void __cdecl SendToAllUser(LPSTR szMsg, ...);

#ifdef GS_UNICODE
	static void MakeNoticeMsgUnicode(void * lpNotice, BYTE btType, LPSTR szNoticeMsg, int iSize);
#endif

	// Dont have prefix
	//void __thiscall SetSpeed(unsigned char);
	//void __thiscall SetColor(unsigned long);
	//void __thiscall SetCount(unsigned char);
	//void __thiscall SetDelay(unsigned short);
	//void __thiscall SetType(unsigned char);

	PMSG_NOTICE m_Notice;
};



#endif