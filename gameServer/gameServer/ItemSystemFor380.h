// ItemSystemFor380.h: interface for the CItemSystemFor380 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSYSTEMFOR380_H__F545D1C3_2C57_497F_961A_77B689252FFE__INCLUDED_)
#define AFX_ITEMSYSTEMFOR380_H__F545D1C3_2C57_497F_961A_77B689252FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\common\zzzitem.h"

#define ITEMOPTION_FOR380ITEM_EFFECT_NONE							0
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDATTACKSUCCESSRATEPVP		1
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDDAMAGE					2
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSESUCCESSRATEPVP		3
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSE					4
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXHP						5
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXSD						6
#define ITEMOPTION_FOR380ITEM_EFFECT_OPREFILLON						7
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDREFILLSD					8

struct ITEMOPTION_FOR380ITEM
{
	BOOL IsLoad;	// 0
	BYTE m_Type;	// 4
	BYTE m_Index;	// 5
	BYTE m_Option1;	// 6
	WORD m_Value1;	// 8
	BYTE m_Option2;	// A
	WORD m_Value2;	// C
};





struct ITEMOPTION_FOR380ITEM_EFFECT
{
	short OpAddAttackSuccessRatePVP;	// 0
	short OpAddDamage;	// 2
	short OpAddDefenseSuccessRatePvP;	// 4
	short OpAddDefense;	// 6
	short OpAddMaxHP;	// 8
	short OpAddMaxSD;	// A
	short OpRefillOn;	// C
	short OpAddRefillSD;	// E
};




class CItemSystemFor380
{

public:

	CItemSystemFor380();
	virtual ~CItemSystemFor380();

	BOOL Load380ItemOptionInfo(LPSTR filename);
	BOOL Is380Item( CItem const *  pItem);
	BOOL Is380OptionItem( CItem const * pItem);
	void InitEffectValue( ITEMOPTION_FOR380ITEM_EFFECT* pItemEffect);
	int ApplyFor380Option(struct OBJECTSTRUCT* lpObj);
	void SetOptionItemByMacro(struct OBJECTSTRUCT* lpObj, BYTE invenrotyTargetPos, int bOption);
	int ChaosMix380ItemOption(struct OBJECTSTRUCT* lpObj);

private:

	void _InitOption();
	int _CalcItemEffectValue(int iItemOptionType, int iItemEffectValue, ITEMOPTION_FOR380ITEM_EFFECT* pItemEffect);
	int _SetOption(CItem* pItem, int bOption);

private:

	ITEMOPTION_FOR380ITEM m_itemOption[MAX_ITEMS];	// 4
	BOOL m_bSystemFor380ItemOption;	// 20004
	int m_iNeedJewelOfHarmonyCount;	// 20008
	int m_iNeedJewelOfSuhoCount;	// 2000C
	int m_iNeedZenFor380Option;	// 20010
	int m_iRateSuccessRateForMix1;	// 20014
	int m_iRateSuccessRateForMix2;	// 20018
	int m_iRateSuccessRateForMix3;	// 2001C

};


inline void DebugLog(LPSTR szLog, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, szLog);
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);

	OutputDebugString(szBuffer);
}

extern CItemSystemFor380 g_kItemSystemFor380;

#endif // !defined(AFX_ITEMSYSTEMFOR380_H__F545D1C3_2C57_497F_961A_77B689252FFE__INCLUDED_)
