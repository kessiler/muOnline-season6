#include "stdafx.h"
//---------------------------------------------------------------------------

void WINAPI DebugPrint(const PCHAR fmt, ...)
{
	char szBuf[2048];
	int iBufLen	= sizeof(szBuf);
	ZeroMemory(szBuf, iBufLen);
	// ----
	va_list args;
	va_start(args, fmt);
	int iLen	= vsprintf_s(szBuf, iBufLen, fmt, args);
	va_end(args);
	// ----
	szBuf[iLen]	= '\n';
	iLen++;
	// ----
	OutputDebugStringA(szBuf);
}
//---------------------------------------------------------------------------


