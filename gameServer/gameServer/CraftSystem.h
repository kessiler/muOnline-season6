#pragma once
// -------------------------------------------------------------------------------

#ifdef __NOVUS__
// -------------------------------------------------------------------------------

#include "CraftSystemDefine.h"
// -------------------------------------------------------------------------------

class CraftSystem
{
public:
			CraftSystem();
	virtual ~CraftSystem();
	// ----
	void	Init();
	void	Load();
	void	ReadStageData(char * File);
	void	ReadMainData(char * File);
	// ----
	bool	CheckStageReq(LPOBJ lpUser);
	bool	CheckQuestReq(LPOBJ lpUser);
	bool	SearchQuestItem(LPOBJ lpUser, BYTE QuestItemID);
	bool	DeleteQuestItem(LPOBJ lpUser, BYTE QuestItemID);
	bool	CheckAllReq(LPOBJ lpUser);
	// ----
	bool	Dialog(LPOBJ lpUser, LPOBJ lpNpc);
	// ----
	void	SendCraftData(LPOBJ lpUser, bool ByDialog);
	void	FinishQuestReq(LPOBJ lpUser);
	// ----
private:
	int		m_StageLoadedCount;
	WORD	m_NpcID;
	BYTE	m_NpcMap;
	BYTE	m_NpcX;
	BYTE	m_NpcY;
	CRAFT_STAGE_DATA	m_StageData[MAX_CRAFT_STAGE_COUNT];
	// ----
}; extern CraftSystem g_CraftSystem;
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------