// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once

#include "ImperialGuardianRewardExp.h"

typedef struct _stMonsterIndexInfo
{
	int ObjIndex;
	bool bLive;
	bool bState;
}stMonsterIndexInfo;

typedef struct _stZoneInfo
{
  int m_State;
  int m_nPartyNumber;
  char ukn08h;
  bool m_bTimeAttack;
  bool m_bLootTime;
  bool m_bWaitPlayer;
  bool ukn0Ch;
  char ukn0Dh;
  bool ukn0Eh;
  char ukn0Fh;
  __int16 m_nMonsterRegenTableIndex;
  int m_nMaxUserLevel;
  int ukn18h;
  std::vector<stMonsterIndexInfo> vtMonsterInfo;
  std::vector<int> vtPlayers;
  std::vector<int> vtUkn3Ch;
  int m_TimeAttack_RemainMSec;
  int m_LootTime_MSec;
  int m_WaitPlayer_MSec;
  int m_WaitPlayer_TickCount;
  int m_LootTime_TickCount;
  int m_TimeAttack_TickCount;
  std::vector<int> vtGateInfo;

		int m_nMaxUserReset;
}stZoneInfo;


struct OBJECTSRUCT;

class CImperialGuardian
{
public:

	static const int MAX_EVENTZONE = 4;
	inline static bool IsGoodZone(int nZoneIndex) { return (nZoneIndex >= 0 && nZoneIndex < MAX_EVENTZONE);};
	inline static bool IsEventMap(int MapNumber) { return (MapNumber >= 69 && MapNumber <= 72); }

	enum IG_STATE
	{ 
		STATE_INVALID = -1,
		STATE_READY = 0, 
		STATE_TIMEATTACK = 1, 
		STATE_WAITPLAYER = 2,
		STATE_LOOTTIME = 3, 
		STATE_WARPNEXTZONE = 4,
		STATE_MISSIONFAIL = 5,
		STATE_ALLKICK = 6,
		STATE_MISSIONCLEAR = 7,
		STATE_CHECKDUNGEON = 8,
	};

	CImperialGuardian(void);
	virtual ~CImperialGuardian(void);

	bool IsEnableEvent() { return this->m_bEventOn; }

	void Init();
	void InitEventDungeon();
	void LoadScript(LPCSTR lpFileName);

	void Run();
	void ProcReady(int nZoneIndex);
	void ProcBeginTimeAttack(int nZoneIndex);
	void ProcBeginLootTime(int nZoneIndex);
	void ProcBeginWaitPlayer(int nZoneIndex);
	void ProcAllWarpNextZone(int nZoneIndex);
	void ProcAllKick(int nZoneIndex);
	void ProcMissionFail(int nZoneIndex);
	void ProcMissionClear(int nZoneIndex);

	bool IsEmptyZone(int nZoneIndex);
	void KickInvalidUser();
	bool CheckValidUser(int nUserNumber);
	int GetZoneState(int nZoneIndex);
	bool SetZoneState(int nZoneIndex, int nState);

	void CGEnterPortal(int nUserIndex, int nDestZoneIndex);
	void RegenMonster(int nZoneIndex, int nMonsterRegenTableIndex, int nMaxUserLevel, int nMaxUserReset, char bOnlyCreateGate);
	void DeleteMonster(int nZoneIndex, int nMonsterRegenTableIndex);
	void InitZone(int nZoneIndex);
	bool SetMonster(int nIndex, int MonsterClass, int nMaxLevel, int nMaxUserReset);

	int GetGateNumber(int nZoneIndex, int nDayOfWeek);
	int GetMapNumber(int nDayOfWeek);
	void ProcCheckDungeon(int nZoneIndex);
	int GetLiveMonsterCount(int nZoneIndex);
	void CheckLiveMonster(int nZoneIndex);
	int GetCurrentZoneIndex(int nUserNumber);
	bool AddUserInZone(int nZoneIndex, int nUserNumber);
	void RemoveUserInZone(int nZoneIndex, int nUserNumber);
	void RemoveUserInAllZone(int nUserNumber);
	int GetPlayerCount(int nZoneIndex);
	int GetTotalPlayerCount();
	bool IsLastZone(int nZoneIndex);

	bool GCNotifyZoneClear(int nZoneIndex);
	bool GCNotifyAllZoneClear(int nZoneIndex);
	void GCMissionFail(int nZoneIndex);
	void GCMissionFailUseDie(int nUserIndex);

	void GCSendDataToUser(int nIndex, LPBYTE lpMsg, DWORD nSize);
	bool GCNotifyRemainTickCount(int nZoneIndex, BYTE btMsgType, DWORD dwRemainTick);
	void SetGateBlockState(int nMapNumber, int nZoneIndex, signed int nX, signed int nY, int nGateState, int nDir);

	void GCSendCastleGateInfo(int nGateIndex, int nZoneIndex, int nUserIndex);
	void DropItem(int nIndex, int nMonsterIndex);
	int CheckOverlapMysteriousPaper(int nIndex, int nItemLevel);
	void RollBackUserPos(int nUserNumber);
	int CheckGaionOrderPaper(int nIndex);
	int CheckFullSecromicon(int nIndex);
	bool IsAttackAbleMonster(int nMonsterIndex);
	void SetAtackAbleState(int nZoneIndex, int nMonsterClass, bool bState);

	void SetDayOfWeekGM(int nDayOfWeeks);
	void SetCheatModeGM(int nCheatMode);
	void WarpZoneGM(int nUserIndex, int nZoneIndex);
	stZoneInfo GetZoneInfo(int nZoneIndex);
	void SetTargetMoveAllMonster(int nZoneIndex, int nTargetNumber);
	void DestroyGate(int nZoneIndex, int nIndex, int nTargetIndex);
	void GCSendServerMsgAll(int nZoneIndex, char *szMsg);
	int ImperialGuardianLevelUp(int iIndex, int iAddExp);
	void RegAllPartyUser(int nZoneIndex, int nFirstEnterUserIndex);
	bool IsRegPartyUser(int nZoneIndex, int nUserIndex);
	
	void UserMonsterCountCheck();
	void MonsterBaseAct(struct OBJECTSTRUCT *lpObj);
	bool ChangeUserIndex(int nUserIndex, int nCurrentUserIndex, int nZoneIndex);
	bool SendCurStateToUser(int nUserIndex, int nZoneIndex);
	bool SendCurGateInfoToUser(int nUserIndex, int nZoneIndex);

	int GetPlayUserCountRightNow(int nZoneIndex);
	int GetMixNeedZen() { return MixNeedZen; }
private:

	__int16 m_CheatModeGM;
	int m_nDayOfWeek;
	int ukn0Ch;
	bool m_bScriptLoad;
	bool m_bEventOn;
	bool m_bPromotionOn;
	char ukn13h;
	_stZoneInfo m_ZoneInfo[MAX_EVENTZONE];
	__int16 m_DayOfWeekGM;

	DWORD m_LootTime;
	DWORD m_WaitPlayerTime;
	DWORD TimeAttackTime;

	int MixNeedZen;
	CImperialGuardianRewardExp m_RewardExpInfo;

	CRITICAL_SECTION m_cs;
};

extern CImperialGuardian g_ImperialGuardian;