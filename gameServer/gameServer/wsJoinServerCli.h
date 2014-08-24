// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef WSJOINSERVERCLI_H
#define WSJOINSERVERCLI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\MyWinsockBase.h"


#define MAX_WS_BUF_LEN	819200


class wsJoinServerCli : public MyWinsockBase
{

public:

	BOOL SetProtocolCore(WsProtocolCore pc);
	BOOL FDWRITE_MsgDataSend();
	BOOL DataRecv();
	BOOL DataSend(PCHAR buf, int len);
	BOOL Connect(LPSTR ip_addr, WORD port, DWORD WinMsgNum);

	wsJoinServerCli();
	virtual ~wsJoinServerCli();

protected:
	
	BYTE m_SendBuf[MAX_WS_BUF_LEN];	// 1C
	int m_SendBufLen;	// C801C
	BYTE m_RecvBuf[MAX_WS_BUF_LEN];	// C8020
	int  m_RecvBufLen;	// 190020
	WsProtocolCore wsProtocolCore;	// 190024
};

#endif
