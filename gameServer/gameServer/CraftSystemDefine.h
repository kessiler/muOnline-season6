#pragma once
// -------------------------------------------------------------------------------

#include "..\include\prodef.h"
// -------------------------------------------------------------------------------

#define CRAFTSYSTEM_DEBUG			0
#define MAX_CRAFT_LEVEL				15
#define MAX_CRAFT_STAGE_COUNT		MAX_CRAFT_LEVEL + 1 //+1 week quest
#define MAX_CRAFT_STAGE_QUEST		5
#define MAX_CRAFT_QUEST_ITEM		5
// -------------------------------------------------------------------------------

struct CRAFT_REQ_USERDATA
{
	PBMSG_HEAD2 h;
};
// -------------------------------------------------------------------------------

struct CRAFT_ANS_USERDATA
{
	PBMSG_HEAD2 h;
	bool		ByDialog;
	WORD		CraftLevel;
	WORD		CraftStage;
	WORD		CraftQuest;
	bool		ItemCheck[MAX_CRAFT_QUEST_ITEM];
};
// -------------------------------------------------------------------------------

struct CRAFT_ANS_QUESTFINISH
{
	PBMSG_HEAD2 h;
	bool		Result;
};
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

namespace CraftProtocol
{
	enum T
	{
		UserData	= 0,
		QuestFinish	= 1,
		Case		= 0xFB,
	};
};
// -------------------------------------------------------------------------------