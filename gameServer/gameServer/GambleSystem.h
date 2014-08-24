// GambleSystem.h: interface for the CGambleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMBLESYSTEM_H__FD1B162D_3B86_44CF_9A91_0BCE55602DB5__INCLUDED_)
#define AFX_GAMBLESYSTEM_H__FD1B162D_3B86_44CF_9A91_0BCE55602DB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GAMBLE_MAX_SECRET_ITEM_CLASS 4
#define GAMBLE_MAX_NORMAL_ITEM 5
#define GAMBLE_MAX_SECRET_ITEM_TYPE 6
#define GAMBLE_MAX_ITEM_INFO	25
#define GAMBLE_MAX_ITEM			150

class CItemBagAttrEx
{

public:

	CItemBagAttrEx()
	{
		this->m_type = 0;
		this->m_index = 0;
		this->m_minLevel = 0;
		this->m_maxLevel = 0;
		this->m_isskill = 0;
		this->m_isluck = 0;
		this->m_isoption = 0;
		this->m_isexitem = 0;
	}

	BYTE m_type;	// 0
	BYTE m_index;	// 1

	BYTE m_minLevel;	// 2
	BYTE m_maxLevel;	// 3

	BYTE m_isskill;	// 4
	BYTE m_isluck;	// 5

	BYTE m_isoption;	// 6
	BYTE m_isexitem;	// 7
};

struct ST_GAMBLE_INFO
{
	int m_iSkillRate;
	int m_iLuckRate;
	int m_iOptionRate;
	int m_iExcRate;
	int m_iSortItemRate;

	ST_GAMBLE_INFO()
	{
		m_iSkillRate = 0;
		m_iLuckRate = 0;
		m_iOptionRate = 0;
		m_iExcRate = 0;
		m_iSortItemRate = 0;
	};
};

class CGambleSystem  
{
public:
	CGambleSystem();
	virtual ~CGambleSystem();

	void Init(char* filename);
	void Load(char* filename);
	BYTE GetLevel(int n);
	BYTE TryGamble(int aIndex, BYTE MapNumber, int cX, int cY, BYTE btRareItemNum);
	int GetBagItemCount();
	BYTE SortExcNumberPerKind(int n);
	BYTE SortExcOption();

private:
	//Checks the file load process
	BOOL bLoad; //0x04

	//It Holds Common Item Rates (Skill, Luck, Option, Exc... etc)
	ST_GAMBLE_INFO	m_GamblingInfo[GAMBLE_MAX_ITEM_INFO];
	
	//Jewel of Life Option for all items (Special and Common)
	int m_iBagItemOptionRate[GAMBLE_MAX_SECRET_ITEM_CLASS]; //0x1FC
	
	//Special Item Rate
	DWORD m_dwSpecialItemRate[GAMBLE_MAX_NORMAL_ITEM]; //0x20C
	
	//Exc Option Rate for all items (Special and Common)
	int m_iBagItemExcRate[GAMBLE_MAX_SECRET_ITEM_CLASS]; //0x220

	//Sort Exc Option Variables
	int m_iExcRatePerItemKind[GAMBLE_MAX_SECRET_ITEM_TYPE]; //0x230
	int m_iGeneralExcRate[GAMBLE_MAX_SECRET_ITEM_TYPE]; //0x248
	
	//Saves All Item Count of the bag
	int m_iGambleBagCount; //0x260
	
	//Select Item, Level Rate for Common Item only
	int m_iDropRatePerItemCount[GAMBLE_MAX_ITEM_INFO]; //0x264
	
	//It is used only on void Load... (webzen mistake?)
	int m_iRateKindCount;			//0x2C8
	
	//It Holds all item data excluding those Special
	CItemBagAttrEx BagObject[GAMBLE_MAX_ITEM]; //20C
};

extern CGambleSystem	g_GambleSystem;

#endif // !defined(AFX_GAMBLESYSTEM_H__FD1B162D_3B86_44CF_9A91_0BCE55602DB5__INCLUDED_)
