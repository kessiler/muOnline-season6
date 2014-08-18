#pragma once
// -------------------------------------------------------------------------------

#include "ProtocolDefine.h"
// -------------------------------------------------------------------------------

#define MAX_CRAFT_LEVEL				15
#define MAX_CRAFT_STAGE_COUNT		MAX_CRAFT_LEVEL + 1 //+1 week quest
#define MAX_CRAFT_STAGE_QUEST		5
#define MAX_CRAFT_QUEST_ITEM		5
#define MAX_CRAFT_TEXT_LINE			100
// -------------------------------------------------------------------------------

struct CRAFT_QUEST_ITEM
{
	WORD	ID;
	bool	IsCrafted;
	BYTE	MinLevel;
	BYTE	MaxLevel;
	BYTE	MinOption;
	BYTE	MaxOption;
	BYTE	IsLuck;
	BYTE	IsSkill;
	bool	IsExcellent;
	bool	IsAncient;
	bool	IsSocket;
};
// -------------------------------------------------------------------------------

struct CRAFT_QUEST_DATA
{
	char	QuestName[80];
	BYTE	ItemCount;
	int		ReqMoney;
	CRAFT_QUEST_ITEM ItemData[MAX_CRAFT_QUEST_ITEM];
};
// -------------------------------------------------------------------------------

struct CRAFT_STAGE_DATA
{
	char	StageName[80];
	BYTE	QuestCount;
	int		ReqLevel;
	int		ReqReset;
	CRAFT_QUEST_DATA QuestData[MAX_CRAFT_STAGE_QUEST];
};
// -------------------------------------------------------------------------------

struct CRAFT_ANS_USERDATA
{
	PBMSG_HEAD2 h;
	bool		ByDialog;
	WORD		CraftLevel;
	WORD		CraftStage;
	WORD		CraftQuest;
	bool		ItemCheck[5];
};
// ----------------------------------------------------------------------------------------------

struct CRAFT_ANS_QUESTFINISH
{
	PBMSG_HEAD2 h;
	bool		Result;
};
// ----------------------------------------------------------------------------------------------

struct CRAFT_REQ_QUESTFINISH
{
	PBMSG_HEAD2 h;
};
// ----------------------------------------------------------------------------------------------

class CraftSystem
{
public:
			CraftSystem();
	virtual ~CraftSystem();
	// ----
	void	Load();
	void	ReadStageData(char * File);
	// ----
	bool	IsNotCrafter();
	BYTE	CheckStageReq();
	bool	CheckQuestReq();
	bool	CheckAllReq();
	// ----
	void	SetData(CRAFT_ANS_USERDATA * aRecv);
	void	ReqQuestFinish();
	void	GetQuestResult(CRAFT_ANS_QUESTFINISH * aRecv);
	// ----
	CRAFT_STAGE_DATA	m_StageData[MAX_CRAFT_STAGE_COUNT];
	// ----
}; extern CraftSystem gCraftSystem;
// -------------------------------------------------------------------------------