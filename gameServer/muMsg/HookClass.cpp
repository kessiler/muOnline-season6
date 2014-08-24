// HookClass.cpp: implementation of the HookClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HookClass.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HookClass g_Hook;
#define MAIN_recv	0x00870614

TCHAR szFormatedPacket[65536];
CRITICAL_SECTION criti;

int FormatPacket(LPBYTE lpBuffer, int nBufferSize, LPTSTR pBufferOut, int nMaxLen)
{
	for(int i=0;i<nBufferSize; i++)
	{
		if ( int(strlen(pBufferOut)+3) > nMaxLen )
			break;

		pBufferOut[i*3+0] = ((lpBuffer[i] >> 8) >= 0x0A )? 'A' + (lpBuffer[i] >> 8) - 0x0A : '0' + (lpBuffer[i] >> 8);
		pBufferOut[i*3+1] = ((lpBuffer[i] & 0x0F) >= 0x0A )? 'A' + (lpBuffer[i] & 0x0F) - 0x0A : '0' + (lpBuffer[i] & 0x0F);
		pBufferOut[i*3+2] = ' ';
	}
	pBufferOut[i*3+0]='\0';
	return strlen(pBufferOut);
}





int __stdcall Hook_recv(SOCKET s, char *buf, int len, int flags)
{
	EnterCriticalSection(&criti);
	int iStringLen = FormatPacket((LPBYTE)buf, len, szFormatedPacket, sizeof(szFormatedPacket));
	FILE * fp = fopen("C:\\Program Files\\Webzen\\Mu\\recv_Log.txt", "a+t");
	fputs(szFormatedPacket, fp);
	fclose(fp);
	LeaveCriticalSection(&criti);

	return recv(s, buf, len, flags);
}






int 









HookClass::HookClass()
{
	InitializeCriticalSection(&criti);

	__try
	{
		*(DWORD *)(MAIN_recv) = (DWORD)Hook_recv;
		MessageBox(NULL, "recv Hooked", 0, 0);
	}
	__except(1)
	{
		MessageBox(NULL, "Can't Hook", 0, 0);
	}
}

HookClass::~HookClass()
{
	DeleteCriticalSection(&criti);
}
