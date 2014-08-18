#pragma once
// ----------------------------------------------------------------------------------------------

#include "ProtocolDefine.h"
// ----------------------------------------------------------------------------------------------

#define	MAX_XOR_KEYS		32
#define MAX_CHECK_VARS		10
#define MAX_STAT_LIMIT		300
#define MAX_SPEED_LIMIT		100
#define MAX_FRAME_VALUE		40
// ----------------------------------------------------------------------------------------------

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
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CHEATGUARD_REQ_CHECK
{
	PBMSG_HEAD	Head;
	// ----
	short		Level;
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
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CHEATGUARD_ANS_DATA
{
	bool		Status;
	int			Value;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CHEATGUARD_ANS_CHECK
{
	PBMSG_HEAD	Head;
	// ----
	CHEATGUARD_ANS_DATA Data[MAX_CHECK_VARS];
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

class CheatGuard
{
public:
	void	Load();
	// ----
	void	ChangeXORFilter();
	bool	IsCorrectFrame();
	int		GetLargerFrame();
	void	Check(CHEATGUARD_REQ_CHECK * pRequest);
	// ----
	BYTE	XOR[MAX_XOR_KEYS];
	// ----
}; extern CheatGuard gCheatGuard;
// ----------------------------------------------------------------------------------------------