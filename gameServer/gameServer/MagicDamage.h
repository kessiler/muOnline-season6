//-> Decompiled by DarkSim | 17.06.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#define MAX_SKILL				620	//6.3 GMO
#define MAX_REQ_SKILL_STATUS	3
// -------------------------------------------------------------------------------

#include "..\common\classdef.h"
// -------------------------------------------------------------------------------

enum eSkillType	//not complete
{
	AT_SKILL_POISON = 0x1,
	AT_SKILL_METEO = 0x2,
	AT_SKILL_THUNDER = 0x3,
	AT_SKILL_FIREBALL = 0x4,
	AT_SKILL_FLAME = 0x5,
	AT_SKILL_TELEPORT = 0x6,
	AT_SKILL_SLOW = 0x7,
	AT_SKILL_STORM = 0x8,
	AT_SKILL_EVIL = 0x9,
	AT_SKILL_HELL = 0xa,
	AT_SKILL_POWERWAVE = 0xb,
	AT_SKILL_FLASH = 0xc,
	AT_SKILL_BLAST = 0xd,
	AT_SKILL_INFERNO = 0xe,
	AT_SKILL_TARGET_TELEPORT = 0xf,
	AT_SKILL_MAGICDEFENSE = 0x10,
	AT_SKILL_ENERGYBALL = 0x11,
	AT_SKILL_BLOCKING = 0x12,
	AT_SKILL_SWORD1 = 0x13,
	AT_SKILL_SWORD2 = 0x14,
	AT_SKILL_SWORD3 = 0x15,
	AT_SKILL_SWORD4 = 0x16,
	AT_SKILL_SWORD5 = 0x17,
	AT_SKILL_CROSSBOW = 0x18,
	AT_SKILL_BOW = 0x19,
	AT_SKILL_HEALING = 0x1a,
	AT_SKILL_DEFENSE = 0x1b,
	AT_SKILL_ATTACK = 0x1c,
	AT_SKILL_CALLMON1 = 0x1e,
	AT_SKILL_CALLMON2 = 0x1f,
	AT_SKILL_CALLMON3 = 0x20,
	AT_SKILL_CALLMON4 = 0x21,
	AT_SKILL_CALLMON5 = 0x22,
	AT_SKILL_CALLMON6 = 0x23,
	AT_SKILL_CALLMON7 = 0x24,
	AT_SKILL_WHEEL = 0x29,
	AT_SKILL_BLOWOFFURY = 0x2a,
	AT_SKILL_STRIKE = 0x2b,
	AT_SKILL_KNIGHTSPEAR = 0x2f,
	AT_SKILL_KNIGHTADDLIFE = 0x30,
	AT_SKILL_KNIGHTDINORANT = 0x31,
	AT_SKILL_ELFHARDEN = 0x33,
	AT_SKILL_PENETRATION = 0x34,
	AT_SKILL_DEFENSEDOWN = 0x37,
	AT_SKILL_SWORD6 = 0x38,
	AT_SKILL_5CROSSBOW = 0x36,
	AT_SKILL_EXPPOISON = 0x26,
	AT_SKILL_EXPICE = 0x27,
	AT_SKILL_EXPHELL = 0x28,
	AT_SKILL_EXPHELL_START = 0x3a,
	AT_SKILL_IMPROVE_AG_REFILL = 0x35,
	AT_SKILL_DEVILFIRE = 0x32,
	AT_SKILL_COMBO = 0x3b,
	AT_SKILL_SPEAR = 0x3c,
	AT_SKILL_FIREBURST = 0x3d,
	AT_SKILL_DARKHORSE_ATTACK = 0x3e,
	AT_SKILL_RECALL_PARTY = 0x3f,
	AT_SKILL_ADD_CRITICALDAMAGE = 0x40,
	AT_SKILL_ELECTRICSPARK = 0x41,
	AT_SKILL_LONGSPEAR = 0x42,
	AT_SKILL_RUSH = 0x2c,
	AT_SKILL_JAVALIN = 0x2d,
	AT_SKILL_DEEPIMPACT = 0x2e,
	AT_SKILL_ONE_FLASH = 0x39,
	AT_SKILL_DEATH_CANNON = 0x49,
	AT_SKILL_SPACE_SPLIT = 0x4a,
	AT_SKILL_BRAND_OF_SKILL = 0x4b,
	AT_SKILL_STUN = 0x43,
	AT_SKILL_REMOVAL_STUN = 0x44,
	AT_SKILL_ADD_MANA = 0x45,
	AT_SKILL_INVISIBLE = 0x46,
	AT_SKILL_REMOVAL_INVISIBLE = 0x47,
	AT_SKILL_REMOVAL_MAGIC = 0x48,
	AT_SKILL_FENRIR_ATTACK = 0x4c,
	AT_SKILL_INFINITY_ARROW = 0x4d,
	AT_SKILL_FIRESCREAM = 0x4e,
	AT_SKILL_EXPLOSION = 0x4f,
	AT_SKILL_SUMMON = 0xc8,
	AT_SKILL_IMMUNE_TO_MAGIC = 0xc9,
	AT_SKILL_IMMUNE_TO_HARM = 0xca,

	AT_SKILL_RAGEFIGHTER_WEAPON1 = 0x104,
	AT_SKILL_RAGEFIGHTER_WEAPON2 = 0x105,
	AT_SKILL_RAGEFIGHTER_CHAINDRIVE = 0x106,
	AT_SKILL_RAGEFIGHTER_DARKSIDE = 0x107,
	AT_SKILL_RAGEFIGHTER_DRAGONLORE = 0x108,
	AT_SKILL_RAGEFIGHTER_DRAGONSLAYER = 0x109,
	AT_SKILL_RAGEFIGHTER_REDARMOR = 0x10A,
	AT_SKILL_RAGEFIGHTER_FITNESS = 0x10B,
	AT_SKILL_RAGEFIGHTER_DEFRATE = 0x10C,
	AT_SKILL_RAGEFIGHTER_SIEGE = 0x10D,
	AT_SKILL_PHOENIXSHOT = 0x10E,
};
// -------------------------------------------------------------------------------

class CMagicDamage
{
private:
	int		Get(int skill);
	int		CheckSkillAttr(int skill, int attr);
	void	Set(char * name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay, BYTE byItemSkill, BYTE byIsDamage);
	void	SetMasterLevelSkillInfo(int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex);
	void	SetEx(int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int * pReqStatus);
	// ----
public:
			CMagicDamage();
	virtual ~CMagicDamage();
	// ----
	void	Init();
	void	LogSkillList(char* Buffer, int iSize);
	void	LogSkillList(char* filename);
	void	LogSkillNameList(char* Buffer, int iSize);
	void	LogSkillNameList(char* filename);
	int		SkillGet(int skill);
	int		SkillGetMana(int skill);
	int		SkillGetRequireEnergy(int skill);
	int		SkillGetRequireLevel(int skill);
	int		SkillGetRequireClass(int Cclass, int ChangeUP, int ThirdChangeUp, int skill);
	int		SkillGetBP(int skill);
	int		GetSkillAttr(int skill);
	int		GetskillRequireLeadership(int skill);
	int		GetDelayTime(int skill);
	int		GetSkillType(int iSkill);
	int		CheckStatus(int iSkill, int iGuildStatus);
	int		CheckBrandOfSkill(int iSkill);
	int		CheckKillCount(int iSkill, int iKillCount);
	int		GetSkillDistance(int skill);
	int		GetRequireMLPoint(int iSkill);
	int		GetRequireStrength(int iSkill);
	int		GetRequireDexterity(int iSkill);
	WORD	GetBrandOfSkill(int iSkill);
	WORD	GetNormalBrandOfSkill(int iSkill);
	BYTE	IsItemAttachedSkill(int iSkill);
	int		GetSkillUseType(int iSkill);
	int		GetSkillLevel(int iSkill);
	int		GetSkillGroup(int iSkill);
	int		GetSkillRank(int iSkill);
	// ----
private:
	int		m_Damage[MAX_SKILL];								//+0x4
	char	m_Name[MAX_SKILL][50];								//+0x964
	BYTE	m_rLevel[MAX_SKILL];								//+0x7e94
	WORD	m_Mana[MAX_SKILL];									//+0x80ec
	BYTE	m_Distance[MAX_SKILL];								//+0x859c
	int		m_RequireEnergy[MAX_SKILL];							//+0x87f4
	BYTE	m_RequireClass[MAX_SKILL][MAX_TYPE_PLAYER];			//+0x9154
	int		m_RequireLevel[MAX_SKILL];							//+0xa1bc
	int		m_RequireLeadership[MAX_SKILL];						//+0xab1c
	int		m_Delay[MAX_SKILL];									//+0xb47c
	WORD	m_BrainPower[MAX_SKILL];							//+0xbddc
	int		m_Attr[MAX_SKILL];									//+0xc28c
	int		m_iSkillType[MAX_SKILL];							//+0xcbec
	int		m_iSkillUseType[MAX_SKILL];							//+0xd54c
	WORD	m_iSkillBrand[MAX_SKILL];							//+0xdeac
	int		m_iKillCount[MAX_SKILL];							//+0xe35c
	int		m_iRequireStatus[MAX_SKILL][MAX_REQ_SKILL_STATUS];	//+0xecbc
    short	m_sSkillRank[MAX_SKILL];							//+0x108dc
    short	m_sSkillGroup[MAX_SKILL];							//+0x10d8c
    short	m_sReqm_iMasterLevelPoint[MAX_SKILL];				//+0x1123c
    short	m_sReqAG[MAX_SKILL];								//+0x
    short	m_sReqShield[MAX_SKILL];							//+0x
    short	m_sDuration[MAX_SKILL];								//+0x
    short	m_sReqStrength[MAX_SKILL];							//+0x124fc
    short	m_sReqDexterity[MAX_SKILL];							//+0x129ac
	BYTE	m_byItemSkill[MAX_SKILL];							//+0x12e5c
	BYTE	m_byIsDamage[MAX_SKILL];							//+0x130b4
	// ----
	enum 
	{
		SKILL_TYPE_NONE		= -1,
		SKILL_TYPE_PHYSICS	= 0,
		SKILL_TYPE_MAGIC	= 1,
	};
	// ----
}; extern CMagicDamage MagicDamageC;
// -------------------------------------------------------------------------------