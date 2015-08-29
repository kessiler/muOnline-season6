#if !defined(__USER_H__)
#define __USER_H__

#define MAX_CONNECTOBJECT		500

#include "giocp.h"
#include "CriticalSec.h"

typedef struct
{
    int		m_Index;
    BOOL	Connected;
    SOCKET	m_socket;
    char	m_Ip[16];
    DWORD	Time;

    PER_SOCKET_CONTEXT	PerSocketContext;
    CCriticalSec		m_Lock;
} OBJECTSTRUCT, *LPOBJECTSTRUCT;



extern OBJECTSTRUCT		gObj[];

extern void  gObjInit();
extern void  gObjEnd();
extern int   gObjAdd(SOCKET aSocket, char *ip);
extern int	 gObjDel(int index);
extern int	 gObjGetSocket(SOCKET socket);
extern void  gObjSetTime(int aIndex);

#endif