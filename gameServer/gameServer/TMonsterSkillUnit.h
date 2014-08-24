// TMonsterSkillUnit.h: interface for the TMonsterSkillUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERSKILLUNIT_H__9F3D0706_317D_4E48_8A4A_439053F8F0B0__INCLUDED_)
#define AFX_TMONSTERSKILLUNIT_H__9F3D0706_317D_4E48_8A4A_439053F8F0B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TMonsterSkillElement.h"


#define MAX_MONSTER_SKILL_UNIT_ARRAY	(200)

class TSkillUnit
{

public:

private:

};

class TMonsterSkillUnit : public TSkillUnit
{

public:

	TMonsterSkillUnit();
	virtual ~TMonsterSkillUnit();

	void RunSkill(int iIndex, int iTargetIndex);
	void Reset();

	static int LoadData(char* lpszFileName);
	static int DelAllSkillUnit();
	static class TMonsterSkillUnit* FindSkillUnit(int iUnitNumber);

public:

	char m_szUnitName[20];	// 4
	int m_iUnitNumber;	// 18
	int m_iUnitTargetType;	// 1C
	int m_iUnitScopeType;	// 20
	int m_iUnitScopeValue;	// 24
	int m_iDelay;	// 28
	int m_iElementsCount;	// 2C
	TMonsterSkillElement* m_lpElementsSlot[5];	// 30

	static BOOL s_bDataLoad;
	static TMonsterSkillUnit s_MonsterSkillUnitArray[MAX_MONSTER_SKILL_UNIT_ARRAY];

};


#endif // !defined(AFX_TMONSTERSKILLUNIT_H__9F3D0706_317D_4E48_8A4A_439053F8F0B0__INCLUDED_)
