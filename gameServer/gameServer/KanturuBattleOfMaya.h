// KanturuBattleOfMaya.h: interface for the CKanturuBattleOfMaya class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLEOFMAYA_H__69EB902F_6582_4C1D_A741_77B1411DAC09__INCLUDED_)
#define AFX_KANTURUBATTLEOFMAYA_H__69EB902F_6582_4C1D_A741_77B1411DAC09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"
#include "KanturuMaya.h"


#define KANTURU_MAYA_AI_CHANGE_TIME	4
#define KANTURU_MAYA_TIME_INFO		3
#define KANTURU_MAYA_STATE_INFO		19
#define KANTURU_MAYA_GROUP_NUMBER	3

// States
#define KANTURU_MAYA_NONE			0
#define KANTURU_MAYA_STANBY1		1
#define KANTURU_MAYA_NOTIFY1		2
#define KANTURU_MAYA_START1			3
#define KANTURU_MAYA1				4
#define KANTURU_MAYA_END1			5
#define KANTURU_MAYA_ENDCYCLE1		6
#define KANTURU_MAYA_STANBY2		7
#define KANTURU_MAYA_START2			8
#define KANTURU_MAYA2				9
#define KANTURU_MAYA_END2			10
#define KANTURU_MAYA_ENDCYCLE2		11
#define KANTURU_MAYA_STANBY3		12
#define KANTURU_MAYA_START3			13
#define KANTURU_MAYA3				14
#define KANTURU_MAYA_END3			15
#define KANTURU_MAYA_ENDCYCLE3		16
#define KANTURU_MAYA_END			17
#define KANTURU_MAYA_ENDCYCLE		18

class CKanturuBattleOfMaya
{

public:

	CKanturuBattleOfMaya();
	virtual ~CKanturuBattleOfMaya();

	void ResetAllData();
	BOOL LoadData(LPSTR lpszFileName);
	void Run();
	void SetState(int iBattleOfMayaState);
	void SetNextState(int iCurrentState);
	void SetState_NONE();
	void SetState_STANBY1();
	void SetState_NOTIFY1();
	void SetState_START1();
	void SetState_MAYA1();
	void SetState_END_MAYA1();
	void SetState_ENDCYCLE_MAYA1();
	void SetState_STANBY2();
	void SetState_START2();
	void SetState_MAYA2();
	void SetState_END_MAYA2();
	void SetState_ENDCYCLE_MAYA2();
	void SetState_STANBY3();
	void SetState_START3();
	void SetState_MAYA3();
	void SetState_END_MAYA3();
	void SetState_ENDCYCLE_MAYA3();
	void SetState_END();
	void SetState_ENDCYCLE();
	void ProcState_NONE();
	void ProcState_STANBY1();
	void ProcState_NOTIFY1();
	void ProcState_START1();
	void ProcState_MAYA1();
	void ProcState_END_MAYA1();
	void ProcState_ENDCYCLE_MAYA1();
	void ProcState_STANBY2();
	void ProcState_START2();
	void ProcState_MAYA2();
	void ProcState_END_MAYA2();
	void ProcState_ENDCYCLE_MAYA2();
	void ProcState_STANBY3();
	void ProcState_START3();
	void ProcState_MAYA3();
	void ProcState_END_MAYA3();
	void ProcState_ENDCYCLE_MAYA3();
	void ProcState_END();
	void ProcState_ENDCYCLE();
	void SetBattleOfMayaState(int iState){this->m_iBattleOfMayaState = iState;}
	int GetBattleOfMayaState(){return this->m_iBattleOfMayaState;}
	void SetBattleSceneState(int iScene)
	{
		this->m_iSceneState = iScene;
		this->m_bIsSceneSuccess = FALSE;
	}

	int GetBattleSceneState(){return this->m_iSceneState;}
	void SetSuccessValue(BOOL bIsSuccess){this->m_bIsSucccess = bIsSuccess;}
	BOOL GetSuccessValue(){return this->m_bIsSucccess;}
	void SetSceneSuccessValue(BOOL bIsSuccess){this->m_bIsSceneSuccess = bIsSuccess;}
	BOOL GetSceneSuccessValue(){return this->m_bIsSceneSuccess;}
	void SetEntrancePermit(BOOL bEntrancePermit){this->m_bEntrancePermit = bEntrancePermit;}
	BOOL GetEntrancePermit(){return this->m_bEntrancePermit;}
	int CheckStateTime();
	int GetRemainTime();
	void CheckMayaBattleUser();
	void SetAIMonsterGroup(int iGroupNumber);
	void ResetAIMonster();
	void ChangeAI(int iGroupNumber);

private:

	int m_bFileDataLoad;	// 4
	int m_iBattleOfMayaState;	// 8
	int m_iSceneState;	// C
	CKanturuStateInfo m_BattleTimeInfo[KANTURU_MAYA_TIME_INFO];	// 10
	int m_BattleTimeInfoCount;	// 4C
	CKanturuStateInfo m_StateInfo[KANTURU_MAYA_STATE_INFO];	// 50
	int m_StateInfoCount;	// 1CC
	BOOL m_bEntrancePermit;	// 1D0
	BOOL m_bIsSucccess;	// 1D4
	BOOL m_bIsSceneSuccess;	// 1D8
	CKanturuMaya m_KanturuMaya;	// 1DC
	int m_iMayaHandDieTimeCounter;	// 1F0
	int m_iMayaLeftHandObjIndex;	// 1F4
	int m_iMayaRightHandObjIndex;	// 1F8
	int m_iMayaHandAIChangeTime[KANTURU_MAYA_AI_CHANGE_TIME];	// 1FC
	int m_iMayaHandAIChangeTimeCount;	// 20C
	int m_iMayaHandAIAppliedTime;	// 210
	int m_iMayaHandCurrentAI;	// 214
	int m_iMayaHandGroupNumber[KANTURU_MAYA_GROUP_NUMBER];	// 218
	int m_iMayaHAndGroupNumberCount;	// 224
	int m_iMayaIceStormUsingRate;	// 228

};


#endif // !defined(AFX_KANTURUBATTLEOFMAYA_H__69EB902F_6582_4C1D_A741_77B1411DAC09__INCLUDED_)
