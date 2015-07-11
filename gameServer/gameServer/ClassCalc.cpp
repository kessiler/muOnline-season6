#include "StdAfx.h"
#include "ClassCalc.h"
#include "logproc.h"
#include "user.h"
#include "GameMain.h"


ClassCalc g_ClassCalc;

ClassCalc::ClassCalc() {}
ClassCalc::~ClassCalc() {}

void ClassCalc::Load()
{
	this->ReadData(gDirPath.GetNewPath("Custom\\ClassCalc.ini"));
}

void ClassCalc::ReadData(char * File)
{ 
	char * ClassType[] = { "CLASS_WIZARD", "CLASS_KNIGHT", "CLASS_ELF", 
		"CLASS_MAGUMSA", "CLASS_DARKLORD", "CLASS_SUMMONER", "CLASS_MONK" };
	char szFloat[256];
	// ----
	for( int Class = CLASS_WIZARD; Class < MAX_TYPE_PLAYER; Class++ )
	{
		this->m_Data[Class].LevelPoints					= GetPrivateProfileInt(ClassType[Class], "LevelPoints", 0, File);
		this->m_Data[Class].AttackDamageMinRightDiv1	= GetPrivateProfileInt(ClassType[Class], "AttackDamageMinRightDiv1", 0, File);
		this->m_Data[Class].AttackDamageMaxRightDiv1	= GetPrivateProfileInt(ClassType[Class], "AttackDamageMaxRightDiv1", 0, File);
		this->m_Data[Class].AttackDamageMinLeftDiv1		= GetPrivateProfileInt(ClassType[Class], "AttackDamageMinLeftDiv1", 0, File);
		this->m_Data[Class].AttackDamageMaxLeftDiv1		= GetPrivateProfileInt(ClassType[Class], "AttackDamageMaxLeftDiv1", 0, File);
		if( Class == CLASS_ELF || Class == CLASS_MAGUMSA || Class == CLASS_DARKLORD || Class == CLASS_MONK )
		{
			this->m_Data[Class].AttackDamageMinRightDiv2	= GetPrivateProfileInt(ClassType[Class], "AttackDamageMinRightDiv2", 0, File);
			this->m_Data[Class].AttackDamageMaxRightDiv2	= GetPrivateProfileInt(ClassType[Class], "AttackDamageMaxRightDiv2", 0, File);
			this->m_Data[Class].AttackDamageMinLeftDiv2		= GetPrivateProfileInt(ClassType[Class], "AttackDamageMinLeftDiv2", 0, File);
			this->m_Data[Class].AttackDamageMaxLeftDiv2		= GetPrivateProfileInt(ClassType[Class], "AttackDamageMaxLeftDiv2", 0, File);
		}
		if( Class == CLASS_ELF )
		{
			this->m_Data[Class].AttackDamageMinRightDiv3	= GetPrivateProfileInt(ClassType[Class], "AttackDamageMinRightDiv3", 0, File);
			this->m_Data[Class].AttackDamageMaxRightDiv3	= GetPrivateProfileInt(ClassType[Class], "AttackDamageMaxRightDiv3", 0, File);
			this->m_Data[Class].AttackDamageMinLeftDiv3		= GetPrivateProfileInt(ClassType[Class], "AttackDamageMinLeftDiv3", 0, File);
			this->m_Data[Class].AttackDamageMaxLeftDiv3		= GetPrivateProfileInt(ClassType[Class], "AttackDamageMaxLeftDiv3", 0, File);
		}
		if( Class == CLASS_WIZARD || Class == CLASS_MAGUMSA || Class == CLASS_SUMMONER )
		{
			this->m_Data[Class].MagicDamageMinDiv	= GetPrivateProfileInt(ClassType[Class], "MagicDamageMinDiv", 0, File);
			this->m_Data[Class].MagicDamageMaxDiv	= GetPrivateProfileInt(ClassType[Class], "MagicDamageMaxDiv", 0, File);
			if( Class == CLASS_SUMMONER )
			{
				this->m_Data[Class].CurseDamageMinDiv	= GetPrivateProfileInt(ClassType[Class], "CurseDamageMinDiv", 0, File);
				this->m_Data[Class].CurseDamageMaxDiv	= GetPrivateProfileInt(ClassType[Class], "CurseDamageMaxDiv", 0, File);
			}
		}
		this->m_Data[Class].AttackSpeedDiv			= GetPrivateProfileInt(ClassType[Class], "AttackSpeedDiv", 0, File);
		this->m_Data[Class].MagicSpeedDiv			= GetPrivateProfileInt(ClassType[Class], "MagicSpeedDiv", 0, File);
		this->m_Data[Class].DefenseDiv				= GetPrivateProfileInt(ClassType[Class], "DefenseDiv", 0, File);
		if( Class == CLASS_MONK )
		{
			GetPrivateProfileString(ClassType[Class], "PvPAttackRateFact1", "0", szFloat, 5, File);
			sscanf(szFloat, "%f", &this->m_Data[Class].PvPAttackRateFact1RF);
		}
		else
		{
			this->m_Data[Class].PvPAttackRateFact1	= GetPrivateProfileInt(ClassType[Class], "PvPAttackRateFact1", 0, File);
		}
		this->m_Data[Class].PvPAttackRateFact2		= GetPrivateProfileInt(ClassType[Class], "PvPAttackRateFact2", 0, File);
		if( Class != CLASS_DARKLORD && Class != CLASS_WIZARD )
		{
			this->m_Data[Class].PvPAttackRateDiv	= GetPrivateProfileInt(ClassType[Class], "PvPAttackRateDiv", 0, File);
		}
		if( Class == CLASS_MONK )
		{
			GetPrivateProfileString(ClassType[Class], "PvPDefenseRateFact", "0", szFloat, 5, File);
			sscanf(szFloat, "%f", &this->m_Data[Class].PvPDefenseRateFactRF);
		}
		else
		{
			this->m_Data[Class].PvPDefenseRateFact		= GetPrivateProfileInt(ClassType[Class], "PvPDefenseRateFact", 0, File);
		}
		this->m_Data[Class].PvPDefenseRateDiv		= GetPrivateProfileInt(ClassType[Class], "PvPDefenseRateDiv", 0, File);
		this->m_Data[Class].SuccessBlockDiv			= GetPrivateProfileInt(ClassType[Class], "SuccessBlockDiv", 0, File);
		if( Class == CLASS_DARKLORD )
		{
			this->m_Data[Class].PetAttackDamageMinFact		= GetPrivateProfileInt(ClassType[Class], "PetAttackDamageMinFact", 0, File);
			this->m_Data[Class].PetAttackDamageMinDiv		= GetPrivateProfileInt(ClassType[Class], "PetAttackDamageMinDiv", 0, File);
			this->m_Data[Class].PetAttackDamageMinStart		= GetPrivateProfileInt(ClassType[Class], "PetAttackDamageMinStart", 0, File);
			this->m_Data[Class].PetAttackDamageMaxFact		= GetPrivateProfileInt(ClassType[Class], "PetAttackDamageMaxFact", 0, File);
			this->m_Data[Class].PetAttackDamageMaxDiv		= GetPrivateProfileInt(ClassType[Class], "PetAttackDamageMaxDiv", 0, File);
			this->m_Data[Class].PetAttackDamageMaxStart		= GetPrivateProfileInt(ClassType[Class], "PetAttackDamageMaxStart", 0, File);
			this->m_Data[Class].PetCriticalAttackRate		= GetPrivateProfileInt(ClassType[Class], "PetCriticalAttackRate", 0, File);
			this->m_Data[Class].PetAttackSpeedFact			= GetPrivateProfileInt(ClassType[Class], "PetAttackSpeedFact", 0, File);
			this->m_Data[Class].PetAttackSpeedDiv1			= GetPrivateProfileInt(ClassType[Class], "PetAttackSpeedDiv1", 0, File);
			this->m_Data[Class].PetAttackSpeedDiv2			= GetPrivateProfileInt(ClassType[Class], "PetAttackSpeedDiv2", 0, File);
			this->m_Data[Class].PetAttackSpeedStart			= GetPrivateProfileInt(ClassType[Class], "PetAttackSpeedStart", 0, File);
			this->m_Data[Class].PetSuccessAttackRateDiv		= GetPrivateProfileInt(ClassType[Class], "PetSuccessAttackRateDiv", 0, File);
			this->m_Data[Class].PetSuccessAttackRateStart	= GetPrivateProfileInt(ClassType[Class], "PetSuccessAttackRateStart", 0, File);
		}
	}
}