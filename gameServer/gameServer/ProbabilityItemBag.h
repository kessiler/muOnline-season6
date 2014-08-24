#pragma once
// -------------------------------------------------------------------------------

#include "MapClass.h"
#include "user.h"
#include "..\common\zzzitem.h"
#include "ItemBagEx.h"
#include "GambleSystem.h"
// -------------------------------------------------------------------------------

#define MAX_PROBABILITY_LEVEL		150
#define IS_PROBABILITY_RANGE(n)		( ((n) < 0 )?FALSE:( (n) > MAX_PROBABILITY_LEVEL-1 )?FALSE:TRUE )
// -------------------------------------------------------------------------------

class CItemBagAttrEx2: public CItemBagAttrEx	//-> Complete
{
public:
    CItemBagAttrEx2()	//-> 004b8de0
	{
		this->m_isSetItem = 0;
	};
	// ----
    BYTE m_isSetItem;
};
// -------------------------------------------------------------------------------

class CProbabilityItemBag	//-> Complete
{
public:
				CProbabilityItemBag();
	virtual		~CProbabilityItemBag();
	// ----
	void		ProbabilityItemBagInit(char * name);
	void		LoadItem(char * script_file);
	// ----
	int			GetDropItemNumber(int nDropItemRate);
	char *		GetEventName();
	// ----
private:
	int			GetBagCount();
	int			IsEnableEventItemDrop(int aIndex);
	BYTE		GetLevel(int n);
	// ----
public:
	int			DropNewYearLuckyBagEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropGMPresentBoxEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropIllusionTempleGhostItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	void		DropEventSetItem(int aIndex, int itemnum, BYTE btMapNumber, int X, int Y);
	int			DropCherryBlossomEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY, int * pMoney);
	int			DropRaklionSelupanItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			GetDropItemNumberEx(int nDropItemRate);
	int			DropEventItemEx(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropGoldenBoxItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropSilverBoxItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropJewelleryCaseItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropNewMonsterItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int			DropBoxOfGRPColorItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	// ----
private:
	int			m_bLoad;										//+4
	char		m_sEventName[255];								//+8
	int			m_iEventItemType;								//+264
	int			m_iEventItemLevel;								//+268
	int			m_iDropZen;										//+272
	int			m_iEventItemDropRate;							//+276
	int			m_iItemDropRate;								//+280
	int			m_iExItemDropRate;								//+284
	int			m_iBagObjectCount;								//+288
	CItemBagDropMapInfo DropMapInfo[MAX_NUMBER_MAP];
	CItemBagAttrEx2	BagObject[MAX_PROBABILITY_LEVEL];
	int			m_iEventItemDropRateEx[20];
	int			m_iDropRatePerItemCount[20];
	int			m_iRateKindCount;								//+2048
};
// -------------------------------------------------------------------------------