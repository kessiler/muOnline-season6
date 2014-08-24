// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------
#pragma once

typedef struct _stMonsterStatRef
{
	//int minUserReset;
	//int maxUserReset;
	int minUserLevel;
	int maxUserLevel;
	float level_mul;
	float hp_mul;
	float mp_mul;
	float min_dmg_mul;
	float max_dmg_mul;
	float def_mul;
	float magic_def_mul;
	float attack_rate_mul;
	float def_rate_mul;

	_stMonsterStatRef()
	{
		//minUserReset = 0;
		//maxUserReset = 0;
		this->minUserLevel = 0;
		this->maxUserLevel = 0;
		this->level_mul = 0;
		this->hp_mul = 0;
		this->mp_mul = 0;
		this->min_dmg_mul = 0;
		this->max_dmg_mul = 0;
		this->def_mul = 0;
		this->magic_def_mul = 0;
		this->attack_rate_mul = 0;
		this->def_rate_mul = 0;
	}
}stMonsterStatRef;

struct OBJECTSTRUCT;

class CMonsterStatCalc
{
public:
	CMonsterStatCalc(void);
	virtual ~CMonsterStatCalc(void);

	void LoadScript(const char *Filename);
	int ConvertMonsterStat(struct OBJECTSTRUCT *lpObj, int nUserMaxLevel, int nUserMaxReset);

private:
	BOOL m_bScriptLoad;
	std::vector<stMonsterStatRef> m_MonsterStatRef;
}; extern CMonsterStatCalc g_MonsterStatCalc;
