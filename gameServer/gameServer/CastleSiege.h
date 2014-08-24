#ifndef CASTLESIEGE_H
#define CASTLESIEGE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#if (_GSCS==1)

using namespace std;

#include "protocol.h"

#define CS_MAX_UPGRADE 3

#define CASTLESIEGE_DATALOAD_1 1
#define CASTLESIEGE_DATALOAD_2 2
#define CASTLESIEGE_DATALOAD_3 3
#define CASTLESIEGE_DATALOAD_4 4

#define CASTLESIEGE_STATE_NONE -1
#define CASTLESIEGE_STATE_IDLE_1 0
#define CASTLESIEGE_STATE_REGSIEGE 1
#define CASTLESIEGE_STATE_IDLE_2 2
#define CASTLESIEGE_STATE_REGMARK 3
#define CASTLESIEGE_STATE_IDLE_3 4
#define CASTLESIEGE_STATE_NOTIFY 5
#define CASTLESIEGE_STATE_READYSIEGE 6
#define CASTLESIEGE_STATE_STARTSIEGE 7
#define CASTLESIEGE_STATE_ENDSIEGE 8
#define CASTLESIEGE_STATE_ENDCYCLE 9

#define CASTLESIEGE_LEFTTIME_NOTRUNNING -3
#define CASTLESIEGE_LEFTTIME_ENDSIEGE -2
#define CASTLESIEGE_LEFTTIME_ONSIGE -1

#define CASTLESIEGEREADY(state) (  ((state) == CASTLESIEGE_STATE_STARTSIEGE)?TRUE:FALSE)
#define CASTLESIEGECHECKSTATE(state) (((state) == CASTLESIEGE_STATE_STARTSIEGE)?TRUE:FALSE)

struct PMSG_SENDMINIMAPDATA
{
	BYTE btX;
	BYTE btY;
};

struct _CS_SCHEDULE_DATA
{
	_CS_SCHEDULE_DATA();
	virtual ~_CS_SCHEDULE_DATA();
	void Clear();

	BOOL m_bIN_USE;
	int m_iSTATE;
	int m_iADD_DAY;
	int m_iADD_HOUR;
	int m_iADD_MIN;
	int m_iGAP_SEC;
};

struct _CS_NPC_DATA
{
	_CS_NPC_DATA();
	virtual ~_CS_NPC_DATA();
	void Clear();
	void SetBaseValue();

	BOOL m_bIN_USE;

	int m_iNPC_NUM;
	int m_iNPC_INDEX;

	BOOL m_bNPC_DBSAVE;
	int m_iNPC_SIDE;
	int m_iNPC_OBJINDEX;
	int m_iNPC_DF_LEVEL;
	int m_iNPC_RG_LEVEL;
	int m_iNPC_MAXHP;
	int m_iNPC_HP;
	int m_iNPC_SX;
	int m_iNPC_SY;
	int m_iNPC_DX;
	int m_iNPC_DY;
	int m_iNPC_DIR;
	int m_iNPC_BASE_DF_LEVEL;
	int m_iNPC_BASE_RG_LEVEL;
	int m_iNPC_BASE_MAXHP;
	int m_iNPC_BASE_HP;
	int m_iNPC_BASE_SX;
	int m_iNPC_BASE_SY;
	int m_iNPC_BASE_DX;
	int m_iNPC_BASE_DY;
	int m_iNPC_BASE_DIR;
	int m_iCS_GATE_LEVER_INDEX;
	int m_iNPC_LIVE;
	int m_iNPC_AUTH;

	union
	{
		struct
		{
			BYTE m_btCsNpcExistVal1;
			BYTE m_btCsNpcExistVal2;
			BYTE m_btCsNpcExistVal3;
			BYTE m_btCsNpcExistVal4;
		};
		int m_iCsNpcExistVal;
	};
};

struct _CS_REG_GUILD_DATA
{
	_CS_REG_GUILD_DATA();
	virtual ~_CS_REG_GUILD_DATA();
	void Clear();

	BOOL m_bIN_USE;
	std::basic_string<char> m_strGuildName;
	int m_iRegMarkCount;
	int m_iGuildMemberCount;
	int m_iGuildMasterLevel;
	int m_iSeqNum;
	__int64 m_i64TotolScore;
};

struct _CS_TOTAL_GUILD_DATA
{
	_CS_TOTAL_GUILD_DATA();
	virtual ~_CS_TOTAL_GUILD_DATA();
	void Clear();

	BOOL m_bIN_USE;
	int m_iCsGuildID;
	BOOL m_bGuildInvolved;
	int m_iGuildScore; //season 2.5 add-on
};

struct _CS_MINIMAP_DATA
{
	_CS_MINIMAP_DATA();
	virtual ~_CS_MINIMAP_DATA();
	void Clear();

	int m_iGuildMasterIndex;
	char m_szGuildName[16];
	int m_iMiniMapPointCount;
	PMSG_SENDMINIMAPDATA m_stMiniMapPoint[1000];
};

struct PMSG_ANS_NOTIFYCSJOINSIDE
{
	PBMSG_HEAD2 h;
	BYTE btCsJoinSide;
};

struct CSP_REQ_NPCUPDATEDATA
{
	PWMSG_HEAD h;
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

struct CSP_REQ_NPCSAVEDATA
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

struct CSP_REQ_CSGUILDUNIONINFO
{
    PWMSG_HEAD h;
    WORD wMapSvrNum;
    int iCount;
};

struct PMSG_ANS_SENDMINIMAPDATA
{
    PWMSG_HEAD h;
    int iCount;
};

struct PMSG_ANS_SENDNPCMINIMAPDATA
{
    PWMSG_HEAD h;
    int iCount;
};

struct PMSG_SENDNPCMINIMAPDATA
{
    BYTE btNpcType;
    BYTE btX;
    BYTE btY;
};

struct PMSG_NPCDBLIST
{
    int iNpcNumber;
    int iNpcIndex;
    int iNpcDfLevel;
    int iNpcRgLevel;
    int iNpcMaxHp;
    int iNpcHp;
    BYTE btNpcX;
    BYTE btNpcY;
    BYTE btNpcLive;
};

struct CSP_REQ_CSSAVETOTALGUILDINFO
{
	PWMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_CSSAVETOTALGUILDINFO
{
	char szGuildName[8];
	int iCsGuildID;
	int iGuildInvolved;
	int iGuildScore; //season 2.5 add-on
};

struct PMSG_ANS_NOTIFY_CROWNSWITCH_INFO
{
	PBMSG_HEAD2 h;
	BYTE btIndex1;
	BYTE btIndex2;
	BYTE btSwitchState;
	BYTE btJoinSide;
	BYTE szGuildName[8];
	BYTE szUserName[10];
};

struct PMSG_ANS_NOTIFYCROWNSTATE
{
    PBMSG_HEAD2 h;
    BYTE btCrownState;
};


class CCastleSiege
{
public:
	CCastleSiege();
	virtual ~CCastleSiege();

public:
	void Clear();
	int Ready(int iMapSvrGroup);
	int LoadPreFixData(char * lpszFileName);
	int LoadData(char * lpszFileName);
	int DataRequest();
	int Init();
	void Run();
	static BYTE ScheduleStateCompFunc(_CS_SCHEDULE_DATA & A,_CS_SCHEDULE_DATA & B);
	int GetDataLoadState(){return m_iCastleDataLoadState;}
	void SetDataLoadState(int iDataLoadState){m_iCastleDataLoadState = iDataLoadState;}
	void SetDbDataLoadOK(BOOL bDbDataLoadOK){m_bDbDataLoadOK = bDbDataLoadOK;}
	int FirstCreateDbNPC();
	int SetCastleInitData(CSP_ANS_CSINITDATA * lpMsg);
	int SetCastleNpcData(CSP_CSINITDATA * lpMsg,int iCOUNT);
	int CheckSync();
	void GetNextDay(SYSTEMTIME * st, int iAfterDay, int iAfterHour, int iAfterMin, int iAfterSec);
	int GetStateGapSec(int iCastleSiegeState);
	void SetState(int iCastleSiegeState,BOOL bSetRemainMsec);
	void SetState_NONE(BOOL bSetRemainMsec);
	void SetState_IDLE_1(BOOL bSetRemainMsec);
	void SetState_REGSIEGE(BOOL bSetRemainMsec);
	void SetState_IDLE_2(BOOL bSetRemainMsec);
	void SetState_REGMARK(BOOL bSetRemainMsec);
	void SetState_IDLE_3(BOOL bSetRemainMsec);
	void SetState_NOTIFY(BOOL bSetRemainMsec);
	void SetState_READYSIEGE(BOOL bSetRemainMsec);
	void SetState_STARTSIEGE(BOOL bSetRemainMsec);
	void SetState_ENDSIEGE(BOOL bSetRemainMsec);
	void SetState_ENDCYCLE(BOOL bSetRemainMsec);
	void ProcState_NONE();
	void ProcState_IDLE_1();
	void ProcState_REGSIEGE();
	void ProcState_IDLE_2();
	void ProcState_REGMARK();
	void ProcState_IDLE_3();
	void ProcState_NOTIFY();
	void ProcState_READYSIEGE();
	void ProcState_STARTSIEGE();
	void ProcState_ENDSIEGE();
	void ProcState_ENDCYCLE();
	int GetCastleState(){return m_iCastleSiegeState;}
	SYSTEMTIME GetCastleLeftSiegeDate() {return m_tmLeftCastleSiegeDate;}
	int CalcCastleLeftSiegeDate();
	int GetCastleStateTerm(SYSTEMTIME * tmStateStartDate, SYSTEMTIME * tmStateEndDate);
	int GetCurRemainSec();
	void ClearDbNPC();
	void ClearNonDbNPC();
	void ClearNonDbNPC_MidWin();
	void ClearAllNPC();
	void CreateDbNPC();
	void CreateNonDbNPC(BOOL bDelFirst);
	int CheckAddDbNPC(int iIndex, int iNpcType, int iNpcIndex, BYTE & btResult);
	int AddDbNPC(int iNpcType, int iNpcIndex);
	int RepairDbNPC(int iNpcType, int iNpcIndex, int iNpcHP, int iNpcMaxHP);
	int PayForUpgradeDbNpc(int iIndex, int iNpcType, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex);
	int UpgradeDbNPC(int iIndex, int iNpcType, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex);
	int UpgradeDbNPC_DFLEVEL(int iNpcType, int iNpcIndex, int iNpcDfLevel);
	int UpgradeDbNPC_RGLEVEL(int iNpcType, int  iNpcIndex, int iNpcRgLevel);
	int UpgradeDbNPC_MAXHP(int iNpcType, int iNpcIndex, int iNpcMaxHP);
	void StoreDbNpc();
	int DelNPC(int iIndex, int iNpcType, int iMonsterExistVal, BOOL bDbSave);
	void SendNpcStateList(int iIndex, int iNpcType);
	int GetNpcData(int iNpcType, int iNpcIndex,_CS_NPC_DATA & pRetNpcData);
	void AdjustDbNpcLevel();
	void SendCsGateStateViewPort(int iGateIndex, BYTE btOperate);
	void CreateAllCsGateLever();
	int CreateCsGateLever(int iX, int iY);
	int LinkCsGateLever(int iLeverIndex, int iGateIndex);
	int CheckLeverAlive(int iLeverIndex);
	int CheckCsGateAlive(int iGateIndex);
	void CheckCsDbNpcAlive();
	void SetCrownIndex(int iIndex){m_iCrownIndex = iIndex;}
	void NotifyCrownState(BYTE btState);
	BOOL GetRegCrownAvailable(){return m_bRegCrownAvailable;}
	void SetRegCrownAvailable(BOOL bRegCrownAvailable){m_bRegCrownAvailable = bRegCrownAvailable;}
	int GetCastleTowerAccessable(){return m_bCastleTowerAccessable;}
	void SetCastleTowerAccessable(BOOL bTowerAccessable){m_bCastleTowerAccessable = bTowerAccessable;}
	int DelGemOfDefend(int iIndex, int iNeedGemOfDefend);
	void SetTaxRate(int iTaxType, int iTaxRate);
	int CheckCastleHasMoney(__int64 i64CastleMoney);
	__int64 GetCastleMoney() { return m_i64CastleMoney; }
	void SetCastleMoney(__int64 i64CastleMoney);
	void ResetCastleTaxInfo();
	void SetIsSiegeGuildList(BOOL bIsSiegGuildList){m_btIsSiegeGuildList = bIsSiegGuildList;}
	void SendCastleStateSync();
	BOOL GetDbNpcCreated(){return m_bDbNpcCreated;}
	void SetDbNpcCreated(BOOL bDbNpcCreated){m_bDbNpcCreated = bDbNpcCreated;}
	int CheckGuildOwnCastle(char * lpszGuildName);
	void ResetCastleCycle();
	void SetGateBlockState(int iX, int iY, int iGateState);
	char * GetCastleOwnerGuild(){return m_szCastleOwnerGuild;}
	BOOL GetHuntZoneEnter(){return m_bEnterHuntZone;}
	void SetHuntZoneEnter(BOOL bEnterHuntZone){m_bEnterHuntZone = bEnterHuntZone;}
	int GetTaxRateChaos();
	int GetTaxRateStore();
	int GetTaxHuntZone();
	void CheckBuildCsGuildInfo();
	void SetCalcRegGuildList(CSP_CALCREGGUILDLIST * lpMsg, int iCOUNT);
	void MakeCsTotalGuildInfo(CSP_CSGUILDUNIONINFO * lpMsg, int iCOUNT);
	void SaveCsTotalGuildInfo();
	void SetCsTotalGuildInfo(CSP_CSLOADTOTALGUILDINFO * lpMsg, int iCOUNT);
	int GetCsJoinSide(char * lpszGuildName, BYTE * btCsJoinSide, bool * bCsGuildInvolved);
	void SetAllUserCsJoinSide();
	void ResetAllUserCsJoinSide();
	void NotifySelfCsJoinSide(int iIndex);
	int OperateGate(int iNpcIndex, BOOL bOpenType);
	int OperateGate(int iObjIndex, int iMonsterExistVal, BOOL bOpenType);
	int CheckAttackGuildExist();
	int CheckCastleOwnerMember(int iIndex);
	int CheckCastleOwnerUnionMember(int iIndex);
	int CheckGuardianStatueExist();
	void ChangeWinnerGuild(int iCsJoinSide);
	int CheckMiddleWinnerGuild();
	int CheckCastleSiegeResult();
	int CheckUnionGuildMaster(int iIndex);
	void SetCrownUserIndex(int iIndex){this->m_iCastleCrownAccessUser = iIndex;}
	int GetCrownUserIndex(){return m_iCastleCrownAccessUser;}
	void ResetCrownUserIndex(){m_iCastleCrownAccessUser=-1;}
	int GetCrownAccessUserX(){return m_btCastleCrownAccessUserX;}
	int GetCrownAccessUserY(){return m_btCastleCrownAccessUserY;}
	void SetCrownAccessUserX(BYTE iX){m_btCastleCrownAccessUserX = iX;}
	void SetCrownAccessUserY(BYTE iY){m_btCastleCrownAccessUserY = iY;}
	void SetCrownSwitchUserIndex(int iMonsterClass, int iIndex);
	int GetCrownSwitchUserIndex(int iMonsterClass);
	void ResetCrownSwitchUserIndex(int iMonsterClass);
	void SetCrownAccessTickCount();
	void ResetCrownAccessTickCount();
	int CheckOverlapCsMarks(int iIndex);
	int GetCsAttkGuildList(PMSG_CSATTKGUILDLIST * lpMsgBody, int & iCount);
	int CheckTeleportMagicAxisY(int iStartY, int iTargetX, int iTargetY);
	void NotifyCsSelfLeftTime(int iIndex);
	void CheckReviveNonDbNPC();
	void CheckReviveGuardianStatue();
	void ReSpawnAllUser();
	void ReSpawnEnemyUser(int bRefreshOwnerUser);
	void NotifyAllUserCsStartState(BYTE btStartState);
	void NotifyAllUserCsProgState(BYTE btProgState, char * lpszGuildName);
	void ClearCastleTowerBarrier();
	void SetAllCastleGateState(BOOL bOpenType);
	void AddMiniMapDataReqUser(int iIndex);
	void DelMiniMapDataReqUser(int iIndex);
	void OperateMiniMapWork();
	void SendMapServerGroupMsg(char *  lpszMsg);
	void SendAllUserAnyData(LPBYTE lpMsg, int iSize);
	void SendAllUserAnyMsg(char *  lpMsg, int iSize);
	void SendCsUserAnyData(LPBYTE lpMsg, int iSize);
	void SendCsUserAnyMsg(char * lpszMsg);
	static BYTE RegGuildScoreCompFunc(_CS_REG_GUILD_DATA & A, _CS_REG_GUILD_DATA & B);
	void OperateGmCommand(int iIndex, int iGmCommand, void * lpParam);
	void CreateDbNPC_INS();
	void SavePcRoomUserList();
	DWORD GetCrownAccessTickCount(){return m_dwCrownAccessTime;}
	void NotifyCrownSwitchInfo(int iIndex);
	void GetStateDate(int iCastleSiegeState, SYSTEMTIME* st);

	BOOL m_bCastleSiegeEnable;
	BOOL m_bFileDataLoadOK;
	BOOL m_bDbDataLoadOK;
	BOOL m_bDoRun;

	int m_iMapSvrGroup;

	int m_iCastleDataLoadState;
	DWORD m_dwDbDataLoadTickCount;

	int m_iCastleSiegeCycle;
	int m_iCastleSiegeState;

	SYSTEMTIME m_tmSiegeEndSchedule;
	BOOL m_bFixCastleCycleStartDate;

	SYSTEMTIME m_tmFixCastleCycleStartDate;
	int m_iFixCastleSpecificState;

	BOOL m_bFixCastleStateStartDate;
	SYSTEMTIME m_tmFixCastleStateStartDate;

	int m_iCS_REMAIN_MSEC;
	int m_iCS_TICK_COUNT;

	SYSTEMTIME m_tmStateStartDate;
	SYSTEMTIME m_tmLeftCastleSiegeDate;

	DWORD m_dwNPC_DBSAVE_TICK_COUNT;
	DWORD m_dwCALC_LEFTSIEGE_TICK_COUNT;
	DWORD m_dwEVENT_MSG_TICK_COUNT;
	DWORD m_dwCHECK_GATE_ALIVE_COUNT;
	DWORD m_dwCS_JOINSIDE_REFRESH_TICK_COUNT;
	DWORD m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT;
	DWORD m_dwCS_MINIMAP_SEND_TICK_COUNT;
	DWORD m_dwCS_LEFTTIME_TICK_COUNT;
	DWORD m_dwCS_STARTTIME_TICK_COUNT;

	SYSTEMTIME m_tmStartDate;
	SYSTEMTIME m_tmEndDate;

	BYTE m_btIsSiegeGuildList;
	BYTE m_btIsSiegeEnded;
	BYTE m_btIsCastleOccupied;

	char m_szCastleOwnerGuild[16];

	__int64 m_i64CastleMoney;
	int m_iTaxRateChaos;
	int m_iTaxRateStore;
	int m_iTaxHuntZone;
	BOOL m_bEnterHuntZone;

	std::vector<_CS_SCHEDULE_DATA> m_vtScheduleData;
	CRITICAL_SECTION m_critScheduleData;

	std::vector<_CS_NPC_DATA> m_vtNpcData;
	CRITICAL_SECTION m_critNpcData;

	BOOL m_bDbNpcCreated;

	std::map<string,_CS_TOTAL_GUILD_DATA> m_mapCsBasicGuildInfo;
	CRITICAL_SECTION m_critCsBasicGuildInfo;

	std::map<std::string,_CS_TOTAL_GUILD_DATA> m_mapCsTotalGuildInfo;
	CRITICAL_SECTION m_critCsTotalGuildInfo;

	BOOL m_bCsBasicGuildInfoLoadOK;
	BOOL m_bCsTotalGuildInfoLoadOK;

	std::map<int,_CS_MINIMAP_DATA > m_mapMiniMapData;
	CRITICAL_SECTION m_critCsMiniMap;

	std::vector<int> m_vtMiniMapReqUser;

	BOOL m_bCastleTowerAccessable;
	BOOL m_bRegCrownAvailable;

	int m_iCrownIndex;
	int m_iCastleCrownAccessUser;

	BYTE m_btCastleCrownAccessUserX;
	BYTE m_btCastleCrownAccessUserY;

	int m_iCastleSwitchAccessUser1;
	int m_iCastleSwitchAccessUser2;

	DWORD m_dwCrownAccessTime;
	char m_szMiddleWinnerGuild[16];
}; 

extern CCastleSiege g_CastleSiege;

static int g_iNpcUpDfLevel_CGATE[CS_MAX_UPGRADE][0x3] = {
	1,	2,	3000000,
	2,	3,	3000000,
	3,	4,	3000000
};

static int g_iNpcUpDfLevel_CSTATUE[CS_MAX_UPGRADE][0x3] = {
	1,	3,	3000000,
	2,	5,	3000000,
	3,	7,	3000000,
};

//season4.5 changed
static int g_iNpcUpMaxHP_CGATE[0x3][CS_MAX_UPGRADE] = {
	2500000,	2,	1000000,
	3500000,	3,	1000000,
	5200000,	4,	1000000,
};

//season4.5 changed
static int g_iNpcUpMaxHP_CSTATUE[CS_MAX_UPGRADE][0x3] = {
	2200000,	3,	1000000,
	3400000,	5,	1000000,
	5000000,	7,	1000000
};

static int g_iNpcUpRgLevel_CSTATUE[CS_MAX_UPGRADE][0x3] = {
	1,	3,	5000000,
	2,	5,	5000000,
	3,	7,	5000000
};

//season4.5 changed??
static int g_iCsUserReSpawnArea[0x3][0x4] = {
	35,		17,		144,	48,
	74,		144,	115,	154,
	35,		17,		144,	48
};

static int g_iNpcDefense_CGATE[CS_MAX_UPGRADE + 1] = {
	100, 180, 300, 520
};
static int g_iNpcDefense_CSTATUE[CS_MAX_UPGRADE + 1] = {
	80, 180, 340, 550
};

static int g_iCastleWallStepAxisY[3] = {
	113, 160, 203
};

//#endif

#endif