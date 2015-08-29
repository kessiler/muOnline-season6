#if !defined(__UDPPROTOCOL_H__)
#define __UDPPROTOCOL_H__

#include "ProDef.h"

typedef struct
{
    PBMSG_HEAD h;
    short ServerCode;
    BYTE Percent;
    short UserCount;
    short AccountCount;
    short PCbangCount;
    short MaxUserCount;
} PMSG_SERVERINFO, *LPPMSG_SERVERINFO;

typedef struct
{
    PBMSG_HEAD	h;
    int			iQueueCount;
} PMSG_JOINSERVER_STAT, *LPPMSG_JOINSERVER_STAT;

extern void UdpProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, char *ip);

#endif