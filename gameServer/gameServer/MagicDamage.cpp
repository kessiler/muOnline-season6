//-> Decompiled by DarkSim | 17.06.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "MagicDamage.h"
#include "..\include\readscript.h"
#include "..\common\WzMemScript.h"
#include "LogProc.h"
#include "GameMain.h"
#include "MasterLevelSkillTreeSystem.h"
#include "SkillAdditionInfo.h"
// -------------------------------------------------------------------------------

CMagicDamage MagicDamageC;
// -------------------------------------------------------------------------------

//004c2170	-> 100% (Identical)
CMagicDamage::CMagicDamage()
{
	return;
}
// -------------------------------------------------------------------------------

//004c21f0	-> 100% (Identical)
CMagicDamage::~CMagicDamage()
{
	return;
}
// -------------------------------------------------------------------------------

//004c2220	-> 100% (Identical)
void CMagicDamage::Init()
{
	for( int n = 0; n < MAX_SKILL; n++ )
	{
		this->m_Damage[n] = -1;
	}
}
// -------------------------------------------------------------------------------

//004c2270	-> 100% (Identical)
void CMagicDamage::LogSkillList(char * filename)
{
	this->Init();
	// ----
	SMDFile = fopen(filename, "r");	//ok
	// ----
	if( SMDFile == NULL )
	{
		MsgBox(lMsg.Get(MSGGET(1, 199)), filename);
		return;
	}
	// ----
	int Token;
	int n=0;
	int number;
	char name[50];
	int requireLevel;
	int damage;
	int mana;
	int bp=0;
	int dis;
	int rEnergy;
	int iSkillType;
	int rLeadership;
	int rDelay;
	int attr;
	int iSkillUseType;
	int iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_REQ_SKILL_STATUS] = {0};
	BYTE RequireClass[MAX_TYPE_PLAYER];
	short SkillRank;
	short SkillGroup;
	short rm_iMasterLevelPoint;
	short rAG;
	short rShield;
	short Duration;
	short rStrength;
	short rDexterity;
	BYTE IconNumber;
	int iUseType;
	BYTE btItemSkill;
	BYTE btIsDamage;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		if( Token == NUMBER )
		{
			number = TokenNumber;
			// ----
			Token = GetToken();
			strcpy(name, TokenString);
			// ----
			Token = GetToken();
			requireLevel = TokenNumber;
			// ----
			Token = GetToken();
			damage = TokenNumber;
			// ----
			Token = GetToken();
			mana = TokenNumber;
			// ----
			Token = GetToken();
			bp = TokenNumber;
			// ----
			Token = GetToken();
			dis = TokenNumber;
			// ----
			Token = GetToken();
			rDelay = TokenNumber;
			// ----
			Token = GetToken();
			rEnergy = TokenNumber;
			// ----
			Token = GetToken();
			rLeadership = TokenNumber;
			// ----
			Token = GetToken();
			attr = TokenNumber;
			// ----
			Token = GetToken();
			iSkillType = TokenNumber;
			// ----
			Token = GetToken();
			iSkillUseType = TokenNumber;
			// ----
			Token = GetToken();
			iSkillBrand = TokenNumber;
			// ----
			Token = GetToken();
			iKillCount = TokenNumber;
			// ----
			Token = GetToken();
			RequireStatus[0] = TokenNumber;
			// ----
			Token = GetToken();
			RequireStatus[1] = TokenNumber;
			// ----
			Token = GetToken();
			RequireStatus[2] = TokenNumber;
			// ----
			Token = GetToken();
			RequireClass[CLASS_WIZARD] = TokenNumber;
			// ----
			Token = GetToken();
			RequireClass[CLASS_KNIGHT] = TokenNumber;
			// ----
			Token = GetToken();
			RequireClass[CLASS_ELF] = TokenNumber;
			// ----
			Token = GetToken();
			RequireClass[CLASS_MAGUMSA] = TokenNumber;
			// ----
			Token = GetToken();
			RequireClass[CLASS_DARKLORD] = TokenNumber;
			// ----
			Token = GetToken();
			RequireClass[CLASS_SUMMONER] = TokenNumber;
			// ----
			Token = GetToken();
			RequireClass[CLASS_MONK] = TokenNumber;
			// ----
			Token = GetToken();
			SkillRank = TokenNumber;
			// ----
			Token = GetToken();
			SkillGroup = TokenNumber;
			// ----
			Token = GetToken();
			rm_iMasterLevelPoint = TokenNumber;
			// ----
			Token = GetToken();
			rAG = TokenNumber;
			// ----
			Token = GetToken();
			rShield = TokenNumber;
			// ----
			Token = GetToken();
			Duration = TokenNumber;
			// ----
			Token = GetToken();
			rStrength = TokenNumber;
			// ----
			Token = GetToken();
			rDexterity = TokenNumber;
			// ----
			Token = GetToken();
			IconNumber = TokenNumber;
			// ----
			Token = GetToken();
			iUseType = TokenNumber;
			// ----
			Token = GetToken();
			btItemSkill = TokenNumber;
			// ----
			Token = GetToken();
			btIsDamage = TokenNumber;
			// ----
			this->Set(name, number,damage, requireLevel, mana, bp, dis, attr, iSkillType, RequireClass, rEnergy, rLeadership, rDelay, btItemSkill, btIsDamage);
			this->SetEx(number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
			this->SetMasterLevelSkillInfo(number, SkillRank, SkillGroup, rm_iMasterLevelPoint, rAG, rShield, Duration, rStrength, rDexterity);
			DefMagicInf[number].Set(number, 0);
		}
	}
	// ----
	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 200)), filename);
}
// -------------------------------------------------------------------------------

//004c2d30	-> 100% (Identical)
void CMagicDamage::LogSkillNameList(char* filename)
{
	int Token;
	int n;
	int number;
	char name[50];
	// ----
	SMDFile = fopen(filename, "r");	//ok
	// ----
	if( SMDFile == NULL )
	{
		MsgBox(lMsg.Get(MSGGET(1, 199)), filename);
		return;
	}
	// ----
	n = 0;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		if( Token == NUMBER )
		{
			number = TokenNumber;
			// ----
			Token = GetToken();
			strcpy(name, TokenString);
			strcpy(this->m_Name[number], name);
		}
	}
	// ----
	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 200)), filename);
}
// -------------------------------------------------------------------------------

//004c2e60	-> 100% (Identical)
void CMagicDamage::LogSkillList(char* Buffer, int iSize)
{
	this->Init();
	// ----
	CWzMemScript WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);
	// ----
	int Token;
	int n=0;
	int number;
	char name[50];
	int requireLevel;
	int damage;
	int mana;
	int bp=0;
	int dis;
	int rEnergy;
	int iSkillType;
	int rLeadership;
	int rDelay;
	int attr;
	int iSkillUseType;
	int iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_REQ_SKILL_STATUS] = {0};
	BYTE RequireClass[MAX_TYPE_PLAYER];
	int SkillRank;
	int SkillGroup;
	short rm_iMasterLevelPoint;
	int rAG;
	int rShield;
	short Duration;
	int rStrength;
	int rDexterity;
	BYTE IconNumber;
	int iUseType;
	BYTE btItemSkill;
	BYTE btIsDamage;
	// ----
	while(true)
	{
		Token = WzMemScript.GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		if( Token == NUMBER )
		{
			number = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			strcpy(name, WzMemScript.GetString() );
			// ----
			Token = WzMemScript.GetToken();
			requireLevel = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			damage = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			mana = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			bp = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			dis = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rDelay = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rEnergy = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rLeadership = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			attr = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			iSkillType = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			iSkillUseType = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			iSkillBrand = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			iKillCount = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			RequireStatus[0] = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			RequireStatus[1] = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			RequireStatus[2] = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			RequireClass[CLASS_WIZARD] = WzMemScript.GetNumber(); //SM
			// ----
			Token = WzMemScript.GetToken();
			RequireClass[CLASS_KNIGHT] = WzMemScript.GetNumber(); //DK
			// ----
			Token = WzMemScript.GetToken();
			RequireClass[CLASS_ELF] = WzMemScript.GetNumber(); //ELF
			// ----
			Token = WzMemScript.GetToken();
			RequireClass[CLASS_MAGUMSA] = WzMemScript.GetNumber(); //MG
			// ----
			Token = WzMemScript.GetToken();
			RequireClass[CLASS_DARKLORD] = WzMemScript.GetNumber(); //DL
			// ----
			Token = WzMemScript.GetToken();
			RequireClass[CLASS_SUMMONER] = WzMemScript.GetNumber(); //SUM
			// ----
			Token = WzMemScript.GetToken();
			RequireClass[CLASS_MONK] = WzMemScript.GetNumber(); //RF
			// ----
			Token = WzMemScript.GetToken();
			SkillRank = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			SkillGroup = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rm_iMasterLevelPoint = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rAG = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rShield = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			Duration = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rStrength = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			rDexterity = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			IconNumber = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			iUseType = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			btItemSkill = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();
			btIsDamage = WzMemScript.GetNumber();
			// ----
			this->Set(name, number,damage, requireLevel, mana, bp, dis, attr, iSkillType, RequireClass, rEnergy, rLeadership, rDelay, btItemSkill, btIsDamage);
			this->SetEx(number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
			this->SetMasterLevelSkillInfo(number, SkillRank, SkillGroup, rm_iMasterLevelPoint, rAG, rShield, Duration, rStrength, rDexterity);
			DefMagicInf[number].Set(number, 0);
		}
	}
	// ----
	LogAdd(lMsg.Get(MSGGET(1, 200)), "Skill");
}
// -------------------------------------------------------------------------------

//004c3700	-> 100% (Identical)
void CMagicDamage::LogSkillNameList(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int Token;
	int n;
	int number;
	char name[50];
	// ----
	WzMemScript.SetBuffer(Buffer, iSize);
	// ----
	n = 0;
	// ----
	while(true)
	{
		Token = WzMemScript.GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		if( Token == NUMBER )
		{
			number = TokenNumber;
			// ----
			Token = WzMemScript.GetToken();	
			strcpy(name, TokenString);
			strcpy(this->m_Name[number], name);
		}
	}
	// ----
	LogAdd(lMsg.Get(MSGGET(1, 200)), "Skill_Local");
}
// -------------------------------------------------------------------------------

//004c3870	-> (Identical)
void CMagicDamage::SetMasterLevelSkillInfo(int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		MsgBox(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return;
	}
	//only
	//for
	//line
	//number
	if( this->m_Damage[iSkill] == -1 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 50)), 
			__FILE__,
			__LINE__);	//Line: 17
		return;
	}
	// ----
	this->m_sSkillRank[iSkill]				= iSkillRank;
	this->m_sSkillGroup[iSkill]				= iSkillGroup;
	this->m_sReqm_iMasterLevelPoint[iSkill] = iRequireMLPoint;
	this->m_sReqAG[iSkill]					= iHP;
	this->m_sReqShield[iSkill]				= iSD;
	this->m_sDuration[iSkill]				= wKeepTime;
	this->m_sReqStrength[iSkill]			= iRequireStr;
	this->m_sReqDexterity[iSkill]			= iRequireDex;
}
// -------------------------------------------------------------------------------

//004c39e0	-> 100% (Identical)
void CMagicDamage::SetEx(int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int * pReqStatus)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		MsgBox(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return;
	}
	//only
	//for
	//line
	//number
	if( this->m_Damage[iSkill] == -1 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 50)), 
			__FILE__,
			__LINE__);	//Line: 17
		return;
	}
	// ----
	this->m_iSkillUseType[iSkill]	= iSkillUseType;
	this->m_iSkillBrand[iSkill]		= iSkillBrand;
	this->m_iKillCount[iSkill]		= iKillCount;
	memcpy(this->m_iRequireStatus[iSkill], pReqStatus, sizeof(this->m_iRequireStatus[0]));
}
// -------------------------------------------------------------------------------

//004c3b00	-> 100% (Identical)
void CMagicDamage::Set(char * name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay, BYTE byItemSkill, BYTE byIsDamage)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		MsgBox(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return;
	}
	//for line
	//number
	if( this->m_Damage[skill] != -1 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 50)), 
			__FILE__, 
			__LINE__);	//Line: 15
		return;
	}
	// ----
	strcpy(this->m_Name[skill], name);
	this->m_Damage[skill]				= damage;
	this->m_rLevel[skill]				= rlevel;
	this->m_Mana[skill]					= mana;
	this->m_Distance[skill]				= dis;
	this->m_RequireLeadership[skill]	= rLeadership;
	this->m_Delay[skill]				= rdelay;
	this->m_BrainPower[skill]			= bp;
	this->m_Attr[skill]					= Attr;	
	this->m_iSkillType[skill]			= iSkillType;
	memcpy(this->m_RequireClass[skill], ReqClass, sizeof(this->m_RequireClass[0]));
	// ----
	int nBaseSkillReqEng = (rEnergy * rlevel * 4);
	// ----
	if(nBaseSkillReqEng > 0)
	{
		nBaseSkillReqEng = nBaseSkillReqEng / 100;
	}
	else
	{
		nBaseSkillReqEng = 0;
	}
	// ----
	this->m_RequireEnergy[skill]	= nBaseSkillReqEng + 20;
	this->m_byItemSkill[skill]		= byItemSkill;
	this->m_byIsDamage[skill]		= byIsDamage;
	// ----
	//if( skill == AT_SKILL_BLAST )
	//{
	//	LogAddL("skill:%d", this->m_RequireEnergy[skill]);
	//}
	// ----
	this->m_RequireLevel[skill] = 0;
	// ----
#ifdef __CUSTOMS__
	if( g_SkillAdditionInfo.m_SkillUseReqLevel[skill] > 0 )
	{
		this->m_RequireLevel[skill] = g_SkillAdditionInfo.m_SkillUseReqLevel[skill];
	}
#else
	if( skill == AT_SKILL_WHEEL )
	{
		this->m_RequireLevel[skill] = 80;
	}
	else if( skill == AT_SKILL_BLOWOFFURY )
	{
		this->m_RequireLevel[skill] = 170;
	}
	else if( skill == AT_SKILL_STRIKE )
	{
		this->m_RequireLevel[skill] = 160;
	}
	else if( skill == AT_SKILL_KNIGHTSPEAR )
	{
		this->m_RequireLevel[skill] = 28;
	}
	else if( skill == AT_SKILL_KNIGHTADDLIFE ) 
	{
		this->m_RequireLevel[skill] = 120;
	}
	else if( skill == AT_SKILL_PENETRATION )	
	{
		this->m_RequireLevel[skill] = 130;
	}
#endif
	// ----
	if( skill == AT_SKILL_CALLMON1 )
	{
		this->m_RequireEnergy[skill] = 30;
	}
	else if( skill == AT_SKILL_CALLMON2 )
	{
		this->m_RequireEnergy[skill] = 60;
	}
	else if( skill == AT_SKILL_CALLMON3 )
	{
		this->m_RequireEnergy[skill] = 90;
	}
	else if( skill == AT_SKILL_CALLMON4 )
	{
		this->m_RequireEnergy[skill] = 130;
	}
	else if( skill == AT_SKILL_CALLMON5 )
	{
		this->m_RequireEnergy[skill] = 170;
	}
	else if( skill == AT_SKILL_CALLMON6 )
	{
		this->m_RequireEnergy[skill] = 210;
	}
	else if( skill == AT_SKILL_CALLMON7 )
	{
		this->m_RequireEnergy[skill] = 300;
	}
	else if( skill == AT_SKILL_WHEEL )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_BLOWOFFURY )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_STRIKE )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_KNIGHTSPEAR )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_KNIGHTADDLIFE )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_KNIGHTDINORANT )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_DEFENSEDOWN )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_ELFHARDEN  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_PENETRATION )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_CROSSBOW  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_ENERGYBALL )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_BLOCKING )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_SWORD1 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_SWORD2 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_SWORD3 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_SWORD4 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_SWORD5 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if( skill == AT_SKILL_BOW )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 56  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 60  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 44  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 45  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 46  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 57  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 73  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 74  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 67  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 68  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 69  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 70  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 71  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 72  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 76  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 77  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 78  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 79  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 62  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 214  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 215  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 216  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 217  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 219  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 220  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 223  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 224  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 221  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 222  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 225  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 230  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 218  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 232  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 233  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 234  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 235  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 236  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 237  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 238  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 239  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 260  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 261  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 262  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 263  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 264  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 265  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 266  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 267  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 268  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 269  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 270  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
}
// -------------------------------------------------------------------------------

//004c48e0	-> 100% (Identical)
int CMagicDamage::Get(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return this->m_Damage[0];
	}
	//for line
	//number
	if( this->m_Damage[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 15
		return this->m_Damage[0];
	}
	// ----
	return this->m_Damage[skill];
}
// -------------------------------------------------------------------------------

//004c49b0	-> 100% (Identical)
int CMagicDamage::SkillGet(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return this->m_Damage[0];
	}
	//for line
	//number
	if( this->m_Damage[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 15
		LogAddC(2, "Skill: %d", skill);
		return this->m_Damage[0];
	}
	// ----
	return this->m_Damage[skill];
}
// -------------------------------------------------------------------------------

//004c4a80	-> 100% (Identical)
int CMagicDamage::SkillGetMana(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return this->m_Mana[0];
	}
	//for line
	//number
	if( this->m_Damage[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 15
		return this->m_Damage[0];
	}
	// ----
	return this->m_Mana[skill];
}
// -------------------------------------------------------------------------------

//004c4b60	-> 100% (Identical)
int CMagicDamage::SkillGetBP(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return 0;
	}
	//for line
	//number
	if( this->m_Damage[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 15
		return 0;
	}
	// ----
	return this->m_BrainPower[skill];
}
// -------------------------------------------------------------------------------

//004c4c30	-> 100% (Identical)
int CMagicDamage::SkillGetRequireEnergy(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	//for line
	//number
	if( this->m_RequireEnergy[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 15
		return -1;
	}
	// ----
	return this->m_RequireEnergy[skill];
}
// -------------------------------------------------------------------------------

//004c4d00	-> 100% (Identical)
BOOL CMagicDamage::SkillGetRequireClass(int Cclass, int ChangeUP, int ThirdChangeUp, int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	//for line
	//number
	if( Cclass < 0 
		|| Cclass > MAX_TYPE_PLAYER-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 15
		return -1;
	}
	// ----
	int requireclass = this->m_RequireClass[skill][Cclass];
	// ----
	if( requireclass == 0 )
	{
		return false;
	}
	// ----
	if( requireclass == 3 )
	{
		if( ThirdChangeUp > 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if( requireclass > 1 )
		{
			if( requireclass != (ChangeUP+1) )
			{
				return false;
			}
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004c4e20	-> 100% (Identical)
int CMagicDamage::GetSkillAttr(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 )
	{
		return -1;
	}
	// ----
	return this->m_Attr[skill];
}
// -------------------------------------------------------------------------------

//004c4e70	-> 100% (Identical)
int CMagicDamage::CheckSkillAttr(int skill, int attr)
{
	if( skill < 0 || skill > MAX_SKILL-1 )
	{
		return false;
	}
	// ----
	return this->m_Attr[skill];
}
// -------------------------------------------------------------------------------

//004c4ec0	-> 100% (Identical)
int CMagicDamage::GetskillRequireLeadership(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	//for line
	//number
	if( this->m_RequireLeadership[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 15
		return -1;
	}
	// ----
	return this->m_RequireLeadership[skill];
}
// -------------------------------------------------------------------------------

//004c4f90	-> 100% (Identical)
int CMagicDamage::GetDelayTime(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return false;
	}
	// ----
	return this->m_Delay[skill];
}
// -------------------------------------------------------------------------------

//004c5010	-> 100% (Identical)
int CMagicDamage::GetSkillType(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__,
			__LINE__);	//Line: 6
		return false;
	}
	// ----
	return this->m_iSkillType[iSkill];
}
// -------------------------------------------------------------------------------

//004c5090	-> 100% (Identical)
int CMagicDamage::SkillGetRequireLevel(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)),
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_RequireLevel[skill];
}
// -------------------------------------------------------------------------------

//004c5110	-> 100% (Identical)
BOOL CMagicDamage::CheckStatus(int iSkill, int iGuildStatus)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)),
			__FILE__, 
			__LINE__);	//Line: 6
		return false;
	}
	// ----
	if( this->m_iSkillUseType[iSkill] == false )
	{
		return true;
	}
	// ----
	if( g_MasterSkillSystem.CheckMasterLevelSkill(iSkill) != false )
	{
		return true;
	}
	// ----
	if( iGuildStatus == G_MASTER )
	{
		if( this->m_iRequireStatus[iSkill][0] == 1 )
		{
			return true;
		}
	}
	else if( iGuildStatus == G_SUB_MASTER )
	{
		if( this->m_iRequireStatus[iSkill][1] == 1 )
		{
			return true;
		}
	}
	else if( iGuildStatus == G_BATTLE_MASTER )
	{
		if( this->m_iRequireStatus[iSkill][2] == 1 )
		{
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//004c5240	-> 100% (Identical)
BOOL CMagicDamage::CheckBrandOfSkill(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return false;
	}
	// ----
	if( this->m_iSkillBrand[iSkill] > 0 )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//004c52d0	-> 100% (Identical)
int CMagicDamage::CheckKillCount(int iSkill, int iKillCount)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return iKillCount - this->m_iKillCount[iSkill];
}
// -------------------------------------------------------------------------------

//004c5350	-> 100% (Identical)
int CMagicDamage::GetSkillDistance(int skill)
{
	if( skill < 0 
		|| skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_Distance[skill];
}
// -------------------------------------------------------------------------------

//004c53d0	-> 100% (Identical)
int CMagicDamage::GetRequireMLPoint(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_sReqm_iMasterLevelPoint[iSkill];
}
// -------------------------------------------------------------------------------

//004c5450	-> 100% (Identical)
int CMagicDamage::GetRequireStrength(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_sReqStrength[iSkill];
}
// -------------------------------------------------------------------------------

//004c54d0	-> 100% (Identical)
int CMagicDamage::GetRequireDexterity(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_sReqDexterity[iSkill];
}
// -------------------------------------------------------------------------------

//004c5550	-> 100% (Identical)
WORD CMagicDamage::GetBrandOfSkill(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_iSkillBrand[iSkill];
}
// -------------------------------------------------------------------------------

//004c55d0	-> 100% (Identical)
WORD CMagicDamage::GetNormalBrandOfSkill(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	int	Cnt				= 0;
	WORD nBrandOfSkill, iBrandOfSkill = iSkill;
	// ----
	for(;;)
	{
		nBrandOfSkill = MagicDamageC.GetBrandOfSkill(iBrandOfSkill);
		// ----
		if( g_MasterSkillSystem.CheckMasterLevelSkill(nBrandOfSkill) )
		{
			iBrandOfSkill = nBrandOfSkill;
			// ----
			Cnt++;
			// ----
			if( Cnt > 3 )
			{
				LogAddTD("[GetNormalBrandOfSkill] fail!!! - %d", iSkill);
				return iSkill;
			}
			// ----
			continue;
		}
		// ----
		return nBrandOfSkill;
	}
}
// -------------------------------------------------------------------------------

//004c56d0	-> 100% (Identical)
BYTE CMagicDamage::IsItemAttachedSkill(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_byItemSkill[iSkill];
}
// -------------------------------------------------------------------------------

//004c5750	-> 100% (Identical)
int CMagicDamage::GetSkillUseType(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_iSkillUseType[iSkill];
}
// -------------------------------------------------------------------------------

//004c57d0	-> 100% (Identical)
int CMagicDamage::GetSkillLevel(int iSkill)
{
	if ( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_rLevel[iSkill];
}
// -------------------------------------------------------------------------------

//004c5850	-> 100% (Identical)
int CMagicDamage::GetSkillGroup(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_sSkillGroup[iSkill];
}
// -------------------------------------------------------------------------------

//004c58d0	-> 100% (Identical)
int CMagicDamage::GetSkillRank(int iSkill)
{
	if( iSkill < 0 
		|| iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	return this->m_sSkillRank[iSkill];
}
// -------------------------------------------------------------------------------