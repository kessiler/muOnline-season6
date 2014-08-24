//	GS-CS	1.00.90	JPN		-	Completed
#include "stdafx.h"
#include "Weapon.h"
#include "user.h"
#include "gObjMonster.h"
#include "ObjAttack.h"
#include "BuffEffectSlot.h"

CWeapon g_CsNPC_Weapon;

#define MAX_ST_CS_WEAPON	(4)

struct ST_CS_WEAPON
{
	BYTE btXBegin;	// 0
	BYTE btYBegin;	// 1
	BYTE btXEnd;	// 2
	BYTE btYEnd;	// 3
};

static const ST_CS_WEAPON WEAPON_CS_ATTACKER[MAX_ST_CS_WEAPON] =
{
	0x3E, 0x67, 0x48, 0x70, 0x58, 0x68, 0x7C, 0x6F,
	0x74, 0x69, 0x7C, 0x70, 0x49, 0x56, 0x69, 0x67
};

static const ST_CS_WEAPON WEAPON_CS_DEFENSER[MAX_ST_CS_WEAPON] =
{
	0x3D, 0x58, 0x5D, 0x6C, 0x5C, 0x59, 0x7F, 0x6F,
	0x54, 0x34, 0x66, 0x42, 0x00, 0x00, 0x00, 0x00
};

CWeapon::CWeapon()
{
	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)	// #error Cahnge to MAX_WEAPON_DAMAGED_TARGET_INFO
	{
		this->m_WeaponDamagedTargetInfo[i].RESET();
	}

	for (int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)	// #error Cahnge to MAX_WEAPON_CAL_DAMAGER_INFO
	{
		this->m_WeaponCalDamageInfo[i].RESET();
	}
}

CWeapon::~CWeapon()
{
	return;
}

BOOL CWeapon::SetWeaponCalDamageInfo(WORD wObjIndex, BYTE btTargetX, BYTE btTargetY, int iDelayTime)
{
	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == FALSE )
		{
			this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex = wObjIndex;
			this->m_WeaponCalDamageInfo[i].m_iTargetX = btTargetX;
			this->m_WeaponCalDamageInfo[i].m_iTargetY = btTargetY;
			this->m_WeaponCalDamageInfo[i].m_iLimitTime = GetTickCount() + iDelayTime;
			this->m_WeaponCalDamageInfo[i].m_IsUsed = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWeapon::GetWeaponCalDamageInfo(WORD wObjIndex, BYTE & btTargetX, BYTE & btTargetY)
{
	for( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == TRUE && this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == wObjIndex)
		{
			btTargetX = this->m_WeaponCalDamageInfo[i].m_iTargetX;
			btTargetY = this->m_WeaponCalDamageInfo[i].m_iTargetY;

			this->m_WeaponCalDamageInfo[i].RESET();

			return TRUE;
		}
	}

	return FALSE;
}

int  CWeapon::GetAttackDamage(int iObjClass)
{
	int iDamage = 0;

	if ( iObjClass == 221 )
	{
		iDamage = 300;
	}
	else if ( iObjClass == 222 )
	{
		iDamage = 200;
	}

	return iDamage;
}

BOOL CWeapon::GetTargetPointXY(int iObjClass, int iTargetPointIndex, BYTE &btX, BYTE &btY, BOOL bRandomPos)
{
	if ( iObjClass == 221 )
	{
		if ( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_ST_CS_WEAPON )
		{
			int tx = WEAPON_CS_ATTACKER[iTargetPointIndex].btXEnd - WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin;
			int ty = WEAPON_CS_ATTACKER[iTargetPointIndex].btYEnd - WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin;

			if ( bRandomPos != FALSE )
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin + rand() % tx;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin + rand() % ty;
			}
			else
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin + tx / 2;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin + ty / 2;
			}
		}
	}

	if ( iObjClass == 222 )
	{
		if ( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_ST_CS_WEAPON )
		{
			int tx = WEAPON_CS_DEFENSER[iTargetPointIndex].btXEnd - WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin;
			int ty = WEAPON_CS_DEFENSER[iTargetPointIndex].btYEnd - WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin;

			if ( bRandomPos != FALSE )
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin + rand() % tx;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin + rand() % ty;
			}
			else
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin + tx / 2;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin + ty / 2;
			}
		}
	}

	return TRUE;
}

BOOL CWeapon::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkill, int iSkillSuccess, BOOL& bAllMiss)
{
	return TRUE;
}

BOOL CWeapon::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, int iCriticalDamage, int iActionType)
{
	int iSkill = 0;
	int iSkillSuccess = TRUE;
	BYTE MsgDamage = 0;
	int ManaChange = 0;

	if ( lpMagic != NULL )
	{
		iSkill = lpMagic->m_Skill;
	}

	if ( (lpTargetObj->Authority & 2) == 2 )
	{
		return FALSE;
	}

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
	{
		return FALSE;
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( gObjIsConnected(lpTargetObj) == FALSE )
		{
			return FALSE;
		}
	}

	if ( gObjAttackQ(lpTargetObj) == 0 )
	{
		return FALSE;
	}

	lpObj->m_TotalAttackCount++;

	int MSBFlag = 0;

	if ( iCriticalDamage != 0 )
	{
		MsgDamage = 3;
	}
	else
	{
		MsgDamage = 0;
	}

	int iAttackDamage = this->GetAttackDamage(lpObj->Class);

	int iTargetDefense = lpTargetObj->m_Defense;//loc8

	int iSkillDefense = gObjGetTotalValueOfEffect(lpObj,AT_CW_ALTAR_STATUS);//loc9

	iTargetDefense -= (iTargetDefense * iSkillDefense)/100;

	BOOL bAllMiss = FALSE;
	
	if ( this->MissCheck(lpObj, lpTargetObj, iSkill, iSkillSuccess, bAllMiss) == FALSE )
	{
		return FALSE;
	}

	if ( bAllMiss != FALSE )
	{
		iAttackDamage = (iAttackDamage * 30)/100;
	}

	if ( lpTargetObj->DamageMinus != 0 )
	{
		int lc11 = iAttackDamage;
		iAttackDamage -= (iAttackDamage * lpTargetObj->DamageMinus)/100;
	}

	int iTargetLevel = lpObj->Level / 10;

	if ( iAttackDamage < iTargetLevel )
	{
		if ( iTargetLevel < 1 )
		{
			iTargetLevel = 1;
		}

		iAttackDamage = iTargetLevel;
	}

	if ( lpTargetObj->m_SkillNumber == 18 )
	{
		if ( iAttackDamage > 1 )
		{
			iAttackDamage = iAttackDamage >> 1;
		}
	}

	if ( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if ( iAttackDamage > 1 )
		{
			float lc13 = (float)(iAttackDamage * 8 ) / 10.0f;
			iAttackDamage = (int)( lc13 );
		}
	}

	if ( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem * Wing = &lpTargetObj->pInventory[7];

		if ( iAttackDamage > 1 )
		{
			if ( Wing->m_Type > ITEMGET(12,2) )
			{
				float lc15 = float((int)iAttackDamage *  (int)(75 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int )lc15;
			}
			else
			{
				float lc16 = float((int)iAttackDamage *  (int)(88 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
		}
	}

	if ( gObjDenorantSprite(lpTargetObj) != FALSE )
	{
		CItem * Dino = &lpTargetObj->pInventory[8];

		int ReduceDamage = 90 - Dino->IsDinorantReduceAttackDamaege();

		lpObj->Life -= 1.0f;

		if ( lpObj->Life < 0.0f )
		{
			lpObj->Life = 0;
		}
		else
		{
			iAttackDamage = (iAttackDamage * ReduceDamage)/100;
		}

		GCReFillSend(lpObj->m_Index, lpObj->Life, (BYTE)-1, 0, lpObj->iShield);
	}

	if ( gObjDarkHorse(lpTargetObj) != FALSE )
	{
		CItem * DarkHorse = &lpTargetObj->pInventory[8];

		int DHPercent = 100-(DarkHorse->m_PetItem_Level+30)/2;

		lpTargetObj->Life -= 1.0f;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0;
		}
		else
		{
			iAttackDamage = (iAttackDamage * DHPercent)/100;
		}

		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, (BYTE)-1, 0, lpTargetObj->iShield);
	}

	if ( lpTargetObj->Live != FALSE )
	{
		if ( gObjCheckUsedBuffEffect(lpTargetObj,4) == 1 && iAttackDamage > 0 )//changed
		{
			int iValue1 = 0;//loc21 
			int iValue2 = 0;//loc22
			
			gObjCheckUsedBuffEffect(lpTargetObj,4,&iValue1,&iValue2);//changed

			int ManaPercent = 0 ;//loc23

			if(iValue2 > 0)//new
			{
				ManaPercent = (WORD)lpTargetObj->Mana * iValue2 / 1000;//changed
			}
			else
			{
				ManaPercent = (WORD)lpTargetObj->Mana * 2 / 100;
			}

			if ( ManaPercent < lpTargetObj->Mana )
			{
				lpTargetObj->Mana -= ManaPercent;
				
				int loc24 = 0;//changed

				if(iValue1 > 0)//changed
				{
					loc24 = iAttackDamage * iValue1 / 100;//changed
				}

				iAttackDamage -= loc24;
				ManaChange = TRUE;
			}
			
		}

		lpTargetObj->Life -= iAttackDamage;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0;
		}
	}

	if ( iAttackDamage >= 1 )
	{
		if ( lpTargetObj->Type == OBJ_USER )
		{
			gObjArmorRandomDurDown(lpTargetObj, lpObj);
		}
	}

	if ( iAttackDamage >= 5 )
	{
		if ( lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( (rand()%26) == 0 )
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
			}
		}
		else if ( (rand()%4) == 0 )
		{
			if ( gObjUniriaSprite(lpTargetObj) == FALSE )
			{
				MSBFlag = TRUE;
			}
		}
	}

	if ( ManaChange != FALSE )
	{
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, (BYTE)-1, 0, lpTargetObj->BP);
	}

	lpObj->m_Rest = 0;

	if ( iAttackDamage > 0 )
	{
		gObjLifeCheck(lpTargetObj, lpObj, iAttackDamage, 0, MSBFlag, MsgDamage, iSkill, 0);
	}

	return TRUE;
}

BOOL CWeapon::AddWeaponDamagedTargetInfo(int iWeaponIndex, int iTargetIndex, int iDelayTime)
{
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)
	{
		if ( this->m_WeaponDamagedTargetInfo[i].m_IsUsed == FALSE )
		{
			this->m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex = iWeaponIndex;
			this->m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex = iTargetIndex;
			this->m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime = GetTickCount() + iDelayTime;
			this->m_WeaponDamagedTargetInfo[i].m_IsUsed = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

void CWeapon::WeaponAttackProc()
{
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)
	{
		if ( this->m_WeaponDamagedTargetInfo[i].m_IsUsed == TRUE )
		{
			if ( this->m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime > GetTickCount() )
			{
				continue;
			}

			LPOBJ lpTargetObj = &gObj[this->m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex];
			LPOBJ lpWeaponObj = & gObj[this->m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex];

			if ( gObjIsConnected(lpTargetObj) == FALSE )
			{
				this->m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}

			if (  OBJMAX_RANGE(lpWeaponObj->m_Index) == FALSE )
			{
				this->m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}

			this->Attack(lpWeaponObj, lpTargetObj, NULL, FALSE, 0);

			this->m_WeaponDamagedTargetInfo[i].RESET();
		}
	}

}

void CWeapon::WeaponAct(int iIndex)
{
	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == TRUE )
		{
			if ( this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == iIndex )
			{
				if ( this->m_WeaponCalDamageInfo[i].m_iLimitTime < GetTickCount() )
				{
					this->m_WeaponCalDamageInfo[i].RESET();
				}
			}
		}
	}
}