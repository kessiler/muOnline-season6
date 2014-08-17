// wsGameServer.cpp: implementation of the CwsJoinServer class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Prodef.h"
#include "Protocol.h"
#include "wsJoinServer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CwsJoinServer::CwsJoinServer()
{

	int n;

	for( n=0; n<MAX_SOCKETINDEX; n++) m_SockIndex[n] = -1;
}

CwsJoinServer::~CwsJoinServer()
{

}

BOOL CwsJoinServer::CreateServer(char *ip_addr, WORD port, DWORD WinServerMsg, DWORD WinClientMsg)
{
	sockaddr_in	addr;
	int			nResult;

#ifdef _DEBUG
	if( m_hWnd == NULL ) { gWindow.PrintLog("윈도우 핸들 에러"); return FALSE; }
#endif
	memset(&addr, 0, sizeof(sockaddr_in));
	
	addr.sin_family			= PF_INET;
    addr.sin_addr.s_addr	= htonl( INADDR_ANY );
	addr.sin_port			= htons( port );
 
	nResult = bind(m_socket, (LPSOCKADDR)&addr, sizeof(addr) );
    if( nResult == SOCKET_ERROR ) 
	{
		gWindow.PrintLog("Server bind error %d", WSAGetLastError());
		closesocket(m_socket);
		return FALSE;
    }

	nResult = listen( m_socket, 8 );
    if( nResult == SOCKET_ERROR) {
		closesocket(m_socket);
		gWindow.PrintLog("Server listen error %d", WSAGetLastError());
		return FALSE;
    }

	nResult = WSAAsyncSelect( m_socket, gWindow.WindowID, WinServerMsg, FD_READ|FD_WRITE|FD_ACCEPT|FD_CLOSE);
    if( nResult == SOCKET_ERROR) {
		closesocket(m_socket);
		gWindow.PrintLog("Server WSAAsyncSelect error %d", WSAGetLastError());
        return FALSE;
    }
	//cLogProc.Add("서버 소켓 CREATE");

	m_WinClientMsg = WinClientMsg;
	return TRUE;
}

BOOL CwsJoinServer::AcceptSocket(SOCKET & clientSocket, IN_ADDR	& cInAddr)
{
	SOCKET		cSocket;
	SOCKADDR_IN cAddr;
	int			Len;
	//int			nResult;
	
	Len = sizeof(SOCKADDR_IN);
	cSocket = accept( m_socket, (LPSOCKADDR)&cAddr, &Len);
	if( cSocket != INVALID_SOCKET )
	{
		memcpy( &cInAddr, &cAddr.sin_addr.s_addr, 4 );
		//gWindow.PrintLog("사용자가 접속요청(Accept) Address:%s, port %d socket number:%d", inet_ntoa(clientIn), ntohs(clientAddr.sin_port) , clientSocket);
		/*nResult = WSAAsyncSelect( cSocket, m_hWnd, m_WinClientMsg, FD_READ|FD_WRITE|FD_CLOSE );
		if( nResult == SOCKET_ERROR) {
			closesocket(cSocket);
			//gWindow.PrintLog("Server WSAAsyncSelect error %d", WSAGetLastError());
			return FALSE;
		}*/
	}
	clientSocket = cSocket;
	return TRUE;
}


#include "giocp.h"


BOOL CwsJoinServer::DataSend(short uindex, char *buf, int len)
{
	return IoSend(uindex, (LPBYTE)buf, len);
}

int CwsJoinServer::SetSocketBuffer(int index, SOCKET socket, char *ip)
{
	sb[index].Clear();
	sb[index].live = 1;	
	sb[index].m_socket = socket;
	strcpy(sb[index].Ip_addr, ip);
	return TRUE;
}
