#ifndef DEVILSQUAREGROUND_H
#define DEVILSQUAREGROUND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#define MAX_DEVILSQUARE_GROUND	7
#define MAX_ST_DEVILSQUARE_MONSTER	15

#define DEVIL_SQUARE_GROUND_1	0
#define DEVIL_SQUARE_GROUND_2	1
#define DEVIL_SQUARE_GROUND_3	2
#define DEVIL_SQUARE_GROUND_4	3
#define DEVIL_SQUARE_GROUND_5	4
#define DEVIL_SQUARE_GROUND_6	5
#define DEVIL_SQUARE_GROUND_7	6

struct PMSG_ANS_EVENTUSERSCORE
{
	PBMSG_HEAD h;	// C1:01
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int Score;	// 1C
	int Class;	// 20
	int SquareNum;	// 24
};

struct DevilSquareMonsterInfo
{
	WORD m_Type;	// 0
	int m_RegenStartTime;	// 4
	int m_RegenEndTime;	// 8
	int m_iDevilSquareNumber;	// C
};

struct DevilSquareBossMonsterInfo
{
	WORD m_Type;	// 0
	int m_RegenStartTime;	// 4
	int m_RegenEndTime;	// 8
	int X;	// C
	int Y;	// 10
	int TX;	// 14
	int TY;	// 18
	int m_iDevilSquareNumber;	// 1C
};

struct DevilSquareScoreInfo
{
	char Name[10];	// 0
	int TotalScore;	// C
	int BonusExp;	// 10
	int BonusZen;	// 14
};

#pragma pack(1)
struct PMSG_DEVILSQUARERESULT
{
	PBMSG_HEAD h;	// C1:93
	BYTE MyRank;	// 3
	BYTE Count;	// 4
	DevilSquareScoreInfo Score[11];	// 5
};
#pragma pack()

class CDevilSquareGround	// size 0x494
{

public:

	CDevilSquareGround();
	virtual ~CDevilSquareGround();

	void Init(int Index);
	void Clear();
#ifdef __CUSTOMS__
	void SetBonus(int index, int zen, int exp, BYTE	ItemCount, WORD ItemID, BYTE ItemLevel);
#else
	void SetBonus(int index, int zen, int exp);
#endif
	void IncExp(float exp);
	void Set(WORD type, int starttime, int endtime);
	void SetBoss(WORD type, int starttime, int x,  int y,  int tx, int ty);
	void IncObjCount();
	long GetObjCount();
	void RegenBossMonster(int currtime);
	int FindMonsterType(WORD type);
	WORD GetMonsterType(int currtime);
	void SendRankingInfo(LPOBJ lpObj);
	void ClearScore();
	void SortScore();
	void InsertObj(LPOBJ lpObj);
	void SendScore();
	// ----
	int DelUser(int sUserIndex);	//1.01.00

private:

	DevilSquareMonsterInfo m_DevilSquareMonsterInfo[MAX_ST_DEVILSQUARE_MONSTER];	// 4
	DevilSquareBossMonsterInfo m_DevilSquareBossMonsterInfo[MAX_ST_DEVILSQUARE_MONSTER];	// F4
	std::vector<OBJECTSTRUCT *> m_DevilSquareRankList;	// 2D4
	PMSG_DEVILSQUARERESULT m_DevilSquareScoreInfoTOP10;	// 2E4
	int m_iIndex;	// 3F4
#ifdef __CUSTOMS__
	int m_Bonus[4][5];	// 3F8
#else
	int m_Bonus[4][2];	// 3F8
#endif
	long m_dwObjCount;	// 418
	int m_BonusScoreTable[5][MAX_DEVILSQUARE_GROUND];	// 41C
};

#endif