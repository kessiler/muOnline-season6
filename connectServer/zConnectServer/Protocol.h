#if !defined(__PROTOCOL_H__)
#define __PROTOCOL_H__

#include "ProDef.h"

struct PMSG_REQ_SERVERLIST
{
    PBMSG_HEAD2 h;
};

struct PMSG_CONNECTRESULT
{
    PBMSG_HEAD h;
    BYTE result;
};

typedef struct
{
    PBMSG_HEAD h;
    BYTE Version1;
    BYTE Version2;
    BYTE Version3;
} PMSG_VERSION,*LPPMSG_VERSION;

typedef struct
{
    PBMSG_HEAD	h;
    BYTE	Version[3];
    char 	IpAddress[100];
    short	Port;
    char	Id[20];
    char	Pass[20];
    char	Folder[20];
} PMSG_FILESERVERINFO, *LPPMSG_FILESERVERINFO;

typedef struct
{
    PWMSG_HEAD2 h;
    BYTE CountH;
    BYTE CountL;
} PMSG_SEND_SERVERLIST, *LPMSG_SEND_SERVERLIST;

typedef struct
{
    PBMSG_HEAD2 h;
    WORD ServerIndex;
} PMSG_REQ_SERVER_INFO, *LPMSG_REQ_SERVER_INFO;

typedef struct
{
    PBMSG_HEAD2 h;
    char Ip[16];
    WORD Port;
} PMSG_ANS_SERVER_INFO, *LPMSG_ANS_SERVER_INFO;

typedef struct
{
    WORD Index;
    BYTE Load;
    BYTE Unk;
} CS_SERVER_DATA;

void ProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, int aIndex);
void CSVersionRecv(LPPMSG_VERSION lpMsg,int aIndex);
void CSVersionHttpRecv(LPPMSG_VERSION lpMsg,int aIndex);

void SCConnectResultSend(BYTE result, int aIndex);
void CLGetServerList(int aIndex);
void CLGetServerList(PMSG_REQ_SERVERLIST* lpMsg,int aIndex);
void CLGetServerInfo(LPMSG_REQ_SERVER_INFO lpMsg,int aIndex);
void LogProtocol(char* szType,BYTE* aRecv,int aLen);

extern BYTE g_ServerListPacket[sizeof(PMSG_SEND_SERVERLIST) + sizeof(CS_SERVER_DATA) * MAX_SERVER_LIST];

#endif