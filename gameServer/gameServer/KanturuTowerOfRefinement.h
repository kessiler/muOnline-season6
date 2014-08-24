// KanturuTowerOfRefinement.h: interface for the CKanturuTowerOfRefinement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUTOWEROFREFINEMENT_H__5A9FF48E_75EB_42CC_9E98_D70BD55A4064__INCLUDED_)
#define AFX_KANTURUTOWEROFREFINEMENT_H__5A9FF48E_75EB_42CC_9E98_D70BD55A4064__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"


#define KANTURU_TOWER_OF_REFINEMENT_STATE_INFO	6

// States
#define KANTURU_TOWER_OF_REFINEMENT_NONE			0
#define KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION	1
#define KANTURU_TOWER_OF_REFINEMENT_NOTIFY1			2
#define KANTURU_TOWER_OF_REFINEMENT_CLOSE			3
#define KANTURU_TOWER_OF_REFINEMENT_NOTIFY2			4
#define KANTURU_TOWER_OF_REFINEMENT_END				5

class CKanturuTowerOfRefinement
{

public:
	
	CKanturuTowerOfRefinement();
	virtual ~CKanturuTowerOfRefinement();

	void Init();
	BOOL LoadData(LPSTR lpszFileName);
	void ResetAllData();
	void Run();
	void SetState(int iTowerOfRefinement);
	void SetNextState(int iCurrentState);
	void SetState_NONE();
	void SetState_REVITALIZATION();
	void SetState_NOTIFY1();
	void SetState_CLOSE();
	void SetState_NOTIFY2();
	void SetState_END();
	void ProcState_NONE();
	void ProcState_REVITALIZATION();
	void ProcState_NOTIFY1();
	void ProcState_CLOSE();
	void ProcState_NOTIFY2();
	void ProcState_END();
	void SetTowerOfRefinementState(int iTowerOfRefinement){this->m_iTowerOfRefinementState = iTowerOfRefinement;}
	int GetTowerOfRefinementState(){return this->m_iTowerOfRefinementState;}
	void SetEntrancePermit(BOOL bEntrancePermit){this->m_bEntrancePermit = bEntrancePermit;}
	BOOL GetEntrancePermit(){return this->m_bEntrancePermit;}
	void SetUseTowerOfRefinement(BOOL bUse){this->m_bIsUseTowerOfRefinement = bUse;}
	BOOL IsUseTowerOfRefinement(){return this->m_bIsUseTowerOfRefinement;}
	void CheckStateTime();
	int GetRemainTime();

private:

	int m_iTowerOfRefinementState;	// 4
	BOOL m_bEntrancePermit;	// 8
	BOOL m_bIsUseTowerOfRefinement;	// C
	BOOL m_bFileDataLoad;	// 10
	CKanturuStateInfo m_StateInfo[KANTURU_TOWER_OF_REFINEMENT_STATE_INFO];	// 14
	int m_StateInfoCount;	// 8C

};

#endif // !defined(AFX_KANTURUTOWEROFREFINEMENT_H__5A9FF48E_75EB_42CC_9E98_D70BD55A4064__INCLUDED_)
