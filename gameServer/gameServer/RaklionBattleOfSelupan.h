// RaklionBattleOfSelupan.h: interface for the CRaklionBattleOfSelupan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONBATTLEOFSELUPAN_H__6F8F30E0_8BD1_44FC_B9E8_F63016DF64B8__INCLUDED_)
#define AFX_RAKLIONBATTLEOFSELUPAN_H__6F8F30E0_8BD1_44FC_B9E8_F63016DF64B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RaklionSelupan.h"

#define SELUPAN_STATE_NONE					0
#define SELUPAN_STATE_STANDBY				1
#define SELUPAN_STATE_PATTERN_1				2
#define SELUPAN_STATE_PATTERN_2				3
#define SELUPAN_STATE_PATTERN_3				4
#define SELUPAN_STATE_PATTERN_4				5
#define SELUPAN_STATE_PATTERN_5				6	
#define SELUPAN_STATE_PATTERN_6				7
#define SELUPAN_STATE_PATTERN_7				8
#define SELUPAN_STATE_DIE					9
#define STATE_PATTERN_1						0
#define STATE_PATTERN_2						1
#define STATE_PATTERN_3						2
#define STATE_PATTERN_4						3
#define STATE_PATTERN_5						4
#define STATE_PATTERN_6						5
#define STATE_PATTERN_7						6
#define MAX_STATE_PATTERN_COUNT				7
#define SKILL_FIRSTSKILL		0
#define SKILL_POISON_ATTACK		1
#define	SKILL_ICESTORM			2
#define	SKILL_ICESTRIKE			3
#define	SKILL_SUMMON			4
#define	SKILL_HEAL				5
#define	SKILL_FREEZE			6
#define	SKILL_TELEPORT			7
#define	SKILL_INVINCIBILITY		8
#define MAX_SELUPAN_SKILL		9



struct RAKLION_STATE_SKILL_INFO
{
	BOOL bIsSkillEnabled[MAX_SELUPAN_SKILL];
};

class CRaklionBattleOfSelupan  
{
public:
	CRaklionBattleOfSelupan();
	virtual ~CRaklionBattleOfSelupan();

	void ResetAllData();
	int Init();

	void Run();
	void SetState(int iBattleOfSelupanState);

	void SetState_NONE();
	void SetState_STANDBY();
	void SetState_PATTERN_1();
	void SetState_PATTERN_2();
	void SetState_PATTERN_3();
	void SetState_PATTERN_4();
	void SetState_PATTERN_5();
	void SetState_PATTERN_6();
	void SetState_PATTERN_7();
	void SetState_DIE();

	void ProcState_NONE();
	void ProcState_STANDBY();
	void ProcState_PATTERN_1();
	void ProcState_PATTERN_2();
	void ProcState_PATTERN_3();
	void ProcState_PATTERN_4();
	void ProcState_PATTERN_5();
	void ProcState_PATTERN_6();
	void ProcState_PATTERN_7();
	void ProcState_DIE();

	void PatternStateSkillUseProc(int SkillIndex);
	void SetPatternCondition(int iPattern,int iCondition);
	void SelupanLifeCheck();
	int GetBattleUserCount();
	void CreateSelupan();
	void DeleteSelupan();
	int GetSelupanObjIndex();
	void DeleteSummonMonster();
	void SetSelupanSkillDelay(int SkillDelay);
	void GmCommandSelupanPattern(int StatePattern);

	void SetBattleOfSelupanState	(int m_iBattleOfSelupanState)	{			this->m_iBattleOfSelupanState	= m_iBattleOfSelupanState;	}
	int GetBattleOfSelupanState		(							)	{	return	this->m_iBattleOfSelupanState;								}
	void SetSuccessValue			(BOOL bIsSuccess)				{			this->m_bIsSucccess				= bIsSuccess;				}
	BOOL GetSuccessValue			(							)	{	return	this->m_bIsSucccess;										}

private:
	int							m_004;									// this + 4
	int							m_iBattleOfSelupanState;				// this + 8
	int							m_PatternCondition[6];					// this + C
	DWORD						m_dwSkillDelay;							// this + 24
	DWORD						m_dwSkillDelayTick;						// this + 28
	BOOL						m_bIsUseFirstSkill;						// this + 2C
	BOOL						m_bIsSucccess;							// this + 30
	CRaklionSelupan				m_RaklionSelupan;						// this + 34
	RAKLION_STATE_SKILL_INFO	m_PatternSkill[MAX_STATE_PATTERN_COUNT];// this + CD4
	
	//e08
};

#endif // !defined(AFX_RAKLIONBATTLEOFSELUPAN_H__6F8F30E0_8BD1_44FC_B9E8_F63016DF64B8__INCLUDED_)
