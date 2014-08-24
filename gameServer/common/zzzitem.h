#ifndef ZZZITEM_H
#define ZZZITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>

#define MAX_TYPE_PLAYER 7
#define MAX_ITEM_LEVEL	15
#define MAX_TYPE_ITEMS 16
#define MAX_SUBTYPE_ITEMS 512
#define MAX_EXOPTION_SIZE 8
#define ITEMGET(x,y) ( (x)*MAX_SUBTYPE_ITEMS + (y))
#define MAX_PET_LEVEL 50
#define MAX_ITEM_DURABILITY	255
#define MAX_ITEMS (MAX_TYPE_ITEMS*MAX_SUBTYPE_ITEMS)
#define MAX_ITEM_SPECIAL_ATTRIBUTE 7
#define MAX_SELL_PRICE	1200000000

#define DBI_GET_TYPE(x) ( ((x)&((MAX_TYPE_ITEMS-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define DBI_GET_LEVEL(x)  ( ((x)>>3)& MAX_ITEM_LEVEL )
#define DBI_GET_SKILL(x)  ( (x) >> 7 )
#define DBI_GET_LUCK(x)  ( ((x) >> 2) & 0x01)
#define DBI_GET_OPTION(x)  ( ( (x) ) & 0x03 )
#define DBI_GET_OPTION16(x) ( (x) & 0x40  )
#define DBI_GET_DINO_OPTION(x)  ( DBI_GET_OPTION16((x)) >> 4 )
#define DBI_GET_NOPTION(x)  ( (x) & 0x3F  )
#define DBI_GET_380OPTION(x) ( ((x) & 0x08) << 4  )
#define ITEM_GET_TYPE(x)  ( (x)/MAX_SUBTYPE_ITEMS  )
#define ITEM_GET_INDEX(x) ( (x)%MAX_SUBTYPE_ITEMS  )

#define DBI_TYPE			0
#define DBI_OPTION_DATA		1
#define DBI_DUR				2
#define DBI_SERIAL1			3
#define DBI_SERIAL2			4
#define DBI_SERIAL3			5
#define DBI_SERIAL4			6
#define DBI_NOPTION_DATA	7
#define DBI_SOPTION_DATA	8
#define DBI_OPTION380_DATA	9
#define DBI_JOH_DATA		10

#define MAX_ITEM_INFO	(7+5) //7 Common + 5 Socket

/*#define I_TYPE		0
#define I_OPTION	1
#define I_DUR		2
#define I_NOPTION	3
#define I_SOPTION	4
#define I_380OPTION	5
#define I_JOHOPTION	6
*/
#define ITEM_DUR_RANGE(x) (((x)<0)?FALSE:((x)>MAX_ITEM_DURABILITY-1)?FALSE:TRUE )

typedef struct
{
	char Name[32]; // 0
	BYTE HaveItemInfo; // 20  {A8}
	BYTE TwoHand;	// 21 {A9}
	BYTE Level; // 22
	BYTE Width;	// 23
	BYTE Height;	// 24
	char Serial; // 25
	BYTE OptionFlag; // 26
	BYTE MondownFlag; // 27 {AF}
	BYTE AttackSpeed; // 28 {B0}
	BYTE WalkSpeed; // 29 {B1}
	BYTE DamageMin; // 2A
	BYTE DamageMax; // 2B
	BYTE SuccessfulBlocking; // 2C
	BYTE Defense; // 2D
	BYTE MagicDefense; // 2E
	BYTE Speed;	// 2F
	BYTE Durability; // 30
	BYTE MagicDurability; // 31 {B9}
	BYTE AttackDur;	// 32 {BA}
	BYTE DefenceDur; // 33 {BC}
	WORD RequireStrength; // 34 {BD}
	WORD RequireDexterity; // 36 {BF}
	WORD RequireEnergy; // 38 {C1}
	WORD RequireLevel; // 3A {C3}
	WORD Value; // 3C {C5}
	BYTE RequireClass[MAX_TYPE_PLAYER]; // 3E {C7}
	BYTE Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE]; // 44 {CD}
	WORD RequireVitality;	// 4C {D5}
	int BuyMoney; // 50  {D8}
	int MagicPW; // 54
	float RepaireMoneyRate;	// 58
	float AllRepaireMoneyRate; // 5C
	bool QuestItem;	// 60
	BYTE SetAttr; // 61
	BYTE ResistanceType;	// 62
	int ItemSlot; // 64
	int SkillType; // 68
	int RequireLeadership;	// 6C

}  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;

extern  ITEM_ATTRIBUTE ItemAttribute[MAX_ITEMS];


namespace MuItem
{
	enum ItemNumber
	{
		miDIVINE_SWORD = ITEMGET(0, 19),
		miDIVINE_SCEPTER = ITEMGET(2, 13),
		miDIVINE_CROSSBOW = ITEMGET(4, 18),
		miDIVINE_STAFF = ITEMGET(5, 10),
		miDIVINE_STICK = ITEMGET(5, 36),


		miCHAOS_WEAPON_AXE_DRAGON = ITEMGET(2, 6),
		miCHAOS_WEAPON_BOW_NATURE = ITEMGET(4, 6),
		miCHAOS_WEAPON_STAFF_LIGHT = ITEMGET(5, 7),

		miSWORD_CRYSTAL = ITEMGET(2, 5),

		miBOLT = ITEMGET(4, 7),
		miARROW = ITEMGET(4, 15),


		miWINGS_FAIRY = ITEMGET(12, 0),
		miWINGS_HEAVEN = ITEMGET(12, 1),
		miWINGS_SATAN = ITEMGET(12, 2),

		miWINGS_SPIRIT = ITEMGET(12, 3),
		miWINGS_SOUL = ITEMGET(12, 4),
		miWINGS_DRAGON = ITEMGET(12, 5),
		miWINGS_DARKNESS = ITEMGET(12, 6),

		miWINGS_STORM = ITEMGET(12, 36),
		miWINGS_ETERNAL = ITEMGET(12, 37),
		miWINGS_ILLUSION = ITEMGET(12, 38),
		miWINGS_RUIN = ITEMGET(12, 39),
		miCAPE_EMPEROR = ITEMGET(12, 40),
		miWINGS_CURSE = ITEMGET(12, 41),
		miWINGS_DESPAIR = ITEMGET(12, 42),
		miWINGS_DIMENSION = ITEMGET(12, 43),

		miCAPE_FIGTHER = ITEMGET(12, 49),
		miCAPE_OVERRULE = ITEMGET(12, 50),

		miCAPE_LORD = ITEMGET(13, 30),

		miWING_SMALL_CAPE_LORD = ITEMGET(12, 130),
		miWING_SMALL_CURSE = ITEMGET(12, 131),
		miWING_SMALL_FAIRY = ITEMGET(12, 132),
		miWING_SMALL_HEAVEN = ITEMGET(12, 133),
		miWING_SMALL_SATAN = ITEMGET(12, 134),
		miWING_SMALL_CLOAK = ITEMGET(12, 135),


		miORB_TWISTING = ITEMGET(12, 7),
		miORB_HEALING = ITEMGET(12, 8),
		miORB_GREATER_FORTITUDE = ITEMGET(12, 9),
		miORB_GREATER_DAMAGE = ITEMGET(12, 10),
		miORB_SUMMON = ITEMGET(12, 11),
		miORB_RAGEFUL_BLOW = ITEMGET(12, 12),
		miORB_IMPALE = ITEMGET(12, 13),
		miORB_SWELL_LIFE = ITEMGET(12, 14),
		miORB_FIRE_SLASH = ITEMGET(12, 16),
		miORB_PENETRATION = ITEMGET(12, 17),
		miORB_ICE_ARROW = ITEMGET(12, 18),
		miORB_DEATH_STAB = ITEMGET(12, 19),
		miORB_DESTRUCTION = ITEMGET(12, 44),
		miORB_MULTISHOT = ITEMGET(12, 45),
		miORB_RECOVERY = ITEMGET(12, 46),
		miORB_FLAMESTRIKE = ITEMGET(12, 47),

		miSCROLL_FIREBURST = ITEMGET(12, 21),
		miSCROLL_SUMMON = ITEMGET(12, 22),
		miSCROLL_CRITICAL_DMG = ITEMGET(12, 23),
		miSCROLL_ELECTRIC_SPARK = ITEMGET(12, 24),
		miSCROLL_CHAOTIC_DISEIER = ITEMGET(12, 48),


		miHELPER_ANGEL = ITEMGET(13, 0),
		miHELPER_IMP = ITEMGET(13, 1),
		miHELPER_UNIRIA = ITEMGET(13, 2),
		miHELPER_DINORANT = ITEMGET(13, 3),
		miHELPER_DARK_HORSE = ITEMGET(13, 4),
		miHELPER_DARK_RAVEN = ITEMGET(13, 5),
		miHELPER_DEMON = ITEMGET(13, 64),
		miHELPER_SPIRIT_GUARDIAN = ITEMGET(13, 65),

		miHELPER_PET_RUDOLF = ITEMGET(13, 67),
		miHELPER_PET_PANDA = ITEMGET(13, 80),
		miHELPER_PET_UNICORN = ITEMGET(13, 106),
		miHELPER_PET_SKELETON = ITEMGET(13, 123),


		miHELPER_FENRIR = ITEMGET(13, 37),

		miJEWEL_CHAOS = ITEMGET(12, 15),
		miJEWEL_BLESS = ITEMGET(14, 13),
		miJEWEL_SOUL = ITEMGET(14, 14),
		miJEWEL_LIFE = ITEMGET(14, 16),

		miJEWEL_CREATION = ITEMGET(14, 22),
		miJEWEL_GUARDIAN = ITEMGET(14, 31),
		miJEWEL_GEMSTONE = ITEMGET(14, 41),
		miJEWEL_HARMONY = ITEMGET(14, 42),
		miJEWEL_REFINE_LOW = ITEMGET(14, 43),
		miJEWEL_REFINE_HIGH = ITEMGET(14, 44),



		miJEWEL_BUNDLE_BLESS = ITEMGET(12, 30),
		miJEWEL_BUNDLE_SOUL = ITEMGET(12, 31),
		miJEWEL_BUNDLE_LIFE = ITEMGET(12, 136),
		miJEWEL_BUNDLE_CREATION = ITEMGET(12, 137),
		miJEWEL_BUNDLE_GUARDIAN = ITEMGET(12, 138),
		miJEWEL_BUNDLE_GEMSTONE = ITEMGET(12, 139),
		miJEWEL_BUNDLE_HARMONY = ITEMGET(12, 140),
		miJEWEL_BUNDLE_CHAOS = ITEMGET(12, 141),
		miJEWEL_BUNDLE_REFINE_LOW = ITEMGET(12, 142),
		miJEWEL_BUNDLE_REFINE_HIGH = ITEMGET(12, 143),


		miMIX_GEM_OF_SECRET = ITEMGET(12, 26),

		miRING_ICE = ITEMGET(13, 8),
		miRING_POISON = ITEMGET(13, 9),
		miRING_TRANSFORM = ITEMGET(13, 10),
		miRING_WIZARD = ITEMGET(13, 20),
		miRING_FIRE = ITEMGET(13, 21),
		miRING_EARTH = ITEMGET(13, 22),
		miRING_WIND = ITEMGET(13, 23),
		miRING_MAGIC = ITEMGET(13, 24),
		miRING_MOONSTONE = ITEMGET(13, 38),
		miRING_ELITE_SKELETON = ITEMGET(13, 39),
		miRING_JACK_OLANTERN = ITEMGET(13, 40),
		miRING_CHRISTMAS = ITEMGET(13, 41),
		miRING_GM = ITEMGET(13, 42),
		miRING_SEAL_ASCENSION = ITEMGET(13, 43),
		miRING_SEAL_WEALTH = ITEMGET(13, 44),
		miRING_SEAL_SUSTENANCE = ITEMGET(13, 45),
		miRING_SNOWMAN = ITEMGET(13, 68),
		miRING_PANDA = ITEMGET(13, 76),
		miRING_PANDA_BROWN = ITEMGET(13, 77),
		miRING_PANDA_PINK = ITEMGET(13, 78),
		miRING_LETHAL_WIZARD = ITEMGET(13, 107),
		miRING_SAPPHIRE = ITEMGET(13, 109),
		miRING_RUBY = ITEMGET(13, 110),
		miRING_TOPAZ = ITEMGET(13, 111),
		miRING_AMETHYST = ITEMGET(13, 112),
		miRING_SEAL_WEALTH2 = ITEMGET(13, 116),
		miRING_SKELETON = ITEMGET(13, 122),




		miPENDANT_LIGHTNING = ITEMGET(13, 12),
		miPENDANT_FIRE = ITEMGET(13, 13),
		miPENDANT_ICE = ITEMGET(13, 25),
		miPENDANT_WIND = ITEMGET(13, 26),
		miPENDANT_WATER = ITEMGET(13, 27),
		miPENDANT_ABILITY = ITEMGET(13, 28),

		miNECKLACE_RUBY = ITEMGET(13, 113),
		miNECKLACE_EMERALD = ITEMGET(13, 114),
		miNECKLACE_SAPPHIRE = ITEMGET(13, 115),



		miCONTRACT_SUMMON = ITEMGET(13, 7),
		miGUARDIAN_LIFE_STONE = ITEMGET(13, 11),

		miMIX_LOCH_FEATHER = ITEMGET(13, 14),
		miFRUITS = ITEMGET(13, 15),

		miBC_SCROLL_ARCHANGEL = ITEMGET(13, 16),
		miBC_BLOOD_BONE = ITEMGET(13, 17),
		miBC_CLOAK = ITEMGET(13, 18),
		miQUEST_DIVINE_WEAPON = ITEMGET(13, 19),

		miCC_GUARD_ARMOR = ITEMGET(13, 29),

		miMIX_SPIRIT = ITEMGET(13, 31),
		miMIX_SPLINTER_ARMOR = ITEMGET(13, 32),
		miMIX_BLESS_GUARDIAN = ITEMGET(13, 33),
		miMIX_CLAW_BEAST = ITEMGET(13, 34),
		miMIX_FRAGMENT_HORN = ITEMGET(13, 35),
		miMIX_BROKEN_HORN = ITEMGET(13, 36),

		miDS_TICKET = ITEMGET(13, 46),
		miBC_TICKET = ITEMGET(13, 47),
		miKALIMA_TICKET = ITEMGET(13, 48),
		miIT_OLD_SCROLL = ITEMGET(13, 49),
		miIT_COVENANT = ITEMGET(13, 50),
		miIT_BLOOD_SCROLL = ITEMGET(13, 51),
		miMIX_CONDOR_FLAME = ITEMGET(13, 52),
		miMIX_CONDOR_FEATHER = ITEMGET(13, 53),



		miHAWK_FIGURINE = ITEMGET(13, 128),
		miWORN_HORSESHOE = ITEMGET(13, 134),


		miPOTION_APPLE = ITEMGET(14, 0),
		miPOTION_HEALING_SMALL = ITEMGET(14, 1),
		miPOTION_HEALING_MEDIUM = ITEMGET(14, 2),
		miPOTION_HEALING_LARGE = ITEMGET(14, 3),
		miPOTION_MANA_SMALL = ITEMGET(14, 4),
		miPOTION_MANA_MEDIUM = ITEMGET(14, 5),
		miPOTION_MANA_LARGE = ITEMGET(14, 6),

		miPOTION_SIEGE = ITEMGET(14, 7),
		miPOTION_ANTITODE = ITEMGET(14, 8),
		miPOTION_ALE = ITEMGET(14, 9),
		miPOTION_REMEDY_LOVE = ITEMGET(14, 20),
		miPOTION_SD_SMALL = ITEMGET(14, 35),
		miPOTION_SD_MEDIUM = ITEMGET(14, 36),
		miPOTION_SD_LARGE = ITEMGET(14, 37),
		miPOTION_COMPLEX_SMALL = ITEMGET(14, 38),
		miPOTION_COMPLEX_MEDIUM = ITEMGET(14, 39),
		miPOTION_COMPLEX_LARGE = ITEMGET(14, 40),
		miPOTION_HEALING_ELITE = ITEMGET(14, 70),
		miPOTION_MANA_ELITE = ITEMGET(14, 71),
		miPOTION_HEALING_ELITE_MED = ITEMGET(14, 94),


		miTOWN_PORTAL_SCROLL = ITEMGET(14, 10),
		miDS_KEY = ITEMGET(14, 17),
		miDS_EYE = ITEMGET(14, 18),
		miDS_INVITATION = ITEMGET(14, 19),

		miRENA = ITEMGET(14, 21),

		miQUEST_EMPEROR_SCROLL = ITEMGET(14, 23),
		miQUEST_BROKEN_SWORD = ITEMGET(14, 24),
		miQUEST_ELF_TEAR = ITEMGET(14, 25),
		miQUEST_WIZARD_SOUL_SHARD = ITEMGET(14, 26),

		miKALIMA_LOST_MAP = ITEMGET(14, 28),
		miKALIMA_SYMBOL_KUNDUN = ITEMGET(14, 29),


		miPUMPKIN_LUCK = ITEMGET(14, 45),
		miJACK_OLANTERN_BLESS = ITEMGET(14, 46),
		miJACK_OLANTERN_WRATH = ITEMGET(14, 47),
		miJACK_OLANTERN_CRY = ITEMGET(14, 48),
		miJACK_OLANTERN_FOOD = ITEMGET(14, 49),
		miJACK_OLANTERN_DRINK = ITEMGET(14, 50),
		miCHRISTMAS_STAR = ITEMGET(14, 51),

		miFIRECRACKER = ITEMGET(14, 63),

		miQUEST_DEATHBEAM_FLAME = ITEMGET(14, 65),
		miQUEST_HELLMINER_HORN = ITEMGET(14, 66),
		miQUEST_DARKPHOENIX_FEATHER = ITEMGET(14, 67),
		miQUEST_ABYSSAL_EYE = ITEMGET(14, 68),

		miCHERRY_BLOSSOM_WINE = ITEMGET(14, 85),
		miCHERRY_BLOSSOM_RICE_CAKE = ITEMGET(14, 86),
		miCHERRY_BLOSSOM_FLOWER_PETAL = ITEMGET(14, 87),

		miCHERRY_BLOSSOM_BRANCH_GOLD = ITEMGET(14, 90),


		miLUCKY_COIN = ITEMGET(14, 100),

		miIGF_SUSPICIOUS_SCRAP = ITEMGET(14, 101),
		miIGF_GAION_ORDER = ITEMGET(14, 102),
		miIGF_SECROMICON_FRAGMENT_1 = ITEMGET(14, 103),
		miIGF_SECROMICON_FRAGMENT_2 = ITEMGET(14, 104),
		miIGF_SECROMICON_FRAGMENT_3 = ITEMGET(14, 105),
		miIGF_SECROMICON_FRAGMENT_4 = ITEMGET(14, 106),
		miIGF_SECROMICON_FRAGMENT_5 = ITEMGET(14, 107),
		miIGF_SECROMICON_FRAGMENT_6 = ITEMGET(14, 108),
		miIGF_SECROMICON_COMPLETE = ITEMGET(14, 109),

		miDG_DIMENSSION_SIGN = ITEMGET(14, 110),
		miDG_DIMENSSION_MIRROR = ITEMGET(14, 111),

		miMIX_SILVER_KEY = ITEMGET(14, 112),
		miMIX_GOLDEN_KEY = ITEMGET(14, 113),
		miMIX_SEALED_GOLDEN_BOX = ITEMGET(14, 121),
		miMIX_SEALED_SILVER_BOX = ITEMGET(14, 122),
		miGOLDEN_BOX = ITEMGET(14, 123),
		miSILVER_BOX = ITEMGET(14, 124),

		miJEWELRY_CASE_SHINE = ITEMGET(14, 141),
		miJEWELRY_CASE_ELEGANT = ITEMGET(14, 142),
		miJEWELRY_CASE_STEEL = ITEMGET(14, 143),
		miJEWELRY_CASE_OLD = ITEMGET(14, 144),

		miREQUEST_LETTER_LV1 = ITEMGET(14, 151),

		miBOX_PURPLE = ITEMGET(14, 159),

	};


	enum GroupItemIndex
	{
		miGROUP_SWORD_START = ITEMGET(0, 0),
		miGROUP_SWORD_END = ITEMGET(0, MAX_SUBTYPE_ITEMS-1),

		miGROUP_AXE_START = ITEMGET(1, 0),
		miGROUP_AXE_END = ITEMGET(1, MAX_SUBTYPE_ITEMS-1),

		miGROUP_MACE_SCEPTER_START = ITEMGET(2, 0),
		miGROUP_MACE_SCEPTER_END = ITEMGET(2, MAX_SUBTYPE_ITEMS-1),

		miGROUP_SPEAR_START = ITEMGET(3, 0),
		miGROUP_SPEAR_END = ITEMGET(3, MAX_SUBTYPE_ITEMS-1),

		miGROUP_BOW_CROSSBOW_START = ITEMGET(4, 0),
		miGROUP_BOW_CROSSBOW_END = ITEMGET(4, MAX_SUBTYPE_ITEMS-1),

		miGROUP_STAFF_STICK_START = ITEMGET(5, 0),
		miGROUP_STAFF_STICK_END = ITEMGET(5, MAX_SUBTYPE_ITEMS-1),

		miGROUP_SHIELD_START = ITEMGET(6, 0),
		miGROUP_SHIELD_END = ITEMGET(6, MAX_SUBTYPE_ITEMS-1),

		miGROUP_HELM_START = ITEMGET(7, 0),
		miGROUP_HELM_END = ITEMGET(7, MAX_SUBTYPE_ITEMS-1),

	};
};


namespace MuItemType
{
	enum Type
	{
		mitNONE = -1,
		mitSWORD_TWO_HAND = 0,
		mitSWORD_ONE_HAND = 1,
		mitMACE = 2,
		mitSPEAR = 3,
		mitSTAFF_ONE_HAND = 4,
		mitSTAFF_TWO_HAND = 5,
		mitSHIELD = 6,
		mitBOW = 7,
		mitCROSSBOW = 8,
		mitSTICK = 9,
		mitBOOK = 10,
		mitSCEPTER = 11,
		mitGloves = 12,
	};
};

class CItem
{

public:
	CItem();

//	void Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3,BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE SocketOption[5], BYTE SocketIndex, BYTE DbVersion);
	void Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, BYTE PeriodItemOption, BYTE DbVersion);

	void Value();
	void OldValue();
	int GetSize(int& w, int& h);
	void Clear();
	int IsItem();
	int IsSetItem();
	int GetAddStatType();
	void SetPetItemInfo(int petlevel, int petexp);
	int AddPetItemExp(int petexp);
	int DecPetItemExp(int percent);
	void PetValue();
	int PetItemLevelDown(int exp);
	int ItemDamageMin();
	int ItemDefense();
	int BookSpellDmg();
	int IsClass(char aClass, int ChangeUP, int ChangeUP3rd);
	LPSTR GetName();
	int GetLevel();
	void PlusSpecial(int* Value, int Special);
	void PlusSpecialPercent(int* Value, int Special, WORD Percent);
	void PlusSpecialPercentEx(int* Value, int SourceValue, int Special);
#if( FIX_MAXSTAT == 1 )
	void SetItemPlusSpecialStat(WORD* Value, int Special);
#else
	void SetItemPlusSpecialStat(short* Value, int Special);
#endif
	int GetWeaponType();
	void PlusSpecialSetRing(BYTE* Value);
	DWORD GetNumber();
	int IsExtItem();
	int IsExtLifeAdd();
	int IsExtManaAdd();
	int IsExtDamageMinus();
	int IsExtDamageReflect();
	int IsExtDefenseSuccessfull();
	int IsExtMonsterMoney();
	int IsExtExcellentDamage();
	int IsExtAttackRate();
	int IsExtAttackRate2();
	int IsExtAttackSpeed();
	int IsExtMonsterDieLife();
	int IsExtMonsterDieMana();
	int IsWingOpGetOnePercentDamage();
	int IsWingOpGetManaToMoster();
	int IsDinorantReduceAttackDamaege();
	int IsFenrirIncLastAttackDamage();
	int IsFenrirDecLastAttackDamage();
	int IsFenrirSpecial();
	int IsThirdWingOpGetOnePercentDamage();
	int SimpleDurabilityDown(int iDur);
	int DurabilityDown(int dur, int aIndex);
	int DurabilityDown2(int dur, int aIndex);
	int NormalWeaponDurabilityDown(int defence, int aIndex);
	int BowWeaponDurabilityDown(int defence, int aIndex);
	int StaffWeaponDurabilityDown(int defence, int aIndex);
	int LuckyItemArmorDurabilityDown(int damagemin, int aIndex);
	int ArmorDurabilityDown(int damagemin, int aIndex);
	int CheckDurabilityState();

	bool IsEnableSetSocketItem();
	bool SetSeedSphere(BYTE SlotIndex, BYTE SeedIndex, BYTE SphereLevel);
	bool RemoveSeedSphere(BYTE SlotIndex);
	void MakeSocketSlot(BYTE SlotCount);

	int GetDetailItemType();
	int GetMagicBookProperty();

	bool IsPeriodItem();			//ok
	bool IsPeriodItemExpire();		//ok
	void SetPeriodItem();			//ok
	void SetPeriodItemExpire();		//ok
#ifdef NEWWINGS
	int IsNewWingOpGetOnePercentDamage(); 
#endif

	DWORD m_Number; // 0
	char m_serial;	// 4
	short m_Type; // 6
	short m_Level; // 8
	BYTE m_Part;	// A
	BYTE m_Class;	// B
	BYTE m_TwoHand;	// C
	BYTE m_AttackSpeed;	// D
	BYTE m_WalkSpeed;	// E
	WORD m_DamageMin;	// 10
	WORD m_DamageMax;	// 12
	BYTE m_SuccessfulBlocking;	// 14
	WORD m_Defense;	// 16
	WORD m_MagicDefense;	// 18
	BYTE m_Speed;	// 1A
	WORD m_DamageMinOrigin;	// 1C
	WORD m_DefenseOrigin;	// 1E
	WORD m_Magic;	// 20
	WORD /*m_BookSpell*/m_Curse;	// 22
	float m_Durability; // 24
	WORD m_DurabilitySmall; // 28
	float m_BaseDurability;	// 2C
	BYTE m_SpecialNum;	// 30
	WORD m_Special[8];	// 31  // BYTE -> WORD
	BYTE m_SpecialValue[8];	// 39
	WORD m_RequireStrength;	// 42
	WORD m_RequireDexterity;	// 44
	WORD m_RequireEnergy;	// 46
	WORD m_RequireLevel;	// 48
	WORD m_RequireVitality;	// 4A
	WORD m_RequireLeaderShip;	// 4C
	WORD m_Leadership;	// 4E
	BYTE m_RequireClass[MAX_TYPE_PLAYER];	// 50
	BYTE m_Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE];	// 56
	int m_Value;	// 60
	DWORD m_SellMoney;	// 64
	DWORD m_BuyMoney;	// 68
	int m_iPShopValue;	// 6C
	bool m_bItemExist;	// 70
	int m_OldSellMoney;	// 74
	int m_OldBuyMoney;	// 78
	BYTE m_Option1;	// 7C  Skill
	BYTE m_Option2;	// 7D  Luck
	BYTE m_Option3;	// 7E  Option
	BYTE m_NewOption;	// 7F ExcellentOption
	float m_DurabilityState[4];	// 80
	float m_CurrentDurabilityState;	// 90
	bool m_SkillChange;	// 94
	bool m_QuestItem;	// 95
	BYTE m_SetOption;	// 96
	BYTE m_SetAddStat;	// 97
	bool m_IsValidItem;	// 98
	BYTE m_SkillResistance[MAX_ITEM_SPECIAL_ATTRIBUTE];	// 99
	BOOL m_IsLoadPetItemInfo;	// A0
	int  m_PetItem_Level;	// A4
	int  m_PetItem_Exp; // A8
	BYTE m_JewelOfHarmonyOption;	// AC
	WORD m_HJOpStrength;	// AE
	WORD m_HJOpDexterity;	// B0
	BYTE m_ItemOptionEx;	// B2
	//Season4 Add-on
//	BYTE m_SocketOptionIndex; //B3
//	BYTE m_SocketOption[5]; // B4
//	BYTE m_SetOptionList; //B9

	BYTE m_BonusSocketOption;
	BYTE m_SocketOption[5];
	BYTE m_ImproveDurabilityRate;
	BYTE m_PeriodItemOption; // bit0 : period item flag, bit1 : expired item

};

class CPetItemExp
{

public:

	int m_DarkSpiritExpTable[MAX_PET_LEVEL+2];
	int m_DarkHorseExpTable[MAX_PET_LEVEL+2];

public:

	CPetItemExp();
};


extern int g_MaxItemIndexOfEachItemType[MAX_TYPE_ITEMS];

void BufferItemtoConvert3(LPBYTE buf, int& type, BYTE& level, BYTE& op1, BYTE& op2, BYTE& op3, BYTE& dur);
void ItemByteConvert7(LPBYTE buf, CItem * const item , int maxitem);
void ItemByteConvert10(LPBYTE buf, CItem * const item , int maxitem);
void ItemByteConvert16(LPBYTE buf, CItem * const item , int maxitem);
void ItemByteConvert(LPBYTE buf, CItem item);
void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx, BYTE SocketOption[5], BYTE SocketIndex, BYTE PeriodItemOption);
int ItemGetNumberMake(int type, int index);
void ItemGetSize(int index, int & width, int & height);
BOOL HasItemDurability(int index);
int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem);
int ItemGetAttackDurability(int index);
int ItemGetDefenseDurability(int index);
float GetRepairItemRate(int index);
float GetAllRepairItemRate(int index);
void CalRepairRate( int itemtype, int itemsubtype, LPITEM_ATTRIBUTE p);
BOOL OpenItemScript(char* FileName);
BOOL OpenItemNameScript(char* FileName);
BOOL OpenItemScript(char* Buffer, int iSize);
BOOL OpenItemNameScript(char* Buffer, int iSize);
int zzzItemLevel(int type, int index, int level);
int GetLevelItem(int type, int index, int level);
int GetSerialItem(int type);
int IsItem(int item_num);
LPITEM_ATTRIBUTE GetItemAttr(int item_num);

#endif