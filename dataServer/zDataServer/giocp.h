#ifndef __GIOCP_H__
#define __GIOCP_H__

//#define DATA_BUFSIZE	8192
#include <winsock2.h>
#include "Wzqueue.h"

#define MAX_WORKER_THREAD	16

typedef struct _PER_IO_CONTEXT
{
	OVERLAPPED					Overlapped;
	WSABUF						wsabuf;
	CHAR						Buffer[MAX_BUFF_SIZE];
//	CHAR						BufferSecond[MAX_BUFF_SIZE];
//	int							nSecondOfs;
	int							nTotalBytes;
	int							nSentBytes;
	int							IOOperation;
//	int							nWaitIO;			// IO 작업을 진행했다면.. 기다리게 알린다.
	_ListNode*					pQList;
	_PER_IO_CONTEXT*			pIOContextForward;
} PER_IO_CONTEXT, * LPPER_IO_CONTEXT;

typedef struct _PER_SOCKET_CONTEXT
{
	SOCKET			m_socket;
	int				nIndex;
	PER_IO_CONTEXT	IOContext[2];			// 0 : recv, 1 : send
	WZQueue*		WzSendQ;
	DWORD			dwIOCount;
	_PER_SOCKET_CONTEXT* lpCtxtBack; //new
	_PER_SOCKET_CONTEXT* lpCtxtForward; //new
} PER_SOCKET_CONTEXT, *LPPER_SOCKET_CONTEXT;

extern CRITICAL_SECTION criti;

//extern BOOL	g_bLive;
extern WZQueue	WzRecvQ;
extern WZQueue	WzRecvQ2;
extern WZQueue	WzRecvQ3;
extern WZQueue	WzRecvQ4;


int  SocketInit();
int  CreateGIocp(int server_port);
int  IoSend(int aIndex, LPBYTE lpMsg, DWORD size);
LPPER_SOCKET_CONTEXT UpdateCompletionPort( SOCKET sd, int ClientIndex, BOOL bAddToList ) ;
void CloseClient ( int index );

#endif
