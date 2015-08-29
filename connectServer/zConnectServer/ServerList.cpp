#include "stdafx.h"
#include "ServerList.h"
#include "ServerInit.h"
#include "ReadScript.h"
#include "Window.h"

SERVER_STRUCT g_ServerList[MAX_SERVER_LIST];
int g_ServerListCount = 0;
int g_LiveGSCount = 0;

CRITICAL_SECTION gscrit;

void LoadConnectServerList(char* szFileName)
{
    SMDFile = fopen(szFileName,"r");

    if( SMDFile == NULL )
    {
        g_Window.PrintLog("[Error] [%s] Server list load fail!", szFileName);
        MessageBox(NULL, "Enable to load ServerList.txt", "Loading Error", ERROR);
        ExitProcess(0);
        return;
    }

    g_ServerListCount = 0;
    memset(g_ServerList,0x00,sizeof(g_ServerList));

    int Token;

    while( TRUE )
    {
        Token = GetToken();

        if( Token == END )
            break;

        while( TRUE )
        {
            if( strcmp(TokenString,"end") == NULL )
                break;

            if( g_ServerListCount >= MAX_SERVER_LIST )
            {
                g_Window.PrintLog("[Error] Server list count is max");
                break;
            }

            LP_SERVER_STRUCT lpServer = &g_ServerList[g_ServerListCount];

            lpServer->bUsed = TRUE;
            lpServer->Live = FALSE;
            lpServer->ServerCode = (int)TokenNumber;
            Token = GetToken();
            strncpy(lpServer->Name,TokenString,20);
            Token = GetToken();
            strncpy(lpServer->IpAddress,TokenString,16);
            Token = GetToken();
            lpServer->Port = TokenNumber;
            Token = GetToken();
            g_ServerListCount++;
        }
    }

    fclose(SMDFile);
    SetServerList();

    g_Window.PrintLog("[%s] Server list loaded [%d]", szFileName, g_ServerListCount);
}

void SetServerList()
{
    std::map<int, LP_SERVER_STRUCT>::iterator Itor_;
    LP_SERVER_STRUCT lpServer = NULL;

    char szTmp[256];

    for(int i = 0; i < MAX_SERVER_LIST; i++)
    {
        lpServer = &g_ServerList[i];

        if( lpServer->bUsed == FALSE )
            continue;

        if( lpServer->Live )
        {
            sprintf(szTmp,"%s - ON",lpServer->Name);
        }
        else
        {
            sprintf(szTmp,"%s - OFF",lpServer->Name);
        }
    }
}

int g_bJoinServerLive = FALSE;
DWORD g_dwJoinServerRefreshTime = NULL;
int g_bJoinServerConnectLimit;
int g_JoinServerRecvQCount;

void CheckServerState()
{
    LP_SERVER_STRUCT lpServer = NULL;

    if( g_bJoinServerLive )
    {
        if( GetTickCount() - g_dwJoinServerRefreshTime > 2000 )
        {
            g_bJoinServerLive = FALSE;
            g_Window.PrintLog("[JoinServer] Connection time out");
        }
    }

    int bResetList = FALSE;
    g_LiveGSCount = 0;

    for(int i = 0; i < MAX_SERVER_LIST; i++)
    {
        lpServer = &g_ServerList[i];

        if( lpServer->bUsed == FALSE )
            continue;

        if( lpServer->Live )
        {
            if( GetTickCount() - lpServer->dwTICK_COUNT > 2000 )
            {
                bResetList = TRUE;
                lpServer->Live = FALSE;
                g_Window.PrintLog("[GameServer] [%d][%s] Connection time out", lpServer->ServerCode, lpServer->Name);
            }
            else
            {
                g_LiveGSCount++;
            }
        }
    }

    if( bResetList )
    {
        SetServerList();
    }
}

void GameServerInfoUpdate(int ServerCode, int ServerLoad, int UserCount, int MaxUserCount)
{
    std::map<int,LP_SERVER_STRUCT>::iterator Itor_;
    LP_SERVER_STRUCT lpServer = NULL;
    int bUpdateList = FALSE;

    lpServer = FindServer(ServerCode);

    if( lpServer == NULL )
    {
        g_Window.PrintLog("[GameServer] Update information error (Server not found)");
        return;
    }

    if( lpServer->ServerCode != ServerCode )
    {
        g_Window.PrintLog("[GameServer] Update information error (Server code dismatch)");
        return;
    }

    lpServer->MaxUserCount = MaxUserCount;
    lpServer->UserCount = UserCount;
    lpServer->Percent = ServerLoad;
    lpServer->dwTICK_COUNT = GetTickCount();

    if( lpServer->Live == FALSE )
    {
        g_Window.PrintLog("[GameServer] [%d][%s] Information has been updated", ServerCode, lpServer->Name);
        lpServer->Live = TRUE;
        bUpdateList = TRUE;
    }

    if( bUpdateList == TRUE )
    {
        SetServerList();
    }
}

LP_SERVER_STRUCT FindServer(int ServerCode)
{
    for(int i = 0; i < MAX_SERVER_LIST; i++)
    {
        if( g_ServerList[i].bUsed == FALSE ||
                g_ServerList[i].ServerCode != ServerCode )
        {
            continue;
        }
        return &g_ServerList[i];
    }

    return NULL;
}