#pragma once
// -------------------------------------------------------------------------------

#include "SkillDelay.h"
#include "ComboAttack.h"
#include "giocp.h"
#include "..\ggsvr\ggsvr.h"
#include "MagicInf.h"
#include "..\common\zzzitem.h"
#include "..\common\classdef.h"
#include "GuildClass.h"
#include "TDurMagicKeyChecker.h"
#include "TMonsterAIAgro.h"
#include "TMonsterSkillElementInfo.h"
#include "JewelOfHarmonySystem.h"
#include "ItemSystemFor380.h"
#include "BuffScriptLoader.h"
#include "PacketStatistics.h"	//1.01.03 up
#include "HacktoolBlockEx.h"	//1.01.03 up
#include "PCBangPointSystem.h"
#include "ItemSocketOptionSystem.h"
#include "GameShop.h"
#ifdef WZQUEST
#include "QuestExpUserInfo.h"
#endif
// -------------------------------------------------------------------------------

#define OBJMAX			12400//15600	// 9000
#define OBJMAXUSER		1000	// 1000
#define OBJ_MAXMONSTER	10500//14000	// 7400
// -------------------------------------------------------------------------------

#define CRYWOLF_MAP_RANGE(map)		( ( ((map)) == MAP_INDEX_CRYWOLF_FIRSTZONE  )?TRUE:FALSE  )
#define OBJ_STARTUSERINDEX			(OBJMAX - OBJMAXUSER)
#define OBJ_CALLMONCOUNT			(OBJ_STARTUSERINDEX - OBJ_MAXMONSTER)
#define MAX_MONSTER_SEND_MSG		10
#define MAX_MONSTER_SEND_ATTACK_MSG	50
#define MAX_MONSTER_TYPE			1024 // 1.01.00 GS
#define MAX_GUILD_LEN				8
#define MAX_ACCOUNT_LEN				10
#ifdef EXPINV
#define INVENTORY_SIZE				236	//-> 236
#define WAREHOUSE_SIZE				240	//-> 240
#define WAREHOUSE_BUFF				4096
#define MAIN_INVENTORY_SIZE			204	//-> 204
#define INVENTORY_MAP_SIZE			224	//-> 224
#else
#define INVENTORY_SIZE				108
#define WAREHOUSE_SIZE				120
#define WAREHOUSE_BUFF				2048
#define MAIN_INVENTORY_SIZE			76
#define INVENTORY_MAP_SIZE			96
#endif
#define TRADE_BOX_SIZE				32
#define MAGIC_SIZE					MAX_MAGIC
#define INVETORY_WEAR_SIZE			12
#define INVENTORY_BAG_START			INVETORY_WEAR_SIZE
#define TRADE_BOX_MAP_SIZE			32
#define PSHOP_SIZE					32
#define PSHOP_MAP_SIZE				32
#define MAX_CHAR_LEVEL				400
#define MAX_SELF_DEFENSE			7
#define MAX_ST_HIT_DAMAGE			40
#define MAX_ZEN						2000000000
#define MAX_TRADE_ZEN				1000000000
#define MAX_ZEN_DROP				8000000//25000000
#define MAX_WAREHOUSE_ZEN			MAX_ZEN
#define ACTIVE_ITEM_SEARCH			0
#define CHAOS_BOX_SIZE				32
#define CHAOS_BOX_MAP_SIZE			32
// -------------------------------------------------------------------------------

enum MAP_INDEX
{
	MAP_INDEX_RORENCIA					= 0,
	MAP_INDEX_DUNGEON					= 1,
	MAP_INDEX_DEVIAS					= 2,
	MAP_INDEX_NORIA						= 3,
	MAP_INDEX_LOSTTOWER					= 4,
	MAP_INDEX_RESERVED					= 5,
	MAP_INDEX_BATTLESOCCER				= 6,
	MAP_INDEX_ATHLANSE					= 7,
	MAP_INDEX_TARKAN					= 8,
	MAP_INDEX_DEVILSQUARE				= 9,
	MAP_INDEX_ICARUS					= 10,
	MAP_INDEX_BLOODCASTLE1				= 11,
	MAP_INDEX_BLOODCASTLE2				= 12,
	MAP_INDEX_BLOODCASTLE3				= 13,
	MAP_INDEX_BLOODCASTLE4				= 14,
	MAP_INDEX_BLOODCASTLE5				= 15,
	MAP_INDEX_BLOODCASTLE6				= 16,
	MAP_INDEX_BLOODCASTLE7				= 17,
	MAP_INDEX_CHAOSCASTLE1				= 18,
	MAP_INDEX_CHAOSCASTLE2				= 19,
	MAP_INDEX_CHAOSCASTLE3				= 20,
	MAP_INDEX_CHAOSCASTLE4				= 21,
	MAP_INDEX_CHAOSCASTLE5				= 22,
	MAP_INDEX_CHAOSCASTLE6				= 23,
	MAP_INDEX_KALIMA1					= 24,
	MAP_INDEX_KALIMA2					= 25,
	MAP_INDEX_KALIMA3					= 26,
	MAP_INDEX_KALIMA4					= 27,
	MAP_INDEX_KALIMA5					= 28,
	MAP_INDEX_KALIMA6					= 29,
	MAP_INDEX_CASTLESIEGE				= 30,
	MAP_INDEX_CASTLEHUNTZONE			= 31,
	MAP_INDEX_DEVILSQUARE2				= 32,
	MAP_INDEX_AIDA						= 33,
	MAP_INDEX_CRYWOLF_FIRSTZONE			= 34,
	MAP_INDEX_CRYWOLF_SECONDZONE		= 35,
	MAP_INDEX_KALIMA7					= 36,
	MAP_INDEX_KANTURU1					= 37,
	MAP_INDEX_KANTURU2					= 38,
	MAP_INDEX_KANTURU_BOSS				= 39,
	MAP_INDEX_GM_MAP					= 40,
	MAP_INDEX_3RD_CHANGEUP_QUEST		= 41,
	MAP_INDEX_3RD_CHANGEUP_BOSS_QUEST	= 42,
	MAP_INDEX_ILLUSION_TEMPLE1			= 45,
	MAP_INDEX_ILLUSION_TEMPLE2			= 46,
	MAP_INDEX_ILLUSION_TEMPLE3			= 47,
	MAP_INDEX_ILLUSION_TEMPLE4			= 48,
	MAP_INDEX_ILLUSION_TEMPLE5			= 49,
	MAP_INDEX_ILLUSION_TEMPLE6			= 50,
	MAP_INDEX_ELBELAND					= 51,
	MAP_INDEX_BLOODCASTLE8				= 52,
	MAP_INDEX_CHAOSCASTLE7				= 53,
	MAP_INDEX_SWAMP_OF_CALMNESS			= 56,
	MAP_INDEX_RAKLION_FIELD				= 57,
	MAP_INDEX_RAKLION_BOSS				= 58,
	MAP_INDEX_XMASEVENT					= 62,
	MAP_INDEX_VULCANUS					= 63,
	MAP_INDEX_PKFIELD					= 64,
};
// -------------------------------------------------------------------------------

enum EXP_GETTING_EVENT_TYPE
{
	EVENT_TYPE_NONE = 0x0,
	EVENT_TYPE_PARTY = 0x1,
	EVENT_TYPE_DEVILSQUARE = 0x2,
	EVENT_TYPE_CHAOSCASTLE = 0x3,
	EVENT_TYPE_BLOODCASTLE = 0x4,
	EVENT_TYPE_CRYWOLF = 0x5,
};
// -------------------------------------------------------------------------------

enum SKILL_LIST_TYPE 
{
  SKILL_LIST_TYPE_DEFALT = 0x0,
  SKILL_LIST_TYPE_ADD = 0x1,
  SKILL_LIST_TYPE_DEL = 0x2,
};
// -------------------------------------------------------------------------------

enum KILLCOUNT_OPTION 
{
  KILLCOUNT_RESET = 0x0,
  KILLCOUNT_INC = 0x1,
  KILLCOUNT_DEC = 0x2,
};
// -------------------------------------------------------------------------------

enum CHARACTER_AUTO_RECUPERATION_TIME 
{
  CART_10_SEC = 0x2710,
  CART_15_SEC = 0x3a98,
  CART_25_SEC = 0x61a8,
};
// -------------------------------------------------------------------------------

enum HP_MP_AG_VALUE_PER_TIME 
{
  VPT_HP_MP_AG_1_PER_1_SEC = 0x1,
  VPT_HP_MP_AG_5_PER_1_SEC = 0x5,
  VPT_HP_MP_AG_10_PER_1_SEC = 0xa,
};
// -------------------------------------------------------------------------------

enum FRUIT_ITEM_TYPE 
{
  FRUIT_ENERGY = 0x0,
  FRUIT_VITALITY = 0x1,
  FRUIT_DEXTERITY = 0x2,
  FRUIT_STRENGTH = 0x3,
  FRUIT_LEADERSHIP = 0x4,
};
// -------------------------------------------------------------------------------

enum STAT_FRUIT_USING_RESULT 
{
  STAT_PLUS_SUCCESS = 0x0,
  STAT_PLUS_FAILED = 0x1,
  STAT_PLUS_PREVENTED = 0x2,
  STAT_MINUS_SUCCESS = 0x3,
  STAT_MINUS_FAILED = 0x4,
  STAT_MINUS_PREVENTED = 0x5,
  STAT_EX_MINUS_SUCCESS = 0x6,
  STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT = 0x10,
  STAT_PLUS_PREVENTED_BY_MAX = 0x21,
  STAT_MINUS_PREVENTED_BY_MAX = 0x25,
  STAT_MINUS_PREVENTED_BY_DEFAULT = 0x26,
};
// -------------------------------------------------------------------------------

#define MAX_RESISTENCE_TYPE		7
#define R_ICE					0
#define R_POISON				1
#define R_LIGHTNING				2
#define R_FIRE					3
#define R_EARTH					4
#define R_WIND					5
#define R_WATER					6
// -------------------------------------------------------------------------------

#define OBJ_EMPTY				-1
#define OBJ_MONSTER				2
#define OBJ_USER				1
#define OBJ_NPC					3
// -------------------------------------------------------------------------------

#define PLAYER_EMPTY			0
#define PLAYER_CONNECTED		1
#define PLAYER_LOGGED			2
#define PLAYER_PLAYING			3
#define PLAYER_TEMPORARY		4	//new value
// -------------------------------------------------------------------------------

#ifdef SEASON6DOT3_ENG
#define MAX_MAGIC				150
#else
#define MAX_MAGIC				60
#endif
#define MAX_VIEWPORT			75
#define MAX_VIEWPORT_MONSTER	20
#define MAX_ROAD_PATH_TABLE		16
#define MAX_CHECKSUM_KEY		1024
#define OBJMAX_RANGE(aIndex)	( ((aIndex) < 0 )?FALSE:( (aIndex) > OBJMAX-1 )?FALSE:TRUE   )
#define FRIEND_SERVER_STATE_LOGIN_FAIL	0
#define FRIEND_SERVER_STATE_OFFLINE		1
#define FRIEND_SERVER_STATE_ONLINE		2
#define MAIN_INVENTORY_RANGE(x) (((x)<0)?FALSE:((x)>MAIN_INVENTORY_SIZE-1)?FALSE:TRUE )
#define INVENTORY_RANGE(x)		(((x)<0)?FALSE:((x)>INVENTORY_SIZE-1)?FALSE:TRUE )
#define WAREHOUSE_RANGE(x)		(((x)<0)?FALSE:((x)>WAREHOUSE_SIZE-1)?FALSE:TRUE )
#define TRADE_BOX_RANGE(x)		(((x)<0)?FALSE:((x)>TRADE_BOX_SIZE-1)?FALSE:TRUE )
#define OBJMON_RANGE(x)			( ((x)<0)?FALSE:((x)>OBJ_MAXMONSTER-1)?FALSE:TRUE)
#define PSHOP_RANGE(x)			( (((x)-MAIN_INVENTORY_SIZE)<0)?FALSE:(((x)-MAIN_INVENTORY_SIZE)>(INVENTORY_SIZE-MAIN_INVENTORY_SIZE-1))?FALSE:TRUE )
#define ATTRIBUTE_RANGE(x)		(((x)<51)?FALSE:((x)>58)?FALSE:TRUE )
// -------------------------------------------------------------------------------

#define CURRENT_DB_VERSION		3
#define CHAR_SET_SIZE			18
#define CS_CLASS				0
#define CS_WEAPON1_TYPE			1
#define CS_WEAPON2_TYPE			2
#define CS_WEAPON1_DATA			12
#define CS_WEAPON2_DATA			13
#define CS_HELMET1				13
#define CS_HELMET2				9
#define CS_HELMET3				3
#define CS_ARMOR1				14
#define CS_ARMOR2				9
#define CS_ARMOR3				3
#define CS_PANTS1				14
#define CS_PANTS2				9
#define CS_PANTS3				4
#define CS_GLOVES1				15
#define CS_GLOVES2				9
#define CS_GLOVES3				4
#define CS_BOOTS1				15
#define CS_BOOTS2				9
#define CS_BOOTS3				5
// -------------------------------------------------------------------------------

#define CHECK_LIMIT(value, limit) ( ((value)<0)?FALSE:((value)>((limit)-1))?FALSE:TRUE  )
#define CHECK_CLASS(value, type) ( ((value)!=(type))?FALSE:TRUE )
#ifdef SEASON6DOT3_ENG
#define CS_GET_CLASS(x) ( (((x)>>4)<<5)&(0xE0) )
#else
#define CS_GET_CLASS(x) ( (x) <<5 ) & 0xE0
#endif
#define CS_GET_MASTERCLASS(x) ( (((x)>>1)<<3)&(0x08) )
#define CS_GET_CHANGEUP(x) ( ((x)&0x07) )
#define CS_SET_CLASS(x) ( ((x)<<5)& 0xE0 )
#define CS_SET_CHANGEUP(x) ( ((x) << 4) & 0x10 )
#define CS_SET_3RD_CHANGEUP(x) ( ((x) << 3) & 8 )
#define CS_SET_HELMET1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_HELMET2(x) ( ((x) & 0x10 ) << 3 )
#define CS_SET_HELMET3(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_ARMOR1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_ARMOR2(x) ( ((x) & 0x10 ) << 2 )
#define CS_SET_ARMOR3(x) ( ((x) & 0x0F )      )
#define CS_SET_PANTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_PANTS2(x) ( ((x) & 0x10 ) << 1 )
#define CS_SET_PANTS3(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_GLOVES1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_GLOVES2(x) ( ((x) & 0x10 )      )
#define CS_SET_GLOVES3(x) ( ((x) & 0x0F )      )
#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_WING1(x)  ( ((x) & 0x03 ) << 2 )
#define CS_SET_HELPER(x) ( ((x) & 0x03 )      )
#define CS_SET_SMALLLEVEL_RH(x)		( (x)       )
#define CS_SET_SMALLLEVEL_LH(x)		( (x) << 3  ) 
#define CS_SET_SMALLLEVEL_HELMET(x)	( (x) << 6  )
#define CS_SET_SMALLLEVEL_ARMOR(x)	( (x) << 9  )
#define CS_SET_SMALLLEVEL_PANTS(x)	( (x) << 12 )
#define CS_SET_SMALLLEVEL_GLOVES(x)	( (x) << 15 )
#define CS_SET_SMALLLEVEL_BOOTS(x)	( (x) << 18 )
#define CS_SET_SMALLLEVEL1(x)		( ((x) >> 16) & 0xFF )
#define CS_SET_SMALLLEVEL2(x)		( ((x) >> 8 ) & 0xFF )
#define CS_SET_SMALLLEVEL3(x)		((x) & 0xFF )
// -------------------------------------------------------------------------------

typedef union tagPATHTABLE
{
	short sPathTable[16];
	char cPathTable[32];
} PATHTABLE, * LPPATHTABLE;
// -------------------------------------------------------------------------------

typedef struct tagActionState
{
	DWORD Rest:1;	// 0
	DWORD Attack:1;	// 1
	DWORD Move:1;	// 2
	DWORD Escape:1;	// 3
	DWORD Emotion:4;	// 4
	DWORD EmotionCount:8;	// 8
} ACTION_STATE, * LPACTION_STATE;
// -------------------------------------------------------------------------------

typedef struct tagInterfaceState
{
	DWORD use	: 2;
	DWORD state : 4;
	DWORD type	: 10;

} INTERFACE_STATE, * LPINTERFACE_STATE;
// -------------------------------------------------------------------------------

extern short	RoadPathTable[MAX_ROAD_PATH_TABLE];
extern int		gServerMaxUser;	
extern BOOL		g_EnergyCheckOff;
// -------------------------------------------------------------------------------

struct HITDAMAGE_STRUCT
{
	short number;	// 0
	int HitDamage;	// 4
	DWORD LastHitTime;	// 8
};
// -------------------------------------------------------------------------------

struct VIEWPORT_STRUCT
{
	char state;	// 0
	short number;	// 2
	BYTE type;	// 4
	short index;	// 6
	int dis;	// 8
};
// -------------------------------------------------------------------------------

struct VIEWPORT_PLAYER_STRUCT
{
	char state;	// 0
	short number;	// 2
	BYTE type;	// 4
	short index;	// 6
	int dis;	// 8

};
// -------------------------------------------------------------------------------

struct MessageStateMachine
{
	int MsgCode;	// 0
	int SendUser;	// 4
	DWORD MsgTime;	// 8
	int SubCode;	// C
};
// -------------------------------------------------------------------------------

struct ExMessageStateMachine
{
	int MsgCode;	// 0
	int SendUser;	// 4
	DWORD MsgTime;	// 8
	int SubCode;	// C
	int SubCode2;	// 10
};
// -------------------------------------------------------------------------------

typedef struct tagSEND_MONSTER_ATTACK_MENSSAGE
{
	int iMsgCode;	// unk0
	int iIndex;	// unk4
	DWORD dwDelay;	// unk8
	int iSubCode;	// unkC
	int iSubCode2;	// unk10
}SEND_MONSTER_ATTACK_MENSSAGE;
// -------------------------------------------------------------------------------

extern ExMessageStateMachine	gSMAttackProcMsg[OBJMAX][MAX_MONSTER_SEND_ATTACK_MSG];
extern MessageStateMachine		gSMMsg[OBJMAX][MAX_MONSTER_SEND_MSG];
// -------------------------------------------------------------------------------

struct MONSTERKILLINFO
{
	int MonIndex;
	int KillCount;
};
// -------------------------------------------------------------------------------

struct BUFF_MANAGER
{
	BYTE btBuffIndex; //210
	BYTE btVpIndex; //211
	BYTE btEffectType1; //212
	BYTE btEffectType2; //213
	int iValue1; //214
	int iValue2; //218
	DWORD dwBuffTickCount; //21C
	int iDuration; //220
};
// -------------------------------------------------------------------------------

struct PMSG_USE_STAT_FRUIT
{
	struct PBMSG_HEAD h;
	BYTE result;
	WORD btStatValue;
	BYTE btFruitType;
};
// -------------------------------------------------------------------------------

typedef struct _MASTERLEVEL_PASSIVE_SKILL_OPTION 
{
	float	MpsAttackSuccessRate;		//+1248
	float	MpsBlockingRate;			//+1252
	float	MpsPVPAttackDmgRate;		//+1256
	float	MpsPVPBlockingRate;			//+1260
	float	MpsDownDur1;				//+1264
	BYTE	btMpsDownDur1Level;			//+1268??? maybe [4]?
	float	MpsDownDur2;				//+1272
	float	MpsDownDur3;				//+1276
	float	MpsResistancePoison;		//+1280
	float	MpsResistanceThunder;		//+1284
	float	MpsResistanceIce;			//+1288
	float	MpsAutoRecoverLife;			//+1292
	float	MpsAutoRecoverMana;			//+1296
	float	MpsAutoRecoverAG;			//+1300
	float	MpsWeaponMastery;			//+1304 (dk, mg, dl)
	//float	ukn_3C;				// 3C
	float	MpsTwoHandSwordStrength; // 40
	float	MpsOneHandSwordStrength; // 44
	float	MpsMaceStrength;		// 48
	float	MpsSpearStrength;		// 4C
	float	MpsTwoHandSwordMastery;// 50
	float	MpsOneHandSwordMastery;// 54
	//float	ukn_58;				// 58
	float	MpsSpearMastery;		// 5C
	float	MpsMaceMastery;		// 60
	//float	ukn_44;				// 64
	float MpsMagicMastery;		// 68 dw, mg
	float MpsTwoHandStaffStrength; // 6C
	float MpsOneHandStaffStrength; // 70
	float MpsShieldStrength;	// 74
	float MpsOneHandStaffMastery;// 78
	float MpsTwoHandStaffMastery;// 7C
	float MpsShieldMastery;		// 80
	float MpsBowStrength;		// 84
	float MpsBowMastery;		// 88
	float MpsCrossbowStrength;	// 8C
	float MpsCrossbowMastery;	// 90
	//float ukn_94;				// 94
	//float ukn_98;				// 98
	float MpsWeaponMastery2;	// 9C Elf Weapon Mastery
	float MpsSummonMonster1;	//+1408
	float MpsSummonMonster2;	//+1412
	float MpsSummonMonster3;	//+1414
	float MpsMagicMastery2;		// AC summoner 
	float MpsFireTomeStrength;	// B0
	float MpsWindTomeStrength;	// B4
	float MpsLightTomeStrength;	// B8
	float MpsFireTomeMastery;	// BC
	float MpsWindTomeMastery;	// C0
	float MpsLightTomeMastery;	// C4
	float MpsStickStrength;		// C8
	float MpsTomeStrength;		// CC
	float MpsStickMastery;		// D0
	float MpsTomeMastery;		// D4
	float ukn_D8;				// D8
	float ukn_DC;				// DC
	//float ukn_E0;				// E0
	float MpsMinWizCurseInc;	// E4
	//float ukn_E8;				// E8
	//float ukn_EC;				// EC
	float MpsDarkSpiritStr1;	// F0
	float MpsDarkSpiritStr2;	// F4
	float MpsDarkSpiritStr3;	// F8
	float MpsScepterStrength;	// FC
	float MpsScepterMastery;	// 100
	float MpsShieldStrength3;	// 104 dl
	float MpsShieldMastery3;	// 108 dl
	float MpsUseScepterPetStr;	// 10C dl
	float MpsCommandAttackInc;	// 110
	float MpsMinAttackInc;		// 114
	float MpsMaxAttackInc;		// 118
	float ukn_11C;				// 11C
	//float ukn_120;				// 120
	float MpsDefence;			//+1540
	float MpsMaxHP;				//+1544
	float MpsMaxBP;				//+1548
	float MpsMaxMP;				//+1552
	float MpsMonsterDieGetMana;	//+1556
	float MpsMonsterDieGetLife;	//+1560
	float MpsMonsterDieGetSD;	//+1564
	//float ukn_140;				// 140
	float MpsMaxSD;				//+1572
	float MpsSDSpeed;			//+1576
	float MpsMaxAttackDamage;	// 14C
	float MpsMinAttackDamage;	// 150
	float MpsDecreaseMana;		//+1588
	float MpsMaxMagicDamage;	//+1592
	float MpsMinMagicDamage;	//+1596
	float MpsPetDurDownSpeed;	//+1600
	//float ukn_164;				// 164
	//float ukn_168;				// 168
	//int iMpsMaxAttackMagicDamage;
	//int iMpsMinAttackMagicDamage;
	float MpsPlusExp;
	float MpsExpWizardryCrit;	// temp
	float MpsManaShieldAddMana;	// temp
	float MpsBerserkInc1;		// temp
	float MpsBerserkInc2;		// temp
	float MpsBerserkInc3;		// temp
	float MpsCritBuffExcDamage;	// temp
	float MpsShieldMastery2;	// temp
	float MpsShieldStrength2;	//temp
	float MpsFistStrength;		//tem
	float MpsFistMastery;
	/*float MpsWingAttackIncrease;
	float MpsWingDefenseIncrease;
	float MpsDarkHorseStrength;
	float MpsArmorSetBonusIncrease;
	float MpsReflectDamage;
	float MpsEnergyIncrease;
	float MpsVitalityIncrease;
	float MpsDexterityIncrease;
	float MpsStrengthIncrease;
	float MpsLeadershipIncrease;
	float MpsCriticalRateIncrease;
	float MpsRestoreFullMP;
	float MpsRestoreFullHP;
	float MpsExcelentRateIncrease;
	float MpsRestoreFullSD;
	float MpsDoubleDamageRate;
	float MpsIgnoreDef;
	float MpsDarkSpiritStr4;
	float MpsDarkSpiritStr5;*/
} MASTERLEVEL_PASSIVE_SKILL_OPTION;
// -------------------------------------------------------------------------------

#ifdef __ROOT__
struct QuestUser
{
	short	SceneIndex;
	short	EpisodeIndex;
	BYTE	HuntData[5];
	BYTE	TalkIndex;
	bool	Reward;
};
#endif

//size 2238 for GS-CS
struct OBJECTSTRUCT
{
	int		m_Index;
	int		Connected;
	char	LoginMsgSnd;
	char	LoginMsgCount;
	char	CloseCount;
	char	CloseType;
	BOOL	bEnableDelCharacter;
	struct	_PER_SOCKET_CONTEXT* PerSocketContext;
	SOCKET	m_socket;
	char	Ip_addr[16];
	int		UserNumber;
	int		DBNumber;
	DWORD	ukn_30;
	BYTE	Magumsa;
	DWORD	AutoSaveTime;
	DWORD	ConnectCheckTime;
	DWORD	CheckTick;
	BYTE	CheckSpeedHack;
	DWORD	CheckTick2;
	BYTE	CheckTickCount;
	DWORD	SaveTimeForStatics;
	int		iPingTime;
	BYTE	m_TimeCount;
	DWORD	m_dwPKTimer;
	short	CheckSumTableNum;
	DWORD	CheckSumTime;
	WORD	Type;
	BYTE	Live;
	char	CharacterPos;
	char	AccountID[11];
	char	Name[11];
	char	SubName[30];
	char	LastJoominNumber[14];
	bool	PlusStatQuestClear;
	bool	ComboSkillquestClear;
	struct	ComboSkillData comboSkill;
	WORD	Class;
	BYTE	DbClass;
	BYTE	ChangeUP;
	BYTE	ThirdChangeUp;
	short	Level;
	int		LevelUpPoint;
	int		iFruitPoint;
	DWORD	Experience;
	DWORD	NextExp;
	short	m_nMasterLevel;
	__int64 m_i64MasterLevelExp;
	__int64 m_i64NextMasterLevelExp;
	int		m_iMasterLevelPoint;
	int		m_bMasterLevelDBLoad;
	int		Money;
	__int64	MonsterMoney;
#if( FIX_MAXSTAT == 1 )
	WORD  Strength;								// DC
	WORD  Dexterity;							// DE
	WORD  Vitality;								// E0
	WORD  Energy;								// E2
#else
	short Strength;								// DC
	short Dexterity;							// DE
	short Vitality;								// E0
	short Energy;								// E2
#endif
	float	Life;
	float	MaxLife;
	int		m_iScriptMaxLife;
	float	FillLife;
	float	FillLifeMax;
	float	Mana;
	float	MaxMana;
	WORD	Leadership;
	WORD	AddLeadership;
	WORD	ChatLitmitTime;
	BYTE	ChatLimitTimeSec;
	BYTE	FillLifeCount;
#if( FIX_MAXSTAT == 1 )
	WORD  AddStrength;							// 108
	WORD  AddDexterity;							// 10A
	WORD  AddVitality;							// 10C
	WORD  AddEnergy;							// 10E
#else
	short AddStrength;							// 108
	short AddDexterity;							// 10A
	short AddVitality;							// 10C
	short AddEnergy;							// 10E
#endif
	int		BP;
	int		MaxBP;
	int		AddBP;
	float	VitalityToLife;
	float	EnergyToMana;
	char	m_PK_Count;
	char	m_PK_Level;
	int		m_PK_Time;
	short	X;
	short	Y;
	BYTE	Dir;
	BYTE	MapNumber;
	int		AddLife;
	int		AddMana;
	int		iShield;
	int		iMaxShield;
	int		iAddShield;
	int		iFillShieldMax;
	int		iFillShield;
	int		iFillShieldCount;
	DWORD	dwShieldAutoRefillTimer;
	BYTE	DamageMinus;
	BYTE	DamageReflect;
	short	MonsterDieGetMoney;
	BYTE	MonsterDieGetLife;
	BYTE	MonsterDieGetMana;
	BYTE	StartX;
	BYTE	StartY;
	short	m_OldX;
	short	m_OldY;
	short	TX;
	short	TY;
	short	MTX;
	short	MTY;
	int		PathCount;
	int		PathCur;
	char	PathStartEnd;
	short	PathOri[15];
	short	PathX[15];
	short	PathY[15];
	char	PathDir[15];
	DWORD	PathTime;
	BYTE	m_MoveGateNumber;
	short	m_wRaiseTalismanSaveX;
	short	m_wRaiseTalismanSaveY;
	short	m_wRaiseTalismanSaveMap;
	short	m_wTeleportTalismanSaveX;
	short	m_wTeleportTalismanSaveY;
	short	m_wTeleportTalismanSaveMap;
	DWORD	Authority;
	DWORD	Penalty;
	BYTE	m_cAccountItemBlock;
	struct	tagActionState m_ActState;
	BYTE	m_ActionNumber;
	DWORD	m_State;
	char	m_StateSub;
	BYTE	m_Rest;
	char	m_ViewState;
	BYTE	m_BuffEffectCount;
	struct	BUFF_MANAGER m_BuffEffectList[MAX_STATE_COUNT];
	DWORD	m_LastMoveTime;
	DWORD	m_LastAttackTime;
	BYTE	m_FriendServerOnline;
	int		m_DetectSpeedHackTime;
	DWORD	m_SumLastAttackTime;
	DWORD	m_DetectCount;
	int		m_DetectedHackKickCount;
	int		m_SpeedHackPenalty;
	BYTE	m_AttackSpeedHackDetectedCount;
	DWORD	m_PacketCheckTime;
	BYTE	m_ShopTime;
	DWORD	m_TotalAttackTime;
	int		m_TotalAttackCount;
	DWORD	TeleportTime;
	char	Teleport;
	char	KillerType;
	char	DieRegen;
	char	RegenOk;
	BYTE	RegenMapNumber;
	BYTE	RegenMapX;
	BYTE	RegenMapY;
	DWORD	RegenTime;
	DWORD	MaxRegenTime;
	short	m_PosNum;
	DWORD	LifeRefillTimer;
	DWORD	CurActionTime;
	DWORD	NextActionTime;
	DWORD	DelayActionTime;
	char	DelayLevel;
	char	m_PoisonType;
	char	m_IceType;
#ifdef OPT
	char	m_iMonsterStunDelay;
	char	m_iMonsterSleepDelay;
#endif
	char	m_iMonsterBattleDelay;
	char	m_cKalimaGateExist;
	int		m_iKalimaGateIndex;
	char	m_cKalimaGateEnterCount;
	struct	OBJECTSTRUCT * lpAttackObj;
	short	m_SkillNumber;
	DWORD	m_SkillTime;
	bool	m_bAttackerKilled;
	char	m_ManaFillCount;
	char	m_LifeFillCount;
	int		SelfDefense[7];
	DWORD	SelfDefenseTime[7];
	DWORD	MySelfDefenseTime;
	char	m_Drink;
	int		m_SkillAttack;
	char	m_SkillAttackTime;
	int		PartyNumber;
	int		PartyTargetUser;
	int		GuildNumber;
	struct	_GUILD_INFO_STRUCT* lpGuild;
	char	GuildName[11];
	int		GuildStatus;
	int		iGuildUnionTimeStamp;
	int		m_RecallMon;
	int		m_Change;
	short	TargetNumber;
	short	TargetShopNumber;
	short	ShopNumber;
	short	LastAttackerID;
	int		m_AttackDamageMin;
	int		m_AttackDamageMax;
	int		m_MagicDamageMin;
	int		m_MagicDamageMax;
	int		m_AttackDamageLeft;
	int		m_AttackDamageRight;
	int		m_AttackDamageMaxLeft;
	int		m_AttackDamageMinLeft;
	int		m_AttackDamageMaxRight;
	int		m_AttackDamageMinRight;
	int		m_AttackRating;
	int		m_AttackSpeed;
	int		m_MagicSpeed;
	int		m_Defense;
	int		m_MagicDefense;
	int		m_SuccessfulBlocking;
	int		m_Curse;
	int		m_CurseDamgeMin;
	int		m_CurseDamgeMax;
	short	m_MoveSpeed;
	short	m_MoveRange;
	short	m_AttackRange;
	short	m_AttackType;
	short	m_ViewRange;
	short	m_Attribute;
	short	m_ItemRate;
	short	m_MoneyRate;
	int		m_CriticalDamage;
	int		m_ExcelentDamage;
	class	CMagicInf* m_lpMagicBack;
	class	CMagicInf* Magic;
	char	MagicCount;
	BYTE	UseMagicNumber;
	DWORD	UseMagicTime;
	char	UseMagicCount;
	short	OSAttackSerial;
#ifdef OPT
	BYTE	SASCount;
#endif
	DWORD	SkillAttackTime;
	MASTERLEVEL_PASSIVE_SKILL_OPTION m_MPSkillOpt;	// 1.01.00 
	int		m_nSoulBarrierDefence;
	short	m_nSoulBarrierManaRate;
	BYTE	CharSet[18];
	char	m_Resistance[MAX_RESISTENCE_TYPE];
	char	m_AddResistance[MAX_RESISTENCE_TYPE];
	int		FrustrumX[4];
	int		FrustrumY[4];
	struct	VIEWPORT_STRUCT VpPlayer[MAX_VIEWPORT];
	struct	VIEWPORT_PLAYER_STRUCT VpPlayer2[MAX_VIEWPORT];
	int		VPCount;
	int		VPCount2;
	struct	HITDAMAGE_STRUCT sHD[MAX_ST_HIT_DAMAGE];
	short	sHDCount;
	struct	tagInterfaceState m_IfState;
	DWORD	m_InterfaceTime;
	class	CItem* pInventory;
	LPBYTE	pInventoryMap;
	char*	pInventoryCount;
	char	pTransaction;
	class	CItem* Inventory1;
	LPBYTE	InventoryMap1;
	char	InventoryCount1;
	class	CItem* Inventory2;
	LPBYTE	InventoryMap2;
	char	InventoryCount2;
	class	CItem* Trade;
	LPBYTE	TradeMap;
	int		TradeMoney;
	bool	TradeOk;
	class	CItem* pWarehouse;
	LPBYTE	pWarehouseMap;
	char	WarehouseCount;
	short	WarehousePW;
	BYTE	WarehouseLock;
	BYTE	WarehouseUnfailLock;
	int		WarehouseMoney;
	int		WarehouseSave;
	class	CItem* pChaosBox;
	LPBYTE	pChaosBoxMap;
	int		ChaosMoney;
	int		ChaosSuccessRate;
	BOOL	ChaosLock;
	DWORD	m_Option;
	int		m_nEventScore;
	int		m_nEventExp;
	int		m_nEventMoney;
	BYTE	m_bDevilSquareIndex;
	bool	m_bDevilSquareAuth;
	char	m_cBloodCastleIndex;
	char	m_cBloodCastleSubIndex;
	int		m_iBloodCastleEXP;
	bool	m_bBloodCastleComplete;
	char	m_cChaosCastleIndex;
	char	m_cChaosCastleSubIndex;
	int		m_iChaosCastleBlowTime;
	char	m_cKillUserCount;
	char	m_cKillMonsterCount;
	int		m_iDuelUserReserved;
	int		m_iDuelUserRequested;
	int		m_iDuelUser;
	BYTE	m_btDuelScore;
	int		m_iDuelTickCount;
	bool	m_bPShopOpen;
	bool	m_bPShopTransaction;
	bool	m_bPShopItemChange;
	bool	m_bPShopRedrawAbs;
	char	m_szPShopText[36];
	bool	m_bPShopWantDeal;
	int		m_iPShopDealerIndex;
	char	m_szPShopDealerName[10];
	struct	_RTL_CRITICAL_SECTION m_critPShopTrade;
	int		m_iVpPShopPlayer[MAX_VIEWPORT];
	WORD	m_wVpPShopPlayerCount;
	bool	IsInBattleGround;
	bool	HaveWeaponInHand;
	short	EventChipCount;
	int		MutoNumber;
	BOOL	UseEventServer;
	bool	LoadWareHouseInfo;
	int		iStoneCount;
	int		m_i3rdQuestState;
	int		m_i3rdQuestIndex;
	MONSTERKILLINFO MonsterKillInfo[5];
	BYTE	m_Quest[50];
	bool	m_SendQuestInfo;
	int		m_SkyBossMonSheildLinkIndex;	//-> Need check, maybe unused
	int		m_SkyBossMonSheild;
	int		m_SkyBossMonSheildTime;
	int		m_MaxLifePower;
#ifdef OPT
	int		m_PacketChecksumTime;
#endif
	int		m_CheckLifeTime;
	BYTE	m_MoveOtherServer;
	char	BackName[11];
	char	m_BossGoldDerconMapNumber;
	bool	m_InWebzen;
	char	m_LastTeleportTime;
	BYTE	m_ClientHackLogCount;
	bool	m_bIsInMonsterHerd;
	BOOL	m_bIsMonsterAttackFirst;
	class	MonsterHerd* m_lpMonsterHerd;
	int		fSkillFrustrumX[4];
	int		fSkillFrustrumY[4];
	BYTE	SkillHellFire2State;
	BYTE	SkillHellFire2Count;
	DWORD	SkillHellFire2Time;
	BYTE	m_ReqWarehouseOpen;
#ifdef OPT
	int		m_NotAttackAreaCount;
#endif
	short	SetOpAddSkillAttack;
	short	SetOpAddExDamage;
	short	SetOpAddExDamageSuccessRate;
	short	SetOpAddCriticalDamage;
	short	SetOpAddCriticalDamageSuccessRate;
	short	SetOpIncAGValue;
	short	SetOpAddDamage;
	short	SetOpAddMinAttackDamage;
	short	SetOpAddMaxAttackDamage;
	short	SetOpAddAttackDamage;
	short	SetOpAddDefence;
	short	SetOpAddDefenceRate;
	short	SetOpAddMagicPower;
	BYTE	SetOpIgnoreDefense;
	BYTE	SetOpDoubleDamage;
	BYTE	SetOpTwoHandSwordImproveDamage;
	BYTE	SetOpImproveSuccessAttackRate;
	BYTE	SetOpReflectionDamage;
	BYTE	SetOpImproveSheldDefence;
	BYTE	SetOpDecreaseAG;
	BYTE	SetOpImproveItemDropRate;
	bool	IsFullSetItem;
	class	TDurMagicKeyChecker DurMagicKeyChecker;
	WORD	SkillRecallParty_Time;
	BYTE	SkillRecallParty_MapNumber;
	BYTE	SkillRecallParty_X;
	BYTE	SkillRecallParty_Y;
	bool	bIsChaosMixCompleted;
	bool	SkillLongSpearChange;
	class	CSkillDelay SkillDelay;
	int		iObjectSecTimer;
	bool	m_bMapSvrMoveQuit;
	bool	m_bMapSvrMoveReq;
	DWORD	m_dwMapSvrQuitTick;
	short	m_sPrevMapSvrCode;
	short	m_sDestMapNumber;
	BYTE	m_btDestX;
	BYTE	m_btDestY;
	// ----
	union
	{
		struct
		{
				BYTE	m_btCsNpcExistVal1;
				BYTE	m_btCsNpcExistVal2;
				BYTE	m_btCsNpcExistVal3;
				BYTE	m_btCsNpcExistVal4;
		};
		int		m_iCsNpcExistVal;
	};
	// ----
	BYTE	m_btCsNpcType;
	BYTE	m_btCsGateOpen;
	int		m_iCsGateLeverLinkIndex;
	BYTE	m_btCsNpcDfLevel;
	BYTE	m_btCsNpcRgLevel;
	BYTE	m_btCsJoinSide;
	bool	m_bCsGuildInvolved;
	bool	m_bIsCastleNPCUpgradeCompleted;
	BYTE	m_btWeaponState;
	int		m_iWeaponUser;
	BYTE	m_btKillCount;
	int		m_iAccumulatedDamage;
	BYTE	m_btLifeStoneCount;
	BYTE	m_btCreationState;
	int		m_iCreatedActivationTime;
	int		m_iAccumulatedCrownAccessTime;
	class	TMonsterSkillElementInfo m_MonsterSkillElementInfo;
	int		m_iBasicAI;
	int		m_iCurrentAI;
	int		m_iCurrentAIState;
	int		m_iLastAIRunTime;
	int		m_iGroupNumber;
	int		m_iSubGroupNumber;
	int		m_iGroupMemberGuid;
	int		m_iRegenType;
	class	TMonsterAIAgro m_Agro;
	int		m_iLastAutomataRuntime;
	int		m_iLastAutomataDelay;
	int		m_iCrywolfMVPScore;
#ifdef OPT
	int		m_iPcBangRoom;	//2030 (new) confirmed thru illusion
#endif
	DWORD	m_dwLastCheckTick;
	int		m_iAutoRecuperationTime;
	int		m_iSkillDistanceErrorCount;
	DWORD	m_dwSkillDistanceErrorTick;
	struct	JEWELOFHARMONY_ITEM_EFFECT m_JewelOfHarmonyEffect;
	struct	ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;
	BOOL	m_bKanturuEntranceByNPC;
	WORD	m_wItemEffectType;
	int		m_iItemEffectValidTime;
#ifdef OLDCASHSHOP
	WORD	m_wCashPoint; //0x209C
#endif
#ifdef PCBANG
	int		m_iPcBangConnectionType; // 0x20A0
	PCBANG_POINT_SYSTEM	m_PcBangPointSystem; // 0x20A4
	CPCBangPointTimer	m_PCBangPointTimer; //0x20C4
#endif
	int		m_iIllusionTempleIndex;
	bool	m_bSkillKeyRecv;
	int		m_iPeriodItemEffectIndex;
	BYTE	Summoner;
	BYTE	RageFighter;
	SOCKET_OPTION_LIST	m_SeedOptionList[35];
	SOCKET_OPTION_LIST	m_BonusOptionList[7];
	SOCKET_OPTION_LIST	m_SetOptionList[2];
	WORD	m_btRefillHPSocketOption;
	WORD	m_btRefillMPSocketOption;
	WORD	m_wSocketOptionMonsterDieGetHp;
	WORD	m_wSocketOptionMonsterDieGetMana;
	BYTE	m_btAGReduceRate;
	int		m_PeriodDataIndex;
	PacketStatistics m_kRecvPacketStatistics;
	BYTE	m_bGMSummon;
	DWORD	m_dwHacktoolCheckTerm; 
	HACKTOOL_USE_INFO m_HacktoolUseInfo[HACKTOOL_DATA_SIZE];
#ifdef OPT
	DWORD	m_dwUseHacktoolList[10];
#endif
	BYTE	m_btSculptPos;
	BYTE	m_btCharmPos;
	BYTE	m_btArtifactPos;
	bool	m_bUseGambleState;
	DWORD	m_dwPostTickCount;
	bool	IsOffTrade;
	int		Reset;
	int		GrandReset;
	int		m_InfinityArrowDmgMul;
	bool	m_MUHelperOn;
	bool	m_MUHelperReadyPay;
	BYTE	m_MUHelperStage;
	DWORD	m_MUHelperTick;
	// ----
#ifdef GAMESHOP
	GAMESHOP_USER_DATA GameShop;
#endif
	short	m_ImperialZoneID;
#ifdef GENS
	int		m_NextContributePoint; //0x234A
	int		m_GensInfluence; //0x23B4 (9140)
	char	m_szCharName[11]; // 0x23B8 (9144)
	char	m_szTargetCharName[11]; //0x23C2 (9154)
	int		m_KillUserCnt; // 0x2428 (9256)
	int		m_KillUserTotCnt; // 0x2450 (9296)
	int		m_UserBattleZoneEnable; //0x2454 (9300)
	int		m_ContributePoint; //0x2458 (9304)
	int		m_GensRanking; //0x2460 (9312)
	int		m_GensClass; //0x2464 (9316)
#ifdef OPT
	int		m_GensAbusingTick; //0x2468 (9320)
#endif
#endif
	// ----
#ifdef SEASON6DOT3_ENG
	BYTE	ExpandedInventory;
	BYTE	ExpandedWarehouse;
	char	m_btDarkSideTargetNum;
	short	m_wDarkSideTargetList[5];
	bool	bTemporaryUser;
#endif
	// ----
#ifdef __NOVUS__
	WORD	m_CraftLevel;
	WORD	m_CraftStage;
	WORD	m_CraftQuest;
#endif
	// ----
#ifdef __CUSTOMS__
	int		m_AddMixMoney;
	int		m_AddMixSuccessRate;
	bool	m_ConnectEx;
	DWORD	m_ShopPointExTime;
	char	m_Password[11];
	bool	m_PremiumState;
	char	m_PremiumEndDays[13];
#endif
#ifdef __ROOT__
	QuestUser m_QuestData[40];
	int		m_QuestCount;
	BYTE	m_QuestSceneStatus;
	BYTE	m_QuestPrevEpisode;
#endif
#ifdef OFFEXP
#ifdef __ALIEN__
	int DbOffExp;
#endif
	bool OffExp;
	int OffExpTick;
	int OffExpSpeed;
#endif
#ifdef __ALIEN__
	DWORD	m_PotionTick;
#endif
#ifdef WZQUEST
	UserQuestInfo* m_pUserQuestInfo;
	int m_bUserQuestInfoSent;
#endif
}; typedef OBJECTSTRUCT * LPOBJ;
// -------------------------------------------------------------------------------

//extern OBJECTSTRUCT gObj[OBJMAX];
extern int gObjCSFlag;
extern int gItemLoop;
extern int gItemLoop2;
extern int gItemLoopMax;
extern int gObjTotalUser;
extern int gDisconnect;
extern int gCurConnectUser; 
extern int skillSuccess;
extern int GuildInfoOfs;
extern int GuildInfoCount;
extern int GuilUserOfs;
extern int GuildUserCount;
extern int lOfsChange;
extern int ChangeCount;
extern int gObjCallMonCount;
extern int gObjMonCount;
extern int gObjCount;
// -------------------------------------------------------------------------------

class OBJECTSTRUCT_CLASS
{
public:
	OBJECTSTRUCT_CLASS();
	virtual ~OBJECTSTRUCT_CLASS();
};
// -------------------------------------------------------------------------------

extern OBJECTSTRUCT_CLASS	g_OBJECTSTRUCT_CLASS;
extern OBJECTSTRUCT			* gObj;
// -------------------------------------------------------------------------------

#include "ViewPortGuild.h"
#include "protocol.h"
// -------------------------------------------------------------------------------

void gObjSkillUseProcTime500(LPOBJ lpObj);
void MonsterAndMsgProc();
void MoveMonsterProc();
void gObjRefillProcess(int aIndex, LPOBJ lpObj);
void gObjInit();
void gObjEnd();
void gObjClearViewportOfMine(LPOBJ lpObj);
void gObjClearViewport(LPOBJ lpObj);
void gObjCloseSet(int aIndex, int Flag);
void gObjCharTradeClear(LPOBJ lpObj);
void gObjCharZeroSet(int aIndex);
int gObjGetSocket(SOCKET socket);
void gObjSetTradeOption(int aIndex, int option);
void gObjSetDuelOption(int aIndex, int option);
bool IsDuelEnable(int aIndex);
bool IsOnDuel(int aIndex1, int aIndex2);
int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over);
void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, int ItemDbByte, bool* bAllItemExist);
void gObjSetBP(int aIndex);
int gObjSetCharacter(LPBYTE lpdata, int aIndex);
int gObjCanItemTouch(LPOBJ lpObj, int type);
void gObjMagicTextSave(LPOBJ lpObj);
void ItemIsBufExOption(BYTE * buf, class CItem* lpItem);
void gObjStatTextSave(LPOBJ lpObj);
void gObjItemTextSave(LPOBJ lpObj);
void gObjWarehouseTextSave(LPOBJ lpObj);
void gObjAuthorityCodeSet(LPOBJ lpObj);
int gObjSetPosMonster(int aIndex, int PosTableNum);
int gObjSetMonster(int aIndex, int MonsterClass);
void gObjDestroy(SOCKET aSocket, int client);
short gObjAddSearch(SOCKET aSocket, char* ip);
short gObjAdd(SOCKET aSocket, char* ip, int aIndex);
short gObjMonsterRecall(int iMapNumber);
short gObjAddMonster(int iMapNumber);
short gObjAddCallMon();
void gObjUserKill(int aIndex);
void gObjAllLogOut();
void gObjAllDisconnect();
void gObjTradeSave(LPOBJ lpObj, int index);
short gObjMemFree(int index);
int gObjGameClose(int aIndex);
short gObjDel(int index);
short gObjSometimeClose(int index);
int gObjIsGamePlaing(LPOBJ lpObj);
int gObjIsConnectedGP(int aIndex, char* CharName);
int gObjIsConnectedGP(int aIndex);
int gObjIsConnected(LPOBJ lpObj, int dbnumber);
int gObjIsConnected(int aIndex);
int gObjIsConnected(LPOBJ lpObj);
BOOL gObjIsConnectedEx(int aIndex);
int gObjIsAccontConnect(int aIndex, char* accountid);
int gObjJoominCheck(int aIndex, char* szInJN);
int gObjTaiwanJoominCheck(int aIndex, char* szInJN);
int gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName);
BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber,int ukn_30, char* JoominNumber,BYTE PcBangRoom);
int gObjGetUserNumber(int aIndex);
int gObjGetNumber(int aIndex, int& UN, int& DBN);
LPSTR gObjGetAccountId(int aIndex);
int gObjGetIndex(char* szId);
int gObjUserIdConnectCheck(char* szId, int index);
BYTE GetPathPacketDirPos(int px, int py);
int gObjCalDistance(LPOBJ lpObj1, LPOBJ lpObj2);
int gObjPositionCheck(LPOBJ lpObj);
int gObjCheckTileArea(int aIndex, int x, int y, int dis);
int ExtentCheck(int x, int y, int w, int h);
void gObjSetInventory1Pointer(LPOBJ lpObj);
void gObjSetInventory2Pointer(LPOBJ lpObj);
void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode);
void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition);
void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2);
void gObjMsgProc(LPOBJ lpObj);
void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
int gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj);
int BackSpringCheck(int& x, int& y, int& dir, BYTE map);
int gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count);
bool gObjLevelUp(LPOBJ lpObj, __int64 & addexp, int iMonsterType, int iEventType);
int gObjLevelUpPointAdd(BYTE type, LPOBJ lpObj);
void gObjMonsterItemLoadScript(LPOBJ lpObj, char* script_file);
void gObjGiveItemSearch(LPOBJ lpObj, int maxlevel);
BYTE gObjWarehouseInsertItem(LPOBJ lpObj, int type, int index, int level, int pos);
void gObjGiveItemWarehouseSearch(LPOBJ lpObj, int maxlevel);
int gObjGuildMasterCapacityTest(LPOBJ lpObj);
void gObjNextExpCal(LPOBJ lpObj);
BOOL retResistance(LPOBJ lpObj, int Resistance_Type);
int retCalcSkillResistance(LPOBJ lpTargetObj, int isDouble);
int gObjAttackQ(LPOBJ lpObj);
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj, int MSBDamage);
void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjInterfaceTimeCheck(LPOBJ lpObj);
void gObjPkDownTimeCheck(LPOBJ lpObj, int TargetLevel);
BOOL gObjAngelSprite(LPOBJ lpObj);
BOOL gObjSatanSprite(LPOBJ lpObj);
BOOL gObjDemonSprite(LPOBJ lpObj);
BOOL gObjSpiritGuardianSprite(LPOBJ lpObj);
BOOL gObjPandaSprite(LPOBJ lpObj);
BOOL gObjUnicornSprite(LPOBJ lpObj); // Season 5 Episode 2 JPN
BOOL gObjSkeletonSprite(LPOBJ lpObj); // Season 5 Episode 2 JPN
BOOL gObjFindInventoryEquipment(int aIndex);	//1.01.03
BOOL gObjSetInventoryEquipment(LPOBJ lpObj, BYTE btPos);	//1.01.03
BOOL gObjInventoryEquipment(LPOBJ lpObj);	//1.01.03
int gObjWingSprite(LPOBJ lpObj);
int gObjUniriaSprite(LPOBJ lpObj);
void gObjSpriteDamage(LPOBJ lpObj, int damage);
void gObjSecondDurDown(LPOBJ lpObj);
void gObjChangeDurProc(LPOBJ lpObj);
void gObjChangeDurProc2(LPOBJ lpObj);
void gObjWingDurProc(LPOBJ lpObj);
void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type);
void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj);
bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
int gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, BYTE MSBFlag, int AttackDamage, BOOL bCombo, BYTE byBarrageCount, BYTE byReflect);
void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag);
int gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool& bSendExp);
void gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, int MSBFlag);
void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj);
void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage);
BOOL CheckOutOfInventory(int aIndex, int sy, int height);	//-> 1.01.00
BOOL InventoryExtentCheck(int x, int y, int w, int h);		//-> 1.01.00
BOOL CheckOutOfWarehouse(int aIndex, int sy, int height);	//-> 1.01.00
BOOL WarehouseExtentCheck(int x, int y, int w, int h);		//-> 1.01.00
int gObjInventoryTrans(int aIndex);
int gObjInventoryCommit(int aIndex);
int gObjInventoryRollback(int aIndex);
void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte);
void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
bool gObjFixInventoryPointer(int aIndex);
void gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte);
void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
BYTE gObjInventoryDeleteItem(int aIndex, int itempos);
BYTE gObjWarehouseDeleteItem(int aIndex, int itempos);
BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos);
BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
DWORD gGetItemNumber();
void gPlusItemNumber();
BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
BYTE gObjOnlyInventoryInsertItem(int aIndex, class CItem item);
BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level);
int gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel);
int gObjCashShopSearchItem(LPOBJ lpObj, int item_type, int nLevel, int add_dur, int arg5);
int gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur);
BYTE gObjShopBuyInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
BYTE gObjShopBuyInventoryInsertItem(int aIndex, class CItem item);
BYTE gObjInventoryInsertItemTemp(LPOBJ lpObj, class CMapItem* Item);
BYTE gObjInventoryInsertItem(int aIndex, class CMapItem* item);
BYTE gObjInventoryInsertItem(int aIndex, class CItem item);
BYTE gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1, int op2, int op3);
BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
BOOL CheckLotteryInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
BOOL CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
int gObjIsItemPut(LPOBJ lpObj, class CItem* lpItem, int pos);
BYTE gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height);
BYTE gObjMapRectCheck(BYTE * lpMapBuf, int sx, int sy, int ex, int ey, int width, int height);
void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte);
void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte);
BYTE gObjChaosBoxInsertItemPos(int aIndex, class CItem item, int pos, int source);
BYTE gObjWarehouseInsertItemPos(int aIndex, class CItem item, int pos, int source);
BYTE gObjInventoryInsertItemPos(int aIndex, class CItem item, int pos, int RequestCheck);
int gObjInventorySearchSerialNumber(LPOBJ lpObj, DWORD serial);
int gObjWarehouseSearchSerialNumber(LPOBJ lpObj, DWORD sirial);
BYTE gObjInventoryMoveItem(int aIndex, BYTE source, BYTE target, int& durSsend, int& durTsend, BYTE sFlag, BYTE tFlag, LPBYTE siteminfo);
BYTE gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height);
int gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
BYTE gObjTradeInventoryMove(LPOBJ lpObj, BYTE source, BYTE target);
BYTE gObjInventoryTradeMove(LPOBJ lpObj, BYTE source, BYTE target);
BYTE gObjTradeTradeMove(LPOBJ lpObj, BYTE source, BYTE target);
void gObjTradeCancel(int aIndex);
void gObjTempInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte);
BYTE gObjTempInventoryRectCheck(int aIndex, BYTE * TempMap, int sx, int sy, int width, int height);
BYTE gObjTempInventoryInsertItem(LPOBJ lpObj, class CItem item, BYTE * TempMap);
int TradeItemInventoryPutTest(int aIndex);
int TradeitemInventoryPut(int aIndex);
void gObjTradeOkButton(int aIndex);
BYTE LevelSmallConvert(int level);
BYTE LevelSmallConvert(int aIndex, int inventoryindex);
void gObjAttackDamageCalc(int aIndex);
void gObjMakePreviewCharSet(int aIndex);
void gObjViewportPaint(HWND hWnd, short aIndex);
void InitFrustrum();
void CreateFrustrum(int x, int y, int aIndex);
bool TestFrustrum2(int x, int y, int aIndex);
int gObjCheckViewport(int aIndex, int x, int y);
void gObjViewportClose(LPOBJ lpObj);
void gObjViewportListCreate(short aIndex);
int ViewportAdd(int aIndex, int aAddIndex, int aType);
int ViewportAdd2(int aIndex, int aAddIndex, int aType);
void ViewportDel(short aIndex, int aDelIndex);
void Viewport2Del(short aIndex, int aDelIndex);
void gObjViewportAllDel(short aIndex);
void gObjViewportListDestroy(short aIndex);
void PShop_ViewportListRegenarate(short aIndex);
bool PShop_CheckInventoryEmpty(short aIndex);
void gObjStateSetCreate(int aIndex);
void gObjSetState();
void gObjSecondProc();
void gObjManaPotionFill(LPOBJ lpObj);
void gObjRestPotionFill(LPOBJ lpObj);
void gObjUseDrink(LPOBJ lpObj, int level);
int gObjCurMoveMake(BYTE * const path, LPOBJ lpObj);
void gObjViewportListProtocolDestroy(LPOBJ lpObj);
void gObjViewportListProtocolCreate(LPOBJ lpObj);
void gObjViewportListProtocol(short aIndex);
void gObjSkillUseProc(LPOBJ lpObj);
void gObjSkillBeAttackProc(LPOBJ lpObj);
void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y);
int gObjMoveGate(int aIndex, int gt);
void gObjTeleport(int aIndex, int map, int x, int y);
void gObjMoveDataLoadingOK(int aIndex);
class CMagicInf* gObjGetMagic(LPOBJ lpObj, int mIndex);
class CMagicInf* gObjGetMagicSearch(LPOBJ lpObj, int skillnumber);
int gObjMagicManaUse(LPOBJ lpObj, class CMagicInf* lpMagic);
int gObjMagicBPUse(LPOBJ lpObj, class CMagicInf* lpMagic);
int gObjPosMagicAttack(LPOBJ lpObj, class CMagicInf* lpMagic, BYTE x, BYTE y);
void gObjMagicAddEnergyCheckOnOff(int flag);
int gObjMagicAdd(LPOBJ lpObj, int aSkill, BYTE Level);
int gObjMagicDel(LPOBJ lpObj, DWORD aSkill, BYTE Level);
int gObjMagicAdd(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, short & SkillNumber);
int gObjWeaponMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level);
int gObjMonsterMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level);
int gObjMagicEnergyCheck(LPOBJ lpObj, WORD aSkill);
int gObjItemLevelUp(LPOBJ lpObj, int source, int target);
int gObjItemRandomLevelUp(LPOBJ lpObj, int source, int target);
int gObjItemRandomOption3Up(LPOBJ lpObj, int source, int target);
void gObjAbilityReSet(LPOBJ lpObj);
int gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjGuildWarEndSend(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, BYTE Result1, BYTE Result2);
void gObjGuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2);
void gObjGuildWarEnd(struct _GUILD_INFO_STRUCT* lpGuild, struct _GUILD_INFO_STRUCT* lpTargetGuild);
int gObjGuildWarProc(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, int score);
int gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGuildWarMasterClose(LPOBJ lpObj);
int gObjGuildWarItemGive(struct _GUILD_INFO_STRUCT* lpWinGuild, struct _GUILD_INFO_STRUCT* lpLoseGuild);
int gObjGetPkTime(LPOBJ lpObj, int& hour, int& min);
int gObjMonsterCall(int aIndex, int MonsterType, int x, int y);
void gObjMonsterCallKill(int aIndex);
int gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y);
int gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex);
int gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex);
int gObjCheckAttackArea(int aIndex, int TarObjIndex);
int gUserFindDevilSquareInvitation(int aIndex);
int gUserFindDevilSquareKeyEyes(int aIndex);
void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel);
LPOBJ gObjFind(char* targetcharname);
int gObjFind10EventChip(int aIndex);
int gObjDelete10EventChip(int aIndex);
void gObjSetPosition(int aIndex, int x, int y);
int gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetItemCountInIventory(int aIndex, int itemnum);
int gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetManaItemPos(int aIndex);
void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count);
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint);
int gObjCheckStatPointUp(int aIndex);
int gObjCheckStatPointDown(int aIndex);
void gObjUseCircle(int aIndex, int pos);
void gObjUsePlusStatFruit(int aIndex, int pos);
void gObjUseMinusStatFruit(int aIndex, int pos);
void gObjUseCashShopStatFruit(LPOBJ lpObj, int pos);
void gObjUseCashShopStatPotion(LPOBJ lpObj, int pos);
void gObjUseFullRecoveryPotion(LPOBJ lpObj, int pos);
void gObjUseRecoveryPotion(LPOBJ lpObj, int pos, double value);
void gObjUseIndulgence(LPOBJ lpObj, int pos);
void gObjCalcMaxLifePower(int aIndex);
void gObjDelayLifeCheck(int aIndex);
int gObjDuelCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjDuelCheck(LPOBJ lpObj);
void gObjResetDuel(LPOBJ lpObj);
void gObjSendDuelEnd(LPOBJ lpObj);
void gObjCheckAllUserDuelStop();
//void gObjNProtectGGCheckSum(int aIndex);
void SkillFrustrum(BYTE bangle, int aIndex);
int SkillTestFrustrum(int x, int y, int aIndex);
int gObjCheckMaxZen(int aIndex, int nAddZen);
void MakeRandomSetItem(int aIndex);
void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber);
void gObjRecall(int aIndex, int mapnumber, int x, int y);
void gObjSetExpPetItem(int aIndex, int exp);
int gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);
int gObjGetRandomFreeLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);
int gObjCheckAttackTypeMagic(int iClass, int iSkill);
BOOL gObjGetRandomFreeArea(int iMapNumber, BYTE &cX, BYTE &cY, int iSX, int iSY, int iDX, int iDY, int iLoopCount);
int gObjGetGuildUnionNumber(LPOBJ lpObj);
void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen);
int gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjNotifyUpdateUnionV1(LPOBJ lpObj);
void gObjNotifyUpdateUnionV2(LPOBJ lpObj);
void gObjUnionUpdateProc(int aIndex);
void gObjSetKillCount(int aIndex, int iOption);
void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel);
void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue);
void gObjReady4Relife(LPOBJ lpObj);
int gObjCheckUserPCBang(LPOBJ lpObj);
BOOL IsOKPCBangBenefitAll(LPOBJ lpObj);
void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick);
void MsgOutput(int aIndex, char* msg, ...);
void gProcessAutoRecuperation(LPOBJ lpObj);
void gObjShieldAutoRefill(LPOBJ lpObj);
int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
int gObjCheckSerial0ItemList(class CItem* lpItem);
int gObjCheckInventorySerial0Item(LPOBJ lpObj);
BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum);
void gObjSaveChaosBoxItemList(LPOBJ lpObj);
BOOL gObjCheckRaiseTalismanMap(int MapNumber);
BOOL gObjItemUseNewJewels(LPOBJ lpObj, int source, int target,int type);
int gObjDeleteItemDur(int aIndex, OBJECTSTRUCT * lpObj, int ItemCode, int DurCount);
int gObjGetItemDur(int aIndex, OBJECTSTRUCT * lpObj, int ItemCode);
int gObjGetIndexByAccount(char* szId);
// -------------------------------------------------------------------------------