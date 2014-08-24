// RaklionSelupanMng.cpp: implementation of the CRaklionSelupanMng class.
// GS	1.00.90	JPN	0xXXXXXXXX	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionSelupan.h"
#include "User.h"
#include "TmonsterSkillManager.h"
#include "LogProc.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "MagicInf.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//0060C1C0 - identical
CRaklionSelupan::CRaklionSelupan()
{
	this->ClearData();
}

CRaklionSelupan::~CRaklionSelupan()
{

}
//0060C3D0 - identical
void CRaklionSelupan::ClearData()
{
	this->m_iSelupanObjIndex	= -1;
	this->m_iBerserkIndex		= 0;
	this->m_iBossAttackMin		= 0;
	this->m_iBossAttackMax		= 0;
}

//0060C430 - identical
void CRaklionSelupan::SelupanAct_FirstSkill()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = this->GetSelupanTargetIndex();
	
	LPOBJ lpObj = &gObj[aIndex];

	if ( tIndex == -1 )
	{
		tIndex = aIndex;
	}
#if (DEBUG_RAKLION == 1)
	else
#endif
	{
		CMagicInf lpMagic;

		memset(&lpMagic, 0, 0x14);

		lpMagic.m_Skill = 0xFD;

		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 31, 0,&lpMagic);
	}

	LogAddTD("[ RAKLION ][ FirstSkill ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);
}


//0060C540 - identical
void CRaklionSelupan::SelupanAct_PoisonAttack()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = this->GetSelupanTargetIndex();

	LPOBJ lpObj = &gObj[aIndex];


	if ( tIndex == -1 )
	{
		tIndex = aIndex;
		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 7, NULL);
	}
	else
	{
		CMagicInf lpMagic;

		memset(&lpMagic, 0, 0x14);

		lpMagic.m_Skill = 0xFA;

		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 1,&lpMagic);
	}

	LogAddTD("[ RAKLION ][ PoisonAttack ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);

}
//0060C660 - identical
void CRaklionSelupan::SelupanAct_IceStorm()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = this->GetSelupanTargetIndex();

	LPOBJ lpObj = &gObj[aIndex];

	if ( tIndex == -1 )
	{
		tIndex = aIndex;
		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 7, NULL);
	}
	else
	{
		CMagicInf lpMagic;

		memset(&lpMagic, 0, 0x14);

		lpMagic.m_Skill = 0xFB;
		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 2,&lpMagic);
	}
	LogAddTD("[ RAKLION ][ IceStorm ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);
}

//0060C780 - identical
void CRaklionSelupan::SelupanAct_IceStrike()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = this->GetSelupanTargetIndex();

	LPOBJ lpObj = &gObj[aIndex];

	if ( tIndex == -1 )
	{
		tIndex = aIndex;
		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 7, NULL);
	}
	else
	{
		CMagicInf lpMagic;

		memset(&lpMagic, 0, 0x14);

		lpMagic.m_Skill = 0xFC;
		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 3,&lpMagic);
	}

	LogAddTD("[ RAKLION ][ IceStrike ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);
}
//0060C8A0 - identical
void CRaklionSelupan::SelupanAct_SummonMonster()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = aIndex;

	LPOBJ lpObj = &gObj[aIndex];
	
	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 30, 4, NULL);
	
	this->CreateSummonMonster();
	
	LogAddTD("[ RAKLION ][ SummonMonster ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);
}
//0060C930 - identical
void CRaklionSelupan::SelupanAct_Heal()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = aIndex;

	LPOBJ lpObj = &gObj[aIndex];

	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 20, 5, NULL);

	LogAddTD("[ RAKLION ][ Heal ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);
}
//0060C9B0	- identical
void CRaklionSelupan::SelupanAct_Freeze()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = this->GetSelupanTargetIndex();

	LPOBJ lpObj = &gObj[aIndex];

	if ( tIndex == -1 )
	{
		tIndex = aIndex;
		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 7, NULL);
	}
	else
	{
		TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 6, NULL);
	}
	LogAddTD("[ RAKLION ][ Freeze ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);
}
//0060CA70 - identical
void CRaklionSelupan::SelupanAct_Teleport()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = this->GetSelupanTargetIndex();

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[tIndex];
	
	if ( tIndex < 0 )
	{
		tIndex = aIndex;
	}

	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 7, NULL);
	LogAddTD("[ RAKLION ][ Teleport ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);
}

//0060CB20 - identical
void CRaklionSelupan::SelupanAct_Invincibility()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = aIndex;

	LPOBJ lpObj = &gObj[aIndex];
	
	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 10, 8, NULL);
	
	LogAddTD("[ RAKLION ][ Invincibility ] Skill Using : Index(%d) %s", aIndex,lpObj->Name);

}
//0060CBA0 - identical
void CRaklionSelupan::SelupanAct_BerserkCansel()
{
	int aIndex = this->GetSelupanObjIndex();
	
	LPOBJ lpObj = &gObj[aIndex];
	
	lpObj->m_AttackDamageMin = this->m_iBossAttackMin;
	lpObj->m_AttackDamageMax = this->m_iBossAttackMax;
	
	LogAddTD("[ RAKLION ][ Berserk Cancel ] MinDamage(%d) Damage(%d~%d)",lpObj->m_AttackDamageMin,lpObj->m_AttackDamageMin,lpObj->m_AttackDamageMax);

}

//0060CC50 - identical
void CRaklionSelupan::SelupanAct_Berserk1()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = aIndex;

	LPOBJ lpObj = &gObj[aIndex];

	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 9, NULL);
	
	this->m_iBerserkIndex = 1;

	int loc5 = 0;
	loc5 = this->m_iBerserkIndex * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = this->m_iBossAttackMin + loc5;
	lpObj->m_AttackDamageMax = this->m_iBossAttackMax + loc5;


	LogAddTD("[ RAKLION ][ Berserk1 ] Skill Using : Index(%d) Damage(%d~%d)",aIndex,lpObj->m_AttackDamageMin,lpObj->m_AttackDamageMax);
}

//0060CD50 - identical
void CRaklionSelupan::SelupanAct_Berserk2()
{
	int aIndex = this->GetSelupanObjIndex();
	int tIndex = aIndex;

	LPOBJ lpObj = &gObj[aIndex];

	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 9, NULL);
	
	this->m_iBerserkIndex = 2;

	int loc5 = 0;
	loc5 = this->m_iBerserkIndex * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = this->m_iBossAttackMin + loc5;
	lpObj->m_AttackDamageMax = this->m_iBossAttackMax + loc5;


	LogAddTD("[ RAKLION ][ Berserk2 ] Skill Using : Index(%d) Damage(%d~%d)",aIndex,lpObj->m_AttackDamageMin,lpObj->m_AttackDamageMax);
}

//0060CE50 - identical
void CRaklionSelupan::SelupanAct_Berserk3()
{
int aIndex = this->GetSelupanObjIndex();
	int tIndex = aIndex;

	LPOBJ lpObj = &gObj[aIndex];

	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 9, NULL);
	
	this->m_iBerserkIndex = 3;

	int loc5 = 0;
	loc5 = this->m_iBerserkIndex * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = this->m_iBossAttackMin + loc5;
	lpObj->m_AttackDamageMax = this->m_iBossAttackMax + loc5;


	LogAddTD("[ RAKLION ][ Berserk3 ] Skill Using : Index(%d) Damage(%d~%d)",aIndex,lpObj->m_AttackDamageMin,lpObj->m_AttackDamageMax);
}

//0060CF50 - identical
void CRaklionSelupan::SelupanAct_Berserk4()
{
int aIndex = this->GetSelupanObjIndex();
	int tIndex = aIndex;

	LPOBJ lpObj = &gObj[aIndex];

	TMonsterSkillManager::UseMonsterSkill(aIndex, tIndex, 1, 9, NULL);
	
	this->m_iBerserkIndex = 4;

	int loc5 = 0;
	loc5 = this->m_iBerserkIndex * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

	lpObj->m_AttackDamageMin = this->m_iBossAttackMin + loc5;
	lpObj->m_AttackDamageMax = this->m_iBossAttackMax + loc5;


	LogAddTD("[ RAKLION ][ Berserk4 ] Skill Using : Index(%d) Damage(%d~%d)",aIndex,lpObj->m_AttackDamageMin,lpObj->m_AttackDamageMax);
}

//0060D050 - identical
DWORD CRaklionSelupan::GetSelupanLife()
{
	int aIndex = this->GetSelupanObjIndex();
	
	LPOBJ lpObj = &gObj[aIndex];

	return lpObj->Life;
}

//0060D0A0 - identical
void CRaklionSelupan::SetSelupanLife(int arg1)
{
	int aIndex = this->GetSelupanObjIndex();

	LPOBJ lpObj = &gObj[aIndex];
	
	float loc4 = lpObj->AddLife + lpObj->MaxLife;

	lpObj->Life = loc4 * arg1 / 100;
}

// 0060D120 - identical
void CRaklionSelupan::InitSelupanObjIndex()
{
	for(int i = 0;i < OBJ_MAXMONSTER;i++)
	{
		if( gObj[i].Connected == PLAYER_PLAYING )
		{
			if( gObj[i].Class == 459)
			{
				this->m_iSelupanObjIndex = i;
			}
		}	
	}

	if( this->m_iSelupanObjIndex == -1 )
	{
		LogAddC(2,"[ RAKLION ] [ InitSelupanIndex ] error : SelupanObjIndex(%d)",this->m_iSelupanObjIndex);
	}
}
//0060D1E0 - identical
BOOL CRaklionSelupan::CreateSelupan()
{
	int m_Type = -1;

	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if( gMSetBase.m_Mp[n].m_Type == 459 )
		{
			m_Type = n;
		}
	}

	LPOBJ lpObj = NULL;
	int result = 0;


	result = gObjAddMonster(MAP_INDEX_RAKLION_BOSS);

	if ( result < 0 )
	{
		LogAddTD("[RAKLION] CreateSelupan error");
		return FALSE;
	}
	
	lpObj = &gObj[result];
	
	gObjSetPosMonster(result, m_Type);
	
	gObjSetMonster(result, 459); 
	
	LogAddTD("[RAKLION] Create Selupan : X (%d) / Y(%d)",lpObj->X,lpObj->Y);

	LPOBJ lpTargetObj = NULL;

	for (int n=0;n<OBJMAX;n++)
	{
		lpTargetObj = &gObj[n];

		if(lpTargetObj->MapNumber == MAP_INDEX_RAKLION_BOSS)
		{
			gObjStateSetCreate(n);
		}
	}

	for (int n=0;n<OBJMAX;n++)
	{
		lpTargetObj = &gObj[n];

		if(lpTargetObj->MapNumber == MAP_INDEX_RAKLION_BOSS)
		{
			gObjViewportListDestroy(n);
		}
	}
	
	for (int  n=0;n<OBJMAX;n++)
	{
		lpTargetObj = &gObj[n];

		if(lpTargetObj->MapNumber == MAP_INDEX_RAKLION_BOSS)
		{
			gObjViewportListCreate(n);
		}
	}
	
	for (int n=0;n<OBJMAX;n++)
	{
		lpTargetObj = &gObj[n];

		if(lpTargetObj->MapNumber == MAP_INDEX_RAKLION_BOSS)
		{
			gObjViewportListProtocol(n);
		}
	}

	gObjSetState();

	this->m_iBossAttackMin = lpObj->m_AttackDamageMin;
	this->m_iBossAttackMax = lpObj->m_AttackDamageMax;

	return TRUE;
}

//0060D4D0 - identical
void CRaklionSelupan::DeleteSelupan()
{
	int aIndex = this->GetSelupanObjIndex();
	gObjViewportListProtocolDestroy(&gObj[aIndex]);
	gObjViewportClose(&gObj[aIndex]);
	gObjDel(aIndex);
}

//0060D550 - identical
int CRaklionSelupan::GetSelupanTargetIndex()
{
	int iSelupanIndex = this->GetSelupanObjIndex();

	LPOBJ lpObj = &gObj[iSelupanIndex];

	if( lpObj->TargetNumber >= 0 && lpObj->TargetNumber < OBJMAX )
	{
		return lpObj->TargetNumber;
	}

	return gObjMonsterSearchEnemy(lpObj,lpObj->Type);
}

//0060D5E0 - identical
void CRaklionSelupan::SetSelupanSkillDelay(int SkillDelay)
{
	if ( SkillDelay < 0 || SkillDelay > 50000 )
	{
		LogAddC(2,"[ RAKLION ][ SetSelupanSkillDelay ] SkillDelay error : Delay(%d)",this->m_iSelupanSkillDelay);
		return;
	}
	
	this->m_iSelupanSkillDelay = SkillDelay;
}


//0060D640 - identical
int CRaklionSelupan::GetSelupanSkillDelay()
{
	if ( this->m_iSelupanSkillDelay < 0 || this->m_iSelupanSkillDelay > 50000 )
	{
		LogAddC(2,"[ RAKLION ][ GetSelupanSkillDelay ] SkillDelay error : Delay(%d)",this->m_iSelupanSkillDelay);
		return -1;
	}

	return this->m_iSelupanSkillDelay;
}

//0060D6B0- identical
int CRaklionSelupan::GetBerserkIndex()
{
	return this->m_iBerserkIndex;
}

//0060D6E0 - identical
void CRaklionSelupan::SetBossAttackDamage(int DamageMin,int DamageMax)
{
	this->m_iBossAttackMin = DamageMin;
	this->m_iBossAttackMax = DamageMax;
}

//0060D720 -identical
void CRaklionSelupan::SetSelupanObjIndex(int arg1)
{
	if ( arg1 < 0 || arg1 > OBJ_MAXMONSTER ) 
	{
		LogAddC(2,"[ RAKLION ][ SetSelupanObjIndex ] SelupanObjIndex error : Index(%d)", arg1);
		return;
	}

	this->m_iSelupanObjIndex = arg1;
}

//0060D780 - identical
int CRaklionSelupan::GetSelupanObjIndex()
{
	this->InitSelupanObjIndex();

	return this->m_iSelupanObjIndex;
}

//0060D7B0 - identical
void CRaklionSelupan::CreateSummonMonster()
{
	LPOBJ lpObj = NULL;//loc2 
	int loc3 = -1;
	int loc4 = -1;

	for (int i = 0; i < OBJ_MAXMONSTER; ++i )
	{
		lpObj = &gObj[i];

		if ( lpObj->MapNumber != MAP_INDEX_RAKLION_BOSS )
		{
			continue;
		}

		if(lpObj->Class == 457 || lpObj->Class == 458)
		{
			if(lpObj->Connected == 3 && lpObj->DieRegen == 0 && lpObj->Live == 1) 
			{
				continue;
			}

			loc4 = lpObj->Class;
			loc3 = lpObj->m_PosNum;
			
			gObjCharZeroSet(i);
			
			gObjDel(lpObj->m_Index);

			lpObj->Class = 0;

			int iMonsterIndex = gObjAddMonster(MAP_INDEX_RAKLION_BOSS);

			lpObj = &gObj[iMonsterIndex];

			if (iMonsterIndex >= 0)
			{
				gObjSetPosMonster(iMonsterIndex, loc3);
				gObjSetMonster(iMonsterIndex, loc4); 
				lpObj->Live = 1;
				lpObj->DieRegen = 0;
				LogAddTD("[RAKLION] Summoned Monster : %s => X-(%d) Y-(%d)",lpObj->Name,lpObj->X,lpObj->Y);
				return;
			}

			LogAddTD("[RAKLION] CreateSelupan error");	
			return;
		}
	}
}

//0060D9C0 - identical
void CRaklionSelupan::DeleteSummonMonster()
{
	LPOBJ lpObj = NULL;

	for(int i=0;i< OBJ_MAXMONSTER;i++)
	{
		lpObj = &gObj[i];

		if(lpObj->MapNumber != MAP_INDEX_RAKLION_BOSS)
		{
			continue;
		}
			
		if(lpObj->Class == 457 || lpObj->Class == 458)
		{
			gObjViewportListProtocolDestroy(lpObj);
			gObjViewportClose(lpObj);
			gObjDel(lpObj->m_Index);
			lpObj->Live = FALSE; 
			lpObj->DieRegen = TRUE;
		}
	}

	LogAddC(5,"[ RAKLION ] [ DeleteSummonMonster ] Delete All SummonMonster");
}
