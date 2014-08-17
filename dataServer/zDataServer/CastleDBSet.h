// CastleDBSet.h: interface for the CCastleDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLEDBSET_H__866EE47E_B854_4D47_8664_64C3026FFE59__INCLUDED_)
#define AFX_CASTLEDBSET_H__866EE47E_B854_4D47_8664_64C3026FFE59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"

#include "DBConBase.h"

struct CASTLE_DATA 
{
	WORD wStartYear;
	BYTE btStartMonth;
	BYTE btStartDay;
	WORD wEndYear;
	BYTE btEndMonth;
	BYTE btEndDay;
	BYTE btIsSiegeGuildList;
	BYTE btIsSiegeEnded;
	BYTE btIsCastleOccupied;
	char szCastleOwnGuild[MAX_GUILDNAMESTRING+1];
	__int64 i64CastleMoney;
	int iTaxRateChaos;
	int iTaxRateStore;
	int iTaxHuntZone;
	int iFirstCreate;
};

struct CSP_ANS_OWNERGUILDMASTER 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szCastleOwnGuild[MAX_GUILDNAMESTRING];
	char szCastleOwnGuildMaster[MAX_IDSTRING];
};

struct CSP_REQ_NPCBUY 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcDfLevel;
	int iNpcRgLevel;
	int iNpcMaxHp;
	int iNpcHp;
	BYTE btNpcX;
	BYTE btNpcY;
	BYTE btNpcDIR;
	int iBuyCost;
};

struct CSP_REQ_NPCREPAIR 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	int iNpcNumber;
	int iNpcIndex;
	int iRepairCost;
};

struct CSP_ANS_NPCREPAIR 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcMaxHp;
	int iNpcHp;
	int iRepairCost;
};

struct CSP_REQ_NPCUPGRADE 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcUpType;
	int iNpcUpValue;
	int iNpcUpIndex;
};

struct CSP_ANS_TAXINFO 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	__int64 i64CastleMoney;
	int iTaxRateChaos;
	int iTaxRateStore;
	int iTaxHuntZone;
};

struct CSP_ANS_TAXRATECHANGE 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	int iTaxKind;
	int iTaxRate;
};

struct CSP_REQ_SDEDCHANGE 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	WORD wStartYear;
	BYTE btStartMonth;
	BYTE btStartDay;
	WORD wEndYear;
	BYTE btEndMonth;
	BYTE btEndDay;
};
struct CSP_ANS_SDEDCHANGE 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	WORD wStartYear;
	BYTE btStartMonth;
	BYTE btStartDay;
	WORD wEndYear;
	BYTE btEndMonth;
	BYTE btEndDay;
};
struct CSP_REQ_GUILDREGINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
};
struct CSP_ANS_GUILDREGINFO 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
	int iRegMarkCount;
	bool bIsGiveUp;
	BYTE btRegRank;
};
struct CSP_ANS_NPCBUY 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	int iNpcNumber;
	int iNpcIndex;
	int iBuyCost;
};
struct CSP_ANS_NPCUPGRADE 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcUpType;
	int iNpcUpValue;
	int iNpcUpIndex;
};

struct CSP_REQ_TAXINFO 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
};

struct CSP_REQ_TAXRATECHANGE 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	int iTaxKind;
	int iTaxRate;
};

struct CSP_REQ_MONEYCHANGE 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	int iMoneyChanged;
};

struct CSP_ANS_MONEYCHANGE 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	int iMoneyChanged;
	__int64 i64CastleMoney;
};
struct CSP_REQ_SIEGEENDCHANGE 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int bIsSiegeEnded;
};

struct CSP_ANS_SIEGEENDCHANGE 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int bIsSiegeEnded;
};
struct CSP_REQ_CASTLEOWNERCHANGE 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	BOOL bIsCastleOccupied;
	char szOwnerGuildName[MAX_GUILDNAMESTRING];
};
struct CSP_ANS_CASTLEOWNERCHANGE 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	BOOL bIsCastleOccupied;
	char szOwnerGuildName[MAX_GUILDNAMESTRING];
};
struct CSP_REQ_REGATTACKGUILD 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	char szEnemyGuildName[MAX_GUILDNAMESTRING];
};


struct CSP_ANS_REGATTACKGUILD 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szEnemyGuildName[MAX_GUILDNAMESTRING];
};
struct CSP_REQ_MAPSVRMULTICAST 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	char szMsgText[128];
};

struct CSP_ANS_MAPSVRMULTICAST 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	char szMsgText[128];
};
struct CSP_REQ_CASTLESIEGEEND 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};
struct CSP_ANS_CASTLESIEGEEND 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
};
struct CSP_ANS_GUILDREGMARK 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
	int iItemPos;
	int iRegMarkCount;
};
struct CSP_REQ_GUILDREGMARK 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
	int iItemPos;
};
struct CSP_ANS_GUILDRESETMARK 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
	int iRegMarkCount;
};
struct CSP_REQ_GUILDRESETMARK 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
};
struct CSP_ANS_GUILDSETGIVEUP 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
	BOOL bIsGiveUp;
	int iRegMarkCount;
};
struct CSP_REQ_GUILDSETGIVEUP 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[MAX_GUILDNAMESTRING];
	BOOL bIsGiveUp;
};
struct CSP_REQ_NPCREMOVE 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iNpcNumber;
	int iNpcIndex;
};
struct CSP_ANS_NPCREMOVE 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iNpcNumber;
	int iNpcIndex;
};
struct CSP_REQ_CASTLESTATESYNC 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iCastleState;
	int iTaxRateChaos;
	int iTaxRateStore;
	int iTaxHuntZone;
	char szOwnerGuildName[MAX_GUILDNAMESTRING];
};
struct CSP_ANS_CASTLESTATESYNC 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iCastleState;
	int iTaxRateChaos;
	int iTaxRateStore;
	int iTaxHuntZone;
	char szOwnerGuildName[MAX_GUILDNAMESTRING];
};
struct CSP_NPCDATA
{
	int iNpcNumber;
	int iNpcIndex;
	int iNpcDfLevel;
	int iNpcRgLevel;
	int iNpcMaxHp;
	int iNpcHp;
	BYTE btNpcX;
	BYTE btNpcY;
	BYTE btNpcDIR;
};

struct CSP_GUILDREGINFO 
{
	char szGuildName[MAX_GUILDNAMESTRING];
	int iRegMarkCount;
	bool bIsGiveUp;
	BYTE btRegRank;
};

struct CSP_REQ_NPCSAVEDATA 
{
	PWMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_ANS_NPCSAVEDATA 
{
	PBMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
};

struct CSP_CALCREGGUILDLIST 
{
	char szGuildName[MAX_GUILDNAMESTRING];
	int iRegMarkCount;
	int iGuildMemberCount;
	int iGuildMasterLevel;
	int iSeqNum;
};

struct CSP_CSGUILDUNIONINFO 
{
	char szGuildName[MAX_GUILDNAMESTRING];
	int iCsGuildID;
};

struct CSP_CSLOADTOTALGUILDINFO 
{
	char szGuildName[MAX_GUILDNAMESTRING];
	int iCsGuildID;
	int iGuildInvolved;
	int iGuildScore;
};

struct CSP_REQ_NPCUPDATEDATA 
{
	PWMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_NPCSAVEDATA 
{
	int iNpcNumber;
	int iNpcIndex;
	int iNpcDfLevel;
	int iNpcRgLevel;
	int iNpcMaxHp;
	int iNpcHp;
	BYTE btNpcX;
	BYTE btNpcY;
	BYTE btNpcDIR;
};

struct CSP_REQ_CASTLETRIBUTEMONEY 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	int iCastleTributeMoney;
};

struct CSP_ANS_CASTLETRIBUTEMONEY 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
};

struct CSP_REQ_RESETCASTLETAXINFO 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct CSP_ANS_RESETCASTLETAXINFO 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
};
struct CSP_REQ_RESETSIEGEGUILDINFO 
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct CSP_ANS_RESETSIEGEGUILDINFO 
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
};

struct CSP_REQ_CSINITDATA 
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
	int iCastleEventCycle;
};


struct CSP_ANS_CSINITDATA 
{
	PWMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
	WORD wStartYear;
	BYTE btStartMonth;
	BYTE btStartDay;
	WORD wEndYear;
	BYTE btEndMonth;
	BYTE btEndDay;
	BYTE btIsSiegeGuildList;
	BYTE btIsSiegeEnded;
	BYTE btIsCastleOccupied;
	char szCastleOwnGuild[MAX_GUILDNAMESTRING];
	__int64 i64CastleMoney;
	int iTaxRateChaos;
	int iTaxRateStore;
	int iTaxHuntZone;
	int iFirstCreate;
	int iCount;
};

struct CSP_REQ_NPCDATA 
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
	int iIndex;
};

struct CSP_ANS_NPCDATA 
{
	PWMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	int iCount;
};

struct CSP_REQ_ALLGUILDREGINFO 
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
	int iIndex;
};

struct CSP_ANS_ALLGUILDREGINFO
{
	PWMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	int iCount;
};

struct CSP_REQ_CALCREGGUILDLIST 
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
};

struct CSP_ANS_CALCREGGUILDLIST 
{
	PWMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_REQ_CSGUILDUNIONINFO 
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_ANS_CSGUILDUNIONINFO 
{
	PWMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_REQ_CSSAVETOTALGUILDINFO 
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_CSSAVETOTALGUILDINFO 
{
	char szGuildName[MAX_GUILDNAMESTRING];
	int iCsGuildID;
	int iGuildInvolved;
	int iGuildScore;
};

struct CSP_ANS_CSSAVETOTALGUILDINFO 
{
	PBMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
};

struct CSP_REQ_CSLOADTOTALGUILDINFO 
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
};

struct CSP_ANS_CSLOADTOTALGUILDINFO 
{
	PWMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_NPCUPDATEDATA
{
	int iNpcNumber;
	int iNpcIndex;
	int iNpcDfLevel;
	int iNpcRgLevel;
	int iNpcMaxHp;
	int iNpcHp;
	BYTE btNpcX;
	BYTE btNpcY;
	BYTE btNpcDIR;
};

class CCastleDBSet  : public CDBConBase
{
public:
	CCastleDBSet();
	virtual ~CCastleDBSet();
	
	BOOL Connect();
	BOOL DSDB_QueryCastleTotalInfo(int iMapSvrGroup, int iCastleEventCycle, CASTLE_DATA* lpCastleData);
	BOOL DSDB_QueryOwnerGuildMaster(int iMapSvrGroup, CSP_ANS_OWNERGUILDMASTER* lpOwnerGuildMaster);
	BOOL DSDB_QueryCastleNpcBuy(int iMapSvrGroup, CSP_REQ_NPCBUY* lpNpcBuy, int* lpiResult);
	BOOL DSDB_QueryCastleNpcRepair(int iMapSvrGroup, CSP_REQ_NPCREPAIR* lpNpcRepair, CSP_ANS_NPCREPAIR* lpNpcRepairResult,  int* lpiResult);
	BOOL DSDB_QueryCastleNpcUpgrade(int iMapSvrGroup, CSP_REQ_NPCUPGRADE* lpNpcUpgrade);
	BOOL DSDB_QueryTaxInfo(int iMapSvrGroup, CSP_ANS_TAXINFO* lpTaxInfo);
	BOOL DSDB_QueryTaxRateChange(int iMapSvrGroup, int iTaxType, int iTaxRate,  CSP_ANS_TAXRATECHANGE* lpTaxRateChange, int* lpiResult);
	BOOL DSDB_QueryCastleMoneyChange(int iMapSvrGroup, int iMoneyChange, __int64* i64ResultMoney,  int* lpiResult);
	BOOL DSDB_QuerySiegeDateChange(int iMapSvrGroup, CSP_REQ_SDEDCHANGE* lpSdEdChange, int* lpiResult);
	BOOL DSDB_QueryGuildMarkRegInfo(int iMapSvrGroup, char* lpszGuildName, CSP_ANS_GUILDREGINFO* lpGuildRegInfo,  int* lpiResult);
	BOOL DSDB_QuerySiegeEndedChange(int iMapSvrGroup, int bIsCastleSiegeEnded, int* lpiResult);
	BOOL DSDB_QueryCastleOwnerChange(int iMapSvrGroup, CSP_REQ_CASTLEOWNERCHANGE* lpCastleOwnerInfo, CSP_ANS_CASTLEOWNERCHANGE* lpCastleOwnerInfoResult,  int* lpiResult);
	BOOL DSDB_QueryRegAttackGuild(int iMapSvrGroup, CSP_REQ_REGATTACKGUILD* lpRegAttackGuild, CSP_ANS_REGATTACKGUILD* lpRegAttackGuildResult,  int* lpiResult);
	BOOL DSDB_QueryRestartCastleState(int iMapSvrGroup, CSP_REQ_CASTLESIEGEEND* lpCastleSiegeEnd, int* lpiResult);
	BOOL DSDB_QueryGuildMarkRegMark(int iMapSvrGroup, char* lpszGuildName, CSP_ANS_GUILDREGMARK* lpGuildRegMark,  int* lpiResult);
	BOOL DSDB_QueryGuildMarkReset(int iMapSvrGroup, char* lpszGuildName, CSP_ANS_GUILDRESETMARK* lpGuildResetMark);
	BOOL DSDB_QueryGuildSetGiveUp(int iMapSvrGroup, char* lpszGuildName, int bIsGiveUp,  CSP_ANS_GUILDSETGIVEUP* lpGuildSetGiveUp);
	BOOL DSDB_QueryCastleNpcRemove(int iMapSvrGroup, CSP_REQ_NPCREMOVE* lpNpcRemove, int* lpiResult);
	BOOL DSDB_QueryResetCastleTaxInfo(int iMapSvrGroup, int* lpiResult);
	BOOL DSDB_QueryResetSiegeGuildInfo(int iMapSvrGroup, int* lpiResult);
	BOOL DSDB_QueryResetRegSiegeInfo(int iMapSvrGroup, int* lpiResult);
	BOOL DSDB_QueryCastleNpcInfo(int iMapSvrGroup, CSP_NPCDATA* lpNpcData, int* lpiCount);
	BOOL DSDB_QueryAllGuildMarkRegInfo(int iMapSvrGroup, CSP_GUILDREGINFO* lpGuildRegInfo, int* lpiCount);
	BOOL DSDB_QueryFirstCreateNPC(int iMapSvrGroup, CSP_REQ_NPCSAVEDATA* lpNpcSaveData);
	BOOL DSDB_QueryCalcRegGuildList(int iMapSvrGroup, CSP_CALCREGGUILDLIST* lpCalcRegGuildList, int* lpiCount);
	BOOL DSDB_QueryCsGuildUnionInfo(int iMapSvrGroup, char* lpszGuildName, int iCsGuildID,  CSP_CSGUILDUNIONINFO* lpCsGuildUnionInfo, int* lpiCount);
	BOOL DSDB_QueryCsClearTotalGuildInfo(int iMapSvrGroup);
	BOOL DSDB_QueryCsSaveTotalGuildInfo(int iMapSvrGroup, char* lpszGuildName, int iCsGuildID,  int iCsGuildInvolved, int iCsGuildScore);
	BOOL DSDB_QueryCsSaveTotalGuildOK(int iMapSvrGroup, int* lpiResult);
	BOOL DSDB_QueryCsLoadTotalGuildInfo(int iMapSvrGroup, CSP_CSLOADTOTALGUILDINFO* lpLoadTotalGuildInfo, int* lpiCount);
	BOOL DSDB_QueryCastleNpcUpdate(int iMapSvrGroup, CSP_REQ_NPCUPDATEDATA* lpNpcSaveData);
};

#endif // !defined(AFX_CASTLEDBSET_H__866EE47E_B854_4D47_8664_64C3026FFE59__INCLUDED_)
