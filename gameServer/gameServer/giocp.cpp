//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include <winsock2.h>
#include "spe.h"
#include "giocp.h"
#include "logproc.h"
#include "GameMain.h"
#include "GameServer.h"
#include "user.h"
#include "CheatGuard.h"
#include "ConnectEx.h"
#include "OfflineTrade.h"
#ifdef __MAKELO__
#include "OfflineAttack.h"
#endif
#ifdef OFFEXP
#include "OffExp.h"
#endif

enum {
	RECV_IO,
	SEND_IO
} SOCKET_FLAG;

HANDLE g_CompletionPort;
DWORD g_dwThreadCount;
enum SOCKET_FLAG;
CRITICAL_SECTION criti;
HANDLE g_ThreadHandles[MAX_IO_THREAD_HANDLES];
int g_ServerPort;
HANDLE g_IocpThreadHandle;
LPBYTE ExSendBuf;

SOCKET g_Listen = INVALID_SOCKET;

void GiocpInit()
{
	ExSendBuf=new BYTE[MAX_EXSENDBUF_SIZE];
}

void GiocpDelete()
{
	delete[] ExSendBuf;
}

int CreateGIocp(int server_port)
{
	DWORD	ThreadID;
	
	g_ServerPort = server_port;
	if ((g_IocpThreadHandle = CreateThread(NULL, 0, IocpServerWorker, (LPVOID)NULL,
			0, &ThreadID)) == NULL)
	{
		LogAdd("CreateThread() failed with error %d", GetLastError());
		return FALSE;
	}
	return TRUE;
}

void DestroyGIocp()
{
	LogAddTD("Error-L10");

	closesocket(g_Listen);

	for (DWORD dwCPU=0; dwCPU < g_dwThreadCount;dwCPU++ )
	{
		TerminateThread( g_ThreadHandles[dwCPU] , 0);
	}

	TerminateThread(g_IocpThreadHandle, 0);

	if ( g_CompletionPort != NULL )
	{
		CloseHandle(g_CompletionPort);
		g_CompletionPort=NULL;
	} 
	
}

//1.01.03
int IsHackTool(BYTE headcode, int size, int uIndex, BYTE xcode)
{
	char	temp[1000];
	int		iOutPacketCount;
	int		iOutPacketTotalSize;
	int		iOutEncryptPacketCount;
	// ----
	if( uIndex < 0 || uIndex >= OBJMAX )
	{
		return 0;
	}
	// ----
	iOutPacketCount			= 0;
	iOutEncryptPacketCount	= 0;
	iOutPacketTotalSize		= 0;
	gObj[uIndex].m_kRecvPacketStatistics.GetStatistics(iOutPacketCount, iOutEncryptPacketCount, (DWORD&)iOutPacketTotalSize);
	// ----
	if( gObj[uIndex].m_kRecvPacketStatistics.Process() == PacketStatistics::PS_RESET )
	{
		temp[0] = 0;
		wsprintfA(temp, "PacketCount = %d, iOutEncryptPacketCount = %d, PacketTotalSize = %d",
			iOutPacketCount, iOutEncryptPacketCount, iOutPacketTotalSize);
		//TRACE_LOG(temp);
	}
	// ----
	if( iOutPacketCount > g_CheatGuard.m_PacketsPerSecond )
	{
		g_CheatGuard.WriteLog("[%s][%s] [PacketTool] Packets Per Second = %d", gObj[uIndex].AccountID, gObj[uIndex].Name, iOutPacketCount);
		g_CheatGuard.Disconnect(uIndex);
		//LogAddC(2, "[HACKTOOL] : Packets Per Second = %dip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
		//	iOutPacketCount, gObj[uIndex].Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode,
		//	__FILE__, __LINE__, gObj[uIndex].Connected);
		return 1;
	}
	if( headcode == 0xC3 || headcode == 0xC8 )
	{
		return 0;
	}
	// ----
	//Not need check for 0xC3...
	gObj[uIndex].m_kRecvPacketStatistics.AddPacketInfo(xcode == 0xC3 || xcode == 0xC4, size);
	// ----
	return 0;
}

int CreateListenSocket()
{
	sockaddr_in InternetAddr;
	int nRet;

	g_Listen=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if ( g_Listen == -1 )
	{
		LogAdd("WSASocket() failed with error %d", WSAGetLastError() );
		return 0;
	}
	else
	{
		InternetAddr.sin_family=AF_INET;
		InternetAddr.sin_addr.S_un.S_addr=htonl(0);
		InternetAddr.sin_port=htons(g_ServerPort);
		nRet=bind(g_Listen, (sockaddr*)&InternetAddr, 16);
		
		if ( nRet == -1 )
		{
			MsgBox(lMsg.Get(MSGGET(2, 55)));
			SendMessage(ghWnd, WM_CLOSE, 0,0);	// Kill aplication
			return 0 ;
		}
		else
		{
			nRet=listen(g_Listen, 5);
			if (nRet == -1)
			{
				LogAdd("listen() failed with error %d", WSAGetLastError());
				return 0;
			}
			else
			{
				return 1;
			}
		}
	} 
}

DWORD WINAPI IocpServerWorker(LPVOID p)
{
	SYSTEM_INFO SystemInfo;
	DWORD ThreadID;
	SOCKET Accept;
	int nRet;
	int ClientIndex;
	SOCKADDR_IN cAddr;
	IN_ADDR		cInAddr;
	int			cAddrlen = sizeof( cAddr );
	LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL; 
	DWORD RecvBytes;
	DWORD Flags=0;
	
	InitializeCriticalSection(&criti);
	GetSystemInfo(&SystemInfo);

	g_dwThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	__try
	{

		g_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if ( g_CompletionPort == NULL)
		{
			LogAdd("CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}
		
		for(DWORD dwCPU = 0; dwCPU < g_dwThreadCount; dwCPU++)
		{
			HANDLE ThreadHandle;
			// Create a server worker thread and pass the completion port to the thread.
			
			ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, g_CompletionPort, 0, &ThreadID);
			if ( ThreadHandle == NULL)
			{
				LogAdd("CreateThread() failed with error %d", GetLastError());
				__leave;
			}
			g_ThreadHandles[dwCPU] = ThreadHandle;
			CloseHandle(ThreadHandle);

		}

		if (!CreateListenSocket() )
			__leave;

		
		while(TRUE)		
		{
			Accept = WSAAccept(g_Listen, (LPSOCKADDR)&cAddr, &cAddrlen, NULL, 0);
			if (Accept==SOCKET_ERROR)
			{
				EnterCriticalSection(&criti);
				LogAdd("WSAAccept() failed with error %d", WSAGetLastError());
				LeaveCriticalSection(&criti);
				continue;
			}
			EnterCriticalSection(&criti);

			memcpy( &cInAddr, &cAddr.sin_addr.s_addr, 4 );

			ClientIndex = gObjAddSearch(Accept, inet_ntoa(cInAddr) );
			
			if ( ClientIndex == -1 )
			{
				LogAddL("error-L2 : ClientIndex = -1");
				closesocket(Accept);
				LeaveCriticalSection(&criti);
				continue;
			}

			if (UpdateCompletionPort(Accept, ClientIndex, 1) == 0 )
			{
				LogAddL("error-L1 : %d %d CreateIoCompletionPort failed with error %d", Accept, ClientIndex, GetLastError() );
				closesocket(Accept);
				LeaveCriticalSection(&criti);
				continue;
			}

			if (gObjAdd(Accept, inet_ntoa(cInAddr), ClientIndex) == -1 )
			{
				LogAddL("error-L1 : %d %d gObjAdd() failed with error %d", Accept, ClientIndex, GetLastError() );
				LeaveCriticalSection(&criti);
				closesocket(Accept);
				continue;
			}
				
			memset(&gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped, 0, sizeof(OVERLAPPED));
			memset(&gObj[ClientIndex].PerSocketContext->IOContext[1].Overlapped, 0, sizeof(OVERLAPPED));
			gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.buf = gObj[ClientIndex].PerSocketContext->IOContext[0].Buffer;
			gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.len = MAX_IO_BUFFER_SIZE;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nTotalBytes = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nSentBytes = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO    = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nSecondOfs = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].IOOperation = RECV_IO;
			
			gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.buf = gObj[ClientIndex].PerSocketContext->IOContext[0].Buffer;
			gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.len = MAX_IO_BUFFER_SIZE;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nTotalBytes= 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nSentBytes = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nWaitIO    = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nSecondOfs = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].IOOperation = SEND_IO;
			gObj[ClientIndex].PerSocketContext->m_socket = Accept;
			gObj[ClientIndex].PerSocketContext->nIndex   = ClientIndex;

			nRet = WSARecv(Accept, &(gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf), 1, &RecvBytes, &Flags,
				&(gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped), NULL);

			if( nRet == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING )
			{
				LogAdd("error-L1 : WSARecv() failed with error %d", WSAGetLastError());
				gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
				CloseClient(gObj[ClientIndex].PerSocketContext, FALSE);
				LeaveCriticalSection(&criti);
				continue;
			}

			gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 1;
			gObj[ClientIndex].PerSocketContext->dwIOCount++;

			LeaveCriticalSection(&criti);
			SCPJoinResultSend(ClientIndex, 1);

		}
	}
	__finally  
	{
		if( g_CompletionPort )
		{
			for (DWORD i = 0; i < g_dwThreadCount; i++) 
					PostQueuedCompletionStatus(g_CompletionPort, 0, 0, NULL); 
		}
		if( g_CompletionPort )
		{
			CloseHandle(g_CompletionPort);
			g_CompletionPort = NULL;
		}
		if (g_Listen != INVALID_SOCKET)  
		{ 
            closesocket(g_Listen);  
            g_Listen = INVALID_SOCKET; 
		} 
	}
	return TRUE;
}

DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID)
{
	HANDLE	CompletionPort = (HANDLE) CompletionPortID;
	DWORD	dwIoSize;
	DWORD	RecvBytes;
	DWORD	Flags;
	DWORD	dwSendNumBytes = 0;
	BOOL	bSuccess = FALSE; 
	int		nRet;
	int     ClientIndex;

	LPPER_SOCKET_CONTEXT	lpPerSocketContext = NULL;
	LPOVERLAPPED			lpOverlapped = NULL; 
	LPPER_IO_CONTEXT		lpIOContext = NULL;
	
	while(TRUE)
	{
		bSuccess = GetQueuedCompletionStatus(
			CompletionPort,
			&dwIoSize,
			(LPDWORD)&ClientIndex,
			&lpOverlapped,
			INFINITE
		);

		if( !bSuccess )
		{
			if( lpOverlapped != NULL )
			{
				int aError = GetLastError();
				if ( (aError != ERROR_NETNAME_DELETED) && (aError != ERROR_CONNECTION_ABORTED) && (aError != ERROR_OPERATION_ABORTED) && (aError != 0x79) )
				{
					EnterCriticalSection(&criti);
					LogAdd("Error Thread : GetQueueCompletionStatus( %d )", GetLastError());
					LeaveCriticalSection(&criti);
					return 0;
				}
			}
		}

		EnterCriticalSection(&criti);
		lpPerSocketContext = gObj[ClientIndex].PerSocketContext;
		
		lpPerSocketContext->dwIOCount--;
		
		if( dwIoSize == 0 )
		{
			LogAdd("Connection Closed, dwIoSize == 0 (Index:%d)", lpPerSocketContext->nIndex);
			CloseClient(lpPerSocketContext, 0);
			LeaveCriticalSection(&criti);
			continue;
		}

		lpIOContext = (LPPER_IO_CONTEXT)lpOverlapped;

		if( lpIOContext == NULL ) continue;
		if( lpIOContext->IOOperation == SEND_IO )
		{
			lpIOContext->nSentBytes += dwIoSize;

			if( lpIOContext->nSentBytes >= lpIOContext->nTotalBytes )
			{
				lpIOContext->nWaitIO = 0;
				if( lpIOContext->nSecondOfs > 0 )
				{
					IoSendSecond(lpPerSocketContext);
				}
			}
			else
			{
				IoMoreSend(lpPerSocketContext);
			}
		}
		else if( lpIOContext->IOOperation == RECV_IO )
		{
			RecvBytes = 0;

			lpIOContext->nSentBytes += dwIoSize;
			
			if( RecvDataParse(lpIOContext, lpPerSocketContext->nIndex) == FALSE )
			{
				LogAdd("error-L1 : Socket Header error %d, %d", WSAGetLastError(), lpPerSocketContext->nIndex);
				CloseClient(lpPerSocketContext, 0);
				LeaveCriticalSection(&criti);
				continue;
			}
			
			lpIOContext->nWaitIO = 0;
			Flags = 0;
			ZeroMemory(&(lpIOContext->Overlapped), sizeof(OVERLAPPED));
				
			lpIOContext->wsabuf.len		= MAX_IO_BUFFER_SIZE-lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf		= lpIOContext->Buffer+lpIOContext->nSentBytes;
			lpIOContext->IOOperation	= RECV_IO;
				
			nRet = WSARecv(lpPerSocketContext->m_socket, &(lpIOContext->wsabuf), 1, &RecvBytes, &Flags,
				&(lpIOContext->Overlapped), NULL);
			
			if( nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING) )
			{
				LogAdd("WSARecv() failed with error %d", WSAGetLastError());
				//lpIOContext->nWaitIO = 2;
				CloseClient( lpPerSocketContext, FALSE);
				LeaveCriticalSection(&criti);
				continue;
			}
				
			lpPerSocketContext->dwIOCount++;
			lpIOContext->nWaitIO = 1;
		}
		LeaveCriticalSection(&criti);
	}
	return TRUE;
}

BOOL RecvDataParse(LPPER_IO_CONTEXT	lpIOContext, int uIndex)	
{
	if ( lpIOContext->nSentBytes < 3 )
	{
		return TRUE;
	}

	LPBYTE recvbuf;
	int lOfs = 0;
	int size = 0;
	BYTE headcode;
	BYTE xcode = 0;
	BYTE byDec[7024];

	recvbuf = (LPBYTE)lpIOContext->Buffer;

	while ( 1 )
	{
		if( recvbuf[lOfs] == 0xC1 || 
			recvbuf[lOfs] == 0xC3 ) 
		{
			PBMSG_HEAD* lphead = (PBMSG_HEAD*)(recvbuf+lOfs);
			size				= lphead->size;
			headcode			= lphead->headcode;
			xcode				= recvbuf[lOfs];
		}
		else if( recvbuf[lOfs] == 0xC2 || 
				 recvbuf[lOfs] == 0xC4 ) //
		{
			PWMSG_HEAD* lphead	= (PWMSG_HEAD*)(recvbuf+lOfs);
			size				= ((WORD)(lphead->sizeH)<<8);
			size			   |= (WORD)(lphead->sizeL);
			headcode			= lphead->headcode;
			xcode				= recvbuf[lOfs];
		}
		else  // 헤더가 맞지 않다면..
		{
			LogAdd("error-L1 : Header error (%s %d)lOfs:%d, size:%d",
				__FILE__, __LINE__, lOfs, 
				lpIOContext->nSentBytes);
			lpIOContext->nSentBytes = 0;
			return FALSE;
		}
		if ( size <= 0 )
		{
			LogAdd("error-L1 : size %d",size);
			return FALSE;
		}

		if ( size <= lpIOContext->nSentBytes )
		{
			if ( xcode == 0xC3 )
			{
				int iSize = g_SimpleModulusCS.Decrypt( byDec+2,(recvbuf+lOfs+2), size-2);

				if ( iSize < 0 )
				{

				}
				else
				{
					PBMSG_HEAD* lphead = (PBMSG_HEAD*)(byDec+1);
					headcode		= lphead->headcode;
					BYTE subhead	= byDec[2];
					byDec[1]		= 0xC1;
					byDec[2]		= ((BYTE)iSize)+2;

					CStreamPacketEngine_Server PacketStream;

					PacketStream.Clear();

					if ( PacketStream.AddData(byDec+1, iSize+2) == 0 )
					{
						LogAddC(2, "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
							gObj[uIndex].Ip_addr, 
							gObj[uIndex].AccountID, 
							gObj[uIndex].Name, 
							headcode, 
							__FILE__, __LINE__, 
							gObj[uIndex].Connected);
						return FALSE;
					}

					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						LogAddC(2, "error-L1 : CStreamPacketEngine ExtractPacket Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
							gObj[uIndex].Ip_addr, 
							gObj[uIndex].AccountID, 
							gObj[uIndex].Name, 
							headcode, 
							__FILE__, __LINE__, 
							gObj[uIndex].Connected);
						return FALSE;
					}

					if ( xcode == 0xC3 && headcode == 0xC5 )
					{
						LogAdd("암호패킷 : [0x%x]" , headcode);
					}

					if( IsHackTool(headcode, size, uIndex, xcode) == 1 )
					{
						return FALSE;
					}
					ProtocolCore(headcode, byDec, iSize, uIndex, 1, subhead);
				}
			}

			else if ( xcode == 0xC4 )
			{
				int iSize = g_SimpleModulusCS.Decrypt( byDec+3, (recvbuf+lOfs+3), size-3);
				if ( iSize < 0 )
				{

				}
				else
				{
					BYTE* pDecBuf = byDec+1;
					BYTE subhead	= byDec[3];
					byDec[1]		= 0xC2;
					WORD wsize		= ((WORD)iSize)+3;
					byDec[2]		= SET_NUMBERH(wsize);
					byDec[3]		= SET_NUMBERL(wsize);

					CStreamPacketEngine_Server PacketStream;

					PacketStream.Clear();
					if ( PacketStream.AddData(byDec+1, iSize+3) == 0 )
					{
						LogAddC(2, "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",	
							gObj[uIndex].Ip_addr, 
							gObj[uIndex].AccountID, 
							gObj[uIndex].Name, 
							headcode, 
							__FILE__, 
							__LINE__, 
							gObj[uIndex].Connected);
						return FALSE;
					}

					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						LogAddC(2, "error-L1 : CStreamPacketEngine ExtractPacket Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
							gObj[uIndex].Ip_addr, 
							gObj[uIndex].AccountID, 
							gObj[uIndex].Name, 
							headcode, 
							__FILE__, 
							__LINE__, 
							gObj[uIndex].Connected);
						return FALSE;
					}

					headcode = pDecBuf[2];

					if ( xcode == 0xC4 && headcode == 0xC5 )
					{
						LogAdd("암호패킷 : [0x%x]" , headcode);
					}
					if( IsHackTool(headcode, size, uIndex, xcode) == TRUE )
					{
						return FALSE;
					}
					ProtocolCore(headcode, byDec, iSize, uIndex, 1, subhead);
				}
			}
			else
			{
				CStreamPacketEngine_Server ps;
				ps.Clear();
				
				if ( ps.AddData(recvbuf+lOfs, size) == 0 )
				{
					LogAddC(2, "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
						gObj[uIndex].Ip_addr, 
						gObj[uIndex].AccountID, 
						gObj[uIndex].Name, 
						headcode, 
						__FILE__, __LINE__, 
						gObj[uIndex].Connected);
					return FALSE;
				}

				if ( ps.ExtractPacket(byDec) != 0 )
				{
					LogAddC(2, "error-L1 : CStreamPacketEngine ExtractPacket Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
						gObj[uIndex].Ip_addr, 
						gObj[uIndex].AccountID, 
						gObj[uIndex].Name, 
						headcode, 
						__FILE__, __LINE__, 
						gObj[uIndex].Connected);
					return FALSE;
				}
				if( IsHackTool(headcode, size, uIndex, xcode) == TRUE )
				{
					return FALSE;
				}
				ProtocolCore(headcode, byDec, size, uIndex, 0, -1);
			}

			lOfs += size;
			lpIOContext->nSentBytes  -= size;

			if ( lpIOContext->nSentBytes <= 0 )
			{
				break;
			}
		}
		else if ( lOfs > 0 )
		{
			if ( lpIOContext->nSentBytes < 1 )
			{
				LogAdd("error-L1 : recvbuflen 1 %s %d", __FILE__, __LINE__);
				break;
			}

			if ( lpIOContext->nSentBytes < MAX_IO_BUFFER_SIZE ) 
			{
				memcpy(recvbuf, &recvbuf[lOfs], lpIOContext->nSentBytes);
				LogAdd("Message copy %d", lpIOContext->nSentBytes);
				//break;
			}
			break;
		
		}
		else
		{
			break;
		}
		
	}

	return true;
}

int DataSend(int aIndex, LPBYTE lpMsg, DWORD dwSize)
{
	DWORD SendBytes;
	LPPER_SOCKET_CONTEXT lpPerSocketContext;
	LPBYTE SendBuf;
	

	if ( aIndex < OBJ_STARTUSERINDEX )
	{
		return true;
	}

	EnterCriticalSection(&criti);

	if ( ((aIndex < 0)? FALSE : (aIndex > OBJMAX-1)? FALSE : TRUE )  == FALSE )
	{
		LogAdd("error-L2 : Index(%d) %x %x %x ", dwSize, lpMsg[0], lpMsg[1], lpMsg[2]);
		LeaveCriticalSection(&criti);
		return false;
	}

	if ( lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4 )
	{
		int ret;
		BYTE btsize;

		if ( lpMsg[0] == 0xC3 )
		{
			btsize = lpMsg[1];
			lpMsg[1]=gNSerialCheck[aIndex].GetSendSerial();
			ret = g_SimpleModulusSC.Encrypt(&ExSendBuf[2], &lpMsg[1], dwSize-1);
			ExSendBuf[0] = 0xC3;
			ExSendBuf[1] = ret + 2;
			SendBuf = ExSendBuf;
			dwSize = ret + 2;
			lpMsg[1] = btsize;
		}
		else
		{
			btsize = lpMsg[2];
			lpMsg[2] = gNSerialCheck[aIndex].GetSendSerial();
			ret = g_SimpleModulusSC.Encrypt(&ExSendBuf[3], &lpMsg[2], dwSize-2);
			ExSendBuf[0] = 0xC4;
			ExSendBuf[1] = SET_NUMBERH(ret +3 );
			ExSendBuf[2] = SET_NUMBERL(ret+3);
			SendBuf = ExSendBuf;
			dwSize = ret + 3;
			lpMsg[2] = btsize;
		}
	}
	else
	{
		SendBuf = lpMsg;
	}

	if ( gObj[aIndex].Connected < PLAYER_CONNECTED )
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	lpPerSocketContext= gObj[aIndex].PerSocketContext;

	if ( dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		LogAdd("Error : Max msg(%d) %s %d", dwSize, __FILE__, __LINE__);
		CloseClient(aIndex);
		LeaveCriticalSection(&criti);
		return false;
	}

	LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		if ( ( lpIoCtxt->nSecondOfs + dwSize ) > MAX_IO_BUFFER_SIZE-1 )
		{
			LogAdd("(%d)error-L2 MAX BUFFER OVER %d %d %d [%s][%s]", 
				aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, 
				dwSize, gObj[aIndex].AccountID, gObj[aIndex].Name);

			lpIoCtxt->nWaitIO = 0;
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return true;
		}

		memcpy( &lpIoCtxt->BufferSecond[lpIoCtxt->nSecondOfs], SendBuf, dwSize);
		lpIoCtxt->nSecondOfs += dwSize;
		LeaveCriticalSection(&criti);
		return true;
	}

	lpIoCtxt->nTotalBytes = 0;
	
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}

	if ( (lpIoCtxt->nTotalBytes+dwSize) > MAX_IO_BUFFER_SIZE-1 )
	{
		LogAdd("(%d)error-L2 MAX BUFFER OVER %d %d [%s][%s]", 
			aIndex, lpIoCtxt->nTotalBytes, dwSize, 
			gObj[aIndex].AccountID, gObj[aIndex].Name);

		lpIoCtxt->nWaitIO = 0;
		CloseClient(aIndex);
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	memcpy( &lpIoCtxt->Buffer[lpIoCtxt->nTotalBytes], SendBuf, dwSize);
	lpIoCtxt->nTotalBytes += dwSize;
	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = SEND_IO;

	if ( WSASend( gObj[aIndex].m_socket, &lpIoCtxt->wsabuf , 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{

		if ( WSAGetLastError() != WSA_IO_PENDING )	
		{
			lpIoCtxt->nWaitIO = 0;
			

			if ( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", 
					__LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			else if ( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", 
					__LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[3], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return true;
}





int IoSendSecond(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{
	DWORD SendBytes;
	int aIndex;
	EnterCriticalSection(&criti);
	aIndex = lpPerSocketContext->nIndex;
	LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->nTotalBytes = 0;
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}
	else
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = SEND_IO;

	if ( WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].Ip_addr);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	
	return true;
}


int IoMoreSend(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{
	DWORD SendBytes;
	int aIndex;
	
	EnterCriticalSection(&criti);
	aIndex = lpPerSocketContext->nIndex;
	LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

	if ( (lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0 )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer[lpIoCtxt->nSentBytes];
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation = SEND_IO;

	if ( WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].Ip_addr);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return true;
}


BOOL UpdateCompletionPort(SOCKET sd, int ClientIndex, BOOL bAddToList)
{
	LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL;

	HANDLE cp = CreateIoCompletionPort((HANDLE)sd, g_CompletionPort, (DWORD)ClientIndex, 0); 
    
	if (cp == NULL)
	{
		LogAdd("CreateIoCompletionPort: %d", GetLastError() );
		return FALSE;
	}

	gObj[ClientIndex].PerSocketContext->dwIOCount = 0;
	return TRUE;
}


void CloseClient ( LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful )
{
	int index = -1;
	index = lpPerSocketContext->nIndex ;

	if ( index >= OBJ_STARTUSERINDEX && index < OBJMAX )
	{
#ifdef __CUSTOMS__
		if( g_OfflineTrade.Update(index, 0) )
		{
			return;
		}
#endif

#ifdef OFFEXP
		if( OffExp.Update(index, 0) )
		{
			return;
		}
#endif

#ifdef __MAKELO__
		if( g_OfflineAttack.CharInfo[index].IsOffline )
		{
			gObj[index].CheckTick = GetTickCount();
			return; 
		}
#endif
		if ( gObj[index].m_socket != INVALID_SOCKET )
		{
			if (closesocket(gObj[index].m_socket) == -1 )
			{
				if ( WSAGetLastError() != 10038 )
				{
					return;
				}
			}

			gObj[index].m_socket = INVALID_SOCKET;
		}

		gObjDel(index);
	}
}

void CloseClient(int index)
{
	if ( index < 0 || index > OBJMAX-1 )
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}

	if ( gObj[index].Connected == PLAYER_EMPTY )
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}

#ifdef __CUSTOMS__
	if( g_OfflineTrade.Update(index, 0) )
	{
		return;
	}
#endif

#ifdef OFFEXP
	if( OffExp.Update(index, 0) )
	{
		return;
	}
#endif

#ifdef __MAKELO__
	if( g_OfflineAttack.CharInfo[index].IsOffline )
	{	
		gObj[index].CheckTick = GetTickCount();
		return; 
	}
#endif

	EnterCriticalSection(&criti);

	if ( gObj[index].m_socket != INVALID_SOCKET )
	{
		closesocket(gObj[index].m_socket );
		gObj[index].m_socket = INVALID_SOCKET;
	}
	else
	{
		LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	LeaveCriticalSection(&criti);
}

void CloseClientEx(int index)
{
	if ( index < 0 || index > OBJMAX-1 )
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}

	if ( gObj[index].Connected == PLAYER_EMPTY )
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}
	
	EnterCriticalSection(&criti);

	if ( gObj[index].m_socket != INVALID_SOCKET )
	{
		closesocket(gObj[index].m_socket );
		gObj[index].m_socket = INVALID_SOCKET;
	}
	else
	{
		LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	LeaveCriticalSection(&criti);
}

void ResponErrorCloseClient(int index)
{
	if ( index < 0 || index > OBJMAX-1 )
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}

	if ( gObj[index].Connected == PLAYER_EMPTY )
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}
	
#ifdef __CUSTOMS__
	if( g_OfflineTrade.Update(index, 0) )
	{
		return;
	}
#endif

#ifdef OFFEXP
	if( OffExp.Update(index, 0) )
	{
		return;
	}
#endif

#ifdef __MAKELO__
	if( g_OfflineAttack.CharInfo[index].IsOffline )
	{	
		gObj[index].CheckTick = GetTickCount();
		return; 
	}
#endif

	EnterCriticalSection(&criti);
	closesocket(gObj[index].m_socket);
	gObj[index].m_socket = INVALID_SOCKET;

	if ( gObj[index].m_socket == INVALID_SOCKET )
	{
		LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	gObjDel(index);
	LeaveCriticalSection(&criti);
}