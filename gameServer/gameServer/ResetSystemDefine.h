#pragma once
// -------------------------------------------------------------------------------

#include "..\include\prodef.h"
// -------------------------------------------------------------------------------

#define	RESETSYSTEM_DEBUG	0
#define	MAX_RESET_GROUP		500
#define MAX_RESET_ITEM		5
// -------------------------------------------------------------------------

struct RESET_REQ_USERDATA
{
	PBMSG_HEAD2 h;
};
// -------------------------------------------------------------------------------

struct RESET_ANS_USERDATA
{
	PBMSG_HEAD2 h;
	bool		ByDialog;
	int			Reset;
	int			GrandReset;
	bool		ItemCheck[MAX_RESET_ITEM];
};
// -------------------------------------------------------------------------------

struct RESET_ITEM_DATA
{
	WORD	ID;
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
// -------------------------------------------------------------------------

struct RESET_REWARD_DATA
{
	int		LevelPoint;
	int		GensPoint;
	int		WCoinC;
	int		WCoinP;
	int		GoblinPoint;
};
// -------------------------------------------------------------------------

struct RESET_GROUP_DATA
{
	int		MinReset;
	int		MaxReset;
	int		ReqLevel;
	int		ReqMoney;
	int		ItemCount;
	RESET_ITEM_DATA		ItemData[MAX_RESET_ITEM];
	RESET_REWARD_DATA	RewardData[MAX_TYPE_PLAYER];
};
// -------------------------------------------------------------------------

namespace ResetProtocol
{
	enum T
	{
		UserData	= 5,
		ResetFinish	= 6,
		Case		= 0xFB,
	};
};
// -------------------------------------------------------------------------------