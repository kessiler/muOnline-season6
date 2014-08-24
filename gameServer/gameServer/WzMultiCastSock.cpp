// GS-N	1.00.90	JPN		-	Completed
#include "stdafx.h"
#include "WzMultiCastSock.h"
#include "..\include\prodef.h"
#include <Ws2tcpip.h>

CWzMultiCastSock::CWzMultiCastSock()
{
	WSADATA wsadata;

	WSAStartup(0x101, &wsadata);
	this->m_skSocket = INVALID_SOCKET;
	this->m_iIPINFO_COUNT = 0;
	this->m_lpfnCALLBACK_RECVNOTIFY = NULL;
	this->m_lpfnCALLBACK_ERRORNOTIFY = NULL;
	memset(this->m_stIPINFO_LIST, 0, sizeof(this->m_stIPINFO_LIST));
}


CWzMultiCastSock::~CWzMultiCastSock()
{
	shutdown(this->m_skSocket, SD_RECEIVE);
	closesocket(this->m_skSocket);
	WSACleanup();
}


int CWzMultiCastSock::DoEnumIpAddress()
{
	this->m_iIPINFO_COUNT = 0;
	memset(this->m_stIPINFO_LIST, 0, sizeof(this->m_stIPINFO_LIST));

	char buf[128];
	gethostname(buf, sizeof(buf));
	hostent * lpHostent = gethostbyname(buf);
	in_addr * lpaddr;

	switch ( lpHostent->h_addrtype )
	{
		case 2:
			while ( lpaddr = (in_addr *)(*lpHostent->h_addr_list++)  )
			{
				this->m_stIPINFO_LIST[this->m_iIPINFO_COUNT].m_bInUsed = TRUE;
				strcpy(this->m_stIPINFO_LIST[this->m_iIPINFO_COUNT++].m_szIPADDR, inet_ntoa(*lpaddr));

				if ( this->m_iIPINFO_COUNT >= MAX_MULTICAST_SOCK )
					break;
			}
	}

	return this->m_iIPINFO_COUNT;
}



BOOL CWzMultiCastSock::GetEnummedIpAddress(int iIPINDEX, LPSTR lpszOutputIpAddress)
{
	if ( iIPINDEX < 0 || iIPINDEX >= MAX_MULTICAST_SOCK )
		return FALSE;

	if ( this->m_stIPINFO_LIST[iIPINDEX].m_bInUsed == TRUE )
	{
		if ( lpszOutputIpAddress != NULL )
		{
			memcpy(lpszOutputIpAddress, this->m_stIPINFO_LIST[iIPINDEX].m_szIPADDR, sizeof(this->m_stIPINFO_LIST[iIPINDEX].m_szIPADDR));
			return TRUE;
		}
	}

	return FALSE;
}



BOOL CWzMultiCastSock::StartMultiCast(LPSTR lpszIPADDR)
{
	if ( this->m_skSocket != INVALID_SOCKET )
	{
		closesocket(this->m_skSocket);
		this->m_skSocket = INVALID_SOCKET;
	}

	if ( !lpszIPADDR )
	{
		int iCOUNT = this->DoEnumIpAddress();

		if ( iCOUNT > 0 )
		{
			char szText[16];
			BOOL bRES = FALSE;

			bRES = this->GetEnummedIpAddress(0, szText);

			if ( bRES )
			{
				memcpy(this->m_szIPADDR_JOIN, szText, sizeof(szText));
				this->m_szIPADDR_JOIN[15] = 0;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		memcpy(this->m_szIPADDR_JOIN, lpszIPADDR, sizeof(this->m_szIPADDR_JOIN));
		this->m_szIPADDR_JOIN[15] = 0;
	}

	this->m_skSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if ( this->m_skSocket == INVALID_SOCKET )
	{
		this->m_skSocket = INVALID_SOCKET;
		return FALSE;
	}

	sockaddr_in sockaddr;

	sockaddr.sin_addr.S_un.S_addr = htonl(0);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(37769);
	
	int iERR = bind(this->m_skSocket, (::sockaddr *)&sockaddr, 16);

	if ( iERR == -1 )
	{
		this->SaveLastError(0);
		closesocket(this->m_skSocket);
		this->m_skSocket = INVALID_SOCKET;

		return FALSE;
	}

	ip_mreq mreq;
	memset(&mreq, 0, sizeof(mreq));
	mreq.imr_multiaddr.S_un.S_addr = inet_addr("227.100.100.100");
	mreq.imr_interface.S_un.S_addr = inet_addr(this->m_szIPADDR_JOIN);
	iERR = setsockopt(this->m_skSocket, IPPROTO_IP, 12, (char *)&mreq, sizeof(mreq));

	if ( iERR == -1 )
	{
		this->SaveLastError(0);
		closesocket(this->m_skSocket);
		this->m_skSocket = INVALID_SOCKET;

		return FALSE;
	}

	DWORD addr = inet_addr(this->m_szIPADDR_JOIN);
	iERR = setsockopt(this->m_skSocket, IPPROTO_IP, 9, (char *)&addr, sizeof(addr));

	if ( iERR == -1 )
	{
		this->SaveLastError(0);
		closesocket(this->m_skSocket);
		this->m_skSocket = INVALID_SOCKET;

		return FALSE;
	}
	this->m_hWorkerThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int (__stdcall*)(void *))CWzMultiCastSock::RecvThreadProc, this, NULL, &this->m_idWorkerThread);

	return TRUE;
}


BOOL CWzMultiCastSock::ReStartMultiCast()
{
	this->EndMultiCast();
	return this->StartMultiCast(this->m_szIPADDR_JOIN);
}	



void CWzMultiCastSock::EndMultiCast()
{
	if ( this->m_skSocket != INVALID_SOCKET )
	{
		closesocket(this->m_skSocket);
		this->m_skSocket = INVALID_SOCKET;
	}

	if ( this->m_hWorkerThread != INVALID_HANDLE_VALUE )
	{
		TerminateThread(this->m_hWorkerThread, 0);
		CloseHandle(this->m_hWorkerThread);
		this->m_hWorkerThread = INVALID_HANDLE_VALUE;
	}
}


BOOL CWzMultiCastSock::SendData(char * lpBUFFER, int iBUFSIZE)
{
	if ( !lpBUFFER )
		return FALSE;

	sockaddr_in sockaddr;
	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.S_un.S_addr = inet_addr("227.100.100.100");
	sockaddr.sin_port = htons(37769);

	int iSENDBYTES = sendto(this->m_skSocket, lpBUFFER, iBUFSIZE, 0, (::sockaddr *)&sockaddr, 16);

	if ( iSENDBYTES == -1 )
	{
		this->SaveLastError(0);
		closesocket(this->m_skSocket);
		this->m_skSocket = INVALID_SOCKET;

		return FALSE;
	}

	if ( iSENDBYTES < -1 )
	{
		this->SaveLastError(0);
		closesocket(this->m_skSocket);
		this->m_skSocket = INVALID_SOCKET;

		return FALSE;
	}

	return TRUE;
}


void CWzMultiCastSock::SaveLastError(int iErrorNo)
{
	if ( iErrorNo == 0 )
	{
		this->m_uLastErrorNum = WSAGetLastError();
	}
	else
	{
		this->m_uLastErrorNum = iErrorNo;
	}

	memset(this->m_szLastErrorMessage, 0, sizeof(this->m_szLastErrorMessage));
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, this->m_uLastErrorNum, LANG_NEUTRAL,
		this->m_szLastErrorMessage, sizeof(this->m_szLastErrorMessage),
		NULL);

	if ( this->m_lpfnCALLBACK_ERRORNOTIFY )
	{
		this->m_lpfnCALLBACK_ERRORNOTIFY(this, this->m_uLastErrorNum, this->m_szLastErrorMessage);
	}
}


void CWzMultiCastSock::SavePrivateErrorMessage(LPSTR lpszErrorMessage)
{
	this->m_uLastErrorNum = 0;
	strcpy(this->m_szLastErrorMessage, lpszErrorMessage);

	if ( this->m_lpfnCALLBACK_ERRORNOTIFY )
	{
		this->m_lpfnCALLBACK_ERRORNOTIFY(this, this->m_uLastErrorNum, this->m_szLastErrorMessage);
	}
}


unsigned int __stdcall CWzMultiCastSock::RecvThreadProc(void * lpParam)
{
	char cBUFFER[1024];
	sockaddr_in sockaddr;
	int isockaddr_len = 16;
	CWzMultiCastSock * lpCMSock = (CWzMultiCastSock *)lpParam;

	while ( true )
	{
		int iRecvLen = recvfrom(lpCMSock->m_skSocket, cBUFFER, sizeof(cBUFFER), 0,
			(::sockaddr *)&sockaddr, &isockaddr_len);

		if ( iRecvLen == -1 )
		{
			lpCMSock->SaveLastError(0);
			closesocket(lpCMSock->m_skSocket);
			lpCMSock->m_skSocket = INVALID_SOCKET;
			break;
		}

		if ( iRecvLen < 0 )
		{
			lpCMSock->SavePrivateErrorMessage("RecvThreadProc() ERROR : iRecvLen < 0");
			closesocket(lpCMSock->m_skSocket);
			lpCMSock->m_skSocket = INVALID_SOCKET;
			break;
		}

		int iSize = 0;
		BYTE btHeadCode = 0;
		BYTE btSubCode = 0;

		switch ( (BYTE)cBUFFER[0] )
		{
			case 0xC1:
				iSize = (BYTE)cBUFFER[1];
				btHeadCode = (BYTE)cBUFFER[2];
				btSubCode = (BYTE)cBUFFER[3];

				if ( lpCMSock->m_lpfnCALLBACK_RECVNOTIFY )
				{
					lpCMSock->m_lpfnCALLBACK_RECVNOTIFY(lpCMSock, sockaddr.sin_addr.S_un.S_addr,
						btHeadCode, btSubCode, cBUFFER, iRecvLen);
				}
				break;
			case 0xC2:
				iSize = ((BYTE)cBUFFER[1] << 8 )+ ((BYTE)cBUFFER[2]);
				btHeadCode = (BYTE)cBUFFER[3];
				btSubCode = (BYTE)cBUFFER[4];

				if ( lpCMSock->m_lpfnCALLBACK_RECVNOTIFY )
				{
					lpCMSock->m_lpfnCALLBACK_RECVNOTIFY(lpCMSock, sockaddr.sin_addr.S_un.S_addr,
						btHeadCode, btSubCode, cBUFFER, iRecvLen);
				}
		}
	}

	return FALSE;
}
