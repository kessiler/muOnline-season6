// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef WHATSUPDUMMYSERVER_H
#define WHATSUPDUMMYSERVER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef long (__stdcall *WhatsUpDummyServerProc)(HWND, unsigned int, unsigned int, long);

#define WINSOCK_REQUESTED_VERSION 0x202




class CWhatsUpDummyServer
{

public:

	CWhatsUpDummyServer();
	~CWhatsUpDummyServer();
	int Start(HWND hWnd, WORD wPort);

	static LRESULT __cdecl ParentWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	static WhatsUpDummyServerProc m_lpOldProc;

private:

	SOCKET m_sckDUMMY;	// 0
	WORD m_wPORT;	// 4
	HWND m_hParentWnd;	// 8
};

//extern static WNDPROC CWhatsUpDummyServer::m_lpOldProc;


#endif