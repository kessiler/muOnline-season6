#include "stdafx.h"
#include "Protocol.h"
#include "ServerInit.h"
#include "ServerList.h"
#include "giocp.h"
#include "winutil.h"
#include "Window.h"

BYTE g_ServerListPacket[sizeof(PMSG_SEND_SERVERLIST) + sizeof(CS_SERVER_DATA) * MAX_SERVER_LIST] = {0};

void LogProtocol(char* szType,BYTE* aRecv,int aLen)
{
    char szTmp[2048];

    sprintf(szTmp,"[%s] Packet: (",szType);

    for(int i = 0; i < aLen; i++)
    {
        int len = strlen(szTmp);
        if( i != 0 )
        {
            sprintf(&szTmp[len],",");
            len++;
        }

        sprintf(&szTmp[len],"0x%02X",aRecv[i]);
    }

    int len = strlen(szTmp);
    sprintf(&szTmp[len]," )");

    g_Window.PrintLog(szTmp);
}

void ProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, int aIndex)
{
#if ( LOG_PROTOCOL == 1 )
    LogProtocol("RECV",aRecv,aLen);
#endif
    // ----
    switch(protoNum)
    {
    case 0x01:	//-> 20040304
    {
        //40140B
    }
    break;
    // --
    case 0x05:
    {
        CSVersionRecv((LPPMSG_VERSION)aRecv, aIndex);
    }
    break;
    // --
    case 0x06:
    {
        CSVersionHttpRecv((LPPMSG_VERSION)aRecv, aIndex);
    }
    break;
    // --
    case 0x07:	//-> 20040304
    {
        //401226
    }
    break;
    // --
    case 0xF4:
    {
        switch(aRecv[3])
        {
        case 0x02:	//-> 20040304
        {
            //4013E8
        }
        break;
        // --
        case 0x03:
        {
            CLGetServerInfo((LPMSG_REQ_SERVER_INFO)aRecv,aIndex);
        }
        break;
        // --
        case 0x04:	//-> 20040304
        {
            //401253
        }
        break;
        // --
        case 0x06:
        {
            CLGetServerList(aIndex);
        }
        break;
        // --
        case 0x07:
        {
            CLGetServerList((PMSG_REQ_SERVERLIST*)aRecv,aIndex);
        }
        break;
        }
    }
    break;
    }
}

void SCConnectResultSend(BYTE result, int aIndex)
{
    PMSG_CONNECTRESULT	pMsg;

    pMsg.h.c		= PMHC_BYTE;
    pMsg.h.headcode = 0x00;
    pMsg.h.size		= sizeof(pMsg);
    pMsg.result		= result;

    IoSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CSVersionRecv(LPPMSG_VERSION lpMsg,int aIndex)
{
    ////LogAddC(3,"RECV VERSION[%d.%02d.%02d]",lpMsg->Version1,lpMsg->Version2,lpMsg->Version3);
    ////LogAddC(3,"FTP VERSION[%d.%02d.%02d]",FtpServer.Version1,FtpServer.Version2,FtpServer.Version3);

    if( lpMsg->Version1 == FtpServer.Version1 &&
            lpMsg->Version2 == FtpServer.Version2 &&
            lpMsg->Version3 == FtpServer.Version3 )
    {
        PMSG_CONNECTRESULT	pMsg;

        pMsg.h.c		= PMHC_BYTE;
        pMsg.h.headcode = 0x02;
        pMsg.h.size		= sizeof(pMsg);
        pMsg.result		= 0;

        IoSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
        return;
    }

    PMSG_FILESERVERINFO pMsg;
    pMsg.h.c = PMHC_BYTE;
    pMsg.h.headcode = 0x05;
    pMsg.h.size = sizeof(pMsg);

    pMsg.Version[0] = FtpServer.Version1;
    pMsg.Version[1] = FtpServer.Version2;
    pMsg.Version[2] = FtpServer.Version3;

    strcpy(pMsg.IpAddress,FtpServer.Adress);
    strcpy(pMsg.Id,FtpServer.User);
    strcpy(pMsg.Pass,FtpServer.Pass);
    strcpy(pMsg.Folder,FtpServer.VersionFileName);

    pMsg.Port = FtpServer.Port;

    BuxConvert(pMsg.IpAddress, 100);
    BuxConvert(pMsg.Id,   20);
    BuxConvert(pMsg.Pass, 20);
    BuxConvert(pMsg.Folder,  20);

    IoSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void CSVersionHttpRecv(LPPMSG_VERSION lpMsg,int aIndex)
{
    ////LogAddC(3,"RECV VERSION[%d.%02d.%02d]",lpMsg->Version1,lpMsg->Version2,lpMsg->Version3);
    ////LogAddC(3,"HTTP VERSION[%d.%02d.%02d]",HttpServer.Version1,HttpServer.Version2,HttpServer.Version3);

    if( lpMsg->Version1 == FtpServer.Version1 &&
            lpMsg->Version2 == FtpServer.Version2 &&
            lpMsg->Version3 == FtpServer.Version3 )
    {
        PMSG_CONNECTRESULT	pMsg;

        pMsg.h.c		= PMHC_BYTE;
        pMsg.h.headcode = 0x02;
        pMsg.h.size		= sizeof(pMsg);
        pMsg.result		= 0;

        IoSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
        return;
    }

    PMSG_FILESERVERINFO pMsg;
    pMsg.h.c = PMHC_BYTE;
    pMsg.h.headcode = 0x06;
    pMsg.h.size = sizeof(pMsg);

    pMsg.Version[0] = HttpServer.Version1;
    pMsg.Version[1] = HttpServer.Version2;
    pMsg.Version[2] = HttpServer.Version3;

    strcpy(pMsg.IpAddress,HttpServer.Adress);
    strcpy(pMsg.Id,"");
    strcpy(pMsg.Pass,"");
    strcpy(pMsg.Folder,HttpServer.VersionFileName);

    pMsg.Port = 0;

    BuxConvert(pMsg.IpAddress, 100);
    BuxConvert(pMsg.Id,   20);
    BuxConvert(pMsg.Pass, 20);
    BuxConvert(pMsg.Folder,  20);

    IoSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void CLGetServerList(int aIndex)
{
    PMSG_SEND_SERVERLIST pMsg;
    CS_SERVER_DATA pMsgBody;

    int lOfs = sizeof(pMsg);
    int count = 0;

    //EnterCriticalSection(&gscrit);

    LP_SERVER_STRUCT lpServer = NULL;

    for(int i = 0; i < MAX_SERVER_LIST; i++)
    {
        lpServer = &g_ServerList[i];

        if( lpServer->bUsed == FALSE )
            continue;

        if( lpServer->Live == FALSE )
            continue;

        pMsgBody.Index = lpServer->ServerCode;
        pMsgBody.Load = lpServer->Percent;
        pMsgBody.Unk = 0xCC;

        memcpy(&g_ServerListPacket[lOfs],&pMsgBody,sizeof(pMsgBody));
        lOfs += sizeof(pMsgBody);
        count++;
    }

    pMsg.h.c = PMHC_WORD;
    pMsg.h.headcode = 0xF4;
    pMsg.h.subcode = 0x06;
    pMsg.CountH = count >> 8;
    pMsg.CountL = count & 0xFF;

    pMsg.h.sizeH = lOfs >> 8;
    pMsg.h.sizeL = lOfs & 0xFF;

    memcpy(&g_ServerListPacket[0],&pMsg,sizeof(pMsg));

    IoSend(aIndex,g_ServerListPacket,lOfs);
}

void CLGetServerList(PMSG_REQ_SERVERLIST* lpMsg,int aIndex)
{
    PMSG_SEND_SERVERLIST pMsg;
    CS_SERVER_DATA pMsgBody;

    int lOfs = sizeof(pMsg);
    int count = 0;

    //EnterCriticalSection(&gscrit);

    LP_SERVER_STRUCT lpServer = NULL;

    for(int i = 0; i < MAX_SERVER_LIST; i++)
    {
        lpServer = &g_ServerList[i];

        if( lpServer->bUsed == FALSE )
            continue;

        if( lpServer->Live == FALSE )
            continue;

        pMsgBody.Index = lpServer->ServerCode;
        pMsgBody.Load = lpServer->Percent;
        pMsgBody.Unk = 0xCC;

        memcpy(&g_ServerListPacket[lOfs],&pMsgBody,sizeof(pMsgBody));
        lOfs += sizeof(pMsgBody);
        count++;
    }

    pMsg.h.c = PMHC_WORD;
    pMsg.h.headcode = 0xF4;
    pMsg.h.subcode = 0x06;
    pMsg.CountH = count >> 8;
    pMsg.CountL = count & 0xFF;

    pMsg.h.sizeH = lOfs >> 8;
    pMsg.h.sizeL = lOfs & 0xFF;

    memcpy(&g_ServerListPacket[0],&pMsg,sizeof(pMsg));

    IoSend(aIndex,g_ServerListPacket,lOfs);

    //LeaveCriticalSection(&gscrit);
}

void CLGetServerInfo(LPMSG_REQ_SERVER_INFO lpMsg,int aIndex)
{
    PMSG_ANS_SERVER_INFO pMsg;

    int ServerIndex = lpMsg->ServerIndex;

    //EnterCriticalSection(&gscrit);

    LP_SERVER_STRUCT lpServer = FindServer(ServerIndex);

    if( lpServer == NULL )
    {
        ////LogAddC(2,"[GAMESERVER] ServerInfo not found - %d, ClientIndex - [%d](%s)",ServerIndex,aIndex,gObj[aIndex].m_Ip);
        //LeaveCriticalSection(&gscrit);
        return;
    }

    /*if( lpServer->Live == FALSE )
    {
    	////LogAddC(2,"[GAMESERVER] Server not online - %d, ClientIndex - [%d](%s)",ServerIndex,aIndex,gObj[aIndex].m_Ip);
    	//LeaveCriticalSection(&gscrit);
    	return;
    }*/

    pMsg.h.c = PMHC_BYTE;
    pMsg.h.headcode = 0xF4;
    pMsg.h.subcode = 0x03;
    pMsg.h.size = sizeof(pMsg);

    strcpy(pMsg.Ip,lpServer->IpAddress);
    pMsg.Port = lpServer->Port;

    IoSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));

    //LeaveCriticalSection(&gscrit);
}