#include "stdafx.h"
#include <stdio.h>
#include "Prodef.h"
#include <winsock2.h>
#include "Protocol.h"
#include "DelayHandler.h"
#include "WzQueue.h"
#include "giocp.h"
#include "ServerPrc.h"
#include "WZIPCheck.h"
#include "Settings.h"

enum {
	RECV_IO,
	SEND_IO
} SOCKET_FLAG;


HANDLE			g_CompletionPort=NULL;

DWORD           g_dwThreadCount = 0;        //worker thread count 

WZQueue	WzRecvQ2;
WZQueue	WzRecvQ3;
WZQueue	WzRecvQ4;

CRITICAL_SECTION criti;

WZQueue	WzSendQ;
WZQueue	WzRecvQ;

HANDLE          g_ThreadHandles[MAX_WORKER_THREAD]; 

CRITICAL_SECTION    g_CriticalSection;      // guard access to the global context list 

LPPER_SOCKET_CONTEXT	g_pCtxtList = NULL;
LPPER_SOCKET_CONTEXT	g_pCtxtListHead = NULL;

int		IoSend(int aIndex, LPBYTE lpMsg, DWORD dwSize);
BOOL	RecvDataParse(LPPER_IO_CONTEXT	lpIOContext, int uIndex);
void	CloseClient ( LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful ) ;

DWORD	WINAPI IocpServerWorker(LPVOID p);
DWORD	WINAPI ServerRecvWorkerThread(LPVOID p);
DWORD   WINAPI ServerWorkerThread(LPVOID CompletionPortID);
DWORD   WINAPI ServerRecvWorkerThreadSecond(LPVOID p);
DWORD   WINAPI ServerRecvWorkerThreadThird(LPVOID p);
DWORD   WINAPI ServerRecvWorkerThreadFourth(LPVOID p);

LPPER_SOCKET_CONTEXT ContextAllocate(SOCKET sd, int ClientIndex);
void CtxtListAddTo(LPPER_SOCKET_CONTEXT lpPerSocketContext);
BOOL CtxtListDeleteFrom(LPPER_SOCKET_CONTEXT lpPerSocketContext);
void CtxtListFree();

SOCKET			g_Listen = INVALID_SOCKET;
int				g_ServerPort;

int CreateGIocp(int server_port)
{
	DWORD	ThreadID;
	HANDLE	ThreadHandle;

	g_ServerPort = server_port;
	if ((ThreadHandle = CreateThread(NULL, 0, IocpServerWorker, (LPVOID)NULL,
			0, &ThreadID)) == NULL)
	{
		gWindow.PrintLog("CreateThread() failed with error %d", GetLastError());
		return FALSE;
	}

	CloseHandle(ThreadHandle );
	return TRUE;
}

BOOL CreateListenSocket(void) 
{	
	SOCKADDR_IN InternetAddr;
	int			nRet;

	g_Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_Listen == INVALID_SOCKET)
	{
		gWindow.PrintLog("WSASocket() failed with error %d", WSAGetLastError());
		return FALSE;
	} 
	
	InternetAddr.sin_family			= AF_INET;
	InternetAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	InternetAddr.sin_port			= htons(g_ServerPort);
	
	nRet = bind(g_Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr));
	if( nRet == SOCKET_ERROR )
	{
		gWindow.PrintLog("bind() failed with error %d", WSAGetLastError());
		return FALSE;
	}

	nRet = listen(g_Listen, 5);

	if (nRet == SOCKET_ERROR)
	{
		gWindow.PrintLog("listen() failed with error %d", WSAGetLastError());
		return FALSE;
	} 
	return TRUE;
}

DWORD WINAPI IocpServerWorker(LPVOID p)
{
	SYSTEM_INFO SystemInfo;
	DWORD		ThreadID;
	SOCKET		Accept;
	LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL;
	int			nRet;
	int			ClientIndex;
	SOCKADDR_IN cAddr;
	IN_ADDR		cInAddr;
	int			cAddrlen = sizeof( cAddr );

	DWORD RecvBytes;
	DWORD Flags=0;
		
	InitializeCriticalSection(&criti);

	GetSystemInfo(&SystemInfo);
	g_dwThreadCount = SystemInfo.dwNumberOfProcessors * 2;

	if( g_dwThreadCount > gSettings.MaxThreadCount) //webzen season 6.2 fix
	{
		g_dwThreadCount = gSettings.MaxThreadCount;
		gWindow.PrintLog(" **** Worker Thread Max Count Fix 16ea ****");
	}
	
    __try  
    { 
		g_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if ( g_CompletionPort == NULL)
		{
			gWindow.PrintLog("CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}
		
		for(DWORD dwCPU = 0; dwCPU < g_dwThreadCount; dwCPU++)
		{
			HANDLE ThreadHandle;
			// Create a server worker thread and pass the completion port to the thread.
			
			ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, g_CompletionPort, 0, &ThreadID);
			if ( ThreadHandle == NULL)
			{
				gWindow.PrintLog("CreateThread() failed with error %d", GetLastError());
				__leave;
			}
			g_ThreadHandles[dwCPU] = ThreadHandle;
		}

		HANDLE ThreadHandle; //ebp-7c
		ThreadHandle = CreateThread(NULL, 0, ServerRecvWorkerThread, (LPVOID)NULL,0, &ThreadID);
		if (ThreadHandle == NULL)
		{
			gWindow.PrintLog("CreateThread() failed with error %d", GetLastError());
			__leave;
		}

		if(SetThreadPriority(ThreadHandle, THREAD_PRIORITY_HIGHEST) == FALSE) //webzen season 6.2 add-on
		{
			gWindow.PrintLog("SetThreadPriority() failed with error %d", GetLastError());
		}

		ThreadHandle = CreateThread(NULL, 0, ServerRecvWorkerThreadSecond, (LPVOID)NULL, 0, &ThreadID); //webzen season6.2 add-on
		if (ThreadHandle == NULL)
		{
			gWindow.PrintLog("RecvWorkerthread Second CreateThread() failed with error %d", GetLastError());
			__leave;
		}

		ThreadHandle = CreateThread(NULL, 0, ServerRecvWorkerThreadThird, (LPVOID)NULL, 0, &ThreadID); //webzen season6.2 add-on
		if (ThreadHandle == NULL)
		{
			gWindow.PrintLog("RecvWorkerthread Third CreateThread() failed with error %d", GetLastError());
			__leave;
		}

		ThreadHandle = CreateThread(NULL, 0, ServerRecvWorkerThreadFourth, (LPVOID)NULL, 0, &ThreadID); //webzen season6.2 add-on
		if (ThreadHandle == NULL)
		{
			gWindow.PrintLog("RecvWorkerthread Fourth CreateThread() failed with error %d", GetLastError());
			__leave;
		}
		
		if (!CreateListenSocket() )
			__leave;

		while(TRUE)		
		{
			Accept = WSAAccept(g_Listen, (LPSOCKADDR)&cAddr, &cAddrlen, NULL, 0);
			if (Accept==SOCKET_ERROR)
			{
				EnterCriticalSection(&criti);
				gWindow.PrintLog("WSAAccept() failed with error %d", WSAGetLastError());
				LeaveCriticalSection(&criti);
				__leave;
			}

			int SocketData = MSG_PARTIAL;

			if(setsockopt(Accept, SOL_SOCKET, SO_RCVBUF, (char*)&SocketData, 4) == SOCKET_ERROR) //webzen season6.2 add-on
			{
				EnterCriticalSection(&criti);
				gWindow.PrintLog("setsockopt() failed with error %d", WSAGetLastError());
				LeaveCriticalSection(&criti);
				__leave;
			}

			EnterCriticalSection(&criti);

			memcpy( &cInAddr, &cAddr.sin_addr.s_addr, 4 );
		
			if(g_WZIPCheck.CheckIP(inet_ntoa(cInAddr)) == FALSE) //webzen season6.2 add-on
			{
				closesocket(Accept);
				LeaveCriticalSection(&criti);
				gWindow.PrintLog("[ANTI-HACKING] Attempted to connect illegal IP : %d.%d.%d.%d ", cInAddr.S_un.S_un_b.s_b1, cInAddr.S_un.S_un_b.s_b2, cInAddr.S_un.S_un_b.s_b3, cInAddr.S_un.S_un_b.s_b4);
				continue;
			}
			
			ClientIndex = gSObjAdd(Accept, inet_ntoa(cInAddr));

			gWindow.PrintLog("UserIndex [%d] Connected IP : %d.%d.%d.%d ", ClientIndex, cInAddr.S_un.S_un_b.s_b1, cInAddr.S_un.S_un_b.s_b2, cInAddr.S_un.S_un_b.s_b3, cInAddr.S_un.S_un_b.s_b4);
			
			if( ClientIndex == -1 ) 
			{
				gWindow.PrintLog("Error : Max User Index");
				closesocket(Accept);
				LeaveCriticalSection(&criti);
				continue;
			}

			gWindow.PrintLog("Socket number %d connected", Accept);

			lpPerSocketContext = UpdateCompletionPort(Accept, ClientIndex, TRUE);
			if( lpPerSocketContext == FALSE )
			{ 
				gWindow.PrintLog("CreateIoCompletionPort failed with error %d", GetLastError());				
				LeaveCriticalSection(&criti);
				__leave;
			}
			
			gSObj[ClientIndex].lpPerSocketContext = lpPerSocketContext;

			lpPerSocketContext->nIndex = ClientIndex;

			nRet = WSARecv(Accept, &(lpPerSocketContext->IOContext[0].wsabuf), 1, &RecvBytes, &Flags,
				&(lpPerSocketContext->IOContext[0].Overlapped), NULL);
			
			if( nRet == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING )
			{
				gWindow.PrintLog("WSARecv() failed with error %d", WSAGetLastError());
				//gSObj[ClientIndex].lpPerSocketContext->IOContext[0].nWaitIO = 4;
				CloseClient(lpPerSocketContext, FALSE);
				LeaveCriticalSection(&criti);
				continue;
			}
			else
			{
				//gSObj[ClientIndex].lpPerSocketContext->IOContext[0].nWaitIO = 1;
				lpPerSocketContext->dwIOCount++; //gSObj[ClientIndex].lpPerSocketContext->dwIOCount++;
			}
			LeaveCriticalSection(&criti);
		}	// while
	}	// __try
	__finally  
	{
		if( g_CompletionPort )
		{
			for (DWORD i = 0; i < g_dwThreadCount; i++) 
					PostQueuedCompletionStatus(g_CompletionPort, 0, 0, NULL); 
		}

		CtxtListFree();

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

DWORD WINAPI ServerRecvWorkerThread(LPVOID p)
{
	BYTE RecvData[MAX_BUFF_SIZE]; //0
	unsigned int nSize=0; //4
	BYTE headcode; //8
	int  uindex; //c
	int	 loopN; //10

	int  iSessionId; //14
	bool bProcessPacket; //18

	int  nCount=0; //1C

	while(TRUE)
	{
		EnterCriticalSection(&criti);

		loopN = WzRecvQ.GetCount();	

		if( loopN > MAX_NODE-1 )
		{
			loopN = MAX_NODE-1;
			gWindow.PrintLog("error : Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti);
		if( loopN > 0 )
		{
			//DWORD dwTime = GetTickCount();
			//for( int n=0; n<loopN; n++)
			{
				if( WzRecvQ.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE )
				{
					bProcessPacket = true;
					if(iSessionId != g_DelayHandler.GetQuerySessionId())
					{
						if(g_DelayHandler.CheckHeadCode(headcode) == false)
						{
							bProcessPacket = false;
							gWindow.PrintLog("[DelayHandler] Drop Delayed Packet -> Head : %X", headcode);
						}
						else
						{
							gWindow.PrintLog("[DelayHandler] Process Delayed Packet -> Head : %X", headcode);
						}
					}
					if(bProcessPacket != false)
					{
						ProtocolCore(headcode, RecvData, nSize, uindex);
					}
				}
			}
			//gWindow.PrintLog("Process End : [%d]", GetTickCount() - dwTime);
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID)
{
	HANDLE	CompletionPort = (HANDLE) CompletionPortID; //loc1
	DWORD	dwIoSize; //loc2
	DWORD	RecvBytes; //loc3
	DWORD	Flags; //loc4
	DWORD	dwSendNumBytes = 0; //loc5
	BOOL	bSuccess = FALSE; //loc6 
	int		nRet; //loc7
	//int     ClientIndex;

	LPPER_SOCKET_CONTEXT	lpPerSocketContext = NULL; //loc8
	LPOVERLAPPED			lpOverlapped = NULL; //loc9
	LPPER_IO_CONTEXT		lpIOContext = NULL; //loc10

	while(TRUE)
	{
		bSuccess = GetQueuedCompletionStatus(
			CompletionPort,
			&dwIoSize,
			(LPDWORD)&lpPerSocketContext,
			&lpOverlapped,
			INFINITE
		);

		if( !bSuccess )
		{
			EnterCriticalSection(&criti);
			gWindow.PrintLog("GetQueuedCompletionStatus failed with error %d", GetLastError() );
			CloseClient(lpPerSocketContext, FALSE);
			LeaveCriticalSection(&criti);
			continue;
		}

		if(lpPerSocketContext == NULL)
		{
			return FALSE;
		}

		EnterCriticalSection(&criti);
		
		lpPerSocketContext->dwIOCount--;
		if( bSuccess == FALSE || bSuccess != FALSE && dwIoSize == 0 )
		{
			gWindow.PrintLog("GetQueuedCompletionStatus dwIoSize is zero (%d)", GetLastError() );
			CloseClient(lpPerSocketContext, FALSE);
			LeaveCriticalSection(&criti);
			continue;
		}

		lpIOContext = (LPPER_IO_CONTEXT)lpOverlapped;

		if( lpIOContext->IOOperation == SEND_IO )
		{
			/*lpIOContext->nSentBytes += dwIoSize;

			if( lpIOContext->nSentBytes >= lpIOContext->nTotalBytes )
			{
				lpIOContext->nWaitIO = 0;
				if( lpIOContext->nSecondOfs > 0 )
				{
					//IoSend(lpPerSocketContext);
				}
			}
			else
			{
				//IoSend(lpPerSocketContext);
			}*/
		}
		else if( lpIOContext->IOOperation == RECV_IO )
		{
			RecvBytes = 0;

			lpIOContext->nSentBytes += dwIoSize;
			
			RecvDataParse(lpIOContext, lpPerSocketContext->nIndex);

			//lpIOContext->nWaitIO = 0;
			Flags = 0;
			ZeroMemory(&(lpIOContext->Overlapped), sizeof(OVERLAPPED));
			
			lpIOContext->wsabuf.len		= MAX_BUFF_SIZE-lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf		= lpIOContext->Buffer+lpIOContext->nSentBytes;
			lpIOContext->IOOperation	= RECV_IO;
			
			nRet = WSARecv(lpPerSocketContext->m_socket, &(lpIOContext->wsabuf), 1, &RecvBytes, &Flags,
				&(lpIOContext->Overlapped), NULL);
			if( nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING) )
			{
				gWindow.PrintLog("WSARecv() failed with error %d", WSAGetLastError());
				//lpIOContext->nWaitIO = 2;
				CloseClient( lpPerSocketContext, FALSE);
				LeaveCriticalSection(&criti);
				continue;
			}
			else
			{	
				lpPerSocketContext->dwIOCount++;
				//lpIOContext->nWaitIO = 1;
			}
		}
		LeaveCriticalSection(&criti);
	}
	//return TRUE;
}

BOOL RecvDataParse(LPPER_IO_CONTEXT	lpIOContext, int uIndex)
{
	if( lpIOContext->nSentBytes < 3 ) return FALSE;
	
	LPBYTE	recvbuf;
	int		lOfs=0;
	int		size=0;
	BYTE	headcode;
	
	recvbuf = (LPBYTE)lpIOContext->Buffer;

	while(1)
	{
		if( recvbuf[lOfs] == 0xC1 ) //
		{
			LPPBMSG_HEAD lphead = (LPPBMSG_HEAD)(recvbuf+lOfs);
			size				= lphead->size;
			headcode			= lphead->headcode;
		}
		else if( recvbuf[lOfs] == 0xC2 ) //
		{
			LPPWMSG_HEAD lphead	= (LPPWMSG_HEAD)(recvbuf+lOfs);
			size				= ((WORD)(lphead->sizeH)<<8);
			size			   |= (WORD)(lphead->sizeL);
			headcode			= lphead->headcode;
		}
		else  // 헤더가 맞지 않다면..
		{
			gWindow.PrintLog("error-L1 : Header error (%s %d)lOfs:%d, size:%d", __FILE__, __LINE__, lOfs, lpIOContext->nSentBytes);
			lpIOContext->nSentBytes = 0;
			return FALSE;
		}
		if( size <= 0 )
		{
			gWindow.PrintLog("error : size가 %d이다.", size);
			return FALSE;
		}
		if( size <= lpIOContext->nSentBytes )
		{
			switch(headcode)
			{
			case 0xF8:	//-> Gens
			case 0xD1:
				{
					WzRecvQ4.AddToQueue((LPBYTE)(recvbuf+lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}
				break;
				// --
			case 0x52:	//-> GDItemCreate
			case 0x54:	//-> GDPropertyItemCreate
			case 0x55:	//-> GDPetItemCreate
				{
					WzRecvQ3.AddToQueue((LPBYTE)(recvbuf+lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}
				break;
				// --
			case 0xD0:	//-> CashShop
			case 0xD2:
				{
					WzRecvQ2.AddToQueue((LPBYTE)(recvbuf+lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}
				break;
				// --
			default:	//-> Other
				{
					WzRecvQ.AddToQueue((LPBYTE)(recvbuf+lOfs), size, headcode, uIndex, g_DelayHandler.GetQuerySessionId());
				}
				break;
			}
			// ----
			lOfs += size;
			lpIOContext->nSentBytes -= size;			
			if( lpIOContext->nSentBytes <= 0 ) break;
		}			
		else 
		{
			if( lOfs > 0 )
			{
				if( lpIOContext->nSentBytes < 1 ) 
				{
					gWindow.PrintLog("error : recvbuflen 이 1보다 작다..");
					break;
				}
				else 
				{
					if( lpIOContext->nSentBytes < MAX_BUFF_SIZE )
					{
						memcpy(recvbuf, (recvbuf+lOfs), lpIOContext->nSentBytes); // 남은만큼 복사한다.
						gWindow.PrintLog("error : copy %d",lpIOContext->nSentBytes);
					}
					break;
				}
			}
			break;
		}
	}
	return 0;
}

int IoSend(int aIndex, LPBYTE lpMsg, DWORD dwSize)
{	
	EnterCriticalSection(&criti);
	DWORD SendBytes;
	LPPER_SOCKET_CONTEXT lpPerSocketContext;
	/*if( gSObj[aIndex].Connected < 1 )
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}
	lpPerSocketContext = gSObj[aIndex].lpPerSocketContext;*/
	if( dwSize > MAX_BUFF_SIZE )
	{
		gWindow.PrintLog("Error : Max msg %s %d", __FILE__, __LINE__);
		LeaveCriticalSection(&criti);
		//CloseClient( aIndex );
		return FALSE;
	}

	if( gSObj[aIndex].Connected < 1 )
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	lpPerSocketContext = gSObj[aIndex].lpPerSocketContext;

	if(lpPerSocketContext == NULL)
	{
		gWindow.PrintLog("error : user socket context buffer free.");
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

	lpIoCtxt->wsabuf.buf = (char*)lpMsg;
	lpIoCtxt->wsabuf.len = dwSize;

	/*if( lpIoCtxt->nWaitIO > 0 )	// 쓰기 작업이 아직 완료되지 않았다면.. 세컨트로 복사
	{	
		if( (lpIoCtxt->nSecondOfs+dwSize) > MAX_BUFF_SIZE-1)	// 크기를 넘었다.
		{
			gWindow.PrintLog("error-L2 MAX BUFFER OVER");
			lpIoCtxt->nWaitIO = 0;
			CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return TRUE;
		}
		else
		{
			// 세컨드를 일단 복사..
			memcpy(lpIoCtxt->BufferSecond+lpIoCtxt->nSecondOfs, lpMsg, dwSize);
			lpIoCtxt->nSecondOfs+=dwSize;
			//gWindow.PrintLog("세컨드 복사.. %d+=%d (%s)", lpIoCtxt->nSecondOfs, dwSize, gSObj[aIndex].AccountID);
			LeaveCriticalSection(&criti);
			return TRUE;
		}
	}
	else
	{
		lpIoCtxt->nTotalBytes = 0;
		if( lpIoCtxt->nSecondOfs > 0 )
		{
			memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
			lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
			//gWindow.PrintLog("프론트로 복사.. %d", lpIoCtxt->nTotalBytes);
			lpIoCtxt->nSecondOfs = 0;
		}
		if( (lpIoCtxt->nTotalBytes+dwSize) > MAX_BUFF_SIZE-1)	// 크기를 넘었다.
		{
			gWindow.PrintLog("error-L2 MAX BUFFER OVER");
			lpIoCtxt->nWaitIO = 0;
			CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return FALSE;
		}
		else
		{
			memcpy(lpIoCtxt->Buffer+lpIoCtxt->nTotalBytes, lpMsg, dwSize);
			lpIoCtxt->nTotalBytes += dwSize;
			//gWindow.PrintLog("메시지 복사.. %d", lpIoCtxt->nTotalBytes);
		}
	}
	// 쓰기작업이 완료되었다면..

	lpIoCtxt->wsabuf.buf	= (char*)lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes;*/
	lpIoCtxt->nTotalBytes	= dwSize;
	lpIoCtxt->nSentBytes	= 0;
	lpIoCtxt->IOOperation	= SEND_IO;

	if (WSASend(gSObj[aIndex].m_socket, &(lpIoCtxt->wsabuf), 1, &SendBytes, 0,
		&(lpIoCtxt->Overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			gWindow.PrintLog("WSASend() failed with error %d", WSAGetLastError());			
			//lpIoCtxt->nWaitIO = 3;
			//CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return FALSE;
		}
	}
	else 
	{
		lpPerSocketContext->dwIOCount++;
	}
	//lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return TRUE;
}

LPPER_SOCKET_CONTEXT UpdateCompletionPort( SOCKET sd, int ClientIndex, BOOL bAddToList ) 
{
    LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL;

	lpPerSocketContext = ContextAllocate(sd, ClientIndex);

	if(lpPerSocketContext == NULL)
	{
		return FALSE;
	}
	
    HANDLE cp = CreateIoCompletionPort((HANDLE)sd, g_CompletionPort, (DWORD)lpPerSocketContext, 0); 
    
	if (cp == NULL)
	{
        gWindow.PrintLog("CreateIoCompletionPort: %d", GetLastError());
		HeapFree(GetProcessHeap(), 0, lpPerSocketContext);
        return FALSE;
	}

	if(bAddToList != FALSE)
	{
		CtxtListAddTo(lpPerSocketContext);
	}
	//gSObj[ClientIndex].lpPerSocketContext->dwIOCount = 0;
    return lpPerSocketContext; 
}

LPPER_SOCKET_CONTEXT ContextAllocate(SOCKET sd, int ClientIndex)
{ //completed
	LPPER_SOCKET_CONTEXT lpPerSocketContext;

	EnterCriticalSection(&criti);

	lpPerSocketContext = (LPPER_SOCKET_CONTEXT)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(_PER_SOCKET_CONTEXT));
	lpPerSocketContext->WzSendQ	= new WZQueue();

	if(lpPerSocketContext != NULL)
	{
		lpPerSocketContext->m_socket = sd;
		lpPerSocketContext->nIndex = ClientIndex;

		lpPerSocketContext->lpCtxtBack = NULL;
		lpPerSocketContext->lpCtxtForward = NULL;

		lpPerSocketContext->IOContext[RECV_IO].Overlapped.Internal = 0;
		lpPerSocketContext->IOContext[RECV_IO].Overlapped.InternalHigh = 0;
		lpPerSocketContext->IOContext[RECV_IO].Overlapped.Offset = 0;
		lpPerSocketContext->IOContext[RECV_IO].Overlapped.OffsetHigh = 0;
		lpPerSocketContext->IOContext[RECV_IO].Overlapped.hEvent = NULL;
		lpPerSocketContext->IOContext[RECV_IO].IOOperation = RECV_IO;
		lpPerSocketContext->IOContext[RECV_IO].pIOContextForward = NULL;
		lpPerSocketContext->IOContext[RECV_IO].nTotalBytes = 0;
		lpPerSocketContext->IOContext[RECV_IO].nSentBytes = 0;
		lpPerSocketContext->IOContext[RECV_IO].wsabuf.buf = lpPerSocketContext->IOContext[RECV_IO].Buffer;
		lpPerSocketContext->IOContext[RECV_IO].wsabuf.len = MAX_BUFF_SIZE;
		
		lpPerSocketContext->IOContext[SEND_IO].Overlapped.Internal = 0;
		lpPerSocketContext->IOContext[SEND_IO].Overlapped.InternalHigh = 0;
		lpPerSocketContext->IOContext[SEND_IO].Overlapped.Offset = 0;
		lpPerSocketContext->IOContext[SEND_IO].Overlapped.OffsetHigh = 0;
		lpPerSocketContext->IOContext[SEND_IO].Overlapped.hEvent = NULL;		
		lpPerSocketContext->IOContext[SEND_IO].IOOperation = SEND_IO;
		lpPerSocketContext->IOContext[SEND_IO].pIOContextForward = 0;
		lpPerSocketContext->IOContext[SEND_IO].nTotalBytes = 0;
		lpPerSocketContext->IOContext[SEND_IO].nSentBytes = 0;
		lpPerSocketContext->IOContext[SEND_IO].wsabuf.buf = lpPerSocketContext->IOContext[SEND_IO].Buffer;
		lpPerSocketContext->IOContext[SEND_IO].wsabuf.len = MAX_BUFF_SIZE;
	}
	else
	{
		gWindow.PrintLog("HeapAlloc PER_SOCKET_CONTEXT Failed: %d", GetLastError());
	}

	LeaveCriticalSection(&criti);

	return lpPerSocketContext;
}

void CtxtListAddTo(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{ //completed
	LPPER_SOCKET_CONTEXT pTemp;

	EnterCriticalSection(&criti);

	if(g_pCtxtList == NULL)
	{
		lpPerSocketContext->lpCtxtBack = NULL;
		lpPerSocketContext->lpCtxtForward = NULL;
		g_pCtxtList = lpPerSocketContext;
		g_pCtxtListHead = lpPerSocketContext;
	}
	else
	{
		pTemp = g_pCtxtList;
		g_pCtxtList = lpPerSocketContext;
		lpPerSocketContext->lpCtxtBack = pTemp;
		lpPerSocketContext->lpCtxtForward = NULL;
		pTemp->lpCtxtForward = lpPerSocketContext;
	}

	LeaveCriticalSection(&criti);
}

BOOL CtxtListDeleteFrom(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{ //completed
	LPPER_SOCKET_CONTEXT	pBack; //loc1
	LPPER_SOCKET_CONTEXT	pForward; //loc2

	LPPER_IO_CONTEXT	pNextIO = NULL; //loc3
	LPPER_IO_CONTEXT	pTempIO = NULL; //loc4

	BYTE TempBuf[MAX_BUFF_SIZE];
	UINT nSize;
	BYTE headcode;
	int uindex;
	int iSessionId;

	if(lpPerSocketContext->dwIOCount > 0)
	{
		return FALSE;
	}

	EnterCriticalSection(&criti);

	if(lpPerSocketContext != NULL)
	{
		pBack = lpPerSocketContext->lpCtxtBack;
		pForward = lpPerSocketContext->lpCtxtForward;

		if(pBack == NULL && pForward == NULL)
		{
			g_pCtxtList = NULL;
			g_pCtxtListHead = NULL;
		}
		else if(pBack == NULL && pForward != NULL)
		{
			pForward->lpCtxtBack = NULL;
			g_pCtxtList = pForward;
		}
		else if(pBack != NULL && pForward == NULL)
		{
			pBack->lpCtxtForward = NULL;
			g_pCtxtList = pBack;
		}
		else if(pBack != NULL && pForward != NULL)
		{
			pBack->lpCtxtForward = pForward;
			pForward->lpCtxtBack = pBack;
		}

		while( lpPerSocketContext->WzSendQ->GetFromQueue(TempBuf, &nSize, &headcode, &uindex, &iSessionId) != 0 )
		{
			continue;
		}

		delete lpPerSocketContext->WzSendQ;

		HeapFree(GetProcessHeap(), 0, lpPerSocketContext);
		lpPerSocketContext = NULL;
	}
	else
	{
		gWindow.PrintLog("CtxtListDeleteFrom: lpPerSocketContext is NULL");
	}

	LeaveCriticalSection(&criti);

	return TRUE;
}

void CtxtListFree()
{ //completed
	EnterCriticalSection(&criti);

	LPPER_SOCKET_CONTEXT pTemp1 = g_pCtxtList; //loc1
	LPPER_SOCKET_CONTEXT pTemp2; //loc2

	while(pTemp1 != 0)
	{
		pTemp2 = pTemp1->lpCtxtBack;
		pTemp1 = pTemp2;
	}
	
	LeaveCriticalSection(&criti);
}

void CloseClient ( LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful ) 
{ 
	if(lpPerSocketContext == NULL ) return; //HermeX Fix

	int index=lpPerSocketContext->nIndex;
	CloseClient(index);
}

void CloseClient ( int index ) ///LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful ) 
{
	if( index < 0 || index > MAX_OBJECT-1 ) return;
	if( gSObj[index].Connected == 0 ) return;

	EnterCriticalSection(&criti);

	LPPER_SOCKET_CONTEXT lpPerSocketContext = gSObj[index].lpPerSocketContext;

	if(lpPerSocketContext != NULL)
	{
		index = lpPerSocketContext->nIndex;

		if(lpPerSocketContext->m_socket != INVALID_SOCKET)
		{
			closesocket(lpPerSocketContext->m_socket);
			lpPerSocketContext->m_socket = INVALID_SOCKET;
		}

		if(CtxtListDeleteFrom(lpPerSocketContext) == TRUE)
		{
			lpPerSocketContext = NULL;
			gSObj[index].lpPerSocketContext = NULL;
		}
	}
	else
	{
		gWindow.PrintLog("CloseClient: lpPerSocketContext is NULL");
	}

	if(index >= 0)
	{
		gWindow.PrintLog("CloseClient(%d)", index);
		gSObjDel(index);
	}

	LeaveCriticalSection(&criti);

//	closesocket(gSObj[index].m_socket);
//
//	gSObj[index].m_socket = INVALID_SOCKET;
//	gWindow.PrintLog("error-L2:CloseClient %d (강제종료??)", index);

    return;
}

DWORD WINAPI ServerRecvWorkerThreadSecond(LPVOID p)
{
	BYTE RecvData[MAX_BUFF_SIZE]; //0
	unsigned int nSize=0; //4
	BYTE headcode; //8
	int  uindex; //c
	int	 loopN; //10

	int  iSessionId; //14
	bool bProcessPacket; //18

	int  nCount=0; //1C

	while(TRUE)
	{
		EnterCriticalSection(&criti);

		loopN = WzRecvQ2.GetCount();	

		if( loopN > MAX_NODE-1 )
		{
			loopN = MAX_NODE-1;
			gWindow.PrintLog("error : Q2 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti);
		if( loopN > 0 )
		{
			//DWORD dwTime = GetTickCount();
			//for( int n=0; n<loopN; n++)
			{
				if( WzRecvQ2.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE )
				{
					bProcessPacket = true;
					if(iSessionId != g_DelayHandler.GetQuerySessionId())
					{
						if(g_DelayHandler.CheckHeadCode(headcode) == false)
						{
							bProcessPacket = false;
							gWindow.PrintLog("[DelayHandler] Drop Delayed Packet -> Head : %X", headcode);
						}
						else
						{
							gWindow.PrintLog("[DelayHandler] Process Delayed Packet -> Head : %X", headcode);
						}
					}
					if(bProcessPacket != false)
					{
						ProtocolCore(headcode, RecvData, nSize, uindex);
					}
				}
			}
			//gWindow.PrintLog("Process End : [%d]", GetTickCount() - dwTime);
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThreadThird(LPVOID p)
{
	BYTE RecvData[MAX_BUFF_SIZE]; //0
	unsigned int nSize=0; //4
	BYTE headcode; //8
	int  uindex; //c
	int	 loopN; //10

	int  iSessionId; //14
	bool bProcessPacket; //18

	int  nCount=0; //1C

	while(TRUE)
	{
		EnterCriticalSection(&criti);

		loopN = WzRecvQ3.GetCount();	

		if( loopN > MAX_NODE-1 )
		{
			loopN = MAX_NODE-1;
			gWindow.PrintLog("error : Q3 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti);
		if( loopN > 0 )
		{
			//DWORD dwTime = GetTickCount();
			//for( int n=0; n<loopN; n++)
			{
				if( WzRecvQ3.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE )
				{
					bProcessPacket = true;
					if(iSessionId != g_DelayHandler.GetQuerySessionId())
					{
						if(g_DelayHandler.CheckHeadCode(headcode) == false)
						{
							bProcessPacket = false;
							gWindow.PrintLog("[DelayHandler] Drop Delayed Packet -> Head : %X", headcode);
						}
						else
						{
							gWindow.PrintLog("[DelayHandler] Process Delayed Packet -> Head : %X", headcode);
						}
					}
					if(bProcessPacket != false)
					{
						ProtocolCore(headcode, RecvData, nSize, uindex);
					}
				}
			}
			//gWindow.PrintLog("Process End : [%d]", GetTickCount() - dwTime);
		}
		Sleep(1);
	}
	return TRUE;
}

DWORD WINAPI ServerRecvWorkerThreadFourth(LPVOID p)
{
	BYTE RecvData[MAX_BUFF_SIZE]; //0
	unsigned int nSize=0; //4
	BYTE headcode; //8
	int  uindex; //c
	int	 loopN; //10

	int  iSessionId; //14
	bool bProcessPacket; //18

	int  nCount=0; //1C

	while(TRUE)
	{
		EnterCriticalSection(&criti);

		loopN = WzRecvQ4.GetCount();	

		if( loopN > MAX_NODE-1 )
		{
			loopN = MAX_NODE-1;
			gWindow.PrintLog("error : Q3 Loop MAX %s %d", __FILE__, __LINE__);
		}
		LeaveCriticalSection(&criti);
		if( loopN > 0 )
		{
			//DWORD dwTime = GetTickCount();
			//for( int n=0; n<loopN; n++)
			{
				if( WzRecvQ4.GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex, &iSessionId) == TRUE )
				{
					bProcessPacket = true;
					if(iSessionId != g_DelayHandler.GetQuerySessionId())
					{
						if(g_DelayHandler.CheckHeadCode(headcode) == false)
						{
							bProcessPacket = false;
							gWindow.PrintLog("[DelayHandler] Drop Delayed Packet -> Head : %X", headcode);
						}
						else
						{
							gWindow.PrintLog("[DelayHandler] Process Delayed Packet -> Head : %X", headcode);
						}
					}
					if(bProcessPacket != false)
					{
						ProtocolCore(headcode, RecvData, nSize, uindex);
					}
				}
			}
			//gWindow.PrintLog("Process End : [%d]", GetTickCount() - dwTime);
		}
		Sleep(1);
	}
	return TRUE;
}