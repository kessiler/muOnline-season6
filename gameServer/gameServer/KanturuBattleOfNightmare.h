// KanturuBattleOfNightmare.h: interface for the CKanturuBattleOfNightmare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLEOFNIGHTMARE_H__3AA73932_6DB3_404B_92CE_9AD80A1B5DE7__INCLUDED_)
#define AFX_KANTURUBATTLEOFNIGHTMARE_H__3AA73932_6DB3_404B_92CE_9AD80A1B5DE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"

#define KANTURU_NIGHTMARE_STATE_INFO	6

// States
#define KANTURU_NIGHTMARE_NONE		0
#define KANTURU_NIGHTMARE_IDLE		1
#define KANTURU_NIGHTMARE_NOTIFY	2
#define KANTURU_NIGHTMARE_START		3
#define KANTURU_NIGHTMARE_END		4
#define KANTURU_NIGHTMARE_ENDCYCLE	5

class CKanturuBattleOfNightmare
{

public:

	CKanturuBattleOfNightmare();
	virtual ~CKanturuBattleOfNightmare();

	int LoadData(LPSTR lpszFileName);
	void ResetAllData();
	void Run();
	void SetState(int iBattleOfNightmareState);
	void SetNextState(int iCurrentState);
	void SetState_NONE();
	void SetState_IDLE();
	void SetState_NOTIFY();
	void SetState_START();
	void SetState_END();
	void SetState_ENDCYCLE();
	void ProcState_NONE();
	void ProcState_IDLE();
	void ProcState_NOTIFY();
	void ProcState_START();
	void ProcState_END();
	void ProcState_ENDCYCLE();
	void SetBattleOfNightmareState(int iBattleOfNightmareState){this->m_iBattleOfNightmareState = iBattleOfNightmareState;}
	int GetBattleOfNightmareState(){return this->m_iBattleOfNightmareState;}
	void SetSuccessValue(BOOL bIsSuccess){this->m_bIsSucccess = bIsSuccess;}
	BOOL GetSuccessValue(){return this->m_bIsSucccess;}
	void CheckStateTime();
	int GetRemainTime();
	void CheckNightmareBattleUser();

private:

	int m_iBattleOfNightmareState;	// 4
	BOOL m_bIsSucccess;	// 8
	BOOL m_bFileDataLoad;	// C
	CKanturuStateInfo m_StateInfo[KANTURU_NIGHTMARE_STATE_INFO];	// 10
	int m_StateInfoCount;	// 88
	int m_iNightmareAIGroupNumber;	// 8C
	int m_iNightmareObjIndex;	// 90
};

#endif // !defined(AFX_KANTURUBATTLEOFNIGHTMARE_H__3AA73932_6DB3_404B_92CE_9AD80A1B5DE7__INCLUDED_)
