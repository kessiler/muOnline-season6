#ifndef RINGATTACKEVENT_H
#define RINGATTACKEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MonsterHerd.h"

#define MAX_RINGMONSTER_MAP	3


class CRingMonsterHerd : public MonsterHerd
{

public:

	CRingMonsterHerd();
	virtual ~CRingMonsterHerd();

	void SendEventGiftWinner(int iIndex, int iGiftKind);
	BYTE GetX(){return this->m_iCUR_X;}
	BYTE GetY(){return this->m_iCUR_Y;}

	virtual int Start();	// 4
	virtual int MonsterHerdItemDrop(LPOBJ lpObj);	// 8
	virtual void MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj);	// C

private:

	BOOL m_bEventSuccess;	// 48

};

//*****************************************************

struct RINGMONSTER_DATA
{
	int m_iType;	// 0
	BOOL m_bDoRegen;	// 4
	BOOL m_bDoAttackFirst;	// 8
};

struct RINGMONSTER_EVENT_TIME
{
	int m_iHour;	// 0
	int m_iMinute;	// 4
};

class CRingAttackEvent
{

public:

	CRingAttackEvent();
	virtual ~CRingAttackEvent();

	void StartEvent();
	void StopEvent();
	void EnableEvent(BOOL bDoEvent);
	void Run();
	void CheckSync();
	void SetState(int iSTATE);
	void ProcState_None();
	void ProcState_Closed();
	void ProcState_Playing();
	void SetState_None();
	void SetState_Closed();
	void SetState_Playing();
	void Move();
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;}
	void Start_Menual();
	void End_Menual();

	virtual int Load(char* lpszFileName);	// 4

private:

	CRingMonsterHerd m_RingMonsterHerd[MAX_RINGMONSTER_MAP];	// 4
	BOOL m_bDoEvent;	// E8
	BOOL m_bHasData;	// EC
	BOOL m_bMonsterToDest;	// F0
	int m_iTIME_MSEC_REMAIN;	// F4
	int m_iTIME_MIN_OPEN;	// F8
	int m_iTIME_MIN_PLAY;	// FC
	int m_iTIME_MONSTER_TO_DEST;	// 100
	int m_iTIME_TICK_COUNT;	// 104
	int m_iTIME_NOTIFY_COUNT;	// 108
	int m_iEVENT_STATE;	// 10C
	int m_iMOVE_RAND_SIZE;	// 110
	int m_iRADIUS_MIN;	// 114
	int m_iRADIUS_MAX;	// 118
	std::vector<RINGMONSTER_DATA> m_vtMonsterAddData;	// 11C
	CRITICAL_SECTION m_critMonsterAddData;	// 12C
	std::vector<RINGMONSTER_EVENT_TIME> m_vtEventTime;	// 144
	BOOL m_bMenualStart;	// 154

};



#endif