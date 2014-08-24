// TMonsterAIElement.h: interface for the TMonsterAIElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIELEMENT_H__AF48ED05_A6AC_4E97_AB67_D5879406F578__INCLUDED_)
#define AFX_TMONSTERAIELEMENT_H__AF48ED05_A6AC_4E97_AB67_D5879406F578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TMonsterAIElement.h"
#include "TMonsterAIState.h"
#include "TMonsterAIMovePath.h"
#include "MapClass.h"

#define MAX_AI_ELEMENT			100
#define MAX_AI_ELEMENT_CLASS	8


enum MONSTER_AI_ELEMENT_STATE
{
	MAE_STATE_NORMAL = 0x0,
	MAE_STATE_MOVE = 0x1,
	MAE_STATE_ATTACK = 0x2,
	MAE_STATE_HEAL = 0x3,
	MAE_STATE_AVOID = 0x4,
	MAE_STATE_HELP = 0x5,
	MAE_STATE_SPECIAL = 0x6,
	MAE_STATE_EVENT = 0x7,
};

enum MONSTER_AI_ELEMENT_TYPE
{
	MAE_TYPE_COMMON_NORMAL = 0x1,
	MAE_TYPE_MOVE_NORMAL = 0xb,
	MAE_TYPE_MOVE_TARGET = 0xc,
	MAE_TYPE_GROUP_MOVE = 0xd,
	MAE_TYPE_GROUP_MOVE_TARGET = 0xe,
	MAE_TYPE_ATTACK_NORMAL = 0x15,
	MAE_TYPE_ATTACK_AREA = 0x16,
	MAE_TYPE_ATTACK_PENETRATION = 0x17,
	MAE_TYPE_HEAL_SELF = 0x1f,
	MAE_TYPE_HEAL_GROUP = 0x20,
	MAE_TYPE_AVOID_NORMAL = 0x29,
	MAE_TYPE_HELP_HP = 0x33,
	MAE_TYPE_HELP_BUFF = 0x34,
	MAE_TYPE_HELP_TARGET = 0x35,
	MAE_TYPE_SPECIAL = 0x3d,
	MAE_TYPE_SPECIAL_SOMMON = 0x3e,
	MAE_TYPE_SPECIAL_IMMUNE = 0x40,
	MAE_TYPE_SPECIAL_NIGHTMARE_SUMMON = 0x41,
	MAE_TYPE_SPECIAL_WARP = 0x42,
	MAE_TYPE_SPECIAL_SKILLATTACK = 0x43,
	MAE_TYPE_SPECIAL_CHANGEAI = 0x44,
	MAE_TYPE_EVENT = 0x47,
};

enum MONSTER_AI_ELELMENT_TARGET_TYPE
{
	MAE_TARGET_TYPE_CHARACTER = 0x1,
	MAE_TARGET_TYPE_XY = 0x2,
	MAE_TARGET_TYPE_MONSTER = 0x3,
};

class TMonsterAIElement  
{

public:

	TMonsterAIElement();
	virtual ~TMonsterAIElement();

	int ForceAIElement(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementCommon(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementMove(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementMoveTarget(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementGroupMove(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementAttack(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementAttackArea(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementAttackPenetration(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementAvoid(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementHealSelf(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementHealGroup(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementSpecialSommon(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementSpecialImmune(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementNightmareSummon(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementNightmareWarp(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementSkillAttack(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	int ApplyElementAIChange(int iIndex, int iTargetIndex,  TMonsterAIState* pAIState);
	void Reset();
	//char* __thiscall GetElementName();

	static int LoadData(char* lpszFileName);
	static int DelAllAIElement();
	static TMonsterAIElement* FindAIElement(int iElementNumber);

private:

	char m_szElementName[50];	// 4
	int m_iElementNumber;	// 38
	int m_iElementClass;	// 3C
	int m_iElementType;	// 40
	int m_iSuccessRate;	// 44
	int m_iDelayTime;	// 48
	int m_iTargetType;	// 4C
	int m_iX;	// 50
	int m_iY;	// 54

public:

	static BOOL s_bDataLoad;
	static TMonsterAIElement s_MonsterAIElementArray[MAX_AI_ELEMENT];
	static TMonsterAIMovePath s_MonsterAIMovePath[MAX_NUMBER_MAP];

};

#endif // !defined(AFX_TMONSTERAIELEMENT_H__AF48ED05_A6AC_4E97_AB67_D5879406F578__INCLUDED_)
