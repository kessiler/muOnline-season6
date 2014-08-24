// JewelOfHarmonySystem.h: interface for the CJewelOfHarmonySystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JEWELOFHARMONYSYSTEM_H__8B3CA307_ABAB_4546_A6B1_22C3DFC70764__INCLUDED_)
#define AFX_JEWELOFHARMONYSYSTEM_H__8B3CA307_ABAB_4546_A6B1_22C3DFC70764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\common\zzzitem.h"
#include "TRandomPoolMgr.h"

#define MAX_JOH_ITEM_TYPE	4
#define MAX_JOH_ITEM_INDEX	16
#define MAX_JOH_ITEM_OPTION	14



enum eJewelOfHarmonySystem_ItemType {
  JEWELOFHARMONY_ITEM_TYPE_NULL = 0x0,
  JEWELOFHARMONY_ITEM_TYPE_WEAPON = 0x1,
  JEWELOFHARMONY_ITEM_TYPE_STAFF = 0x2,
  JEWELOFHARMONY_ITEM_TYPE_DEFENSE = 0x3,
};

enum eJewelOfHarmonyItemOption {
  AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM = 0x0,
  AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE = 0x1,
  AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE = 0x2,
  AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR = 0x3,
  AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX = 0x4,
  AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH = 0x5,
  AT_JEWELOFHARMONY_WEAPON_IMPROVE_CRITICALDAMAGE = 0x6,
  AT_JEWELOFHARMONY_WEAPON_IMPROVE_SKILLDAMAGE = 0x7,
  AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKSUCCESRATE_PVP = 0x8,
  AT_JEWELOFHARMONY_WEAPON_DECREASE_SD_RATE = 0x9,
  AT_JEWELOFHARMONY_WEAPON_IMPROVE_IGNORE_SD_RATE = 0xa,
  AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE = 0x1,
  AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR = 0x2,
  AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX = 0x3,
  AT_JEWELOFHARMONY_STAFF_IMPROVE_SKILLDAMAGE = 0x4,
  AT_JEWELOFHARMONY_STAFF_IMPROVE_CRITICALDAMAGE = 0x5,
  AT_JEWELOFHARMONY_STAFF_DECREASE_SD_RATE = 0x6,
  AT_JEWELOFHARMONY_STAFF_IMPROVE_ATTACKSUCCESRATE_PVP = 0x7,
  AT_JEWELOFHARMONY_STAFF_IMPROVE_IGNORE_SD_RATE = 0x8,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE = 0x1,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG = 0x2,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP = 0x3,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_HP = 0x4,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_MP = 0x5,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_BLOCKSUCCESRATE_PVP = 0x6,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE = 0x7,
  AT_JEWELOFHARMONY_DEFENSE_IMPROVE_SD_RATE = 0x8,
};



struct JEWELOFHARMONY_ITEM_OPTION
{
	BOOL bValid;	// 0
	BYTE iRandomWeight;	// 4
	BYTE iRequireLevel;	// 5
	BYTE iItemEffectValue[MAX_JOH_ITEM_OPTION];	// 6
	DWORD iZenForRestore[MAX_JOH_ITEM_OPTION];	// 14
	char szOptionName[256];	// 4C
};


struct JEWELOFHARMONY_ITEM_EFFECT
{
	short HJOpAddMinAttackDamage;	// 0
	short HJOpAddMaxAttackDamage;	// 2
	short HJOpRequireStr;	// 4
	short HJOpRequireDex;	// 6
	short HJOpAddAttackDamage;	// 8
	short HJOpAddCriticalDamage;	// A
	short HJOpAddSkillAttack;	// C
	short HJOpAddAttackSuccessRatePVP;	// E
	short HJOpDecreaseSDRate;	// 10
	short HJOpAddIgnoreSDRate;	// 12
	short HJOpAddMagicPower;	// 14
	short HJOpAddDefense;	// 16
	short HJOpAddMaxAG;	// 18
	short HJOpAddMaxHP;	// 1A
	short HJOpAddRefillHP;	// 1C
	short HJOpAddRefillMP;	// 1E
	short HJOpAddDefenseSuccessRatePvP;	// 20
	short HJOpAddDamageDecrease;	// 22
	short HJOpAddSDRate;	// 24
};



class CJewelOfHarmonySystem
{

public:

	CJewelOfHarmonySystem();
	virtual ~CJewelOfHarmonySystem();

	BOOL LoadScript(LPSTR lpszFileName);
	BOOL LoadScriptOfSmelt(LPSTR lpszFileName);
	BOOL IsJewelOfHarmonyOriginal(short type);
	BOOL IsJewelOfHarmonyPurity(short type);
	BOOL IsJewelOfHarmonySmeltingItems(short type);
	void SetEnableToUsePuritySystem(int bEnable);
	BOOL IsEnableToUsePuritySystem();
	void PurityJewelOfHarmony(struct OBJECTSTRUCT * lpObj);
	BYTE GetItemStrengthenOption(CItem* pItem);
	BYTE GetItemOptionLevel(CItem* pItem);
	BOOL IsActive(CItem* pItem);
	BOOL IsStrengthenByJewelOfHarmony(CItem* pItem);
	BOOL StrengthenItemByJewelOfRise(struct OBJECTSTRUCT * lpObj, int source, int target);	//-> New
	BOOL StrengthenItemByJewelOfHarmony(struct OBJECTSTRUCT * lpObj, int source, int target);
	void StrengthenItemByMacro(struct OBJECTSTRUCT * lpObj, BYTE invenrotyTargetPos, BYTE btOptionType,  BYTE btOptionLevel);
	void SetApplyStrengthenItem(struct OBJECTSTRUCT * lpObj);
	void InitEffectValue(JEWELOFHARMONY_ITEM_EFFECT* pItemEffect);
	int GetItemEffectValue(CItem* pItem, int iOptionType);
	BOOL IsEnableToMakeSmeltingStoneItem(CItem* pItem);
	BOOL MakeSmeltingStoneItem(struct OBJECTSTRUCT * lpObj);
	BOOL SmeltItemBySmeltingStone(struct OBJECTSTRUCT * lpObj, int source, int target);
	BOOL RestoreStrengthenItem(struct OBJECTSTRUCT * lpObj);
	BYTE MakeCharSetData(struct OBJECTSTRUCT * lpObj);
	BOOL NpcJewelOfHarmony(struct OBJECTSTRUCT * lpNpc, struct OBJECTSTRUCT * lpObj);
	BYTE ShowStrengthenOption(CItem* pItem);
	BOOL IsEnableToTrade(struct OBJECTSTRUCT * lpObj);

public: //season 4 change

	void _InitOption();
	BOOL _IsJewelOfHarmonySmeltingItemNor( short type);
	BOOL _IsJewelOfHarmonySmeltingItemExt(short type);
	BYTE _GetItemOptionLevel(CItem* pItem);
	BYTE _GetItemOptionRequireLevel(CItem* pItem);
	int _GetItemType(CItem* pItem);
	int _GetSelectRandomOption(CItem* pItem, int iItemType);
	BOOL _MakeOption(CItem* pItem, BYTE btOptionType, BYTE btOptionLevel);
	BOOL _CalcItemEffectValue(CItem* pItem, JEWELOFHARMONY_ITEM_EFFECT* pItemEffect);
	int _GetZenForRestoreItem(CItem* pItem);

private:

	BOOL m_bEnable;	// 4
	int JEWEL_OF_HARMONY_ITEMINDEX;	// 8
	int JEWEL_OF_HARMONY_PURITY_ITEMINDEX;	// C
	int JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX;	// 10
	int JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX;	// 14
	JEWELOFHARMONY_ITEM_OPTION m_itemOption[MAX_JOH_ITEM_TYPE][MAX_JOH_ITEM_INDEX];	// 18
	BOOL m_bSystemPrutiyJewel;	// 5318
	BOOL m_bSystemMixSmeltingStone;	// 531C
	BOOL m_bSystemRestoreStrengthen;	// 5320
	BOOL m_bSystemStrengthenItem;	// 5324
	BOOL m_bSystemSmeltingItem;	// 5328
	int m_iRatePuritySuccess;	// 532C
	int m_iZenForPurity;	// 5330
	int m_iRateMixSmeltingStoneNor;	// 5334
	int m_iRateMixSmeltingStoneExt;	// 5338
	int m_iZenForMixSmeltingStone;	// 533C
	int m_iRateStrengthenSuccess;	// 5340
	int m_iRateSmeltingSuccessNor;	// 5344
	int m_iRateSmeltingSuccessExt;	// 5348
	std::map<int,int> m_mapEnableMixList;	// 534C
	TRandomPoolMgr m_kRandomPool;	// 535C

};


extern CJewelOfHarmonySystem g_kJewelOfHarmonySystem;

#endif // !defined(AFX_JEWELOFHARMONYSYSTEM_H__8B3CA307_ABAB_4546_A6B1_22C3DFC70764__INCLUDED_)
