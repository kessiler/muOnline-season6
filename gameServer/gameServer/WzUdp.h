// WzUdp.h: interface for the WzUdp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZUDP_H__E6DD88C5_2F76_4C09_82DD_FE7D27E6822D__INCLUDED_)
#define AFX_WZUDP_H__E6DD88C5_2F76_4C09_82DD_FE7D27E6822D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_BUFFER_LENGTH   4096

#include "WzQueue.h"

#define DATA_BUFSIZE 2048

typedef struct
{
   OVERLAPPED	Overlapped;
   WSABUF		DataBuf;
   CHAR			Buffer[DATA_BUFSIZE];
   DWORD		Bytes;
	int			lOfs; 
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;


class WzUdp  
{	
	int		m_Port;
	
	HANDLE	m_ThreadHandle;
	DWORD	m_ThreadID;

public:
	SOCKET	m_Socket;
	LPBYTE	m_Recvbuf;
	LPBYTE  m_Sendbuf;
	int     m_dwLength;
	int		m_dwRecvOfs;
	int     m_dwSendLength;
	

	PER_IO_OPERATION_DATA	m_PerIoSendData;

	SOCKADDR_IN		m_SockAddr;

	//WZQueue RecvQueue;
	//WZQueue	SendQueue;

	WzUdp();
	virtual ~WzUdp();

	BOOL Init();	
	BOOL CreateSocket();
	BOOL Run();
	BOOL SendData(LPBYTE SendData, DWORD nSendDataLen);
	BOOL MuProtocolParse(LPBYTE RecvData, int & nRecvDataLen);
	BOOL Close();
	BOOL SendSet(char *ip, int port);
	BOOL RecvSet(int port);
	BOOL RecvThread();
	BOOL SetProtocolCore(void (*pc)(BYTE, BYTE*, int));
	
	void (*ProtocolCore)(BYTE protoNum, BYTE *aRecv, int aLen);
};

#endif // !defined(AFX_WZUDP_H__E6DD88C5_2F76_4C09_82DD_FE7D27E6822D__INCLUDED_)
