#pragma once
// -------------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------------

#define MAX_PKCLEAR_ITEM	10
// -------------------------------------------------------------------------------

struct PKCLEAR_ITEM_DATA
{
	WORD	ID;
	BYTE	MinLevel;
	BYTE	MaxLevel;
	BYTE	MinDur;
	BYTE	MaxDur;
};
// -------------------------------------------------------------------------------

class PKClear
{
public:
			PKClear();
			~PKClear();
	// ----
	void	Init();
	void	Load();
	void	ReadData(char * File);
	// ----
	bool	SearchItem(LPOBJ lpUser, BYTE QuestItemID);
	bool	DeleteItem(LPOBJ lpUser, BYTE QuestItemID);
	bool	CheckReq(LPOBJ lpUser);
	// ----
	bool	Dialog(LPOBJ lpUser, LPOBJ lpNpc);
	// ----
private:
	WORD	m_NpcID;
	BYTE	m_NpcMap;
	BYTE	m_NpcX;
	BYTE	m_NpcY;
	BYTE	m_ReqPK;
	WORD	m_ReqLevel;
	WORD	m_ReqReset;
	DWORD	m_Cost[4];
	PKCLEAR_ITEM_DATA m_ItemData[MAX_PKCLEAR_ITEM];
	BYTE	m_ItemLoaded;
	// ----
}; extern PKClear g_PKClear;
// -------------------------------------------------------------------------------