// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------


#ifndef MYWINSOCKBASE_H
#define MYWINSOCKBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MY_WINSOCK_VERSION 0x202		// 2.2

#include <winsock2.h>

typedef void (*WsProtocolCore)(BYTE,LPBYTE, int);

class MyWinsockBase
{
private:

	BOOL Startup();

public:

	BOOL CreateSocket(HWND hWnd);
	BOOL Close(SOCKET socket);
	BOOL Close();
	BOOL Close2(SOCKET socket);
	SOCKET GetSocket();
	int GetSendBuffSize();
	int GetRecvBuffSize();
	BOOL GetConnect();
	void SetConnect(BOOL connected);

	MyWinsockBase();
	~MyWinsockBase();

protected:

	int m_iMaxSockets;	// 0
	HWND m_hWnd;	// 4
	SOCKET m_socket;	// 8
	int m_sendbufsize;	// C
	int m_recvbufsize;	// 10
	BOOL m_Connect;	// 14
};



#endif