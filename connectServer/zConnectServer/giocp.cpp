#include "stdafx.h"
#include "zConnectServer.h"
#include "Prodef.h"
#include <winsock2.h>
#include "User.h"
#include "Protocol.h"
#include "WzQueue.h"
#include "giocp.h"
#include "CriticalSec.h"
#include "Window.h"
#include "WhiteList.h"
#include "BlackList.h"

long	ConnectCount = 0;
long	DisConnectCount = 0;

char	ErrorMsg[512];
void	GetLastErrorMsg()
{
    LPVOID lpMsgBuf;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, (LPTSTR)&lpMsgBuf, 0, NULL) != 0)
    {
        strcpy(ErrorMsg, (LPCTSTR)lpMsgBuf);
        LocalFree(lpMsgBuf);
    }
}



enum {
    RECV_IO,
    SEND_IO
} SOCKET_FLAG;


CCriticalSec	 g_CS;

DWORD           g_dwThreadCount = 0;        //worker thread count
HANDLE          g_ThreadHandles[MAX_WORKER_THREAD];
SOCKET			g_Listen = INVALID_SOCKET;
int				g_ServerPort;
HANDLE			g_CompletionPort = NULL;
HANDLE			g_IocpThreadHandle;

int		IoSend(int aIndex, LPBYTE lpMsg, DWORD dwSize);
BOOL	ParsingRecvData(LPPER_IO_CONTEXT	lpIOContext, int uIndex);
void	CloseClient(LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful);
int		IoSendSecond(LPPER_SOCKET_CONTEXT lpPerSocketContext);
int		IoMoreSend(LPPER_SOCKET_CONTEXT lpPerSocketContext);

DWORD	IocpServerWorker(void);
DWORD   WINAPI IOCPWorkerThread(LPVOID CompletionPortID);

BOOL IocpServerStart()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    return TRUE;
}

int CreateGIocp(int server_port)
{
    g_ServerPort = server_port;

    DWORD ThreadID;

    g_IocpThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IocpServerWorker, 0, 0, &ThreadID);

    if (g_IocpThreadHandle == NULL)
    {
        g_Window.PrintLog("[Error] CreateThread() %d", GetLastError());
        return FALSE;
    }

    return TRUE;
}

void DestroyGIocp()
{
    for (DWORD dwCPU = 0; dwCPU < g_dwThreadCount; dwCPU++)
    {
        TerminateThread(g_ThreadHandles[dwCPU], 0);
    }
    TerminateThread(g_IocpThreadHandle, 0);

    if (g_CompletionPort)
    {
        CloseHandle(g_CompletionPort);
        g_CompletionPort = NULL;
    }
}

BOOL CreateListenSocket(void)
{
    SOCKADDR_IN InternetAddr;
    int			nRet;

    CCSLock cs(&g_CS);

    g_Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (g_Listen == INVALID_SOCKET)
    {
        g_Window.PrintLog("[Error] WSASocket() %d", WSAGetLastError());
        return FALSE;
    }

    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(g_ServerPort);

    nRet = bind(g_Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));
    if (nRet == SOCKET_ERROR)
    {
        g_Window.PrintLog("[Error] Port already in use");
        return FALSE;
    }


    nRet = listen(g_Listen, SOMAXCONN);

    if (nRet == SOCKET_ERROR)
    {
        g_Window.PrintLog("[Error] listen() %d", WSAGetLastError());
        return FALSE;
    }
    g_Window.PrintLog("[Server] [%d] Port created", g_ServerPort);
    return TRUE;
}


DWORD IocpServerWorker(void)
{
    SYSTEM_INFO SystemInfo;
    DWORD		ThreadID;
    SOCKET		Accept;
    int			nRet;
    int			ClientIndex;
    SOCKADDR_IN cAddr;
    IN_ADDR		cInAddr;
    int			cAddrlen = sizeof(cAddr);

    LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL;

    DWORD RecvBytes;
    DWORD Flags = 0;

    //InitializeCriticalSection(&criti);

    GetSystemInfo(&SystemInfo);
    g_dwThreadCount = SystemInfo.dwNumberOfProcessors * 2;

    __try
    {
        g_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
        if (g_CompletionPort == NULL)
        {
            g_CS.Lock();
            g_Window.PrintLog("[Error] CreateIoCompletionPort: %d", GetLastError());
            g_CS.UnLock();
            __leave;
        }

        for (DWORD dwCPU = 0; dwCPU < g_dwThreadCount; dwCPU++)
        {
            HANDLE ThreadHandle;
            // Create a server worker thread and pass the completion port to the thread.

            ThreadHandle = CreateThread(NULL, 0, IOCPWorkerThread, g_CompletionPort, 0, &ThreadID);
            if (ThreadHandle == NULL)
            {
                g_CS.Lock();
                g_Window.PrintLog("[Error] CreateThread() %d\n", GetLastError());
                g_CS.UnLock();
                __leave;
            }
            g_ThreadHandles[dwCPU] = ThreadHandle;
        }

        if (!CreateListenSocket())
            __leave;

        while (TRUE)
        {
            Accept = WSAAccept(g_Listen, (LPSOCKADDR)&cAddr, &cAddrlen, NULL, 0);

            if (Accept == SOCKET_ERROR)
            {
                g_CS.Lock();
                GetLastErrorMsg();
                g_Window.PrintLog("[Error] WSAAccept() %d, %s", WSAGetLastError(), ErrorMsg);
                g_CS.UnLock();
                __leave;
            }
            // ----
            memcpy(&cInAddr, &cAddr.sin_addr.s_addr, 4);
            // ----
            if (g_BlackList.Check(inet_ntoa(cInAddr)))
            {
                g_Window.PrintLog("[Connect] [%s] Aborted by BlackList", inet_ntoa(cInAddr));
                g_CS.Lock();
                closesocket(Accept);
                g_CS.UnLock();
                continue;
            }
            // ----
            if (!g_WhiteList.Check(inet_ntoa(cInAddr)))
            {
                g_Window.PrintLog("[Connect] [%s] Aborted by WhiteList", inet_ntoa(cInAddr));
                g_CS.Lock();
                closesocket(Accept);
                g_CS.UnLock();
                continue;
            }
            // ----
            ClientIndex = gObjAdd(Accept, inet_ntoa(cInAddr));
            g_Window.PrintLog("[Connect] [%s] [%d]", inet_ntoa(cInAddr), ClientIndex);
            // ----
            if (ClientIndex == -1)
            {
                g_CS.Lock();
                closesocket(Accept);
                g_CS.UnLock();
                continue;
            }

            ConnectCount++;
            if (UpdateCompletionPort(Accept, ClientIndex, TRUE) == FALSE)
            {
                g_CS.Lock();
                GetLastErrorMsg();
                ////LogAddC(2,"Error : File : [%s], Line : [%d]", __FILE__, __LINE__);
                g_Window.PrintLog("[Error] CreateIoCompletionPort %d, %s", GetLastError(), ErrorMsg);;
                g_CS.UnLock();
                continue;
            }

            gObj[ClientIndex].PerSocketContext.IOContext[0].wsabuf.buf = gObj[ClientIndex].PerSocketContext.IOContext[0].Buffer;
            gObj[ClientIndex].PerSocketContext.IOContext[0].wsabuf.len = MAX_BUFF_SIZE;
            gObj[ClientIndex].PerSocketContext.IOContext[0].nTotalBytes = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[0].nSentBytes = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[0].nWaitIO = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[0].nSecondOfs = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[0].IOOperation = RECV_IO;

            gObj[ClientIndex].PerSocketContext.IOContext[1].wsabuf.buf = gObj[ClientIndex].PerSocketContext.IOContext[0].Buffer;
            gObj[ClientIndex].PerSocketContext.IOContext[1].wsabuf.len = MAX_BUFF_SIZE;
            gObj[ClientIndex].PerSocketContext.IOContext[1].nTotalBytes = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[1].nSentBytes = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[1].nWaitIO = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[1].nSecondOfs = 0;
            gObj[ClientIndex].PerSocketContext.IOContext[1].IOOperation = SEND_IO;
            gObj[ClientIndex].PerSocketContext.m_socket = Accept;
            gObj[ClientIndex].PerSocketContext.nIndex = ClientIndex;

            nRet = WSARecv(Accept, &(gObj[ClientIndex].PerSocketContext.IOContext[0].wsabuf), 1, &RecvBytes, &Flags,
                           &(gObj[ClientIndex].PerSocketContext.IOContext[0].Overlapped), NULL);

            if (nRet == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
            {
                g_CS.Lock();
                GetLastErrorMsg();
                g_Window.PrintLog("[Error] IocpServerWorker WSARecv() %d, %s [%d]", WSAGetLastError(), ErrorMsg, ClientIndex);
                g_CS.UnLock();

                gObj[ClientIndex].PerSocketContext.IOContext[0].nWaitIO = 4;
                CloseClient(&gObj[ClientIndex].PerSocketContext, FALSE);
                continue;
            }
            else
            {
                gObj[ClientIndex].PerSocketContext.IOContext[0].nWaitIO = 1;
                gObj[ClientIndex].PerSocketContext.dwIOCount++;
            }

            SCConnectResultSend(0x01, ClientIndex);
        }	// while
    }	// __try
    __finally
    {
        if (g_CompletionPort)
        {
            for (DWORD i = 0; i < g_dwThreadCount; i++)
                PostQueuedCompletionStatus(g_CompletionPort, 0, 0, NULL);
        }
        if (g_CompletionPort)
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

void IOCPRecvSignal(LPPER_SOCKET_CONTEXT lpPerSocketContext, LPPER_IO_CONTEXT lpIOContext, DWORD dwIoSize)
{
    DWORD	RecvBytes = 0;
    lpIOContext->nSentBytes += dwIoSize;

    if (gObj[lpPerSocketContext->nIndex].Connected < 1)
    {
        return;
    }

    ParsingRecvData(lpIOContext, lpPerSocketContext->nIndex);

    if (gObj[lpPerSocketContext->nIndex].m_socket == INVALID_SOCKET)
        return;

    lpIOContext->nWaitIO = 0;

    ZeroMemory(&(lpIOContext->Overlapped), sizeof(OVERLAPPED));
    lpIOContext->wsabuf.len = MAX_BUFF_SIZE - lpIOContext->nSentBytes;
    lpIOContext->wsabuf.buf = lpIOContext->Buffer + lpIOContext->nSentBytes;
    lpIOContext->IOOperation = RECV_IO;

    DWORD Flags = 0;
    int nRet = WSARecv(lpPerSocketContext->m_socket, &(lpIOContext->wsabuf), 1, &RecvBytes, &Flags,
                       &(lpIOContext->Overlapped), NULL);
    if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
    {
        g_Window.PrintLog("[Error] WSARecv() %d", WSAGetLastError());
        CloseClient(lpPerSocketContext, FALSE);
        return;
    }
    else
    {
        InterlockedIncrement(&lpPerSocketContext->dwIOCount);
        lpIOContext->nWaitIO = 1;
    }
}

void IOCPSendSignal(LPPER_SOCKET_CONTEXT lpPerSocketContext, LPPER_IO_CONTEXT lpIOContext, DWORD dwIoSize)
{
    int aIndex = lpPerSocketContext->nIndex;

    if (aIndex < 0 || aIndex > MAX_CONNECTOBJECT - 1) return;

    lpIOContext->nSentBytes += dwIoSize;
    if (lpIOContext->nSentBytes >= lpIOContext->nTotalBytes)
    {
        lpIOContext->nWaitIO = 0;
        if (lpIOContext->nSecondOfs > 0)
        {
            IoSendSecond(lpPerSocketContext);
        }
    }
    else
    {
        IoMoreSend(lpPerSocketContext);
    }
}

void ProcessIOCPSignal(DWORD ClientIndex, DWORD dwIoSize, LPPER_IO_CONTEXT lpOverlapped)
{
    LPPER_SOCKET_CONTEXT	lpPerSocketContext = NULL;
    LPPER_IO_CONTEXT		lpIOContext = NULL;

    if (ClientIndex < 0 || ClientIndex > MAX_CONNECTOBJECT - 1) return;

    CCSLock cs(&gObj[ClientIndex].m_Lock);

    lpPerSocketContext = &gObj[ClientIndex].PerSocketContext;

    InterlockedDecrement(&lpPerSocketContext->dwIOCount);
    if (0 == dwIoSize)
    {
        CloseClient(lpPerSocketContext, FALSE);
        return;
    }

    lpIOContext = (LPPER_IO_CONTEXT)lpOverlapped;
    if (lpIOContext->IOOperation == SEND_IO)
    {
        IOCPSendSignal(lpPerSocketContext, lpIOContext, dwIoSize);
    }
    else if (lpIOContext->IOOperation == RECV_IO)
    {
        IOCPRecvSignal(lpPerSocketContext, lpIOContext, dwIoSize);
    }
}

DWORD WINAPI IOCPWorkerThread(LPVOID CompletionPortID)
{
    HANDLE	CompletionPort = (HANDLE)CompletionPortID;
    DWORD	dwIoSize;

    DWORD	dwSendNumBytes = 0;
    BOOL	bSuccess = FALSE;
    int     ClientIndex;

    LPOVERLAPPED			lpOverlapped = NULL;

    while (TRUE)
    {
        bSuccess = GetQueuedCompletionStatus(
                       CompletionPort,
                       &dwIoSize,
                       (LPDWORD)&ClientIndex,
                       &lpOverlapped,
                       INFINITE
                   );

        if (!bSuccess)
        {
            if (lpOverlapped != NULL)
            {
                int aError = GetLastError();
                if ((aError != ERROR_NETNAME_DELETED) &&
                        (aError != ERROR_CONNECTION_ABORTED) &&
                        (aError != ERROR_OPERATION_ABORTED) &&
                        (aError != ERROR_SEM_TIMEOUT) &&
                        (aError != ERROR_NETWORK_UNREACHABLE) &&
                        (aError != ERROR_PORT_UNREACHABLE) &&
                        (aError != ERROR_HOST_UNREACHABLE))
                {
                    g_CS.Lock();
                    GetLastErrorMsg();
                    g_Window.PrintLog("[Error] GetQueueCompletionStatus %d, %s", GetLastError(), ErrorMsg);
                    g_CS.UnLock();
                    return FALSE;
                }
            }
        }

        ProcessIOCPSignal(ClientIndex, dwIoSize, (LPPER_IO_CONTEXT)lpOverlapped);
    }
    return TRUE;
}

BOOL ParsingRecvData(LPPER_IO_CONTEXT	lpIOContext, int uIndex)
{
    if (lpIOContext->nSentBytes < 3) return FALSE;

    LPBYTE	recvbuf;
    int		lOfs = 0;
    int		size = 0;
    BYTE	headcode;

    recvbuf = (LPBYTE)lpIOContext->Buffer;

    while (TRUE)
    {
        if (recvbuf[lOfs] == 0xC1) //
        {
            LPPBMSG_HEAD lphead = (LPPBMSG_HEAD)(recvbuf + lOfs);
            size = lphead->size;
            headcode = lphead->headcode;
        }
        else if (recvbuf[lOfs] == 0xC2) //
        {
            LPPWMSG_HEAD lphead = (LPPWMSG_HEAD)(recvbuf + lOfs);
            size = ((WORD)(lphead->sizeH) << 8);
            size |= (WORD)(lphead->sizeL);
            headcode = lphead->headcode;
        }
        else
        {
            g_CS.Lock();
            g_Window.PrintLog("[Error] Invalid PacketHeader [%d] [%d] [%s]", recvbuf[lOfs], lpIOContext->nSentBytes, gObj[uIndex].m_Ip);
            g_CS.UnLock();
            lpIOContext->nSentBytes = 0;
            return FALSE;
        }
        if (size <= 0)
        {
            g_CS.Lock();
            g_Window.PrintLog("[Error] Packet size [%d]", size);
            g_CS.UnLock();
            return FALSE;
        }
        if (size <= lpIOContext->nSentBytes)
        {
            ProtocolCore(headcode, (recvbuf + lOfs), size, uIndex);

            lOfs += size;
            lpIOContext->nSentBytes -= size;
            if (lpIOContext->nSentBytes <= 0) break;
        }
        else
        {
            if (lOfs > 0)
            {
                if (lpIOContext->nSentBytes < 1)
                {
                    g_CS.Lock();
                    g_Window.PrintLog("[Error] recvbuflen < 1");
                    g_CS.UnLock();
                    break;
                }
                else
                {
                    if (lpIOContext->nSentBytes < MAX_BUFF_SIZE)
                    {
                        memcpy(recvbuf, (recvbuf + lOfs), lpIOContext->nSentBytes);
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
    if (aIndex < 0 || aIndex > MAX_CONNECTOBJECT - 1) return FALSE;

    CCSLock cs(&gObj[aIndex].m_Lock);


    DWORD SendBytes;
    LPPER_SOCKET_CONTEXT lpPerSocketContext;

    if (gObj[aIndex].Connected < 1)
    {
        return FALSE;
    }

    lpPerSocketContext = &gObj[aIndex].PerSocketContext;

    if (dwSize > MAX_BUFF_SIZE)
    {
        g_CS.Lock();
        g_Window.PrintLog("[Error] Max msg (%d)", dwSize);
        g_CS.UnLock();
        CloseClient(aIndex);
        return FALSE;
    }

    LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

    if (lpIoCtxt->nWaitIO > 0)
    {
        if ((lpIoCtxt->nSecondOfs + dwSize) > MAX_BUFF_SIZE - 1)
        {
            g_CS.Lock();
            g_Window.PrintLog("[Error] Buffer over [%d] [%d] [%d]",
                              dwSize, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs + dwSize);
            g_CS.UnLock();
            lpIoCtxt->nWaitIO = 0;
            CloseClient(aIndex);
            return TRUE;
        }
        else
        {
            memcpy(lpIoCtxt->BufferSecond + lpIoCtxt->nSecondOfs, lpMsg, dwSize);
            lpIoCtxt->nSecondOfs += dwSize;
            return TRUE;
        }
    }
    else
    {
        lpIoCtxt->nTotalBytes = 0;
        if (lpIoCtxt->nSecondOfs > 0)
        {
            memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
            lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
            lpIoCtxt->nSecondOfs = 0;
        }
        if ((lpIoCtxt->nTotalBytes + dwSize) > MAX_BUFF_SIZE - 1)
        {
            g_CS.Lock();
            g_Window.PrintLog("[Error] Max buffer size is over");
            g_CS.UnLock();
            lpIoCtxt->nWaitIO = 0;
            CloseClient(aIndex);
            return FALSE;
        }
        else
        {
            memcpy(lpIoCtxt->Buffer + lpIoCtxt->nTotalBytes, lpMsg, dwSize);
            lpIoCtxt->nTotalBytes += dwSize;
        }
    }

    lpIoCtxt->wsabuf.buf = (char*)lpIoCtxt->Buffer;
    lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
    lpIoCtxt->nSentBytes = 0;
    lpIoCtxt->IOOperation = SEND_IO;

    if (WSASend(gObj[aIndex].m_socket, &(lpIoCtxt->wsabuf), 1, &SendBytes, 0,
                &(lpIoCtxt->Overlapped), NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            g_CS.Lock();
            GetLastErrorMsg();
            g_Window.PrintLog("WSASend() failed with error %d, %s", WSAGetLastError(), ErrorMsg);
            g_CS.UnLock();
            CloseClient(aIndex);
            return FALSE;
        }
    }
    else
    {
        InterlockedIncrement(&lpPerSocketContext->dwIOCount);
    }
    lpIoCtxt->nWaitIO = 1;
    return TRUE;
}

int IoSendSecond(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{
    DWORD SendBytes;
    int aIndex = lpPerSocketContext->nIndex;

    if (gObj[aIndex].Connected < 1)
    {
        return FALSE;
    }

    LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

    if (lpIoCtxt->nWaitIO > 0)
    {
        return FALSE;
    }

    lpIoCtxt->nTotalBytes = 0;
    if (lpIoCtxt->nSecondOfs > 0)
    {
        memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
        lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
        lpIoCtxt->nSecondOfs = 0;
    }
    else
    {
        return FALSE;
    }

    lpIoCtxt->wsabuf.buf = (char*)lpIoCtxt->Buffer;
    lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
    lpIoCtxt->nSentBytes = 0;
    lpIoCtxt->IOOperation = SEND_IO;

    if (WSASend(gObj[aIndex].m_socket, &(lpIoCtxt->wsabuf), 1, &SendBytes, 0,
                &(lpIoCtxt->Overlapped), NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            g_CS.Lock();
            GetLastErrorMsg();
            g_Window.PrintLog("[Error] WSASend() %d, %s", WSAGetLastError(), ErrorMsg);
            g_CS.UnLock();
            CloseClient(aIndex);
            return FALSE;
        }
    }
    else
    {
        InterlockedIncrement(&lpPerSocketContext->dwIOCount);
    }
    lpIoCtxt->nWaitIO = 1;
    return TRUE;
}

int IoMoreSend(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{
    DWORD SendBytes;

    int aIndex = lpPerSocketContext->nIndex;

    LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

    if ((lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0)
    {
        return FALSE;
    }

    lpIoCtxt->wsabuf.buf = (char*)lpIoCtxt->Buffer + lpIoCtxt->nSentBytes;
    lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
    lpIoCtxt->IOOperation = SEND_IO;

    if (WSASend(gObj[aIndex].m_socket, &(lpIoCtxt->wsabuf), 1, &SendBytes, 0,
                &(lpIoCtxt->Overlapped), NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            g_CS.Lock();
            GetLastErrorMsg();
            g_Window.PrintLog("[Error] WSASend() %d, %s", WSAGetLastError(), ErrorMsg);
            g_CS.UnLock();
            CloseClient(aIndex);
            return FALSE;
        }
    }
    else
    {
        InterlockedIncrement(&lpPerSocketContext->dwIOCount);
    }
    lpIoCtxt->nWaitIO = 1;
    return TRUE;
}


BOOL UpdateCompletionPort(SOCKET sd, int ClientIndex, BOOL bAddToList)
{
    LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL;

    HANDLE CompletionPort;

    CompletionPort = CreateIoCompletionPort((HANDLE)sd, g_CompletionPort, (DWORD)ClientIndex, 0);

    if (CompletionPort == NULL)
    {
        g_Window.PrintLog("[Error] !CompletionPort [%d] [%d]", CompletionPort, g_Listen);
        g_CS.Lock();
        GetLastErrorMsg();
        g_Window.PrintLog("[Error] [%d] [%d] CreateIoCompletionPort: %d, %s",
                          sd, ClientIndex, GetLastError(), ErrorMsg);
        g_CS.UnLock();
        return FALSE;
    }
    g_CompletionPort = CompletionPort;
    InterlockedExchange(&gObj[ClientIndex].PerSocketContext.dwIOCount, 0);
    return TRUE;
}

void CloseClient(LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful)
{
    int index = lpPerSocketContext->nIndex;
    if (index < 0 || index > MAX_CONNECTOBJECT - 1) return;
    CCSLock cs(&gObj[index].m_Lock);

    if (INVALID_SOCKET != gObj[index].m_socket)
    {
        if (closesocket(gObj[index].m_socket) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != 10038)
            {
                return;
            }
        }

        gObj[index].m_socket = INVALID_SOCKET;

        InterlockedIncrement(&DisConnectCount);
        g_Window.PrintLog("[Disconnect] [%s] [%d]", gObj[index].m_Ip, index);
    }

    gObjDel(index);
    return;
}

void CloseClient(int index)
{
    if (index < 0 || index > MAX_CONNECTOBJECT - 1) return;
    CCSLock cs(&gObj[index].m_Lock);

    if (gObj[index].Connected == 0) return;

    closesocket(gObj[index].m_socket);
    gObj[index].m_socket = INVALID_SOCKET;
    InterlockedIncrement(&ConnectCount);
    g_Window.PrintLog("[Disconnect] [%s] [%d]", gObj[index].m_Ip, index);

    gObjDel(index);
    return;
}
