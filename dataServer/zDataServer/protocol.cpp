#include "stdafx.h"
#include "protocol.h"
#include "ProDef.h"
#include "ServerPrc.h"
#include "winutil.h"
#include "BadSyntaxChk.h"
#include "WzItemSerial.h"
#include "QuestExpDBSet.h"

void GetJoinInfo(LPSDHP_SERVERINFO lpMsg, short aIndex);
void GJPCharacterListRequestCS(LPSDHP_GETCHARLIST lpCLMsg, int aIndex);
void GDCharacterClose(LPSDHP_USERCLOSE lpMsg, short aIndex);
void GJPCharacterCreate(LPSDHP_CREATECHAR lpMsg, short aIndex);
void GJPCharacterDelete(LPSDHP_CHARDELETE lpMsg, short aIndex);
void GDPGetWarehouse(LPSDHP_GETWAREHOUSEDB lpMsg, int aIndex);
void GDPWarehouseNoItemSend(int aIndex, int userindex, char *szId);
void GDPSetWarehouse(LPSDHP_GETWAREHOUSEDB_SAVE lpMsg, int aIndex);
void GDPSetWarehouseMoney(LPSDHP_WAREHOUSEMONEY_SAVE lpMsg, int aIndex);
void GJPGetCharacterInfo(LPSDHP_DBCHARINFOREQUEST lpMsg, short aIndex);
void GJPSetCharacterInfo(LPSDHP_DBCHAR_INFOSAVE lpMsg, short aIndex);
void GJPSetItemSave(LPSDHP_DBCHAR_ITEMSAVE lpMsg, short aIndex);
void GDReqQuestMonKillInfo(BYTE*lpRecv, int aIndex);
void DGSendQuestMonKillInfo(char *szCharName, int iUserIndex, int aIndex);
void GDReqQuestMonKillInfoSave(BYTE*lpRecv, int aIndex);
void GetGameServerInfo(LPSDHP_GAMESERVERINFO lpMsg, short aIndex);
void DSSendResultSend(int aIndex, BYTE headcode, BYTE result);
void GDLoadMedalInfo(LPSDHP_LOAD_MEDALINFO lpMsg, int aIndex);
void GDReqMasterLevelInfo(LPMLP_REQ_MASTERLEVEL_INFO lpMsg, int aIndex);
void GDReqMasterLevelInfoSave(LPMLP_REQ_MASTERLEVEL_INFOSAVE lpMsg, int aIndex);
void GDSkillKeyDataRecv(LPSDHP_SKILLKEYDATA lpMsg);
void GDSkillKeyDataSend(char *name, int userindex, int aIndex);
void GDZenSave(LPSDHP_ZENSAVE lpMsg, int aIndex);
void GDItemCreate(LPSDHP_ITEMCREATE lpMsg, int aIndex);
void GDPropertyItemCreate(LPSDHP_PROPERTY_ITEMCREATE lpMsg, int aIndex);
void GDDeleteTempUserInfo(LPSDHP_DELETE_TEMPUSERINFO lpMsg);
void GDCharacterTransfer(LPSDHP_CHARACTER_TRANSFER lpMsg, int aIndex);
void GDPetItemCreate(LPSDHP_PET_ITEMCREATE lpMsg, int aIndex);
void DGGetPetItemInfo(BYTE *lpRecv, int aIndex);
void DGSetPetItemInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleTotalInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqOwnerGuildMaster(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleNpcBuy(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleNpcRepair(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleNpcUpgrade(BYTE *lpRecv, int aIndex);
void DS_GDReqTaxInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqTaxRateChange(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleMoneyChange(BYTE *lpRecv, int aIndex);
void DS_GDReqSiegeDateChange(BYTE *lpRecv, int aIndex);
void DS_GDReqGuildMarkRegInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqSiegeEndedChange(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleOwnerChange(BYTE *lpRecv, int aIndex);
void DS_GDReqRegAttackGuild(BYTE *lpRecv, int aIndex);
void DS_GDReqRestartCastleState(BYTE *lpRecv, int aIndex);
void DS_GDReqMapSvrMsgMultiCast(BYTE *lpRecv, int aIndex);
void DS_GDReqRegGuildMark(BYTE *lpRecv, int aIndex);
void DS_GDReqGuildMarkReset(BYTE *lpRecv, int aIndex);
void DS_GDReqGuildSetGiveUp(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleNpcRemove(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleStateSync(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleTributeMoney(BYTE *lpRecv, int aIndex);
void DS_GDReqResetCastleTaxInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqResetSiegeGuildInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqResetRegSiegeInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleInitData(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleNpcInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqAllGuildMarkRegInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqFirstCreateNPC(BYTE *lpRecv, int aIndex);
void DS_GDReqCalcRegGuildList(BYTE *lpRecv, int aIndex);
void DS_GDReqCsGuildUnionInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqCsSaveTotalGuildInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqCsLoadTotalGuildInfo(BYTE *lpRecv, int aIndex);
void DS_GDReqCastleNpcUpdate(BYTE *lpRecv, int aIndex);
void GDReqCrywolfSync(BYTE *lpRecv, int aIndex);
void GDReqCrywolfInfoLoad(BYTE *lpRecv, int aIndex);
void GDReqCrywolfInfoSave(BYTE *lpRecv, int aIndex);
void GDReqUserID(BYTE *lpRecv, int aIndex);
void DGChangeName(LPSDHP_CHANGE_NAME lpMsg, int aIndex);
void GJPSetAccountInfo(LPSDHP_REQ_SETACCOUNTINFO lpMsg, int aIndex);
void DGBroadCastGlobalMessage(LPBYTE lpData);

void GensGetMemberInfo(PMSG_GENS_USERINFO_REQ * aRecv, int aIndex);
void GensSaveMemberInfo(PMSG_GENS_SAVEINFO * aRecv, int aIndex);
void GensRegMemberInfo(PMSG_GENS_REGUSER_REQ * aRecv, int aIndex);
void GensDelMemberInfo(PMSG_GENS_DELUSER_REQ * aRecv, int aIndex);
void GensGetRewardResult(PMSG_GENS_REWARD_REQ * aRecv, int aIndex);
void GensSetRewardDate(PMSG_GENS_REWARD_SET * aRecv, int aIndex);

void GDReqPeriodItemExInsert(BYTE *lpRecv, int aIndex);
void GDReqPeriodItemExDelete(BYTE *lpRecv, int aIndex);
void GDReqPeriodItemExSelect(BYTE *lpRecv, int aIndex);
void GDReqLuckyItemInsert(BYTE *lpRecv, int aIndex);
void GDReqLuckyItemInsert2nd(BYTE *lpRecv, int aIndex);
void GDReqLuckyItemDelete(BYTE *lpRecv, int aIndex);
void GDReqLuckyItemSelect(BYTE *lpRecv, int aIndex);

void GDReqPeriodItemInsert(BYTE *lpRecv, int aIndex);
void GDReqPeriodItemDelete(BYTE *lpRecv, int aIndex);
void GDReqPeriodItemUpdate(BYTE *lpRecv, int aIndex);
void GDReqPeriodItemSelect(BYTE *lpRecv, int aIndex);

void GDSetExtendedInvenCount(LPSDHP_REQ_SET_EXTENDEDINVEN_COUNT lpMsg, int aIndex);
void GDSetExtendedWarehouseCount(LPSDHP_REQ_SET_EXTENDEDWAREHOUSE_COUNT lpMsg, int aIndex);

void GDGameShopReqStorage(GAMESHOP_GD_STORAGE * aRecv, int aIndex);
void GDGameShopSaveStorageItem(GAMESHOP_GD_SAVE_STORAGEITEM * aRecv, int aIndex);
void GDGameShopDelStorageItem(GAMESHOP_GD_DEL_STORAGEITEM * aRecv, int aIndex);
void GDGameShopSavePoint(GAMESHOP_GD_SAVE_POINT * aRecv, int aIndex);
void GDGameShopReqPoint(GAMESHOP_GD_REQ_POINT * aRecv, int aIndex);
void GDMacroLoad(LPSDHP_MACRODATA lpMsg, int aIndex);
void GDMacroSave(LPSDHP_MACRODATA lpMsg, int aIndex);

void GDReqQuestExpInfoSave(LP_PMSG_QUESTEXP_INFO lpMsg);
void GDReqQuestExpInfoLoad(LPPMSG_REQ_QUESTEXP_INFO lpMsg, int aIndex);

struct PMSG_DEFAULT2
{
	PBMSG_HEAD h;
	BYTE subcode;
};

void ProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, short aIndex)
{
	if(protoNum > 0)
	{
//		cAM.SetMode(AM_ACTIVE);
	}

	switch(protoNum)
	{
	case 0:
		GetJoinInfo((SDHP_SERVERINFO *)aRecv, aIndex);
		break;
	case 1:
		GJPCharacterListRequestCS((SDHP_GETCHARLIST *)aRecv, aIndex);
		break;
	case 2:
		GDCharacterClose((SDHP_USERCLOSE *)aRecv, aIndex);
		break;
	case 4:
		GJPCharacterCreate((SDHP_CREATECHAR *)aRecv, aIndex);
		break;
	case 5:
		GJPCharacterDelete((SDHP_CHARDELETE *)aRecv, aIndex);
		break;
	case 6:
		GJPGetCharacterInfo((SDHP_DBCHARINFOREQUEST *)aRecv, aIndex);
		break;
    case 7:
		GJPSetCharacterInfo((SDHP_DBCHAR_INFOSAVE *)aRecv, aIndex);
		break;
    case 8:
		GDPGetWarehouse((SDHP_GETWAREHOUSEDB *)aRecv, aIndex);
		break;
    case 9:
		GDPSetWarehouse((SDHP_GETWAREHOUSEDB_SAVE *)aRecv, aIndex);
		break;
    case 0x11:
		GJPSetItemSave((SDHP_DBCHAR_ITEMSAVE *)aRecv, aIndex);
		break;
    case 0x12:
		GDPSetWarehouseMoney((SDHP_WAREHOUSEMONEY_SAVE *)aRecv, aIndex);
		break;
    case 0x13:
		GDReqQuestMonKillInfo(aRecv, aIndex);
		break;
    case 0x14:
		GDReqQuestMonKillInfoSave(aRecv, aIndex);
		break;
    case 0x16:
		DGChangeName((SDHP_CHANGE_NAME *)aRecv, aIndex);
		break;
	case 0x17:
		GDMacroLoad((SDHP_MACRODATA *)aRecv, aIndex);
		break;
	case 0x18:
		GDMacroSave((SDHP_MACRODATA *)aRecv, aIndex);
		break;
    case 0x20:
		GetGameServerInfo((SDHP_GAMESERVERINFO *)aRecv, aIndex);
		break;
    case 0x2D:
		GDLoadMedalInfo((SDHP_LOAD_MEDALINFO *)aRecv, aIndex);
		break;
    case 0x30:
		GDReqMasterLevelInfo((MLP_REQ_MASTERLEVEL_INFO *)aRecv, aIndex);
		break;
    case 0x31:
		GDReqMasterLevelInfoSave((MLP_REQ_MASTERLEVEL_INFOSAVE *)aRecv, aIndex);
		break;
    case 0x60:
		GDSkillKeyDataRecv((SDHP_SKILLKEYDATA *)aRecv);
		break;
    case 0x52:
		GDItemCreate((SDHP_ITEMCREATE *)aRecv, aIndex);
		break;
    case 0x54:
		GDPropertyItemCreate((SDHP_PROPERTY_ITEMCREATE *)aRecv, aIndex);
		break;
    case 0x55:
		GDPetItemCreate((SDHP_PET_ITEMCREATE *)aRecv, aIndex);
		break;
    case 0x56:
		DGGetPetItemInfo(aRecv, aIndex);
		break;
    case 0x57:
		DGSetPetItemInfo(aRecv, aIndex);
		break;
    case 0x80:
		{
			PMSG_DEFAULT2* lpDef = (PMSG_DEFAULT2*)aRecv;

		  switch ( lpDef->subcode )
		  {
			case 0:
			  DS_GDReqCastleTotalInfo(aRecv, aIndex);
			  break;
			case 1:
			  DS_GDReqOwnerGuildMaster(aRecv, aIndex);
			  break;
			case 3:
			  DS_GDReqCastleNpcBuy(aRecv, aIndex);
			  break;
			case 4:
			  DS_GDReqCastleNpcRepair(aRecv, aIndex);
			  break;
			case 5:
			  DS_GDReqCastleNpcUpgrade(aRecv, aIndex);
			  break;
			case 6:
			  DS_GDReqTaxInfo(aRecv, aIndex);
			  break;
			case 7:
			  DS_GDReqTaxRateChange(aRecv, aIndex);
			  break;
			case 8:
			  DS_GDReqCastleMoneyChange(aRecv, aIndex);
			  break;
			case 9:
			  DS_GDReqSiegeDateChange(aRecv, aIndex);
			  break;
			case 0xA:
			  DS_GDReqGuildMarkRegInfo(aRecv, aIndex);
			  break;
			case 0xB:
			  DS_GDReqSiegeEndedChange(aRecv, aIndex);
			  break;
			case 0xC:
			  DS_GDReqCastleOwnerChange(aRecv, aIndex);
			  break;
			case 0xD:
			  DS_GDReqRegAttackGuild(aRecv, aIndex);
			  break;
			case 0xE:
			  DS_GDReqRestartCastleState(aRecv, aIndex);
			  break;
			case 0xF:
			  DS_GDReqMapSvrMsgMultiCast(aRecv, aIndex);
			  break;
			case 0x10:
			  DS_GDReqRegGuildMark(aRecv, aIndex);
			  break;
			case 0x11:
			  DS_GDReqGuildMarkReset(aRecv, aIndex);
			  break;
			case 0x12:
			  DS_GDReqGuildSetGiveUp(aRecv, aIndex);
			  break;
			case 0x16:
			  DS_GDReqCastleNpcRemove(aRecv, aIndex);
			  break;
			case 0x17:
			  DS_GDReqCastleStateSync(aRecv, aIndex);
			  break;
			case 0x18:
			  DS_GDReqCastleTributeMoney(aRecv, aIndex);
			  break;
			case 0x19:
			  DS_GDReqResetCastleTaxInfo(aRecv, aIndex);
			  break;
			case 0x1A:
			  DS_GDReqResetSiegeGuildInfo(aRecv, aIndex);
			  break;
			case 0x1B:
			  DS_GDReqResetRegSiegeInfo(aRecv, aIndex);
			  break;
		  }
		}
		break;
    case 0x81:
		DS_GDReqCastleInitData(aRecv, aIndex);
		break;
    case 0x82:
		DS_GDReqCastleNpcInfo(aRecv, aIndex);
		break;
    case 0x83:
		DS_GDReqAllGuildMarkRegInfo(aRecv, aIndex);
		break;
    case 0x84:
		DS_GDReqFirstCreateNPC(aRecv, aIndex);
		break;
    case 0x85:
		DS_GDReqCalcRegGuildList(aRecv, aIndex);
		break;
    case 0x86:
		DS_GDReqCsGuildUnionInfo(aRecv, aIndex);
		break;
    case 0x87:
		DS_GDReqCsSaveTotalGuildInfo(aRecv, aIndex);
		break;
    case 0x88:
		DS_GDReqCsLoadTotalGuildInfo(aRecv, aIndex);
		break;
    case 0x89:
		DS_GDReqCastleNpcUpdate(aRecv, aIndex);
		break;
    case 0xB0:
		GDReqCrywolfSync(aRecv, aIndex);
		break;
    case 0xB1:
		GDReqCrywolfInfoLoad(aRecv, aIndex);
		break;
    case 0xB2:
		GDReqCrywolfInfoSave(aRecv, aIndex);
		break;
    case 0xCF:
      GDCharacterTransfer((SDHP_CHARACTER_TRANSFER *)aRecv, aIndex);
      break;
	case 0xCE:
		GDDeleteTempUserInfo((SDHP_DELETE_TEMPUSERINFO *)aRecv);
		break;
	case 0xD0: //-> CashShop
		{
			PMSG_DEFAULT2* lpDef = (PMSG_DEFAULT2*)aRecv;
			// ----
			switch ( lpDef->subcode )
			{
			case 3:
				GDReqPeriodItemInsert(aRecv, aIndex);
				break;
			case 5:
				GDReqPeriodItemSelect(aRecv, aIndex);
				break;
			case 7:
				GDReqPeriodItemUpdate(aRecv, aIndex);
				break;
			case 9:
				GDReqPeriodItemDelete(aRecv, aIndex);
				break;
			}
		}
		break;
		// ----
	case 0xA2:
		{
			GJPSetAccountInfo((SDHP_REQ_SETACCOUNTINFO*)aRecv, aIndex);
		}
		break;
		// ----
	case 0xD1:	//-> Period
		{
			PMSG_DEFAULT2* lpDef = (PMSG_DEFAULT2*)aRecv;
			// ----
			switch ( lpDef->subcode )
			{
			case 1:
				GDReqPeriodItemExInsert(aRecv, aIndex);
				break;
			case 2:
				GDReqPeriodItemExDelete(aRecv, aIndex);
				break;
			case 3:
				GDReqPeriodItemExSelect(aRecv, aIndex);
				break;
			case 0x20:
				GDReqLuckyItemSelect(aRecv, aIndex);
				break;
			case 0x22:
				GDReqLuckyItemInsert(aRecv, aIndex);
				break;
			case 0x23:
				GDReqLuckyItemDelete(aRecv, aIndex);
				break;
			case 0x24:
				GDReqLuckyItemInsert2nd(aRecv, aIndex);
				break;
			}
		}
		break;
		// --
	case 0xD3:
		{
			PWMSG_HEAD2 * lpDef2	= (PWMSG_HEAD2*)aRecv;
			// ----
			switch(lpDef2->subcode)
			{
			case 1:	//-> Save storage
				{
					GDGameShopSaveStorageItem((GAMESHOP_GD_SAVE_STORAGEITEM*)aRecv, aIndex);
				}
				break;
				// --
			case 2:	//-> Req storage
				{
					GDGameShopDelStorageItem((GAMESHOP_GD_DEL_STORAGEITEM*)aRecv, aIndex);
				}
				break;
				// --
			case 3:
				{
					GDGameShopReqStorage((GAMESHOP_GD_STORAGE*)aRecv, aIndex);
				}
				break;
			}
		}
		break;
	case 0xD2:	//-> CashShop
		{
			PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2*)aRecv;
			// ----
			switch(lpDef->subcode)
			{
			case 3:	//-> Save point
				{
					GDGameShopSavePoint((GAMESHOP_GD_SAVE_POINT*)aRecv, aIndex);
				}
				break;
				// --
			case 4:	//-> Req point
				{
					GDGameShopReqPoint((GAMESHOP_GD_REQ_POINT*)aRecv, aIndex);
				}
				break;
			}
		}
		break;
		// --
	case 0xF8: //-> Gens
		{
			PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2*)aRecv;
			// ----
			switch(lpDef->subcode)
			{
			case 0x01: //-> Get user gens info
				{
					GensGetMemberInfo((PMSG_GENS_USERINFO_REQ*)aRecv, aIndex);
					//-> Sender: ReqExDBGensInfo(LPOBJ lpObj)
				}
				break;
				// --
			case 0x02: //->
				{
					
				}
				break;
				// --
			case 0x03: //-> Register gens member
				{
					GensRegMemberInfo((PMSG_GENS_REGUSER_REQ*)aRecv, aIndex);
					//-> Sender: ReqRegGensMember(LPOBJ lpObj, unsigned char btInfluence)
				}
				break;
				// --
			case 0x04: //-> Delete gens member
				{
					GensDelMemberInfo((PMSG_GENS_DELUSER_REQ*)aRecv, aIndex);
					//-> Sender: ReqSecedeGensMember(LPOBJ lpObj)
				}
				break;
				// --
			case 0x07: //-> Save contribution & class
				{
					GensSaveMemberInfo((PMSG_GENS_SAVEINFO*)aRecv, aIndex);
					//-> Sender: GDReqSaveContributePoint(LPOBJ lpObj)
				}
				break;
				// --
			case 0x08: //-> Check reward send
				{
					GensGetRewardResult((PMSG_GENS_REWARD_REQ*)aRecv, aIndex);
				}
				break;
				// --
			case 0x09:
				{
					GensSetRewardDate((PMSG_GENS_REWARD_SET*)aRecv, aIndex);
				}
				break;
			}
		}
		break;
		// --
	case 0xAE:
		{
			//MUHELPER_REQ_MACROSAVE * Request = (MUHELPER_REQ_MACROSAVE*)aRecv;
			//gCharDbSet.SaveMacroBind(Request->Name, Request->MacroBind);
		}
		break;
		// --
	case 0xD5:
		{
			GDSetExtendedInvenCount((SDHP_REQ_SET_EXTENDEDINVEN_COUNT *)aRecv, aIndex);
		}
		break;
		// --
	case 0xD6:
		{
			GDSetExtendedWarehouseCount((SDHP_REQ_SET_EXTENDEDWAREHOUSE_COUNT *)aRecv, aIndex);
		}
		break;
		// --
	case 0xF6:
		{
			GDReqQuestExpInfoSave((_PMSG_QUESTEXP_INFO *)aRecv);
		}
		break;
		// --
    case 0xF7:
		{
			GDReqQuestExpInfoLoad((PMSG_REQ_QUESTEXP_INFO *)aRecv, aIndex);
		}
		break;
		// ---
	case 0xD7:	//Custom
		{
#if( __MUANGEL__ == 1 )
			PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2*)aRecv;
			// ----
			switch(lpDef->subcode)
			{
			case 0:
				UPDATEWAREHOUSESTATUS * pRequest = (UPDATEWAREHOUSESTATUS*)aRecv;
				gACDbSet.SetWarehouseStatus(pRequest->AccountID, pRequest->Status);
				break;
			}
#endif
		}
		break;
	}
}

void GetJoinInfo(LPSDHP_SERVERINFO lpMsg, short aIndex)
{
	SDHP_RESULT pResult;

	pResult.h.c = PMHC_BYTE;
	pResult.h.headcode = 0;
	pResult.h.size = sizeof(SDHP_RESULT);
	pResult.Result = 1;
	pResult.ItemCount = gItemCount;

	if(gSObjSetInfo(aIndex, lpMsg->Port, lpMsg->Type, lpMsg->ServerName, lpMsg->ServerCode) == FALSE)
	{
		pResult.Result = 0;
		gWindow.PrintLog("Client Join Fail %s %d", __FILE__, __LINE__);
	}

	wsjServer.DataSend(aIndex, (char*)&pResult, pResult.h.size);
}

struct SDHP_ANS_ACCOUNTINFO
{
	PBMSG_HEAD h;
	char AccountId[MAX_IDSTRING+1];
	short Number;
	BYTE Result;
	BYTE bSummoner;
	BYTE bRageFighter;
};

void GJPCharacterListRequestCS(LPSDHP_GETCHARLIST lpCLMsg, int aIndex)
{
	/////////////////////////////////////////////////////////////////////////
	SDHP_ANS_ACCOUNTINFO Result;

	char characterName[MAX_IDSTRING+1];
	char szId[MAX_IDSTRING+1];

	memset(Result.AccountId, 0, MAX_IDSTRING+1);
	memcpy(Result.AccountId, lpCLMsg->Id, MAX_IDSTRING);

	Result.h.set((LPBYTE)&Result, 0xA1, sizeof(SDHP_ANS_ACCOUNTINFO));
	
	Result.Number = lpCLMsg->Number;

	Result.bSummoner = gACDbSet.GetSummonerCardInfo(Result.AccountId);
	Result.bRageFighter = gACDbSet.GetRageFighterCardInfo(Result.AccountId);
	Result.Result = 0;

	wsjServer.DataSend(aIndex, (char*)&Result, sizeof(SDHP_ANS_ACCOUNTINFO));
	//////////////////////////////////////////////////////////////////////////

	BYTE result = 1;
	int listcount = 0;

	int n;
	SDHP_CHARLISTCOUNT pCount;
	SDHP_CHARLIST pCharList;
	char SendBuf[512];

	int lOfs = 0;
	int Count = 0;

	CString strCharId[5];

	memset(&pCharList, 0, sizeof(SDHP_CHARLIST));
	memset(pCount.AccountId, 0, sizeof(pCount.AccountId));
	memset(szId, 0, MAX_IDSTRING+1);

	memcpy(szId, lpCLMsg->Id, MAX_IDSTRING);
	memcpy(pCount.AccountId, lpCLMsg->Id, MAX_IDSTRING);

	memset(SendBuf, 0, 512);
	pCount.GenerableClass = 0;

	lOfs += 28;

	if(result == 1)
	{
		if(gACDbSet.GetAccountId(szId) == 1)
		{
			result = 1;
		}
		else
		{
			result = 0;

			if(gACDbSet.CreateAccountCharacter(szId) == 1)
			{
				result = 1;
			}
		}
	}

	AccountCharacterInfo ACInfo;

	if(result == 1)
	{
		memset(&ACInfo, 0, sizeof(AccountCharacterInfo));

		if(gACDbSet.GetAccountInfo(szId, &ACInfo) == FALSE)
		{
			result = 0;
		}
		else
		{
			result = 1;

			strCharId[0] = ACInfo.GameID1;
			strCharId[1] = ACInfo.GameID2;
			strCharId[2] = ACInfo.GameID3;
			strCharId[3] = ACInfo.GameID4;
			strCharId[4] = ACInfo.GameID5;

			pCount.MoveCnt = ACInfo.MoveCnt;
			pCount.ExpandedWarehouse = ACInfo.ExpandedWarehouse;

			listcount = 0;

			for ( n = 0; n < 5; n++ )
			{
				if(strCharId[n].GetLength() > 0 && strCharId[n].GetLength() <= MAX_IDSTRING)
				{
					memset(characterName, 0, MAX_IDSTRING+1);
					strcpy(characterName, strCharId[n].GetBuffer(MAX_IDSTRING));

					int _level;
					int _class;
					BYTE Inventory[MAX_DBINVENTORY];

					BOOL bGetPrevChar = FALSE;
					BYTE _ctlcode = 0;
					BYTE _dbverstion = 0;
					char _pkLevel = 0;
					BYTE _btGuildStatus = 0xFF;

					bGetPrevChar = gCharPreDBSet.GetChar(characterName, _level, _class, Inventory, _ctlcode, _dbverstion, _btGuildStatus);

					if(bGetPrevChar == TRUE)
					{
						pCharList.Index = n;
						pCharList.Level = _level;
						pCharList.Class = _class;
						pCharList.CtlCode = _ctlcode;
						pCharList.DbVersion = _dbverstion;
						pCharList.btGuildStatus = _btGuildStatus;
						memcpy(pCharList.Name, characterName, MAX_IDSTRING);

						if( (_ctlcode&0x80) == FALSE)
						{
							if(_level >= 150)
							{
								if(_level >= 220)
								{
									if(pCount.GenerableClass < 2)
									{
										pCount.GenerableClass = 2;
									}
								}
								else if(pCount.GenerableClass < 2)
								{
									pCount.GenerableClass = 1;
								}
							}
						}

						if ( _dbverstion == 0)
						{
							for ( int i = 0; i < MAX_EQUIPMENT; i++ )
							{
								memcpy(&pCharList.dbInventory[2 * i], &Inventory[7 * i], 2);
							}							
						}
						else if ( _dbverstion == 1 || _dbverstion == 2)
						{
							for(int k = 0; k < MAX_EQUIPMENT; k++)
							{
								memcpy(&pCharList.dbInventory[3 * k], &Inventory[10 * k], 2);
								pCharList.dbInventory[3 * k + 2] = Inventory[10 * k + 7];
							}							
						}
						else
						{
							for(int j = 0; j < MAX_EQUIPMENT; j++)
							{
								memcpy(&pCharList.dbInventory[4 * j], &Inventory[16 * j], 2);
								pCharList.dbInventory[4 * j + 2] = Inventory[16 * j + 7];
								pCharList.dbInventory[4 * j + 3] = Inventory[16 * j + 9];
							}
						}

						memcpy(&SendBuf[lOfs], &pCharList, sizeof(SDHP_CHARLIST));
						lOfs += sizeof(SDHP_CHARLIST);
						Count++;
					}
				}
			}
		}
	}

	pCount.h.c = PMHC_WORD;
	pCount.h.headcode = 0x01;
	pCount.Number = lpCLMsg->Number;
	pCount.Count = Count;
	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendBuf, &pCount, sizeof(SDHP_CHARLISTCOUNT));

	wsjServer.DataSend(aIndex, (char*)SendBuf, lOfs);
}

void GDCharacterClose(LPSDHP_USERCLOSE lpMsg, short aIndex)
{
	char szChar[MAX_IDSTRING+1];
	char szGuildName[MAX_GUILDNAMESTRING+1];

	memset(szChar, 0, MAX_IDSTRING+1);
	memset(szGuildName, 0, MAX_GUILDNAMESTRING+1);

	memcpy(szChar, lpMsg->CharName, MAX_IDSTRING);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);

	for ( int n = 0; n < 100; n++ )
	{
		if(gSObj[n].Connected != 0)
		{
			wsjServer.DataSend(n, (char*)lpMsg, lpMsg->h.size);
		}
	}
}

void GJPCharacterCreate(LPSDHP_CREATECHAR lpMsg, short aIndex)
{
	char szId[MAX_IDSTRING+1];
	char accountId[MAX_IDSTRING+1];
	BYTE defclass;
	int Result = 0;
	CharacterInfo_Struct tCharInfo;
	SDHP_CREATECHARRESULT pMsgResult;
	int CurCharCount;

	Result = 1;

	accountId[MAX_IDSTRING] = '\0';
	memcpy(accountId, lpMsg->AccountId, MAX_IDSTRING);

	CurCharCount = gACDbSet.GetAccountCharBlank(accountId);

	if(CurCharCount < 0)
	{
		gWindow.PrintLog("error-L3 : Character Max Over :%d User(%s)", 5, accountId);
		Result = 2;
	}

	szId[MAX_IDSTRING] = 0;
	memcpy(szId, lpMsg->Name, MAX_IDSTRING);

	if ( SQLSyntexCheck(szId) == FALSE)
	{
		Result = 0;
	}

	if ( cBadStrChk.CmpString(szId) == TRUE )
	{
		Result = 0;
	}
	
	if ( gMuName.CmpString(szId) == 1 )
	{
		Result = 0;
	}

	pMsgResult.h.c = PMHC_BYTE;
	pMsgResult.h.size = sizeof(SDHP_CREATECHARRESULT);
	pMsgResult.h.headcode = 0x04;
	pMsgResult.Result = Result;
	pMsgResult.Number = lpMsg->Number;
	pMsgResult.ClassSkin = lpMsg->ClassSkin;
	pMsgResult.Pos = CurCharCount;

	memcpy(pMsgResult.AccountId, lpMsg->AccountId, MAX_IDSTRING);
	memcpy(pMsgResult.Name, lpMsg->Name, MAX_IDSTRING);

	if ( Result == 1 )
	{
		memset(&tCharInfo, 0, sizeof(CharacterInfo_Struct));
		defclass = lpMsg->ClassSkin >> 4;

		if ( defclass > 7 )
		{
			gWindow.PrintLog("error-L3 : Class over value %s %d",__FILE__, __LINE__);
			defclass = 1;
		}
		
		strcpy(tCharInfo.Name, szId);
		ItemByteConvert16(tCharInfo.dbInventory, DCInfo.DefClass[defclass].Equipment, MAX_EQUIPMENT);
		
		for (int n = 0; n < MAX_EQUIPMENT; n++ )
		{
			pMsgResult.Equipment[2 * n] = tCharInfo.dbInventory[16 * n];
			pMsgResult.Equipment[2 * n + 1] = tCharInfo.dbInventory[16 * n + 1];
		}

		pMsgResult.Result = gCharDbSet.CreateCharacter(accountId, szId, lpMsg->ClassSkin);
		pMsgResult.Level = 1;

		if ( lpMsg->ClassSkin == DB_CLASS_TYPE_DARKLORD )
		{
			pMsgResult.Level = 1;
		}

		gWindow.PrintLog("캐릭터 생성 %s %s %d", accountId, szId, lpMsg->ClassSkin);
	}
	
	if ( pMsgResult.Result == 1 )
	{
		gACDbSet.SaveAccountCharacter(accountId, CurCharCount, szId);
	}
	else
	{
		gWindow.PrintLog("[ERROR!!] CreateCharacter Fail Result : %d", pMsgResult.Result);
	}
	
	wsjServer.DataSend(aIndex, (char*)&pMsgResult, pMsgResult.h.size);
}

void GJPCharacterDelete(LPSDHP_CHARDELETE lpMsg, short aIndex)
{
	SDHP_CHARDELETERESULT pResult;
	char szAccountId[MAX_IDSTRING+1];
	char szCharName[MAX_IDSTRING+1];
	char szGuildName[MAX_GUILDNAMESTRING+1];

	memset(szAccountId, 0, MAX_IDSTRING+1);
	memset(szCharName, 0, MAX_IDSTRING+1);
	memset(szGuildName, 0, MAX_GUILDNAMESTRING+1);

	pResult.h.c = PMHC_BYTE;
	pResult.h.size = sizeof(SDHP_CHARDELETERESULT);
	pResult.h.headcode = 0x05;
	pResult.Result = 1;
	pResult.Number = lpMsg->Number;

	memcpy(pResult.AccountID, lpMsg->AccountID, MAX_IDSTRING);

	szAccountId[MAX_IDSTRING] = '\0';
	szCharName[MAX_IDSTRING] = '\0';
	szGuildName[MAX_GUILDNAMESTRING] = '\0';

	memcpy(szAccountId, lpMsg->AccountID, MAX_IDSTRING);
	memcpy(szCharName, lpMsg->Name, MAX_IDSTRING);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);

	if ( SQLSyntexCheck(szAccountId) == FALSE || SQLSyntexCheck(szCharName) == FALSE)
	{
		pResult.Result = 0;
	}
	
	if ( strlen(szAccountId) < 1 )
	{
		pResult.Result = 0;
	}
	
	if ( strlen(szCharName) < 1 )
	{
		pResult.Result = 0;
	}
	
	if ( pResult.Result == 1 )
	{
		if ( gCharDbSet.DeleteCharacter(szAccountId, szCharName) == FALSE)
		{
			pResult.Result = 0;
		}
		else
		{
			if( gACDbSet.DeleteAccountCharacter(szAccountId, szCharName) >= 0 )
			{
				pResult.Result = 1;
			}
			
			if ( gCharDbSet.DeleteCharacter_DregInfo(szCharName) == 1 )
			{
				gWindow.PrintLog("Character Dreg Info Delete [%s]", szCharName);
			}
		}
    }

	wsjServer.DataSend(aIndex, (char*)&pResult, pResult.h.size);
  
	if ( lpMsg->Guild == 1 )
	{
		SDHP_GUILDDESTROY pgmsg;

		memcpy(pgmsg.GuildName, szGuildName, MAX_GUILDNAMESTRING);
		memcpy(pgmsg.Master, szCharName, MAX_IDSTRING);

		pgmsg.NumberH = SET_NUMBERH(lpMsg->Number);
		pgmsg.NumberL = SET_NUMBERL(lpMsg->Number);

		gWindow.PrintLog("Character Delete... [%s] Guild Delete [%s] ", szCharName, szGuildName);
	}
	else if ( lpMsg->Guild == 2 )
    {
		SDHP_GUILDDESTROY pgmsg;

		memcpy(pgmsg.GuildName, szGuildName, MAX_GUILDNAMESTRING);
		memcpy(pgmsg.Master, szCharName, MAX_IDSTRING);

		pgmsg.NumberH = SET_NUMBERH(lpMsg->Number);
		pgmsg.NumberL = SET_NUMBERL(lpMsg->Number);
    }
}

void GDPGetWarehouse(LPSDHP_GETWAREHOUSEDB lpMsg, int aIndex)
{
	char szId[MAX_IDSTRING+1];
	SDHP_GETWAREHOUSEDB_SAVE pMsg;

	int _dbversion = 0;

	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountID, MAX_IDSTRING);

	pMsg.h.c = PMHC_WORD;
	pMsg.h.headcode = 0x08;
	pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
	pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
	pMsg.aIndex = lpMsg->aIndex;
	pMsg.Money = 0;

	int len = strlen(szId);

	if(len <= 0 || len > MAX_IDSTRING)
	{
		return;
	}

	if(CWhDBSet.Get(szId) == FALSE)
	{
		CWhDBSet.Create(szId);
		GDPWarehouseNoItemSend(aIndex, pMsg.aIndex, szId);
		return;
	}

	memcpy(pMsg.AccountID, szId, MAX_IDSTRING);
    CWhDBSet.Load(szId, pMsg.dbItems, pMsg.Money, pMsg.DbVersion, pMsg.pw);

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(SDHP_GETWAREHOUSEDB_SAVE));
}

void GDPWarehouseNoItemSend(int aIndex, int userindex, char *szId)
{
	SDHP_GETWAREHOUSEDB_RESULT pMsg;

	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0x10;
	pMsg.h.size = sizeof(SDHP_GETWAREHOUSEDB_RESULT);
	pMsg.aIndex = userindex;
	memcpy(pMsg.AccountID, szId, MAX_IDSTRING);

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(SDHP_GETWAREHOUSEDB_RESULT));
}

void GDPSetWarehouse(LPSDHP_GETWAREHOUSEDB_SAVE lpMsg, int aIndex)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountID, MAX_IDSTRING);
	
	int len = strlen(szId);

	if(len <= 0 || len > MAX_IDSTRING)
	{
		gWindow.PrintLog("%s] 저장 에러 %s %d", szId, __FILE__, __LINE__);
		return ;
	}
	CWhDBSet.Save(szId, lpMsg->dbItems, lpMsg->Money, lpMsg->pw);
}

void GDPSetWarehouseMoney(LPSDHP_WAREHOUSEMONEY_SAVE lpMsg, int aIndex)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountID, MAX_IDSTRING);
	
	int len = strlen(szId);

	if(len <= 0 || len > MAX_IDSTRING)
	{
		gWindow.PrintLog("%s] 저장 에러 %s %d", szId, __FILE__, __LINE__);
		return ;
	}
	CWhDBSet.MoneyUpdate(szId, lpMsg->Money);
}

void GJPGetCharacterInfo(LPSDHP_DBCHARINFOREQUEST lpMsg, short aIndex)
{
	SDHP_DBCHAR_INFORESULT pChar;
	CharacterInfo_Struct CharObj;
	char szAccountId[MAX_IDSTRING+1];
	
	pChar.h.c = PMHC_WORD;
	pChar.h.headcode = 0x6;
	pChar.h.sizeH = HIBYTE(sizeof(SDHP_DBCHAR_INFORESULT));
	pChar.h.sizeL = LOBYTE(sizeof(SDHP_DBCHAR_INFORESULT));
	pChar.result = 0x1;
	pChar.Number = lpMsg->Number;

	szAccountId[MAX_IDSTRING] = '\0';
	pChar.Name[MAX_IDSTRING] = '\0';
	
	memcpy(pChar.Name, lpMsg->Name, MAX_IDSTRING);
	memcpy(szAccountId, lpMsg->AccountID, MAX_IDSTRING);
	memcpy(pChar.AccountID, szAccountId, MAX_IDSTRING);
	BuxConvert(pChar.Name, MAX_IDSTRING);

	if ( SpaceSyntexCheck(pChar.Name) == 1 )
	{
		if ( gCharDbSet.GetCharacter(szAccountId, pChar.Name, &CharObj) == FALSE )
		{
			pChar.result = 0;
		}

		if ( pChar.result == 1 )
		{
			pChar.Class = CharObj.Class;
			pChar.Level = LOWORD(CharObj.Level);
			pChar.LevelUpPoint = CharObj.LevelUpPoint;
			pChar.Exp = CharObj.Experience;
			pChar.NextExp = CharObj.NextExperience;
			pChar.Money = CharObj.Money;
			pChar.Str = CharObj.Strength;
			pChar.Dex = CharObj.Dexterity;
			pChar.Vit = CharObj.Vitality;
			pChar.Energy = CharObj.Energy;
			pChar.Life = (WORD)(CharObj.Life * 10.0);
			pChar.MaxLife = (WORD)(CharObj.MaxLife * 10.0);
			pChar.Mana = (WORD)(CharObj.Mana * 10.0);
			pChar.MaxMana = (WORD)(CharObj.MaxMana * 10.0);
			pChar.MapNumber = CharObj.MapNumber;
			pChar.MapX = CharObj.MapX;
			pChar.MapY = CharObj.MapY;
			pChar.Dir = CharObj.Dir;
			pChar.PkCount = CharObj.PkCount;
			pChar.PkLevel = CharObj.PkLevel;
			pChar.PkTime = CharObj.PkTime;
			pChar.CtlCode = CharObj.CtlCode;
			pChar.DbVersion = CharObj.DbVersion;
			pChar.Leadership = CharObj.Leadership;
			pChar.ChatLitmitTime = CharObj.ChatLitmitTime;
			pChar.iFruitPoint = CharObj.FruitPoint;
			pChar.Reset = CharObj.Reset;
			pChar.GrandReset = CharObj.GrandReset;
			pChar.ExpandedInventory = CharObj.ExpandedInventory;
#ifdef __ALIEN__
			pChar.DbOffExp = CharObj.DbOffExp;
#endif
			memcpy(pChar.dbInventory, CharObj.dbInventory, MAX_DBINVENTORY);
			memcpy(pChar.dbMagicList, CharObj.dbMagicList, MAX_DBMAGIC);
			memcpy(pChar.dbQuest, CharObj.dbQuest, MAX_QUEST);
			gACDbSet.CurAccountCharacterSet(szAccountId, pChar.Name);		
		}
	}
	else
	{
		pChar.result = 0;
	}

	wsjServer.DataSend(aIndex, (char*)&pChar, sizeof(SDHP_DBCHAR_INFORESULT));
	
	if ( pChar.result == 1 )
	{
		GDSkillKeyDataSend(pChar.Name, lpMsg->Number, aIndex);
	}
}

void GJPSetCharacterInfo(LPSDHP_DBCHAR_INFOSAVE lpMsg, short aIndex)
{
	CharacterInfo_Struct CharObject;
	memset(&CharObject, 0, sizeof(CharacterInfo_Struct));

	if ( strlen(lpMsg->Name) < 1 )
	{
		gWindow.PrintLog("error-L2 : Name is zero %s %d", __FILE__, __LINE__);
		return;
	}

	CharObject.AccountID[MAX_IDSTRING] = '\0';
	CharObject.Name[MAX_IDSTRING] = '\0';

	memcpy(CharObject.Name, lpMsg->Name, MAX_IDSTRING);

	gCharDbSet.GetAccountID(CharObject.Name, CharObject.AccountID);

	gWindow.PrintLine(1);
	gWindow.PrintLog("Account = %s", CharObject.AccountID);
	gWindow.PrintLine(1);

	CharObject.Class = lpMsg->Class;
	CharObject.Level = lpMsg->Level;
	CharObject.LevelUpPoint = lpMsg->LevelUpPoint;
	CharObject.Experience = lpMsg->Exp;
	CharObject.NextExperience = lpMsg->NextExp;
	CharObject.Money = lpMsg->Money;
	CharObject.Strength = lpMsg->Str;
	CharObject.Dexterity = lpMsg->Dex;
	CharObject.Vitality = lpMsg->Vit;
	CharObject.Energy = lpMsg->Energy;
    
	if ( lpMsg->Life != 0)
	{
      CharObject.Life = (float)lpMsg->Life;
	  CharObject.Life /= 10.0f;
	}
    else
	{
		CharObject.Life = 0;
	}
    
	if ( lpMsg->MaxLife != 0)
	{
      CharObject.MaxLife = (float)lpMsg->MaxLife;
	  CharObject.MaxLife /= 10.0f;
	}
    else
	{
		CharObject.MaxLife = 0;
	}
	
	if ( lpMsg->Mana != 0)
	{
      CharObject.Mana = (float)lpMsg->Mana;
	  CharObject.Mana /= 10.0f;
	}
	else
	{
		CharObject.Mana = 0;
	}
	
	if ( lpMsg->MaxMana != 0)
	{
		CharObject.MaxMana = (float)lpMsg->MaxMana;
		CharObject.MaxMana /= 10.0f;
	}
	else
	{
		CharObject.MaxMana = 0;
	}
	
	CharObject.MapNumber = lpMsg->MapNumber;
	CharObject.MapX = lpMsg->MapX;
	CharObject.MapY = lpMsg->MapY;
	CharObject.Dir = lpMsg->Dir;
	CharObject.PkCount = lpMsg->PkCount;
	CharObject.PkLevel = lpMsg->PkLevel;
	CharObject.PkTime = lpMsg->PkTime;
	CharObject.Leadership = lpMsg->Leadership;
	CharObject.ChatLitmitTime = lpMsg->ChatLitmitTime;
	CharObject.FruitPoint = lpMsg->iFruitPoint;
	CharObject.Reset = lpMsg->Reset;
	CharObject.GrandReset = lpMsg->GrandReset;
	CharObject.ExpandedInventory = lpMsg->ExpandedInventory;

	memcpy(CharObject.dbInventory, lpMsg->dbInventory, MAX_DBINVENTORY);
	memcpy(CharObject.dbMagicList, lpMsg->dbMagicList, MAX_DBMAGIC);
	memcpy(CharObject.dbQuest, lpMsg->dbQuest, MAX_QUEST);
	gCharDbSet.SaveCharacter(CharObject.Name, &CharObject);
	
	if ( lpMsg->CharInfoSave == 1 )
	{
		gCharDbSet.CurCharNameSave(CharObject.Name);
	}
}

void GJPSetItemSave(LPSDHP_DBCHAR_ITEMSAVE lpMsg, short aIndex)
{
	char szName[MAX_IDSTRING+1];
	memset(szName, 0, MAX_IDSTRING+1);
	memcpy(szName, lpMsg->Name, MAX_IDSTRING);
	
	if ( strlen(szName) <= 0)
	{
		return;
	}
	
	gCharDbSet.SaveItem(szName, lpMsg->dbInventory);
}

void GDReqQuestMonKillInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	LPSDHP_REQ_QUEST_MONSTER_KILL lpMsg = (LPSDHP_REQ_QUEST_MONSTER_KILL)lpRecv;
	DGSendQuestMonKillInfo(lpMsg->szCharName, lpMsg->iUserIndex, aIndex);
}

void DGSendQuestMonKillInfo(char *szCharName, int iUserIndex, int aIndex)
{
	SDHP_QUEST_MONSTER_KILL_INFO pMsgSend;

	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x14, sizeof(SDHP_QUEST_MONSTER_KILL_INFO));
	pMsgSend.iUserIndex = iUserIndex;

	if ( QuestMonKillDBSet.DSDB_QueryQuestMonKillInfoLoad(szCharName, &pMsgSend) == TRUE )
	{
		pMsgSend.btQuestDBIndex = -1;
		pMsgSend.iQuestMonIndex1 = -1;
		pMsgSend.iKillCount1 = -1;
		pMsgSend.iQuestMonIndex2 = -1;
		pMsgSend.iKillCount2 = -1;
		pMsgSend.iQuestMonIndex3 = -1;
		pMsgSend.iKillCount3 = -1;
		pMsgSend.iQuestMonIndex4 = -1;
		pMsgSend.iKillCount4 = -1;
		pMsgSend.iQuestMonIndex5 = -1;
		pMsgSend.iKillCount5 = -1;
	}

	memcpy(pMsgSend.szCharName, szCharName, MAX_IDSTRING);
	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(SDHP_QUEST_MONSTER_KILL_INFO));
}

void GDReqQuestMonKillInfoSave(BYTE*lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

    QuestMonKillDBSet.DSDB_QueryQuestMonKillInfoSave((LPSDHP_QUEST_MONSTER_KILL_INFO)lpRecv);
}

void GetGameServerInfo(LPSDHP_GAMESERVERINFO lpMsg, short aIndex)
{
	gGSDbSet.SetCount(lpMsg->ItemCount);
}

void DSSendResultSend(int aIndex, BYTE headcode, BYTE result)
{
	SDHP_DEFAULT_RESULT pMsg;

	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = headcode;
	pMsg.h.size = sizeof(SDHP_DEFAULT_RESULT);
	pMsg.Result = result;
	wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);
}

void GDLoadMedalInfo(LPSDHP_LOAD_MEDALINFO lpMsg, int aIndex)
{
	SDHP_LOAD_MEDALINFO_RESULT pMsg;

	memset(&pMsg, 0, sizeof(SDHP_LOAD_MEDALINFO_RESULT));
	pMsg.h.set((LPBYTE)&pMsg, 0x2D, sizeof(SDHP_LOAD_MEDALINFO_RESULT));

	pMsg.wIndex = lpMsg->wIndex;
	pMsg.dwItemSerial = lpMsg->dwItemSerial;
	pMsg.wItemIndex = lpMsg->wItemIndex;

	if ( gGSDbSet.GetPropertyItemInfo(lpMsg->dwItemSerial, &pMsg) == 1 )
	{
		wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);
	}
	else
	{
		gWindow.PrintLog("error-L2 : GetPropertyItemInfo() serial:%u index: %d", lpMsg->dwItemSerial, lpMsg->wIndex);
	}
}

void GDReqMasterLevelInfo(LPMLP_REQ_MASTERLEVEL_INFO lpMsg, int aIndex)
{
	MLP_ANS_MASTERLEVEL_INFO pMsgSend;

	if ( lpMsg == NULL)
	{
		return;
	}
  
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x30, sizeof(MLP_ANS_MASTERLEVEL_INFO));
    pMsgSend.iUserIndex = lpMsg->iUserIndex;
    pMsgSend.btResult = 1;

    if ( MasterLevelSystemDBSet.DSDB_QueryMasterLevelSystemLoad(lpMsg->szCharName, &pMsgSend) == TRUE )
	{
		pMsgSend.btResult = 0;
	}

    memcpy(pMsgSend.szCharName, lpMsg->szCharName, MAX_IDSTRING);
    pMsgSend.szCharName[MAX_IDSTRING] = '\0';

    wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(MLP_ANS_MASTERLEVEL_INFO));
}

void GDReqMasterLevelInfoSave(LPMLP_REQ_MASTERLEVEL_INFOSAVE lpMsg, int aIndex)
{
	if (lpMsg == NULL)
	{
		return;
	}

	MasterLevelSystemDBSet.DSDB_QueryMasterLevelSystemSave(lpMsg);
}

void GDSkillKeyDataRecv(LPSDHP_SKILLKEYDATA lpMsg)
{
	char szName[MAX_IDSTRING+1];

	memset(szName, 0, MAX_IDSTRING+1);
	memcpy(szName, lpMsg->Name, MAX_IDSTRING);

	gOptionDbSet.Save(szName, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, lpMsg->RkeyDefine, lpMsg->QWERLevel);
}

void GDSkillKeyDataSend(char *name, int userindex, int aIndex)
{
	SDHP_SKILLKEYDATA_SEND pMsg;

	BYTE KeyBuf[20];
	
	if ( gOptionDbSet.Load(name, KeyBuf, pMsg.GameOption, pMsg.QkeyDefine, pMsg.WkeyDefine, pMsg.EkeyDefine, pMsg.ChatWindow, pMsg.RkeyDefine, pMsg.QWERLevel) < 1 )
	{
		return;
	}

	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0x60;
	pMsg.h.size = sizeof(SDHP_SKILLKEYDATA_SEND);

    memcpy(pMsg.Name, name, MAX_IDSTRING);
    memcpy(pMsg.SkillKeyBuffer, KeyBuf, sizeof(KeyBuf));

    pMsg.aIndex = userindex;

	wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);
}

void GDZenSave(LPSDHP_ZENSAVE lpMsg, int aIndex)
{
	char szId[MAX_IDSTRING+1];

	SDHP_ZENLOAD pMsg;

	int Zen = lpMsg->Total;
	
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountID, MAX_IDSTRING);
	
	CZenEvenDBSet.SetZen(szId, Zen);
	
	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0x50;
	pMsg.h.size = sizeof(SDHP_ZENLOAD);
	memcpy(pMsg.AccountID, szId, MAX_IDSTRING);
	pMsg.Total = Zen;
	pMsg.Number = lpMsg->Number;

	wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);
}

void GDItemCreate(LPSDHP_ITEMCREATE lpMsg, int aIndex)
{
	SDHP_ITEMCREATERECV pMsg;

	DWORD s_num = gWzItemSerial.GetSerial();

	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0x52;
	pMsg.h.size = sizeof(SDHP_ITEMCREATERECV);
	pMsg.Dur = lpMsg->Dur;
	pMsg.Level = lpMsg->Level;
	pMsg.MapNumber = lpMsg->MapNumber;
	pMsg.Type = lpMsg->Type;
	pMsg.x = lpMsg->x;
	pMsg.y = lpMsg->y;
	pMsg.Op1 = lpMsg->Op1;
	pMsg.Op2 = lpMsg->Op2;
	pMsg.Op3 = lpMsg->Op3;
	pMsg.aIndex = lpMsg->aIndex;
	pMsg.NewOption = lpMsg->NewOption;
	pMsg.m_Number = s_num;
	pMsg.lootindex = lpMsg->lootindex;
	pMsg.SetOption = lpMsg->SetOption;
	pMsg.lDuration = lpMsg->lDuration;
	pMsg.dwEventIndex = lpMsg->dwEventIndex;
	wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);

	if( lpMsg->Type == ITEM_POTION+13 || 
		lpMsg->Type == ITEM_POTION+14 || 
		lpMsg->Type == ITEM_POTION+16 || 
		lpMsg->Type == ITEM_POTION+22 || 
		lpMsg->Type == ITEM_WING+15 || 
		lpMsg->Type == ITEM_WING+30 || 
		lpMsg->Type == ITEM_WING+31 || 
		lpMsg->Type == ITEM_POTION+31 )
	{
	//	ITEM_SERIAL_LOG.Output("[Item Serial] [%s][%d] Type(%d) Serial(%u)",
	//		gSObj[aIndex].Ip, gSObj[aIndex].Port, lpMsg->Type,s_num);
	}
}

void GDPropertyItemCreate(LPSDHP_PROPERTY_ITEMCREATE lpMsg, int aIndex)
{
	SDHP_PROPERTY_ITEMRECV pMsg;

	DWORD s_num = gWzItemSerial.GetSerial();

	pMsg.h.set((LPBYTE)&pMsg, 0x54, sizeof(SDHP_PROPERTY_ITEMRECV));
	
	if ( gGSDbSet.CreatePropertyItemInfo(s_num, lpMsg) == FALSE)
	{
		gWindow.PrintLog("error-L1 : CreatePropertyItemInfo %u ", s_num);
	}

	pMsg.Dur = lpMsg->Dur;
	pMsg.Level = lpMsg->Level;
	pMsg.MapNumber = lpMsg->MapNumber;
	pMsg.Type = lpMsg->Type;
	pMsg.x = lpMsg->x;
	pMsg.y = lpMsg->y;
	pMsg.Op1 = lpMsg->Op1;
	pMsg.Op2 = lpMsg->Op2;
	pMsg.Op3 = lpMsg->Op3;
	pMsg.aIndex = lpMsg->aIndex;
	pMsg.NewOption = lpMsg->NewOption;
	pMsg.m_Number = s_num;
	pMsg.lootindex = lpMsg->lootindex;
	pMsg.SetOption = lpMsg->SetOption;
	pMsg.dwTime = lpMsg->dwTime;
	memcpy(pMsg.szName, lpMsg->szName, MAX_IDSTRING);
	memcpy(pMsg.szName_T, lpMsg->szName_T, MAX_IDSTRING);
	pMsg.btClass = lpMsg->btClass;
	pMsg.btClass_T = lpMsg->btClass_T;
	pMsg.wLevel = lpMsg->wLevel;
	pMsg.wLevel_T = lpMsg->wLevel_T;
	pMsg.wItemIndex = lpMsg->wItemIndex;
	pMsg.wRemainTime = lpMsg->wRemainTime;
	
	wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);
}

void GDDeleteTempUserInfo(LPSDHP_DELETE_TEMPUSERINFO lpMsg)
{
	SDHP_DELETE_TEMPUSERINFO_RESULT pMsg;
	
	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0xCE;
	pMsg.h.size = sizeof(SDHP_DELETE_TEMPUSERINFO_RESULT);
	
	memcpy(pMsg.CharName, lpMsg->CharName, MAX_IDSTRING+1);
	
	int iIndex = gSObjIndexSerrchbyServerCode(lpMsg->ServerCode);
	
	if ( iIndex != -1 )
	{
		wsjServer.DataSend(iIndex, (char*)&pMsg, pMsg.h.size);
	}
}

void GDCharacterTransfer(LPSDHP_CHARACTER_TRANSFER lpMsg, int aIndex)
{
	SDHP_CHARACTER_TRANSFER_RESULT pMsg;

	char id[MAX_IDSTRING+1];

	memset(id, 0, MAX_IDSTRING);
	memcpy(id, lpMsg->Account, MAX_IDSTRING);

	pMsg.Result = 1;

	if ( strlen(id) > 1 )
	{
		pMsg.Result = gCharDbSet.CharServerMove(id);
	}

	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0xCF;
	pMsg.h.size = sizeof(SDHP_CHARACTER_TRANSFER_RESULT);

	memcpy(pMsg.Account, lpMsg->Account, MAX_IDSTRING);

	pMsg.Number = lpMsg->Number;
	
	wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);
}

void GDPetItemCreate(LPSDHP_PET_ITEMCREATE lpMsg, int aIndex)
{
	SDHP_PET_ITEMCREATERECV pMsg;

	DWORD s_num = gWzItemSerial.GetSerial();
	
	pMsg.h.set((LPBYTE)&pMsg, 0x52, sizeof(SDHP_PET_ITEMCREATERECV));
	
	if (PetDBSet.CreatePetItemInfo(s_num, 1, 0) == FALSE)
	{
		gWindow.PrintLog("error-L1 : CreatePetItemInfo %u ", s_num);
	}

	pMsg.Dur = lpMsg->Dur;
	pMsg.Level = lpMsg->Level;
	pMsg.MapNumber = lpMsg->MapNumber;
	pMsg.Type = lpMsg->Type;
	pMsg.x = lpMsg->x;
	pMsg.y = lpMsg->y;
	pMsg.Op1 = lpMsg->Op1;
	pMsg.Op2 = lpMsg->Op2;
	pMsg.Op3 = lpMsg->Op3;
	pMsg.aIndex = lpMsg->aIndex;
	pMsg.NewOption = lpMsg->NewOption;
	pMsg.m_Number = s_num;
	pMsg.lootindex = lpMsg->lootindex;
	pMsg.SetOption = lpMsg->SetOption;
	
	wsjServer.DataSend(aIndex, (char*)&pMsg, pMsg.h.size);
}

void DGGetPetItemInfo(BYTE *lpRecv, int aIndex)
{
	Recv_PetItem_Info pMsg; //10
	
	SDHP_RECV_PETITEM_INFO pHeader; //24
	
	SDHP_REQUEST_PETITEM_INFO* lpHeader; //28

	Request_PetItem_Info* lpItem; //2c
	
	BYTE lpMsgBuf[4096]; //102C
	
	int MsgOfs = 0;
	
	lpHeader = (SDHP_REQUEST_PETITEM_INFO*)lpRecv;
	
	if ( lpHeader->nCount < 1 )
	{
		return;
	}
	
	gWindow.PrintLog("펫 아이템 정보 요청 count %d", lpHeader->nCount);
	
	MsgOfs += sizeof(SDHP_REQUEST_PETITEM_INFO);
	
	lpItem = (Request_PetItem_Info*)(lpRecv+MsgOfs);

	int level = 0;
	int exp = 0;

	for (int n = 0; n < lpHeader->nCount; n++)
	{
		PetDBSet.LoadPetInfo(lpItem[n].nSerial, level, exp);
		pMsg.nPos = lpItem[n].nPos;
		pMsg.nSerial = lpItem[n].nSerial;
		pMsg.Level = level;
		pMsg.Exp = exp;
		memcpy(&lpMsgBuf[MsgOfs], &pMsg, sizeof(Recv_PetItem_Info));
		MsgOfs += sizeof(Recv_PetItem_Info);
		gWindow.PrintLog("펫 아이템 (Serial:%u) Level : %d Exp : %d", lpItem[n].nSerial, level, exp);
	}

	pHeader.h.set((LPBYTE)&pHeader, 0x56, MsgOfs);
	memcpy(pHeader.AccountID, lpHeader->AccountID, MAX_IDSTRING);
	pHeader.Number = lpHeader->Number;
	pHeader.InvenType = lpHeader->InvenType;
	pHeader.nCount = lpHeader->nCount;
	memcpy(lpMsgBuf, &pHeader, sizeof(SDHP_REQUEST_PETITEM_INFO));

	wsjServer.DataSend(aIndex, (char*)&lpMsgBuf, MsgOfs);
}

void DGSetPetItemInfo(BYTE *lpRecv, int aIndex)
{
	SDHP_SAVE_PETITEM_INFO* lpHeader = (SDHP_SAVE_PETITEM_INFO*)lpRecv;
	
	if ( lpHeader->nCount < 1 )
	{
		return;
	}
    
	gWindow.PrintLog("펫 아이템 정보 저장 요청 count %d", lpHeader->nCount);
	Save_PetItem_Info* lpMsg = (Save_PetItem_Info*)&lpRecv[5];
    
	for (int n = 0; n < lpHeader->nCount; n++)
    {
		PetDBSet.SavePetInfo(lpMsg[n].nSerial, lpMsg[n].Level, lpMsg[n].Exp);
		gWindow.PrintLog("펫 아이템 정보 저장 (Serial : %u) Level : %d   Exp : %d", lpMsg[n].nSerial, lpMsg[n].Level, lpMsg[n].Exp);
    }
}

void DS_GDReqCastleTotalInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CASTLEDATA* lpMsg = (CSP_REQ_CASTLEDATA*)lpRecv;
	CASTLE_DATA pCastleData;  
	CSP_ANS_CASTLEDATA pMsgSend;

    pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x00, sizeof(CSP_ANS_CASTLEDATA));
    
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iRES = CastleDBSet.DSDB_QueryCastleTotalInfo(lpMsg->wMapSvrNum, lpMsg->iCastleEventCycle, &pCastleData );
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CASTLEDATA));
    }
    else
    {
		pMsgSend.iResult = 1;
		pMsgSend.wStartYear = pCastleData.wStartYear;
		pMsgSend.btStartMonth = pCastleData.btStartMonth;
		pMsgSend.btStartDay = pCastleData.btStartDay;
		pMsgSend.wEndYear = pCastleData.wEndYear;
		pMsgSend.btEndMonth = pCastleData.btEndMonth;
		pMsgSend.btEndDay = pCastleData.btEndDay;
		pMsgSend.btIsSiegeGuildList = pCastleData.btIsSiegeGuildList;
		pMsgSend.btIsSiegeEnded = pCastleData.btIsSiegeEnded;
		pMsgSend.btIsCastleOccupied = pCastleData.btIsCastleOccupied;
		pMsgSend.i64CastleMoney = pCastleData.i64CastleMoney;
		pMsgSend.iTaxRateChaos = pCastleData.iTaxRateChaos;
		pMsgSend.iTaxRateStore = pCastleData.iTaxRateStore;
		pMsgSend.iTaxHuntZone = pCastleData.iTaxHuntZone;
		pMsgSend.iFirstCreate = pCastleData.iFirstCreate;
		
		memset(pMsgSend.szCastleOwnGuild, 0, MAX_GUILDNAMESTRING);
		memcpy(pMsgSend.szCastleOwnGuild, pCastleData.szCastleOwnGuild, MAX_GUILDNAMESTRING);
		
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CASTLEDATA));
	}
}

void DS_GDReqOwnerGuildMaster(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_OWNERGUILDMASTER* lpMsg = (CSP_REQ_OWNERGUILDMASTER*)lpRecv;
	CSP_ANS_OWNERGUILDMASTER pMsgSend;

    pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x01, sizeof(CSP_ANS_OWNERGUILDMASTER));

    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;

    int iRES = CastleDBSet.DSDB_QueryOwnerGuildMaster(lpMsg->wMapSvrNum, &pMsgSend);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}

    wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_OWNERGUILDMASTER));
}

void DS_GDReqCastleNpcBuy(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_NPCBUY* lpMsg = (CSP_REQ_NPCBUY*)lpRecv;
	CSP_ANS_NPCBUY pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x03, sizeof(CSP_ANS_NPCBUY));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    
	pMsgSend.iIndex = lpMsg->iIndex;
    pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
    pMsgSend.iNpcIndex = lpMsg->iNpcIndex;
    pMsgSend.iBuyCost = lpMsg->iBuyCost;
    
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryCastleNpcBuy(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
	}

    wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_NPCBUY));
}

void DS_GDReqCastleNpcRepair(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_NPCREPAIR* lpMsg = (CSP_REQ_NPCREPAIR*)lpRecv;
	CSP_ANS_NPCREPAIR pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x04, sizeof(CSP_ANS_NPCREPAIR));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;
    pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
    pMsgSend.iNpcIndex = lpMsg->iNpcIndex;
    pMsgSend.iRepairCost = lpMsg->iRepairCost;
    
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryCastleNpcRepair(lpMsg->wMapSvrNum, lpMsg, &pMsgSend, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
	}

    wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_NPCREPAIR));	
}

void DS_GDReqCastleNpcUpgrade(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_NPCUPGRADE* lpMsg = (CSP_REQ_NPCUPGRADE*)lpRecv;
	CSP_ANS_NPCUPGRADE pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x05, sizeof(CSP_ANS_NPCUPGRADE));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;
    pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
    pMsgSend.iNpcIndex = lpMsg->iNpcIndex;
    pMsgSend.iNpcUpType = lpMsg->iNpcUpType;
    pMsgSend.iNpcUpValue = lpMsg->iNpcUpValue;
    pMsgSend.iNpcUpIndex = lpMsg->iNpcUpIndex;

    int iRES = CastleDBSet.DSDB_QueryCastleNpcUpgrade(lpMsg->wMapSvrNum, lpMsg);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = 1;
	}

    wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_NPCUPGRADE));		
}

void DS_GDReqTaxInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}
    CSP_REQ_TAXINFO* lpMsg = (CSP_REQ_TAXINFO*)lpRecv;
	CSP_ANS_TAXINFO pMsgSend;
    pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x06, sizeof(CSP_ANS_TAXINFO));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;

    int iRES = CastleDBSet.DSDB_QueryTaxInfo(lpMsg->wMapSvrNum, &pMsgSend);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = 1;
	}

    wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_TAXINFO));		
}

void DS_GDReqTaxRateChange(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}
    
	CSP_REQ_TAXRATECHANGE* lpMsg = (CSP_REQ_TAXRATECHANGE*)lpRecv;
	CSP_ANS_TAXRATECHANGE pMsgSend;
    pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x07, sizeof(CSP_ANS_TAXRATECHANGE));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;
    
	int iQueryResult = 0;

    int iRES = CastleDBSet.DSDB_QueryTaxRateChange(lpMsg->wMapSvrNum, lpMsg->iTaxKind, lpMsg->iTaxRate, &pMsgSend, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
	}

    wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_TAXRATECHANGE));		
}

void DS_GDReqCastleMoneyChange(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_MONEYCHANGE* lpMsg = (CSP_REQ_MONEYCHANGE*)lpRecv;
	CSP_ANS_MONEYCHANGE pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x08, sizeof(CSP_ANS_MONEYCHANGE));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;
	pMsgSend.iMoneyChanged = lpMsg->iMoneyChanged;

    int iQueryResult = 0;
	__int64 i64MoneyResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryCastleMoneyChange(lpMsg->wMapSvrNum, lpMsg->iMoneyChanged, &i64MoneyResult, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_MONEYCHANGE));	
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
		pMsgSend.i64CastleMoney = i64MoneyResult;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_MONEYCHANGE));	
	}
}

void DS_GDReqSiegeDateChange(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}
    CSP_REQ_SDEDCHANGE* lpMsg = (CSP_REQ_SDEDCHANGE*)lpRecv;
	CSP_ANS_SDEDCHANGE pMsgSend;
    pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x09, sizeof(CSP_ANS_SDEDCHANGE));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;

    int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QuerySiegeDateChange(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_SDEDCHANGE));	
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
		pMsgSend.wStartYear = lpMsg->wStartYear;
		pMsgSend.btStartMonth = lpMsg->btStartMonth;
		pMsgSend.btStartDay = lpMsg->btStartDay;
		pMsgSend.wEndYear = lpMsg->wEndYear;
		pMsgSend.btEndMonth = lpMsg->btEndMonth;
		pMsgSend.btEndDay = lpMsg->btEndDay;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_SDEDCHANGE));	
	}	
}

void DS_GDReqGuildMarkRegInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_GUILDREGINFO* lpMsg = (CSP_REQ_GUILDREGINFO*)lpRecv;

	int iQueryResult = 0;
	
	CSP_ANS_GUILDREGINFO pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0xA, sizeof(CSP_ANS_GUILDREGINFO));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;
	
	char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, MAX_GUILDNAMESTRING);
    
	int iRES = CastleDBSet.DSDB_QueryGuildMarkRegInfo(lpMsg->wMapSvrNum, szGuildName, &pMsgSend, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;	
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_GUILDREGINFO));	
}

void DS_GDReqSiegeEndedChange(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_SIEGEENDCHANGE* lpMsg = (CSP_REQ_SIEGEENDCHANGE*)lpRecv;
	CSP_ANS_SIEGEENDCHANGE pMsgSend;

    pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x0B, sizeof(CSP_ANS_SIEGEENDCHANGE));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QuerySiegeEndedChange(lpMsg->wMapSvrNum, lpMsg->bIsSiegeEnded, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;	
	}
	
	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_SIEGEENDCHANGE));	
}

void DS_GDReqCastleOwnerChange(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_CASTLEOWNERCHANGE* lpMsg = (CSP_REQ_CASTLEOWNERCHANGE*)lpRecv;
	CSP_ANS_CASTLEOWNERCHANGE pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x0C, sizeof(CSP_ANS_CASTLEOWNERCHANGE));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryCastleOwnerChange(lpMsg->wMapSvrNum, lpMsg, &pMsgSend, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;	
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CASTLEOWNERCHANGE));	
}

void DS_GDReqRegAttackGuild(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_REGATTACKGUILD* lpMsg = (CSP_REQ_REGATTACKGUILD*)lpRecv;
	CSP_ANS_REGATTACKGUILD pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x0D, sizeof(CSP_ANS_REGATTACKGUILD));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;
    
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryRegAttackGuild(lpMsg->wMapSvrNum, lpMsg, &pMsgSend, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;	
	}
	
	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_REGATTACKGUILD));		
}

void DS_GDReqRestartCastleState(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_CASTLESIEGEEND* lpMsg = (CSP_REQ_CASTLESIEGEEND*)lpRecv;
	CSP_ANS_CASTLESIEGEEND pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x0E, sizeof(CSP_ANS_CASTLESIEGEEND));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    
	int iQueryResult = 0;

    int iRES = CastleDBSet.DSDB_QueryRestartCastleState(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;	
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CASTLESIEGEEND));	
}

void DS_GDReqMapSvrMsgMultiCast(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_MAPSVRMULTICAST* lpMsg = (CSP_REQ_MAPSVRMULTICAST*)lpRecv;
	CSP_ANS_MAPSVRMULTICAST pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x0F, sizeof(CSP_ANS_MAPSVRMULTICAST));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    
	memcpy(pMsgSend.szMsgText, lpMsg->szMsgText, sizeof(lpMsg->szMsgText));
    
	for ( int iIDX = 0; iIDX < MAX_SERVEROBJECT; iIDX++)
    {
		if ( gSObj[iIDX].Connected != 0)
		{
			if ( gSObj[iIDX].iMapSvrGroup == lpMsg->wMapSvrNum )
			{
				wsjServer.DataSend(iIDX, (char*)&pMsgSend, sizeof(CSP_ANS_MAPSVRMULTICAST));
			}
		}
    }
}

void DS_GDReqRegGuildMark(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL )
	{
		return;
	}

    CSP_REQ_GUILDREGMARK* lpMsg = (CSP_REQ_GUILDREGMARK*)lpRecv;
	CSP_ANS_GUILDREGMARK pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x10, sizeof(CSP_ANS_GUILDREGMARK));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iIndex = lpMsg->iIndex;
	pMsgSend.iItemPos = lpMsg->iItemPos;
	
	char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, MAX_GUILDNAMESTRING);
	
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryGuildMarkRegMark(lpMsg->wMapSvrNum, szGuildName, &pMsgSend, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;	
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_GUILDREGMARK));	
}

void DS_GDReqGuildMarkReset(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_GUILDRESETMARK* lpMsg = (CSP_REQ_GUILDRESETMARK*)lpRecv;
	CSP_ANS_GUILDRESETMARK pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x11, sizeof(CSP_ANS_GUILDRESETMARK));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, MAX_GUILDNAMESTRING);

    int iRES = CastleDBSet.DSDB_QueryGuildMarkReset(lpMsg->wMapSvrNum, szGuildName, &pMsgSend);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_GUILDRESETMARK));		
}

void DS_GDReqGuildSetGiveUp(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}
    
	CSP_REQ_GUILDSETGIVEUP* lpMsg = (CSP_REQ_GUILDSETGIVEUP*)lpRecv;
	CSP_ANS_GUILDSETGIVEUP pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x12, sizeof(CSP_ANS_GUILDSETGIVEUP));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;
	
	char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, MAX_GUILDNAMESTRING);
    
	int iRES = CastleDBSet.DSDB_QueryGuildSetGiveUp(lpMsg->wMapSvrNum, szGuildName, lpMsg->bIsGiveUp, &pMsgSend);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_GUILDSETGIVEUP));	
}

void DS_GDReqCastleNpcRemove(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_NPCREMOVE* lpMsg = (CSP_REQ_NPCREMOVE*)lpRecv;
	CSP_ANS_NPCREMOVE pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x16, sizeof(CSP_ANS_NPCREMOVE));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
	pMsgSend.iNpcIndex = lpMsg->iNpcIndex;
	
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryCastleNpcRemove(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
    else
	{
		pMsgSend.iResult = iQueryResult;	
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_NPCREMOVE));		
}

void DS_GDReqCastleStateSync(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_CASTLESTATESYNC* lpMsg = (CSP_REQ_CASTLESTATESYNC*)lpRecv;
	CSP_ANS_CASTLESTATESYNC pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x17, sizeof(CSP_ANS_CASTLESTATESYNC));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    pMsgSend.iCastleState = lpMsg->iCastleState;
    pMsgSend.iTaxRateChaos = lpMsg->iTaxRateChaos;
    pMsgSend.iTaxRateStore = lpMsg->iTaxRateStore;
    pMsgSend.iTaxHuntZone = lpMsg->iTaxHuntZone;
    memcpy(pMsgSend.szOwnerGuildName, lpMsg->szOwnerGuildName, MAX_GUILDNAMESTRING);
    
	for ( int iIDX = 0; iIDX < MAX_SERVEROBJECT; iIDX++)
    {
		if ( gSObj[iIDX].Connected != 0)
		{
			if ( gSObj[iIDX].iMapSvrGroup == lpMsg->wMapSvrNum )
			{
				wsjServer.DataSend(iIDX, (char*)&pMsgSend, sizeof(CSP_ANS_CASTLESTATESYNC));
			}
		}
    }
}

void DS_GDReqCastleTributeMoney(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_CASTLETRIBUTEMONEY* lpMsg = (CSP_REQ_CASTLETRIBUTEMONEY*)lpRecv;
	CSP_ANS_CASTLETRIBUTEMONEY pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x18, sizeof(CSP_ANS_CASTLETRIBUTEMONEY));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
    
	int iQueryResult = 0;
	__int64 i64MoneyResult = 0;
	
	if (lpMsg->iCastleTributeMoney < 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		BOOL bResult = CastleDBSet.DSDB_QueryCastleMoneyChange(lpMsg->wMapSvrNum, lpMsg->iCastleTributeMoney, &i64MoneyResult, &iQueryResult);

		if(bResult != 0)
		{
			pMsgSend.iResult = 0;
			wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CASTLESTATESYNC));//webzen's mistake
			return;
		}

		pMsgSend.iResult = iQueryResult;
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CASTLETRIBUTEMONEY));
}

void DS_GDReqResetCastleTaxInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_RESETCASTLETAXINFO* lpMsg = (CSP_REQ_RESETCASTLETAXINFO*)lpRecv;
	CSP_ANS_RESETCASTLETAXINFO pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x19, sizeof(CSP_ANS_RESETCASTLETAXINFO));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryResetCastleTaxInfo(lpMsg->wMapSvrNum, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_RESETCASTLETAXINFO));
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_RESETCASTLETAXINFO));
	}
}

void DS_GDReqResetSiegeGuildInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_RESETSIEGEGUILDINFO* lpMsg = (CSP_REQ_RESETSIEGEGUILDINFO*)lpRecv;
	CSP_ANS_RESETSIEGEGUILDINFO pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 0x1A, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryResetSiegeGuildInfo(lpMsg->wMapSvrNum, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
}

void DS_GDReqResetRegSiegeInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

    CSP_REQ_RESETSIEGEGUILDINFO* lpMsg = (CSP_REQ_RESETSIEGEGUILDINFO*)lpRecv;
	CSP_ANS_RESETSIEGEGUILDINFO pMsgSend;
    
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x80, 27, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
    pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iQueryResult = 0;
    
	int iRES = CastleDBSet.DSDB_QueryResetRegSiegeInfo(lpMsg->wMapSvrNum, &iQueryResult);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
    else
	{
		pMsgSend.iResult = iQueryResult;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
}

void DS_GDReqCastleInitData(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CSINITDATA* lpMsg = (CSP_REQ_CSINITDATA*)lpRecv;
	CASTLE_DATA pCastleData;
	
	char cBUFFER[5920];
	
	CSP_ANS_CSINITDATA* lpMsgSend = (CSP_ANS_CSINITDATA*)cBUFFER;
	CSP_NPCDATA* lpMsgSendBody = (CSP_NPCDATA *)&cBUFFER[64];
	
	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iDataCount = 200;
	
	lpMsgSend->iCount = 0;
	
	int iRES = CastleDBSet.DSDB_QueryCastleTotalInfo(lpMsg->wMapSvrNum, lpMsg->iCastleEventCycle, &pCastleData);
    
	if ( iRES != 0)
    {
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x81, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
    }
    else
    {
		lpMsgSend->iResult = 0;
		lpMsgSend->wStartYear = pCastleData.wStartYear;
		lpMsgSend->btStartMonth = pCastleData.btStartMonth;
		lpMsgSend->btStartDay = pCastleData.btStartDay;
		lpMsgSend->wEndYear = pCastleData.wEndYear;
		lpMsgSend->btEndMonth = pCastleData.btEndMonth;
		lpMsgSend->btEndDay = pCastleData.btEndDay;
		lpMsgSend->btIsSiegeGuildList = pCastleData.btIsSiegeGuildList;
		lpMsgSend->btIsSiegeEnded = pCastleData.btIsSiegeEnded;
		lpMsgSend->btIsCastleOccupied = pCastleData.btIsCastleOccupied;
		lpMsgSend->i64CastleMoney = pCastleData.i64CastleMoney;
		lpMsgSend->iTaxRateChaos = pCastleData.iTaxRateChaos;
		lpMsgSend->iTaxRateStore = pCastleData.iTaxRateStore;
		lpMsgSend->iTaxHuntZone = pCastleData.iTaxHuntZone;
		lpMsgSend->iFirstCreate = pCastleData.iFirstCreate;
		
		memset(lpMsgSend->szCastleOwnGuild, 0, MAX_GUILDNAMESTRING);
		memcpy(lpMsgSend->szCastleOwnGuild, pCastleData.szCastleOwnGuild, MAX_GUILDNAMESTRING);
		
		iRES = CastleDBSet.DSDB_QueryCastleNpcInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);
		
		if ( iRES != 0)
		{
			lpMsgSend->iResult = 0;
			lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x81, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
			wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
		}
		else
		{
			lpMsgSend->iResult = 1;
			lpMsgSend->iCount = iDataCount;
			lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x81, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
			wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
		}
	}
}

void DS_GDReqCastleNpcInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCDATA* lpMsg = (CSP_REQ_NPCDATA*)lpRecv;
	
	char cBUFFER[5876];
	
	CSP_ANS_NPCDATA* lpMsgSend = (CSP_ANS_NPCDATA*)cBUFFER;
	CSP_NPCDATA* lpMsgSendBody = (CSP_NPCDATA *)&cBUFFER[20];
	
	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;	
	
	int iDataCount = 200;
	
	lpMsgSend->iCount = 0;
	
	int iRES = CastleDBSet.DSDB_QueryCastleNpcInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);
	
	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x82, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x82, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
	}
}

void DS_GDReqAllGuildMarkRegInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_ALLGUILDREGINFO* lpMsg = (CSP_REQ_ALLGUILDREGINFO*)lpRecv;
	
	char cBUFFER[1876];
	
	CSP_ANS_ALLGUILDREGINFO* lpMsgSend = (CSP_ANS_ALLGUILDREGINFO*)cBUFFER;
	CSP_GUILDREGINFO* lpMsgSendBody = (CSP_GUILDREGINFO *)&cBUFFER[20];
	
	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;	
	lpMsgSend->iIndex = lpMsg->iIndex;
	
	int iDataCount = 100;
	
	lpMsgSend->iCount = 0;
	
	int iRES = CastleDBSet.DSDB_QueryAllGuildMarkRegInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);
	
	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x83, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x83, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
	}	
}

void DS_GDReqFirstCreateNPC(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCSAVEDATA* lpMsg = (CSP_REQ_NPCSAVEDATA*)lpRecv;
	CSP_NPCSAVEDATA* lpMsgBody = (CSP_NPCSAVEDATA*)&lpRecv[12];
	
	CSP_ANS_NPCSAVEDATA pMsgSend;	
	pMsgSend.h.set((LPBYTE)&pMsgSend, 0x84, sizeof(CSP_ANS_NPCSAVEDATA));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iRES = CastleDBSet.DSDB_QueryFirstCreateNPC(lpMsg->wMapSvrNum,lpMsg);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_NPCSAVEDATA));
	}
	else
	{
		pMsgSend.iResult = 1;
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_NPCSAVEDATA));		
	}
}

void DS_GDReqCalcRegGuildList(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CALCREGGUILDLIST* lpMsg = (CSP_REQ_CALCREGGUILDLIST*)lpRecv;
	
	char cBUFFER[2672];
	
	CSP_ANS_CALCREGGUILDLIST* lpMsgSend = (CSP_ANS_CALCREGGUILDLIST*)cBUFFER;
	CSP_CALCREGGUILDLIST* lpMsgSendBody = (CSP_CALCREGGUILDLIST *)&cBUFFER[16];
	
	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;	
	
	int iDataCount = 100;
	
	lpMsgSend->iCount = 0;
	
	int iRES = CastleDBSet.DSDB_QueryCalcRegGuildList(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);
	
	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x85, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x85, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
	}		
}

void DS_GDReqCsGuildUnionInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CSGUILDUNIONINFO* lpMsg = (CSP_REQ_CSGUILDUNIONINFO*)lpRecv;
	CSP_CSGUILDUNIONINFO* lpMsgBody = (CSP_CSGUILDUNIONINFO*)&lpRecv[12];
	
	char cBUFFER[1472];
	
	CSP_ANS_CSGUILDUNIONINFO* lpMsgSend = (CSP_ANS_CSGUILDUNIONINFO*)cBUFFER;
	CSP_CSGUILDUNIONINFO* lpMsgSendBody = (CSP_CSGUILDUNIONINFO *)&cBUFFER[16];
	
	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;	

	if (lpMsg->iCount < 0)
	{
		lpMsg->iCount = 0;
	}

	int iRET_COUNT = 0;
	
	for (int iGCNT=0; iGCNT < lpMsg->iCount; iGCNT++)
	{
		char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
		memcpy(szGuildName, lpMsgBody[iGCNT].szGuildName, MAX_GUILDNAMESTRING);
		
		int iRES = CastleDBSet.DSDB_QueryCsGuildUnionInfo(lpMsg->wMapSvrNum, szGuildName, lpMsgBody[iGCNT].iCsGuildID, lpMsgSendBody, &iRET_COUNT);
		
		if (iRES != 0)
		{
			lpMsgSend->iResult = 0;
			lpMsgSend->iCount = 0;
			lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x86, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
			wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
			return;
		}
	}

	lpMsgSend->iResult = 1;
	lpMsgSend->iCount = iRET_COUNT;	
	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x86, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
	wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
}

void DS_GDReqCsSaveTotalGuildInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CSSAVETOTALGUILDINFO* lpMsg = (CSP_REQ_CSSAVETOTALGUILDINFO*)lpRecv;
	CSP_CSSAVETOTALGUILDINFO* lpMsgBody = (CSP_CSSAVETOTALGUILDINFO*)&lpRecv[12];
	
	CSP_ANS_CSSAVETOTALGUILDINFO pMsgSend;
	
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iRES = CastleDBSet.DSDB_QueryCsClearTotalGuildInfo(lpMsg->wMapSvrNum);
    
	if ( iRES != 0)
	{
		pMsgSend.iResult = 0;
		pMsgSend.h.set((LPBYTE)&pMsgSend, 0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
		wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
	}
	else
	{
		for (int iGCNT = 0; iGCNT < lpMsg->iCount; iGCNT++)
		{
			char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
			memcpy(szGuildName, lpMsgBody[iGCNT].szGuildName, MAX_GUILDNAMESTRING);

			int iRES = CastleDBSet.DSDB_QueryCsSaveTotalGuildInfo(lpMsg->wMapSvrNum, szGuildName, lpMsgBody[iGCNT].iCsGuildID,lpMsgBody[iGCNT].iGuildInvolved,lpMsgBody[iGCNT].iGuildScore);
			
			if (iRES != 0)
			{
				pMsgSend.iResult = 0;
				pMsgSend.h.set((LPBYTE)&pMsgSend, 0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
				wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
				return;
			}
		}

		int iQueryResult = 0;
		
		iRES = CastleDBSet.DSDB_QueryCsSaveTotalGuildOK(lpMsg->wMapSvrNum, &iQueryResult);
		
		if ( iRES != 0)
		{
			pMsgSend.iResult = 0;
			pMsgSend.h.set((LPBYTE)&pMsgSend, 0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
			wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
		}
		else
		{
			pMsgSend.iResult = 1;
			pMsgSend.iResult = 1; //??
			pMsgSend.h.set((LPBYTE)&pMsgSend, 0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
			wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
		}
	}
}

void DS_GDReqCsLoadTotalGuildInfo(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}
	
	CSP_REQ_CSLOADTOTALGUILDINFO* lpMsg = (CSP_REQ_CSLOADTOTALGUILDINFO*)lpRecv;
	
	char cBUFFER[2272];
	
	CSP_ANS_CSLOADTOTALGUILDINFO* lpMsgSend = (CSP_ANS_CSLOADTOTALGUILDINFO*)cBUFFER;
	CSP_CSLOADTOTALGUILDINFO* lpMsgSendBody = (CSP_CSLOADTOTALGUILDINFO *)&cBUFFER[16];
	
	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;

	int iDataCount = 100;
	
	lpMsgSend->iCount = 0;
	
	int iRES = CastleDBSet.DSDB_QueryCsLoadTotalGuildInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);
	
	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x88, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x88, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
		wsjServer.DataSend(aIndex, (char*)lpMsgSend, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
	}	
}

void DS_GDReqCastleNpcUpdate(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCUPDATEDATA* lpMsg = (CSP_REQ_NPCUPDATEDATA*)lpRecv;
	CSP_NPCUPDATEDATA* lpMsgBody = (CSP_NPCUPDATEDATA*)&lpRecv[12];
	
	CSP_ANS_NPCSAVEDATA pMsgSend;
	pMsgSend.h.set((LPBYTE)&pMsgSend,0x89,sizeof(CSP_ANS_NPCSAVEDATA));

	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	
	int iRES = CastleDBSet.DSDB_QueryCastleNpcUpdate(lpMsg->wMapSvrNum,lpMsg);
	
	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = 1;
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CSP_ANS_NPCSAVEDATA));
}

void GDReqCrywolfSync(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CWP_REQ_CRYWOLFSYNC* lpMsg = (CWP_REQ_CRYWOLFSYNC*)lpRecv;
	CWP_ANS_CRYWOLFSYNC pMsgSend;
	
	pMsgSend.h.set((LPBYTE)&pMsgSend,0xB0,sizeof(CWP_ANS_CRYWOLFSYNC));
    pMsgSend.iCrywolfState = lpMsg->iCrywolfState;
    pMsgSend.iOccupationState = lpMsg->iOccupationState;
    pMsgSend.btResult = 1;
    
	for (int iIDX = 0; iIDX < MAX_SERVEROBJECT; iIDX++)
    {
		if ( gSObj[iIDX].Connected != 0)
		{
			if ( gSObj[iIDX].iMapSvrGroup == lpMsg->wMapSvrNum )
			{
				wsjServer.DataSend(iIDX, (char*)&pMsgSend, sizeof(CWP_ANS_CRYWOLFSYNC));
			}
		}
    }
}

void GDReqCrywolfInfoLoad(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}

	CWP_REQ_CRYWOLFINFOLOAD* lpMsg = (CWP_REQ_CRYWOLFINFOLOAD*)lpRecv;
	CWP_ANS_CRYWOLFINFOLOAD pMsgSend;
	
	pMsgSend.h.set((LPBYTE)&pMsgSend,0xB1,sizeof(CWP_ANS_CRYWOLFINFOLOAD));
	pMsgSend.btResult = 0;
	
	int iResult = CrywolfDBSet.DSDB_QueryCrywolfInfoLoad(lpMsg->wMapSvrNum, &pMsgSend);
	
	if (iResult == 0)
	{
		pMsgSend.btResult = 1;
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CWP_ANS_CRYWOLFINFOLOAD));
}

void GDReqCrywolfInfoSave(BYTE *lpRecv, int aIndex)
{
	if ( lpRecv == NULL)
	{
		return;
	}
	
	CWP_REQ_CRYWOLFINFOSAVE* lpMsg = (CWP_REQ_CRYWOLFINFOSAVE*)lpRecv;
	int iResult = CrywolfDBSet.DSDB_QueryCrywolfInfoSave(lpMsg->wMapSvrNum, lpMsg);


	CWP_ANS_CRYWOLFINFOSAVE pMsgSend;
	pMsgSend.h.set((LPBYTE)&pMsgSend,0xB2,sizeof(CWP_ANS_CRYWOLFINFOSAVE));
	pMsgSend.btResult = 0;
	
	if (iResult == 0)
	{
		pMsgSend.btResult = 1;
	}

	wsjServer.DataSend(aIndex, (char*)&pMsgSend, sizeof(CWP_ANS_CRYWOLFINFOSAVE));
}

void GDReqPeriodItemExInsert(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_PERIODITEMEX_INSERT* lpMsg = (PMSG_REQ_PERIODITEMEX_INSERT*)lpRecv;

	PMSG_ANS_PERIODITEMEX_INSERT pMsg;
	pMsg.head.set((LPBYTE)&pMsg, 0xD1, 0x01, sizeof(PMSG_ANS_PERIODITEMEX_INSERT));
	pMsg.btResultCode = PeriodItemExDBSet.Insert(lpMsg->dwUserGuid, lpMsg->chCharacterName, lpMsg->btItemType, lpMsg->wItemCode, lpMsg->btOptionType, lpMsg->btEffectType1, lpMsg->btEffectType2, lpMsg->dwSerial, lpMsg->dwDuration, lpMsg->lBuyDate, lpMsg->lExpireDate);
	pMsg.dwUserGuid = lpMsg->dwUserGuid;
	pMsg.wUserIndex = lpMsg->wUserIndex;
	pMsg.btItemtype = lpMsg->btItemType;
	pMsg.wItemCode = lpMsg->wItemCode;
	pMsg.btOptionType = lpMsg->btOptionType;
	pMsg.btEffectType1 = lpMsg->btEffectType1;
	pMsg.btEffectType2 = lpMsg->btEffectType2;
	pMsg.dwSerial = lpMsg->dwSerial;
	pMsg.dwDuration = lpMsg->dwDuration;
	pMsg.lBuyDate = lpMsg->lBuyDate;
	pMsg.lExpireDate = lpMsg->lExpireDate;

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(PMSG_ANS_PERIODITEMEX_INSERT));
}

void GDReqPeriodItemExDelete(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_PERIODITEMEX_DELETE* lpMsg = (PMSG_REQ_PERIODITEMEX_DELETE*)lpRecv;

	PMSG_ANS_PERIODITEMEX_DELETE pMsg;
	pMsg.head.set((LPBYTE)&pMsg, 0xD1, 0x02, sizeof(PMSG_ANS_PERIODITEMEX_DELETE));

	pMsg.btResultCode = PeriodItemExDBSet.Delete(lpMsg->dwUserGuid, lpMsg->chCharacterName, lpMsg->wItemCode, lpMsg->btItemType, lpMsg->dwSerial);
	pMsg.dwUserGuid = lpMsg->dwUserGuid;
	pMsg.wUserIndex = lpMsg->wUserIndex;

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(PMSG_ANS_PERIODITEMEX_DELETE));
}

void GDReqPeriodItemExSelect(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_PERIODITEMEX_SELECT* lpMsg = (PMSG_REQ_PERIODITEMEX_SELECT*)lpRecv;

	PMSG_ANS_PERIODITEMEX_LIST_COUNT pListCount;
	PMSG_ANS_PERIODITEMEX_LIST pList;

	//PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST pExpiredList = {0};
	//PERIOD_EXPIRED_ITEMLIST ExpiredItemList;

	pList.head.set((LPBYTE)&pList, 0xD1, 0x05, sizeof(PMSG_ANS_PERIODITEMEX_LIST));
	pListCount.head.set((LPBYTE)&pListCount, 0xD1, 0x04, sizeof(PMSG_ANS_PERIODITEMEX_LIST_COUNT));
	//pExpiredList.head.set((LPBYTE)&pExpiredList, 0xD1, 0x03, sizeof(PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST));

	/*int iReturnCode = 0;
	iReturnCode = PeriodItemExDBSet.SelectExpiredItemList(lpMsg->dwUserGuid, lpMsg->chCharacterName, &ExpiredItemList);
	
	if(iReturnCode < 0)
	{
		//do nothing :)
	}
	else
	{
		pExpiredList.wUserIndex = lpMsg->wUserIndex;
		pExpiredList.btExpiredItemCount = iReturnCode;
		
		for (int i = 0; i < pExpiredList.btExpiredItemCount; i++ )
		{
			pExpiredList.wItemCode[i] = ExpiredItemList.ExpiredItem[i].wItemCode;
			pExpiredList.dwSerial[i] = ExpiredItemList.ExpiredItem[i].dwSerial;
		}

		wsjServer.DataSend(aIndex, (char*)&pExpiredList, pExpiredList.head.size);
	}*/

	PERIOD_ITEMLIST ItemData;
	ItemData.dwUserGuid = lpMsg->dwUserGuid;
	memcpy(ItemData.chCharacterName, lpMsg->chCharacterName, MAX_IDSTRING+1);

	pListCount.btResult = PeriodItemExDBSet.Select(&ItemData);
	pListCount.btListCount = ItemData.btItemCount;
	pListCount.dwUserGuid = lpMsg->dwUserGuid;
	pListCount.wUserIndex = lpMsg->wUserIndex;

	for(int iItemCount = 0; iItemCount < pListCount.btListCount; iItemCount++)
	{
		pList.dwUserGuid = lpMsg->dwUserGuid;
		pList.wUserIndex = lpMsg->wUserIndex;
		pList.wItemCode = ItemData.ItemData[iItemCount].wItemCode;
		pList.btEffectCategory = ItemData.ItemData[iItemCount].btEffectCategory;
		pList.btEffectType1 = ItemData.ItemData[iItemCount].btEffectType1;
		pList.btEffectType2 = ItemData.ItemData[iItemCount].btEffectType2;
		pList.btItemType = ItemData.ItemData[iItemCount].btItemType;
		pList.btUsedInfo = ItemData.ItemData[iItemCount].btUsedInfo;
		pList.dwSerial = ItemData.ItemData[iItemCount].dwSerial;
		pList.lItemBuyDate = ItemData.ItemData[iItemCount].lItemBuyDate;
		pList.lItemExpireDate = ItemData.ItemData[iItemCount].lItemExpireDate;

		gWindow.PrintLog("[%d] [%d]", pList.wItemCode, pList.dwSerial);

		wsjServer.DataSend(aIndex, (char*)&pList, pList.head.size);
	}
	wsjServer.DataSend(aIndex, (char*)&pListCount, pListCount.head.size);
}

void GDReqLuckyItemInsert(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_LUCKYITEM_INSERT* lpMsg = (PMSG_REQ_LUCKYITEM_INSERT*)lpRecv;

	g_LuckyItemDBSet.Insert(lpMsg->dwUserGuid, lpMsg->szCharName, lpMsg->LuckyItemDBInfo.wItemCode, lpMsg->LuckyItemDBInfo.dwSerial, lpMsg->LuckyItemDBInfo.wDurabilitySmall);
}

void GDReqLuckyItemInsert2nd(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_LUCKYITEM_INSERT_2ND* lpMsg = (PMSG_REQ_LUCKYITEM_INSERT_2ND*)lpRecv;

	int iItemCnt = lpMsg->btItemCnt;

	if(iItemCnt <= 0)
	{
		return;
	}

	for(int i = 0; i < iItemCnt; i++)
	{
		g_LuckyItemDBSet.Insert(lpMsg->dwUserGuid, lpMsg->szCharName, lpMsg->LuckyItemDBInfo[i].wItemCode, lpMsg->LuckyItemDBInfo[i].dwSerial, lpMsg->LuckyItemDBInfo[i].wDurabilitySmall);
	}
}

void GDReqLuckyItemDelete(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_LUCKYITEM_DELETE* lpMsg = (PMSG_REQ_LUCKYITEM_DELETE*)lpRecv;

	g_LuckyItemDBSet.Delete(lpMsg->szCharName, lpMsg->wItemCode, lpMsg->dwSerial);
}

void GDReqLuckyItemSelect(BYTE *lpRecv, int aIndex)
{
	char sendbuf[2048] = {0};
	int lOfs = 0;

	PMSG_REQ_LUCKYITEM_SELECT* lpMsg = (PMSG_REQ_LUCKYITEM_SELECT*)lpRecv;

	PMSG_LUCKYITME_DB_INFO LuckyItemInfo[45] = {0};
	
	int iResult = g_LuckyItemDBSet.SelectLuckyItemList(lpMsg->chCharacterName, LuckyItemInfo);

	if(iResult <= 0)
	{
		return;
	}

	lOfs = 8;

	PMSG_ANS_LUCKYITEM_SELECT pMsg;
	pMsg.btItemCnt = iResult;
    pMsg.wUserIndex = lpMsg->wUserIndex;

    memcpy(&sendbuf[lOfs], LuckyItemInfo, pMsg.btItemCnt*12);
    
	lOfs += (pMsg.btItemCnt*12);

	pMsg.head.set((LPBYTE)&pMsg, 0xD4, lOfs);
    memcpy(sendbuf, &pMsg, 8);
	wsjServer.DataSend(aIndex, (char*)&sendbuf, lOfs);
}

void GDReqUserID(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_USERID* lpMsg = (PMSG_REQ_USERID*)lpRecv;
	PMSG_ANS_USERID pMsg;

	char chBuffer[MAX_IDSTRING+1];
	memset(chBuffer, 0, MAX_IDSTRING+1);
	memcpy(chBuffer, lpMsg->chUserName, MAX_IDSTRING);
	chBuffer[MAX_IDSTRING] = '\0';

	pMsg.head.set((LPBYTE)&pMsg, 0xD3, 0x01, sizeof(PMSG_ANS_USERID));
	pMsg.dwKey = lpMsg->dwKey;
	pMsg.dwUserGUID = lpMsg->dwUserGUID;
	pMsg.btResult = gCharDbSet.GetCharacterID(lpMsg->chUserName, pMsg.chUserID);

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(PMSG_ANS_USERID));
}

void DGChangeName(LPSDHP_CHANGE_NAME lpMsg, int aIndex)
{
	int Result = 0;

	SDHP_CHANGE_NAME_RESULT pResult;
	
	char accountId[MAX_IDSTRING+1];
	char OldName[MAX_IDSTRING+1];
	char NewName[MAX_IDSTRING+1];

	accountId[MAX_IDSTRING] = '\0';
	memcpy(accountId, lpMsg->AccountId, MAX_IDSTRING);

	OldName[MAX_IDSTRING] = '\0';
	NewName[MAX_IDSTRING] = '\0';
	memcpy(OldName, lpMsg->OldName, MAX_IDSTRING);
	memcpy(NewName, lpMsg->NewName, MAX_IDSTRING);

	if ( SQLSyntexCheck(NewName) == FALSE)
	{
		Result = 1;
	}
	
	if ( cBadStrChk.CmpString(NewName) == 1 )
	{
		Result = 1;
	}
	
	if ( gMuName.CmpString(NewName) == 1 )
	{
		Result = 1;
	}
	
	if ( Result == 0)
	{
		Result = gCharDbSet.ChangeName(accountId, OldName, NewName);
	}
	
	pResult.h.set((LPBYTE)&pResult, 0x16, sizeof(SDHP_CHANGE_NAME_RESULT));
	pResult.btResult = Result;
	pResult.aIndex = lpMsg->aIndex;

	memcpy(pResult.AccountId, lpMsg->AccountId, MAX_IDSTRING);
	memcpy(pResult.OldName, lpMsg->OldName, MAX_IDSTRING);
	memcpy(pResult.NewName, lpMsg->NewName, MAX_IDSTRING);

	wsjServer.DataSend(aIndex, (char*)&pResult, sizeof(PMSG_ANS_USERID));
}

void GJPSetAccountInfo(LPSDHP_REQ_SETACCOUNTINFO lpMsg, int aIndex)
{
	SDHP_ANS_SETACCOUNTINFO Result;
	
	if (lpMsg->isSummoner == 1)
	{
		Result.ResultSummoner = gACDbSet.SetSummonerCardInfo(lpMsg->AccountId);
	}
	
	if (lpMsg->isRageFighter == 1)
	{
		Result.ResultRageFighter = gACDbSet.SetRageFighterCardInfo(lpMsg->AccountId);
	}

	

	memset(Result.AccountId, 0, MAX_IDSTRING+1);
	memcpy(Result.AccountId, lpMsg->AccountId, MAX_IDSTRING);
	
	Result.h.set((LPBYTE)&Result, 0xA2, sizeof(SDHP_ANS_SETACCOUNTINFO));
	Result.Number = lpMsg->Number;

	wsjServer.DataSend(aIndex, (char*)&Result, sizeof(SDHP_ANS_SETACCOUNTINFO));
}

void DGBroadCastGlobalMessage(LPBYTE lpData)
{
	LPMSG_SYS_BROADCAST	lpMsg	= (LPMSG_SYS_BROADCAST)lpData;

	gWindow.PrintLog("(BroadCasting) %s", lpMsg->szMessage);

	MSG_SYS_BROADCAST	res;
	res.h.set((LPBYTE)&res, 0xCD, sizeof(res));

	res.btSize = lpMsg->btSize;
	CopyMemory( res.szMessage, lpMsg->szMessage, MAX_CHAT);

	for (int iIDX = 0; iIDX < MAX_SERVEROBJECT; iIDX++)
	{
		if ( gSObj[iIDX].Connected != 0)
		{
			//if ( gSObj[iIDX].iMapSvrGroup == 0 )
			{
				wsjServer.DataSend(iIDX, (char*)&res, sizeof(MSG_SYS_BROADCAST));
			}
		}
	}
}
// -----------------------------------------------------------------

void GensGetMemberInfo(PMSG_GENS_USERINFO_REQ * aRecv, int aIndex)
{
	PMSG_GENS_USERINFO_ANS Result;
	// ----
	Result.h.set((LPBYTE)&Result, 0xF8, 0x01, sizeof(PMSG_GENS_USERINFO_ANS));
	// ----
	Result.IndexH	= aRecv->IndexH;
	Result.IndexL	= aRecv->IndexL;
	// ----
	if( !gGensDBSet.GetMemberInfo(aRecv->Name, &Result) )
	{
		gWindow.PrintLog("[%s] Failed to GetMemberInfo", __FUNCTION__);
		return;
	}
	// ----
	wsjServer.DataSend(aIndex, (char*)&Result, sizeof(PMSG_GENS_USERINFO_ANS));
}
// -----------------------------------------------------------------

void GensSaveMemberInfo(PMSG_GENS_SAVEINFO * aRecv, int aIndex)
{
	PMSG_GENS_USERINFO_ANS Result;
	// ----
	Result.h.set((LPBYTE)&Result, 0xF8, 0x01, sizeof(PMSG_GENS_USERINFO_ANS));
	// ----
	Result.IndexH = SET_NUMBERH(aRecv->Index);
	Result.IndexL = SET_NUMBERL(aRecv->Index);
	// ----
	if( !gGensDBSet.SaveMemberInfo(aRecv->Name, aRecv->Influence, aRecv->ContributePoint, aRecv->GensClass) )
	{
		gWindow.PrintLog("[%s] Failed to SaveMemberInfo", __FUNCTION__);
		return;
	}
	// ----
	Result.Influence		= aRecv->Influence;
	Result.ContributePoint	= aRecv->ContributePoint;
	Result.GensClass		= aRecv->GensClass;
	//-> Missing Gens Ranking...
	// ----
	wsjServer.DataSend(aIndex, (char*)&Result, sizeof(PMSG_GENS_USERINFO_ANS));
}
// -----------------------------------------------------------------

void GensRegMemberInfo(PMSG_GENS_REGUSER_REQ * aRecv, int aIndex)
{
	PMSG_GENS_REGUSER_ANS Result;
	// ----
	Result.h.set((LPBYTE)&Result, 0xF8, 0x03, sizeof(PMSG_GENS_REGUSER_ANS));
	// ----
	Result.IndexH	= aRecv->IndexH;
	Result.IndexL	= aRecv->IndexL;
	// ----
	if( !gGensDBSet.RegMemberInfo(aRecv->Name, aRecv->Influence) )
	{
		gWindow.PrintLog("[%s] Failed to RegMemberInfo", __FUNCTION__);
		Result.Result = true;
	}
	// ----
	Result.Influence	= aRecv->Influence;
	Result.Result		= false;
	// ----
	wsjServer.DataSend(aIndex, (char*)&Result, sizeof(PMSG_GENS_REGUSER_ANS));
}
// -----------------------------------------------------------------

void GensDelMemberInfo(PMSG_GENS_DELUSER_REQ * aRecv, int aIndex)
{
	PMSG_GENS_DELUSER_ANS Result;
	// ----
	Result.h.set((LPBYTE)&Result, 0xF8, 0x04, sizeof(PMSG_GENS_REGUSER_ANS));
	// ----
	Result.IndexH	= aRecv->IndexH;
	Result.IndexL	= aRecv->IndexL;
	// ----
	if( !gGensDBSet.DelMemberInfo(aRecv->Name) )
	{
		gWindow.PrintLog("[%s] Failed to DelMemberInfo", __FUNCTION__);
		Result.Result = true;
	}
	// ----
	Result.Result = false;
	// ----
	wsjServer.DataSend(aIndex, (char*)&Result, sizeof(PMSG_GENS_REGUSER_ANS));
}
// -----------------------------------------------------------------

void GensGetRewardResult(PMSG_GENS_REWARD_REQ * aRecv, int aIndex)
{
	PMSG_GENS_REWARD_ANS Result;
	// ----
	Result.h.set((LPBYTE)&Result, 0xF8, 0x05, sizeof(PMSG_GENS_REWARD_ANS));
	// ----
	Result.aIndex = aRecv->aIndex;
	Result.Result = true;
	// ----
	if( !gGensDBSet.CheckRewardDate(aRecv->Name) )
	{
		gWindow.PrintLog("[%s] Reward false", __FUNCTION__);
		Result.Result = false;
	}
	// ----
	wsjServer.DataSend(aIndex, (char*)&Result, sizeof(PMSG_GENS_REWARD_ANS));
}
// -----------------------------------------------------------------

void GensSetRewardDate(PMSG_GENS_REWARD_SET * aRecv, int aIndex)
{
	if( !gGensDBSet.SetRewardDate(aRecv->Name) )
	{
		gWindow.PrintLog("[%s] Error update reward date ", __FUNCTION__);
	}
}
// -----------------------------------------------------------------

void GDReqPeriodItemInsert(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_PERIODITEM_INSERT* lpMsg = (PMSG_REQ_PERIODITEM_INSERT*)lpRecv;

	PMSG_ANS_PERIODITEM_INSERT pMsg;
	pMsg.head.set((LPBYTE)&pMsg, 0xD0, 0x04, sizeof(PMSG_ANS_PERIODITEM_INSERT));
	
	pMsg.btResult = PeriodItemDBSet.Insert(lpMsg->iUserGuid, lpMsg->chUserID, lpMsg->iItemCode, lpMsg->btEffectCategory, lpMsg->btItemEffectType1, lpMsg->btItemEffectType2, lpMsg->iItemPeriodDate, lpMsg->chExpireDate);
	pMsg.iUserIndex = lpMsg->iUserIndex;
	pMsg.iItemCode = lpMsg->iItemCode;
	pMsg.btItemEffectType1 = lpMsg->btItemEffectType1;
	pMsg.btItemEffectType2 = lpMsg->btItemEffectType2;
	pMsg.iItemPeriodDate = lpMsg->iItemPeriodDate;

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(PMSG_ANS_PERIODITEM_INSERT));
}

void GDReqPeriodItemDelete(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_PERIODITEM_DELETE* lpMsg = (PMSG_REQ_PERIODITEM_DELETE*)lpRecv;

	PMSG_ANS_PERIODITEM_DELETE pMsg;
	pMsg.head.set((LPBYTE)&pMsg, 0xD0, 0x0A, sizeof(PMSG_ANS_PERIODITEM_DELETE));

	pMsg.btCode = PeriodItemDBSet.Delete(lpMsg->iUserGuid, lpMsg->chCharacterName);
	pMsg.iUserGuid = lpMsg->iUserGuid;
	pMsg.iUserIndex = lpMsg->iUserIndex;

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(PMSG_ANS_PERIODITEM_DELETE));
}

void GDReqPeriodItemUpdate(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_PERIODITEM_UPDATE* lpMsg = (PMSG_REQ_PERIODITEM_UPDATE*)lpRecv;

	PMSG_ANS_PERIODITEM_UPDATE pMsg;
	pMsg.head.set((LPBYTE)&pMsg, 0xD0, 0x08, sizeof(PMSG_ANS_PERIODITEM_UPDATE));

	pMsg.btResult = PeriodItemDBSet.Update(lpMsg->iUserGuid, lpMsg->chCharacterName, lpMsg->iItemCode, lpMsg->iUsedTime, lpMsg->iLeftTime);
	pMsg.iUserIndex = lpMsg->iUserIndex;
	pMsg.iItemCode = lpMsg->iItemCode;

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(PMSG_ANS_PERIODITEM_UPDATE));
}

void GDReqPeriodItemSelect(BYTE *lpRecv, int aIndex)
{
	PMSG_REQ_PERIODITEM_LIST* lpMsg = (PMSG_REQ_PERIODITEM_LIST*)lpRecv;
	PMSG_ANS_PERIODITEM_LIST pList;

	pList.head.set((LPBYTE)&pList, 0xD0, 0x06, sizeof(PMSG_ANS_PERIODITEM_LIST));

	ITEMPERIOD_INFO ItemData;

	ItemData.iUserGuid = lpMsg->iUserGuid;
	memcpy(ItemData.chCharacterName, lpMsg->chCharacterName, MAX_IDSTRING+1);

	pList.btResult = PeriodItemDBSet.Select(&ItemData);
	pList.btItemUseCount = ItemData.btDateInfoUseCount;
	pList.iUserIndex = lpMsg->iUserIndex;
	pList.iUserGuid = lpMsg->iUserGuid;
	
	for(int iItemCount = 0; iItemCount < pList.btItemUseCount; iItemCount++)
	{
		pList.Date[iItemCount].iItemCode = ItemData.DateInfo[iItemCount].iItemCode;
		pList.Date[iItemCount].btEffectCategory = ItemData.DateInfo[iItemCount].btEffectCategory;
		pList.Date[iItemCount].btEffectType1 = ItemData.DateInfo[iItemCount].btEffectType1;
		pList.Date[iItemCount].btEffectType2 = ItemData.DateInfo[iItemCount].btEffectType2;
		pList.Date[iItemCount].btUsedInfo = ItemData.DateInfo[iItemCount].btUsedInfo;
		pList.Date[iItemCount].dwItemUsedTime = ItemData.DateInfo[iItemCount].dwItemUsedTime;
		pList.Date[iItemCount].dwItemLeftTime = ItemData.DateInfo[iItemCount].dwItemLeftTime;
	}

	wsjServer.DataSend(aIndex, (char*)&pList, pList.head.size);
}

void GDSetExtendedInvenCount(LPSDHP_REQ_SET_EXTENDEDINVEN_COUNT lpMsg, int aIndex)
{
	SDHP_ANS_SET_EXTENDEDINVEN_COUNT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xD5, sizeof(SDHP_ANS_SET_EXTENDEDINVEN_COUNT));

	pMsg.Number = lpMsg->Number;
	pMsg.Result = 1;
	pMsg.ExtendedInvenCount = 0;
	pMsg.EventIndex = lpMsg->EventIndex;
	pMsg.ItemPos = lpMsg->ItemPos;
	pMsg.BuyAtInGameShop = lpMsg->BuyAtInGameShop;
	pMsg.IsReplace = lpMsg->IsReplace;

	if( (lpMsg->ItemPos > 203 || lpMsg->ItemPos < 0) && lpMsg->BuyAtInGameShop == 0 && lpMsg->IsReplace == 0)
	{
		pMsg.Result = 0;
		gACDbSet.GetCharacterExtendedInvenCount(lpMsg->szCharName, &pMsg.ExtendedInvenCount);
		wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(SDHP_ANS_SET_EXTENDEDINVEN_COUNT));
		return;
	}

	if(lpMsg->IsReplace == 1)
	{
		pMsg.Result = gACDbSet.SetCharacterExtendedInvenCountReplace(lpMsg->szCharName, lpMsg->ExtendedInvenCount);
	}
	else
	{
		pMsg.Result = gACDbSet.SetCharacterExtendedInvenCountAdd(lpMsg->szCharName, lpMsg->ExtendedInvenCount);
	}

	gACDbSet.GetCharacterExtendedInvenCount(lpMsg->szCharName, &pMsg.ExtendedInvenCount);
	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(SDHP_ANS_SET_EXTENDEDINVEN_COUNT));
}

void GDSetExtendedWarehouseCount(LPSDHP_REQ_SET_EXTENDEDWAREHOUSE_COUNT lpMsg, int aIndex)
{
	SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xD6, sizeof(SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT));

	pMsg.Number = lpMsg->Number;
	pMsg.Result = 1;
	pMsg.ExtendedWarehouseCount = 0;
	pMsg.EventIndex = lpMsg->EventIndex;
	pMsg.ItemPos = lpMsg->ItemPos;
	pMsg.BuyAtInGameShop = lpMsg->BuyAtInGameShop;
	pMsg.IsReplace = lpMsg->IsReplace;
	
	if( (lpMsg->ItemPos > 203 || lpMsg->ItemPos < 0) && lpMsg->BuyAtInGameShop == 0 && lpMsg->IsReplace == 0)
	{
		pMsg.Result = 0;
		gACDbSet.GetCharacterExtendedWarehouseCount(lpMsg->AccountId, &pMsg.ExtendedWarehouseCount);
		wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT));
		gWindow.PrintLog("Extend Warehouse fail %d %d", lpMsg->ItemPos, pMsg.ExtendedWarehouseCount);
		return;
	}

	if(lpMsg->IsReplace == 1)
	{
		pMsg.Result = gACDbSet.SetCharacterExtendedWarehouseCountReplace(lpMsg->AccountId, lpMsg->ExtendedWarehouseCount);
	}
	else
	{
		pMsg.Result = gACDbSet.SetCharacterExtendedWarehouseCountAdd(lpMsg->AccountId, lpMsg->ExtendedWarehouseCount);
	}

	gACDbSet.GetCharacterExtendedWarehouseCount(lpMsg->AccountId, &pMsg.ExtendedWarehouseCount);
	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT));
}

void GDGameShopSaveStorageItem(GAMESHOP_GD_SAVE_STORAGEITEM * aRecv, int aIndex)
{
	gACDbSet.SaveGameShopStorageItem(aRecv->AccountID, aRecv->Item);
}

void GDGameShopDelStorageItem(GAMESHOP_GD_DEL_STORAGEITEM * aRecv, int aIndex)
{
	gACDbSet.DelGameShopStorageItem(aRecv->AccountID, aRecv->Item);
}

void GDGameShopReqStorage(GAMESHOP_GD_STORAGE * aRecv, int aIndex)
{
	GAMESHOP_GD_STORAGE pResult;
	PHeadSubSetW((LPBYTE)&pResult, 0xD3, 1, sizeof(GAMESHOP_GD_STORAGE));
	memcpy(pResult.AccountID, aRecv->AccountID, MAX_IDSTRING);
	pResult.UserIndex = aRecv->UserIndex;
	// ----
	if( gACDbSet.LoadGameShopStorage(aRecv->AccountID, &pResult) )
	{
		wsjServer.DataSend(aIndex, (char*)&pResult, sizeof(pResult));
	}
}

void GDGameShopSavePoint(GAMESHOP_GD_SAVE_POINT * aRecv, int aIndex)
{
	gGameShopDB.SavePoint(aRecv->AccountID, aRecv->WCoinP, aRecv->WCoinC, aRecv->GoblinPoint);
}

void GDGameShopReqPoint(GAMESHOP_GD_REQ_POINT * aRecv, int aIndex)
{
	GAMESHOP_DG_GET_POINT pAnswer;
	pAnswer.h.set((LPBYTE)&pAnswer, 0xD2, 0x02, sizeof(pAnswer));
	// ----
	if( !gGameShopDB.ReqPoint(aRecv->AccountID, &pAnswer) )
	{
		gWindow.PrintLog("[%s] Error", __FUNCTION__);
		return;
	}
	// ----
	pAnswer.UserIndex = aRecv->UserIndex;
	wsjServer.DataSend(aIndex, (char*)&pAnswer, sizeof(pAnswer));

}

void GDMacroLoad(LPSDHP_MACRODATA lpMsg, int aIndex)
{
	SDHP_MACRODATA pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x17, sizeof(SDHP_MACRODATA));

	BOOL bFlag = gCharDbSet.LoadMacroInfo(lpMsg->AccountID, lpMsg->Name, pMsg.btMacroData);

	if(bFlag != FALSE)
	{
		pMsg.btResult = 0;
	}
	else
	{
		pMsg.btResult = 1;
	}
	
	memcpy(pMsg.AccountID, lpMsg->AccountID, MAX_IDSTRING+1);
	memcpy(pMsg.Name, lpMsg->Name, MAX_IDSTRING+1);
	pMsg.aIndex = lpMsg->aIndex;

	wsjServer.DataSend(aIndex, (char*)&pMsg, sizeof(SDHP_MACRODATA));
}

void GDMacroSave(LPSDHP_MACRODATA lpMsg, int aIndex)
{
	gCharDbSet.SaveMacroInfo(lpMsg->AccountID, lpMsg->Name, lpMsg->btMacroData);
}

void GDReqQuestExpInfoSave(LP_PMSG_QUESTEXP_INFO lpMsg)
{
	g_QuestExpDBSet.DSDB_QueryQuestInfoSave(lpMsg);
}

void GDReqQuestExpInfoLoad(LPPMSG_REQ_QUESTEXP_INFO lpMsg, int aIndex)
{
	_QUESTEXP_INFO QuestInfo[25];

	PMSG_ANS_QUESTEXP_INFO pMsg;

	BYTE sendbuf[2048] = {0};

	int lOfs = 0;
	lOfs = 9;

	pMsg.iUserIndex = lpMsg->iUserIndex;

	int iRet = 0;
	
	iRet = g_QuestExpDBSet.DSDB_QueryQuestInfoLoad(lpMsg->szCharName, QuestInfo, &pMsg);

	if(iRet != 0)
	{
//		LogAddC(LOGC_RED,"[QuestExp Error - GDReqQuestExpInfoLoad] Ret:%d, CharName : %s, UserIndex : %d",
//			iRet, lpMsg->szCharName, lpMsg->iUserIndex);
		return;
	}

	memcpy(&sendbuf[lOfs], QuestInfo, 29 * pMsg.btQuestCnt);
	pMsg.head.set((LPBYTE)&pMsg, 0xF6, lOfs +=(29 * pMsg.btQuestCnt));
    memcpy(sendbuf, &pMsg, 9);

	wsjServer.DataSend(aIndex, (char*)&sendbuf, lOfs);
}
