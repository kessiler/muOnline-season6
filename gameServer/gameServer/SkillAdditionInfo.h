//-> Decompiled by DarkSim | 09.06.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

class CSkillAdditionInfo
{
public:
			CSkillAdditionInfo();
	virtual	~CSkillAdditionInfo();
	// ----
	void	Init();
	int		Load(LPSTR lpszFileName);
	short	GetSoulBarrierManaRate_Level(int iLevel);
	// ----
	void	SetInfinityArrowSkillTime(int iValue){this->m_iInfinityArrowSkillTime = iValue;}
	int		GetInfinityArrowSkillTime(){return this->m_iInfinityArrowSkillTime;}
	void	SetInfinityArrowUseLevel(int iValue){this->m_iInfinityArrowUseLevel = iValue;}
	int		GetInfinityArrowUseLevel(){return this->m_iInfinityArrowUseLevel;}
	void	SetInfinityArrowMPConsumptionPlus0(int iValue){this->m_iInfinityArrowMPConsumptionPlus0 = iValue;}
	int		GetInfinityArrowMPConsumptionPlus0(){return this->m_iInfinityArrowMPConsumptionPlus0;}
	void	SetInfinityArrowMPConsumptionPlus1(int iValue){this->m_iInfinityArrowMPConsumptionPlus1 = iValue;}
	int		GetInfinityArrowMPConsumptionPlus1(){return this->m_iInfinityArrowMPConsumptionPlus1;}
	void	SetInfinityArrowMPConsumptionPlus2(int iValue){this->m_iInfinityArrowMPConsumptionPlus2 = iValue;}
	int		GetInfinityArrowMPConsumptionPlus2(){return this->m_iInfinityArrowMPConsumptionPlus2;}
	void	SetInfinityArrowMPConsumptionPlus3(int iValue){this->m_iInfinityArrowMPConsumptionPlus3 = iValue;}
	int		GetInfinityArrowMPConsumptionPlus3(){return this->m_iInfinityArrowMPConsumptionPlus3;}
	void	SetFireScreamSkill(int iValue){this->m_bFireScreamSkill = iValue;}
	BOOL	GetFireScreamSkill(){return this->m_bFireScreamSkill;}
	void	SetFireScreamExplosionAttackDistance(int iValue){this->m_iFireScreamExplosionAttackDistance = iValue;}
	int		GetFireScreamExplosionAttackDistance(){return this->m_iFireScreamExplosionAttackDistance;}
	void	SetFireScreamExplosionRate(int iValue){this->m_iFireScreamExplosionRate = iValue;}
	int		GetFireScreamExplosionRate(){return this->m_iFireScreamExplosionRate;}
	void	SetFireScreamMaxAttackCountSameSerial(int iValue){this->m_iFireScreamMaxAttackCountSameSerial = iValue;}
	int		GetFireScreamMaxAttackCountSameSerial(){return this->m_iFireScreamMaxAttackCountSameSerial;}
	void	SetFireScreamSerialInitCount(int iValue){this->m_iFireScreamSerialInitCount = iValue;}
	int		GetFireScreamSerialInitCount(){return this->m_iFireScreamSerialInitCount;}
	void	SetCheckFireScreamSkill(int iValue){this->m_bCheckFireScreamSerial = iValue;}
	BOOL	GetCheckFireScreamSkill(){return this->m_bCheckFireScreamSerial;}
	// ----
private:
	int		m_iInfinityArrowSkillTime;
	int		m_iInfinityArrowUseLevel;
	int		m_iInfinityArrowMPConsumptionPlus0;
	int		m_iInfinityArrowMPConsumptionPlus1;
	int		m_iInfinityArrowMPConsumptionPlus2;
	int		m_iInfinityArrowMPConsumptionPlus3;
	BOOL	m_bFireScreamSkill;
	int		m_iFireScreamExplosionAttackDistance;
	int		m_iFireScreamExplosionRate;
	int		m_iFireScreamMaxAttackCountSameSerial;
	int		m_iFireScreamSerialInitCount;
	BOOL	m_bCheckFireScreamSerial;
	short	m_nSoulBarrierManaRate_Level[20];	//-> Updated by 1.01.00
#ifdef __CUSTOMS__
public:
	int		m_SkillUseReqLevel[MAX_SKILL];
	WORD	m_SoulBarrierDefenseStart;
	WORD	m_SoulBarrierDefenseDiv1;
	WORD	m_SoulBarrierDefenseDiv2;
	DWORD	m_SoulBarrierDefenseMax;
	WORD	m_SoulBarrierTimeStart;
	WORD	m_SoulBarrierTimeDiv;
	DWORD	m_SoulBarrierTimeMax;
	WORD	m_WizardryEnchantDamageDiv;
	float	m_WizardryEnchantDamageFact;
	DWORD	m_WizardryEnchantDamageMax;
	DWORD	m_WizardryEnchantTime;
	WORD	m_SwellLifeEffectStart;
	WORD	m_SwellLifeEffectDiv1;
	WORD	m_SwellLifeEffectDiv2;
	DWORD	m_SwellLifeEffectMax;
	WORD	m_SwellLifeTimeStart;
	WORD	m_SwellLifeTimeDiv;
	DWORD	m_SwellLifeTimeMax;
	WORD	m_ElfDefenseEffectFact;
	WORD	m_ElfDefenseEffectDiv;
	DWORD	m_ElfDefenseEffectMax;
	DWORD	m_ElfDefenseTime;
	WORD	m_ElfAttackEffectFact;
	WORD	m_ElfAttackEffectDiv;
	DWORD	m_ElfAttackEffectMax;
	DWORD	m_ElfAttackTime;
	WORD	m_CriticalDamageEffectDiv1;
	WORD	m_CriticalDamageEffectDiv2;
	WORD	m_CriticalDamageEffectMax;
	WORD	m_CriticalDamageTimeStart;
	WORD	m_CriticalDamageTimeDiv;
	DWORD	m_CriticalDamageTimeMax;
	WORD	m_BerserkerManaIncDiv;
	WORD	m_BerserkerLifeDecDiv;
	WORD	m_BerserkerTimeStart;
	WORD	m_BerserkerTimeDiv;
	DWORD	m_BerserkerTimeMax;
	WORD	m_ReflectEffectStart;
	WORD	m_ReflectEffectDiv;
	DWORD	m_ReflectEffectMax;
	WORD	m_ReflectTimeStart;
	WORD	m_ReflectTimeDiv;
	DWORD	m_ReflectTimeMax;
	WORD	m_IgnoreDefenseEffectStart;
	WORD	m_IgnoreDefenseEffectSub;
	WORD	m_IgnoreDefenseEffectDiv;
	DWORD	m_IgnoreDefenseEffectMax;
	WORD	m_IgnoreDefenseTimeStart;
	WORD	m_IgnoreDefenseTimeDiv;
	DWORD	m_IgnoreDefenseTimeMax;
	WORD	m_IncreaseHealthEffectStart;
	WORD	m_IncreaseHealthEffectSub;
	float	m_IncreaseHealthEffectDiv;
	DWORD	m_IncreaseHealthEffectMax;
	WORD	m_IncreaseHealthTimeStart;
	WORD	m_IncreaseHealthTimeDiv;
	DWORD	m_IncreaseHealthTimeMax;
	WORD	m_IncreaseBlockEffectStart;
	WORD	m_IncreaseBlockEffectSub;
	WORD	m_IncreaseBlockEffectDiv;
	DWORD	m_IncreaseBlockEffectMax;
	WORD	m_IncreaseBlockTimeStart;
	WORD	m_IncreaseBlockTimeDiv;
	DWORD	m_IncreaseBlockTimeMax;
	// ---
	WORD	m_WeaknessEffectStartVsMob;
	WORD	m_WeaknessEffectDivVsMob;
	WORD	m_WeaknessEffectMaxVsMob;

	WORD	m_WeaknessTimeStartVsMob;
	WORD	m_WeaknessTimeDivVsMob;
	WORD	m_WeaknessTimeMaxVsMob;

	WORD	m_WeaknessEffectStartVsUser;
	WORD	m_WeaknessEffectDivVsUser;
	WORD	m_WeaknessEffectMaxVsUser;

	WORD	m_WeaknessTimeStartVsUser;
	WORD	m_WeaknessTimeDivVsUser;
	WORD	m_WeaknessTimeMaxVsUser;
	//---
	WORD	m_InnovationEffectStartVsMob;
	WORD	m_InnovationEffectDivVsMob;
	WORD	m_InnovationEffectMaxVsMob;

	WORD	m_InnovationTimeStartVsMob;
	WORD	m_InnovationTimeDivVsMob;
	WORD	m_InnovationTimeMaxVsMob;

	WORD	m_InnovationEffectStartVsUser;
	WORD	m_InnovationEffectDivVsUser;
	WORD	m_InnovationEffectMaxVsUser;

	WORD	m_InnovationTimeStartVsUser;
	WORD	m_InnovationTimeDivVsUser;
	WORD	m_InnovationTimeMaxVsUser;
	//---
	WORD	m_SleepRateStartVsMob;
	WORD	m_SleepRateDivVsMob;
	WORD	m_SleepRateMaxVsMob;

	WORD	m_SleepTimeStartVsMob;
	WORD	m_SleepTimeDivVsMob;
	WORD	m_SleepTimeMaxVsMob;

	WORD	m_SleepRateStartVsUser;
	WORD	m_SleepRateDivVsUser;
	WORD	m_SleepRateMaxVsUser;

	WORD	m_SleepTimeStartVsUser;
	WORD	m_SleepTimeDivVsUser;
	WORD	m_SleepTimeMaxVsUser;

#endif
	// ----
}; extern CSkillAdditionInfo g_SkillAdditionInfo;
// -------------------------------------------------------------------------------