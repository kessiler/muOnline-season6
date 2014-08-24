#pragma once
// -------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------

#ifdef __ALIEN__
struct DROPBLOCK_DATA
{
	int ItemType;
	int ItemIndex;
};
#endif
// -------------------------------------------------------------------------

struct DROPEX_DATA
{
	int ItemType;
	int ItemIndex;
	int ItemMinLevel;
	int ItemMaxLevel;
	int ItemMaxOption;
	int ItemLuck;
	int ItemSkill;
	int ItemExcellent;
#ifdef __ALIEN__
	int ItemMaxExc;
	int ItemOptRate;
	int ItemSkillRate;
	int ItemLuckRate;
	int ItemExcRate;
	int ItemAncRate;
#endif
	int ItemAncent;
	int ItemDur;
	int ItemDropRate;
	int MonsterMinLevel;
	int MonsterMaxLevel;
	int MonsterMap;
	// ----
	DROPEX_DATA()
	{
		ZeroMemory(this, sizeof(*this));
	}
};
// -------------------------------------------------------------------------

class DropEx
{
public:
			DropEx();
			~DropEx();
	// ----
	void	Init();
	void	Load();
	void	ReadData(char * File);
#ifdef __ALIEN__
	void	ReadBlockData(char * File);
	bool	IsBlockItem(int ItemID);
#endif
	// ----
	bool	DropItem(LPOBJ lpObj, LPOBJ lpTargetObj);
	// ----
private:
	std::vector<DROPEX_DATA> m_Data;
#ifdef __ALIEN__
	std::vector<DROPBLOCK_DATA> m_NonDrop;
#endif
	int GenExcOption(int amount);
	// ----
}; extern DropEx g_DropEx;
// -------------------------------------------------------------------------