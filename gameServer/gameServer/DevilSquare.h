#ifndef DEVILSQUARE_H
#define DEVILSQUARE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DevilSquareGround.h"
#include "user.h"
#include "..\common\classdef.h"
#include <list>
#include "MasterLevelSystem.h"

enum eDevilSquareState
{
	DevilSquare_CLOSE = 0x0,
	DevilSquare_OPEN = 0x1,
	DevilSquare_PLAYING = 0x2,
};

#define MAX_DEVILSQUARE_USER	15


#define DS_MAP_RANGE(map) ( ( ((map)) == MAP_INDEX_DEVILSQUARE  )?TRUE:( ((map)) == MAP_INDEX_DEVILSQUARE2  )?TRUE:FALSE  )
#define DS_LEVEL_RANGE(level) ( ( ((level)) < 0  )?FALSE:( ((level)) > MAX_DEVILSQUARE_GROUND-1  )?FALSE:TRUE)

struct DEVILSQUARE_START_TIME
{
	int m_iHour;	// 0
	int m_iMinute;	// 4
};

struct GROUND_DATA
{
	int m_BonusScoreTable[MAX_TYPE_PLAYER];
	float m_RewardExp;
};
struct BonusTable
{
	int m_0;
	int m_1;
	int m_2;
	int m_3;
	int m_4;
	float m_5;
};

class CDevilSquare
{

public:

	CDevilSquare();
	virtual ~CDevilSquare();

	void Init();
	void SetState(enum eDevilSquareState eState);
	enum eDevilSquareState GetState(){return this->m_eState;};
	int Run();
	int CheckCloseTime();
	int GetRemainTime(){return this->m_iRemainTime;}
	void SetMonster();
	void ClearMonstr();
	void gDevilSquareMonsterRegen(LPOBJ lpObj);
	void DieProcDevilSquare(LPOBJ lpObj);
	int gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg);
	void gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, int MSBFlag);
	void gObjMonsterScoreDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag);
	void Load(LPSTR filename);
	void CalcScore();
	void SetClose();
	int GetDevilSquareIndex(int iGateNumber);
	int GetUserLevelToEnter(int iUserIndex, int& iMoveGate);
	// ----
	int DelUser(int nDevilSquareIndex, int UserIndex);	//1.01.00
	int LeaveDevilSquare(int nDevilSquareIndex, int UserIndex);	//1.01.00
	// ----
private:

	void SetOpen();
	void SetPlaying();
	void ProcClose();
	void ProcOpen();
	void ProcPlaying();
	void SendEventStartMsg();
	void ItemClear();
	void CheckSync();
	void SendLiveCall();
	void gObjScoreClear();
	void CheckInvalidUser();

public:
	enum eDevilSquareState m_eState;	// 4
	DWORD m_iTime;	// 8
	int m_iRemainTime;	// C
	int m_iremainTimeSec;	// 10
	BOOL m_bSendTimeCount;	// 14
	BOOL m_bQuit;	// 18
	BOOL m_bFlag;	// 1C
	UINT m_hThread;	// 20
	std::list<DEVILSQUARE_START_TIME> m_listDevilSquareOpenTime;	// 24
	int m_iCloseTime;	// 24 -> 30
	int m_iOpenTime;	// 28 -> 34
	int m_iPlaytime;	// 2C -> 38

	CDevilSquareGround m_DevilSquareGround[MAX_DEVILSQUARE_GROUND];	// 30 -> 3C
	int m_BonusScoreTable[MAX_TYPE_PLAYER][MAX_DEVILSQUARE_GROUND];	// 1BA8 -> 1BB4
};



extern CDevilSquare g_DevilSquare;

struct EVENT_LEVEL_LIMITATION_EX
{
	int NormalCharacterMinLevel; // Dark Knight, Dark Wizard, Elf
	int NormalCharacterMaxLevel; // Dark Knight, Dark Wizard, Elf
	int SpecialCharacterMinLevel; // Magic Gladiator, Dark Lord
	int SpecialCharacterMaxLevel; // Magic Gladitor, Dark Lord
	int MoveGate;	// Dest Gate
};

static EVENT_LEVEL_LIMITATION_EX g_sttDEVILSQUARE_LEVEL[MAX_DEVILSQUARE_GROUND] =
{
	// Devil Square 1
	15, 130, // DK, DW, Elf
	10, 110, // MG, DL
	58,	// Gate

	// Devil Square 2
	131, 180, // DK, DW, Elf
	111, 160, // MG, DL
	59,	// Gate

	// Devil Square 3
	181, 230, // DK, DW, Elf
	161, 210, // MG, DL
	60,	// Gate

	// Devil Square 4
	231, 280, // DK, DW, Elf
	211, 260, // MG, DL
	61,	// Gate

	// Devil Square 5
	281, 330, // DK, DW, Elf
	261, 310, // MG, DL
	111,	// Gate

	// Devil Square 6
	331, MAX_CHAR_LEVEL, // DK, DW, Elf
	311, MAX_CHAR_LEVEL, // MG, DL
	112,	// Gate

	// Devil Square 6 - with master level from 1
	1, MAX_CHAR_LEVEL+MASTER_MAX_LEVEL-1, // DK, DW, Elf
	1, MAX_CHAR_LEVEL+MASTER_MAX_LEVEL-1, // MG, DL
	270		// Gate

	// Devil Square 6 - with master level from 400
	//MAX_CHAR_LEVEL, MAX_CHAR_LEVEL+MAX_MASTER_LEVEL-1, // DK, DW, Elf
	//MAX_CHAR_LEVEL, MAX_CHAR_LEVEL+MAX_MASTER_LEVEL-1, // MG, DL
	//270		// Gate
};


unsigned __stdcall DevilSquareThreadFunc(LPVOID p);
void DevilSquareProtocolCore(BYTE protoNum, LPBYTE aRecv, int  aLen);
void DataSendRank(char* pMsg, int size);

#endif