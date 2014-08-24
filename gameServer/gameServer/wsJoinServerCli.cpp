// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

// GS-N 0.99.60T Status : Completed
//	GS-N	1.00.18	JPN	0x0048D170	-	Completed

#include "stdafx.h"
#include "wsJoinServerCli.h"
#include "logproc.h"

wsJoinServerCli::wsJoinServerCli()
{
	return;
}

wsJoinServerCli::~wsJoinServerCli()
{
	return;
}


BOOL wsJoinServerCli::SetProtocolCore(WsProtocolCore pc)
{
	this->wsProtocolCore = pc;
	return TRUE;
}
	
BOOL wsJoinServerCli::Connect(LPSTR ip_addr, WORD port, DWORD WinMsgNum)
{
	hostent* host;
	sockaddr_in addr;
	int nResult;

	host = 0;

	if ( this->m_hWnd  == 0 )
	{
		LogAdd(" Connect Error %s %d", __FILE__, __LINE__);
		return FALSE;
	}
	addr.sin_family = AF_INET;
	addr.sin_port =  htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip_addr);

	if ( addr.sin_addr.S_un.S_addr == -1 )
	{
		host = gethostbyname(ip_addr);
		if ( host == 0 )
		{
			LogAdd("server name not found [%s]", ip_addr);
			return FALSE;
		}

		memcpy(&addr.sin_addr.S_un.S_addr, *host->h_addr_list  , host->h_length  );	
	}

	nResult=connect(this->m_socket, (sockaddr*)&addr, 16);

	if (nResult== -1)
	{
		if ( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			this->Close(this->m_socket );
			return FALSE;
		}
	}

	nResult=WSAAsyncSelect(this->m_socket, this->m_hWnd , WinMsgNum, FD_READ|FD_WRITE|FD_CLOSE);

	if ( nResult == -1 )
	{
		closesocket(this->m_socket );
		LogAdd("Client WSAAsyncSelect error %d", WSAGetLastError());
		return FALSE;
	}
	this->m_SendBufLen=0;
	this->SetConnect(TRUE);

	return TRUE;
}



BOOL wsJoinServerCli::DataSend(PCHAR buf, int len)
{
	int nResult;
	int nLeft;
	int nDx;	// Degeneration X :)
	LPBYTE sendbuf;
	int* sendbuflen;
	
	if ( len <= 0 )
	{
		return 0;
	}

	if ( this->m_socket  == INVALID_SOCKET )	// 
	{
		return 0;
	}

	nDx = 0;

	sendbuf = this->m_SendBuf;
	sendbuflen= &len;	// Strange, dangeorus

	if ( this->m_SendBufLen   > 0 )
	{
		if ( (this->m_SendBufLen +len) < 819200 )
		{
			memcpy( &this->m_SendBuf[this->m_SendBufLen]  , buf, len);	// Review this later

			this->m_SendBufLen +=len;
			return 1;
		}
		this->Close();
		return FALSE;
	}

	nLeft= *sendbuflen;

	while ( true )
	{
		nResult=send(this->m_socket, (char*)&buf[nDx], (*sendbuflen - nDx), 0);

		if ( nResult == -1)
		{
			if ( WSAGetLastError() != WSAEWOULDBLOCK )
			{
				LogAdd("error-L3 : %d (%s %d)", WSAGetLastError(), __FILE__, __LINE__);
				*sendbuflen=0;
				this->Close(this->m_socket );
				return FALSE;
			}

			if( (*sendbuflen + nLeft) > 819200 )
			{
				LogAddC(2,"error-L3 : buffer error closed %d, %d", (*sendbuflen+nLeft), 819200);
				*sendbuflen = 0;
				this->Close(this->m_socket);
				return FALSE;
			}

			if( !this->m_SendBufLen )
			{
				return false;
			}

			if ( nDx >= 0 )
			{
				memcpy( &this->m_SendBuf[this->m_SendBufLen],  &buf[nDx], nLeft);
				
				this->m_SendBufLen +=nLeft;

				LogAdd("error-L3 : Copy1 : nLeft:%d len:%d nDx:%d", nLeft, this->m_SendBufLen , nDx);
				return TRUE;
			}
		}
		else if ( nResult == 0 )
		{
			LogAdd("error-L3 : send()  result is zero", WSAGetLastError());
			break;
		}

		if ( nResult >0 )
		{
			nDx += nResult;
			nLeft -= nResult;
			*sendbuflen -= nResult;
		}
		if ( nLeft <= 0)
		{
			break;
		}
	}
	return TRUE;
}




BOOL wsJoinServerCli::FDWRITE_MsgDataSend()
{
	int nResult;
	int nDx;	// Degeneration X :)
	LPBYTE sendbuf;
	int* sendbuflen;

	nDx=0;
	sendbuf=&this->m_SendBuf[0];
	sendbuflen=&this->m_SendBufLen ;

	if (this->m_SendBufLen < 1)
	{
		return TRUE;
	}

	while (*sendbuflen>0)
	{
		nResult=send(this->m_socket, (PCHAR)&sendbuf[nDx], *sendbuflen-nDx, 0);
		if (nResult == -1)
		{
			if ( WSAGetLastError() != WSAEWOULDBLOCK)
			{
				LogAdd("FD_WRITE send() ¿¡·¯ %d %d", WSAGetLastError(), *sendbuflen);
				this->Close();
				return FALSE;
			}
			else
			{
				LogAdd("FD_WRITE send() WSAEWOULDBLOCK : %d", WSAGetLastError());
				break;
			}
		}
		if (nResult <= 0)
		{
			LogAdd("send() result is zero %d", WSAGetLastError());
			break;
		}
		LogAdd("error-L3 : nDx %d m_SendBufLen %d", nDx, this->m_SendBufLen );

		if (nResult > 0)
		{
			nDx += nResult;
			*sendbuflen -= nResult;
		}

		LogAdd("error-L3 : nDx %d m_SendBufLen %d", nDx, this->m_SendBufLen );
	}
	return TRUE;
}



int wsJoinServerCli::DataRecv()
{
	int nResult;
	unsigned char* recvbuf;
	int* recvbuflen;
	int lOfs;
	int size;
	BYTE headcode;

	recvbuf= (UCHAR*)&this->m_RecvBuf[0]   ;
	recvbuflen=&this->m_RecvBufLen ;
	nResult=recv(this->m_socket , (CHAR*)&recvbuf[*recvbuflen], (819200-*recvbuflen), 0);

	if ( nResult == 0 )
	{
		LogAdd("error-L3 : closed %d - buff:%d(%d) ",
			WSAGetLastError(), this->m_RecvBufLen, 819200 - *recvbuflen);

		return 1;
	}
	else if (nResult == -1 )
	{
		if ( WSAGetLastError() == WSAEWOULDBLOCK )
		{
			return 1;
		}
		LogAdd("error-L3 : recv error %d", WSAGetLastError() );
		return 1;
	}

	*recvbuflen += nResult;

	if (*recvbuflen<3)
	{
		return 3;
	}

	lOfs=0;
	size=0;

	while ( true )
	{
		
		if (recvbuf[lOfs] == 0xC1 )	// Type packet handling C1 Type
		{
			unsigned char* c1subbase = (UCHAR*)&recvbuf[lOfs];
			size=c1subbase[1];
			headcode=c1subbase[2];
		}
		else if ( recvbuf[lOfs] == 0xC2 ) // Type packet handling C2 Type
		{
			unsigned char* c2subbase=(UCHAR*)&recvbuf[lOfs];
			size = c2subbase[1] * 256;
			size |= c2subbase[2];
			headcode = c2subbase[3];
		}
		else
		{
			LogAdd("error-L2 : header error (%s %d)lOfs:%d, size:%d", __FILE__, __LINE__, lOfs, *recvbuflen);
			*recvbuflen = 0;
			return 0;
		}

		if ( size <= 0 )
		{
			LogAdd("error-L2 : size %d", size);
			return 0;
		}

		if ( size <= *recvbuflen )
		{
			this->wsProtocolCore(headcode, &recvbuf[lOfs], size);
			lOfs += size;
			*recvbuflen -= size;

			if ( *recvbuflen <= 0)
			{
				break;
			}
		}
		
		else if ( lOfs > 0 )
		{
			if ( *recvbuflen < 1 ) 
			{
				break;
			}

			memcpy(recvbuf, &recvbuf[lOfs], *recvbuflen);
			break;
		}
		else
		{
			break;
		}
	}
	return 0;
}



