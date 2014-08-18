#pragma once
// ----------------------------------------------------------------------------------------------

#define ITEM_INTER					1171
#define ITEM(x, y)					((x * 512) + y)
#define ITEM2(x, y)					((x * 512) + y + ITEM_INTER)
#define Color4f(r, b, g, o)			((o << 24) + (g << 16) + (b << 8) + r)
#define Naked(Function)				void __declspec(naked) Function()
#define _FC							__fastcall
#define _CD							__cdecl
#define IS_NEWWINGS(x)				((x < ITEM(12, 180)) ? FALSE : (x > ITEM(12, 186)) ? FALSE : TRUE)
#define CS_GET_WEAPON(x, y)			( y | (16 * (x & 0xF0)) )
#define CS_GET_HELM(x, y, z)		( (32 * (z & 0xF)) + 16 * ((x >> 7) & 1) + (y >> 4) )
#define CS_GET_ARMOR(x, y, z)		( (32 * ((z >> 4) & 0xF)) + 16 * ((x >> 6) & 1) + (y & 0xF) )
#define CS_GET_PANTS(x, y, z)		( (32 * (z & 0xF)) + 16 * ((x >> 5) & 1) + (y >> 4) )
#define CS_GET_GLOVES(x, y, z)		( (32 * ((z >> 4) & 0xF)) + 16 * ((x >> 4) & 1) + (y & 0xF) )
#define CS_GET_BOOTS(x, y, z)		( (32 * (z & 0xF)) + 16 * ((x >> 3) & 1) + (y >> 4) )
#define MAX_ITEMOPT_BIT				0x3F
#define GET_ITEMOPT_LEVEL(x)		((x >> 3) & 0xF)
#define GET_ITEMOPT0(x)				(x & 3)
#define GET_ITEMOPT16(x)			(x & 0x40)
#define GET_ITEMOPT_SKILL(x)		(x >> 7)
#define GET_ITEMOPT_LUCK(x)			((x >> 2) & 1)
#define GET_ITEMOPT_0(x)			(x & 1)
#define GET_ITEMOPT_1(x)			(((x & MAX_ITEMOPT_BIT) >> 1) & 1)
#define GET_ITEMOPT_2(x)			(((x & MAX_ITEMOPT_BIT) >> 2) & 1)
#define GET_ITEMOPT_3(x)			(((x & MAX_ITEMOPT_BIT) >> 3) & 1)
#define GET_ITEMOPT_4(x)			(((x & MAX_ITEMOPT_BIT) >> 4) & 1)
#define GET_ITEMOPT_5(x)			(((x & MAX_ITEMOPT_BIT) >> 5) & 1)
// ----------------------------------------------------------------------------------------------

enum eObjectType	//-> Complete
{
	emPlayer	= 1,
	emMonster	= 2,
	emNPC		= 4,
	emNone		= 8,
};
// ----------------------------------------------------------------------------------------------

enum eClassNumber 
{
	CLASS_WIZARD	= 0,
	CLASS_KNIGHT	= 1,
	CLASS_ELF		= 2,
	CLASS_MAGUMSA	= 3,
	CLASS_DARKLORD	= 4,
	CLASS_SUMMONER	= 5,
	CLASS_MONK		= 6,
};
// ----------------------------------------------------------------------------------------------

enum eClassCode // -> Complete -> Season 5.2 JPN
{
	DarkWizard 		= 0,
	SoulMaster 		= 8,
	GrandMaster		= 24,
	// ----
	DarkKnight		= 1,
	BladeKnight		= 9,
	BladeMaster		= 25,
	// ----
	Elf				= 2,
	MuseElf			= 10,
	HightElf		= 26,
	// ----
	MagicGladiator	= 3,
	DuelMaster		= 19,
	// ----
	DarkLord		= 4,
	LordEmperor		= 20,
	// ----
	Summoner		= 5,
	BloodySummoner	= 13,
	DimensionMaster	= 29,
	// ----
	Monk			= 6,
	FirstMaster		= 16
};
// ----------------------------------------------------------------------------------------------

enum ObjState // -> Complete
{
	SelectServer	= 2,
	SwitchCharacter = 4,
	GameProcess		= 5,
};
// ----------------------------------------------------------------------------------------------

enum ObjWindow // -> Season 6.3
{
	FriendList		= 1,
	MoveList		= 2,
	Party			= 3,
	Guild			= 6,
	Trade			= 7,
	Warehouse		= 8,
	ChaosBox		= 9,
	CommandWindow	= 10,
	PetInfo			= 11,
	Shop			= 12,
	Inventory		= 13,
	Store			= 14,
	OtherStore		= 15,
	Character		= 16,
	ChatWindow		= 33,
	FastMenu		= 34,
	Options			= 35,
	Help			= 36,
	FastDial		= 39,
	SkillTree		= 57,
	NPC_Titus		= 63,
	CashShop		= 65,
	FullMap			= 72,
	NPC_Dialog		= 73,
	GensInfo		= 74,
	NPC_Julia		= 75,
	ExpandInventory	= 77,
	ExpandWarehouse	= 78,
};
// ----------------------------------------------------------------------------------------------

namespace ASM
{
	enum T
	{
		JMP			= 0xE9,
		JE			= 0x74,
		JNE			= 0x75,
		JGE			= 0x7D,
		NOP			= 0x90,
		CALL		= 0xE8,
		PUSH		= 0x68,
	};
};
// ----------------------------------------------------------------------------------------------

enum eTextColors	//-> Complete
{
	White			= 0x00,
	Blue			= 0x01,
	Red				= 0x02,
	Gold			= 0x03,
	Green			= 0x04,
	WhiteRed		= 0x05,
	Violet			= 0x06,
	WhiteBlue		= 0x07,
	WhiteBeige		= 0x08,
	GreenBlue		= 0x09,
	Gray			= 0x0A,
	DarkViolet		= 0x0B,
};
// ----------------------------------------------------------------------------------------------

enum eTextWeight	//-> Complete
{
	Bold			= 0x01,
	Normal			= 0x00,
};
// ----------------------------------------------------------------------------------------------

namespace eItemOption
{
	enum T
	{
		Damage			= 80,		//-> Additional Damage +<N>
		WizardryDamage	= 81,
		CurseSpell		= 82,
		Defense			= 84,		//-> Additional Defense +<N>
		Luck			= 85,		//-> Default: 0
		HPRecovery		= 86,
		// ----
		LifeIncrease	= 87,		//-> Increase Max HP +4%%
		ManaIncrease	= 88,		//-> Increase Max Mana +4%%
		DamageDecrease	= 89,		//-> Damage Decrease +4%%
		Reflect			= 90,		//-> Reflect Damage +5%%
		DefenseSuccess	= 91,		//-> Defense success rate +10%%
		IncreaseZen		= 92,		//-> Increases acquisition rate of Zen after hunting monsters +30%%
		// ----
		ExcellentDamage	= 93,		//-> Excellent Damage rate +10%%
		DamageByLevel	= 94,		//-> Increase Damage +level/20
		IncreaseDamage	= 95,		//-> Increase Damage +%d%%
		IncreaseWizardry= 96,		//-> Increase Wizardry Dmg +level/20
		IncreaseSpeed	= 98,		//-> Increase Attacking(Wizardry)speed +%d
		IgnorOpDefense	= 103,
		ReturnAttack	= 106,		//-> Return's the enemy's attack power in <N>%
		CompleteLife	= 107,		//-> Complete recovery of life in <N>% rate
		CompleteMana	= 108,		//-> Complete recovery of Mana in <N>% rate
	};
};
// ----------------------------------------------------------------------------------------------