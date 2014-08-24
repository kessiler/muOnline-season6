// wsShopServerCli.h: interface for the wsShopServerCli class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSSHOPSERVERCLI_H__3D41B7CB_3A46_4A07_A754_CC3FAAB222C5__INCLUDED_)
#define AFX_WSSHOPSERVERCLI_H__3D41B7CB_3A46_4A07_A754_CC3FAAB222C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\COMMON\MyWinsockBase.h"

typedef void(*ShopProtocolCore)(DWORD, LPBYTE, int);

class wsShopServerCli : public MyWinsockBase  
{

public:

	BOOL SetProtocolCore(ShopProtocolCore pc);
	BOOL FDWRITE_MsgDataSend();
	BOOL DataRecv();
	BOOL DataSend(PCHAR buf, int len);
	BOOL Connect(LPSTR ip_addr, WORD port, DWORD WinMsgNum);

	wsShopServerCli();
	virtual ~wsShopServerCli();

private:

	BYTE m_SendBuf[819200];	// 1C
	int m_SendBufLen;	// C801C
	BYTE m_RecvBuf[819200];	// C8020
	int m_RecvBufLen;	// 190020
	ShopProtocolCore wsProtocolCore;	// 190024

};

#endif // !defined(AFX_WSSHOPSERVERCLI_H__3D41B7CB_3A46_4A07_A754_CC3FAAB222C5__INCLUDED_)
