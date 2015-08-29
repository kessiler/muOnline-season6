// WzUdp.cpp: implementation of the WzUdp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zConnectServer.h"
#include <winsock2.h>
#include "WzQueue.h"
#include "WzUdp.h"
#include "UdpProtocol.h"
#include "Window.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD WINAPI WzUdpRecvThread(WzUdp*		lpWzUdp);
DWORD WINAPI WzUDPSendThread(LPVOID p);

WzUdp::WzUdp()
{
    Init();
}

WzUdp::~WzUdp()
{
    Close();
}

BOOL WzUdp::Init()
{
    WSADATA			wsd;

    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        return FALSE;
    }
    m_dwLength  = DEFAULT_BUFFER_LENGTH;
    m_dwRecvOfs = 0;
    m_ThreadHandle = NULL;
    return TRUE;
}

BOOL WzUdp::Close()
{
    TerminateThread(m_ThreadHandle, 0);
    if( m_ThreadHandle != NULL )
    {
        WaitForSingleObject( m_ThreadHandle, INFINITE );
        CloseHandle(m_ThreadHandle);
        m_ThreadHandle = NULL;
    }
    HeapFree(GetProcessHeap(), 0, m_Recvbuf);
    return TRUE;
}

BOOL WzUdp::CreateSocket()
{
    m_Socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, NULL);

    if( m_Socket == INVALID_SOCKET )
    {
        return FALSE;
    }
    return TRUE;
}

BOOL WzUdp::SendSet(char *ip, int port)
{
    m_Port					= port;
    m_SockAddr.sin_family	= AF_INET;
    m_SockAddr.sin_port		= htons(port);
    if( (m_SockAddr.sin_addr.s_addr	= inet_addr(ip)) == INADDR_NONE )
    {
        struct hostent *host=NULL;
        host = gethostbyname(ip);
        if( host )
        {
            CopyMemory(&m_SockAddr.sin_addr, host->h_addr_list[0], host->h_length);
        }
        else
        {
            return FALSE;
        }
    }

    g_Window.PrintLog("[Update] [%d] [Send] Port created", port);
    return TRUE;
}


BOOL WzUdp::RecvSet(int port)
{
    m_Port						= port;
    m_SockAddr.sin_port			= htons(port);
    m_SockAddr.sin_family		= AF_INET;
    m_SockAddr.sin_addr.s_addr	= htonl(INADDR_ANY);

    if( bind(m_Socket, (SOCKADDR *)&m_SockAddr, sizeof(m_SockAddr)) == SOCKET_ERROR )
    {
        return FALSE;
    }

    m_Recvbuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_dwLength);
    if (!m_Recvbuf)
    {
        return FALSE;
    }

    g_Window.PrintLog("[Update] [%d] [Recv] Port created", port);
    return TRUE;
}



BOOL WzUdp::SendData(LPBYTE SendData, DWORD nSendDataLen)
{
    DWORD Ret;

    ZeroMemory(&(m_PerIoSendData.Overlapped), sizeof(OVERLAPPED));

    memcpy(m_PerIoSendData.Buffer, SendData, nSendDataLen);

    m_PerIoSendData.lOfs		= nSendDataLen;
    m_PerIoSendData.DataBuf.buf = m_PerIoSendData.Buffer;
    m_PerIoSendData.DataBuf.len = m_PerIoSendData.lOfs;

    Ret = WSASendTo(m_Socket, &(m_PerIoSendData.DataBuf), 1, &nSendDataLen,
                    0, (SOCKADDR*)&m_SockAddr, sizeof(m_SockAddr), &(m_PerIoSendData.Overlapped), NULL);
    if( Ret == SOCKET_ERROR )
    {
        if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            g_Window.PrintLog("[Error] WSASend() %d\n", WSAGetLastError());
            return -1;
        }
    }
    return TRUE;
}

BOOL WzUdp::MuProtocolParse(LPBYTE RecvData, int & nRecvDataLen, char *ip)
{
    int		lOfs = 0;
    int		size = 0;
    BYTE	headcode;

    while( TRUE )
    {
        if( RecvData[lOfs] == 0xC1 )
        {
            size		= *(RecvData+lOfs+1);
            headcode	= *(RecvData+lOfs+2);
        }
        else if( RecvData[lOfs] == 0xC2 )
        {
            size		 = (WORD)(*(RecvData+lOfs+1));
            size	   <<= 8;
            size		|= (WORD)(*(RecvData+lOfs+2));
            headcode	 = *(RecvData+lOfs+3);
        }
        else
        {
            g_Window.PrintLog("[Error] Wrong packet header (%s %d)lOfs:%d", __FILE__, __LINE__, lOfs);
            m_dwRecvOfs = 0;
            return FALSE;
        }
        if( size <= 0 )
        {
            g_Window.PrintLog("[Error] Packet size %d.", size);
            return FALSE;
        }
        else if( size <= nRecvDataLen )
        {
            UdpProtocolCore(headcode, (RecvData+lOfs), size, ip);
            lOfs		 += size;
            m_dwRecvOfs  -= size;
            if( m_dwRecvOfs <= 0 ) break;
        }
        else
        {
            if( lOfs > 0 )
            {
                if( m_dwRecvOfs < 1 )
                {
                    return FALSE;
                }
                else
                {
                    memcpy(RecvData, (RecvData+lOfs), m_dwRecvOfs);
                    return TRUE;
                }
            }
            break;
        }
    }
    return TRUE;
}


BOOL WzUdp::Run()
{
    if( (m_ThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WzUdpRecvThread, (void*)this, 0, &m_ThreadID) ) == NULL)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL WzUdp::RecvThread()
{
    int			ret;
    DWORD		dwSenderSize;
    SOCKADDR_IN	sender;

    dwSenderSize = sizeof(sender);
    while(1)
    {
        ret = recvfrom(m_Socket, (char*)m_Recvbuf+m_dwRecvOfs,
                       DEFAULT_BUFFER_LENGTH-m_dwRecvOfs, 0, (SOCKADDR *)&sender, (int*)&dwSenderSize);

        if (ret == SOCKET_ERROR)
        {
            g_Window.PrintLog("[Error] recvfrom() %d", WSAGetLastError());
//			SendMessage(g_hWnd,WM_CLOSE,NULL,NULL);
        }
        else if (ret == 0)
        {
        }
        else
        {
            m_dwLength  = ret;
            m_dwRecvOfs += ret;
            MuProtocolParse(m_Recvbuf, m_dwLength, inet_ntoa(sender.sin_addr));
        }
    }
}

DWORD WINAPI WzUdpRecvThread(WzUdp*	lpWzUdp)
{
    lpWzUdp->RecvThread();
    return TRUE;
}

