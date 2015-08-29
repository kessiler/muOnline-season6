#include "stdafx.h"
#include "UdpProtocol.h"
#include "ServerList.h"
#include "Window.h"

void GameServerInfo(LPPMSG_SERVERINFO lpMsg);
void JoinServerInfo(LPPMSG_JOINSERVER_STAT lpMsg);

void UdpProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, char *ip)
{
#if ( LOG_PROTOCOL == 1 )
    LogProtocol("RECV_UDP",aRecv,aLen);
#endif

    switch( protoNum )
    {
    case 0x01 :
    {
        GameServerInfo((LPPMSG_SERVERINFO)aRecv);
    }
    break;

    case 0x02 :
    {
        JoinServerInfo((LPPMSG_JOINSERVER_STAT)aRecv);
    }
    break;
    }
}

void GameServerInfo(LPPMSG_SERVERINFO lpMsg)
{
    GameServerInfoUpdate(lpMsg->ServerCode, lpMsg->Percent, lpMsg->UserCount,lpMsg->MaxUserCount);
}

void JoinServerInfo(LPPMSG_JOINSERVER_STAT lpMsg)
{
    g_JoinServerRecvQCount = lpMsg->iQueueCount;

    if( !g_bJoinServerLive )
    {
        g_Window.PrintLog("[JoinServer] Update completed");
        g_bJoinServerLive = TRUE;
        //g_DisplayInfo.SetJoinServerConnected(TRUE);
    }

    if( g_JoinServerRecvQCount < MAX_JS_RECVQ_LIMIT )
    {
        g_bJoinServerConnectLimit = 0;
    }

    g_dwJoinServerRefreshTime = GetTickCount();
}