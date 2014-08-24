//-> Decompiled by DarkSim | 09.06.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "MagicDamage.h"
#include "SkillAdditionInfo.h"
#include "LogProc.h"
#include "Gamemain.h"
#include "..\include\ReadScript.h"
// -------------------------------------------------------------------------------

CSkillAdditionInfo	g_SkillAdditionInfo;
// -------------------------------------------------------------------------------

//004c6850	-> 100%
CSkillAdditionInfo::CSkillAdditionInfo()
{
	this->Init();
}
// -------------------------------------------------------------------------------

//004c68d0	-> 100%
CSkillAdditionInfo::~CSkillAdditionInfo()
{
	return;
}
// -------------------------------------------------------------------------------

//004c6900	-> 100%
void CSkillAdditionInfo::Init()
{
	this->m_iInfinityArrowSkillTime				= 0;
	this->m_iInfinityArrowUseLevel				= 0;
	this->m_iInfinityArrowMPConsumptionPlus0	= 0;
	this->m_iInfinityArrowMPConsumptionPlus1	= 0;
	this->m_iInfinityArrowMPConsumptionPlus2	= 0;
	this->m_iInfinityArrowMPConsumptionPlus3	= 0;
	// ----
	for( int i = 0; i <= 5; i++ )	//wz mistake
	{
		this->m_nSoulBarrierManaRate_Level[i] = 0;
	}
	// ----
#ifdef __CUSTOMS__
	ZeroMemory(this->m_SkillUseReqLevel, sizeof(m_SkillUseReqLevel));
#endif
}
// -------------------------------------------------------------------------------

//004c69a0	-> 100%
int CSkillAdditionInfo::Load(LPSTR lpszFileName)
{
	if( lpszFileName == NULL || strcmp(lpszFileName, "") == 0 )
	{
		LogAddTD("[Skill Addition Info] - File load error : File Name Error");
		return false;
	}
	// ----
#ifdef __CUSTOMS__
	int Token;
	this->Init();
	// ----
	SMDFile = fopen(gDirPath.GetNewPath(lpszFileName), "r");
	// ----
	if( SMDFile == 0 )
	{
		MsgBox(lMsg.Get(MSGGET(0, 112)), lpszFileName);
		return false;
	}
	// ----
	while(true) 
	{
		Token = GetToken();
		// ----
		if( Token == END || !strcmp(TokenString, "end") )
		{
			break;
		}
		else
		{
			int SkillID = TokenNumber;
			Token = GetToken();
			int Level	= TokenNumber;
			// ----
			if( Level < 1 )
			{
				Level = 1;
			}
			// ----
			this->m_SkillUseReqLevel[SkillID] = Level;
		}
	}
	// ----
	fclose(SMDFile);
	// ----
	char szFloat[256];
	// ----
	this->m_SoulBarrierDefenseStart				= GetPrivateProfileInt("SkillInfo", "SoulBarrierDefenseStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SoulBarrierDefenseDiv1				= GetPrivateProfileInt("SkillInfo", "SoulBarrierDefenseDiv1", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SoulBarrierDefenseDiv2				= GetPrivateProfileInt("SkillInfo", "SoulBarrierDefenseDiv2", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SoulBarrierDefenseMax				= GetPrivateProfileInt("SkillInfo", "SoulBarrierDefenseMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SoulBarrierTimeStart				= GetPrivateProfileInt("SkillInfo", "SoulBarrierTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SoulBarrierTimeDiv					= GetPrivateProfileInt("SkillInfo", "SoulBarrierTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SoulBarrierTimeMax					= GetPrivateProfileInt("SkillInfo", "SoulBarrierTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_WizardryEnchantDamageDiv			= GetPrivateProfileInt("SkillInfo", "WizardryEnchantDamageDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	GetPrivateProfileString("SkillInfo", "WizardryEnchantDamageFact", "10", szFloat, 5, gDirPath.GetNewPath(lpszFileName));
	sscanf(szFloat, "%f", &this->m_WizardryEnchantDamageFact);
	this->m_WizardryEnchantDamageMax			= GetPrivateProfileInt("SkillInfo", "WizardryEnchantDamageMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_WizardryEnchantTime					= GetPrivateProfileInt("SkillInfo", "WizardryEnchantTime", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SwellLifeEffectStart				= GetPrivateProfileInt("SkillInfo", "SwellLifeEffectStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SwellLifeEffectDiv1					= GetPrivateProfileInt("SkillInfo", "SwellLifeEffectDiv1", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SwellLifeEffectDiv2					= GetPrivateProfileInt("SkillInfo", "SwellLifeEffectDiv2", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SwellLifeEffectMax					= GetPrivateProfileInt("SkillInfo", "SwellLifeEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SwellLifeTimeStart					= GetPrivateProfileInt("SkillInfo", "SwellLifeTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SwellLifeTimeDiv					= GetPrivateProfileInt("SkillInfo", "SwellLifeTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_SwellLifeTimeMax					= GetPrivateProfileInt("SkillInfo", "SwellLifeTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfDefenseEffectFact				= GetPrivateProfileInt("SkillInfo", "ElfDefenseEffectFact", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfDefenseEffectDiv					= GetPrivateProfileInt("SkillInfo", "ElfDefenseEffectDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfDefenseEffectMax					= GetPrivateProfileInt("SkillInfo", "ElfDefenseEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfDefenseTime						= GetPrivateProfileInt("SkillInfo", "ElfDefenseTime", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfAttackEffectFact					= GetPrivateProfileInt("SkillInfo", "ElfAttackEffectFact", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfAttackEffectDiv					= GetPrivateProfileInt("SkillInfo", "ElfAttackEffectDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfAttackEffectMax					= GetPrivateProfileInt("SkillInfo", "ElfAttackEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ElfAttackTime						= GetPrivateProfileInt("SkillInfo", "ElfAttackTime", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_CriticalDamageEffectDiv1			= GetPrivateProfileInt("SkillInfo", "CriticalDamageEffectDiv1", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_CriticalDamageEffectDiv2			= GetPrivateProfileInt("SkillInfo", "CriticalDamageEffectDiv2", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_CriticalDamageEffectMax				= GetPrivateProfileInt("SkillInfo", "CriticalDamageEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_CriticalDamageTimeStart				= GetPrivateProfileInt("SkillInfo", "CriticalDamageTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_CriticalDamageTimeDiv				= GetPrivateProfileInt("SkillInfo", "CriticalDamageTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_CriticalDamageTimeMax				= GetPrivateProfileInt("SkillInfo", "CriticalDamageTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_BerserkerManaIncDiv					= GetPrivateProfileInt("SkillInfo", "BerserkerManaIncDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_BerserkerLifeDecDiv					= GetPrivateProfileInt("SkillInfo", "BerserkerLifeDecDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_BerserkerTimeStart					= GetPrivateProfileInt("SkillInfo", "BerserkerTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_BerserkerTimeDiv					= GetPrivateProfileInt("SkillInfo", "BerserkerTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_BerserkerTimeMax					= GetPrivateProfileInt("SkillInfo", "BerserkerTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ReflectEffectStart					= GetPrivateProfileInt("SkillInfo", "ReflectEffectStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ReflectEffectDiv					= GetPrivateProfileInt("SkillInfo", "ReflectEffectDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ReflectEffectMax					= GetPrivateProfileInt("SkillInfo", "ReflectEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ReflectTimeStart					= GetPrivateProfileInt("SkillInfo", "ReflectTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ReflectTimeDiv						= GetPrivateProfileInt("SkillInfo", "ReflectTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_ReflectTimeMax						= GetPrivateProfileInt("SkillInfo", "ReflectTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IgnoreDefenseEffectStart			= GetPrivateProfileInt("SkillInfo", "IgnoreDefenseEffectStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IgnoreDefenseEffectSub				= GetPrivateProfileInt("SkillInfo", "IgnoreDefenseEffectSub", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IgnoreDefenseEffectDiv				= GetPrivateProfileInt("SkillInfo", "IgnoreDefenseEffectDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IgnoreDefenseEffectMax				= GetPrivateProfileInt("SkillInfo", "IgnoreDefenseEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IgnoreDefenseTimeStart				= GetPrivateProfileInt("SkillInfo", "IgnoreDefenseTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IgnoreDefenseTimeDiv				= GetPrivateProfileInt("SkillInfo", "IgnoreDefenseTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IgnoreDefenseTimeMax				= GetPrivateProfileInt("SkillInfo", "IgnoreDefenseTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseHealthEffectStart			= GetPrivateProfileInt("SkillInfo", "IncreaseHealthEffectStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseHealthEffectSub				= GetPrivateProfileInt("SkillInfo", "IncreaseHealthEffectSub", 600, gDirPath.GetNewPath(lpszFileName)); 

	//this->m_IncreaseHealthEffectDiv				= GetPrivateProfileInt("SkillInfo", "IncreaseHealthEffectDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	char szTempEx[256] = { 0 };
	GetPrivateProfileString("SkillInfo", "DecTimePerAttackSpeed", "10.00", szTempEx, 256, gDirPath.GetNewPath(lpszFileName));
	this->m_IncreaseHealthEffectDiv = atof(szTempEx);

	this->m_IncreaseHealthEffectMax				= GetPrivateProfileInt("SkillInfo", "IncreaseHealthEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseHealthTimeStart				= GetPrivateProfileInt("SkillInfo", "IncreaseHealthTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseHealthTimeDiv				= GetPrivateProfileInt("SkillInfo", "IncreaseHealthTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseHealthTimeMax				= GetPrivateProfileInt("SkillInfo", "IncreaseHealthTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseBlockEffectStart			= GetPrivateProfileInt("SkillInfo", "IncreaseBlockEffectStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseBlockEffectSub				= GetPrivateProfileInt("SkillInfo", "IncreaseBlockEffectSub", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseBlockEffectDiv				= GetPrivateProfileInt("SkillInfo", "IncreaseBlockEffectDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseBlockEffectMax				= GetPrivateProfileInt("SkillInfo", "IncreaseBlockEffectMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseBlockTimeStart				= GetPrivateProfileInt("SkillInfo", "IncreaseBlockTimeStart", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseBlockTimeDiv				= GetPrivateProfileInt("SkillInfo", "IncreaseBlockTimeDiv", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_IncreaseBlockTimeMax				= GetPrivateProfileInt("SkillInfo", "IncreaseBlockTimeMax", 600, gDirPath.GetNewPath(lpszFileName)); 
	// ----
	this->m_WeaknessEffectStartVsMob			= GetPrivateProfileInt("SkillInfo", "WeaknessEffectStartVsMob", 4, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessEffectDivVsMob				= GetPrivateProfileInt("SkillInfo", "WeaknessEffectDivVsMob", 58, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessEffectMaxVsMob				= GetPrivateProfileInt("SkillInfo", "WeaknessEffectMaxVsMob", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_WeaknessTimeStartVsMob				= GetPrivateProfileInt("SkillInfo", "WeaknessTimeStartVsMob", 4, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessTimeDivVsMob				= GetPrivateProfileInt("SkillInfo", "WeaknessTimeDivVsMob", 100, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessTimeMaxVsMob				= GetPrivateProfileInt("SkillInfo", "WeaknessTimeMaxVsMob", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_WeaknessEffectStartVsUser			= GetPrivateProfileInt("SkillInfo", "WeaknessEffectStartVsUser", 3, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessEffectDivVsUser				= GetPrivateProfileInt("SkillInfo", "WeaknessEffectDivVsUser", 93, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessEffectMaxVsUser				= GetPrivateProfileInt("SkillInfo", "WeaknessEffectMaxVsUser", 72, gDirPath.GetNewPath(lpszFileName));

	this->m_WeaknessTimeStartVsUser				= GetPrivateProfileInt("SkillInfo", "WeaknessTimeStartVsUser", 5, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessTimeDivVsUser				= GetPrivateProfileInt("SkillInfo", "WeaknessTimeDivVsUser", 300, gDirPath.GetNewPath(lpszFileName));
	this->m_WeaknessTimeMaxVsUser				= GetPrivateProfileInt("SkillInfo", "WeaknessTimeMaxVsUser", 0, gDirPath.GetNewPath(lpszFileName));
	// ----
	this->m_InnovationEffectStartVsMob			= GetPrivateProfileInt("SkillInfo", "InnovationEffectStartVsMob", 20, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationEffectDivVsMob			= GetPrivateProfileInt("SkillInfo", "InnovationEffectDivVsMob", 90, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationEffectMaxVsMob			= GetPrivateProfileInt("SkillInfo", "InnovationEffectMaxVsMob", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_InnovationTimeStartVsMob			= GetPrivateProfileInt("SkillInfo", "InnovationTimeStartVsMob", 4, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationTimeDivVsMob				= GetPrivateProfileInt("SkillInfo", "InnovationTimeDivVsMob", 100, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationTimeMaxVsMob				= GetPrivateProfileInt("SkillInfo", "InnovationTimeMaxVsMob", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_InnovationEffectStartVsUser			= GetPrivateProfileInt("SkillInfo", "InnovationEffectStartVsUser", 12, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationEffectDivVsUser			= GetPrivateProfileInt("SkillInfo", "InnovationEffectDivVsUser", 110, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationEffectMaxVsUser			= GetPrivateProfileInt("SkillInfo", "InnovationEffectMaxVsUser", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_InnovationTimeStartVsUser			= GetPrivateProfileInt("SkillInfo", "InnovationTimeStartVsUser", 5, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationTimeDivVsUser				= GetPrivateProfileInt("SkillInfo", "InnovationTimeDivVsUser", 300, gDirPath.GetNewPath(lpszFileName));
	this->m_InnovationTimeMaxVsUser				= GetPrivateProfileInt("SkillInfo", "InnovationTimeMaxVsUser", 0, gDirPath.GetNewPath(lpszFileName));	
	// ----

	this->m_SleepRateStartVsMob					= GetPrivateProfileInt("SkillInfo", "SleepRateStartVsMob", 20, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepRateDivVsMob					= GetPrivateProfileInt("SkillInfo", "SleepRateDivVsMob", 30, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepRateMaxVsMob					= GetPrivateProfileInt("SkillInfo", "SleepRateMaxVsMob", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_SleepRateStartVsUser				= GetPrivateProfileInt("SkillInfo", "SleepRateStartVsUser", 15, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepRateDivVsUser					= GetPrivateProfileInt("SkillInfo", "SleepRateDivVsUser", 37, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepRateMaxVsUser					= GetPrivateProfileInt("SkillInfo", "SleepRateMaxVsUser", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_SleepTimeStartVsMob					= GetPrivateProfileInt("SkillInfo", "SleepTimeStartVsMob", 5, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepTimeDivVsMob					= GetPrivateProfileInt("SkillInfo", "SleepTimeDivVsMob", 100, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepTimeMaxVsMob					= GetPrivateProfileInt("SkillInfo", "SleepTimeMaxVsMob", 0, gDirPath.GetNewPath(lpszFileName));

	this->m_SleepTimeStartVsUser				= GetPrivateProfileInt("SkillInfo", "SleepTimeStartVsUser", 4, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepTimeDivVsUser					= GetPrivateProfileInt("SkillInfo", "SleepTimeDivVsUser", 250, gDirPath.GetNewPath(lpszFileName));
	this->m_SleepTimeMaxVsUser					= GetPrivateProfileInt("SkillInfo", "SleepTimeMaxVsUser", 0, gDirPath.GetNewPath(lpszFileName));
#endif
	// ----
	this->m_iInfinityArrowSkillTime				= GetPrivateProfileInt("SkillInfo", "InfinityArrowSkillTime", 600, gDirPath.GetNewPath(lpszFileName)); 
	this->m_iInfinityArrowUseLevel				= GetPrivateProfileInt("SkillInfo", "InfinityArrowUseLevel", 220, gDirPath.GetNewPath(lpszFileName)); 
	this->m_iInfinityArrowMPConsumptionPlus0	= GetPrivateProfileInt("SkillInfo", "InfinityArraowMPConsumptionPlus0", 0, gDirPath.GetNewPath(lpszFileName));
	this->m_iInfinityArrowMPConsumptionPlus1	= GetPrivateProfileInt("SkillInfo", "InfinityArraowMPConsumptionPlus1", 0, gDirPath.GetNewPath(lpszFileName));
	this->m_iInfinityArrowMPConsumptionPlus2	= GetPrivateProfileInt("SkillInfo", "InfinityArraowMPConsumptionPlus2", 0, gDirPath.GetNewPath(lpszFileName));
	this->m_iInfinityArrowMPConsumptionPlus3	= GetPrivateProfileInt("SkillInfo", "InfinityArraowMPConsumptionPlus3", 0, gDirPath.GetNewPath(lpszFileName));
	LogAddTD("[Skill Addition Info] [Infinity Arrow] Skill Time[%d], UseLevel[%d], Arrow MP Consumption +0[%d] +1[%d] +2[%d] +3[%d]", this->m_iInfinityArrowSkillTime, this->m_iInfinityArrowUseLevel, this->m_iInfinityArrowMPConsumptionPlus0, this->m_iInfinityArrowMPConsumptionPlus1, this->m_iInfinityArrowMPConsumptionPlus2, this->m_iInfinityArrowMPConsumptionPlus3);
	// ----
	this->m_bFireScreamSkill					= GetPrivateProfileInt("SkillInfo", "FireScreamSkill", 1, gDirPath.GetNewPath(lpszFileName));
	this->m_iFireScreamExplosionAttackDistance	= GetPrivateProfileInt("SkillInfo", "FireScreamExplosionAttackDistance", 1, gDirPath.GetNewPath(lpszFileName));
	this->m_iFireScreamExplosionRate			= GetPrivateProfileInt("SkillInfo", "FireScreamExplosionRate", 300, gDirPath.GetNewPath(lpszFileName));
	this->m_iFireScreamMaxAttackCountSameSerial = GetPrivateProfileInt("SkillInfo", "FireScreamMaxAttackCountSameSerial", 3, gDirPath.GetNewPath(lpszFileName));
	this->m_iFireScreamSerialInitCount			= GetPrivateProfileInt("SkillInfo", "FireScreamSerialInitCount", 100, gDirPath.GetNewPath(lpszFileName));
	this->m_bCheckFireScreamSerial				= GetPrivateProfileInt("SkillInfo", "CheckFireScreamSerial", 1, gDirPath.GetNewPath(lpszFileName));
	LogAddTD("[Skill Addition Info] [Fire Scream] Use(%d) : Explosion Distance(%d) - Rate(%d), MaxAttack_SameSerial(%d), SerialInitCount(%d), Check Serial(%d)", this->m_bFireScreamSkill, this->m_iFireScreamExplosionAttackDistance, this->m_iFireScreamExplosionRate, this->m_iFireScreamMaxAttackCountSameSerial, this->m_iFireScreamSerialInitCount, this->m_bCheckFireScreamSerial);
	// ----
	char szTemp[63]		= { 0 };
	BYTE byDefault		= 0;
	int n				= 1;
	// ----
	for( n = 1; n < 21; n++ )
	{
		wsprintf(szTemp, "SoulBarrierManaRate_Level%d", n);
		byDefault = 20 + n;
		this->m_nSoulBarrierManaRate_Level[n] = GetPrivateProfileInt("SkillInfo", szTemp, byDefault, gDirPath.GetNewPath(lpszFileName));
	}
	// ----
	char szTempLog[511]	= { 0 };
	wsprintf(szTempLog, "[Skill Addition Info] [SoulBarrier Strengrhen] BaseSkill (Level0):%d", this->m_nSoulBarrierManaRate_Level[0]);
	// ----
	for( n = 1; n < 21; n++ )
	{
		wsprintf(szTemp, ", Level%d : %d", n, this->m_nSoulBarrierManaRate_Level[n]);
		strcat(szTempLog, szTemp);
	}
	LogAddTD(szTempLog);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004c6eb0	-> 100%
short CSkillAdditionInfo::GetSoulBarrierManaRate_Level(int iLevel)
{
	if( iLevel < 0 || iLevel > 20 )	//increased (from 5 to 20) by 1.1.0.0
	{
		return 20;
	}
	// ----
	return this->m_nSoulBarrierManaRate_Level[iLevel];
}
// -------------------------------------------------------------------------------