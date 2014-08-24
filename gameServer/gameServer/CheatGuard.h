#pragma once
// -------------------------------------------------------------------------------

#define MAX_CHECK_VARS		10
// -------------------------------------------------------------------------------

#include "..\include\prodef.h"
// -------------------------------------------------------------------------------

enum eCheckStatMember
{
	emLevel			= 0,
	emLevelPoint	= 1,
	emStrength		= 2,
	emDexterity		= 3,
	emVitality		= 4,
	emEnergy		= 5,
	emLeadership	= 6,
	emAttackSpeed	= 7,
	emMagicSpeed	= 8,
	emFrameValue	= 9,
};
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CHEATGUARD_REQ_CHECK
{
	PBMSG_HEAD	Head;
	// ----
	WORD		Level;
	int			LevelPoint;
	WORD		Strength;
	WORD		Dexterity;
	WORD		Vitality;
	WORD		Energy;
	WORD		Leadership;
	int			AttackSpeed;
	int			MagicSpeed;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CHEATGUARD_ANS_DATA
{
	bool		Status;
	int			Value;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CHEATGUARD_ANS_CHECK
{
	PBMSG_HEAD	Head;
	// ----
	CHEATGUARD_ANS_DATA Data[MAX_CHECK_VARS];
};
#pragma pack(pop)
// -------------------------------------------------------------------------------


class CheatGuard
{
public:
			CheatGuard();
			~CheatGuard();
	// ----
	void	Init();
	void	ReadXOR(char * File);
	void	ReadMain(char * File);
	void	Load();
	// ----
	void	Disconnect(int aIndex);
	void	GenerateFileName(char * Buff);
	void	WriteLog(char * Text, ...);
	void	GetCheckResult(CHEATGUARD_ANS_CHECK * pResult, int aIndex);
	void	SendCheckRequest(int aIndex);
	// ----
	BYTE	XOR[32];
	// ----
public:
	bool	m_UseDisconnect;
	int		m_PacketsPerSecond;
	bool	m_SpeedHackCheck;
	// ----
}; extern CheatGuard g_CheatGuard;
// -------------------------------------------------------------------------------