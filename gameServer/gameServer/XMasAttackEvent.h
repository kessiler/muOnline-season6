#ifndef XMASATTACKEVENT_H
#define XMASATTACKEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMasAttackEventData.h"

enum ATTACKEVENT_STATE
{
	ATTACKEVENT_STATE_NONE = 0,
	ATTACKEVENT_STATE_CLOSED = 1,
	ATTACKEVENT_STATE_PLAYING = 2,
};

struct XMASMONSTER_MAP_INFO
{
	BYTE m_btMapNumber;	// 0
	int m_iMinX;	// 4
	int m_iMinY;	// 8
	int m_iMaxX;	// C
	int m_iMaxY;	// 10
};

struct PMSG_ANS_CL_EFFECT
{
	PBMSG_HEAD h;
	WORD wEffectNum;	// 4
};

class CXMasAttackEvent : public CXMasAttackEventData
{

public:

	CXMasAttackEvent();
	virtual ~CXMasAttackEvent();

	virtual int Load(char* lpszFileName);	// 4
	virtual void StartEvent(); //8
	virtual void StopEvent(); //C
	virtual void End_Menual(); //10
	virtual void SetMenualStart(BOOL bMenualStart); //14
	virtual void Start_Menual(); //18

	void EnableEvent(BOOL bDoEvent);

	void SetState(int nSate);

	void ProcState_None();
	void ProcState_Closed();

	void SendMessage(LPSTR lpMsg, int Effect);
	
	void ProcState_Playing();

	void SetState_None();
	void SetState_Closed();

	void SetState_Playing();

	void Move();

	void EventLog(LPSTR Msg, int MapNumber, BYTE cX, BYTE cY);
	void CreateMonsterHerd();

	void AddMonsterToHerd(CXMasMonsterHerd * vtHerdInfo);
	void ClearMonsterHerd();

	BOOL MonsterHerdMoveCheck(CXMasMonsterHerd * vtHerdInfo, XMASMONSTER_MAP_INFO * MonsterMapInfo);
	void Run();
	void CheckSync();

private:
	std::vector<XMASMONSTER_MAP_INFO> m_vtXMasMonsterMapInfo; //5C
	int m_nEvent_State;	//6C
	BOOL m_bMonsterToDest;	//70
	int m_iMOVE_RAND_SIZE;	//74
	int m_iRADIUS_MIN;	//78
	int m_iRADIUS_MAX;	//7C
	int m_iTIME_MIN_OPEN;	//80
	int m_iTIME_MIN_PLAY;	//84
	int m_iTIME_MONSTER_TO_DEST;	//88
	int m_iTIME_NOTIFY_COUNT;	//8C
	int m_iUnk90;
};



#endif