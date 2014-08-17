
#include "StdAfx.h"
#include <winsock2.h>
#include "ProDef.h"
#include "ServerPrc.h"
#include "MapServerManager_DS.h"

ServerObject_Struct	gSObj[MAX_SERVEROBJECT];
int gSObjCount=0;

void gSObjInit()
{
	for( int n=0; n<MAX_SERVEROBJECT; n++)
	{
		memset(&gSObj[n], 0, sizeof(ServerObject_Struct));
	}
	gSObjCount = 0;
	SetDlgItemText(gWindow.WindowID, IDC_CONNECTEDCOUNT, "0");
}

short gSObjAdd(SOCKET socket, char *ip)
{
	int count;

	for(count = 0; count < MAX_SERVEROBJECT; count++)
	{
		if( gSObj[count].Connected == 0 )
		{
			memset(&gSObj[count], 0, sizeof(ServerObject_Struct));
			gSObj[count].Connected = 1;
			gSObj[count].m_socket = socket;
			strcpy(gSObj[count].Ip , ip);
			gSObjCount++;
			// ----
			CString ServerCount;
			ServerCount.Format("%i", gSObjCount);
			SetDlgItemTextA(gWindow.WindowID, IDC_CONNECTEDCOUNT, ServerCount);
			// ----
			return (short)count;
		}
	}

	return -1;
}

short gSObjDel(int index)
{
	gSObj[index].Connected = 0;	// 사용 안함
	gSObjCount--;
	// ----
	CString ServerCount;
	ServerCount.Format("%i", gSObjCount);
	SetDlgItemTextA(gWindow.WindowID, IDC_CONNECTEDCOUNT, ServerCount);
	// ----
	gWindow.PrintLog(" %d server delete", index);
	return 1;
}

//----------------------------------------------------------------------------
// 서버정보를 세팅한다.
//----------------------------------------------------------------------------
BOOL gSObjSetInfo(int aIndex, WORD port, int type, char * servername, WORD ServerCode)
{
	if( gSObj[aIndex].Connected  < 1 ) {
		gWindow.PrintLog("Error : 접속도 안된 상태이다.");
		return FALSE;
	}

	if( gSObj[aIndex].Connected  == 2 ) {
		gWindow.PrintLog("Error : 서버정보를 다시 세팅할수 없다.(%d)",gSObj[aIndex].Connected);
		return FALSE;
	}
	gSObj[aIndex].Connected = 2;	// 인증까지 마친 상태로 만든다.
	gSObj[aIndex].Port = port;
	gSObj[aIndex].Type = type;
	gSObj[aIndex].ServerCode = ServerCode;
	gSObj[aIndex].pServer = ServerCode%20;
	strcpy(gSObj[aIndex].ServerName, servername);
	gSObj[aIndex].iMapSvrGroup = g_MapServerManager.GetMapSvrGroup(ServerCode);
	
	//CString LogString;
	//LogString.Format("[%s][%s:%d] GameServer Connect ", servername, gSObj[aIndex].Ip, port);
	//JoinServerLogDb.Add(LogString.GetBuffer(256));
	//LogAdd(LogString.GetBuffer(256));
	return TRUE;
}

int gSObjIndexSerrchbyServerCode(WORD ServerCode)
{
	for( int n=0; n<MAX_SERVEROBJECT; n++)
	{
		if( gSObj[n].Connected != 0 && gSObj[n].ServerCode == ServerCode) return n;
	}
	return -1;
}