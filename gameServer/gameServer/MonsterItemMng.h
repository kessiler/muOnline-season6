//-> Decompiled by DarkSim | 09.07.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "..\common\zzzitem.h"
// -------------------------------------------------------------------------------

#define MAX_MONSTER_ITEM		1000
#define MAX_MONSTER_EXITEM		100
#define MAX_MONSTER_LEVEL		150
// -------------------------------------------------------------------------------

struct __tagMONSTER_ITEM_DROP_PRATE_	//-> 36
{
	int m_MonsterLevel;			//+5B8 (1464)
	int m_MagicBookRate;		//+5BC (1468)
	int m_JewelOfBlessRate;		//+5C0 (1472)
	int m_JewelOfSoulRate;		//+5C4 (1476)
	int m_JewelOfLifeRate;		//+5C8 (1480)
	int m_JewelOfCreationRate;	//+5CC (1484)
	int m_JewelOfChaosRate;		//+5D0 (1488)
	int m_NormalItemRate;		//+5D4 (1492)
	int m_TotalDropRate;		//+5D8 (1496)
	// ----
	__tagMONSTER_ITEM_DROP_PRATE_::__tagMONSTER_ITEM_DROP_PRATE_()	//-> 0042ed50
	{
		this->m_MonsterLevel		= 0;
		this->m_MagicBookRate		= 0;
		this->m_JewelOfBlessRate	= 0;
		this->m_JewelOfSoulRate		= 0;
		this->m_JewelOfLifeRate		= 0;
		this->m_JewelOfCreationRate	= 0;
		this->m_JewelOfChaosRate	= 0;
		this->m_NormalItemRate		= 0;
		this->m_TotalDropRate		= 0;
	}
};
// -------------------------------------------------------------------------------

class CMonsterItemMng	//-> Complete
{
public:
			CMonsterItemMng();
	virtual ~CMonsterItemMng();
	// ----
	void	Init();
	void	Clear();
	BYTE	InsertItem(int monsterlevel, int type, int index, int itemlevel,  int op1, int op2, int op3);
	CItem * GetItem(int monsterlevel);
	void	gObjGiveItemSearch(int monsterlevel, int maxlevel);
	void	LoadMonsterItemDropRate();
	void	MakeJewelItem();
	void	gObjGiveItemSearchEx(int monsterlevel, int maxlevel);
	void	MagicBookGiveItemSearch(int monsterlevel, int maxlevel);
	void	NormalGiveItemSearch(int monsterlevel, int maxlevel);
	int		CheckMonsterDropItem(int type, int index);
	CItem * GetItemEx(int monsterlevel);
	// ----
private:
	CItem * m_MonsterInvenItems[MAX_MONSTER_LEVEL];				//+4	(4)
	int		m_iMonsterInvenItemCount[MAX_MONSTER_LEVEL];		//+25C	(604)
	char	MonsterName[255];									//+4B4	(1204)
	int		m_bScriptLoaded;									//+5B4	(1460)
	__tagMONSTER_ITEM_DROP_PRATE_ RateData[MAX_MONSTER_LEVEL];	//+5B8	(1464)
	int		m_iMonsterInvenExItemCount[MAX_MONSTER_LEVEL];		//+1AD0 (6864)
	CItem * m_MonsterInvenExItems[MAX_MONSTER_LEVEL];			//+1D28 (7464)
	CItem * m_MonsterJewelItem1;								//+1F80 (8064)
	CItem * m_MonsterJewelItem2;								//+1F84 (8068)
	CItem * m_MonsterJewelItem3;								//+1F88 (8072)
	CItem * m_MonsterJewelItem4;								//+1F8C (8076)
	CItem * m_MonsterJewelItem5;								//+1F90 (8080)
	// ----
}; extern CMonsterItemMng g_MonsterItemMng;
// -------------------------------------------------------------------------------