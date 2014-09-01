#ifndef DSPROTOCOL_H
#define DSPROTOCOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "..\include\prodef.h"
#include "user.h"

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Char List Count Packet
 *	Direction : DataServer -> GameServer
 *  Code : 0xC2
 *	HeadCode : 0x01
 *	BASE PACKET - Complete with SDHP_CHARLIST
 */
struct SDHP_CHARLISTCOUNT
{
	PWMSG_HEAD h;
	short Number;	// 4
	BYTE Count;	// 6
	int DbNumber;	// 8
	BYTE Magumsa;	// C
	char AccountId[11];	// D
	BYTE MoveCnt;	// 18
#ifdef EXPINV
	BYTE ExpandedWarehouse;
#endif
};


struct SDHP_CHARLIST
{
	BYTE Index;	// 0
	char Name[10];	// 1
	WORD Level;	// C
	BYTE Class;	// E
	BYTE CtlCode;	// F
	BYTE dbInventory[48];	// 10
	BYTE DbVersion;	// 40
	BYTE btGuildStatus;	// 41
};

struct SDHP_CREATECHARRESULT
{
	PBMSG_HEAD h;
	BYTE Result;	// 3
	short Number;	// 4
	char AccountId[10];	// 6
	char Name[10];	// 10
	BYTE Pos;	// 1A
	BYTE ClassSkin;	// 1B
	BYTE Equipment[24];	// 1C
	WORD Level;	// 34
};

struct SDHP_CHARDELETERESULT
{
	PBMSG_HEAD h;
	BYTE Result;	// 3
	short Number;	// 4
	char AccountID[10];	// 6
};

struct SDHP_DBCHAR_INFORESULT
{
	PWMSG_HEAD h;
	BYTE result;	// 4
	short Number;	// 6
	char AccountID[10];	// 8
	char Name[11];	// 12
	BYTE Class;	// 1D
	short Level;	// 1E
	int LevelUpPoint;	// 20
	int Exp;	// 24
	int NextExp;	// 28
	int Money;	// 2C
#if( FIX_MAXSTAT == 1 )
	WORD Str;	// 30
	WORD Dex;	// 32
	WORD Vit;	// 34
	WORD Energy;	// 36
#else
	short Str;	// 30
	short Dex;	// 32
	short Vit;	// 34
	short Energy;	// 36
#endif
	WORD Life;	// 38
	WORD MaxLife;	// 3A
	WORD Mana;	// 3C
	WORD MaxMana;	// 3E
	BYTE dbInventory[DB_MAX_INVENTORY_SIZE];	// 40
	BYTE dbMagicList[MAX_MAGIC*3];	// 700
	BYTE MapNumber;	// 7B4
	BYTE MapX;	// 7B5
	BYTE MapY;	// 7B6
	BYTE Dir;	// 7B7
	int PkCount;	// 7B8
	int PkLevel;	// 7BC
	int PkTime;	// 7C0
	BYTE CtlCode;	// 7C4
	BYTE DbVersion;	// 7C5
	BYTE AccountCtlCode;	// 7C6
	BYTE dbQuest[50];	// 7C7
	WORD Leadership;	// 7FA
	WORD ChatLitmitTime;	// 7FC
	int iFruitPoint;	// 800
	int Reset;
	int GrandReset;
#ifdef EXPINV
	int ExpandedInventory;
#endif
#ifdef __ALIEN__
	int DbOffExp;
#endif
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Warehouse DB Save Packet
 *	Direction : GameServer -> DataServer [0x09]
 *	            DataServer -> GameServer [0x08]
 *  Code      : 0xC2
 *	HeadCode  : 0x08, 0x09
 */
struct SDHP_GETWAREHOUSEDB_SAVE
{
	PWMSG_HEAD h;
	char AccountID[10];	// 4
	short aIndex;	// E
	int Money;	// 10
	BYTE dbItems[DB_MAX_VAULT_SIZE];	// 14
	BYTE DbVersion;	// 794
	short pw;	// 796
};

struct SDHP_GETWAREHOUSEDB_RESULT
{
	PBMSG_HEAD h;
	char AccountID[10];	// 3
	short aIndex;	// E
};

struct SDHP_ITEMCREATERECV
{
	PBMSG_HEAD h;
	BYTE x;	// 3
	BYTE y;	// 4
	BYTE MapNumber;	// 5 [235:Mu_2nd_Aniv], [236:CashShop]
	DWORD m_Number;	// 8
	short Type;	// C
	BYTE Level;	// E
	BYTE Dur;	// F
	BYTE Op1;	// 10
	BYTE Op2;	// 11
	BYTE Op3;	// 12
	BYTE NewOption;	// 13
	int aIndex;	// 14
	short lootindex;	// 18
	BYTE SetOption;	// 1A
#ifdef PERIOD
	long lDuration;	//New
	DWORD dwEventIndex;	//New (Unknown for what system it used...)
#endif
};

struct SDHP_SKILLKEYDATA_SEND
{
	PBMSG_HEAD h;
	int aIndex;	// 4
	char Name[10];	// 8
	BYTE SkillKeyBuffer[20];	// 12
	BYTE GameOption;	// 1C
	BYTE QkeyDefine;	// 1D
	BYTE WkeyDefine;	// 1E
	BYTE EkeyDefine;	// 1F
	BYTE ChatWindow;	// 20
	BYTE RkeyDefine;	// 21
	int QWERLevel;	// 22
};


struct SDHP_CHARACTER_TRANSFER_RESULT
{
	PBMSG_HEAD h;
	char Account[10];	// 3
	short Number;	// E
	BYTE Result;	// 10
};


void DataServerProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen);
void TestDSSend();
void DataServerLogin(int server);
void DataServerLoginResult( SDHP_RESULT* lpMsg);
void JGPGetCharList(LPBYTE lpRecv);
void DataServerGetCharListRequest( short aIndex);
void JGCharacterCreateRequest( SDHP_CREATECHARRESULT* lpMsg);
void JGCharacterCreateFailSend(int aIndex, char* id);
void JGCharDelRequest( SDHP_CHARDELETERESULT* lpMsg);
void JGGetCharacterInfo( SDHP_DBCHAR_INFORESULT* lpMsg);
void GCItemListSend(int aIndex);
void GJSetCharacterInfo(struct OBJECTSTRUCT* lpObj, int aIndex, BOOL bMapServerMove, BYTE byExMapNumber);
void GDGetWarehouseList(int aIndex, char* AccountID);
void DGGetWarehouseList( SDHP_GETWAREHOUSEDB_SAVE* lpMsg);
void GDGetWarehouseNoItem( SDHP_GETWAREHOUSEDB_RESULT* lpMsg);
void GDSetWarehouseList(int aIndex);
void GDUserItemSave( OBJECTSTRUCT* lpObj);
void GDSetWarehouseMoney(int aIndex);
void GDGameServerInfoSave();
void ItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
void PetItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level,BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
void ItemSerialCreateRecv( SDHP_ITEMCREATERECV* lpMsg);
void ItemMovePathSave(char* ActID, char* Name, BYTE level, BYTE mapnumber, BYTE x, BYTE y, char* Item, BYTE op1, BYTE op2, BYTE op3, DWORD serial);
void DGOptionDataSend(int aIndex, char* szName, unsigned char* KeyBuffer,unsigned char GO,unsigned char Qk,unsigned char Wk, unsigned char Ek,unsigned char ChatWnd, unsigned char Rk, int QWERLevel);
void DGOptionDataRecv( SDHP_SKILLKEYDATA_SEND* lpMsg);
void DGMoveOtherServer( SDHP_CHARACTER_TRANSFER_RESULT* lpMsg);
void gObjRequestPetItemInfo(int aIndex, int inventype);
void DGRecvPetItemInfo(LPBYTE lpData);
void gObjSavePetItemInfo(int aIndex, int inventype);
void GS_GDReqCastleTotalInfo(int iMapSvrGroup, int iCastleEventCycle);
void GS_GDReqOwnerGuildMaster(int iMapSvrGroup, int aIndex);
void GS_GDReqCastleNpcBuy(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcDfLevel, int iNpcRgLevel, int iNpcMaxHP, int iNpcHP, BYTE btNpcX, BYTE btNpcY, BYTE btNpcDIR,  int iBuyCost);
void GS_GDReqCastleNpcRepair(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iRepairCost);
void GS_GDReqCastleNpcUpgrade(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex);
void GS_GDReqTaxInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqTaxRateChange(int iMapSvrGroup, int aIndex, int iTaxType, int iTaxRate);
void GS_GDReqCastleMoneyChange(int iMapSvrGroup, int aIndex, int iMoneyChange);
void GS_GDReqSiegeDateChange(int iMapSvrGroup, int aIndex, WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay);
void GS_GDReqGuildMarkRegInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqSiegeEndedChange(int iMapSvrGroup, int bIsSiegeEnded);
void GS_GDReqCastleOwnerChange(int iMapSvrGroup, int bIsCastleOccupied, char* lpszGuildName);
void GS_GDReqRegAttackGuild(int iMapSvrGroup, int aIndex);
void GS_GDReqRestartCastleState(int iMapSvrGroup);
void GS_GDReqMapSvrMsgMultiCast(int iMapSvrGroup, char* lpszMsgText);
void GS_GDReqRegGuildMark(int iMapSvrGroup, int aIndex, int iItemPos);
void GS_GDReqGuildMarkReset(int iMapSvrGroup, int aIndex, char* lpszGuildName);
void GS_GDReqGuildSetGiveUp(int iMapSvrGroup, int aIndex, int bIsGiveUp);
void GS_GDReqNpcRemove(int iMapSvrGroup, int iNpcNumber, int iNpcIndex);
void GS_GDReqCastleStateSync(int iMapSvrGroup, int iCastleState, int iTaxRateChaos, int iTaxRateStore, int iTaxHuntZone, char* lpszOwnerGuild);
void GS_GDReqCastleTributeMoney(int iMapSvrGroup, int iCastleTributeMoney);
void GS_GDReqResetCastleTaxInfo(int iMapSvrGroup);
void GS_GDReqResetSiegeGuildInfo(int iMapSvrGroup);
void GS_GDReqResetRegSiegeInfo(int iMapSvrGroup);
void GS_GDReqCastleInitData(int iMapSvrGroup, int iCastleEventCycle);
void GS_GDReqCastleNpcInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqAllGuildMarkRegInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqCalcRegGuildList(int iMapSvrGroup);
void GS_GDReqCsLoadTotalGuildInfo(int iMapSvrGroup);
void GS_DGAnsCastleTotalInfo(LPBYTE lpRecv);
void GS_DGAnsOwnerGuildMaster(LPBYTE lpRecv);
void GS_DGAnsCastleNpcBuy(LPBYTE lpRecv);
void GS_DGAnsCastleNpcRepair(LPBYTE lpRecv);
void GS_DGAnsCastleNpcUpgrade(LPBYTE lpRecv);
void GS_DGAnsTaxInfo(LPBYTE lpRecv);
void GS_DGAnsTaxRateChange(LPBYTE lpRecv);
void GS_DGAnsCastleMoneyChange(LPBYTE lpRecv);
void GS_DGAnsSiegeDateChange(LPBYTE lpRecv);
void GS_DGAnsGuildMarkRegInfo(LPBYTE lpRecv);
void GS_DGAnsSiegeEndedChange(LPBYTE lpRecv);
void GS_DGAnsCastleOwnerChange(LPBYTE lpRecv);
void GS_DGAnsRegAttackGuild(LPBYTE lpRecv);
void GS_DGAnsRestartCastleState(LPBYTE lpRecv);
void GS_DGAnsMapSvrMsgMultiCast(LPBYTE lpRecv);
void GS_DGAnsRegGuildMark(LPBYTE lpRecv);
void GS_DGAnsGuildMarkReset(LPBYTE lpRecv);
void GS_DGAnsGuildSetGiveUp(LPBYTE lpRecv);
void GS_DGAnsNpcRemove(LPBYTE lpRecv);
void GS_DGAnsCastleStateSync(LPBYTE lpRecv);
void GS_DGAnsCastleTributeMoney(LPBYTE lpRecv);
void GS_DGAnsResetCastleTaxInfo(LPBYTE lpRecv);
void GS_DGAnsResetSiegeGuildInfo(LPBYTE lpRecv);
void GS_DGAnsResetRegSiegeInfo(LPBYTE lpRecv);
void GS_DGAnsCastleInitData(LPBYTE lpRecv);
void GS_DGAnsCastleNpcInfo(LPBYTE lpRecv);
void GS_DGAnsAllGuildMarkRegInfo(LPBYTE lpRecv);
void GS_DGAnsFirstCreateNPC(LPBYTE lpRecv);
void GS_DGAnsCalcREgGuildList(LPBYTE lpRecv);
void GS_DGAnsCsGulidUnionInfo(LPBYTE lpRecv);
void GS_DGAnsCsSaveTotalGuildInfo(LPBYTE lpRecv);
void GS_DGAnsCsLoadTotalGuildInfo(LPBYTE lpRecv);
void GS_DGAnsCastleNpcUpdate(LPBYTE lpRecv);
void GDReqCrywolfSync(int iMapSvrGroup, int iCrywolfState, int iOccupationState);
void DGAnsCrywolfSync(LPBYTE lpRecv);
void GDReqCrywolfInfoLoad(int iMapSvrGroup);
void DGAnsCrywolfInfoLoad(LPBYTE lpRecv);
void GDReqCrywolfInfoSave(int iMapSvrGroup, int iCrywolfState, int iOccupationState);
void DGAnsCrywolfInfoSave(LPBYTE lpRecv);



typedef struct
{
	PBMSG_HEAD h;
	int aIndex;
	char szAccountID[10];//8
	char szName[10];//
	char szName2[10];
	BYTE btResult;	// 26
}SDHP_CHANGENAME_RESULT, *LPSDHP_CHANGENAME_RESULT;

void DGChangeNameResult(SDHP_CHANGENAME_RESULT * lpMsg);

typedef struct
{
	PBMSG_HEAD h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	short aIndex;
	BYTE btResult;
	BYTE IsSummonerEnable;
	BYTE IsRageFighterEnable;
}SDHP_ANS_ACCOUNTINFO, *LPSDHP_ANS_ACCOUNTINFO;

void DGAccountInfoStateUpdatedSend(SDHP_ANS_ACCOUNTINFO *lpMsg);

void DGAccountInfoStateUpdate(LPOBJ lpObj, int iIndex);


typedef struct
{
	PBMSG_HEAD h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	short aIndex;
	BYTE btResultSummoner;
	BYTE btResultRageFighter;
}PMSG_ANS_ACCOUNTINFO_STATUS, *LPPMSG_ANS_ACCOUNTINFO_STATUS;

typedef struct
{
	PBMSG_HEAD h;
	BYTE btResult;
	BYTE IsSummonerEnable;
	BYTE IsRageFighterEnable;
}PMSG_UPD_ACCOUNTINFO_CREATE, *LPPMSG_UPD_ACCOUNTINFO_CREATE;

void DGAccountInfoStateRecv(PMSG_ANS_ACCOUNTINFO_STATUS *lpMsg);
void GDSetGameMasterEvent(char * Name, int aIndex);

// -------------------------------------------------------------------------------

struct SDHP_REQ_SET_EXTENDEDINVEN_COUNT
{
	PBMSG_HEAD h;
	char szCharName[MAX_IDSTRING+1];
	WORD Number;
	BYTE ExtendedInvenCount;
	DWORD EventIndex;
	int ItemPos;
	BYTE BuyAtInGameShop;
	BYTE IsReplace;
};

struct SDHP_REQ_SET_EXTENDEDWAREHOUSE_COUNT
{
	PBMSG_HEAD h;
	char AccountId[MAX_IDSTRING+1];
	WORD Number;
	BYTE ExtendedWarehouseCount;
	DWORD EventIndex;
	int ItemPos;
	BYTE BuyAtInGameShop;
	BYTE IsReplace;
};

struct SDHP_ANS_SET_EXTENDEDINVEN_COUNT
{
	PBMSG_HEAD h;
	WORD Number;
	BYTE Result;
	BYTE ExtendedInvenCount;
	DWORD EventIndex;
	int ItemPos;
	BYTE BuyAtInGameShop;
	BYTE IsReplace;
};

struct SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT
{
	PBMSG_HEAD h;
	WORD Number;
	BYTE Result;
	BYTE ExtendedWarehouseCount;
	DWORD EventIndex;
	int ItemPos;
	BYTE BuyAtInGameShop;
	BYTE IsReplace;
};

//1.01.00 start
void GDUpdateExtendedInvenCount(LPOBJ lpObj, BYTE btExtendedInvenCount, DWORD dwEventIndex, int iItemPos, bool bBuyAtInGameShop, bool bIsReplace);
void DGUpdateExtendedInvenCountResult(SDHP_ANS_SET_EXTENDEDINVEN_COUNT * lpMsg);
void GDUpdateExtendedWarehouseCount(LPOBJ lpObj, BYTE btExtendedWarehouseCount, DWORD dwEventIndex, int iItemPos, bool bBuyAtInGameShop, bool bIsReplace);
void DGUpdateExtendedWarehouseResult(SDHP_ANS_SET_EXTENDEDWAREHOUSE_COUNT * lpMsg);
// -------------------------------------------------------------------------------

#ifdef __MUANGEL__
#pragma pack(push, 1)
struct UPDATEWAREHOUSESTATUS
{
	PBMSG_HEAD2 h;
	char	AccountID[MAX_IDSTRING+1];
	BYTE	Status;
};
#pragma pack(pop)

void UpdateWarehouseStatus(LPOBJ lpObj, BYTE State);
#endif
#endif