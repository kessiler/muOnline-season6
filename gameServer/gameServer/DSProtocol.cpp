//GameServer CS 1.00.90 JPN - Completed
#include "stdafx.h"
#include "DSProtocol.h"
#include "logproc.h"
#include "DBSockMng.h"
#include "GameMain.h"
#include "GameServer.h"
//#include "GameServerAuth.h"
#include "MapServerManager.h"
#include "CastleSiegeSync.h"
#include "MixSystem.h"
#include "QuestInfo.h"
#include "TNotice.h"
#include "SProtocol.h"
#include "EDSProtocol.h"
#include "protocol.h"
#include "user.h"
#include "giocp.h"
#include "ObjUseSkill.h"
#include "CashItemPeriodSystem.h"
#include "BloodCastle.h"
#include "..\common\winutil.h"
#include "ObjCalCharacter.h"
#include "CrywolfSync.h"

//#if (_GSCS==1)
#include "Crywolf.h"
#include "CastleSiege.h"
//#endif

#include "QuestUtil.h"
#include "MasterLevelSystem.h"
#include "ItemSocketOptionSystem.h"
#include "TemporaryUserManager.h"	//1.01.00

#ifdef GENS
#include "GensSystem.h"	//1.01.00
#endif

#ifdef PERIOD
#include "PeriodItemEx.h"	//1.01.00
#endif

#ifdef WZQUEST
#include "QuestExpUserMng.h"
#endif

#include "MUHelper.h"
#include "LuckyItem.h"	//1.01.00

#ifdef __CUSTOMS__
#include "ResetSystem.h"
#include "NewsBoard.h"
#endif

#ifdef __ALIEN__
#include "DropEx.h"
#endif

#ifdef __NOVUS__
#include "CraftSystem.h"
#endif

#include "GameShop.h"

//0042B590 - identical
void DataServerProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch ( protoNum )
	{
		case 0x00:
			DataServerLoginResult((SDHP_RESULT *)aRecv);
			break;

		case 0x01:
			JGPGetCharList(aRecv);
			break;

		case 0x04:
			JGCharacterCreateRequest((SDHP_CREATECHARRESULT *)aRecv);
			break;

		case 0x05:
			JGCharDelRequest((SDHP_CHARDELETERESULT *)aRecv);
			break;

		case 0x06:
			JGGetCharacterInfo((SDHP_DBCHAR_INFORESULT *)aRecv);
			break;

		case 0x08:
			DGGetWarehouseList((SDHP_GETWAREHOUSEDB_SAVE *)aRecv);
			break;

		case 0x10:
			GDGetWarehouseNoItem((SDHP_GETWAREHOUSEDB_RESULT *)aRecv);
			break;

			//Season 2.5 Quest Load
		case 0x14:
			g_QuestUtil.Quest3rdLoadInfo(aRecv);
			break;

		case 0x16://Season 4.5 ChangeCharacterName
			DGChangeNameResult((SDHP_CHANGENAME_RESULT *)aRecv);
			break;

#ifdef MUHELPER
		case 0x17:
			g_MUHelper.SendMacro((MUHELPER_MACRO_DS*)aRecv);
			break;
#endif
		case 0x30:
			g_MasterLevelSystem.DGAnsMasterLevelInfo(aRecv);
			break;

		case 0x52:
			ItemSerialCreateRecv((SDHP_ITEMCREATERECV *)aRecv);
			break;

		case 0x56:
			DGRecvPetItemInfo(aRecv);
			break;

		case 0x60:
			DGOptionDataRecv((SDHP_SKILLKEYDATA_SEND *)aRecv);
			break;

		case 0x80:
			{
				PMSG_DEFAULT2 * lpDef1 = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpDef1->subcode )
				{
					case 0x00:
						GS_DGAnsCastleTotalInfo(aRecv);
						break;

					case 0x01:
						GS_DGAnsOwnerGuildMaster(aRecv);
						break;

					case 0x03:
						GS_DGAnsCastleNpcBuy(aRecv);
						break;

					case 0x04:
						GS_DGAnsCastleNpcRepair(aRecv);
						break;

					case 0x05:
						GS_DGAnsCastleNpcUpgrade(aRecv);
						break;

					case 0x06:
						GS_DGAnsTaxInfo(aRecv);
						break;

					case 0x07:
						GS_DGAnsTaxRateChange(aRecv);
						break;

					case 0x08:
						GS_DGAnsCastleMoneyChange(aRecv);
						break;

					case 0x09:
						GS_DGAnsSiegeDateChange(aRecv);
						break;

					case 0x0A:
						GS_DGAnsGuildMarkRegInfo(aRecv);
						break;

					case 0x0B:
						GS_DGAnsSiegeEndedChange(aRecv);
						break;

					case 0x0C:
						GS_DGAnsCastleOwnerChange(aRecv);
						break;

					case 0x0D:
						GS_DGAnsRegAttackGuild(aRecv);
						break;

					case 0x0E:
						GS_DGAnsRestartCastleState(aRecv);
						break;

					case 0x0F:
						GS_DGAnsMapSvrMsgMultiCast(aRecv);
						break;

					case 0x10:
						GS_DGAnsRegGuildMark(aRecv);
						break;

					case 0x11:
						GS_DGAnsGuildMarkReset(aRecv);
						break;

					case 0x12:
						GS_DGAnsGuildSetGiveUp(aRecv);
						break;

					case 0x16:
						GS_DGAnsNpcRemove(aRecv);
						break;

					case 0x17:
						GS_DGAnsCastleStateSync(aRecv);
						break;

					case 0x18:
						GS_DGAnsCastleTributeMoney(aRecv);
						break;

					case 0x19:
						GS_DGAnsResetCastleTaxInfo(aRecv);
						break;

					case 0x1A:
						GS_DGAnsResetSiegeGuildInfo(aRecv);
						break;

					case 0x1B:
						GS_DGAnsResetRegSiegeInfo(aRecv);
						break;
				}
			}
			break;

		case 0x81:
			GS_DGAnsCastleInitData(aRecv);
			break;

		case 0x82:
			GS_DGAnsCastleNpcInfo(aRecv);
			break;

		case 0x83:
			GS_DGAnsAllGuildMarkRegInfo(aRecv);
			break;

		case 0x84:
			GS_DGAnsFirstCreateNPC(aRecv);
			break;

		case 0x85:
			GS_DGAnsCalcREgGuildList(aRecv);
			break;

		case 0x86:
			GS_DGAnsCsGulidUnionInfo(aRecv);
			break;

		case 0x87:
			GS_DGAnsCsSaveTotalGuildInfo(aRecv);
			break;

		case 0x88:
			GS_DGAnsCsLoadTotalGuildInfo(aRecv);
			break;

		case 0x89:
			GS_DGAnsCastleNpcUpdate(aRecv);
			break;

		case 0xB0:
			DGAnsCrywolfSync(aRecv);
			break;

		case 0xB1:
			DGAnsCrywolfInfoLoad(aRecv);
			break;

		case 0xB2:
			DGAnsCrywolfInfoSave(aRecv);
			break;

		case 0xCF:
			DGMoveOtherServer((SDHP_CHARACTER_TRANSFER_RESULT *)aRecv);
			break;

		case 0xD0:
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpMsg->subcode )
				{
					case 0x02:
						g_CashItemPeriodSystem.DGAnsPeriodItemInquiry((PMSG_ANS_PERIODITEM_INQUIRY *)aRecv);
						break;

					case 0x04:
						g_CashItemPeriodSystem.DGAnsPeriodItemInsert((PMSG_ANS_PERIODITEM_INSERT *)aRecv);
						break;

					case 0x06:
						g_CashItemPeriodSystem.DGAnsPeriodItemList((PMSG_ANS_PERIODITEM_LIST *)aRecv);
						break;

					case 0x08:
						g_CashItemPeriodSystem.DGAnsPeriodItemUpdate((PMSG_ANS_PERIODITEM_UPDATE *)aRecv);
						break;
				}
			}
			break;

		//Season3
		case 0xA1:
			DGSummonerStateUpdatedSend((PMSG_ANS_SUMMONER_CREATE *)aRecv);
			break;
		case 0xA2:
			DGSummonerStateRecv((PMSG_ANS_SUMMONER_STATUS *)aRecv);
			break;
		case 0xFF:	// Test Packet
			{
				PMSG_TEST * pMsg = (PMSG_TEST *)aRecv;

				LogAddC(2, "error-L2DataServer RECV : %d (%d)", aLen, pMsg->count);
			}
			break;
			// --
#ifdef WZQUEST
		case 0xF6:
            g_QuestExpUserMng.UserQuestInfoLoad((PMSG_ANS_QUESTEXP_INFO *)aRecv);
            break;
#endif
#ifdef GENS
		case 0xF8: // Gens System
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;
				// ----
				switch(lpMsg->subcode)
				{
				case 0x01: //-> Get member info
					{
						DGAnsGensInfo((PMSG_ANS_GENS_INFO_EXDB*)aRecv);
					}
					break;
					// --
				case 0x03: //-> Register new member
					{
						DGAnsRegGensMember((PMSG_ANS_REG_GENS_MEMBER_EXDB*)aRecv);
					}
					break;
					// --
				case 0x04: //-> Delete member
					{
						 DGAnsSecedeGensMember((PMSG_ANS_SECEDE_GENS_MEMBER_EXDB*)aRecv);
					}
					break;
					// --
				case 0x05: //-> Reward answer
					{
						gGensSystem.DGRewardAns((PMSG_GENS_REWARD_ANS*)aRecv);
					}
					break;
				}
			}
			break;
			// --
#endif
			// --
#ifdef PERIOD
		case 0xD1:	//-> PeriodItemEx Main
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;
				// ----
				switch(lpMsg->subcode)
				{
				case 1:
					g_PeriodItemEx.DGPeriodItemExInsert((PMSG_ANS_PERIODITEMEX_INSERT*)aRecv);
					break;
				case 2:
					g_PeriodItemEx.DGPeriodItemExDelete((PMSG_ANS_PERIODITEMEX_DELETE*)aRecv);
					break;
				case 3:
					g_PeriodItemEx.DGPeriodItemExExpiredItemList((PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST*)aRecv);
					break;
				case 4:
					g_PeriodItemEx.DGPeriodItemExListCount((PMSG_ANS_PERIODITEMEX_LIST_COUNT*)aRecv);
					break;
				case 5:
					g_PeriodItemEx.DGPeriodItemExList((PMSG_ANS_PERIODITEMEX_LIST*)aRecv);
					break;
				}
			}
			break;
			// --
		case 0xD3:
			{
				PWMSG_HEAD2 * lpMsg2	= (PWMSG_HEAD2*)aRecv;
				// ----
				switch(lpMsg2->subcode)
				{
				case 1:
					{
						gGameShop.DGGetStorage((GAMESHOP_GD_STORAGE*)aRecv);
					}
					break;
				}
			}
			break;
			// --
#ifdef GAMESHOP
		case 0xD2:	//-> GameShop
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;
				// ----
				switch(lpMsg->subcode)
				{
				case 2:	//-> Answer point
					{
						gGameShop.DGGetPoint((GAMESHOP_DG_GET_POINT*)aRecv);
					}
					break;
				}
			}
			break;
#endif
			// --
		//case 0xD2:	//-> PeriodItemEx Select
		//	{
		//		g_PeriodItemEx.DGPeriodItemExSelect((PMSG_ANS_PERIODITEMEX_SELECT*)aRecv);
		//	}
		//	break;
			// --
		case 0xD4:	//-> LuckyItem Select
			{
				g_LuckyItemManager.DGAnsLuckyItemList((PMSG_ANS_LUCKYITEM_SELECT*)aRecv);
			}
			break;
#endif
			// --
#ifdef EXPINV
		case 0xD5:
			{
				DGUpdateExtendedInvenCountResult((SDHP_ANS_SET_EXTENDEDINVEN_COUNT*)aRecv);
			}
			break;
			// --
		case 0xD6:
			{
				DGUpdateExtendedWarehouseResult((SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT*)aRecv);
			}
			break;
#endif
	}
}

//0042BD70 - identical
void TestDSSend()
{
	PMSG_TEST pMsg;
	int size;

	size = sizeof(PMSG_TEST);
	pMsg.h.c=0xC2;
	pMsg.h.headcode =0xFF;
	pMsg.h.sizeH = SET_NUMBERH(size);
	pMsg.h.sizeL = SET_NUMBERL(size);

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf );
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	LogAdd("Test Data Send");
}

//0042C7D0 - identical
void DataServerLogin(int server)
{
	SDHP_SERVERINFO pInfo;

	pInfo.h.c =0xC1;
	pInfo.h.size = sizeof(SDHP_SERVERINFO);
	pInfo.h.headcode =0x00;
	pInfo.Port =(WORD)GameServerPort;
	pInfo.Type =1;
	pInfo.ServerCode =gGameServerCode;
	strcpy( (char*)&pInfo.ServerName[0], (const char*)&szServerName[0] );

	cDBSMng.Send(server, (char*)&pInfo , pInfo.h.size  );	
}

//0042C850 - identical
void DataServerLoginResult(SDHP_RESULT * lpMsg)
{
	if (lpMsg->Result == false )
	{
		MsgBox(lMsg.Get(MSGGET(1, 210)));	
		return;
	}
	
	DataServerConnected = TRUE;
	gServerReady++;
	SendMessage(ghWnd, WM_START_SERVER, 0, 0); 
	
}

struct PMSG_CHARLISTCOUNT
{
	PBMSG_HEAD h;	// C1:F3:00
	BYTE subcode;	// 3
	BYTE MaxClass;	// 4
	BYTE MoveCnt;	// 5
	BYTE Count;	// 6
#ifdef SEASON6DOT3_ENG
	bool ExpandedWarehouse;	//-> Season 6.3
#endif
};

struct PMSG_CHARLIST
{
	BYTE Index;	// 0
	char Name[10];	// 1
	WORD Level;	// C
	BYTE CtlCode;	// E
	BYTE CharSet[CHAR_SET_SIZE];	// F
	BYTE btGuildStatus;	// 21
};


//0042C8D0 - identical
void JGPGetCharList(BYTE *lpRecv)
{
	SDHP_CHARLISTCOUNT * lpCount = (SDHP_CHARLISTCOUNT *)lpRecv;
	SDHP_CHARLIST * lpCL;
	char szId[MAX_ACCOUNT_LEN+1];
	BYTE sendbuf[256];
	int lOfs = 0;
	int lsOfs = sizeof(SDHP_CHARLISTCOUNT);
	int aIndex = lpCount->Number;
	BYTE index;
	PMSG_CHARLISTCOUNT pCLCount;	// Packet Char List Count 7
	PMSG_CHARLIST pCList;
	WORD TempInventory[12];
	BYTE btExcellentOption;

	pCLCount.h.c		= 0xC1;
	pCLCount.h.headcode = 0xF3;
	pCLCount.subcode	= 0x00;
	pCLCount.Count		= lpCount->Count;

	szId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szId, lpCount->AccountId, MAX_ACCOUNT_LEN);

	if(gObjIsAccontConnect(aIndex, szId) == FALSE)
	{
		LogAddC(2, "Requested character list doesn't match the user. (%s)%s", gObj[aIndex].AccountID, szId);
		CloseClient(aIndex);
		return;
	}

	gObj[aIndex].Magumsa		= lpCount->Magumsa;
	pCLCount.MaxClass			= lpCount->Magumsa + 2;
	pCLCount.MaxClass			+= 1;
	pCLCount.MoveCnt			= lpCount->MoveCnt;
	// ----
#ifdef EXPINV
	pCLCount.ExpandedWarehouse		= lpCount->ExpandedWarehouse;
	gObj[aIndex].ExpandedWarehouse	= lpCount->ExpandedWarehouse;
#endif

	memset(sendbuf, 0, sizeof(sendbuf));
	lOfs += sizeof(PMSG_CHARLISTCOUNT);

	if( pCLCount.Count > 0 )
	{
#ifdef CHARCREATE_TEMP
		int HightLevel		= 0;
		BYTE GenerableClass = 0;
#endif
		for ( int n=0;n<pCLCount.Count ;n++)
		{
			memset(&pCList, 0, sizeof(pCList));
			lpCL = (SDHP_CHARLIST *)&lpRecv[lsOfs];
#ifdef CHARCREATE_TEMP
			if( lpCL->Level > HightLevel )
			{
				HightLevel = lpCL->Level;
			}
#endif
			pCList.Index = lpCL->Index;
			pCList.Level = lpCL->Level;
			pCList.CtlCode = lpCL->CtlCode;
			pCList.btGuildStatus = lpCL->btGuildStatus;

			if ( gObj[aIndex].m_cAccountItemBlock != 0 )
				pCList.CtlCode |= 0x10;	// Set Block Item

			memcpy(pCList.Name, lpCL->Name, MAX_ACCOUNT_LEN);

			pCList.CharSet[CS_CLASS] = CS_GET_CLASS(lpCL->Class);
			int changeup = CS_GET_CHANGEUP(lpCL->Class);
			pCList.CharSet[CS_CLASS] |= CS_SET_CHANGEUP(changeup);
			pCList.CharSet[CS_CLASS] |= CS_GET_MASTERCLASS(changeup);

			pCList.CharSet[9] = 0;

			if ( lpCL->dbInventory[0] == (BYTE)-1 && (lpCL->dbInventory[2] & 0x80) == 0x80 && (lpCL->dbInventory[3]& 0xF0) == 0xF0  )
			{
				TempInventory[0] = -1;
			}
			else
			{
				TempInventory[0] = (lpCL->dbInventory[0] + (lpCL->dbInventory[2]&0x80)*2)+ (lpCL->dbInventory[3]&0xF0)*32;
			}
			if ( lpCL->dbInventory[4] == (BYTE)-1 && (lpCL->dbInventory[6]& 0x80) == 0x80 && (lpCL->dbInventory[7]& 0xF0) == 0xF0  )
			{
				TempInventory[1] = -1;
			}
			else
			{
				TempInventory[1] = (lpCL->dbInventory[4] + (lpCL->dbInventory[6]&0x80)*2)+ (lpCL->dbInventory[7]&0xF0)*32;
			}
			if ( lpCL->dbInventory[8] == (BYTE)-1 && (lpCL->dbInventory[10]& 0x80) == 0x80 && (lpCL->dbInventory[11]& 0xF0) == 0xF0  )
			{
				TempInventory[2] = 0x1FF;
			}
			else
			{
				TempInventory[2] = ((lpCL->dbInventory[8] + (lpCL->dbInventory[10]&0x80)*2)+ (lpCL->dbInventory[11]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
			}
			if ( lpCL->dbInventory[12] == (BYTE)-1 && (lpCL->dbInventory[14]& 0x80) == 0x80 && (lpCL->dbInventory[15]& 0xF0) == 0xF0  )
			{
				TempInventory[3] = 0x1FF;
			}
			else
			{
				TempInventory[3] = ((lpCL->dbInventory[12] + (lpCL->dbInventory[14]&0x80)*2)+ (lpCL->dbInventory[15]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
			}
			if ( lpCL->dbInventory[16] == (BYTE)-1 && (lpCL->dbInventory[18]& 0x80) == 0x80 && (lpCL->dbInventory[19]& 0xF0) == 0xF0  )
			{
				TempInventory[4] = 0x1FF;
			}
			else
			{
				TempInventory[4] = ((lpCL->dbInventory[16] + (lpCL->dbInventory[18]&0x80)*2)+ (lpCL->dbInventory[19]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
			}
			if ( lpCL->dbInventory[20] == (BYTE)-1 && (lpCL->dbInventory[22]& 0x80) == 0x80 && (lpCL->dbInventory[23]& 0xF0) == 0xF0  )
			{
				TempInventory[5] = 0x1FF;
			}
			else
			{
				TempInventory[5] = ((lpCL->dbInventory[20] + (lpCL->dbInventory[22]&0x80)*2)+ (lpCL->dbInventory[23]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
			}
			if ( lpCL->dbInventory[24] == (BYTE)-1 && (lpCL->dbInventory[26]& 0x80) == 0x80 && (lpCL->dbInventory[27]& 0xF0) == 0xF0  )
			{
				TempInventory[6] = 0x1FF;
			}
			else
			{
				TempInventory[6] = ((lpCL->dbInventory[24] + (lpCL->dbInventory[26]&0x80)*2)+ (lpCL->dbInventory[27]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
			}
			if ( lpCL->dbInventory[28] == (BYTE)-1 && (lpCL->dbInventory[30]& 0x80) == 0x80 && (lpCL->dbInventory[31]& 0xF0) == 0xF0  )
			{
				TempInventory[7] = 0x1FF;
			}
			else
			{
				TempInventory[7] = ((lpCL->dbInventory[28] + (lpCL->dbInventory[30]&0x80)*2)+ (lpCL->dbInventory[31]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
			}
			if ( lpCL->dbInventory[32] == (BYTE)-1 && (lpCL->dbInventory[34]& 0x80) == 0x80 && (lpCL->dbInventory[35]& 0xF0) == 0xF0  )
			{
				TempInventory[8] = 0x1FF;
			}
			else
			{
				TempInventory[8] = ((lpCL->dbInventory[32] + (lpCL->dbInventory[34]&0x80)*2)+ (lpCL->dbInventory[35]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
			}

			pCList.CharSet[12] |= DBI_GET_TYPE(TempInventory[0]);
			pCList.CharSet[1] = TempInventory[0] % 256;

			pCList.CharSet[13] |= DBI_GET_TYPE(TempInventory[1]);
			pCList.CharSet[2] = TempInventory[1] % 256;

			pCList.CharSet[13] |= (int)(TempInventory[2] & 0x1E0) >> 5;
			pCList.CharSet[9] |= (int)(TempInventory[2] & 0x10) << 3;
			pCList.CharSet[3] |= (int)(TempInventory[2] & 0x0F) << 4;

			pCList.CharSet[14] |= (int)(TempInventory[3] & 0x1E0) >> 1;
			pCList.CharSet[9] |= (int)(TempInventory[3] & 0x10) << 2;
			pCList.CharSet[3] |= (int)(TempInventory[3] & 0x0F);

			pCList.CharSet[14] |= (int)(TempInventory[4] & 0x1E0) >> 5;
			pCList.CharSet[9] |= (int)(TempInventory[4] & 0x10) << 1;
			pCList.CharSet[4] |= (int)(TempInventory[4] & 0x0F) << 4;

			pCList.CharSet[15] |= (int)(TempInventory[5] & 0x1E0) >> 1;
			pCList.CharSet[9] |= (int)(TempInventory[5] & 0x10);
			pCList.CharSet[4] |= (int)(TempInventory[5] & 0x0F);

			pCList.CharSet[15] |= (int)(TempInventory[6] & 0x1E0) >> 5;
			pCList.CharSet[9] |= (int)(TempInventory[6] & 0x10) >> 1;
			pCList.CharSet[5] |= (int)(TempInventory[6] & 0x0F) << 4;

			index = 0;

			// Wings Set
			switch (TempInventory[7])
			{
			case 0x1FF: // No Wings
				index |= 0;
				break;
			case 0: // Wings of Elf
				pCList.CharSet[5] |= CS_SET_WING1(1);
				pCList.CharSet[9] |= 1;
				break;
			case 1: // Wings of Heaven
				pCList.CharSet[5] |= CS_SET_WING1(1);
				pCList.CharSet[9] |= 2;
				break;
			case 2: // Wings of Satan
				pCList.CharSet[5] |= CS_SET_WING1(1);
				pCList.CharSet[9] |= 3;
				break;
			case 41:
				pCList.CharSet[5] |= CS_SET_WING1(1);
				pCList.CharSet[9] |= 4;
				break;
			case 3: // Muse Elf Wings
				pCList.CharSet[5] |= CS_SET_WING1(2);
				pCList.CharSet[9] |= 1;
				break;
			case 4: // Soul Master Wings
				pCList.CharSet[5] |= CS_SET_WING1(2);
				pCList.CharSet[9] |= 2;
				break;
			case 5: // Blade Knight Wings
				pCList.CharSet[5] |= CS_SET_WING1(2);
				pCList.CharSet[9] |= 3;
				break;
			case 6: // Magic Gladiator Wings
				pCList.CharSet[5] |= CS_SET_WING1(2);
				pCList.CharSet[9] |= 4;
				break;
			case 30: // Cape of Lord
				pCList.CharSet[5] |= CS_SET_WING1(2);
				pCList.CharSet[9] |= 5;
				break;
			case 42: // Bloody Summoner Wings
				pCList.CharSet[5] |= CS_SET_WING1(2);
				pCList.CharSet[9] |= 6;
				break;
			case 49: // Cloak of Warrior
				pCList.CharSet[5] |= CS_SET_WING1(2);
				pCList.CharSet[9] |= 7;
				break;
			case 36: // Blade Master Wings
				pCList.CharSet[5] |= CS_SET_WING1(-1);
				pCList.CharSet[9] |= 1;
				break;
			case 37: // Grand Master Wings
				pCList.CharSet[5] |= CS_SET_WING1(-1);
				pCList.CharSet[9] |= 2;
				break;
			case 38: // High Elf Wings
				pCList.CharSet[5] |= CS_SET_WING1(-1);
				pCList.CharSet[9] |= 3;
				break;
			case 39: // Duel Master Wings
				pCList.CharSet[5] |= CS_SET_WING1(-1);
				pCList.CharSet[9] |= 4;
				break;
			case 40: // Lord Emperor Cape
				pCList.CharSet[5] |= CS_SET_WING1(-1);
				pCList.CharSet[9] |= 5;
				break;
			case 43: // Dimension Master Wings
				pCList.CharSet[5] |= CS_SET_WING1(-1);
				pCList.CharSet[9] |= 6;
				break;
			case 50: // Fist Master Cloak
				pCList.CharSet[5] |= CS_SET_WING1(-1);
				pCList.CharSet[9] |= 7;
				break;
			case 130: // Small Cape of Lord
				pCList.CharSet[5] |= CS_SET_WING1(3);
				pCList.CharSet[17] |= 32;
				break;
			case 131: // Small Wings of Misery
				pCList.CharSet[5] |= CS_SET_WING1(3);
				pCList.CharSet[17] |= 64;
				break;
			case 132: // Small Wings of Elf
				pCList.CharSet[5] |= CS_SET_WING1(3);
				pCList.CharSet[17] |= 96;
				break;
			case 133: // Small Wings of Heaven
				pCList.CharSet[5] |= CS_SET_WING1(3);
				pCList.CharSet[17] |= 128;
				break;
			case 134: // Small Wings of Satan
				pCList.CharSet[5] |= CS_SET_WING1(3);
				pCList.CharSet[17] |= 160;
				break;
			case 135: // Small Cloak of Warrior
				pCList.CharSet[5] |= CS_SET_WING1(3);
				pCList.CharSet[17] |= 192;
				break;
			}
#ifdef NEWWINGS
			if( IS_NEWWINGS(TempInventory[7]+ITEMGET(12, 0)) )
			{
				pCList.CharSet[5]	|= CS_SET_WING1(3);
				pCList.CharSet[17]	|= (BYTE)(TempInventory[7] - 180 + 1) << 2;
			}
#endif
			// Pets Set
			switch (TempInventory[8])
			{
			case 0x1FF:
				index |= 0x03;
				break;
			case 4: // Dark Horse
				index |= CS_SET_HELPER(-1);
				pCList.CharSet[12] |= 0x01;
				break;
			case 3: // Dinorant
				pCList.CharSet[10] |= 0x01;
				index |= (TempInventory[8] ) & 0x03;
				break;
			case 123: // Skeleton Pet
				pCList.CharSet[16] |= 96;
				break;
			case 80: // Panda Pet
				pCList.CharSet[16] |= 224;
				break;
			case 67: // Rudolf Pet
				pCList.CharSet[16] |= 0x80;
				break;
			case 64: // Demon Pet
				pCList.CharSet[16] |= 0x20;
				break;
			case 65: // Spirit of Guardian Pet
				pCList.CharSet[16] |= 0x40;
				break;
			case 106:	//JPN Unicorn
				pCList.CharSet[16] |= 0xA0;
				break;
			case 37:
				index |= CS_SET_HELPER(-1);
				pCList.CharSet[10] &= 0xFE;
				pCList.CharSet[12] &= 0xFE;
				pCList.CharSet[12] |= 0x04;
				pCList.CharSet[16] = 0;
				btExcellentOption = lpCL->dbInventory[34] & 0x3F;

				if ( (btExcellentOption&1) == 1 ) // Red Fenrir
				{
					pCList.CharSet[16] |= 0x01;
				}
				if ( (btExcellentOption&2) == 2 ) // Blue Fenrir
				{
					pCList.CharSet[16] |= 0x02;
				}
				if ( (btExcellentOption&4) == 4 ) // Gold Fenrir 
				{
					pCList.CharSet[17] |= 0x01;
				}
				break;
			default:
				index |= (TempInventory[8] ) & 0x03;
				break;
			}

			pCList.CharSet[5] |= index;

			int levelindex = 0;

			if ( TempInventory[0] != (WORD)-1 )
			{
				levelindex = LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[1]));
			}
			if ( TempInventory[1] != (WORD)-1 )
			{
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[5])) << 3;
			}
			if ( TempInventory[2] < 0x1FF )
			{
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[9])) << 6;
			}
			if ( TempInventory[3] < 0x1FF )
			{
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[13])) << 9;
			}
			if ( TempInventory[4] < 0x1FF )
			{
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[17])) << 12;
			}
			if ( TempInventory[5] < 0x1FF )
			{
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[21])) << 15;
			}
			if ( TempInventory[6] < 0x1FF )
			{
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[25])) << 18;
			}

			pCList.CharSet[6] = ((int)levelindex >> 0x10) & 0xFF;
			pCList.CharSet[7] = ((int)levelindex >> 0x08) & 0xFF;
			pCList.CharSet[8] = ((int)levelindex) & 0xFF;

			memcpy(&sendbuf[lOfs], &pCList, sizeof(pCList));
			lsOfs += sizeof(SDHP_CHARLIST);
			lOfs += sizeof(pCList);
		}
		// ----
#ifdef CHARCREATE_TEMP
		if( HightLevel >= gCreateSUMLevel )
		{
			GenerableClass += 1;
		}
		// ----
		if( HightLevel >= gCreateMONKLevel )
		{
			GenerableClass += 8;
		}
		// ----
		if( HightLevel >= gCreateMGLevel )
		{
			GenerableClass += 4;
		}
		// ----
		if( HightLevel >= gCreateDLLevel )
		{
			GenerableClass += 2;
		}
		// ----
		PMSG_UPD_SUMMONER_CREATE pMsg;
		pMsg.h.set((LPBYTE)&pMsg, 0xDE, sizeof(pMsg));
		pMsg.btResult = 0;
		pMsg.IsSummonerEnable = GenerableClass;
		// ----
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
#endif
	}
	pCLCount.h.size = lOfs;
	memcpy(sendbuf, &pCLCount, sizeof(PMSG_CHARLISTCOUNT));	

	DataSend(aIndex, sendbuf, lOfs);
}

struct SDHP_GETCHARLIST
{
	PBMSG_HEAD h;	// C1:01
	char Id[10];	// 3
	short Number;	// E
};

//0042F010 - identical
void DataServerGetCharListRequest(short aIndex)
{
	SDHP_GETCHARLIST pMsg;
	char TempId[11];
	char* AccountId;

	memset(TempId, 0, sizeof(TempId));
	AccountId = gObjGetAccountId(aIndex);

	if ( AccountId == 0 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 166)), __FILE__, __LINE__);
		CloseClient(aIndex);
		return;
	}

	// When Len is Less than 1
	if ( strlen(AccountId) < 1 )
	{
		// Error Message
		//
		LogAdd(lMsg.Get(MSGGET(1, 167)), __FILE__, __LINE__);
		CloseClient(aIndex);
		return;
	}

	strcpy((char*)TempId, AccountId);

	pMsg.h.c  = 0xC1;
	pMsg.h.size  = sizeof(SDHP_GETCHARLIST);
	pMsg.h.headcode = 0x01;
	pMsg.Number = aIndex;
	memcpy(pMsg.Id , TempId, sizeof(TempId)-1);

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
	
}

struct PMSG_CHARCREATERESULT
{
	PBMSG_HEAD h;	// C1:F3:01
	BYTE subcode;	// 3
	BYTE Result;	// 4
	BYTE Name[10];	// 5
	BYTE pos;	// F
	WORD Level;	// 10
	BYTE Class;	// 12
	BYTE Equipment[24];	// 13
};

//0042F160 - identical
void JGCharacterCreateRequest(SDHP_CREATECHARRESULT* lpMsg)
{
	PMSG_CHARCREATERESULT pResult;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	int aIndex = lpMsg->Number;

	szAccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(szAccountID, lpMsg->AccountId, sizeof(lpMsg->AccountId));

	if ( gObjIsAccontConnect(aIndex, szAccountID) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 168)), szAccountID);
		CloseClient(aIndex);
		return;
	}

	pResult.h.c = 0xC1;
	pResult.h.size = sizeof(PMSG_CHARCREATERESULT);
	pResult.h.headcode = 0xF3;
	pResult.subcode = 0x01;
	pResult.Result = lpMsg->Result;
	pResult.pos = lpMsg->Pos;
	pResult.Class = (lpMsg->ClassSkin >> 4 << 5) & 0xE0;
	int changeup = lpMsg->ClassSkin & 0x07; 
	pResult.Class |= (changeup << 4) & 0x10;
	pResult.Class |= (changeup << 4) & 0x20; //Season 2.5 add-on
	pResult.Level = lpMsg->Level;
	memcpy(pResult.Name, lpMsg->Name, sizeof(pResult.Name));

	if ( pResult.Result == 1 )
	{
		memcpy(pResult.Equipment, lpMsg->Equipment, sizeof(pResult.Equipment));
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

//0042F2F0 - identical
void JGCharacterCreateFailSend(int aIndex, char* id)
{
	PMSG_CHARCREATERESULT  pResult;

	pResult.h.c = 0xC1;
	pResult.h.size = sizeof (PMSG_CHARCREATERESULT);
	pResult.h.headcode = 0xF3;
	pResult.subcode = 0x01;
	pResult.Result = false;
	pResult.pos = 0;
	memcpy(pResult.Name, id, sizeof(pResult.Name));

	DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}

//0042F360 - identical
void JGCharDelRequest(SDHP_CHARDELETERESULT * lpMsg)
{
	PMSG_RESULT pResult;
	char szAccountId[MAX_ACCOUNT_LEN+1];
	int aIndex = lpMsg->Number;
	szAccountId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szAccountId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	if ( ::gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 169)), szAccountId);
		CloseClient(aIndex);

		return;
	}

	pResult.h.c = 0xC1;
	pResult.h.size = sizeof(PMSG_RESULT);
	pResult.h.headcode = 0xF3;
	pResult.subcode = 0x02;
	pResult.result = lpMsg->Result;

	DataSend(lpMsg->Number, (UCHAR*)&pResult, pResult.h.size);
}

//#pragma pack(push, 1)
struct PMSG_CHARMAPJOINRESULT
{
	PBMSG_HEAD h;	// C3:F3:03
	BYTE subcode;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7

	BYTE ExpHHH; //
	BYTE ExpHHL; //
	BYTE ExpHLH; //
	BYTE ExpHLL; //
	BYTE ExpLHH; //
	BYTE ExpLHL; //
	BYTE ExpLLH; //
	BYTE ExpLLL; //

	BYTE NextExpHHH; //
	BYTE NextExpHHL; //
	BYTE NextExpHLH; //
	BYTE NextExpHLL; //
	BYTE NextExpLHH; //
	BYTE NextExpLHL; //
	BYTE NextExpLLH; //
	BYTE NextExpLLL; //

	WORD LevelUpPoint;	// 10
	WORD Str;	// 12
	WORD Dex;	// 14
	WORD Vit;	// 16
	WORD Energy;	// 18
	WORD Life;	// 1A
	WORD MaxLife;	// 1C
	WORD Mana;	// 1E
	WORD MaxMana;	// 20
	WORD wShield;	// 22
	WORD wMaxShield;	// 24
	WORD BP;	// 26
	WORD MaxBP;	// 28
	int Money;	// 2C
	BYTE PkLevel;	// 30
	BYTE CtlCode;	// 31
	short AddPoint;	// 32
	short MaxAddPoint;	// 34
	WORD Leadership;	// 36
	WORD wMinusPoint;	// 38
	WORD wMaxMinusPoint;	// 3A
#ifdef EXPINV
	BYTE ExpandedInventoryLevel;	//0x44
#endif
	int UpPoint;
};
//#pragma pack(pop)

#ifdef QUESTSYSTEM_TEMP
struct PMSG_QUESTSYSTEM_TEMP
{
	PBMSG_HEAD2 h;
	// ----
	BYTE		Result; 
};
#endif

//004386c0
void JGGetCharacterInfo( SDHP_DBCHAR_INFORESULT * lpMsg)
{
	PMSG_CHARMAPJOINRESULT pjMsg;
	// ----
	char szAccountId[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	int aIndex = lpMsg->Number;
	// ----
	szAccountId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szAccountId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	if ( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 170)), szAccountId);
		CloseClient(aIndex);
		return;
	}

	szName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szName, lpMsg->Name, MAX_ACCOUNT_LEN);

	if ( lpMsg->result == false )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 171)), szName, gObj[aIndex].AccountID);
		CloseClient(aIndex);
		return;
	}

	if ( (lpMsg->CtlCode&1) != 0 )
	{
		LogAddC(2, "error-L1 : Load Block Character infomation", szName, gObj[aIndex].AccountID);
		CloseClient(aIndex);
		return;
	}

	for (int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				if ( !strncmp(szName, gObj[i].Name, MAX_ACCOUNT_LEN) || !strncmp(szAccountId, gObj[i].AccountID, MAX_ACCOUNT_LEN))
				{
					LogAddTD("[Anti-HACK][JGGetCharacterInfo] Attempted Character-Copy by double logging [%s][%s]",	szName, gObj[aIndex].AccountID);
					CloseClient(aIndex);
					return;
				}
			}
		}
	}

	if( gObjSetCharacter( (LPBYTE)lpMsg, aIndex) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 172)), szName);
		CloseClient(aIndex);
		return;
	}

	gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;

	if ( szAuthKey[0] != AUTHKEY0 )
	{
		DestroyGIocp();
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( gObj[aIndex].m_Index != aIndex )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 173)), __FILE__, __LINE__);
		CloseClient(aIndex);
		return;
	}

	if ( lpObj->Money < 0 )
	{
		LogAddC(2, "¡Ú¡Ù ZEN MODIFY ¡Ù¡Ú [%s][%s] %d -> 0", szName, gObj[aIndex].AccountID, lpObj->Money);
		lpObj->Money = 0;
	}

	if ( lpObj->m_bMapSvrMoveReq == false )
	{
		if ( MapNumberCheck(lpObj->MapNumber) == FALSE )
		{
			LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
			CloseClient(aIndex);

			return;
		}

		short wGameServerCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->MapNumber, lpObj->m_sPrevMapSvrCode);

		if ( wGameServerCode != gGameServerCode )
		{
			if ( wGameServerCode == -1 )
			{
				LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpObj->m_Index);
				CloseClient(aIndex);
				return;
			}

			GJReqMapSvrMove(lpObj->m_Index, wGameServerCode, lpObj->MapNumber, lpObj->X, lpObj->Y);
			LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)", wGameServerCode, lpObj->AccountID, lpObj->Name, lpObj->m_Index);
			return;
		}
	}

	lpObj->m_bMapSvrMoveReq = false;

	pjMsg.h.c			= 0xC3;
	pjMsg.h.headcode	= 0xF3;
	pjMsg.h.size		= sizeof(PMSG_CHARMAPJOINRESULT); //0x44
	pjMsg.subcode		= 0x03;

	pjMsg.MapX			= lpObj->X; //124
	pjMsg.MapY			= lpObj->Y; //126
	pjMsg.MapNumber		= lpObj->MapNumber; //129
	pjMsg.Dir			= lpObj->Dir; //128

	__int64 Experience = lpObj->Experience; //0AC

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) != FALSE) //Set ML Exp Info
	{
		Experience = lpObj->m_i64MasterLevelExp;
	}

	pjMsg.ExpHHH = SET_NUMBERH(SET_NUMBERHW(HIDWORD(Experience)));
	pjMsg.ExpHHL = SET_NUMBERL(SET_NUMBERHW(HIDWORD(Experience)));
	pjMsg.ExpHLH = SET_NUMBERH(SET_NUMBERLW(HIDWORD(Experience)));
	pjMsg.ExpHLL = SET_NUMBERL(SET_NUMBERLW(HIDWORD(Experience)));
	pjMsg.ExpLHH = SET_NUMBERH(SET_NUMBERHW(LODWORD(Experience)));
	pjMsg.ExpLHL = SET_NUMBERL(SET_NUMBERHW(LODWORD(Experience)));
	pjMsg.ExpLLH = SET_NUMBERH(SET_NUMBERLW(LODWORD(Experience)));
	pjMsg.ExpLLL = SET_NUMBERL(SET_NUMBERLW(LODWORD(Experience)));

	__int64 NextExperience = lpObj->NextExp;

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) != FALSE) //Set ML NextExp Info
	{
		NextExperience = lpObj->m_i64NextMasterLevelExp;
	}

	pjMsg.NextExpHHH = SET_NUMBERH(SET_NUMBERHW(HIDWORD(NextExperience)));
	pjMsg.NextExpHHL = SET_NUMBERL(SET_NUMBERHW(HIDWORD(NextExperience)));
	pjMsg.NextExpHLH = SET_NUMBERH(SET_NUMBERLW(HIDWORD(NextExperience)));
	pjMsg.NextExpHLL = SET_NUMBERL(SET_NUMBERLW(HIDWORD(NextExperience)));
	pjMsg.NextExpLHH = SET_NUMBERH(SET_NUMBERHW(LODWORD(NextExperience)));
	pjMsg.NextExpLHL = SET_NUMBERL(SET_NUMBERHW(LODWORD(NextExperience)));
	pjMsg.NextExpLLH = SET_NUMBERH(SET_NUMBERLW(LODWORD(NextExperience)));
	pjMsg.NextExpLLL = SET_NUMBERL(SET_NUMBERLW(LODWORD(NextExperience)));

	pjMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pjMsg.Str = lpObj->Strength;
	pjMsg.Dex = lpObj->Dexterity;
	pjMsg.Vit = lpObj->Vitality;
	pjMsg.Energy = lpObj->Energy;
	pjMsg.Money = lpObj->Money;
	pjMsg.PkLevel = lpObj->m_PK_Level;
	pjMsg.Life = lpObj->Life;
	pjMsg.MaxLife = lpObj->AddLife + lpObj->MaxLife;
	pjMsg.Mana = lpObj->Mana;
	pjMsg.MaxMana = lpObj->AddMana + lpObj->MaxMana;
	pjMsg.wShield = lpObj->iShield;
	pjMsg.wMaxShield = lpObj->iMaxShield + lpObj->iAddShield;
	pjMsg.CtlCode = lpMsg->CtlCode;
	pjMsg.BP = lpObj->BP;
	pjMsg.MaxBP = lpObj->MaxBP + lpObj->AddBP;
	pjMsg.Leadership = lpObj->Leadership;
	pjMsg.UpPoint = lpObj->LevelUpPoint;

	short AddPoint = 0;
	short MaxAddPoint = 0;
	short MinusPoint = 0;
	short MaxMinusPoint = 0;
	
#ifdef EXPINV
	pjMsg.ExpandedInventoryLevel = lpObj->ExpandedInventory;
#endif

	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);

	pjMsg.AddPoint = AddPoint;
	pjMsg.MaxAddPoint = MaxAddPoint;
	pjMsg.wMinusPoint = MinusPoint;
	pjMsg.wMaxMinusPoint = MaxMinusPoint;

	LogAddTD("[FRUIT System] [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)", lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);

	if ( AddPoint < 0 || AddPoint > MaxAddPoint || MinusPoint < 0 || MinusPoint > MaxMinusPoint )
	{
		LogAddTD("[FRUIT System] Character Stat Error [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)", lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);
	}

	g_QuestInfo.QuestInfoSave(lpObj);
	
	DataSend(aIndex, (LPBYTE)&pjMsg, pjMsg.h.size);
	// ----
#ifdef MUHELPER
	g_MUHelper.ReqMacro(lpObj->m_Index);
#endif
#ifdef __CUSTOMS__
	g_ResetSystem.SendResetData(lpObj, false);
#endif
#ifdef __NOVUS__
	g_CraftSystem.SendCraftData(lpObj, false);
#endif
	// ----
	GCItemListSend(aIndex);
	GCMagicListMultiSend(lpObj, 0);

#ifdef PERIOD
	g_PeriodItemEx.RequestPeriodItemInfo(lpObj);	//1.01.00
#endif
	g_LuckyItemManager.GDReqLuckyItemSelect(lpObj);	//1.01.00

	g_MasterLevelSystem.GDReqMasterLevelInfo(lpObj);

	CGRequestQuestInfo(aIndex);
	
	//Season 2.5 Quest New CALL
	g_QuestUtil.Quest3rdRequestInfo(lpObj);

	DGGuildMemberInfoRequest(aIndex);
#ifdef GENS
	gGensSystem.ReqExDBGensInfo(lpObj);
	//gGensSystem.GDReqAbusingInfo(lpObj);
	//Off for time
	//gGensSystem.ReqGensRewardDay(lpObj);
#endif

	//1.01.03:
	g_HacktoolBlockEx.CheckHacktoolUseConnectedUser(lpObj);

	int TempSlotIndex = CTemporaryUserManager::Instance()->CheckMatchCharacter(lpObj->Name);
	// ----
	if( TempSlotIndex != -1 )
	{
		CTemporaryUserManager::Instance()->SendEventMapInfo(TempSlotIndex, aIndex);
		CTemporaryUserManager::Instance()->AddToPriviousParty(lpObj->m_Index, TempSlotIndex);
	}
	else
	{
		/*if( lpObj->m_ExChannelCode != -1 && lpObj->m_ExChannelCode != gGameServerCode && lpObj->m_ExChannelCode < 899 )
		{
			PHeadSubSetB(&v14, 0xF1u, 6u, 6);
			v16 = 1;
			v17 = lpObj->m_ExChannelCode % 20 + 1;
			DataSend(v28, &v14, v15);
			return;
		}*/
	}

	FriendListRequest(aIndex);

	if ( gTamaJJangEvent != FALSE )
	{
		if ( gAppearTamaJJang != 0 )
		{
			if ( lpObj->MapNumber == MAP_INDEX_RORENCIA )
			{
				GCMapEventStateSend(MAP_INDEX_RORENCIA, gAppearTamaJJang, 2);
			}
		}
	}

	gObjUseSkill.SkillChangeUse(lpObj->m_Index);
#ifdef WZQUEST
	g_QuestExpUserMng.DB_ReqUserQuestInfo(aIndex);
#endif
	// ----
#ifdef QUESTSYSTEM_TEMP
	PMSG_QUESTSYSTEM_TEMP pResult;
	pResult.h.set((LPBYTE)&pResult, 0xF6, 0x1A, sizeof(pResult));
	pResult.Result = NULL;
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size); 
#endif

	if( lpObj->Reset < g_ServerMinUserReset || lpObj->Reset > g_ServerMaxUserReset )
	{
		char Text[100];
		ZeroMemory(Text, 100);
		sprintf(Text, "Server requirements: %d - %d Reset", g_ServerMinUserReset, g_ServerMaxUserReset);
		GCServerMsgStringSend(Text, lpObj->m_Index, 0);
		gObjCloseSet(aIndex, 2);
		return;
	}

#ifdef __CUSTOMS__
	GCServerMsgStringSend(gWelcomeMessage, lpObj->m_Index, 0);
	// ----
	if( g_NewsBoard.m_ShowOnLogin )
	{
		g_NewsBoard.OpenMain(lpObj);
	}
#endif
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Item List Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC4
 *	HeadCode : 0xF3
 *	SubCode  : 0x10
 *	BASE PACKET - Complete with PMSG_INVENTORYLIST
 */
struct PMSG_INVENTORYLISTCOUNT
{
	PWMSG_HEAD h;	// C4:F3:10
	BYTE subcode;	// 4
	BYTE Count;	// 5
};

struct PMSG_INVENTORYLIST
{
	BYTE Pos;	// 0
	BYTE ItemInfo[MAX_ITEM_INFO];	// 1
};

//004300E0 - identical
void GCItemListSend(int aIndex)
{
	PMSG_INVENTORYLISTCOUNT pMsgILC;	// Packet Message Item List Count
	PMSG_INVENTORYLIST pMsgIL;	// Packet Message Item List
	int sOfs = sizeof(PMSG_INVENTORYLISTCOUNT);
	int pMsgILSize = sizeof(PMSG_INVENTORYLIST);
	BYTE sendBuf[WAREHOUSE_BUFF];
	LPOBJ lpObj = &gObj[aIndex];
	int itemcount = 0;
	
	for ( int n=0;n<INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_bItemExist == false )
			{
				continue;
			}

			pMsgIL.Pos = n;
			ItemByteConvert(pMsgIL.ItemInfo,lpObj->pInventory[n]);
			memcpy(&sendBuf[sOfs], &pMsgIL, pMsgILSize);
			itemcount++;
			sOfs += pMsgILSize;
		}
	}

	pMsgILC.h.c = 0xC4;
	pMsgILC.h.headcode = 0xF3;
	pMsgILC.subcode = 0x10;
	pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
	pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
	pMsgILC.Count = itemcount;
	memcpy(sendBuf, &pMsgILC, sizeof(PMSG_INVENTORYLISTCOUNT));

	DataSend(aIndex, sendBuf, sOfs);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Save Character Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x07
 */
struct SDHP_DBCHAR_INFOSAVE
{
	PWMSG_HEAD h;	// C2:07
	char Name[11];	// 4
	short Level;	// 10
	BYTE Class;	// 12
	int LevelUpPoint;	// 14
	int Exp;	// 18
	int NextExp;	// 1C
	int Money;	// 20
#if( FIX_MAXSTAT == 1 )
	WORD Str;	// 24
	WORD Dex;	// 26
	WORD Vit;	// 28
	WORD Energy;	// 2A
#else
	short Str;	// 24
	short Dex;	// 26
	short Vit;	// 28
	short Energy;	// 2A
#endif
	WORD Life;	// 2C
	WORD MaxLife;	// 2E
	WORD Mana;	// 30
	WORD MaxMana;	// 32
	BYTE dbInventory[DB_MAX_INVENTORY_SIZE];	// 34
	BYTE dbMagicList[MAX_MAGIC*3];	// 6F4
	BYTE MapNumber;	// 7A8
	BYTE MapX;	// 7A9
	BYTE MapY;	// 7AA
	BYTE Dir;	// 7AB
	int PkCount;	// 7AC
	int PkLevel;	// 7B0
	int PkTime;	// 7B4
	BYTE dbQuest[50];	// 7B8
	BYTE CharInfoSave;	// 7EA
	WORD Leadership;	// 7EC
	WORD ChatLitmitTime;	// 7EE
	int iFruitPoint;	// 7F0
	int	Reset;
	int GrandReset;
#ifdef EXPINV
	int ExpandedInventory;
#endif
#ifdef __NOVUS__
	WORD CraftLevel;
	WORD CraftStage;
	WORD CraftQuest;
#endif
#ifdef __ALIEN__
	int DbOffExp;
#endif
};

//00439780	->
void GJSetCharacterInfo(LPOBJ lpObj, int aIndex, BOOL bMapServerMove, BYTE byExMapNumber)
{
	if ( lpObj->m_bMapSvrMoveQuit == true )
	{
		LogAddC(2,"[MapServerMng] GJSetCharacterInfo() - Inventory Already Saved [%s][%s] (%d)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Index);

		return;
	}

	if ( bMapServerMove == TRUE )
	{
		lpObj->m_bMapSvrMoveQuit = TRUE;

		LogAddC(5, "[MapServerMng][%s][%s] GJSetCharacterInfo() - MapServerMove Character Save Start (%d)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Index);
	}


	if ( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type == 6 && lpObj->m_IfState.state == 1)
	{
		GDSetWarehouseList(aIndex);
	}

	if ( szAuthKey[1] != AUTHKEY1 )
	{
		DestroyGIocp();
	}

	SDHP_DBCHAR_INFOSAVE pCSave;
 
	pCSave.h.c = 0xC2;
	pCSave.h.headcode = 0x07;
	pCSave.h.sizeH = sizeof(SDHP_DBCHAR_INFOSAVE) >> 8;
	pCSave.h.sizeL = sizeof(SDHP_DBCHAR_INFOSAVE) & 0xFF;
	pCSave.Name[MAX_ACCOUNT_LEN] = 0;
	memcpy(pCSave.Name, lpObj->Name, sizeof(pCSave.Name)-1);
	pCSave.Class = lpObj->DbClass;
	pCSave.Level = lpObj->Level;
	pCSave.LevelUpPoint = lpObj->LevelUpPoint;
	pCSave.Exp = lpObj->Experience;
	pCSave.NextExp = lpObj->NextExp;
	pCSave.Money = lpObj->Money;
	pCSave.Str = lpObj->Strength;
	pCSave.Dex = lpObj->Dexterity;
	pCSave.Vit = lpObj->Vitality;
	pCSave.Energy = lpObj->Energy;
	pCSave.Life = lpObj->Life*10.0f;
	pCSave.MaxLife =lpObj->MaxLife*10.0f;
	pCSave.Mana = lpObj->Mana*10.0f;
	pCSave.MaxMana = lpObj->MaxMana*10.0f;
	ItemByteConvert16((LPBYTE)pCSave.dbInventory , lpObj->pInventory, INVENTORY_SIZE);
	MagicByteConvert(pCSave.dbMagicList, lpObj->Magic, MAGIC_SIZE);
	pCSave.MapNumber = lpObj->MapNumber;
	pCSave.MapX = lpObj->X;
	pCSave.MapY = lpObj->Y;
	pCSave.Dir = lpObj->Dir;
	pCSave.PkCount = lpObj->m_PK_Count;
	pCSave.PkLevel = lpObj->m_PK_Level;
	pCSave.PkTime = lpObj->m_PK_Time;
	pCSave.Leadership = lpObj->Leadership;
	pCSave.ChatLitmitTime = lpObj->ChatLitmitTime;
	pCSave.iFruitPoint = lpObj->iFruitPoint;
	pCSave.Reset = lpObj->Reset;
	pCSave.GrandReset = lpObj->GrandReset;
#ifdef EXPINV
	pCSave.ExpandedInventory = lpObj->ExpandedInventory;

	if( pCSave.ExpandedInventory < 0 || pCSave.ExpandedInventory > 2 )
	{
		pCSave.ExpandedInventory = 0;
	}
#endif

#ifdef __ALIEN__
	pCSave.DbOffExp = lpObj->DbOffExp;
#endif

	pCSave.CharInfoSave = 0;
	memcpy(pCSave.dbQuest, lpObj->m_Quest, sizeof(pCSave.dbQuest));

	if ( cDBSMng.Send((char*)&pCSave, sizeof(SDHP_DBCHAR_INFOSAVE)) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 174)), lpObj->Name);
		return;
	}

	gObjItemTextSave(lpObj);
	gObjStatTextSave(lpObj);
	gObjSavePetItemInfo(aIndex, 0);
	g_LuckyItemManager.GDReqLuckyItemInsert2nd(aIndex);//Season 6 up (1.01.00)
	g_QuestUtil.Quest3rdSaveInfo(lpObj);//Season 2.5 Quest New CALL
	g_MasterLevelSystem.GDReqMasterLevelInfoSave(lpObj);//Season 3.0 Master Level New CALL
#ifdef WZQUEST
	g_QuestExpUserMng.UserAllQuestInfoSave(lpObj->m_Index);
#endif
#ifdef GENS
	gGensSystem.GDReqSaveContributePoint(lpObj);
	//gGensSystem.DBSaveAbusingKillUserName(lpObj);
#endif

#ifdef GAMESHOP
	gGameShop.GDSaveUserInfo(aIndex);
#endif
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Warehouse Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x08
 */
struct SDHP_GETWAREHOUSEDB
{
	PBMSG_HEAD h;	// C1:08
	char AccountID[10];	// 3
	short aIndex;	// E
};

//00430720 - identical
void GDGetWarehouseList(int aIndex, char* AccountID)
{
	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETWAREHOUSEDB	pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x08;
	pMsg.h.size = sizeof(SDHP_GETWAREHOUSEDB);
	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, AccountID, sizeof(pMsg.AccountID));

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//004307A0 - identical
void DGGetWarehouseList(SDHP_GETWAREHOUSEDB_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	PMSG_TALKRESULT pResult;

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int itype;
	int _type;
	BYTE OptionData;
	CItem item;

	memset(gObj[aIndex].pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);
	gObj[aIndex].WarehouseMoney = lpMsg->Money;
	gObj[aIndex].WarehousePW = lpMsg->pw;
	int dbsize = 7;

	if ( lpMsg->DbVersion >= 3 )
	{
		dbsize = 16;
	}
	else if ( lpMsg->DbVersion >= 1 )
	{
		dbsize = 10;
	}

	for ( int n=0;n<WAREHOUSE_SIZE;n++)
	{
		itype = lpMsg->dbItems[n*dbsize];
		
		if ( dbsize == 16 )
		{
			itype |= (lpMsg->dbItems[n*dbsize+9] & 0xF0) << 5;
			itype |= (lpMsg->dbItems[n*dbsize+7] & 0x80) << 1;
			_type = itype;
		}
		else if ( dbsize == 10 )
		{
			if ( ((lpMsg->dbItems[n*dbsize+7] >> 7) & 0x01) )
			{
				itype += 256;
			}

			_type = (itype / 32 * MAX_SUBTYPE_ITEMS) + itype % 32;
		}
		else
		{
			_type = (itype / MAX_TYPE_ITEMS * MAX_SUBTYPE_ITEMS) + itype % 16;
		}

		gObj[aIndex].pWarehouse[n].Clear();

		if ( dbsize >= 16 )
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 && (lpMsg->dbItems[n*dbsize+9] & 0xF0) == 0xF0 && (lpMsg->dbItems[n*dbsize+7] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else if ( dbsize >= 10 )
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 && (lpMsg->dbItems[n*dbsize+7] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 )
			{
				itype = -1;
			}
		}

		if ( IsItem(_type) == FALSE )
		{
			itype = -1;
		}

		if ( _type == ITEMGET(14,17) || _type == ITEMGET(14,18) )
		{
			if ( lpMsg->dbItems[n*dbsize+2]== 0 )
			{
				_type = -1;
			}
		}

		if ( _type == ITEMGET(14,19) )
		{
			if ( lpMsg->dbItems[n*dbsize+2] == (BYTE)-1 )
			{
				itype = -1;
			}
		}

		if ( itype != -1 )
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbItems[n*dbsize+1]);

			if ( lpMsg->DbVersion	< 2 )
			{
				lpMsg->dbItems[n*dbsize+8] = -1;
				lpMsg->dbItems[n*dbsize+9] = -1;
			}

			if ( _type != ITEMGET(14,11) )
			{
				if ( item.m_Level > 15 ) // [MaxLevel->15] Season 5 Episode 2 JPN
				{
					item.m_Level = 0;
				}
			}

			OptionData = lpMsg->dbItems[n*dbsize+1];
			item.m_Option1 = ( (OptionData) >> 7 );
			item.m_Option2 = DBI_GET_LUCK(OptionData);
			item.m_Option3 = DBI_GET_OPTION(OptionData);

			if ( dbsize >= 10 )
			{
				#if(GS_LIFE_28_UP == 1)
				if ( dbsize >= 10 )
				{
					item.m_Option3 |= DBI_GET_DINO_OPTION(lpMsg->dbItems[n*dbsize+7]);
				}
				#else
				if ( _type == ITEMGET(13,3) )
				{
					item.m_Option3 |= DBI_GET_DINO_OPTION(lpMsg->dbItems[n*dbsize+7]);
				}
				else
				{
					if ( DBI_GET_OPTION16(lpMsg->dbItems[n*dbsize+7]) == 0x40 )
					{
						item.m_Option3 = 4;
					}
				}
				#endif
			}

			item.m_Durability = lpMsg->dbItems[n*dbsize+2];

			if ( dbsize >= 16 )
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbItems[n*dbsize+10];
			}

			if ( dbsize >= 16 )
			{
				item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbItems[n*dbsize+DBI_OPTION380_DATA]);

				if ( item.m_ItemOptionEx != 0 )
				{
					item.m_Type = itype;
					if ( g_kItemSystemFor380.Is380Item(&item)== FALSE )
					{
						item.m_ItemOptionEx = 0;

						LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Warehouse pos[%d]", lpObj->AccountID, lpObj->Name, n);
					}
				}
			}
			//SocketSystem Season 4.5 addon start
			BYTE SocketOption[5];
			BYTE SocketOptionIndex = 0xFF;
			int i = 0;
			memset(&SocketOption, 0xFF, 5);
			
			if ( gItemSocketOption.IsEnableSocketItem(itype) == 1 )
			{
				SocketOptionIndex = lpMsg->dbItems[n*dbsize+10];

				for(i = 0; i < 5; i++)
				{
					SocketOption[i] =  lpMsg->dbItems[((n*dbsize)+11)+i];
				}
			}
			else
			{
				SocketOptionIndex = 0;

				for(i = 0; i < 5; i++)
				{
					SocketOption[i] = 0xFF;
				}
			}
			//SocketSystem Season 4.5 addon end
#ifdef PERIOD
			if( dbsize >= 16 )	//1.01.00
			{
				item.m_PeriodItemOption = (lpMsg->dbItems[n*dbsize+DBI_OPTION380_DATA] & 6) >> 1;
			}
#endif
			if ( lpMsg->DbVersion == 0 )
			{
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, 0, 0, 0, NULL, 0xFF,0, 3);
			}
			else if ( lpMsg->DbVersion <= 2 )
			{
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]), lpMsg->dbItems[n*dbsize+DBI_SOPTION_DATA], item.m_ItemOptionEx, 0, 0xFF, item.m_PeriodItemOption, 1);
			}
			else
			{
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]), lpMsg->dbItems[n*dbsize+DBI_SOPTION_DATA], item.m_ItemOptionEx, SocketOption, SocketOptionIndex, item.m_PeriodItemOption, 3);
			}

			if ( _type == ITEMGET(14,7) )
			{
				
			}
			else if ( (_type >= ITEMGET(14,0) && _type <= ITEMGET(14,8)) || ( _type >= ITEMGET(14,35) && _type <= ITEMGET(14,40) ))
			{
				if ( item.m_Durability == 0.0f )
				{
					item.m_Durability = 1.0f;
				}

				if ( item.m_Durability > 255.0f )
				{
					item.m_Durability = 255.0f;
				}
			}
			else if ( _type != ITEMGET(13,10) && _type != ITEMGET(14,29) && _type != ITEMGET(14,21) && item.m_Level == 3 && _type != ITEMGET(14,100))//Lucky Coin  (S4.5)
			{
				if ( item.m_Durability > item.m_BaseDurability )
				{
					item.m_Durability = item.m_BaseDurability;
				}
			}

			lpObj->pWarehouse[n].m_Option1 = item.m_Option1;
			lpObj->pWarehouse[n].m_Option2 = item.m_Option2;
			lpObj->pWarehouse[n].m_Option3 = item.m_Option3;

			lpObj->pWarehouse[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pWarehouse[n].m_ItemOptionEx = item.m_ItemOptionEx;

			WORD hiword = MAKE_NUMBERW(lpMsg->dbItems[n*dbsize+DBI_SERIAL1], lpMsg->dbItems[n*dbsize+DBI_SERIAL2]);
			WORD loword = MAKE_NUMBERW(lpMsg->dbItems[n*dbsize+DBI_SERIAL3], lpMsg->dbItems[n*dbsize+DBI_SERIAL4]);

			item.m_Number = MAKE_NUMBERDW(hiword, loword);

			gObjWarehouseInsertItemPos(lpObj->m_Index, item, n, -1);
			gObjWarehouseItemSet(lpObj->m_Index, n, 1);
		}
	}

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(PMSG_TALKRESULT);
	pResult.result = 2;

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	GCUserWarehouseSend(&gObj[aIndex]);
#ifdef __MUANGEL__
	UpdateWarehouseStatus(lpObj, 1);
#endif

	if ( bCanWarehouseLock == TRUE )
	{
		if ( gObj[aIndex].WarehousePW > 0 )
		{
			if ( gObj[aIndex].WarehouseLock == 0 )
			{
				GCWarehouseStateSend(aIndex, 12);
			}
			else
			{
				GCWarehouseStateSend(aIndex, 1);
			}
		}
		else
		{
			GCWarehouseStateSend(aIndex, 0);
		}
	}

	lpObj->LoadWareHouseInfo = true;
	lpObj->m_ReqWarehouseOpen = false;

	gObjRequestPetItemInfo(lpObj->m_Index, 1);

	if ( szAuthKey[2] != AUTHKEY2 )
	{
		DestroyGIocp();
	}
}

//00431570 - identical
void GDGetWarehouseNoItem( SDHP_GETWAREHOUSEDB_RESULT * lpMsg)
{
	char szId[11];
	int aIndex = lpMsg->aIndex;
	PMSG_TALKRESULT pResult;

	szId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	if ( !gObjIsAccontConnect(aIndex, szId) )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	if ( lpObj->m_IfState.type == 6 )
	{
		if ( lpObj->m_IfState.state == 0 )
		{
			lpObj->m_IfState.state = 1;
			lpObj->WarehouseSave = 1;
			pResult.h.c = 0xC3;
			pResult.h.headcode = 0x30;
			pResult.h.size = sizeof(pResult);
			pResult.result = 2;

			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			LogAddC(2, lMsg.Get(MSGGET(1, 176)), lpObj->Name);
		
			PMSG_MONEY pMsg; // Season 4.5 fix

			PHeadSetB((LPBYTE)&pMsg, 0x81, 0x0C);
			pMsg.Result = 1;
			pMsg.iMoney = lpObj->Money;
			pMsg.wMoney = lpObj->WarehouseMoney;

			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}

	lpObj->LoadWareHouseInfo = true;
	lpObj->m_ReqWarehouseOpen = false;
#ifdef __MUANGEL__
	UpdateWarehouseStatus(lpObj, 1);
#endif
}

//00431760 - identical
void GDSetWarehouseList(int aIndex)
{
	SDHP_GETWAREHOUSEDB_SAVE pMsg;
	
	if ( gObj[aIndex].LoadWareHouseInfo == false )
	{
		LogAddTD("[%s][%s] WareHouse Save Fail : Not Open",	gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
	pMsg.aIndex = aIndex;
	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0x09;
	pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
	pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
	pMsg.Money = gObj[aIndex].WarehouseMoney;
	pMsg.pw = gObj[aIndex].WarehousePW;
	ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, WAREHOUSE_SIZE);

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
	gObjSavePetItemInfo(aIndex, 1);
	gObjWarehouseTextSave(&gObj[aIndex]);

#ifdef __MUANGEL__
	UpdateWarehouseStatus(&gObj[aIndex], 0);
#endif
}


struct SDHP_DBCHAR_ITEMSAVE
{
	PWMSG_HEAD h;	// C2:11
	char Name[10];	// 4
	BYTE dbInventory[DB_MAX_INVENTORY_SIZE];	// E
};

//00431910  - identical
void GDUserItemSave(LPOBJ lpObj)
{
	SDHP_DBCHAR_ITEMSAVE pMsg;

	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0x11;
	pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
	pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
	memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));
	ItemByteConvert16(pMsg.dbInventory, lpObj->pInventory, INVENTORY_SIZE);

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}

struct SDHP_WAREHOUSEMONEY_SAVE
{
	PBMSG_HEAD h;	// C1:12
	char AccountID[10];	// 3
	short aIndex;	// E
	int Money;	// 10
};

//004319B0 - identical
void GDSetWarehouseMoney(int aIndex)
{
	SDHP_WAREHOUSEMONEY_SAVE pMsg;

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
	pMsg.aIndex = aIndex;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x12;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Money = gObj[aIndex].WarehouseMoney;

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}

//00431A40 - identical
void GDGameServerInfoSave()
{
	return;
}


struct SDHP_ITEMCREATE
{
	PBMSG_HEAD h;	// C1:XX
	BYTE x;	// 3
	BYTE y;	// 4
	BYTE MapNumber;	// 5 [235:Mu_2nd_Aniv], [236:CashShop]
	short Type;	// 6
	BYTE Level;	// 8
	BYTE Dur;	// 9
	BYTE Op1;	// A
	BYTE Op2;	// B
	BYTE Op3;	// C
	BYTE NewOption;	// D
	int aIndex;	// 10
	short lootindex;	// 14
	BYTE SetOption;	// 16
#ifdef PERIOD
	long		lDuration;
	DWORD		dwEventIndex;
#endif
};

//00431A60 - identical
void ItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
#ifdef __ALIEN__
	if( g_DropEx.IsBlockItem(type) )
	{
		if( MapNumber >= 0 && MapNumber <= MAX_NUMBER_MAP )
		{
			LogAddTD("[DropBlock] Item %d has been blocked", type);
			return;
		}
	}
#endif
	SDHP_ITEMCREATE  pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x52;
	pMsg.h.size = sizeof(pMsg);
	pMsg.MapNumber = MapNumber;
	pMsg.x = x;
	pMsg.y = y;
	pMsg.Type = type;
	pMsg.Level = level;

	if (dur == 0 )
	{
		dur = ItemGetDurability(type, level, NewOption, SetOption);
	}

	pMsg.Dur = dur;
	pMsg.Op1 = Op1;
	pMsg.Op2 = Op2;
	pMsg.Op3 = Op3;
	pMsg.NewOption = NewOption;
	pMsg.aIndex = aIndex;
	pMsg.lootindex = LootIndex;
	pMsg.SetOption = SetOption;

#pragma message("##### REMOVE ME ######")
	if( MapNumber == 236 )
	{
		pMsg.lDuration = LootIndex;
	}


	if ( OBJMAX_RANGE(aIndex) != FALSE  && gObj[aIndex].Type != OBJ_USER )
	{
		BYTE NewOption[MAX_EXOPTION_SIZE];

		for ( int i=0;i<MAX_EXOPTION_SIZE;i++)
		{
			NewOption[i] = 0;
		}

		if ( (pMsg.NewOption & 0x20) != 0 )
		{
			NewOption[0] = TRUE;
		}
		if ( (pMsg.NewOption & 0x10) != 0 )
		{
			NewOption[1] = TRUE;
		}

		if ( (pMsg.NewOption & 0x8) != 0 )
		{
			NewOption[2] = TRUE;
		}
		if ( (pMsg.NewOption & 0x4) != 0 )
		{
			NewOption[3] = TRUE;
		}

		if ( (pMsg.NewOption & 0x2) != 0 )
		{
			NewOption[4] = TRUE;
		}
		if ( (pMsg.NewOption & 0x1) != 0 )
		{
			NewOption[5] = TRUE;
		}

		LogAddTD("Monster Item Drop Request [%s] [%d][%d][%d] : [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", gObj[aIndex].Name, MapNumber, pMsg.x, pMsg.y, ItemAttribute[pMsg.Type].Name, pMsg.Level, pMsg.Op1,pMsg.Op2, pMsg.Op3, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],	NewOption[5], NewOption[6], pMsg.SetOption);
	}

	cDBSMng.Send( (char*)&pMsg, pMsg.h.size);
}

//00431D60 - identical
void PetItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	SDHP_ITEMCREATE  pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x55;
	pMsg.h.size = sizeof(pMsg);
	pMsg.MapNumber = MapNumber;
	pMsg.x = x;
	pMsg.y = y;
	pMsg.Type = type;
	pMsg.Level = level;

	if (dur == 0 )
	{
		dur = ItemGetDurability(type, level, NewOption, SetOption);
	}

	pMsg.Dur = dur;
	pMsg.Op1 = Op1;
	pMsg.Op2 = Op2;
	pMsg.Op3 = Op3;
	pMsg.NewOption = NewOption;
	pMsg.aIndex = aIndex;
	pMsg.lootindex = LootIndex;
	pMsg.SetOption = SetOption;

	cDBSMng.Send( (char*)&pMsg, pMsg.h.size);
}

//00431E50 - identical
void ItemSerialCreateRecv(SDHP_ITEMCREATERECV * lpMsg)
{
	LPOBJ lpObj;
	int mapnumber = lpMsg->MapNumber;
	int aIndex = lpMsg->aIndex;
	int lootindex = lpMsg->lootindex;

	if ( mapnumber != (BYTE)-1)
	{
		if ( mapnumber < 0 || (mapnumber > MAX_NUMBER_MAP-1 && mapnumber < 235) )
		{
			return;
		}
	}

	if ( lpMsg->MapNumber == (BYTE)-1 || lpMsg->MapNumber == (BYTE)-2 )
	{
		if ( gObjIsConnectedGP(aIndex) == FALSE )
		{
			LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
			return;
		}
		
		lpObj = &gObj[aIndex];

		if ( lpMsg->MapNumber == (BYTE)-2 )
		{
			if ( lpObj->m_IfState.type != 13 )
			{
				LogAdd("error-L2: DarkTrainerBox not used.");
				return;
			}
		}
		else if ( lpObj->m_IfState.type != 7 )
		{
			LogAdd("error-L2: ChaosBox not used.");
			return;
		}
		
		PMSG_CHAOSMIXRESULT pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x86;
		pMsg.h.size = sizeof(PMSG_CHAOSMIXRESULT);

		if ( lpMsg->MapNumber == (BYTE)-2 )
		{
			pMsg.Result = 100;
		}
		else
		{
			pMsg.Result = 1;
		}

		CItem NewItem;

		NewItem.m_Level = lpMsg->Level;
		NewItem.m_Durability = ItemGetDurability(lpMsg->Type, lpMsg->Level, lpMsg->NewOption, lpMsg->SetOption);

		if ( lpMsg->Type == ITEMGET(14,7) ) // Siege Potion
		{
			NewItem.m_Durability = lpMsg->Dur;
		}

		if ( lpMsg->Type == ITEMGET(13,37) )
		{
			NewItem.m_Durability = lpMsg->Dur;
		}

		NewItem.Convert(lpMsg->Type,lpMsg->Op1, lpMsg->Op2, lpMsg->Op3,lpMsg->NewOption, lpMsg->SetOption,0, NULL, 0xFF, 0, CURRENT_DB_VERSION);
		ItemByteConvert(pMsg.ItemInfo, NewItem);

		NewItem.m_Number = lpMsg->m_Number;
		g_MixSystem.ChaosBoxInit(lpObj);
		gObjChaosBoxInsertItemPos(aIndex, NewItem, 0, -1);
		gObjChaosItemSet(aIndex, 0, 1);
		BYTE ExOption[MAX_EXOPTION_SIZE];
		ItemIsBufExOption(ExOption, &NewItem);

		LogAddTD("[%s][%s] CBMix Item Create Item:%s [%u][%d] [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]",	lpObj->AccountID, lpObj->Name, NewItem.GetName(), lpMsg->m_Number, lpMsg->Dur, NewItem.m_Level, NewItem.m_Option1, NewItem.m_Option2, NewItem.m_Option3, ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpMsg->SetOption);

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		gObj[aIndex].ChaosLock = FALSE;
		return;
	}

	if ( lpMsg->MapNumber == 235 )
	{
		if ( gObj[lpMsg->aIndex].Connected > PLAYER_CONNECTED )
		{
			int iType  = ITEM_GET_TYPE(lpMsg->Type);
			int iTypeIndex = ITEM_GET_INDEX(lpMsg->Type);

			BYTE iItemPos = gObjInventoryInsertItem(&gObj[lpMsg->aIndex], iType, iTypeIndex, lpMsg->Level, lpMsg->m_Number, lpMsg->Dur);

			if ( iItemPos == (BYTE)-1 )
			{
				LogAddTD("[Mu_2Anv_Event] Error : Failed To Gift Item Type:%d, TypeIndex:%d to [%s][%s]", iType, iTypeIndex, gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name);
			}
			else
			{
				::GCInventoryItemOneSend(lpMsg->aIndex, iItemPos);
			}

		}
	}
	else if ( lpMsg->MapNumber == 236 )
	{
		if ( gObj[lpMsg->aIndex].Connected > PLAYER_CONNECTED )
		{
			CItem pCreateItem;
			int iItemType = ITEM_GET_TYPE(lpMsg->Type);
			int iItemIndex	= ITEM_GET_INDEX(lpMsg->Type);
			// ----
			pCreateItem.m_Number = lpMsg->m_Number;
			pCreateItem.m_Level = lpMsg->Level;
			pCreateItem.m_Durability = lpMsg->Dur;
			pCreateItem.Convert(ITEMGET(iItemType, iItemIndex), lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, 0, NULL, 0xFF, 0, 3);
			BYTE btItemPos = gObjInventoryInsertItem(lpMsg->aIndex, pCreateItem);
			// ----
			if ( btItemPos == (BYTE)-1 )
			{
				LogAddTD("[CashShop] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]", iItemType, iItemIndex, gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name);
			}
			else if( pCreateItem.m_Type == ITEMGET(13, 4)
				|| pCreateItem.m_Type == ITEMGET(13, 5) )
			{
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
			}
			else
			{
#ifdef PERIOD
				if( lpMsg->lDuration > 0 )
				{
					g_PeriodItemEx.SetPeriodItemInfo(&gObj[lpMsg->aIndex], lpMsg->Type, lpMsg->m_Number, lpMsg->lDuration);
				}
#endif
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
			}
		}
	}
	else if ( lpMsg->MapNumber == 237 )//Season 4.5 lucky coin trade system addon
	{
		if ( gObj[lpMsg->aIndex].Connected > PLAYER_CONNECTED )
		{
			CItem pCreateItem;
			
			int iItemType = ITEM_GET_TYPE(lpMsg->Type);
			int iItemIndex	= ITEM_GET_INDEX(lpMsg->Type);

			pCreateItem.m_Level = lpMsg->Level;
			pCreateItem.m_Durability = lpMsg->Dur;

			pCreateItem.Convert(ITEMGET(iItemType, iItemIndex), lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, 0, NULL, 0xFF, 0, 3);
			pCreateItem.m_Level = lpMsg->Level;
			
			BYTE btItemPos = gObjInventoryInsertItem(lpMsg->aIndex, pCreateItem);

			if ( btItemPos == (BYTE)-1 )
			{
				LogAddTD("[LuckyCoin Trade] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]", iItemType, iItemIndex, gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name);
				GCSendLuckyCoinResult(lpMsg->aIndex, 2);
			}
			else
			{
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
				GCSendLuckyCoinResult(lpMsg->aIndex, 1);
			}

			gObj[aIndex].ChaosLock = false;
		}
	}
	else if ( lpMsg->MapNumber == 238 )//Season 4.6 Gamble System addon
	{
		if ( gObj[lpMsg->aIndex].Connected > PLAYER_CONNECTED )
		{
			CItem pCreateItem;
			
			int iItemType = ITEM_GET_TYPE(lpMsg->Type);
			int iItemIndex	= ITEM_GET_INDEX(lpMsg->Type);

			pCreateItem.m_Level = lpMsg->Level;
			pCreateItem.m_Durability = lpMsg->Dur;

			pCreateItem.Convert(ITEMGET(iItemType, iItemIndex), lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, 0, NULL, 0xFF, 0, 3);
			pCreateItem.m_Level = lpMsg->Level;
			
			BYTE btItemPos = gObjInventoryInsertItem(lpMsg->aIndex, pCreateItem);

			if ( btItemPos == (BYTE)-1 )
			{
				LogAddTD("[ GAMBLING ] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]", iItemType, iItemIndex, gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name);
			}
			else
			{
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
				
				gObj[aIndex].Money -= g_iGambleSystemMoney;

				if ( gObj[aIndex].Money < 0 )
				{
					gObj[aIndex].Money = 0;
				}
						
				GCMoneySend(aIndex, gObj[aIndex].Money);
			}

			gObj[aIndex].m_bUseGambleState = false;
			
		}
	}
	else if ( lpMsg->MapNumber == 239 )
	{
		if ( gObj[lpMsg->aIndex].Connected > PLAYER_CONNECTED )
		{
			int iType  = ITEM_GET_TYPE(lpMsg->Type);
			int iTypeIndex = ITEM_GET_INDEX(lpMsg->Type);

			BYTE iItemPos = gObjInventoryInsertItem(&gObj[lpMsg->aIndex], iType, iTypeIndex, lpMsg->Level, lpMsg->m_Number, lpMsg->Dur);

			if ( iItemPos == (BYTE)-1 )
			{
				LogAddTD("[GOLDENARCHER] Error : Failed To Gift Item Type:%d, TypeIndex:%d to [%s][%s]", iType, iTypeIndex, gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name);
			}
			else
			{
				::GCInventoryItemOneSend(lpMsg->aIndex, iItemPos);
			}

		}
	}
	else
	{
		int iRetMapNumber = lpMsg->MapNumber;

		if ( lpMsg->MapNumber >= 238 && lpMsg->MapNumber <= 245 )
		{
			mapnumber = g_BloodCastle.GetMapNumByBC_CHAOSGEM(iRetMapNumber);
		}

		if ( lpMsg->MapNumber >= 246 && lpMsg->MapNumber <= 253 )
		{
			mapnumber = g_BloodCastle.GetMapNumByBC_ULTIMATEWEAPON(iRetMapNumber);
		}
		
		BYTE SocketSlotCount = 0;
		BYTE SocketOption[5];
		BYTE SocketOptionIndex = 0;

		if ( gItemSocketOption.IsEnableSocketItem(lpMsg->Type) == 1 )
        {
			SocketSlotCount = gItemSocketOption.GetMakeSocketSlotCount(lpMsg->Type);

			LogAddTD("[SocketItem] Drop Socket Item ( Socket Slot : %d ) - %s, [%d,%d,%d,%d], Serial : %u, ItemCode : %d, Level : %d, MapNumber : %d(%d/%d)",SocketSlotCount,ItemAttribute[lpMsg->Type].Name,lpMsg->Level,lpMsg->Op1,lpMsg->Op2,lpMsg->Op3, lpMsg->m_Number,lpMsg->Type,lpMsg->Level,lpMsg->MapNumber,lpMsg->x,lpMsg->y);
			
			SocketOptionIndex = 0xFF;

			for(int i = 0;i < 5; i++)
			{
				if(i < SocketSlotCount)
				{
					SocketOption[i] = 0xFE;
				}
				else
				{
					SocketOption[i] = 0xFF;
				}
			}

		}
		else
		{
			SocketOptionIndex = 0;

			for(int i = 0; i < 5; i++)
			{
				SocketOption[i] = 0xFF;
			}
		}

		int iItemCount = MapC[mapnumber].MonsterItemDrop(lpMsg->Type, lpMsg->Level, lpMsg->Dur, lpMsg->x, lpMsg->y, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, lootindex, lpMsg->m_Number, 0,SocketOption,SocketOptionIndex);
		
		if ( iItemCount != -1 )
		{
			if ( iRetMapNumber >= 238 && iRetMapNumber <= 245)
			{
				MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + 300000;
				MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + 20000;

			}

			if ( iRetMapNumber >= 246 && iRetMapNumber <= 253)
			{
				MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + 900000;
				MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + 10000;

				int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(mapnumber);

				g_BloodCastle.m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL = lpMsg->m_Number;
			}

			if( ((mapnumber) < MAP_INDEX_ILLUSION_TEMPLE1)?FALSE:((mapnumber) > MAP_INDEX_ILLUSION_TEMPLE6 )?FALSE:TRUE) //Season 2.5 add-on
			{
				if( lpMsg->Type == ITEMGET(14,64) )
				{
					MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + 15000;
					MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + 5000;
				}

				if( lpMsg->Type == ITEMGET(12,15) )
				{
					MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + 300000;
					MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + 10000;
				}
			}
		}

		BYTE NewOption[MAX_EXOPTION_SIZE];

		for ( int i=0;i<MAX_EXOPTION_SIZE;i++)
		{
			NewOption[i] = FALSE;
		}

		if ( (lpMsg->NewOption & 0x20) != 0 )
		{
			NewOption[0] = TRUE;
		}
		if ( (lpMsg->NewOption & 0x10) != 0 )
		{
			NewOption[1] = TRUE;
		}

		if ( (lpMsg->NewOption & 0x8) != 0 )
		{
			NewOption[2] = TRUE;
		}
		if ( (lpMsg->NewOption & 0x4) != 0 )
		{
			NewOption[3] = TRUE;
		}

		if ( (lpMsg->NewOption & 0x2) != 0 )
		{
			NewOption[4] = TRUE;
		}
		if ( (lpMsg->NewOption & 0x1) != 0 )
		{
			NewOption[5] = TRUE;
		}

		LogAddTD("Monster Item Drop [%d][%d][%d] : serial:%u [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", mapnumber, lpMsg->x, lpMsg->y, lpMsg->m_Number, ItemAttribute[lpMsg->Type].Name, lpMsg->Level, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, NewOption[0],NewOption[1],NewOption[2],NewOption[3],NewOption[4],NewOption[5],NewOption[6], lpMsg->SetOption);	
	}
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Item Move Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x53
 */
struct SDHP_ITEMMOVESAVE
{
	PBMSG_HEAD h;	// C1:53
	DWORD Serial;	// 4
	char ServerName[20];	// 8
	char Account[10];	// 1C
	char Name[10];	// 26
	char ItemName[21];	// 30
	BYTE X;	// 45
	BYTE Y;	// 46
	BYTE ItemLevel;	// 47
	BYTE ItemOp1;	// 48
	BYTE ItemOp2;	// 49
	BYTE ItemOp3;	// 4A
};

//00433000 - identical
void ItemMovePathSave(char* ActID, char* Name, BYTE level, BYTE mapnumber, BYTE x, BYTE y, char* Item, BYTE op1, BYTE op2, BYTE op3, DWORD serial)
{
	SDHP_ITEMMOVESAVE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x53;
	pMsg.h.size = sizeof(pMsg);
	pMsg.ItemLevel = level;
	pMsg.Serial = serial;
	pMsg.ItemOp1 = op1;
	pMsg.ItemOp2 = op2;
	pMsg.ItemOp3 = op3;
	pMsg.X = x;
	pMsg.Y = y;
	memcpy(pMsg.Account, ActID, sizeof(pMsg.Account));
	strcpy(pMsg.ItemName, Item);
	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));
	strcpy(pMsg.ServerName, szServerName);

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Skill Key Data Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x60
 */
struct SDHP_SKILLKEYDATA
{
	PBMSG_HEAD h;	// C1:60
	char Name[10];	// 3
	BYTE SkillKeyBuffer[20];	// D // Season 4.5 Remake
	BYTE GameOption;	// 17
	BYTE QkeyDefine;	// 18
	BYTE WkeyDefine;	// 19
	BYTE EkeyDefine;	// 1A
	BYTE ChatWindow;	// 1B
	BYTE RkeyDefine;	// 1C
	int QWERLevel;	// 1D
};

//004330D0 - identical
void DGOptionDataSend(int aIndex, char* szName, BYTE * KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWERLevel)
{
	SDHP_SKILLKEYDATA pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x60;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.Name, szName, sizeof(pMsg.Name));
	memcpy(pMsg.SkillKeyBuffer, KeyBuffer, sizeof(pMsg.SkillKeyBuffer));
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;
	pMsg.ChatWindow = ChatWnd;
	pMsg.RkeyDefine = Rk;
	pMsg.QWERLevel = QWERLevel;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//00433180 - identical
void DGOptionDataRecv(SDHP_SKILLKEYDATA_SEND * lpMsg)
{
	int aIndex = lpMsg->aIndex;
	char szName[MAX_ACCOUNT_LEN+1];

	if ( aIndex < 0 || aIndex >= OBJMAX )
	{
		return;
	}

	memset(szName, 0, sizeof(szName));
	memcpy(szName, lpMsg->Name, sizeof(lpMsg->Name));

	if ( strcmp(gObj[aIndex].Name, szName) != 0 )
	{
		return;
	}

	::GCSkillKeySend(aIndex, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, lpMsg->RkeyDefine, lpMsg->QWERLevel);
}

//00433270 - identical
void DGMoveOtherServer(SDHP_CHARACTER_TRANSFER_RESULT * lpMsg)
{
	if ( !gObjIsConnectedGP(lpMsg->Number))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->Number];

	if ( lpMsg->Result != 0 )
	{
		PMSG_DEFRESULT pResult;

		PHeadSetB((LPBYTE)&pResult, 0x99, sizeof(pResult));
		pResult.result = 1;

		DataSend(lpMsg->Number, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[CharTrasfer] Fail [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpMsg->Result);
		
		lpObj->m_MoveOtherServer = 0;
		
		GCServerMsgStringSend("?®?¦ ??»??? change@webzen.co.kr·? ?®???? ????±???¶?????",lpObj->m_Index, 1);
		return;
	}

	LogAddTD("[CharTrasfer] Success [%s][%s] (%d)",	lpObj->AccountID, lpObj->Name, lpMsg->Result);

	GCServerMsgStringSend("?????? ??·?µ?????.", lpObj->m_Index, 1);
	GCServerMsgStringSend("???? ?­??·? ??????????±? ??¶?????.", lpObj->m_Index, 1);

	GJSetCharacterInfo(lpObj, lpObj->m_Index, 0, 0);

	lpObj->LoadWareHouseInfo = false;
	gObjCloseSet(lpObj->m_Index, 2);
	lpObj->m_MoveOtherServer = 0;
}

struct Request_PetItem_Info
{
	BYTE nPos;	// 0
	int nSerial;	// 4
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Request Pet Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x56
 *	BASE PACKET - Complete with Request_PetItem_Info
 */
struct SDHP_REQUEST_PETITEM_INFO
{
	PWMSG_HEAD h;
	char AccountID[11];	// 4
	WORD Number;	// 10
	BYTE InvenType;	// 12
	BYTE nCount;	// 13
};

//00433440 - identical
void gObjRequestPetItemInfo(int aIndex, int inventype) //Identical gs-cs 56
{
	LPOBJ lpObj = &gObj[aIndex];
	char pbuffer[WAREHOUSE_BUFF];
	int lofs = sizeof(SDHP_REQUEST_PETITEM_INFO);
	int founditemcount = 0;
	Request_PetItem_Info petIteminfo;

	if ( inventype == 0 )
	{
		for ( int n=0;n<INVENTORY_SIZE;n++)
		{
			if ( lpObj->pInventory[n].IsItem() )
			{
				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,4) || lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
				{
					founditemcount++;
					petIteminfo.nPos = n;
					petIteminfo.nSerial = lpObj->pInventory[n].m_Number;
					memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
					lofs +=sizeof(petIteminfo);
				}
			}
		}
	}
	else if ( inventype == 1 )
	{
		for ( int n=0;n<WAREHOUSE_SIZE;n++)
		{
			if ( lpObj->pWarehouse[n].IsItem() )
			{
				if ( lpObj->pWarehouse[n].m_Type == ITEMGET(13,4) || lpObj->pWarehouse[n].m_Type == ITEMGET(13,5) )
				{
					founditemcount++;
					petIteminfo.nPos = n;
					petIteminfo.nSerial = lpObj->pWarehouse[n].m_Number;
					memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
					lofs +=sizeof(petIteminfo);
				}
			}
		}
	}
	
	if ( founditemcount )
	{
		SDHP_REQUEST_PETITEM_INFO pMsg;

		pMsg.h.set((LPBYTE)&pMsg, 0x56, lofs + sizeof(pMsg));
		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
		pMsg.Number = aIndex;
		pMsg.InvenType = inventype;
		pMsg.nCount = founditemcount;
		memcpy(pbuffer, &pMsg, sizeof(pMsg));

		cDBSMng.Send((char*)pbuffer, lofs+sizeof(pMsg));
	}
}

struct Recv_PetItem_Info
{
	BYTE nPos;	// 0
	int nSerial;	// 4
	BYTE Level;	// 8
	int Exp;	// C
};


struct SDHP_RECV_PETITEM_INFO
{
	PWMSG_HEAD h;
	char AccountID[11];	// 4
	WORD Number;	// 10
	BYTE InvenType;	// 12
	BYTE nCount;	// 13
};

//00433830 - identical
void DGRecvPetItemInfo(LPBYTE lpData)
{
	SDHP_RECV_PETITEM_INFO* lpMsg = (SDHP_RECV_PETITEM_INFO *)lpData;
	Recv_PetItem_Info* pPetItemInfo = (Recv_PetItem_Info *)((DWORD)lpData + sizeof(SDHP_RECV_PETITEM_INFO));
	int aIndex = lpMsg->Number;
	char szAccountId[MAX_ACCOUNT_LEN+1];
	szAccountId[MAX_ACCOUNT_LEN]=0;

	memcpy(szAccountId, lpMsg->AccountID, MAX_ACCOUNT_LEN);
	
	if ( !gObjIsAccontConnect(aIndex, szAccountId))
	{
		LogAddC(2, "Request to receive petitem infomation doesn't match the user %s", szAccountId);
		CloseClient(aIndex);
		
		return;
	}

	if ( lpMsg->InvenType == 0 )
	{
		BOOL ReCalc = 0;

		for ( int n=0;n<lpMsg->nCount;n++)
		{
			if ( pPetItemInfo[n].nPos < INVETORY_WEAR_SIZE )
			{
				ReCalc = 1;
			}

			if ( gObj[aIndex].pInventory[pPetItemInfo[n].nPos].IsItem() )
			{
				if ( gObj[aIndex].pInventory[pPetItemInfo[n].nPos].m_Number == pPetItemInfo[n].nSerial )
				{
					gObj[aIndex].pInventory[pPetItemInfo[n].nPos].SetPetItemInfo(pPetItemInfo[n].Level, pPetItemInfo[n].Exp);
				}
			}
		}

		if ( ReCalc != FALSE )
		{
			gObjCalCharacter(aIndex);
		}
	}
	else if ( lpMsg->InvenType == 1 )
	{
		for ( int n=0;n<lpMsg->nCount;n++)
		{

			if ( gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].IsItem() )
			{
				if ( gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].m_Number == pPetItemInfo[n].nSerial )
				{
					gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].SetPetItemInfo(pPetItemInfo[n].Level, pPetItemInfo[n].Exp);
				}
			}
		}

	}
}



struct Save_PetItem_Info
{
	int nSerial;	// 0
	BYTE Level;	// 3
	int Exp;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu PetInfo Save Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x55
 *	BASE PACKET - Complete with Save_PetItem_Info
 */
struct SDHP_SAVE_PETITEM_INFO
{
	PWMSG_HEAD h;
	BYTE nCount;	// 4
};

//00433B90 - identical
void gObjSavePetItemInfo(int aIndex, int inventype)
{
	LPOBJ lpObj = &gObj[aIndex];
	char pbuffer[WAREHOUSE_BUFF];
	int lofs = sizeof(SDHP_SAVE_PETITEM_INFO);
	int founditemcount = 0;
	Save_PetItem_Info petIteminfo;

	if ( inventype == 0 )
	{
		for ( int n=0;n<INVENTORY_SIZE;n++)
		{
			if ( lpObj->pInventory[n].IsItem() )
			{
				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,4) || lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
				{
					if ( lpObj->pInventory[n].m_IsLoadPetItemInfo != FALSE )
					{
						founditemcount++;
						petIteminfo.nSerial = lpObj->pInventory[n].m_Number;
						petIteminfo.Level = lpObj->pInventory[n].m_PetItem_Level;
						petIteminfo.Exp = lpObj->pInventory[n].m_PetItem_Exp;

						LogAddTD("[%s][%s][PI][%d,%s]serial:[%u] Level:%d Exp:%d", lpObj->AccountID, lpObj->Name, n, lpObj->pInventory[n].GetName(),petIteminfo.nSerial, petIteminfo.Level, petIteminfo.Exp);

						memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
						lofs += sizeof(petIteminfo);
					}
				}
			}
		}
	}
	else if ( inventype == 1 )
	{
		for ( int n=0;n<WAREHOUSE_SIZE;n++)
		{
			if ( lpObj->pWarehouse[n].IsItem() )
			{
				if ( lpObj->pWarehouse[n].m_Type == ITEMGET(13,4) || lpObj->pWarehouse[n].m_Type == ITEMGET(13,5) )
				{
					if ( lpObj->pWarehouse[n].m_IsLoadPetItemInfo != FALSE )
					{
						founditemcount++;
						petIteminfo.nSerial = lpObj->pWarehouse[n].m_Number;
						petIteminfo.Level = lpObj->pWarehouse[n].m_PetItem_Level;
						petIteminfo.Exp = lpObj->pWarehouse[n].m_PetItem_Exp;

						LogAddTD("[%s][%s][PW][%d,%s]serial:[%u] Level:%d Exp:%d", lpObj->AccountID, lpObj->Name, n, lpObj->pWarehouse[n].GetName(),petIteminfo.nSerial, petIteminfo.Level, petIteminfo.Exp);

						memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
						lofs += sizeof(petIteminfo);
					}
				}
			}
		}
	}

	if ( founditemcount )
	{
		SDHP_SAVE_PETITEM_INFO pMsg;

		pMsg.h.set((LPBYTE)&pMsg, 0x57, lofs+sizeof(pMsg));
		pMsg.nCount = founditemcount;
		memcpy(pbuffer, &pMsg, sizeof(pMsg));

		cDBSMng.Send(pbuffer, lofs+sizeof(pMsg));
	}
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLEDATA
{
	PBMSG_HEAD2 h;	// C1:80:00
	WORD wMapSvrNum;	// 4
	int iCastleEventCycle;	// 8
};

//004340C0 - identical
void GS_GDReqCastleTotalInfo(int iMapSvrGroup, int iCastleEventCycle)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLEDATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x00, sizeof(CSP_REQ_CASTLEDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleEventCycle = iCastleEventCycle;

	::cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_OWNERGUILDMASTER
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};

//00434180 - identical
void GS_GDReqOwnerGuildMaster(int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_OWNERGUILDMASTER pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x01, sizeof(CSP_REQ_OWNERGUILDMASTER));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCBUY
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iNpcDfLevel;	// 14
	int iNpcRgLevel;	// 18
	int iNpcMaxHp;	// 1C
	int iNpcHp;	// 20
	BYTE btNpcX;	// 24
	BYTE btNpcY;	// 25
	BYTE btNpcDIR;	// 26
	int iBuyCost;	// 28
};

//00434220 - identical
void GS_GDReqCastleNpcBuy(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcDfLevel, int iNpcRgLevel, int iNpcMaxHP, int iNpcHP, BYTE btNpcX, BYTE btNpcY, BYTE btNpcDIR, int iBuyCost)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCBUY pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x03, sizeof(CSP_REQ_NPCBUY));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iNpcDfLevel = iNpcDfLevel;
	pMsg.iNpcRgLevel = iNpcRgLevel;
	pMsg.iNpcMaxHp = iNpcMaxHP;
	pMsg.iNpcHp = iNpcHP;
	pMsg.btNpcX = btNpcX;
	pMsg.btNpcY = btNpcY;
	pMsg.btNpcDIR = btNpcDIR;
	pMsg.iBuyCost = iBuyCost;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCREPAIR
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iRepairCost;	// 14
};

//00434310 - identical
void GS_GDReqCastleNpcRepair(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iRepairCost)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCREPAIR pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x04, sizeof(CSP_REQ_NPCREPAIR));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iRepairCost = iRepairCost;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCUPGRADE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iNpcUpType;	// 14
	int iNpcUpValue;	// 18
	int iNpcUpIndex;	// 1C
};

void GS_GDReqCastleNpcUpgrade(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCUPGRADE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x05, sizeof(CSP_REQ_NPCUPGRADE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iNpcUpType = iNpcUpType;
	pMsg.iNpcUpValue = iNpcUpValue;
	pMsg.iNpcUpIndex = iNpcUpIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_TAXINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 3
	int iIndex;	// 8
};

void GS_GDReqTaxInfo(int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_TAXINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x06, sizeof(CSP_REQ_TAXINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_TAXRATECHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iTaxKind;	// C
	int iTaxRate;	// 10
};

void GS_GDReqTaxRateChange( int iMapSvrGroup, int aIndex, int iTaxType, int iTaxRate)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_TAXRATECHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x07, sizeof(CSP_REQ_TAXRATECHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iTaxRate = iTaxRate; 
	pMsg.iTaxKind = iTaxType;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_MONEYCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iMoneyChanged;	// C
};

void GS_GDReqCastleMoneyChange(int iMapSvrGroup, int aIndex,int iMoneyChange)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_MONEYCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x08, sizeof(CSP_REQ_MONEYCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iMoneyChanged = iMoneyChange;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_SDEDCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	WORD wStartYear;	// C
	BYTE btStartMonth;	// E
	BYTE btStartDay;	// F
	WORD wEndYear;	// 10
	BYTE btEndMonth;	// 12
	BYTE btEndDay;	// 13
};

void GS_GDReqSiegeDateChange(int iMapSvrGroup, int aIndex,WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_SDEDCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x09, sizeof(CSP_REQ_SDEDCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.wStartYear = wStartYear;
	pMsg.btStartMonth = btStartMonth;
	pMsg.btStartDay = btStartDay;
	pMsg.wEndYear = wEndYear;
	pMsg.btEndMonth = btEndMonth;
	pMsg.btEndDay = btEndDay;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDREGINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
};

void GS_GDReqGuildMarkRegInfo(int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_GUILDREGINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0A, sizeof(CSP_REQ_GUILDREGINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_SIEGEENDCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	BOOL bIsSiegeEnded;	// 8
};

void GS_GDReqSiegeEndedChange(int iMapSvrGroup, BOOL bIsSiegeEnded)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}


	CSP_REQ_SIEGEENDCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0B, sizeof(CSP_REQ_SIEGEENDCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.bIsSiegeEnded = bIsSiegeEnded;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLEOWNERCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	BOOL bIsCastleOccupied;	// 8
	char szOwnerGuildName[8];	// C
};

void GS_GDReqCastleOwnerChange( int iMapSvrGroup, BOOL bIsCastleOccupied, char* lpszGuildName)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( lpszGuildName == NULL )
	{
		return;
	}


	CSP_REQ_CASTLEOWNERCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0C, sizeof(CSP_REQ_CASTLEOWNERCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.bIsCastleOccupied = bIsCastleOccupied;
	memcpy(pMsg.szOwnerGuildName, lpszGuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_REGATTACKGUILD
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szEnemyGuildName[8];	// C
};

void GS_GDReqRegAttackGuild( int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_REGATTACKGUILD pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0D, sizeof(CSP_REQ_REGATTACKGUILD));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szEnemyGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLESIEGEEND
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};

void GS_GDReqRestartCastleState( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLESIEGEEND pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0E, sizeof(CSP_REQ_CASTLESIEGEEND));
	pMsg.wMapSvrNum = iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_MAPSVRMULTICAST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	char szMsgText[128];	// 6
};

void GS_GDReqMapSvrMsgMultiCast( int iMapSvrGroup,char * lpszMsgText)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( lpszMsgText == NULL )
	{
		return;
	}


	CSP_REQ_MAPSVRMULTICAST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0F, sizeof(CSP_REQ_MAPSVRMULTICAST));
	pMsg.wMapSvrNum = iMapSvrGroup;
	strcpy(pMsg.szMsgText, lpszMsgText);
	pMsg.szMsgText[127] = 0;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDREGMARK
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
	int iItemPos;	// 14
};

void GS_GDReqRegGuildMark( int iMapSvrGroup, int aIndex, int iItemPos)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	CSP_REQ_GUILDREGMARK pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0x80,0x10, sizeof(CSP_REQ_GUILDREGMARK));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iItemPos = iItemPos;
	memcpy(&pMsg.szGuildName,gObj[aIndex].GuildName,sizeof(pMsg.szGuildName));
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDRESETMARK
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
};

void GS_GDReqGuildMarkReset( int iMapSvrGroup, int aIndex, char* lpszGuildName)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	if ( lpszGuildName == NULL )
	{
		return;
	}

	CSP_REQ_GUILDRESETMARK pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x11, sizeof(CSP_REQ_GUILDRESETMARK));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szGuildName, lpszGuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDSETGIVEUP
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
	BOOL bIsGiveUp;	// 14
};

void GS_GDReqGuildSetGiveUp( int iMapSvrGroup, int aIndex, BOOL bIsGiveUp)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	CSP_REQ_GUILDSETGIVEUP pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x12, sizeof(CSP_REQ_GUILDSETGIVEUP));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.bIsGiveUp = bIsGiveUp;
	memcpy(pMsg.szGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCREMOVE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iNpcNumber;	// 8
	int iNpcIndex;	// C
};

void GS_GDReqNpcRemove(int iMapSvrGroup, int iNpcNumber, int iNpcIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_NPCREMOVE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x16, sizeof(CSP_REQ_NPCREMOVE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLESTATESYNC
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleState;	// 8
	int iTaxRateChaos;	// C
	int iTaxRateStore;	// 10
	int iTaxHuntZone;	// 14
	char szOwnerGuildName[8];	// 18
};

void GS_GDReqCastleStateSync( int iMapSvrGroup, int iCastleState, int iTaxRateChaos, int iTaxRateStore, int iTaxHuntZone, char* lpszOwnerGuild)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLESTATESYNC pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x17, sizeof(CSP_REQ_CASTLESTATESYNC));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleState = iCastleState;
	pMsg.iTaxRateChaos = iTaxRateChaos;
	pMsg.iTaxRateStore = iTaxRateStore;
	pMsg.iTaxHuntZone = iTaxHuntZone;
	memcpy(pMsg.szOwnerGuildName, lpszOwnerGuild, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLETRIBUTEMONEY
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleTributeMoney;	// 8
};

void GS_GDReqCastleTributeMoney(int iMapSvrGroup, int iCastleTributeMoney)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( iCastleTributeMoney < 0 )
	{
		return;
	}

	CSP_REQ_CASTLETRIBUTEMONEY pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x18, sizeof(CSP_REQ_CASTLETRIBUTEMONEY));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleTributeMoney = iCastleTributeMoney;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETCASTLETAXINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};

void GS_GDReqResetCastleTaxInfo( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETCASTLETAXINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x19, sizeof(CSP_REQ_RESETCASTLETAXINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETSIEGEGUILDINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};

void GS_GDReqResetSiegeGuildInfo( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETSIEGEGUILDINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x1A, sizeof(CSP_REQ_RESETSIEGEGUILDINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETREGSIEGEINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 2
};

void GS_GDReqResetRegSiegeInfo( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETREGSIEGEINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x1B, sizeof(CSP_REQ_RESETREGSIEGEINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CSINITDATA
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCastleEventCycle;	// 8
};

void GS_GDReqCastleInitData( int iMapSvrGroup, int iCastleEventCycle)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CSINITDATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x81, sizeof(CSP_REQ_CSINITDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleEventCycle = iCastleEventCycle;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_NPCDATA
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};

void GS_GDReqCastleNpcInfo( int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_NPCDATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x82, sizeof(CSP_REQ_NPCDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_ALLGUILDREGINFO
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};

void GS_GDReqAllGuildMarkRegInfo( int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_ALLGUILDREGINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x83, sizeof(CSP_REQ_ALLGUILDREGINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CALCREGGUILDLIST
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};

void GS_GDReqCalcRegGuildList(int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CALCREGGUILDLIST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x85, sizeof(CSP_REQ_CALCREGGUILDLIST));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CSLOADTOTALGUILDINFO
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};

void GS_GDReqCsLoadTotalGuildInfo(int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CSLOADTOTALGUILDINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x88, sizeof(CSP_REQ_CSLOADTOTALGUILDINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//#if(_GSCS==1)
struct CSP_ANS_CASTLEDATA
{

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xa>*/ /*|0x2|*/ WORD wStartYear;
  /*<thisrel this+0xc>*/ /*|0x1|*/ BYTE btStartMonth;
  /*<thisrel this+0xd>*/ /*|0x1|*/ BYTE btStartDay;
  /*<thisrel this+0xe>*/ /*|0x2|*/ WORD wEndYear;
  /*<thisrel this+0x10>*/ /*|0x1|*/ BYTE btEndMonth;
  /*<thisrel this+0x11>*/ /*|0x1|*/ BYTE btEndDay;
  /*<thisrel this+0x12>*/ /*|0x1|*/ BYTE btIsSiegeGuildList;
  /*<thisrel this+0x13>*/ /*|0x1|*/ BYTE btIsSiegeEnded;
  /*<thisrel this+0x14>*/ /*|0x1|*/ BYTE btIsCastleOccupied;
  /*<thisrel this+0x15>*/ /*|0x8|*/ char szCastleOwnGuild[8];
  /*<thisrel this+0x20>*/ /*|0x8|*/ __int64 i64CastleMoney;
  /*<thisrel this+0x28>*/ /*|0x4|*/ int iTaxRateChaos;
  /*<thisrel this+0x2c>*/ /*|0x4|*/ int iTaxRateStore;
  /*<thisrel this+0x30>*/ /*|0x4|*/ int iTaxHuntZone;
  /*<thisrel this+0x34>*/ /*|0x4|*/ int iFirstCreate;
};
//#endif

void GS_DGAnsCastleTotalInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_CASTLEDATA* lpMsg = (CSP_ANS_CASTLEDATA*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x00] GS_DGAnsCastleTotalInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_OWNERGUILDMASTER {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szCastleOwnGuild[8];
  /*<thisrel this+0x18>*/ /*|0xa|*/ char szCastleOwnGuildMaster[10];

};
//#endif

void GS_DGAnsOwnerGuildMaster(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_OWNERGUILDMASTER* lpMsg = (CSP_ANS_OWNERGUILDMASTER*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x00] GS_DGAnsCastleTotalInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	GCAnsCastleSiegeState(lpMsg->iIndex,lpMsg->iResult,lpMsg->szCastleOwnGuild,lpMsg->szCastleOwnGuildMaster);
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_NPCBUY {

  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iBuyCost;
};
//#endif

void GS_DGAnsCastleNpcBuy(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_NPCBUY* lpMsg = (CSP_ANS_NPCBUY*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x03] GS_DGAnsCastleNpcBuy() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if (lpMsg->iResult == 1)
	{
		BOOL bAddResult = g_CastleSiege.AddDbNPC(lpMsg->iNpcNumber,lpMsg->iNpcIndex);
	
		if ( bAddResult == TRUE)
		{
			if(gObjIsConnected(lpMsg->iIndex))
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iBuyCost;

				if(gObj[lpMsg->iIndex].Money < 0 )
				{
					gObj[lpMsg->iIndex].Money = 0;
				}

				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);
			}

			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() - CCastleSiege::AddDbNPC() OK - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
		else
		{
			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() - CCastleSiege::AddDbNPC() FAILED - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
	}
	else
	{
		LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() FAILED - Result:(%d), Npc:(CLS:%d, IDX:%d)",lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex); 
	}

	GCAnsNpcBuy(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_NPCREPAIR {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iNpcMaxHp;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iNpcHp;
  /*<thisrel this+0x20>*/ /*|0x4|*/ int iRepairCost;
};
//#endif

void GS_DGAnsCastleNpcRepair(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_NPCREPAIR* lpMsg = (CSP_ANS_NPCREPAIR*) lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x04] GS_DGAnsCastleNpcRepair() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == 1)
	{
		BOOL bRepairResult =  g_CastleSiege.RepairDbNPC(lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcHp,lpMsg->iNpcMaxHp);
		if( bRepairResult == TRUE)
		{
			if(gObjIsConnected(lpMsg->iIndex))
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iRepairCost;

				if(gObj[lpMsg->iIndex].Money < 0 )
				{
					gObj[lpMsg->iIndex].Money = 0;
				}

				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);
			}

			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() - CCastleSiege::RepairDbNPC() OK - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
		else
		{
			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() - CCastleSiege::RepairDbNPC() FAILED - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
	}
	else
	{
		LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() FAILED - Result:(%d), Npc:(CLS:%d, IDX:%d)",lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
	}

	GCAnsNpcRepair(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcHp,lpMsg->iNpcMaxHp);
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_NPCUPGRADE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iNpcUpType;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iNpcUpValue;
  /*<thisrel this+0x20>*/ /*|0x4|*/ int iNpcUpIndex;
};
//#endif

void GS_DGAnsCastleNpcUpgrade(LPBYTE lpRecv)
{
//#if (_GSCS==1)
	CSP_ANS_NPCUPGRADE* lpMsg = (CSP_ANS_NPCUPGRADE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x05] GS_DGAnsCastleNpcUpgrade() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == 0)
	{
		LogAddC(2,"[CastleSiege] ERROR - Castle NPC Upgrade Fail() (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
	}
	else
	{
		g_CastleSiege.UpgradeDbNPC(lpMsg->iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,lpMsg->iNpcUpIndex);
		LogAddTD("[CastleSiege] [0x80][0x06] GS_DGAnsTaxInfo() - Npc Upgrade OK (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",lpMsg->iNpcNumber, lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
	}

	GCAnsNpcUpgrade(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);

//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_TAXINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ __int64 i64CastleMoney;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iTaxRateChaos;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iTaxRateStore;
  /*<thisrel this+0x20>*/ /*|0x4|*/ int iTaxHuntZone;
};
//#endif

void GS_DGAnsTaxInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_TAXINFO* lpMsg = (CSP_ANS_TAXINFO*)lpRecv;

	if(lpMsg == NULL)
		return;
	
	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x06] GS_DGAnsTaxInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		GCAnsTaxMoneyInfo(lpMsg->iIndex,lpMsg->iResult,lpMsg->iTaxRateChaos,lpMsg->iTaxRateStore,lpMsg->i64CastleMoney);
		g_CastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_TAXRATECHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iTaxKind;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iTaxRate;
};
//#endif

void GS_DGAnsTaxRateChange(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_TAXRATECHANGE* lpMsg = (CSP_ANS_TAXRATECHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x07] GS_DGAnsTaxRateChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == 1)
	{
		g_CastleSiege.SetTaxRate(lpMsg->iTaxKind,lpMsg->iTaxRate);
		GCAnsTaxRateChange(lpMsg->iIndex,lpMsg->iResult,lpMsg->iTaxKind,lpMsg->iTaxRate);
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_MONEYCHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iMoneyChanged;
  /*<thisrel this+0x18>*/ /*|0x8|*/ __int64 i64CastleMoney;
};
//#endif

void GS_DGAnsCastleMoneyChange(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_MONEYCHANGE* lpMsg = (CSP_ANS_MONEYCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x08] GS_DGAnsCastleMoneyChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		if(gObjIsConnected(lpMsg->iIndex))
		{
			if(lpMsg->iMoneyChanged < 0)
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iMoneyChanged;

				if(gObj[lpMsg->iIndex].Money > MAX_ZEN)
				{
					gObj[lpMsg->iIndex].Money = MAX_ZEN;
				}
				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);

				LogAddTD("[CastleSiege] [0x80][0x08] GS_DGAnsCastleMoneyChange() - Withdraw Request OK [%s][%s] (ReqMoney:%d, TotMoney:%I64d)",gObj[lpMsg->iIndex].AccountID,
				gObj[lpMsg->iIndex].Name,lpMsg->iMoneyChanged,lpMsg->i64CastleMoney);
			}
		}
		g_CastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
	}
	
	GCAnsMoneyDrawOut(lpMsg->iIndex,lpMsg->iResult,lpMsg->i64CastleMoney);
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_SDEDCHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x2|*/ WORD wStartYear;
  /*<thisrel this+0x12>*/ /*|0x1|*/ BYTE btStartMonth;
  /*<thisrel this+0x13>*/ /*|0x1|*/ BYTE btStartDay;
  /*<thisrel this+0x14>*/ /*|0x2|*/ WORD wEndYear;
  /*<thisrel this+0x16>*/ /*|0x1|*/ BYTE btEndMonth;
  /*<thisrel this+0x17>*/ /*|0x1|*/ BYTE btEndDay;
};
//#endif

void GS_DGAnsSiegeDateChange(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_SDEDCHANGE* lpMsg = (CSP_ANS_SDEDCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x09] GS_DGAnsSiegeDateChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
//#else
//	return;
//#endif
}

void GS_DGAnsGuildMarkRegInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_GUILDREGINFO* lpMsg = (CSP_ANS_GUILDREGINFO*)lpRecv;

	if(lpMsg == NULL)
		return;
	
	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0A] GS_DGAnsGuildMarkRegInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	GCAnsGuildRegInfo(lpMsg->iIndex,lpMsg->iResult,lpMsg);
//#else
//	return;
//#endif

}

//#if(_GSCS==1)
struct CSP_ANS_SIEGEENDCHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int bIsSiegeEnded;
};
//#endif

void GS_DGAnsSiegeEndedChange(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_SIEGEENDCHANGE* lpMsg = (CSP_ANS_SIEGEENDCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0B] GS_DGAnsSiegeEndedChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
//#else
//	return;
//#endif

}

//#if(_GSCS==1)
struct CSP_ANS_CASTLEOWNERCHANGE {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int bIsCastleOccupied;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szOwnerGuildName[8];
};
//#endif

void GS_DGAnsCastleOwnerChange(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_CASTLEOWNERCHANGE* lpMsg = (CSP_ANS_CASTLEOWNERCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0C] GS_DGAnsCastleOwnerChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_REGATTACKGUILD {

  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szEnemyGuildName[8];
};
//#endif

void GS_DGAnsRegAttackGuild(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_REGATTACKGUILD* lpMsg = (CSP_ANS_REGATTACKGUILD*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0D] GS_DGAnsRegAttackGuild() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	GCAnsRegCastleSiege(lpMsg->iIndex,lpMsg->iResult,lpMsg->szEnemyGuildName);
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_CASTLESIEGEEND {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
};
//#endif

void GS_DGAnsRestartCastleState(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_CASTLESIEGEEND* lpMsg = (CSP_ANS_CASTLESIEGEEND*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0E] GS_DGAnsRestartCastleState() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	else
	{
		g_CastleSiege.ResetCastleCycle();
	}
//#else
//	return;
//#endif
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_MAPSVRMULTICAST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	char szMsgText[128];	// 6
};

void GS_DGAnsMapSvrMsgMultiCast(LPBYTE lpRecv)
{
	CSP_ANS_MAPSVRMULTICAST * lpMsg = (CSP_ANS_MAPSVRMULTICAST *)lpRecv;

	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x0F] GS_DGAnsMapSvrMsgMultiCast() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}

	char szNotice[144] = {0};
	memcpy(szNotice, lpMsg->szMsgText, 128);

	if ( strlen(szNotice) > 1 )
	{
		PMSG_NOTICE pNotice;

		TNotice::MakeNoticeMsg((TNotice *)&pNotice, 0, szNotice);
		TNotice::SetNoticeProperty((TNotice *)&pNotice, 0, _ARGB(255, 255, 200, 80), 1, 0, 20);
		TNotice::SendNoticeToAllUser((TNotice *)&pNotice);
	}

}

void GS_DGAnsRegGuildMark(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_GUILDREGMARK* lpMsg = (CSP_ANS_GUILDREGMARK*)lpRecv;
		
	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x10] GS_DGAnsRegGuildMark() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
	else
	{
		GCAnsRegGuildMark(lpMsg->iIndex,lpMsg->iResult,lpMsg);
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_GUILDRESETMARK {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iRegMarkCount;
};
//#endif

void GS_DGAnsGuildMarkReset(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_GUILDRESETMARK* lpMsg = (CSP_ANS_GUILDRESETMARK*)lpRecv;
	
	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x11] GS_DGAnsGuildMarkReset() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_GUILDSETGIVEUP {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x18>*/ /*|0x4|*/ int bIsGiveUp;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iRegMarkCount;
};
//#endif

void GS_DGAnsGuildSetGiveUp(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_GUILDSETGIVEUP* lpMsg = (CSP_ANS_GUILDSETGIVEUP*)lpRecv;

	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x12] GS_DGAnsGuildSetGiveUp() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
	
	::GCAnsGiveUpCastleSiege(lpMsg->iIndex,lpMsg->iResult,lpMsg->bIsGiveUp,lpMsg->iRegMarkCount,lpMsg->szGuildName);
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_NPCREMOVE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcIndex;
};
//#endif

void GS_DGAnsNpcRemove(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_NPCREMOVE* lpMsg = (CSP_ANS_NPCREMOVE*)lpRecv;
	
	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x16] GS_DGAnsNpcRemove() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
//#else
//	return;
//#endif
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_CASTLESTATESYNC
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleState;	// 8
	int iTaxRateChaos;	// C
	int iTaxRateStore;	// 10
	int iTaxHuntZone;	// 14
	char szOwnerGuildName[8];	// 18
};

void GS_DGAnsCastleStateSync(LPBYTE lpRecv) //Identical gs-cs 56
{
	CSP_ANS_CASTLESTATESYNC * lpMsg = (CSP_ANS_CASTLESTATESYNC *)lpRecv;

	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		return;
	}

	g_CastleSiegeSync.SetCastleState(lpMsg->iCastleState); 
	g_CastleSiegeSync.SetTaxRateChaos(lpMsg->iTaxRateChaos);
	g_CastleSiegeSync.SetTaxRateStore(lpMsg->iTaxRateStore);
	g_CastleSiegeSync.SetTaxHuntZone(lpMsg->iTaxHuntZone);
	g_CastleSiegeSync.SetCastleOwnerGuild(lpMsg->szOwnerGuildName);
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_CASTLETRIBUTEMONEY
{
	PBMSG_HEAD2 h;
	int iResult;	// 4
	WORD wMapSvrNum;	// 8
};

void GS_DGAnsCastleTributeMoney(LPBYTE lpRecv)
{
	CSP_ANS_CASTLETRIBUTEMONEY * lpMsg = (CSP_ANS_CASTLETRIBUTEMONEY *)lpRecv;

	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x18] GS_DGAnsCastleTributeMoney() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
	
	LogAddTD("[CastleSiege] [0x80][0x18] GS_DGAnsCastleTributeMoney() - Money Tribute OK (%d)",	g_CastleSiegeSync.GetTributeMoney());
	g_CastleSiegeSync.ResetTributeMoney();
	
}

//#if(_GSCS==1)
struct CSP_ANS_RESETCASTLETAXINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;

};
//#endif

void GS_DGAnsResetCastleTaxInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_RESETCASTLETAXINFO* lpMsg = (CSP_ANS_RESETCASTLETAXINFO*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x19] GS_DGAnsResetCastleTaxInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	else
	{
		if(lpMsg->iResult == 1)
		{
			g_CastleSiege.ResetCastleTaxInfo();
		}
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_RESETSIEGEGUILDINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
};
//#endif

void GS_DGAnsResetSiegeGuildInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_RESETSIEGEGUILDINFO* lpMsg = (CSP_ANS_RESETSIEGEGUILDINFO*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x1A] GS_DGAnsResetSiegeGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_RESETREGSIEGEINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
};
//#endif

void GS_DGAnsResetRegSiegeInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_RESETREGSIEGEINFO* lpMsg = (CSP_ANS_RESETREGSIEGEINFO*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x1B] GS_DGAnsResetRegSiegeInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
//#else
//	return;
//#endif
}

void GS_DGAnsCastleInitData(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_CSINITDATA* lpMsg = (CSP_ANS_CSINITDATA*)lpRecv;
	CSP_CSINITDATA* lpMsgBody = (CSP_CSINITDATA*)(lpRecv + sizeof(CSP_ANS_CSINITDATA));

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x81] GS_DGAnsCastleInitData() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(g_CastleSiege.GetDataLoadState() != 2)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2 (%d)",g_CastleSiege.GetDataLoadState()) ;
		return;
	}

	g_CastleSiege.SetDataLoadState(3);

	if(lpMsg->iResult == FALSE)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - lpMsg->iResult == 0");
		MsgBox("[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - lpMsg->iResult == 0");
		return;
	}
	
	BOOL bRET_VAL = FALSE;

	bRET_VAL = g_CastleSiege.SetCastleInitData(lpMsg);

	if(bRET_VAL == FALSE)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - g_CastleSiege.SetCastleInitData() == FALSE");
		return;
	}

	bRET_VAL = g_CastleSiege.SetCastleNpcData(lpMsgBody,lpMsg->iCount);
	
	if(bRET_VAL == FALSE)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - g_CastleSiege.SetCastleNpcData() == FALSE");
		return;
	}

	g_CastleSiege.SetDbDataLoadOK(TRUE);

	g_CastleSiege.SetDataLoadState(4);

	if(lpMsg->iFirstCreate == 1)
	{
		g_CastleSiege.FirstCreateDbNPC();
	}

	g_CastleSiege.Init();
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_NPCDATA {
  /*<thisrel this+0x0>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcDfLevel;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcRgLevel;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcMaxHp;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcHp;
  /*<thisrel this+0x18>*/ /*|0x1|*/ BYTE btNpcX;
  /*<thisrel this+0x19>*/ /*|0x1|*/ BYTE btNpcY;
  /*<thisrel this+0x1a>*/ /*|0x1|*/ BYTE btNpcDIR;
};
//#endif

void GS_DGAnsCastleNpcInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_NPCDATA* lpMsg = (CSP_ANS_NPCDATA*)lpRecv;
	CSP_NPCDATA* lpMsgBody = (CSP_NPCDATA*)(lpRecv+sizeof(CSP_ANS_NPCDATA));

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x82] GS_DGAnsCastleNpcInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_ALLGUILDREGINFO { // <size 0x14>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iCount;
};
struct CSP_GUILDREGINFO {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iRegMarkCount;
  /*<thisrel this+0xc>*/ /*|0x1|*/ BYTE bIsGiveUp;
  /*<thisrel this+0xd>*/ /*|0x1|*/ BYTE btRegRank;
};
struct PMSG_ANS_CSREGGUILDLIST {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
};
struct PMSG_CSREGGUILDLIST {// <size 0xe>
  /*<thisrel this+0x0>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x8>*/ /*|0x1|*/ BYTE btRegMarks1;
  /*<thisrel this+0x9>*/ /*|0x1|*/ BYTE btRegMarks2;
  /*<thisrel this+0xa>*/ /*|0x1|*/ BYTE btRegMarks3;
  /*<thisrel this+0xb>*/ /*|0x1|*/ BYTE btRegMarks4;
  /*<thisrel this+0xc>*/ /*|0x1|*/ BYTE btIsGiveUp;
  /*<thisrel this+0xd>*/ /*|0x1|*/ BYTE btSeqNum;
};
//#endif

void GS_DGAnsAllGuildMarkRegInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_ALLGUILDREGINFO* lpMsg = (CSP_ANS_ALLGUILDREGINFO*)lpRecv;
	CSP_GUILDREGINFO* lpMsgBody = (CSP_GUILDREGINFO*)(lpRecv+sizeof(CSP_ANS_ALLGUILDREGINFO));
	char cBUFFER[1668];
	PMSG_ANS_CSREGGUILDLIST* lpMsgSend;
	PMSG_CSREGGUILDLIST* lpMsgSendBody;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x83] GS_DGAnsAllGuildMarkRegInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(!gObjIsConnected(lpMsg->iIndex))
		return;

	lpMsgSend = (PMSG_ANS_CSREGGUILDLIST*)cBUFFER;
	lpMsgSendBody = (PMSG_CSREGGUILDLIST*)&cBUFFER[12];

	lpMsgSend->iCount = 0;
	lpMsgSend->btResult = lpMsg->iResult;
	
	if(lpMsg->iResult == TRUE)
	{
		lpMsgSend->iCount = lpMsg->iCount;

		for(int i=0;i< lpMsg->iCount;i++)
		{
			lpMsgSendBody[i].btSeqNum = lpMsgBody[i].btRegRank;
			lpMsgSendBody[i].btIsGiveUp = lpMsgBody[i].bIsGiveUp;
			lpMsgSendBody[i].btRegMarks1 = SET_NUMBERH(SET_NUMBERHW(lpMsgBody[i].iRegMarkCount));
			lpMsgSendBody[i].btRegMarks2 = SET_NUMBERL(SET_NUMBERHW(lpMsgBody[i].iRegMarkCount));
			lpMsgSendBody[i].btRegMarks3 = SET_NUMBERH(SET_NUMBERLW(lpMsgBody[i].iRegMarkCount));
			lpMsgSendBody[i].btRegMarks4 = SET_NUMBERL(SET_NUMBERLW(lpMsgBody[i].iRegMarkCount));
			memcpy(&lpMsgSendBody[i].szGuildName,lpMsgBody[i].szGuildName,sizeof(lpMsgSendBody[i].szGuildName));
		}
	}
	
	lpMsgSend->h.set((LPBYTE)lpMsgSend,0xB4,(lpMsgSend->iCount*sizeof(PMSG_CSREGGUILDLIST)+sizeof(PMSG_ANS_CSREGGUILDLIST)));

	DataSend(lpMsg->iIndex,(LPBYTE)lpMsgSend,(lpMsgSend->iCount*sizeof(PMSG_CSREGGUILDLIST)+sizeof(PMSG_ANS_CSREGGUILDLIST)));
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_NPCSAVEDATA {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
};
//#endif

void GS_DGAnsFirstCreateNPC(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_NPCSAVEDATA* lpMsg = (CSP_ANS_NPCSAVEDATA*)lpRecv;

	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x84] GS_DGAnsFirstCreateNPC() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		if(g_CastleSiege.GetDbNpcCreated() == FALSE)
		{
			g_CastleSiege.CreateDbNPC();
			g_CastleSiege.SetDbNpcCreated(1);
		}
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_CALCREGGUILDLIST {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};
//#endif

void GS_DGAnsCalcREgGuildList(LPBYTE lpRecv)
{
//#if (_GSCS==1)
	CSP_ANS_CALCREGGUILDLIST* lpMsg = (CSP_ANS_CALCREGGUILDLIST*)lpRecv;
	CSP_CALCREGGUILDLIST* lpMsgBody = (CSP_CALCREGGUILDLIST*)(lpRecv+sizeof(CSP_ANS_CALCREGGUILDLIST));
	
	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x85] GS_DGAnsCalcREgGuildList() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.SetCalcRegGuildList(lpMsgBody,lpMsg->iCount);
		return;
	}

	LogAddC(2,"[CastleSiege] PACKET-ERROR [0x85] GS_DGAnsCalcREgGuildList() - lpMsg->iResult != 1 (%d)",lpMsg->iResult) ;
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_CSGUILDUNIONINFO {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};
//#endif

void GS_DGAnsCsGulidUnionInfo(LPBYTE lpRecv)
{
//#if (_GSCS==1)
	CSP_ANS_CSGUILDUNIONINFO* lpMsg = (CSP_ANS_CSGUILDUNIONINFO*)lpRecv;
	CSP_CSGUILDUNIONINFO* lpMsgBody = (CSP_CSGUILDUNIONINFO*)(lpRecv+sizeof(CSP_ANS_CSGUILDUNIONINFO));
	
	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x86] GS_DGAnsCsGulidUnionInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.MakeCsTotalGuildInfo(lpMsgBody,lpMsg->iCount);
		return;
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_CSSAVETOTALGUILDINFO {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x3|*/ PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
};
//#endif

void GS_DGAnsCsSaveTotalGuildInfo(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_CSSAVETOTALGUILDINFO* lpMsg = (CSP_ANS_CSSAVETOTALGUILDINFO*)lpRecv;
	
	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x87] GS_DGAnsCsSaveTotalGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.SetIsSiegeGuildList(TRUE);
		return;
	}
	g_CastleSiege.SetIsSiegeGuildList(FALSE);
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_CSLOADTOTALGUILDINFO {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};
//#endif

void GS_DGAnsCsLoadTotalGuildInfo(LPBYTE lpRecv)
{
//#if (_GSCS==1)
	CSP_ANS_CSLOADTOTALGUILDINFO* lpMsg = (CSP_ANS_CSLOADTOTALGUILDINFO*)lpRecv;
	CSP_CSLOADTOTALGUILDINFO* lpMsgBody = (CSP_CSLOADTOTALGUILDINFO*)(lpRecv+sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));

	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x88] GS_DGAnsCsLoadTotalGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.SetCsTotalGuildInfo(lpMsgBody,lpMsg->iCount);
		return;
	}
//#else
//	return;
//#endif
}

//#if(_GSCS==1)
struct CSP_ANS_NPCUPDATEDATA {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x3|*/ PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ WORD wMapSvrNum;
};
//#endif

void GS_DGAnsCastleNpcUpdate(LPBYTE lpRecv)
{
//#if(_GSCS==1)
	CSP_ANS_NPCUPDATEDATA* lpMsg = (CSP_ANS_NPCUPDATEDATA*)lpRecv;

	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x89] GS_DGAnsCastleNpcUpdate() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	char* szResult;

	if(lpMsg->iResult != FALSE)
	{
		szResult = "SUCCEED";
	}
	else
	{
		szResult = "FAIL";
	}

	LogAddTD("[CastleSiege] [0x89] GS_DGAnsCastleNpcUpdate() - Npc Data Update Result : '%s'",szResult);
//#else
//	return;
//#endif
}

struct CWP_REQ_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCrywolfState;	// 8
	int iOccupationState;	// C
};

void GDReqCrywolfSync(int iMapSvrGroup, int iCrywolfState, int iOccupationState)
{
	if ( iMapSvrGroup <0 )
		return;

	CWP_REQ_CRYWOLFSYNC pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB0, sizeof(CWP_REQ_CRYWOLFSYNC));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCrywolfState = iCrywolfState;
	pMsg.iOccupationState = iOccupationState;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

struct CWP_ANS_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
	int iCrywolfState;	// 4
	int iOccupationState;	// 8
};

DWORD dwCrywolfSyncLogTickCount = 0;

void DGAnsCrywolfSync(LPBYTE lpRecv)
{
	CWP_ANS_CRYWOLFSYNC * lpMsg = (CWP_ANS_CRYWOLFSYNC *)lpRecv;

	if ( lpMsg == NULL )
		return;

	int iOldOccupationState = g_CrywolfSync.GetOccupationState();
	g_CrywolfSync.SetCrywolfState(lpMsg->iCrywolfState);
	g_CrywolfSync.SetOccupationState(lpMsg->iOccupationState);

	if ( GetTickCount() > (dwCrywolfSyncLogTickCount+ 60000) )
	{
		LogAddC(4, "[ CRYWOLF ] SYNC Occupation: %d, State: %d", lpMsg->iOccupationState, lpMsg->iCrywolfState);
		dwCrywolfSyncLogTickCount = GetTickCount();
	}
}

struct CWP_REQ_CRYWOLFINFOLOAD
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};

void GDReqCrywolfInfoLoad(int iMapSvrGroup)
{
	if ( iMapSvrGroup <0 )
		return;

	CWP_REQ_CRYWOLFINFOLOAD pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB1, sizeof(CWP_REQ_CRYWOLFINFOLOAD));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

struct CWP_ANS_CRYWOLFINFOLOAD
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
	int iCrywolfState;	// 4
	int iOccupationState;	// 8
};

void DGAnsCrywolfInfoLoad(LPBYTE lpRecv)
{
	CWP_ANS_CRYWOLFINFOLOAD * lpMsg = (CWP_ANS_CRYWOLFINFOLOAD *)lpRecv;

//#if(_GSCS==1)
	if(lpMsg == NULL)
	{
		return;
	}

	int iResult;

	if( true != FALSE && lpMsg->iOccupationState >= 0 && lpMsg->iOccupationState <= 2)
	{
		iResult = 1;
	}
	else
	{
		iResult = 0;
	}

	lpMsg->btResult = iResult;

	if((iResult & 0xFF) != 0)
	{
		g_Crywolf.ApplyCrywolfDBInfo(lpMsg->iCrywolfState,lpMsg->iOccupationState);
		return;
	}

	LogAddC(2,"[ Crywolf ] Incorrect DB OccupationState!!!");
//#endif
}

struct CWP_REQ_CRYWOLFINFOSAVE
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCrywolfState;	// 8
	int iOccupationState;	// C
};

void GDReqCrywolfInfoSave(int iMapSvrGroup, int iCrywolfState, int iOccupationState)
{
	if ( iMapSvrGroup <0 )
		return;

	CWP_REQ_CRYWOLFINFOSAVE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB2, sizeof(CWP_REQ_CRYWOLFINFOSAVE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCrywolfState = iCrywolfState;
	pMsg.iOccupationState = iOccupationState;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

struct CWP_ANS_CRYWOLFINFOSAVE
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
};

void DGAnsCrywolfInfoSave(LPBYTE lpRecv)
{
	CWP_ANS_CRYWOLFINFOSAVE * lpMsg = (CWP_ANS_CRYWOLFINFOSAVE *)lpRecv;
}

typedef struct
{
	PBMSG_HEAD h;
	BYTE subcode;
	char szName[10];
	char szName2[10];
	BYTE btResult;
}PMSG_CHANGE_NAME, *LPPMSG_CHANGE_NAME;
//size 0x19

//004372C0 - identical
void DGChangeNameResult(LPSDHP_CHANGENAME_RESULT lpMsg)
{
	char szAccountID[MAX_ACCOUNT_LEN];

	int aIndex = lpMsg->aIndex;

	BYTE Result = lpMsg->btResult;

	szAccountID[MAX_ACCOUNT_LEN] = 0;

	memcpy(szAccountID, lpMsg->szAccountID, MAX_ACCOUNT_LEN);
	
	if(::gObjIsAccontConnect(aIndex, szAccountID) == FALSE )
	{
		LogAddC(2, lMsg.Get(424), szAccountID);
		CloseClient(aIndex);
		return;
	}

	PMSG_CHANGE_NAME pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x16, sizeof(pMsg));
	
	pMsg.btResult = lpMsg->btResult;

	memcpy(pMsg.szName, lpMsg->szName, MAX_ACCOUNT_LEN);
    memcpy(pMsg.szName2, lpMsg->szName2, MAX_ACCOUNT_LEN);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( Result == 0)
    {
		char Name2[MAX_ACCOUNT_LEN];
		char Name[MAX_ACCOUNT_LEN];
		Name2[MAX_ACCOUNT_LEN] = 0;
		Name[MAX_ACCOUNT_LEN] = 0;

		memcpy(Name2, lpMsg->szName2, MAX_ACCOUNT_LEN);
		memcpy(Name, lpMsg->szName, MAX_ACCOUNT_LEN);

		LogAddTD("Character Name Change : %s,%s", Name, Name2);
    }
}

int N = 0;

//Identical
void DGSummonerStateUpdatedSend(LPPMSG_ANS_SUMMONER_CREATE lpMsg)
{
	char szId[MAX_ACCOUNT_LEN+1];

	szId[MAX_ACCOUNT_LEN] = 0;

	memcpy(szId, lpMsg->szAccountID, MAX_ACCOUNT_LEN);

	int aIndex = lpMsg->aIndex;

	if(::gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 165)), gObj[aIndex].AccountID, szId);
		CloseClient(aIndex);
		return;
	}

	gObj[aIndex].Summoner = 1;//lpMsg->IsSummonerEnable;

	PMSG_UPD_SUMMONER_CREATE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xDE, sizeof(pMsg));

	pMsg.btResult = 0;

	pMsg.IsSummonerEnable = lpMsg->IsSummonerEnable | 15;
	
#ifndef CHARCREATE_TEMP
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
#endif
}

typedef struct
{
	PBMSG_HEAD h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	short aIndex;
}PMSG_REQ_SUMMONER_CREATE, *LPPMSG_REQ_SUMMONER_CREATE;

//Identical (CashShop Buy)
void GDSummonerStateUpdate(LPOBJ lpObj, int iIndex)
{
	PMSG_REQ_SUMMONER_CREATE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA2, sizeof(pMsg));

	memset(pMsg.szAccountID, 0, sizeof(pMsg.szAccountID));
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);

	pMsg.aIndex = lpObj->m_Index;

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}

//Identical
void DGSummonerStateRecv(LPPMSG_ANS_SUMMONER_STATUS lpMsg)
{
	char szId[MAX_ACCOUNT_LEN];

	szId[MAX_ACCOUNT_LEN] = 0;

	memcpy(szId, lpMsg->szAccountID, MAX_ACCOUNT_LEN);

	int aIndex = lpMsg->aIndex;

	if(::gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 165)), gObj[aIndex].AccountID, szId);
		CloseClient(aIndex);
		return;
	}

	if(lpMsg->btResult == 1)
	{
		gObj[aIndex].Summoner = 1;
	}

	PMSG_UPD_SUMMONER_CREATE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xDE, sizeof(pMsg));

	pMsg.btResult = 1;
	pMsg.IsSummonerEnable = lpMsg->btResult;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

struct SDHP_SETGMEVENT
{
	PBMSG_HEAD h;
	char CharName[10];
};

void GDSetGameMasterEvent(char * Name, int aIndex)
{
	char Character[MAX_ACCOUNT_LEN] = { 0 };
	memcpy(Character, Name, MAX_ACCOUNT_LEN);
	// ----
	gObjCloseSet(aIndex, 1);
	// ----
	SDHP_SETGMEVENT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB4, sizeof(pMsg));
	memcpy(pMsg.CharName, Character, sizeof(Character));
	// ----
	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}
// -------------------------------------------------------------------------------

//00440d40	-> 100%
void GDUpdateExtendedInvenCount(LPOBJ lpObj, BYTE btExtendedInvenCount, DWORD dwEventIndex, int iItemPos, bool bBuyAtInGameShop, bool bIsReplace)
{
	SDHP_REQ_SET_EXTENDEDINVEN_COUNT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xD5, sizeof(pMsg));
	// ----
	memcpy(pMsg.szCharName, lpObj->Name, 11);
	pMsg.Number				= lpObj->m_Index;
	pMsg.ExtendedInvenCount = btExtendedInvenCount;
	pMsg.EventIndex			= dwEventIndex;
	pMsg.ItemPos			= iItemPos;
	pMsg.BuyAtInGameShop	= bBuyAtInGameShop;
	pMsg.IsReplace			= bIsReplace;
	// ----
	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}
// -------------------------------------------------------------------------------

//00440dd0	-> ??
void DGUpdateExtendedInvenCountResult(SDHP_ANS_SET_EXTENDEDINVEN_COUNT * lpMsg)
{
	int aIndex = lpMsg->Number;
	// ----
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if( !lpMsg->Result )
	{
		//if( lpMsg->BuyAtInGameShop == 1 )
		//		CCashShopInGame::RollBackUseItem(v8, lpMsg->EventIndex);
		//	else
		//		GCInventoryItemDeleteSend(v9, 0xFFu, 1u);
			GCResultForExtendInvenOrWarehouse(aIndex, 0);
			LogAddTD("[Extended Storage][Inventory] Fail to extend inventory User Name : %s", lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(3483), aIndex, 1);
			return;
	}
	// ----
	lpObj->ExpandedInventory = lpMsg->ExtendedInvenCount;
	// ----
	if( lpObj->ExpandedInventory < 0 || lpObj->ExpandedInventory > 2 )
	{
		lpObj->ExpandedInventory = 0;
	}
	// ----
	//if( lpMsg->BuyAtInGameShop == 1 )
	//{
	//	CCashShopInGame::SetActiveItemResult(v8, lpMsg->EventIndex);
	//}
	//else
	//{
	if( lpMsg->ItemPos > 203 || lpMsg->ItemPos < 0 )
	{
		LogAddTD("[Extended Storage][Inventory] Error Item Pos. User Name : %s. ItemPos : %d", lpObj->Name, lpMsg->ItemPos);
		return;
	}
	// ----
	gObjInventoryItemSet(aIndex, lpMsg->ItemPos, 0xFF);
	lpObj->pInventory[lpMsg->ItemPos].Clear();
	GCInventoryItemDeleteSend(aIndex, lpMsg->ItemPos, 1);
	GCResultForExtendInvenOrWarehouse(aIndex, 1);
	// ----
	char szTemp[0x100] = { 0 };
	wsprintfA(szTemp, lMsg.Get(3481), lpMsg->ExtendedInvenCount);	//maybe 0, 0 in param +
	GCServerMsgStringSend(szTemp, aIndex, 1);	
}
// -------------------------------------------------------------------------------

//00441080	-> 100%
void GDUpdateExtendedWarehouseCount(LPOBJ lpObj, BYTE btExtendedWarehouseCount, DWORD dwEventIndex, int iItemPos, bool bBuyAtInGameShop, bool bIsReplace)
{
	SDHP_REQ_SET_EXTENDEDWAREHOUSE_COUNT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xD6, sizeof(pMsg));
	// ----
	memcpy(pMsg.AccountId, lpObj->AccountID, 11);
	pMsg.Number				= lpObj->m_Index;
	pMsg.ExtendedWarehouseCount = btExtendedWarehouseCount;
	pMsg.EventIndex			= dwEventIndex;
	pMsg.ItemPos			= iItemPos;
	pMsg.BuyAtInGameShop	= bBuyAtInGameShop;
	pMsg.IsReplace			= bIsReplace;
	// ----
	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}
// -------------------------------------------------------------------------------

//00441110	-> ??
void DGUpdateExtendedWarehouseResult(SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT * lpMsg)
{
	int aIndex = lpMsg->Number;
	// ----
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if( !lpMsg->Result )
	{
		//if( lpMsg->BuyAtInGameShop == 1 )
		//		CCashShopInGame::RollBackUseItem(v8, lpMsg->EventIndex);
		//	else
		//		GCInventoryItemDeleteSend(v9, 0xFFu, 1u);
			GCResultForExtendInvenOrWarehouse(aIndex, 0);
			LogAddTD("[Extended Storage][WareHouse] Fail to extend warehouse. User ID : %s", lpObj->AccountID);
			GCServerMsgStringSend(lMsg.Get(3483), aIndex, 1);
			return;
	}
	// ----
	lpObj->ExpandedWarehouse = lpMsg->ExtendedWarehouseCount;
	// ----
	//if( lpMsg->BuyAtInGameShop == 1 )
	//{
	//	CCashShopInGame::SetActiveItemResult(v8, lpMsg->EventIndex);
	//}
	//else
	//{
	if( lpMsg->ItemPos > 203 || lpMsg->ItemPos < 0 )
	{
		LogAddTD("[Extended Storage][WareHouse] Error Item Pos. User Name : %s. ItemPos : %d", lpObj->Name, lpMsg->ItemPos);
		return;
	}
	// ----
	gObjInventoryItemSet(aIndex, lpMsg->ItemPos, 0xFF);
	lpObj->pInventory[lpMsg->ItemPos].Clear();
	GCInventoryItemDeleteSend(aIndex, lpMsg->ItemPos, 1);
	GCResultForExtendInvenOrWarehouse(aIndex, 1);
	// ----
	char szTemp[0x100] = { 0 };
	wsprintfA(szTemp, lMsg.Get(3482), lpMsg->ExtendedWarehouseCount);	//maybe 0, 0 in param +
	GCServerMsgStringSend(szTemp, aIndex, 1);	
}
// -------------------------------------------------------------------------------

#ifdef __MUANGEL__
void UpdateWarehouseStatus(LPOBJ lpObj, BYTE State)
{
	UPDATEWAREHOUSESTATUS pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xD7, 0, sizeof(UPDATEWAREHOUSESTATUS));
	memcpy(pRequest.AccountID, lpObj->AccountID, 10);
	pRequest.Status = State;
	cDBSMng.Send((char*)&pRequest, sizeof(pRequest));
}
#endif
// -------------------------------------------------------------------------------