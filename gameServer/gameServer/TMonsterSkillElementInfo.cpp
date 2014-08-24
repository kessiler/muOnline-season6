// TMonsterSkillElementInfo.cpp: implementation of the TMonsterSkillElementInfo class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TMonsterSkillElementInfo.h"
#include "protocol.h"
#include "user.h"

//////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////

void TMonsterSkillElementInfo::SetSkillElement(int iSkillElement)
{
	//
}

void TMonsterSkillElementInfo::CheckSkillElementInfoProc(LPOBJ lpObj)
{
	if ( lpObj->m_MonsterSkillElementInfo.CheckAttackTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAttackTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAttack();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckDefenseTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecDefenseTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetDefense();
		}
	}
	
	if ( lpObj->m_MonsterSkillElementInfo.CheckAutoAGTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAutoAGTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoAG();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckAutoHPTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAutoHPTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoHP();
		}
		else 
		{
			if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle == 110 )
				lpObj->Life += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP;
			else
	
			lpObj->Life += (((float)(lpObj->Life) * (float)(lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP)))/100.0f;

			if ( lpObj->Life <0 )
			{
				lpObj->Life = 0;
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			}
			else if ( (lpObj->MaxLife + lpObj->AddLife) < lpObj->Life )
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			}
			else
			{
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			}
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckAutoMPTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAutoMPTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoMP();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckAttackTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecAttackTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAttack();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckResistTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecResistTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetResist();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckModifyStatTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecModifyStatTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetModifyStat();
		}
	}

	if ( lpObj->m_MonsterSkillElementInfo.CheckBerserkTime() )
	{
		if ( lpObj->m_MonsterSkillElementInfo.DecBerserkTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetBerserkTime();
		}
	}
}

//005E19C0 
void TMonsterSkillElementInfo::Reset()
{
	this->ResetDefense();
	this->ResetAttack();
	this->ResetAutoHP();
	this->ResetAutoMP();
	this->ResetAutoAG();
	this->ResetImmune();
	this->ResetResist();
	this->ResetModifyStat();
	this->ResetBerserkTime();
}

void  TMonsterSkillElementInfo::ResetDefense()
{
	this->m_iSkillElementDefense = 0;
	this->m_iSkillElementDefenseTime = 0;
}

void  TMonsterSkillElementInfo::ResetAttack()
{
	this->m_iSkillElementAttack = 0;
	this->m_iSkillElementAttackTime = 0;
}

void  TMonsterSkillElementInfo::ResetAutoHP()
{
	this->m_iSkillElementAutoHP = 0;
	this->m_iSkillElementAutoHPCycle = 0;
	this->m_iSkillElementAutoHPTime = 0;
}

void  TMonsterSkillElementInfo::ResetAutoMP()
{
	this->m_iSkillElementAutoMP = 0;
	this->m_iSkillElementAutoMPCycle = 0;
	this->m_iSkillElementAutoMPTime = 0;
}

void  TMonsterSkillElementInfo::ResetAutoAG()
{
	this->m_iSkillElementAutoAG = 0;
	this->m_iSkillElementAutoAGCycle = 0;
	this->m_iSkillElementAutoAGTime = 0;
}

void  TMonsterSkillElementInfo::ResetImmune()
{
	this->m_iSkillElementImmuneNumber = 0;
	this->m_iSkillElementImmuneTime = 0;
}

void  TMonsterSkillElementInfo::ResetResist()
{
	this->m_iSkillElementResistNumber = 0;
	this->m_iSkillElementResistTime = 0;
}

void  TMonsterSkillElementInfo::ResetModifyStat()
{
	this->m_iSkillElementModifyStat = 0;
	this->m_iSkillElementModifyStatType = 0;
	this->m_iSkillElementModifyStatTime = 0;
}

//005E1C00  /> \55            PUSH EBP
void  TMonsterSkillElementInfo::ResetBerserkTime()
{
	this->m_iSkillElementBerserkTime = 0;
}
/////////////////////////////////////////////////////////

BOOL  TMonsterSkillElementInfo::CheckDefenseTime()
{
	return (this->m_iSkillElementDefense <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAttackTime()
{
	return (this->m_iSkillElementAttack <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAutoHPTime()
{
	return (this->m_iSkillElementAutoHPTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAutoMPTime()
{
	return (this->m_iSkillElementAutoMPTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckAutoAGTime()
{
	return (this->m_iSkillElementAutoAGTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckResistTime()
{
	return (this->m_iSkillElementResistTime <= 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::CheckModifyStatTime()
{
	return (this->m_iSkillElementModifyStatTime <= 0)?FALSE:TRUE;
}


//005E1D80  /> \55            PUSH EBP
BOOL  TMonsterSkillElementInfo::CheckBerserkTime()
{
	return (this->m_iSkillElementBerserkTime <= 0)?FALSE:TRUE;
}

////////////////////////////////////////////////////////////////
BOOL  TMonsterSkillElementInfo::DecDefenseTime()
{
	this->m_iSkillElementDefense--;
	return (this->m_iSkillElementDefense > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAttackTime()
{
	this->m_iSkillElementAttack--;
	return (this->m_iSkillElementAttack > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAutoHPTime()
{
	this->m_iSkillElementAutoHPTime--;
	return (this->m_iSkillElementAutoHPTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAutoMPTime()
{
	this->m_iSkillElementAutoMPTime--;
	return (this->m_iSkillElementAutoMPTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecAutoAGTime()
{
	this->m_iSkillElementAutoAGTime--;
	return (this->m_iSkillElementAutoAGTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecResistTime()
{
	this->m_iSkillElementResistTime--;
	return (this->m_iSkillElementResistTime > 0)?FALSE:TRUE;
}

BOOL  TMonsterSkillElementInfo::DecModifyStatTime()
{
	this->m_iSkillElementModifyStatTime--;
	return (this->m_iSkillElementModifyStatTime > 0)?FALSE:TRUE;
}

//005E1F70  /> \55            PUSH EBP
BOOL  TMonsterSkillElementInfo::DecBerserkTime()
{
	this->m_iSkillElementBerserkTime--;
	return (this->m_iSkillElementBerserkTime > 0)?FALSE:TRUE;
}