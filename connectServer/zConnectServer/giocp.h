#ifndef __GIOCP_H__
#define __GIOCP_H__

#include <winsock2.h>
#include "Wzqueue.h"

#define MAX_BUFF_SIZE		10240
#define MAX_WORKER_THREAD	16

typedef struct _PER_IO_CONTEXT
{
    OVERLAPPED					Overlapped;
    WSABUF						wsabuf;
    CHAR						Buffer[MAX_BUFF_SIZE];
    CHAR						BufferSecond[MAX_BUFF_SIZE];
    int							nSecondOfs;
    int							nTotalBytes;
    int							nSentBytes;
    int							IOOperation;
    int							nWaitIO;
} PER_IO_CONTEXT, * LPPER_IO_CONTEXT;

typedef struct _PER_SOCKET_CONTEXT
{
    SOCKET			m_socket;
    int				nIndex;
    PER_IO_CONTEXT	IOContext[2];
    long			dwIOCount;

} PER_SOCKET_CONTEXT, *LPPER_SOCKET_CONTEXT;

BOOL IocpServerStart();
int  SocketInit();
int  CreateGIocp(int server_port);
int  IoSend(int aIndex, LPBYTE lpMsg, DWORD size);
BOOL UpdateCompletionPort( SOCKET sd, int ClientIndex, BOOL bAddToList ) ;
void CloseClient ( int index );
void DestroyGIocp();

#endif
