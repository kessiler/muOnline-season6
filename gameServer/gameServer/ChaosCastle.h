#ifndef CHAOSCASTLE_H
#define CHAOSCASTLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BloodCastle.h"
#include "user.h"

#define MAX_CHAOSCASTLE_MONSTER	100

#define MAX_CHAOSCASTLE_USER	70

#define MAX_CHAOSCASTLE_LEVEL	7

#define MAX_CC_TRAP_STEP	4
#define MAX_CC_HOLLOW_ZONE	4

#define MAX_CC_MONSTERS	100

#define MAX_CC_MONSTER_DROP_ITEM	2

#define MIN_CC_USER_NEED_PLAY	2

#define CC_FLOOR_RANGE(floor) ( ((floor) < 0)?FALSE:((floor) >MAX_CHAOSCASTLE_LEVEL-1 )?FALSE:TRUE )
#define CC_SUB_FLOOR_RANGE(floor) ( ((floor) < 0)?FALSE:((floor) >MAX_CHAOSCASTLE_USER-1 )?FALSE:TRUE )
#define CC_TRAP_STEP_RANGE(floor) ( ((floor) < 0)?FALSE:((floor) >MAX_CC_TRAP_STEP-1 )?FALSE:TRUE )
#define CC_MAP_RANGE(value) ( ( value == MAP_INDEX_CHAOSCASTLE7 ) ? TRUE : ( value < MAP_INDEX_CHAOSCASTLE1 ) ? FALSE : ( value > MAP_INDEX_CHAOSCASTLE6) ? FALSE : TRUE )

#define CC_STATE_NONE		0
#define CC_STATE_CLOSED		1
#define CC_STATE_PLAYING	2
#define CC_STATE_PLAYEND	3

static struct _CHAOSCASTLE_LEVEL
{
	int iLOWER_BOUND;	// 0
	int iUPPER_BOUND;	// 4
	int iLOWER_BOUND_MAGUMSA;	// 8
	int iUPPER_BOUND_MAGUMSA;	// C

} g_sttCHAOSCASTLE_LEVEL[MAX_CHAOSCASTLE_LEVEL] =
{
	// Chaos Castle 1
	15, 49, // DK, DW, Elf
	15, 29, // MG, DL

	// Chaos Castle 2
	50, 119, // DK, DW, Elf
	30, 99, // MG, DL

	// Chaos Castle 3
	120, 179, // DK, DW, Elf
	100, 159, // MG, DL

	// Chaos Castle 4
	180, 239, // DK, DW, Elf
	160, 219, // MG, DL

	// Chaos Castle 5
	240, 299, // DK, DW, Elf
	220, 279, // MG, DL

	// Chaos Castle 6
	300, MAX_CHAR_LEVEL, // DK, DW, Elf
	280, MAX_CHAR_LEVEL, // MG, DL

	// Chaos Castle 7 - with master level from 1
	1, MAX_CHAR_LEVEL+MASTER_MAX_LEVEL-1, // DK, DW, Elf
	1, MAX_CHAR_LEVEL+MASTER_MAX_LEVEL-1 // MG, DL

	// Chaos Castle 7 - with master level from 400
	//MAX_CHAR_LEVEL, MAX_CHAR_LEVEL+MAX_MASTER_LEVEL-1, // DK, DW, Elf
	//MAX_CHAR_LEVEL, MAX_CHAR_LEVEL+MAX_MASTER_LEVEL-1 // MG, DL
};

static int g_iChaosCastle_BlowOutDistance[4][2]={3,4,3,4,2,3,0,1};
static int g_iChaosCastle_BlowOutDamage[4]={15, 15, 10, 5};

static int g_iChaosCastle_EnterCost[MAX_CHAOSCASTLE_LEVEL]={25000, 80000, 150000, 250000, 400000, 650000, 1000000};

static int g_iChaosCastle_GuardSetValue[MAX_CHAOSCASTLE_LEVEL]={2500, 2500, 2500, 2500, 2500, 2500, 2500};

static  int g_iChaosCastle_MonsterItems[MAX_CHAOSCASTLE_LEVEL][MAX_CC_MONSTER_DROP_ITEM][2] =
{ //season4.5 changed
	ITEMGET(14,13), 1, ITEMGET(14,14), 2,	// Chaos Castle 1
	ITEMGET(14,13), 1, ITEMGET(14,14), 3,	// Chaos Castle 2
	ITEMGET(14,13), 1, ITEMGET(14,14), 3,	// Chaos Castle 3
	ITEMGET(14,13), 2, ITEMGET(14,14), 2,	// Chaos Castle 4
	ITEMGET(14,13), 2, ITEMGET(14,14), 3,	// Chaos Castle 5
	ITEMGET(14,13), 2, ITEMGET(14,14), 3,	// Chaos Castle 6
	ITEMGET(14,13), 2, ITEMGET(14,14), 4,	// Chaos Castle 7
};

static  int g_iChaosCastle_MonsterItemsNewServer[MAX_CHAOSCASTLE_LEVEL][MAX_CC_MONSTER_DROP_ITEM][2] =
{ //season4.5 add-on
	NULL, NULL, ITEMGET(14,14), 2,	// Chaos Castle 1
	ITEMGET(14,13), 1, ITEMGET(14,14), 1,	// Chaos Castle 2
	ITEMGET(14,13), 1, ITEMGET(14,14), 2,	// Chaos Castle 3
	ITEMGET(14,13), 1, ITEMGET(14,14), 2,	// Chaos Castle 4
	ITEMGET(14,13), 2, ITEMGET(14,14), 1,	// Chaos Castle 5
	ITEMGET(14,13), 2, ITEMGET(14,14), 2,	// Chaos Castle 6
	ITEMGET(14,13), 2, ITEMGET(14,14), 3,	// Chaos Castle 7
};

static int g_iChaosCastle_GroundAxis[4] = { 23, 75, 44, 108};

static int g_iChaosCastle_DamageAxis[MAX_CC_TRAP_STEP][4] = 
{
	0x17, 0x4B,	0x2C, 0x6C,
	0x19, 0x4D,	0x2A, 0x6A,
	0x1B, 0x4F,	0x28, 0x68,
	0x1D, 0x51,	0x26, 0x66
};

static int g_iChaosCastle_MapHollowZone[MAX_CC_TRAP_STEP][MAX_CC_HOLLOW_ZONE][4] =
{
	0x17, 0x4B,	0x2C, 0x4C,
	0x2B, 0x4D,	0x2C, 0x6C,
	0x17, 0x6B,	0x2A, 0x6C,
	0x17, 0x4D,	0x18, 0x6A,
	0x17, 0x4B,	0x2C, 0x4C,
	0x2B, 0x4D,	0x2C, 0x6C,
	0x17, 0x6B,	0x2A, 0x6C,
	0x17, 0x4D,	0x18, 0x6A,
	0x19, 0x4D,	0x2A, 0x4E,
	0x29, 0x4F,	0x2A, 0x6A,
	0x19, 0x69,	0x28, 0x6A,
	0x19, 0x4F,	0x1A, 0x68,
	0x1B, 0x4F,	0x28, 0x50,
	0x27, 0x51,	0x28, 0x68,
	0x1B, 0x67,	0x26, 0x68,
	0x1B, 0x51,	0x1C, 0x66
};

static RECT g_rtPOINT_FRAME[MAX_CC_TRAP_STEP];
static RECT g_rtPOINT_TRAP[MAX_CC_TRAP_STEP];

struct _MONSTER_ITEM_DROP
{
	int m_iIndex;	// 0
	int m_iItemKind;	// 3

	_MONSTER_ITEM_DROP()	// line : 229
	{
		this->m_iIndex = -1;
		this->m_iItemKind = -1;
	};	// line : 232
};

struct CHAOSCASTLE_START_TIME
{
	int m_iHour;	// 0
	int m_iMinute;	// 4
};


struct _CHAOSCASTLE_USER
{
	_CHAOSCASTLE_USER()	// line : 338
	{
		this->Clear();
	};	// line : 340

	void Clear()	// line :342
	{
		this->m_iIndex = -1;
		this->m_iEXP = 0;
		this->m_iScore = 0;
		this->m_iUserState = 0;
		this->m_bSendQuitMsg = false;
	};	// line : 348

	int m_iIndex;	// 0
	int m_iEXP;	// 4
	int m_iScore;	// 8
	int m_iUserState;	// C
	bool m_bSendQuitMsg;	// 10
};

struct _CHAOSCASTLE_DATA
{
	int m_iCC_STATE;	// 0
	int m_iMapNumber;	// 4
	int m_iChaosCastleIndex;	// 8
	int m_iMISSION_SUCCESS;	// C
	int m_iCC_REMAIN_MSEC;	// 10
	int m_iCC_TICK_COUNT;	// 14
	int m_iCC_NOTIFY_COUNT;	// 18
	bool m_bCC_CAN_ENTER;	// 1C
	bool m_bCC_CAN_PARTY;	// 1D
	bool m_bCC_PLAY_START;	// 1E
	bool m_bCC_MSG_BEFORE_ENTER;	// 1F
	bool m_bCC_MSG_BEFORE_PLAY;	// 20
	bool m_bCC_MSG_BEFORE_END;	// 21
	bool m_bCC_MSG_BEFORE_QUIT;	// 22
	int m_iCC_ENTERED_USER;	// 24
	int m_iCC_FIRST_USER_COUNT;	//28
	int m_iCC_FIRST_MONSTER_COUNT;	// 2C
	long m_lCC_CURRENT_MONSTER_COUNT;	// 30
	int m_iCC_TRAP_STATUS;	// 34
	int m_iCC_TRAP_OLD_STATUS;	// 38
	_CHAOSCASTLE_USER m_UserData[MAX_CHAOSCASTLE_USER];	// 3C
	long m_lCC_MONSTER_COUNT[MAX_CHAOSCASTLE_MONSTER];	// 5B4
	long m_lFallUser[MAX_CHAOSCASTLE_USER];	// 744
	std::map<int,_MONSTER_ITEM_DROP> m_mapMonsterItemList;	// 85C
	float m_iCC_REWARD_EXP;

	void Clear()	// line : 387
	{
		this->m_iMISSION_SUCCESS = -1;
		this->m_iCC_NOTIFY_COUNT = -1;
		this->m_bCC_MSG_BEFORE_ENTER = false;
		this->m_bCC_MSG_BEFORE_PLAY = false;
		this->m_bCC_MSG_BEFORE_END = false;
		this->m_bCC_MSG_BEFORE_QUIT = false;
		this->m_bCC_CAN_ENTER = false;
		this->m_bCC_CAN_PARTY = false;
		this->m_bCC_PLAY_START = false;
		this->m_iCC_ENTERED_USER = 0;
		this->m_iCC_FIRST_USER_COUNT = 0;
		this->m_iCC_FIRST_MONSTER_COUNT = 0;
		this->m_lCC_CURRENT_MONSTER_COUNT = 0;
		this->m_iCC_TRAP_STATUS = 0;
		this->m_iCC_TRAP_OLD_STATUS = 0;

		for ( int iMON = 0;iMON < MAX_CHAOSCASTLE_MONSTER ; iMON++)
		{
			this->m_lCC_MONSTER_COUNT[iMON] = -1;
		}

		for ( int iFALL = 0; iFALL < MAX_CHAOSCASTLE_USER ; iFALL ++)
		{
			this->m_lFallUser[iFALL] = -1;
		}

		this->m_mapMonsterItemList.clear();
	};	// line : 408
};

#ifdef __CUSTOMS__
struct CHAOSCASTLE_REWARD
{
	int		AncientRate;
	BYTE	AncientCount;
	BYTE	ItemCount;
	WORD	ItemID;
	WORD	ItemIndex;
	WORD	ItemLevel;
	// ----
	void CHAOSCASTLE_REWARD::Clear()
	{
		this->AncientRate	= 0;
		this->AncientCount	= 0;
		this->ItemCount		= 0;
		this->ItemID		= -1;
		this->ItemIndex		= -1;
		this->ItemLevel		= 0;
	};
};
#endif

class CChaosCastle
{

public:

	CChaosCastle();
	virtual ~CChaosCastle();

	void Run();
	void Init(bool bEVENT_ENABLE);
	void Load(char* filename);
	void SetState(int iChaosCastleIndex, int iCC_STATE);
	int EnterUserChaosCastle(int iChaosCastleIndex, int iUserIndex);
	int LeaveUserChaosCastle(int iChaosCastleIndex, int iUserIndex);
	void AddFallUser(int iChaosCastleIndex, int iUserIndex);
	void ProcessFallUser(int iChaosCastleIndex);
	void SetUserState(int iUserIndex, int iState);
	int GetCurrentWinUser(int iChaosCastleIndex);
	int CalcSendRewardEXP(int iUserIndex, int iEXP, int iKILLCOUNT_USER, int iKILLCOUNT_MONSTER);
	int LevelUp(int iUserIndex, int iAddExp, int iEventType);
	int GetUserLevelToEnter(int iUserIndex);
	int CheckUserEnterMoney(int iUserIndex, int iEnterLevel);
	int PayUserEnterMoney(int iUserIndex, int iEnterLevel);
	void GiveUserDamage(int iUserIndex, int iDamage);
	void ClearMonster(int iChaosCastleIndex);
	int SetMonster(int iChaosCastleIndex);
	void AddMonsterList(int iChaosCastleIndex, int iMonsterIndex);
	void DelMonsterList(int iChaosCastleIndex, int iMonsterIndex);
	int GetMonsterListCount(int iChaosCastleIndex);
	void SetItemsToMonster(int iChaosCastleIndex);
	void SearchNDropMonsterItem(int iChaosCastleIndex, int iMonsterIndex, int iMaxHitUserIndex);
	void SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);
	void SendChaosCastleAnyMsg(LPBYTE lpMsg, int iSize, int iChaosCastleIndex);
	void SendNoticeMessage(int iChaosCastleIndex, LPSTR lpszMSG);
	void SendNoticeState(int iChaosCastleIndex, int iPlayState);
	void SendWinMessage(int iChaosCastleIndex, int iWinnerIndex);
	void SendFailMessage(int iChaosCastleIndex, int iLoserIndex);
	void SendAllLoserFailMessage(int iChaosCastleIndex, int iWinnerIndex);
	int GetCurrentState(int iChaosCastleIndex);
	int GetCurEnteredUser(int iChaosCastleIndex);
	int GetCurPlayUser(int iChaosCastleIndex);
	int GetRemainTime(int iChaosCastleIndex);
	int GetCurrentRemainSec(int iChaosCastleIndex);
	int CheckEnterLevel(int iIndex, int iLevel);
	int CheckEnterItem(int iIndex);
	int CheckPlayEnded(int iChaosCastleIndex);
	bool CheckCanEnter(int iChaosCastleIndex);
	bool CheckPlayStart(int iChaosCastleIndex);
	bool CheckCanStartPlay(int iChaosCastleIndex);
	int ObjSetPosition(int iIndex, int iX, int iY);
	void SearchNBlowObjs(int iMapNumber, int iX, int iY);
	int BlowObjsFromPoint(int iIndex, int iMapNumber, int& iX, int& iY);
	int BlowObjsToPoint(int iIndex, int iMapNumber, int iX, int iY);
	int CalDistance(int iX1, int iY1, int iX2, int iY2);
	void SafetyCastleZone(int iChaosCastleIndex);
	void UnSafetyCastleZone(int iChaosCastleIndex);
	void SendCastleZoneSafetyInfo(int iChaosCastleIndex, bool bDoSet);
	void CheckUserInDieTile(int iChaosCastleIndex);
	void CheckMonsterInDieTile(int iChaosCastleIndex);
	void PlayFailedRollBack(int iChaosCastleIndex);
	int RewardZettoMoney(int iChaosCastleIndex);
	int ReCalcUserCount(int iChaosCastleIndex);
	void RewardUserEXP(int iChaosCastleIndex, int iChaosCastleSubIndex, BOOL bWinner);
	void ProcessTrapStatus(int iChaosCastleIndex);
	void CalUsersInTrap(int iChaosCastleIndex);
	int CheckWearingMOPH(int iUserIndex);
	void GiveWinnerItem(int iChaosCastleIndex, int iWinnerIndex);
	void PullObjInnerPlace(int iChaosCastleIndex, int iTRAP_STEP);
	void SetMapAttrHollow(int iChaosCastleIndex, int iTRAP_STEP);
	void SetMapAttrFill(int iChaosCastleIndex);
	void CheckRegisterOfflineGift(int iUserIndex);
	int GetChaosCastleMapNumber(int iChaosCastleIndex);
	int GetChaosCastleIndex(int iMAP_NUM);

protected:

	void CheckSync(int iChaosCastleIndex);
	void ClearChaosCastleData(int iChaosCastleIndex);
	void ProcState_None(int iChaosCastleIndex);
	void ProcState_Closed(int iChaosCastleIndex);
	void ProcState_Playing(int iChaosCastleIndex);
	void ProcState_PlayEnd(int iChaosCastleIndex);
	void SetState_None(int iChaosCastleIndex);
	void SetState_Closed(int iChaosCastleIndex);
	void SetState_Playing(int iChaosCastleIndex);
	void SetState_PlayEnd(int iChaosCastleIndex);

private:
	int m_MinPlayersCount;		//-> Custom
	BOOL m_bCC_EVENT_ENABLE;	// 4
	int m_iCC_TIME_MIN_OPEN;	// 8
	int m_iCC_TIME_MIN_PLAY;	// C
	int m_iCC_TIME_MIN_REST;	// 10
	int m_iCC_NORMAL_ITEM_DROP;	// 14
	int m_iCC_EXCEL_ITEM_DROP;	// 18
	int m_iCC_MONSTER_REGEN;	// 1C
	std::vector<CHAOSCASTLE_START_TIME> m_vtChaosCastleOpenTime;	// 20
	_CHAOSCASTLE_DATA m_stChaosCastleData[MAX_CHAOSCASTLE_LEVEL];	// 30
#ifdef __CUSTOMS__
	CHAOSCASTLE_REWARD m_RewardData[MAX_CHAOSCASTLE_LEVEL];
#endif

};

extern CChaosCastle g_ChaosCastle;

#endif