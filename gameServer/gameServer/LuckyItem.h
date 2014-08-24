//-> Decompiled by DarkSim | 10.03.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "..\include\prodef.h"
#include "user.h"
// -------------------------------------------------------------------------------

#define LUCKYITEM_DEBUG					0
// -------------------------------------------------------------------------------

#define	MAX_LUCKYINFO_LEVELRATE			16
#define MAX_LUCKYINFO_ITEMEQUIPMENT		7
#define	MAX_LUCKYINFO_SMELTRATE			10
// -------------------------------------------------------------------------------

struct _ITEM_LEVEL_RATE	//-> 8
{
	int	m_Level;
	int	m_Rate;
	// ----
	_ITEM_LEVEL_RATE::_ITEM_LEVEL_RATE()	//-> 005aecd0
	{
		this->m_Level	= 0;
		this->m_Rate	= 0;
	}
};
// -------------------------------------------------------------------------------

struct _LUCKY_ITEM_EQUIPMENT	//-> 20
{
	int	m_Index;
	int	m_Skill;
	int	m_Luck;
	int	m_Option;
	int	m_Class;
	// ----
	_LUCKY_ITEM_EQUIPMENT::_LUCKY_ITEM_EQUIPMENT()	//-> 005aed10
	{
		this->m_Index	= -1;
		this->m_Skill	= 0;
		this->m_Luck	= 0;
		this->m_Option	= 0;
		this->m_Class	= -1;
	}
};
// -------------------------------------------------------------------------------

struct _SMELT_RATE	//-> 12
{
	int	MinDur;
	int	MaxDur;
	int	SmeltRate;
	// ----
	_SMELT_RATE::_SMELT_RATE()	//-> 005aed70
	{
		this->MinDur	= 0;
		this->MaxDur	= 0;
		this->SmeltRate	= 0;
	}
};
// -------------------------------------------------------------------------------

struct _LUCKY_ITEM_INFO	//-> 420
{
	int						ItemIndex;
	int						ItemKind;
	int						AddOpt3Rate[4];
	int						AddSetOptRate[2];
	_ITEM_LEVEL_RATE		m_ItemLevelRandRate[MAX_LUCKYINFO_LEVELRATE];
	_LUCKY_ITEM_EQUIPMENT	m_ItemEquipment[MAX_LUCKYINFO_ITEMEQUIPMENT];
	_SMELT_RATE				m_ItemSmeltRate[MAX_LUCKYINFO_SMELTRATE];
	// ----
	_LUCKY_ITEM_INFO::_LUCKY_ITEM_INFO()	//-> 005aec20
	{
		this->ItemIndex	= -1;
		this->ItemKind	= -1;
		memset(this->AddOpt3Rate, 0, sizeof(AddOpt3Rate));
	}
};
// -------------------------------------------------------------------------------

struct PMSG_LUCKYITME_DB_INFO
{
	WORD	wItemCode;
	DWORD	dwSerial;
	WORD	wDurabilitySmall;
};
// -------------------------------------------------------------------------------

struct PMSG_REQ_LUCKYITEM_INSERT	//-> 32 (original)
{
	PBMSG_HEAD2 head;
	// ----
	DWORD	dwUserGuid;
	char	szCharName[MAX_IDSTRING + 1];
	// ----
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo;
};
// -------------------------------------------------------------------------------

struct PMSG_REQ_LUCKYITEM_INSERT_2ND	//-> (original)
{
	PBMSG_HEAD2 head;
	// ----
	DWORD	dwUserGuid;
	BYTE	btItemCnt;
	char	szCharName[MAX_IDSTRING + 1];
	// ----
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo[5];
};
// -------------------------------------------------------------------------------

struct PMSG_REQ_LUCKYITEM_DELETE	//-> 24 (original)
{
	PBMSG_HEAD2 head;
	// ----
	WORD	wUserIndex;
	char	szCharName[MAX_IDSTRING+1];
	WORD	wItemCode;
	DWORD	dwSerial;
};
// -------------------------------------------------------------------------------

struct PMSG_ANS_LUCKYITEM_SELECT	//-> (original)
{
	PWMSG_HEAD	head;
	// ----
	WORD	wUserIndex;
	BYTE	btResultCode;
	BYTE	btItemCnt;
};
// -------------------------------------------------------------------------------

struct PMSG_REQ_LUCKYITEM_SELECT	//-> 18 (original)
{
	PBMSG_HEAD2 head;
	// ----
	WORD	wUserIndex;
	char	chCharacterName[MAX_IDSTRING+1];
};
// -------------------------------------------------------------------------------

struct LuckyItemManager	//-> Complete
{
public:
	LuckyItemManager();
	~LuckyItemManager();
	// ----
	int		LoadLuckyItemEquipment(int iType, _LUCKY_ITEM_INFO * pLuckyItemInfo);
	int		LoadLuckyItemInfo(char * lpszFileName);
	void	AddLuckyItemInfo(_LUCKY_ITEM_INFO * pLuckyItemInfo, int iItemCnt);
	// ----
	_LUCKY_ITEM_INFO * GetLuckyItemInfo(int iItemNum);
	_LUCKY_ITEM_INFO * GetCharClassLuckyItemInfo(int iItemNum, int iCharClass);
	_LUCKY_ITEM_EQUIPMENT * GetLuckyItemEquipment(_LUCKY_ITEM_INFO * pLuckyItemInfo, int iCharClass);
	// ----
	int		IsLuckyItemTicket(int iItemNum);
	int		IsLuckyItemEquipment(int iItemNum);
	// ----
	_SMELT_RATE * GetSmeltingRateInfo(int iItemNum);
	// ----
	void	LuckyItemTicketExchange(OBJECTSTRUCT * lpObj);
	BYTE	GambleGetLevel(_ITEM_LEVEL_RATE * pItemLevelRandRate, int iIndex);
	void	LuckyItemSmelting(OBJECTSTRUCT * lpObj);
	int		GambleLuckyItemSmelting(int iItemNum, int iDur);
	int		GetLuckyItemDurabilityTime();
	BYTE	GetLevel(int iMinLv, int iMaxLv);
	int		LuckyItemRepaire(OBJECTSTRUCT * lpObj, int source, int target);
	// ----
	int		GDReqLuckyItemInsert(CItem * pItem, int iObjIndex);
	int		GDReqLuckyItemInsert(short wItemCode, long dwSerial, short wDurabilitySmall, int iObjIndex);
	void	GDReqLuckyItemInsert2nd(int iObjIndex);
	void	GDReqLuckyItemDelete(short wItemCode, long dwSerial, int iObjIndex);
	void	DGAnsLuckyItemList(PMSG_ANS_LUCKYITEM_SELECT * lpRecv);
	void	GDReqLuckyItemSelect(OBJECTSTRUCT * lpObj);
	// ----
private:
	int									TicketCount;
	std::map<int, _LUCKY_ITEM_INFO>		LuckyItemInfo;
	// ----
}; extern LuckyItemManager g_LuckyItemManager;
// -------------------------------------------------------------------------------