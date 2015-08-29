#if !defined(__SERVERLIST_H__)
#define __SERVERLIST_H__

// Structs

typedef struct
{
    BOOL	bUsed;
    BOOL	Live;
    int		ServerCode;
    char	Name[20];
    char	IpAddress[16];
    WORD	Port;
    BYTE	Percent;
    short	UserCount;
    short	MaxUserCount;
    BYTE	bShow;

    DWORD	dwTICK_COUNT;

} SERVER_STRUCT, *LP_SERVER_STRUCT;

// Functions
void LoadConnectServerList(char* szFileName);
void SetServerList();
void CheckServerState();
void GameServerInfoUpdate(int ServerCode, int ServerLoad, int UserCount, int MaxUserCount);
LP_SERVER_STRUCT FindServer(int ServerCode);

// Variables
extern SERVER_STRUCT g_ServerList[MAX_SERVER_LIST];
extern int g_ServerListCount;
extern CRITICAL_SECTION gscrit;

extern int g_bJoinServerLive;
extern DWORD g_dwJoinServerRefreshTime;
extern int g_bJoinServerConnectLimit;
extern int g_JoinServerRecvQCount;
extern int g_LiveGSCount;

#endif