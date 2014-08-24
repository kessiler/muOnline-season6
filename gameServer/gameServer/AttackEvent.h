// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef ATTACKEVENT_H
#define ATTACKEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_ATTACK_EVENT_CREATE_MAP 2

class CAttackEvent
{
public:

	CAttackEvent();
	virtual ~CAttackEvent();
	void Init();
	void Start();
	void End();
	void Run();
	void Active();
	void Start_Menual();
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;};	// line : 39
	void End_Menual();

private:

	bool EventState;	// 4
	DWORD EventStartTime;	// 8
	BYTE m_MapNumber;	// C
	int m_BossMap55;	// 10
	int m_BossMapX55;	// 14
	int m_BossMapY55;	// 18
	int m_BossMap53;	// 1C
	int m_BossMapX53;	// 20
	int m_BossMapY53;	// 24
	BOOL m_bMenualStart;	// 28

};


#endif