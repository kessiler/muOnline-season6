// Kanturu.h: interface for the CKanturu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURU_H__7040132E_6A41_47FB_82ED_FE28C0EB1CF6__INCLUDED_)
#define AFX_KANTURU_H__7040132E_6A41_47FB_82ED_FE28C0EB1CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapClass.h"
#include "KanturuStateInfo.h"
#include "KanturuBattleStanby.h"
#include "KanturuBattleOfMaya.h"
#include "KanturuBattleOfNightmare.h"
#include "KanturuTowerOfRefinement.h"


#define MAX_KANTURU_STATE_INFO	6
#define MAX_KANTURU_MAP_LEVEL	2

#define KANTURU_MAP_CLOSE	0
#define KANTURU_MAP_OPEN	1



#define KANTURU_STATE_NONE					0
#define KANTURU_STATE_BATTLE_STANTBY		1
#define KANTURU_STATE_BATTLE_OF_MAYA		2
#define KANTURU_STATE_BATTLE_OF_NIGHTMARE	3
#define KANTURU_STATE_TOWER_OF_REFINEMENT	4
#define KANTURU_STATE_END					5



class CKanturu
{

public:
  
	CKanturu();
	virtual ~CKanturu();

	//void __thiscall Init();
	void ResetAllData();
	BOOL LoadData(LPSTR lpszFileName);
	void LoadKanturuMapAttr(LPSTR lpszFileName, BYTE btLevel);
	void SetKanturuMapAttr(BYTE btLevel);
	void Run();
	void SetState(int iKanturuState);
	void SetNextState(int iCurrentState);
	void ChangeState(int iState, int DetailState);
	void SetState_NONE();
	void SetState_BATTLE_STANDBY();
	void SetState_BATTLE_OF_MAYA();
	void SetState_BATTLE_OF_NIGHTMARE();
	void SetState_TOWER_OF_REFINEMENT();
	void SetState_END();
	//void __thiscall SetState_ENDCYCLE();
	void ProcState_NONE();
	void ProcState_BATTLE_STANDBY();
	void ProcState_BATTLE_OF_MAYA();
	void ProcState_BATTLE_OF_NIGHTMARE();
	void ProcState_TOWER_OF_REFINEMENT();
	void ProcState_END();
	//void __thiscall ProcState_ENDCYCLE();
	void SetKanturuEnable(BOOL bEnable){this->m_bKanturuEnable = bEnable;}
	BOOL IsKanturuEnable(){return this->m_bKanturuEnable;}
	void SetKanturuState(int iKanturuState){this->m_iKanturuState = iKanturuState;}
	int GetKanturuState(){return this->m_iKanturuState;}
	int GetKanturuDetailState();
	//void __thiscall SetSuccessValue(int);
	//int __thiscall GetSuccessValue();
	void CheckStateTime();
	int GetRemainTime();
	void CheckUserOnKanturuBossMap();
	int CheckEnterKanturu(int iUserIndex);
	BOOL CheckCanEnterKanturuBattle();
	void SetEnableCheckMoonStone(BOOL bEnable){this->m_bEnableCheckMoonStone = bEnable;}
	int GetEnableCheckMoonStone(){return this->m_bEnableCheckMoonStone;}
	BOOL CheckEqipmentMoonStone(int iUserIndex);
	void KanturuMonsterDieProc(int iMonIndex, int iKillerIndex);
	void OperateGmCommand(int iUserIndex, int iCommand);
	void UserMonsterCountCheck();
	int GetKanturuBattleCounter(){return this->m_iKanturuBattleCounter;}
	int GetKanturuBattleDate(){return this->m_iKanturuBattleDate;}

private:

	BOOL m_bKanturuEnable;	// 4
	int m_iKanturuState;	// 8
	BOOL m_bIsSucccess;	// C
	BOOL m_bFileDataLoad;	// 10
	CKanturuStateInfo m_StateInfo[MAX_KANTURU_STATE_INFO];	// 14
	int m_StateInfoCount;	// 8C
	CKanturuBattleStanby m_BattleStanby;	// 90
	CKanturuBattleOfMaya m_BattleOfMaya;	// F4
	CKanturuBattleOfNightmare m_BattleOfNightmare;	// 320
	CKanturuTowerOfRefinement m_TowerOfRefinement;	// 3B4
	MapClass m_KanturuMap[2];	// 444
	BOOL m_bEnableCheckMoonStone;	// A2514
	int m_iKanturuBattleCounter;	// A2518
	int m_iKanturuBattleDate;	// A251C

};


extern CKanturu g_Kanturu;

#endif // !defined(AFX_KANTURU_H__7040132E_6A41_47FB_82ED_FE28C0EB1CF6__INCLUDED_)
