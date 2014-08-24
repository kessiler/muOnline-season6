#pragma once
// -------------------------------------------------------------------------------

#include "MapClass.h"
#include "StdAfx.h"
#ifdef __ALIEN__
#define ZEN_SHOP	".::ZEN SHOP::."
#define WCOIN_SHOP	".::WCOIN SHOP::."
#define SOUL_SHOP	".::SOUL SHOP::."
#endif
// -------------------------------------------------------------------------------

class OfflineTrade
{
public:
			OfflineTrade();
			~OfflineTrade();
	// ----
	void	Init();
	void	Load();
	void	ReadData(char * File);
	// ----
	bool	CheckReq(int UserIndex);
#ifdef __ALIEN__
	void	Start(int UserIndex, int Type);
#else
	void	Start(int UserIndex);
#endif
	bool	Update(int UserIndex, BYTE Type);
	void	Close(char * AccountID);
#ifdef __ALIEN__
	bool	CheckExchange(LPOBJ lpUser, LPOBJ lpTargetUser, int ItemPos);
	void	UpdateExchange(LPOBJ lpUSer, LPOBJ lpTargetUser, int dwCost);
	int		GetSoulCount(int UserIndex);
	int		CheckItem(int UserIndex, int ItemID, BYTE ItemLevel);
	bool	DeleteSoulCount(int UserIndex, int SoulCount);
	bool	EmptyPointInventory(LPOBJ lpUser, int NeedPoint);
	void	AddedSoul(LPOBJ lpUSer, int SoulCount);
#endif
	// ----
private:
	WORD	m_ReqLevel;
	WORD	m_ReqReset;
	bool	m_UseMapList;
	DWORD	m_Cost[4];
	BYTE	m_MapList[MAX_NUMBER_MAP];
	BYTE	m_LoadedMapList;
	// ----

#ifdef __ALIEN__
public:
	bool EnableZen;
	bool EnableWCoin;
	bool EnableSoul;
#endif
}; extern OfflineTrade g_OfflineTrade;
// -------------------------------------------------------------------------------