// KanturuBattleStanby.h: interface for the CKanturuBattleStanby class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLESTANBY_H__79AE8375_169A_480E_92BF_1F37EF8BA77B__INCLUDED_)
#define AFX_KANTURUBATTLESTANBY_H__79AE8375_169A_480E_92BF_1F37EF8BA77B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "KanturuStateInfo.h"


#define KANTURU_STANBY_STATE_INFO	4

// States
#define KANTURU_STANBY_NONE		0
#define KANTURU_STANBY_START	1
#define KANTURU_STANBY_NOTIFY	2
#define KANTURU_STANBY_END		3

class CKanturuBattleStanby
{

public:

	CKanturuBattleStanby();
	virtual ~CKanturuBattleStanby();

	void Init();
	int LoadData(LPSTR lpszFileName);
	void ResetAllData();
	void Run();
	void SetState(int iBattleStanbyState);
	void SetNextState(int iCurrentState);
	void SetState_NONE();
	void SetState_START();
	void SetState_NOTIFY();
	void SetState_END();
	void ProcState_NONE();
	void ProcState_START();
	void ProcState_NOTIFY();
	void ProcState_END();
	void SetBattleStanbyState(int iBattleStanbyState){this->m_iBattleStanbyState = iBattleStanbyState;}
	int GetBattleStanbyState(){return this->m_iBattleStanbyState;}
	void CheckStateTime();
	int GetRemainTime();

private:

	int m_iBattleStanbyState;	// 4
	BOOL m_bIsSucccess;	// 8
	BOOL m_bFileDataLoad;	// C
	CKanturuStateInfo m_StateInfo[KANTURU_STANBY_STATE_INFO];	// 10
	int m_StateInfoCount;	// 60
};
// <size 0x64>

#endif // !defined(AFX_KANTURUBATTLESTANBY_H__79AE8375_169A_480E_92BF_1F37EF8BA77B__INCLUDED_)
