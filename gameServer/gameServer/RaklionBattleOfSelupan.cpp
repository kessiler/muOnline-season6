// RaklionBattleOfSelupan.cpp: implementation of the CRaklionBattleOfSelupan class.
// GS-N 1.00.90  - 0xXXXXXXXX finished
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionBattleOfSelupan.h"
#include "RaklionBattleUserMng.h"
#include "LogProc.h"
#include "RaklionUtil.h"
#include "User.h"
#include "GameMain.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionBattleOfSelupan::CRaklionBattleOfSelupan()
{
	this->ResetAllData();
}

CRaklionBattleOfSelupan::~CRaklionBattleOfSelupan()
{

}
//0060DCB0 - identical
void CRaklionBattleOfSelupan::ResetAllData()
{
	this->m_004 = 0;
	this->m_dwSkillDelay		= 0;
	this->m_dwSkillDelayTick	= 0;
	this->m_bIsUseFirstSkill = 0;

	memset(this->m_PatternSkill,0,0x1C);

	for(int i = 0 ; i< MAX_STATE_PATTERN_COUNT ; i++)
	{
		 this->m_PatternSkill[i].bIsSkillEnabled[SKILL_POISON_ATTACK]		= 1;
		 this->m_PatternSkill[i].bIsSkillEnabled[SKILL_ICESTORM]			= 1;
		 this->m_PatternSkill[i].bIsSkillEnabled[SKILL_ICESTRIKE]			= 1;
		 this->m_PatternSkill[i].bIsSkillEnabled[SKILL_TELEPORT]			= 1;
	}

	this->m_PatternSkill[STATE_PATTERN_2].bIsSkillEnabled[SKILL_FREEZE]			= 1;//D10
	this->m_PatternSkill[STATE_PATTERN_3].bIsSkillEnabled[SKILL_FREEZE]			= 1;//D34

	this->m_PatternSkill[STATE_PATTERN_4].bIsSkillEnabled[SKILL_FREEZE]			= 1;//D58
	this->m_PatternSkill[STATE_PATTERN_4].bIsSkillEnabled[SKILL_HEAL]			= 1;//D54

	this->m_PatternSkill[STATE_PATTERN_5].bIsSkillEnabled[SKILL_SUMMON]			= 1;//D74
	this->m_PatternSkill[STATE_PATTERN_5].bIsSkillEnabled[SKILL_FREEZE]			= 1;//D7C
	this->m_PatternSkill[STATE_PATTERN_5].bIsSkillEnabled[SKILL_HEAL]			= 1;//D78

	this->m_PatternSkill[STATE_PATTERN_6].bIsSkillEnabled[SKILL_SUMMON]			= 1;//D98
	this->m_PatternSkill[STATE_PATTERN_6].bIsSkillEnabled[SKILL_FREEZE]			= 1;//DA0
	this->m_PatternSkill[STATE_PATTERN_6].bIsSkillEnabled[SKILL_HEAL]			= 1;//D9C

	this->m_PatternSkill[STATE_PATTERN_7].bIsSkillEnabled[SKILL_SUMMON]			= 1;//DBC
	this->m_PatternSkill[STATE_PATTERN_7].bIsSkillEnabled[SKILL_FREEZE]			= 1;//DC4
	this->m_PatternSkill[STATE_PATTERN_7].bIsSkillEnabled[SKILL_HEAL]			= 1;//DC0
	this->m_PatternSkill[STATE_PATTERN_7].bIsSkillEnabled[SKILL_INVINCIBILITY]	= 1;//DCC
	
	this->SetSuccessValue(FALSE);
	this->SetBattleOfSelupanState(SELUPAN_STATE_NONE);
}

int CRaklionBattleOfSelupan::Init() //00643540 
{
	//need fix size
	return 1;
}

//0060DF20 - identical
void CRaklionBattleOfSelupan::Run()
{
	if ( this->m_RaklionSelupan.GetSelupanLife() <= 0 && this->GetBattleOfSelupanState() != SELUPAN_STATE_DIE  )
	{
		this->SetState(SELUPAN_STATE_DIE);
		return;
	}

	if ( this->m_dwSkillDelay > GetTickCount() - this->m_dwSkillDelayTick )
	{		
		return;
	}

	switch ( this->m_iBattleOfSelupanState )
	{
	case SELUPAN_STATE_NONE:		this->ProcState_NONE();		break;
	case SELUPAN_STATE_STANDBY:		this->ProcState_STANDBY();	break;
	case SELUPAN_STATE_PATTERN_1:	this->ProcState_PATTERN_1();break;
	case SELUPAN_STATE_PATTERN_2:	this->ProcState_PATTERN_2();break;
	case SELUPAN_STATE_PATTERN_3:	this->ProcState_PATTERN_3();break;
	case SELUPAN_STATE_PATTERN_4:	this->ProcState_PATTERN_4();break;
	case SELUPAN_STATE_PATTERN_5:	this->ProcState_PATTERN_5();break;
	case SELUPAN_STATE_PATTERN_6:	this->ProcState_PATTERN_6();break;
	case SELUPAN_STATE_PATTERN_7:	this->ProcState_PATTERN_7();break;
	case SELUPAN_STATE_DIE:			this->ProcState_DIE();		break;
	}
}

//0060E080- identical
void CRaklionBattleOfSelupan::SetState(int iBattleOfSelupanState)
{
	if ( this->m_dwSkillDelay > GetTickCount() - this->m_dwSkillDelayTick )
	{
		return;
	}
		
	switch ( iBattleOfSelupanState )
	{
	case SELUPAN_STATE_NONE:		this->SetState_NONE();		break;
	case SELUPAN_STATE_STANDBY:		this->SetState_STANDBY();	break;
	case SELUPAN_STATE_PATTERN_1:	this->SetState_PATTERN_1();	break;
	case SELUPAN_STATE_PATTERN_2:	this->SetState_PATTERN_2();	break;
	case SELUPAN_STATE_PATTERN_3:	this->SetState_PATTERN_3();	break;
	case SELUPAN_STATE_PATTERN_4:	this->SetState_PATTERN_4();	break;
	case SELUPAN_STATE_PATTERN_5:	this->SetState_PATTERN_5();	break;
	case SELUPAN_STATE_PATTERN_6:	this->SetState_PATTERN_6();	break;
	case SELUPAN_STATE_PATTERN_7:	this->SetState_PATTERN_7();	break;
	case SELUPAN_STATE_DIE:			this->SetState_DIE();		break;
	}

	g_RaklionUtil.NotifyRaklionChangeState(11, this->GetBattleOfSelupanState());
}

//0060E1A0 - identical
void CRaklionBattleOfSelupan::SetState_NONE()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> NONE", this->m_iBattleOfSelupanState);
	
	this->m_RaklionSelupan.SelupanAct_BerserkCansel();
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_NONE);

}

//0060E1F0 - identical
void CRaklionBattleOfSelupan::SetState_STANDBY()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> STANDBY", this->m_iBattleOfSelupanState);
	
	this->m_RaklionSelupan.InitSelupanObjIndex();
	this->m_RaklionSelupan.CreateSelupan();
	this->m_RaklionSelupan.SelupanAct_BerserkCansel();
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_STANDBY);
}

//0060E260 - identical
void CRaklionBattleOfSelupan::SetState_PATTERN_1()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_1 // Life(%d/4800000)", this->m_iBattleOfSelupanState,this->m_RaklionSelupan.GetSelupanLife());
	
	this->m_RaklionSelupan.SelupanAct_BerserkCansel();
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_PATTERN_1);
}

//0060E2C0 - identical
void CRaklionBattleOfSelupan::SetState_PATTERN_2()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_2 // Life(%d/4800000)", this->m_iBattleOfSelupanState,this->m_RaklionSelupan.GetSelupanLife());

	this->m_RaklionSelupan.SelupanAct_Berserk1();  
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_PATTERN_2);
}

//0060E320 - identical
void CRaklionBattleOfSelupan::SetState_PATTERN_3()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_3 // Life(%d/4800000)", this->m_iBattleOfSelupanState,this->m_RaklionSelupan.GetSelupanLife());
	
	this->m_RaklionSelupan.SelupanAct_Berserk2();
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_PATTERN_3);
}

//0060E380 - identical
void CRaklionBattleOfSelupan::SetState_PATTERN_4()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_4 // Life(%d/4800000)", this->m_iBattleOfSelupanState,this->m_RaklionSelupan.GetSelupanLife());
	
	if ( this->m_RaklionSelupan.GetBerserkIndex() != 2 )
	{
		this->m_RaklionSelupan.SelupanAct_Berserk2();
	}
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_PATTERN_4);
}

//0060E400 - identical
void CRaklionBattleOfSelupan::SetState_PATTERN_5()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_5 // Life(%d/4800000)", this->m_iBattleOfSelupanState,this->m_RaklionSelupan.GetSelupanLife());
	
	this->m_RaklionSelupan.SelupanAct_Berserk3();
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_PATTERN_5);
}

//0060E460 - identical
void CRaklionBattleOfSelupan::SetState_PATTERN_6()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_6 // Life(%d/4800000)", this->m_iBattleOfSelupanState,this->m_RaklionSelupan.GetSelupanLife());
	
	this->m_RaklionSelupan.SelupanAct_Berserk4();
	
	this->SetBattleOfSelupanState(SELUPAN_STATE_PATTERN_6);
}

//0060E4C0 - identical
void CRaklionBattleOfSelupan::SetState_PATTERN_7()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_7 // Life(%d/4800000)", this->m_iBattleOfSelupanState,this->m_RaklionSelupan.GetSelupanLife());
	
	if ( this->m_RaklionSelupan.GetBerserkIndex() != 4 )
	{
		this->m_RaklionSelupan.SelupanAct_Berserk4();
	}

	this->SetBattleOfSelupanState(SELUPAN_STATE_PATTERN_7);
}

//0060E540 - identical
void CRaklionBattleOfSelupan::SetState_DIE()
{
	LogAddC(5, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> DIE", this->m_iBattleOfSelupanState);
	
	this->m_RaklionSelupan.SelupanAct_BerserkCansel();
	
	this->DeleteSummonMonster();
	
	int aIndex = this->m_RaklionSelupan.GetSelupanTargetIndex();

	LPOBJ lpObj = &gObj[aIndex];

	g_RaklionUtil.SendMsgAllUser(lMsg.Get(3412),lpObj->Name);

	time_t ltime;
	time(&ltime);
	tm * today=localtime(&ltime);

	for ( int i = 0; i < g_RaklionBattleUserMng.GetBattleUserCount(); ++i )
	{
		LogAdd("[Raklion][SelupanDie] %d/%d/%d %d:%d:%d %dth Winner : %s (%s)",
			today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,
			today->tm_min,today->tm_sec,i+1,gObj[g_RaklionBattleUserMng.GetBattleUserIndex(i)].Name,gObj[g_RaklionBattleUserMng.GetBattleUserIndex(i)].AccountID);
	}

	this->SetBattleOfSelupanState(SELUPAN_STATE_DIE);
}

//0060E6F0 -identical
void CRaklionBattleOfSelupan::ProcState_NONE()
{

}
//0060E710 -identical
void CRaklionBattleOfSelupan::ProcState_STANDBY()
{

}

//0060E730 - identical
void CRaklionBattleOfSelupan::ProcState_PATTERN_1()
{
	int skill = 0;

	while(this->m_PatternSkill[STATE_PATTERN_1].bIsSkillEnabled[skill] == 0)
	{
		skill = rand() % 9;
	}
	
	this->PatternStateSkillUseProc(skill);
}

//0060E790  - identical
void CRaklionBattleOfSelupan::ProcState_PATTERN_2()
{
	int skill = 0;

	while(this->m_PatternSkill[STATE_PATTERN_2].bIsSkillEnabled[skill] == 0)
	{
		skill = rand() % 9;
	}
	
	this->PatternStateSkillUseProc(skill);
}

//0060E7F0  - identical
void CRaklionBattleOfSelupan::ProcState_PATTERN_3()
{
	int skill = 0;

	while(this->m_PatternSkill[STATE_PATTERN_3].bIsSkillEnabled[skill] == 0)
	{
		skill = rand() % 9;
	}
	
	this->PatternStateSkillUseProc(skill);
}

//0060E850  - identical
void CRaklionBattleOfSelupan::ProcState_PATTERN_4()
{
	int skill = 0;

	while(this->m_PatternSkill[STATE_PATTERN_4].bIsSkillEnabled[skill] == 0)
	{
		skill = rand() % 9;
	}
	
	this->PatternStateSkillUseProc(skill);
}

//0060E8B0 - identical
void CRaklionBattleOfSelupan::ProcState_PATTERN_5()
{
	int skill = 0;

	while(this->m_PatternSkill[STATE_PATTERN_5].bIsSkillEnabled[skill] == 0)
	{
		skill = rand() % 9;
	}
	
	this->PatternStateSkillUseProc(skill);
}

//0060E910 - identical
void CRaklionBattleOfSelupan::ProcState_PATTERN_6()
{
	int skill = 0;

	while(this->m_PatternSkill[STATE_PATTERN_6].bIsSkillEnabled[skill] == 0)
	{
		skill = rand() % 9;
	}
	
	this->PatternStateSkillUseProc(skill);
}

//0060E970 - identical
void CRaklionBattleOfSelupan::ProcState_PATTERN_7()
{
	int skill = 0;

	while(this->m_PatternSkill[STATE_PATTERN_7].bIsSkillEnabled[skill] == 0)
	{
		skill = rand() % 9;
	}
	
	this->PatternStateSkillUseProc(skill);
}

//0060E9D0 - identical
void CRaklionBattleOfSelupan::ProcState_DIE()
{

}

//0060E9F0 - identical
void CRaklionBattleOfSelupan::PatternStateSkillUseProc(int SkillIndex)
{
	int iSelupanIndex = this->m_RaklionSelupan.GetSelupanObjIndex();

	LPOBJ lpObj = &gObj[iSelupanIndex];

	if ( lpObj->Teleport != 0 || lpObj->m_ActState.Move != FALSE )
	{
		return;
	}

	if ( SkillIndex < 0 || SkillIndex >= 9)
	{
		return;
	}

	switch ( SkillIndex )
	{
	case SKILL_FIRSTSKILL:		this->m_RaklionSelupan.SelupanAct_FirstSkill();		break;
	case SKILL_POISON_ATTACK:	this->m_RaklionSelupan.SelupanAct_PoisonAttack();	break;
	case SKILL_ICESTORM:		this->m_RaklionSelupan.SelupanAct_IceStorm();		break;
	case SKILL_ICESTRIKE:		this->m_RaklionSelupan.SelupanAct_IceStrike();		break;
	case SKILL_SUMMON:			this->m_RaklionSelupan.SelupanAct_SummonMonster();	break;
	case SKILL_HEAL:			this->m_RaklionSelupan.SelupanAct_Heal();			break;
	case SKILL_FREEZE:			this->m_RaklionSelupan.SelupanAct_Freeze();			break;
	case SKILL_TELEPORT:		this->m_RaklionSelupan.SelupanAct_Teleport();		break;
	case SKILL_INVINCIBILITY:	this->m_RaklionSelupan.SelupanAct_Invincibility();	break;
	}
	
	this->m_dwSkillDelayTick	= GetTickCount();
	this->m_dwSkillDelay		= this->m_RaklionSelupan.GetSelupanSkillDelay();
}

//0060EB80 - identical
void CRaklionBattleOfSelupan::SetPatternCondition(int iPattern,int iCondition)
{
	if ( iPattern < 0 || iCondition < 0 )
	{
		LogAddC(2,"[RAKLION][SetPatternCondition] error : PatternNum(%d), Condition(%d)",iPattern,iCondition);
	}

	this->m_PatternCondition[iPattern] = iCondition;
}

//0060EBE0 - identical
void CRaklionBattleOfSelupan::SelupanLifeCheck()
{
	int aIndex = 0;
	int MaxLife = 0;
	int Percent = 0;
	LPOBJ lpObj = 0;

	aIndex = this->m_RaklionSelupan.GetSelupanObjIndex();
	
	lpObj = &gObj[aIndex];
	
	if ( lpObj->DieRegen != 0)
	{
		return;
	}

	MaxLife = lpObj->AddLife + lpObj->MaxLife;
	
	if (MaxLife <= 0)
	{
		LogAddC(2,"[RAKLION] SelupanMAXLife error : Index(%d), MaxLife(%d)",lpObj->m_Index,lpObj->MaxLife);
		return;
	}	

	Percent = this->m_RaklionSelupan.GetSelupanLife() * 100 / MaxLife;
	
	if ( Percent < 0 )
	{
		LogAddC(2,"[RAKLION] SelupanLifePercent error : Index(%d), MaxLife(%d)",lpObj->m_Index,Percent);
		return;
	}
	
	if ( this->m_bIsUseFirstSkill == FALSE )
    {
		this->PatternStateSkillUseProc(SKILL_FIRSTSKILL);
		this->m_bIsUseFirstSkill = 1;
		return;
	}
	
	if ( Percent > this->m_PatternCondition[0] )
	{
		if(this->GetBattleOfSelupanState() != SELUPAN_STATE_PATTERN_1)
		{
			this->SetState(SELUPAN_STATE_PATTERN_1);
		}
	}
	else if ( Percent > this->m_PatternCondition[1] )
	{
		if ( this->GetBattleOfSelupanState() != SELUPAN_STATE_PATTERN_2 )
		{
			this->SetState(SELUPAN_STATE_PATTERN_2);
		}
	}
	else if ( Percent > this->m_PatternCondition[2] )
	{
		if ( this->GetBattleOfSelupanState() != SELUPAN_STATE_PATTERN_3 )
		{
			this->SetState(SELUPAN_STATE_PATTERN_3);
		}
	}
	else if ( Percent > this->m_PatternCondition[3] )
	{
		if ( this->GetBattleOfSelupanState() != SELUPAN_STATE_PATTERN_4 )
		{
			this->SetState(SELUPAN_STATE_PATTERN_4);
		}
	}
	else if ( Percent > this->m_PatternCondition[4] )
	{
		if ( this->GetBattleOfSelupanState() != SELUPAN_STATE_PATTERN_5 )
		{
			this->SetState(SELUPAN_STATE_PATTERN_5);
		}
	}
	else if ( Percent > this->m_PatternCondition[5] )
	{
		if ( this->GetBattleOfSelupanState() != SELUPAN_STATE_PATTERN_6 )
		{
			this->SetState(SELUPAN_STATE_PATTERN_6);
		}
	}
	else if ( Percent <= this->m_PatternCondition[5] )
	{
		if ( this->GetBattleOfSelupanState() != SELUPAN_STATE_PATTERN_7 )
		{
			this->SetState(SELUPAN_STATE_PATTERN_7);
		}
	}
}


//0060EE80  /> \55            PUSH EBP
//need to check (unused func)
int CRaklionBattleOfSelupan::GetBattleUserCount()
{
	return g_RaklionBattleUserMng.GetBattleUserCount();
}

//0060EEB0 (unused func)
void CRaklionBattleOfSelupan::CreateSelupan()
{
	this->m_RaklionSelupan.CreateSelupan();
}

//0060EEE0 
void CRaklionBattleOfSelupan::DeleteSelupan()
{
	this->m_RaklionSelupan.DeleteSelupan();
}

//0060EF10 (unused func)
int CRaklionBattleOfSelupan::GetSelupanObjIndex()
{
	return this->m_RaklionSelupan.GetSelupanObjIndex();
}

//0060EF40  
void CRaklionBattleOfSelupan::DeleteSummonMonster()
{
	this->m_RaklionSelupan.DeleteSummonMonster();
}

//0060EF70
void CRaklionBattleOfSelupan::SetSelupanSkillDelay(int SkillDelay)
{
	this->m_RaklionSelupan.SetSelupanSkillDelay(SkillDelay);
}

//0060EFA0 - identical
void CRaklionBattleOfSelupan::GmCommandSelupanPattern(int StatePattern)
{
	if ( StatePattern < 2 || StatePattern > 8)
	{
		LogAddC(2,"[RAKLION] [GmCommandSelupanPattern] Invalid pattern number : %d",StatePattern);
		return;
	}
	
	switch(StatePattern)
	{
	case 2: this->m_RaklionSelupan.SetSelupanLife(0x64);	break;
	case 3: this->m_RaklionSelupan.SetSelupanLife(0x50);	break;
	case 4:	this->m_RaklionSelupan.SetSelupanLife(0x3C);	break;
	case 5:	this->m_RaklionSelupan.SetSelupanLife(0x32);	break;
	case 6:	this->m_RaklionSelupan.SetSelupanLife(0x28);	break;
	case 7:	this->m_RaklionSelupan.SetSelupanLife(0x14);	break;
	case 8:	this->m_RaklionSelupan.SetSelupanLife(0x0A);	break;
	}
}