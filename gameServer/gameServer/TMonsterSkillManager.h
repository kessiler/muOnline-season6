#pragma once
// -------------------------------------------------------------------------------

#include "../common/TSync.h"
#include "TMonsterSkillInfo.h"
#include "TMonsterSkillUnit.h"
#include "user.h"
// -------------------------------------------------------------------------------

#define MAX_MONSTER_SKILL_INFO_ARRAY		(700)	//Increased by 1.01.00
#define MAX_MONSTER_SKILL_DELAY_INFO_ARRAY	(3000)
// -------------------------------------------------------------------------------

struct _ST_MONSTER_SKILL_DELAYTIME_INFO
{
	void RESET()
	{
		this->iIndex = -1;
		this->iTargetIndex = -1;
		this->dwDelayTime = 0;
		this->bIsUsed = FALSE;
		this->lpMonsterSkillUnit = NULL;
	}


	BOOL IsValid()
	{
		if ( !OBJMAX_RANGE(this->iIndex) || !OBJMAX_RANGE(this->iTargetIndex) )
			return FALSE;

		if ( this->lpMonsterSkillUnit == NULL )
			return FALSE;

		return TRUE;
	}

	_ST_MONSTER_SKILL_DELAYTIME_INFO()
	{
		this->RESET();
	}

	BOOL bIsUsed;	// 0
	int iIndex;	// 4
	int iTargetIndex;	// 8
	DWORD dwDelayTime;	// C
	TMonsterSkillUnit* lpMonsterSkillUnit;	// 10
};
// -------------------------------------------------------------------------------

class TMonsterSkillManager  
{
public:
	TMonsterSkillManager();
	~TMonsterSkillManager();
	// ----
	static int		LoadData(char* lpszFileName);
	static void		DelAllSkillManagerInfo();
	static int		CheckMonsterSkill(int iMonsterClass);
	static void		UseMonsterSkill(int iIndex, int iTargetIndex, int iMonsterSkillUnitType, int iMonsterSkillUnit, CMagicInf * lpMagic);
	static class	TMonsterSkillUnit* FindMonsterSkillUnit(int iIndex, int iMonsterSkillUnitType);
	static void		MonsterSkillProc();
	static int		AddMonsterSkillDelayInfo(int iIndex, int iTargetIndex, int iDelayTime, TMonsterSkillUnit * lpMonsterSkillUnit);
	static bool		FindMagicInf(TMonsterSkillUnit * lpMonsterSkillUnit, CMagicInf * lpOutMagic);
public:
	static TSync	s_Sync;
	static BOOL		s_bDataLoad;
	static TMonsterSkillInfo s_MonsterSkillInfoArray[MAX_MONSTER_SKILL_INFO_ARRAY];
	static _ST_MONSTER_SKILL_DELAYTIME_INFO s_MonsterSkillDelayInfoArray[MAX_MONSTER_SKILL_DELAY_INFO_ARRAY];
};
// -------------------------------------------------------------------------------