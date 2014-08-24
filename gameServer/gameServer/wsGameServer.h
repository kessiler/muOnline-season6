// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef WSGAMESERVER_H
#define WSGAMESERVER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\MyWinsockBase.h"
#include "IpCache.h"

class CwsGameServer : public MyWinsockBase
{

public:

	BOOL SetSocketIndex(SOCKET socket, int aIndex);
	BOOL SetSocketBuffer(int index, SOCKET socket, LPSTR ip);
	BOOL AcceptSocket(SOCKET& clientSocket, in_addr& cInAddr);
	BOOL DataRecv(SOCKET socket);
	BOOL FDWRITE_MsgDataSend(SOCKET socket);
	BOOL DataSendW(int uindex, PCHAR buf, int len);
	BOOL DataSocketSend(SOCKET socket, PCHAR buf, int len);
	BOOL CreateServer(LPSTR ip_addr, WORD port, DWORD WinServerMsg, DWORD WinClientMsg);

	CwsGameServer();
	virtual ~CwsGameServer();

public:

	int m_SendSec;	// 1C
	int m_SendSecTmp;	// 20
	int m_RecvSec;	// 24
	int m_RecvSecTmp;	// 28
	DWORD m_WinClientMsg;	// 2C
};


#endif