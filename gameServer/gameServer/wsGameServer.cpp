//	GS-N	1.00.90	JPN	-	Completed

#include "stdafx.h"
#include "wsGameServer.h"
#include "logproc.h"
#include "IpCache.h"




//004AFF30  /> \55            PUSH EBP
CwsGameServer::CwsGameServer()
{
	this->m_SendSec=0;
	this->m_SendSecTmp=0;
}


CwsGameServer::~CwsGameServer()
{
	// Empty
}

BOOL CwsGameServer::SetSocketIndex(SOCKET socket, int index)
{
	return 1;
}

BOOL CwsGameServer::CreateServer(LPSTR ip_addr, WORD port, DWORD WinServerMsg, DWORD WinClientMsg)
{
	sockaddr_in addr;
	int nResult;
	
	if (this->m_hWnd == 0 )
	{
		MsgBox("windows handle error");
		return 0;
	}

	memset(&addr, 0, sizeof(addr) );

	addr.sin_family=AF_INET; // INET
	addr.sin_addr.S_un.S_addr=htonl(0);
	addr.sin_port=htons(port);

	nResult=bind(this->m_socket, (sockaddr*)&addr, 16);

	if ( nResult == -1 )
	{
		LogAdd("Server bind error %d", WSAGetLastError() );
		closesocket(this->m_socket);
		return 0;
	}

	nResult=listen(this->m_socket , 4);

	if (nResult == -1)
	{
		closesocket(this->m_socket );
		LogAdd("Server listen error %d", WSAGetLastError() );
		return 0;
	}

	nResult=WSAAsyncSelect( this->m_socket , this->m_hWnd , WinServerMsg, FD_ACCEPT);

	if (nResult == -1)
	{
		closesocket(this->m_socket);
		LogAdd("Server WSAAsyncSelect error %d", WSAGetLastError());
		return 0;
	}

	this->m_WinClientMsg  =WinClientMsg;
	return 1;
}


BOOL CwsGameServer::AcceptSocket(SOCKET & clientSocket, in_addr & cInAddr)	
{
	SOCKET cSocket;
	sockaddr_in cAddr;
	int Len = 16;
	int nResult;
	WORD ling[2];

	cSocket=accept(this->m_socket , (sockaddr*)&cAddr, &Len);
	
	if (cSocket != INVALID_SOCKET)
	{
		memcpy(&cInAddr, &cAddr.sin_addr.S_un.S_addr , sizeof(cInAddr));

		if (IpCache.AddIp(inet_ntoa( cInAddr)) == 0)
		{
			closesocket( cSocket);
			return 0;
		}

		nResult=WSAAsyncSelect(cSocket, this->m_hWnd , this->m_WinClientMsg , FD_READ|FD_WRITE|FD_CLOSE);

		if ( nResult == -1 )
		{
			closesocket(cSocket);
			return 0;
		}

	}

	clientSocket=cSocket;

	ling[0]=0;
	ling[1]=0;

	setsockopt( clientSocket, SOL_SOCKET, 0xFFFFFF7F, (char*)&ling, sizeof(ling));

	return 1;
}

BOOL CwsGameServer::DataSendW(int uindex, PCHAR buf, int len)
{
	return 1;
}

BOOL CwsGameServer::DataSocketSend(SOCKET socket,  char* buf, int len)
{
	int nResult, nDx, nLeft;

	nDx=0;

	if (socket == INVALID_SOCKET )
	{
		LogAdd("INVALID_SOCKET %s %d", __FILE__, __LINE__);
		return 0;
	}

	nLeft = len;

	while (nLeft > 0)
	{
		nResult=send(socket, &buf[nDx], (len-nDx), 0);

		if (nResult == -1)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return 0;
			}
		}
		else
		{
			nDx += nResult;
			nLeft -=nResult;

			if ( nLeft <= 0)
			{
				break;
			}
		}
	}

	return 1;
}

BOOL CwsGameServer::FDWRITE_MsgDataSend(SOCKET socket)
{
	return 0;
}


BOOL CwsGameServer::DataRecv(SOCKET socket)
{
	return 0;
}

BOOL CwsGameServer::SetSocketBuffer(int index, SOCKET socket, LPSTR ip)
{
	return 1;
}











	