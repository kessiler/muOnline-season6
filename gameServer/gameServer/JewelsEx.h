#pragma once
// -------------------------------------------------------------------------

#define	JEWELSEX_ITEMS	5
#define JEWELSEX_DEBUG	0
// -------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------

struct JEWELSEX_DATA
{
	int		ItemID;
	BYTE	SuccessRate;
	BYTE	AllowAccessory;
	BYTE	AllowWings;
	BYTE	AddLevel;
	BYTE	MinLevel;
	BYTE	MaxLevel;
	BYTE	AddSkill;
	BYTE	AddLuck;
	BYTE	AddOption;
	BYTE	AddExc;
	BYTE	AddExcOption;
	BYTE	MaxExcOptionCount;
};
// -------------------------------------------------------------------------

class JewelsEx
{
public:
			JewelsEx();
			~JewelsEx();
	// ----
	void	Init();
	void	Load();
	void	ReadData(char * File);
	// ----
	int		GetJewelID(int ItemID);
	bool	IsJewelEx(int ItemID);
	BYTE	GetExcOptionCount(BYTE ExcellentOption);
	BYTE	GetExcUnusedOption(BYTE ExcellentOption);
	BYTE	GetExcNewOption(BYTE Amount);
	bool	IsWings(int ItemID);
	bool	IsAccessory(int ItemID);
	bool	GetInsertFlag(LPOBJ lpUser, BYTE ID, BYTE Target);
	bool	CheckItem(LPOBJ lpUser, BYTE ID, BYTE Target);
	void	Upgrade(LPOBJ lpUser, BYTE Source, BYTE Target);
	// ----
private:
	int		m_LoadedCount;
	JEWELSEX_DATA m_Data[JEWELSEX_ITEMS];
	// ----
}; extern JewelsEx g_JewelsEx;
// -------------------------------------------------------------------------