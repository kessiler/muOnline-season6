#include "StdAfx.h"
#include "ProDef.h"
#include <winsock2.h>
#include "MyWinsockBase.h"

MyWinsockBase::MyWinsockBase()
{
	m_socket = INVALID_SOCKET;
	Startup();
}

MyWinsockBase::~MyWinsockBase()
{
	WSACleanup();
}

BOOL MyWinsockBase::Startup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup( wVersionRequested, &wsaData);
	if( err != 0)
	{
		MessageBox(NULL,"WINSOCK 초기화 에러","Error",MB_OK);
		return FALSE;
	}
	if ( SET_NUMBERL( wsaData.wVersion ) != 2 ||
        SET_NUMBERH( wsaData.wVersion ) != 2 ) {
		WSACleanup( );
		MessageBox(NULL,"WINSOCK 버전이 낮습니다.","Error",MB_OK);
		return FALSE;
	}
	m_socket = INVALID_SOCKET;
	m_iMaxSockets = wsaData.iMaxSockets;
	m_Connect = FALSE;
	return TRUE;
}

BOOL MyWinsockBase::CreateSocket()
{	
	int size = sizeof(int);

	m_socket = socket( PF_INET, SOCK_STREAM, 0);
	if( m_socket == INVALID_SOCKET ) 
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MyWinsockBase::Close()
{	
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	m_Connect = FALSE;
	return TRUE;
}


BOOL MyWinsockBase::Close(SOCKET socket)
{
	shutdown(socket,0);
	closesocket(socket);
	m_socket = INVALID_SOCKET;
	m_Connect = FALSE;
	return TRUE;
}

BOOL MyWinsockBase::Close2(SOCKET socket)
{
	shutdown(m_socket,SD_SEND);
	closesocket(socket);
	m_socket = INVALID_SOCKET;
	m_Connect = FALSE;
	return TRUE;
}


SOCKET MyWinsockBase::GetSocket()
{
	return m_socket;
}

int MyWinsockBase::GetRecvBuffSize()
{
	return m_recvbufsize;
}

int MyWinsockBase::GetSendBuffSize()
{
	return m_sendbufsize;
}

BOOL MyWinsockBase::GetConnect()
{
	return m_Connect;
}

void MyWinsockBase::SetConnect(BOOL connected)
{
	m_Connect = connected;
}