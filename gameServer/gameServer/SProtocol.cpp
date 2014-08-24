//	GS-CS	1.00.90	JPN	0xXXXXXXXX	-	Completed
#include "stdafx.h"
#include "SProtocol.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "protocol.h"
#include "logproc.h"
#include "TNotice.h"
#include "..\common\winutil.h"
#include "Shop.h"
#include "MapServerManager.h"
#include "DBSockMng.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "resource.h"
#ifdef __CUSTOMS__
#include "ConnectEx.h"
#endif

//00437750 - identical
void SProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{

	#ifdef TRACE_PROTOCOL
		LogAddHeadHex("JOIN_SERVER", aRecv, aLen);
	#endif

	switch ( protoNum )
	{
		case 0x00:
			JGServerLoginResult((SDHP_RESULT *)aRecv);
			break;

		case 0x01:
			JGPAccountRequest((SDHP_IDPASSRESULT *)aRecv);
			break;

		case 0x06:
			GJPBillCeckRecv((SDHP_BILLSEARCH_RESULT *)aRecv);
			break;

		case 0x07:
			GJPUserKillRecv((SDHP_BILLKILLUSER *)aRecv);
			break;

		case 0x08:
			JGOtherJoin((SDHP_OTHERJOINMSG *)aRecv);
			break;

		case 0x09:
			GJPUserDisconnectRecv((SDHP_BILLKILLUSER *)aRecv);
			break;

		case 0x20:
			JGPExitCodeRecv((SDHP_EXITMSG *)aRecv);
			break;

		case 0x60:
			JGPEventStart((SDHP_EVENTSTART *)aRecv);
			break;

		case 0x61:
			JGPEventControl((SDHP_EVENTCONTROL *)aRecv);
			break;

		case 0x62:
			JGPOptionControl((SDHP_OPTIONCONTROL *)aRecv);
			break;

		case 0x63:
			JGPSendMail((PMSG_JG_MEMO_SEND *)aRecv);
			break;

		case 0x30:
			LoveHeartEventRecv((SDHP_LOVEHEARTEVENT_RESULT *)aRecv);
			break;

		case 0x40:
			AllNoticeRecv((SDHP_NOTICE *)aRecv);
			break;

		case 0x41:
			UserNoticeRecv((SDHP_USER_NOTICE *)aRecv);
			break;

		case 0x7A:
			JGAnsMapSvrMove((PMSG_ANS_MAPSVRMOVE *)aRecv);
			break;

		case 0x7B:
			JGAnsMapSvrAuth((PMSG_ANS_MAPSVRAUTH *)aRecv);
			break;

		case 0x8A:
			//JGRequestAntiHackStatistics((PMSG_HACKTOOL_USE_STATISTICS *)aRecv);
			break;
		case 0x8B:
			//JGRequestHacktoolMapMoveBlock((PMSG_RESERVE_HACKTOOL_MAP_MOVE *)aRecv);
			break;
		case 0x8D:
			if( aRecv[3] == 1 )
			{
				g_HacktoolBlockEx.OnRequestHacktoolStatistics((PMSG_HACKTOOL_CHECK_USE_STATISTICS_REQ*)aRecv);
			}
			else if( aRecv[3] == 2 )
			{
				g_HacktoolBlockEx.OnRequestHacktoolBlockRemove((PMSG_HACKTOOL_BLOCK_REMOVE_REQ*)aRecv);
			}
			break;
	}
}

//004379B0 - identical
void GJServerLogin()
{
	SDHP_SERVERINFO pInfo;

	pInfo.h.c = 0xC1;
	pInfo.h.size = sizeof(SDHP_SERVERINFO);
	pInfo.h.headcode = 0x00;
	pInfo.Port = GameServerPort;
	pInfo.Type = 1;
	strcpy(pInfo.ServerName , szServerName);
	pInfo.ServerCode = gGameServerCode;

	wsJServerCli.DataSend((char*)&pInfo, pInfo.h.size);
}

//00437A30 - identical
void JGServerLoginResult( SDHP_RESULT * lpMsg)	
{
	if ( lpMsg->Result == false )
	{
		LogAdd(lMsg.Get(MSGGET(1, 210)));
		return;
	}

	JoinServerConnected=TRUE;
	gServerReady++;
	SendMessage(ghWnd, WM_START_SERVER, 0, 0);
}

//00437AB0  - identical
void GJPUserClose(LPSTR szAccountID)
{
	SDHP_USERCLOSE_ID pClose;

	pClose.h.c =0xC1;
	pClose.h.size= sizeof(SDHP_USERCLOSE_ID);
	pClose.h.headcode = 0x05;
	memcpy(pClose.szId , szAccountID, 10);

	wsJServerCli.DataSend((char*)&pClose, pClose.h.size);
}

//00437B10 - identical
void GJPUserClose(LPOBJ lpObj)
{
	SDHP_USERCLOSE_ID pClose;

	pClose.h.c =0xC1;
	pClose.h.size= sizeof(SDHP_USERCLOSE_ID);
	pClose.h.headcode = 0x05;
	memcpy(pClose.szId , lpObj->AccountID, 10);
	memcpy(pClose.szName, lpObj->BackName, 10);
	pClose.Level = lpObj->Level;
	pClose.DbClass = lpObj->DbClass;

	wsJServerCli.DataSend((char*)&pClose, pClose.h.size);
}

//00437BB0 identical
void JGPAccountRequest(SDHP_IDPASSRESULT * lpMsg)
{
	char szId[11];
	int aIndex;
	char szJN[14]="";
	
	memset(szId, 0, sizeof(szId));
	memcpy(szId, lpMsg->Id , sizeof(lpMsg->Id));
	memcpy(szJN, lpMsg->JoominNumber , sizeof(lpMsg->JoominNumber));

	aIndex = lpMsg->Number ;
	
	if ( lpMsg->result  == 1 || lpMsg->result  == 15 )
	{
		if ( gNonPK == 0 && gLanguage == 0 )
		{
			if (gJoomin15Check(szJN) == 0 )
			{
				GJPAccountFail(aIndex, szId, lpMsg->DBNumber , lpMsg->UserNumber );
				lpMsg->result  = 17;
			}
		}
			
		if ( gPkLimitFree == 1 && gLanguage == 0 )
		{
			if (gJoominCheck(szJN, 18) == 0 )
			{
				GJPAccountFail(aIndex, szId, lpMsg->DBNumber , lpMsg->UserNumber );
				lpMsg->result  = 17;
			}
		}
	}

	switch ( lpMsg->result  )
	{
		case 1:
		case 15:
			if (gObjSetAccountLogin(aIndex, szId, lpMsg->UserNumber , lpMsg->DBNumber,lpMsg->ukn_30, szJN ,lpMsg->PcBangRoom) == 0 )//Season 4.5 addon
			{
				GJPAccountFail(aIndex, szId, lpMsg->DBNumber , lpMsg->UserNumber  );
				lpMsg->result  = 4;
			}
			else
			{	
				LogAddTD(lMsg.Get(MSGGET(1, 211)), gObj[aIndex].Ip_addr, szId, lpMsg->UserNumber, lpMsg->DBNumber);

#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
				if( gObj[aIndex].m_ConnectEx )
				{
					g_ConnectEx.SendGame(aIndex);
				}
#endif

			}

			if ( lpMsg->UserNumber  == 0 && lpMsg->DBNumber == 0)
			{
				LogAddTD(lMsg.Get(MSGGET(1, 212)), szId, lpMsg->UserNumber , lpMsg->DBNumber );
			}
	}

	if ( lpMsg->result == 0 )
	{
		lpMsg->result = 2;
	}

	if ( lpMsg->result  == 15 )
	{
		lpMsg->result  = 1;
		gObj[aIndex].m_cAccountItemBlock = 1;
	}
	
	gObj[aIndex].LoginMsgSnd = 0;

	if (gObj[aIndex].LoginMsgCount >= 3 )
	{
		lpMsg->result = 8;
	}

	GCJoinResult(lpMsg->result , aIndex);

	if ( lpMsg->result  != 1 )
	{
		if ( gObj[aIndex].LoginMsgCount > 3 )
		{
			CloseClient(aIndex);
		}
	}
}

//00437F30  - identical
void GJPAccountFail(short number, LPSTR Id, int DBNumber, int UserNumber)
{
	SDHP_JOINFAIL pMsg;

	memset(&pMsg, 0, sizeof(SDHP_JOINFAIL));

	pMsg.h.c=0xC1;
	pMsg.h.headcode= 0x02;
	pMsg.h.size = sizeof (SDHP_JOINFAIL);
	pMsg.Number = number;
	pMsg.DBNumber = DBNumber;
	pMsg.UserNumber = UserNumber;

	strcpy(pMsg.Id , Id);

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//00437FB0 - identical
void GJPAccountBlock(short number, LPSTR Id, int DBNumber, int UserNumber, int BlockCode)
{
	SDHP_COMMAND_BLOCK pMsg;

	memset(&pMsg, 0, sizeof(SDHP_COMMAND_BLOCK));

	pMsg.h.c=0xC1;
	pMsg.h.headcode= 0x04;
	pMsg.h.size = sizeof (SDHP_COMMAND_BLOCK);
	pMsg.Number = number;
	pMsg.DBNumber = DBNumber;
	pMsg.UserNumber = UserNumber;
	pMsg.BlockCode = BlockCode;

	strcpy(pMsg.Id , Id);

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//00438040  - identical
void GJPBillCeckRecv(SDHP_BILLSEARCH_RESULT * lpMsg)
{
	int aIndex = lpMsg->Number ;
	char szId[11];
	char EndsDays[13];

	szId[10] = 0;

	memcpy(szId, lpMsg->Id, sizeof(lpMsg->Id));

	if (gObjIsAccontConnect(aIndex, szId) == 0 )
	{
		return ;
	}

	memset(EndsDays, 0, sizeof(EndsDays));
	memcpy(EndsDays, lpMsg->EndsDays, sizeof(lpMsg->EndsDays));
}

//00438140 - identical
void GJPUserKillRecv(SDHP_BILLKILLUSER * lpMsg)
{
	char szId[11];
	int number;
	szId[10] = 0;

	memcpy(szId, lpMsg->Id , sizeof(lpMsg->Id) );
	number = lpMsg->Number;

	if (gObjIsAccontConnect(number, szId) == 0 )
	{
		return;
	}

	GCServerMsgStringSend(lMsg.Get( MSGGET(6, 66)), number, 0);
	gObjUserKill(number);
}

//004381E0 - identical
void JGOtherJoin(SDHP_OTHERJOINMSG * lpMsg)
{
	char szId[11];
	szId[10]=0;
	int n;

	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	for (n= OBJ_STARTUSERINDEX; n<OBJMAX;n++)
	{
		if ( gObj[n].Connected >= PLAYER_LOGGED )
		{
			if ( gObj[n].AccountID[0] == szId[0] )
			{
				if ( gObj[n].AccountID[1] == szId[1] )
				{
					if ( strcmp(gObj[n].AccountID , szId) == 0 )
					{
						GCServerMsgStringSend(lMsg.Get( MSGGET(4, 107)), n, 1 );
						break;
					}
				}
			}
		}
	}
}

//00438300  -identical
void GJPUserDisconnectRecv( SDHP_BILLKILLUSER * lpMsg)
{
	char szId[11];
	szId[10]=0;
	int number;

	memcpy( szId, lpMsg->Id , sizeof( lpMsg->Id ) );
	number = lpMsg->Number;

	if ( gObjIsAccontConnect(number, szId ) == 0 )
	{
		return;
	}

#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
	g_ConnectEx.SendClose(number);
#endif

	GCServerMsgStringSend(lMsg.Get( MSGGET(6, 67) ), number, 0 );
	gObjUserKill(number);
}

//004383A0 - identical
void JGPExitCodeRecv( SDHP_EXITMSG * lpMsg )
{
	if ( lpMsg->ExitCode[0] == 70 && lpMsg->ExitCode[1] == 70 && lpMsg->ExitCode[2] == 88 )
	{
		SendMessage( ghWnd, WM_COMMAND, IDM_3_MINUTE_CLOSE, 0 );
	}
}

//00438410 - identical
void JGPEventStart( SDHP_EVENTSTART * lpMsg )
{
	if ( lpMsg->Event == 1 )
	{
		if (DragonEvent->GetState() != 0 )
		{
			DragonEvent->End();
		}
		else
		{
			DragonEvent->Start();
		}
	}
}

//00438490  - identical
void JGPEventControl( SDHP_EVENTCONTROL * lpMsg )
{
	switch ( lpMsg->Event  )
	{
		case DRAGON_EVENT:
			if ( lpMsg->Stat != 0 )
			{
				if (DragonEvent->GetState() == 0 )
				{
					DragonEvent->Start();
				}
			}
			else
			{
				DragonEvent->End();
			}
			break;

		case XMAS_EVENT:
			gXMasEvent = lpMsg->Stat  ;
			break;

		case FIRECRACKER_EVENT:
			gFireCrackerEvent = lpMsg->Stat  ;
			break;

		case HEARTOFLOVE_EVENT:
			gHeartOfLoveEvent = lpMsg->Stat  ;
			break;

		case MEDAL_EVENT:
			gMedalEvent = lpMsg->Stat  ;
			break;
	}
}

//00438580  - identical
void JGPOptionControl( SDHP_OPTIONCONTROL * lpMsg )
{
	switch ( lpMsg->Option )
	{
		case OPTION_RELOAD:
			ReadCommonServerInfo();
			break;

		case MONSTER_RELOAD:
			GameMonsterAllCloseAndReLoad();
			break;

		case SHOP_RELOAD:
			ShopDataLoad();
			break;

		case ALLOPTION_RELOAD:
			ReadCommonServerInfo();
			GameMonsterAllCloseAndReLoad();
			ShopDataLoad();
			break;

		case EVENTINFO_RELOAD:
			ReadEventInfo(MU_EVENT_ALL );
			break;

		case ETCINFO_RELOAD:
			ReadGameEtcInfo(MU_ETC_ALL );
			break;
	}
}

//00438620 - identical
void LoveHeartEventSend(int aIndex, LPSTR AcountId, LPSTR Name)
{
	SDHP_LOVEHEARTEVENT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(SDHP_LOVEHEARTEVENT);

	memcpy(pMsg.Account , AcountId, sizeof(pMsg.Account));
	memcpy(pMsg.Name , Name, sizeof(pMsg.Name) );

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//00438690  - identical
void LoveHeartEventRecv( SDHP_LOVEHEARTEVENT_RESULT * lpMsg )
{
	char szName[11];
	int number;
	char Msg[256];

	szName[10]=0;

	memcpy(szName, lpMsg->Name, sizeof(lpMsg->Name));
	number = lpMsg->Number;

	switch ( lpMsg->Result )
	{
		case 0: // Registered Hearts of Love PangPangEvent
			{
				wsprintf(Msg, lMsg.Get( MSGGET(4, 80)), number );

				for (int n = OBJ_STARTUSERINDEX;n<OBJMAX;n++)
				{
					if ( gObj[n].Connected >= PLAYER_LOGGED )
					{
						if (gObj[n].Name[0] == szName[0] )
						{
							if ( gObj[n].Name[1] == szName[1] )
							{
								if ( strcmp( gObj[n].Name, szName ) == 0 )
								{
									GCServerMsgStringSend(Msg, n, 1);
									wsprintf(Msg, lMsg.Get( MSGGET( 4, 81 ) ) );
									GCServerMsgStringSend(Msg, n, 1);
									return;
								}
							}
						}
					}
					continue;
				} // For
			}
		break;

		case 1: // Winner of the Love Pang Pang Event
			wsprintf(Msg, lMsg.Get( MSGGET ( 4, 82 ) ), szName, number);
			AllSendServerMsg(Msg);
			break;
	}
}

//004388D0 - identical
void LoveHeartCreateSend()
{
	SDHP_LOVEHEARTCREATE pMsg;

	pMsg.h.c= 0xC1;
	pMsg.h.headcode = 0x31;
	pMsg.h.size=sizeof(SDHP_LOVEHEARTCREATE);

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//00438910 - identical
void AllNoticeRecv( SDHP_NOTICE * lpMsg)
{
	char szNotice[MAX_CHAT_LEN+1];

	szNotice[MAX_CHAT_LEN]=0;

	memcpy(szNotice, lpMsg->Notice , sizeof(lpMsg->Notice)-1);

	LogAddTD("[Mu Manager][Notice Log][Type:ALL] %s", szNotice);

	if ( strlen(szNotice) > 1 )
	{
		AllSendServerMsg(szNotice);
	}
}

//00438990  - identical
void UserNoticeRecv( SDHP_USER_NOTICE * lpMsg)
{
	char szNotice[MAX_CHAT_LEN+1];
	char szId[11];

	szNotice[MAX_CHAT_LEN]=0;
	szId[10]=0;

	memcpy(szNotice, lpMsg->Notice , sizeof(lpMsg->Notice)-1);
	memcpy(szId, lpMsg->szId , sizeof(lpMsg->szId));

	LogAddTD("[Mu Manager][Notice Log][Type:USER] (%s) %s", szId, szNotice);

	if ( strlen(szNotice) > 1 )
	{
		if ( strlen(szId) > 1 )
		{
			for ( int n = OBJ_STARTUSERINDEX;n<OBJMAX; n++)
			{
				if ( gObj[n].Connected >= PLAYER_LOGGED )
				{
					if ( gObj[n].AccountID[0] == szId[0] )
					{
						if ( gObj[n].AccountID[1] == szId[1] )
						{
							if ( strcmp( gObj[n].AccountID , szId) == 0 )
							{
								PMSG_NOTICE pNotice;
								TNotice::MakeNoticeMsg(&pNotice, 0, szNotice);
								TNotice::SetNoticeProperty(&pNotice, 10,  _ARGB(255, 254, 81, 81), 1, 0, 20);
								TNotice::SendNoticeToUser(n, &pNotice);
								break;
							}
						}
					}
				}
			} // For
		}
	}
}

//00438B70 - identical
void GJReqMapSvrMove(int iIndex, WORD wDesMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY)
{
	PMSG_REQ_MAPSVRMOVE pMsg;
	
	if ( gObjIsConnected(iIndex) == PLAYER_EMPTY )
	{
		LogAddC(2, "[MapServerMng] Packet Error GJ [0x7A] - User not Connected : %d", iIndex);
	}
	else
	{
		PHeadSetB((LPBYTE)&pMsg, 0x7A, sizeof(PMSG_REQ_MAPSVRMOVE));
		pMsg.iIndex = iIndex;

		memcpy(pMsg.szAccountID , gObj[iIndex].AccountID, sizeof( pMsg.szAccountID)-1);
		pMsg.szAccountID[10] = 0;
		memcpy(pMsg.szCharName, gObj[iIndex].Name, sizeof( pMsg.szCharName)-1);
		pMsg.szCharName[10] = 0;

		pMsg.wCurMapSvrCode = gGameServerCode;
		pMsg.wDstMapSvrCode = wDesMapSvrCode;
		pMsg.wMapNumber = wMapNumber;
		pMsg.btX = btX;
		pMsg.btY = btY;
#ifdef OPT
		pMsg.btPcbangRoom = gObj[iIndex].m_iPcBangRoom;//Season 4.5 addon
#endif
		wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	}
}

struct PMSG_ANS_MAPSERVERCHANGE
{
	PBMSG_HEAD2 h;	// C1:B1:00
	char szMapSvrIpAddress[16];	// 4
	WORD wMapSvrPort;	// 14
	WORD wMapSvrCode;	// 16
	int iJoinAuthCode1;	// 18
	int iJoinAuthCode2;	// 1C
	int iJoinAuthCode3;	// 20
	int iJoinAuthCode4;	// 24
};

//00438CA0 - identical
void JGAnsMapSvrMove(PMSG_ANS_MAPSVRMOVE * lpMsg)
{
	if ( gObjIsConnected(lpMsg->iIndex ) == FALSE )
	{
		LogAddC(2, "[MapServerMng] Packet Error JG [0x7A] - User not Connected : %d", lpMsg->iIndex);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->iIndex];

	if ( strcmp( lpObj->AccountID , lpMsg->szAccountID )  ||  strcmp( lpObj->Name, lpMsg->szCharName) )
	{
		LogAddC(2, "[MapServerMng] Packet Error JG [0x7A] - User Info Mismatch [%s][%s]-[%s][%s] (%d)", lpMsg->szAccountID, lpMsg->szCharName, lpObj->AccountID, lpObj->Name, lpObj->m_Index);
		return;
	}

	if ( lpMsg->iResult == 0	 )
	{
		PMSG_ANS_MAPSERVERCHANGE pMsg;

		PHeadSubSetBE((LPBYTE)&pMsg, 0xB1, 0x00, sizeof(pMsg));
		
		pMsg.wMapSvrCode = g_MapServerManager.GetMapSvrGroup();
		pMsg.iJoinAuthCode1 = lpMsg->iJoinAuthCode1 ;
		pMsg.iJoinAuthCode2 = lpMsg->iJoinAuthCode2 ;
		pMsg.iJoinAuthCode3 = lpMsg->iJoinAuthCode3;
		pMsg.iJoinAuthCode4 = lpMsg->iJoinAuthCode4 ;
	
		BOOL bRESULT = g_MapServerManager.GetSvrCodeData(lpMsg->wDstMapSvrCode, pMsg.szMapSvrIpAddress, &pMsg.wMapSvrPort);

		if ( bRESULT == FALSE )
		{
			LogAddC(2, "[MapServerMng] Packet Error JG [0x7A] - Failed to find Map Info [%s]-[%s] (%d)",
				lpObj->AccountID, lpObj->Name, lpMsg->wDstMapSvrCode);
			return;
		}

		GJSetCharacterInfo(lpObj, lpObj->m_Index, TRUE, 0);
		lpObj->m_bMapSvrMoveQuit = TRUE;
		lpObj->m_dwMapSvrQuitTick = GetTickCount();

		DataSend(lpMsg->iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		LogAddTD("[MapServerMng] [%s][%s] JG [0x7A] - Move Map Request OK : MAPSVR_D:%d",
			lpObj->AccountID, lpObj->Name, lpMsg->wDstMapSvrCode);
	}
	else
	{
		PMSG_ANS_MAPSERVERCHANGE pMsg = { 0 };//Season 4.5 addon
		PHeadSubSetBE((LPBYTE)&pMsg, 0xB1, 0x00, sizeof(pMsg));//Season 4.5 addon
		DataSend(lpMsg->iIndex, (LPBYTE)&pMsg, pMsg.h.size);//Season 4.5 addon
		
		lpObj->m_MoveGateNumber = -1;//Season 4.5 addon
		lpObj->m_State = 2;//Season 4.5 addon

		PMSG_NOTICE pNotice;

		TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(MSGGET(6, 77)));
		DataSend(lpObj->m_Index, (LPBYTE)&pNotice, pNotice.h.size);

		LogAddC(2, "[MapServerMng] Packet Error JG [0x7A] - Failed to Authorize User [%s][%s] Result:%d",
			lpObj->AccountID, lpObj->Name, lpMsg->iResult);
	}
}
			


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7B 
 */
struct PMSG_REQ_MAPSVRAUTH
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	unsigned short wDstMapSvrCode;
	int iJoinAuthCode1;
	int iJoinAuthCode2;
	int iJoinAuthCode3;
	int iJoinAuthCode4;
};

//00438FF0 - identical
void GJReqMapSvrAuth(int iIndex, LPSTR lpszAccountID, LPSTR lpszCharName, int iJA1, int iJA2, int iJA3, int iJA4)
{
	if ( !OBJMAX_RANGE(iIndex))
	{
		LogAddC(2, "[MapServerMng] Packet Error GJ [0x7B] - Index out of bound : %d", iIndex);
		return;
	}

	PMSG_REQ_MAPSVRAUTH pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x7B, sizeof(pMsg));
	pMsg.iIndex = iIndex;
	memcpy(pMsg.szAccountID, lpszAccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.szCharName, lpszCharName, MAX_ACCOUNT_LEN);
	pMsg.wDstMapSvrCode = gGameServerCode;
	pMsg.iJoinAuthCode1 = iJA1;
	pMsg.iJoinAuthCode2 = iJA2;
	pMsg.iJoinAuthCode3 = iJA3;
	pMsg.iJoinAuthCode4 = iJA4;

	wsJServerCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	gObj[iIndex].m_bMapSvrMoveReq = true;
}

//00439100 - identical
void JGAnsMapSvrAuth(PMSG_ANS_MAPSVRAUTH * lpMsg)
{
	if ( !OBJMAX_RANGE(lpMsg->iIndex))
	{
		LogAddC(2, "[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",
			lpMsg->iIndex);
		
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->iIndex];
	memcpy(lpObj->Name, lpMsg->szCharName, MAX_ACCOUNT_LEN);


	if ( lpObj->m_bMapSvrMoveReq == false )
	{
		LogAddC(2, "[MapServerMng] MapSvr MoveReq Error JG [0x7B] - No Request [%s] (%d)",
			lpObj->AccountID, lpObj->m_Index);

		gObjDel(lpMsg->iIndex);
		return;
	}

	BOOL bAuthOK = FALSE;
	int iSendResult = 0;
	char szId[11];
	char szJN[14] = "";
	int iIndex = lpMsg->iIndex;
	SDHP_DBCHARINFOREQUEST pMsg;
	char szName[11];

	memset(szId, 0, sizeof(szId));
	memcpy(szId, lpMsg->szAccountID, MAX_ACCOUNT_LEN);
	memcpy(szJN, lpMsg->cJoominNumber, sizeof(lpMsg->cJoominNumber));
	
	switch ( lpMsg->iResult )
	{
		case 0:
			iSendResult = 0;
			bAuthOK = FALSE;

			if ( !gObjSetAccountLogin(iIndex, szId, lpMsg->iUserNumber, lpMsg->iDBNumber,lpMsg->ukn_30, szJN,lpMsg->PcBangRoom))//Season 4.5 addon
			{
				GJPAccountFail(iIndex, szId, lpMsg->iDBNumber, lpMsg->iUserNumber);
				lpMsg->iResult = 4;
			}
			else
			{
				LogAddTD(lMsg.Get(MSGGET(1, 211)), gObj[iIndex].Ip_addr, szId, lpMsg->iUserNumber, lpMsg->iDBNumber);
				iSendResult = 1;
				bAuthOK = TRUE;
			}

			if ( lpMsg->iUserNumber == 0 && lpMsg->iDBNumber == 0 )
			{
				LogAddTD(lMsg.Get(MSGGET(1, 212)), szId, lpMsg->iUserNumber, lpMsg->iDBNumber);
			}

			switch ( lpMsg->btBlockCode )
			{
				case 70:	// 0x46
					gObj[iIndex].m_cAccountItemBlock = 1;
					break;
			}
			break;

		default:
			iSendResult = 0;
			bAuthOK = FALSE;
			break;
	}

	GCAnsMapSvrAuth(iIndex, iSendResult);

	if ( bAuthOK == FALSE )
	{
		LogAddC(2, "[MapServerMng] [%s][%s] JG [0x7B] - Join Result Failed : RST:%d, BLK:%c",
			szId, lpMsg->szCharName, lpMsg->iResult, lpMsg->btBlockCode);
		gObjDel(lpMsg->iIndex);

		return;
	}
	
	gObj[iIndex].m_sPrevMapSvrCode = lpMsg->wPrevMapSvrCode;
	gObj[iIndex].m_sDestMapNumber  = lpMsg->wMapNumber;
	gObj[iIndex].m_btDestX = lpMsg->btX;
	gObj[iIndex].m_btDestY = lpMsg->btY;
#ifdef OPT
	gObj[iIndex].m_iPcBangRoom = lpMsg->PcBangRoom;//Season 4.5 addon
#endif
	gObj[iIndex].bEnableDelCharacter = FALSE;

	PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
	memset(szName, 0, sizeof(szName));
	memcpy(szName, lpMsg->szCharName, MAX_ACCOUNT_LEN);
	BuxConvert(szName, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, szName, MAX_ACCOUNT_LEN);
	strcpy(pMsg.AccountID, lpMsg->szAccountID);
	pMsg.Number = lpObj->m_Index;

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);

	LogAddTD("[MapServerMng] [%s][%s] JG [0x7B] - Join Result OK : RST:%d, BLK:%c",
		szId, lpMsg->szCharName, lpMsg->iResult, lpMsg->btBlockCode);

}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Notify Max User
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7C
 */
struct PMSG_NOTIFY_MAXUSER
{
	PBMSG_HEAD h;
	int iSvrCode;	// 4
	int iMaxUserCount;	// 8
	int iCurUserCount;	//	C
};

//00439630  - identical
void GJNotifyMaxUserCount()
{
	PMSG_NOTIFY_MAXUSER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x7C, sizeof(PMSG_NOTIFY_MAXUSER));
	pMsg.iSvrCode = gGameServerCode;
	pMsg.iMaxUserCount = gServerMaxUser;
	pMsg.iCurUserCount = gObjTotalUser;
	
	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//004396A0 - identical
void JGPSendMail(PMSG_JG_MEMO_SEND * lpMsg)
{
	MngFriendMemoSend(lpMsg);
}

struct SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS
{
	PBMSG_HEAD h;	// C1:16
	char szId[10];	// 3
	char szName[10];	// D
	WORD Level;	// 18
	BYTE DbClass;	// 1A
};

//004396D0 - identical
void GJUpdateMatchDBUserCharacters(LPOBJ lpObj)
{
	SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x16, sizeof(SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS));
	pMsg.DbClass = lpObj->DbClass;
	pMsg.Level = lpObj->Level;
	memcpy(pMsg.szId, lpObj->AccountID, sizeof(pMsg.szId));
	memcpy(pMsg.szName, lpObj->BackName, sizeof(pMsg.szName));

	wsJServerCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
}