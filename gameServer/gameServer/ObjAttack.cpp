//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "ObjAttack.h"
#include "BattleSoccerManager.h"
#include "DarkSpirit.h"
#include "ObjUseSkill.h"
#include "gObjMonster.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "IllusionTempleEvent.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSkillTreeSystem.h"
#include "Gamemain.h"
#include "ItemAddOption.h"

#ifdef NPVP
#include "NewPVP.h"
#endif

#ifdef GENS
#include "GensSystem.h"
#endif

#ifdef IMPERIAL
#include "ImperialGuardian.h"
#endif

#include "LuckyItem.h"

#ifdef __ALIEN__
#include "BalanceSystem.h"
#include "ItemOption.h"
#endif

#include "SkillAdditionInfo.h"

CObjAttack gclassObjAttack;

CObjAttack::CObjAttack()
{

}

CObjAttack::~CObjAttack()
{

}

BOOL CObjAttack::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic,  int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo,BYTE byBarrageCount, BYTE byReflect)
{
	int skillSuccess = 0;
	LPOBJ lpCallObj;
	LPOBJ lpCallTargetObj;
	BYTE MsgDamage = 0;
	int ManaChange = 0;
	int iTempShieldDamage = 0;
	int iTotalShieldDamage = 0;
	int bDragonKickSDAttackSuccess = 0;
	
	if ( (lpTargetObj->Authority&2) == 2 )
		return FALSE;

	if ( (lpObj->Authority&32) == 32 ||
		 (lpTargetObj->Authority&32) == 32 )
	{
		if( gObjCheckUsedBuffEffect(lpObj,AT_INVISIBILITY) == TRUE)
		{
			return false;
		}
	}

#ifdef NPVP
	if( lpObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpObj) && g_NewPVP.IsSafeState(*lpObj) )
	{
		return false;
	}
	// ----
	if( lpTargetObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpTargetObj) && g_NewPVP.IsSafeState(*lpTargetObj) )
	{
		return false;
	}
#endif

#ifdef GENS	//-> Need check
	//if( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER && !gGensSystem.IsPkEnable(lpObj, lpTargetObj) )
	//{
	//	return false;
	//}
	if( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) )
		{
			if( gGensSystem.GetGensInfluence(lpObj) == gGensSystem.GetGensInfluence(lpTargetObj) && !gGensSystem.AllowPK )
			{
				return false;
			}
		}
	}
#endif

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
	{
		return false;
	}

	if(g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_READY || g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_END)
	{
		if( ((lpTargetObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE) ? TRUE : FALSE) && lpTargetObj->Type == OBJ_MONSTER)
		{
			return FALSE;
		}
	}

	if ( g_iUseCharacterAutoRecuperationSystem && lpObj->Level <= g_iCharacterRecuperationMaxLevel )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Level <= g_iCharacterRecuperationMaxLevel ) // #warning useless code
			{
				lpObj->m_iAutoRecuperationTime = GetTickCount();
			}
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			if ( lpTargetObj->Level <= g_iCharacterRecuperationMaxLevel )
			{
				lpTargetObj->m_iAutoRecuperationTime = GetTickCount();
			}
		}
	}

	if ( lpObj->Type == OBJ_USER )
		lpObj->dwShieldAutoRefillTimer = GetTickCount();

	if ( lpTargetObj->Type == OBJ_USER )
		lpTargetObj->dwShieldAutoRefillTimer = GetTickCount();

	int skill = 0;
	
	if ( lpMagic )
		skill = lpMagic->m_Skill;

	if ( lpObj->Class == 77 )
	{
		if ( lpObj->m_SkyBossMonSheild && skill == 17 )
		{
			skill = 3;
		}
	}

	skillSuccess = TRUE;

	if ( lpObj->GuildNumber > 0 )
	{
		if ( lpObj->lpGuild )
		{
			if ( lpObj->lpGuild->WarState )
			{
				if ( lpObj->lpGuild->WarType == 1 )
				{
					if ( !GetBattleSoccerGoalMove(0) )
					{
						return TRUE;
					}
				}
			}

			if ( lpObj->lpGuild->WarState )
			{
				if ( lpObj->lpGuild->WarType == 0 )
				{
					if ( lpTargetObj->Type == OBJ_MONSTER )
					{
						return TRUE;
					}
				}
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_iMonsterBattleDelay > 0 )
			return TRUE;

		if ( lpTargetObj->Class == 200 )
		{
			if ( skill )
			{
				gObjMonsterStateProc(lpTargetObj, 7, lpObj->m_Index, 0);
			}
			else
			{
				gObjMonsterStateProc(lpTargetObj, 6, lpObj->m_Index, 0);
			}

			if ( magicsend )
			{
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
			}

			return TRUE;
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj,AT_IMMUNE_MAGIC) == TRUE )
		{
			BOOL bCheckAttackIsMagicType = gObjCheckAttackTypeMagic(lpObj->Class, skill);

			if ( bCheckAttackIsMagicType == 1 )
			{
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, 0);
				return TRUE;
			}
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj,AT_IMMUNE_HARM) == TRUE  )
		{
			BOOL bCheckAttackIsMagicType = gObjCheckAttackTypeMagic(lpObj->Class, skill);

			if ( bCheckAttackIsMagicType == 0 )
			{
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return TRUE;
			}
		}


		if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime > 0 )
		{
			if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime == skill )
			{
				return TRUE;
			}
		}

		if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementBerserkTime > 0 )
		{
			int MSBDamage = 32;
			GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSBDamage, 0);
			return TRUE;
		}
	}

#ifdef IMPERIAL
	if ( lpTargetObj->Class == 523 )
		return TRUE;

	if ( lpTargetObj->Class >= 524 && lpTargetObj->Class <= 528 
		&& lpTargetObj->Class != 526
		&& !g_ImperialGuardian.IsAttackAbleMonster(lpTargetObj->m_Index) )
	{
		return TRUE;
	}
#endif

	if ( lpObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpObj) )
		{
			return FALSE;
		}

		gDarkSpirit[lpObj->m_Index].SetTarget(lpTargetObj->m_Index);
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpTargetObj))
		{
			return FALSE;
		}
	}

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )	// PLAYER VS MONSTER
	{
		if ( lpObj->m_RecallMon >= 0 )
		{
			if ( lpObj->m_RecallMon == lpTargetObj->m_Index )
			{
				return FALSE;
			}
		}
	}

	if ( !gObjAttackQ(lpTargetObj))
		return FALSE;

	if ( lpObj->m_RecallMon >= 0 )
		gObjCallMonsterSetEnemy(lpObj, lpTargetObj->m_Index);

	lpObj->m_TotalAttackCount++;

	if ( AttackDamage == 0 )
	{
		if ( skill != 76 )
		{
			if ( gObjCheckUsedBuffEffect(lpObj,AT_INFINITY_ARROW) == FALSE &&
				 gObjCheckUsedBuffEffect(lpObj,143) == FALSE )
			{
				if ( !this->DecreaseArrow(lpObj) )
				{
					return FALSE;
				}
			}
		}
	}

	if ( this->CheckAttackArea(lpObj, lpTargetObj) == FALSE )
	{
		return FALSE;
	}

	lpCallObj = lpObj;

	if ( lpObj->Type == OBJ_MONSTER )
	{
		if ( lpObj->m_RecallMon >= 0 )
		{
			lpCallObj = &gObj[lpObj->m_RecallMon];
		}
	}

	lpCallTargetObj = lpTargetObj;

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( this->PkCheck(lpCallObj, lpTargetObj) == FALSE )
		return FALSE;

	int Strength = lpObj->Strength + lpObj->AddStrength;
	int Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
	int Vitality = lpObj->Vitality + lpObj->AddVitality;
	int Energy = lpObj->Energy + lpObj->AddEnergy;
	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

	if ( lpObj->pInventory[0].m_Type == ITEMGET(2,5) && lpObj->pInventory[0].m_IsValidItem != false )	// Crystal Sword
	{
		if ( (rand()%20) == 0 )
		{
			skill = 7;
			lpMagic = &DefMagicInf[7];
			magicsend = 1;
		}
	}
	else if ( lpObj->pInventory[1].m_Type == ITEMGET(2,5) && lpObj->pInventory[1].m_IsValidItem != false )	// Crystal Sword
	{
		if ( (rand()%20) == 0 )
		{
			skill = 7;
			lpMagic = &DefMagicInf[7];
			magicsend = 1;
		}
	}

	MSBFlag = 0;
	MsgDamage = 0;
	skillSuccess = this->ResistanceCheck(lpObj, lpTargetObj, skill);
	BOOL skillIceArrowSuccess = skillSuccess;

	if ( skill == 51 || skill == 424 )
	{
		skillSuccess = 0;
	}
	BOOL bAllMiss = FALSE;
	
	if ( gObjCheckUsedBuffEffect(lpObj,AT_INVISIBILITY) == TRUE && (lpObj->Authority & 32) != 32)
	{
		gObjUseSkill.RemoveCloakingEffect(lpObj->m_Index);
	}

	BOOL bDamageReflect = FALSE;
	
	if ( AttackDamage == 0 )
	{
		if ( g_ShieldSystemOn == TRUE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( !this->MissCheckPvP(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, byBarrageCount) )
				{
					return FALSE;
				}
			}
			else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, byBarrageCount) )
			{
				return FALSE;
			}

		}
		else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss, byBarrageCount) )
		{
			return FALSE;
		}

		if ( (skill == 51 || skill == 424) && skillIceArrowSuccess == TRUE )
		{
			gObjAddBuffEffect(lpTargetObj,AT_ICE_ARROW,0,0,0,0,7);
			lpTargetObj->lpAttackObj = lpObj;
			lpTargetObj->PathCount = 0;
			lpTargetObj->PathStartEnd = 0;
			skillSuccess = TRUE;
			gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
		}

		//Check it fuck!
		int targetdefense = this->GetTargetDefense(lpObj, lpTargetObj, MsgDamage);

		if( gObjCheckUsedBuffEffect(lpTargetObj,AT_BERSERKER) == TRUE ||
			gObjCheckUsedBuffEffect(lpTargetObj,150) == TRUE ||
			gObjCheckUsedBuffEffect(lpTargetObj,151) == TRUE ||
			gObjCheckUsedBuffEffect(lpTargetObj,152) == TRUE )
		{
			float fDefense = (lpTargetObj->Dexterity+lpTargetObj->AddDexterity)/3;
			float fValue = (float)gObjGetTotalValueOfEffect(lpTargetObj, 32);
			float fPercent = (40- (fValue+lpObj->m_MPSkillOpt.MpsBerserkInc1))/100;
			fPercent = (0.1f > fPercent) ? 0.1f : fPercent;
		}

		int iCurseValue = 0;
		/*if( !gObjCheckUsedBuffEffect(lpTargetObj,BUFF_INNOVATION2,&iCurseValue,0) )*/
			//gObjCheckUsedBuffEffect(lpTargetObj,AT_INNOVATION,&iCurseValue,0);
			gObjCheckUsedBuffEffect(lpTargetObj, AT_INNOVATION, &iCurseValue, 0);

		targetdefense -= targetdefense * iCurseValue / 100;

		switch( skill )
		{
		case AT_SKILL_SWORD1:
		case AT_SKILL_SWORD2:
		case AT_SKILL_SWORD3:
		case AT_SKILL_SWORD4:
		case AT_SKILL_SWORD5:
		case AT_SKILL_SWORD6:
		case AT_SKILL_WHEEL:
		case AT_SKILL_KNIGHTSPEAR:
		case AT_SKILL_BLOWOFFURY:
		case AT_SKILL_KNIGHTDINORANT:
		case AT_SKILL_STRIKE:
		case AT_SKILL_DEFENSEDOWN:
		case AT_SKILL_RUSH:
		case AT_SKILL_ONE_FLASH:
		case AT_SKILL_SPACE_SPLIT:
		case 235:	//MultiShot
		case 232:	//Destruction
		case 236:	//Flame Strike
		case 238:	//Chaotic Diseier
		case 326:
		case 327:
		case 328:
		case 329:
		case 330:
		case 331:
		case 332:
		case 333:
		case 336:
		case 339:
		case 342:
		case 337:
		//case AT_MSKILL_DK_DESTRUCTION2:
		//case AT_MSKILL_DK_DESTRUCTION3:
		case 344:
		case 346:
		case 411:
		case 431:
		case 479:
		case 481:
		case 493:
		//case AT_MSKILL_MG_FIRESLASH2:
		case 482:
		case 492:
		case 494:
			{
				AttackDamage = this->GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel, lpMagic);

				if ( AttackDamage > 0 )
				{
					gObjWeaponDurDown(lpObj, lpTargetObj, 0);
				}
			}
			break;
		case 76:
			{
				int iFenrirAttackDmg = 0;

				if ( lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA )
				{
					iFenrirAttackDmg = lpObj->Strength / 3 + lpObj->Dexterity / 5 + lpObj->Vitality / 5 + lpObj->Energy / 7;	// #formula
				}
				else if ( lpObj->Class == CLASS_WIZARD )
				{
					iFenrirAttackDmg = lpObj->Strength / 5 + lpObj->Dexterity / 5 + lpObj->Vitality / 7 + lpObj->Energy / 3;	// #formula
				}
				else if ( lpObj->Class == CLASS_ELF )
				{
					iFenrirAttackDmg = lpObj->Strength / 5 + lpObj->Dexterity / 3 + lpObj->Vitality / 7 + lpObj->Energy / 5;	// #formula
				}
				else	// Dark Lord
				{
					iFenrirAttackDmg = lpObj->Strength / 5 + lpObj->Dexterity / 5 + lpObj->Vitality / 7 + lpObj->Energy / 3 + lpObj->Leadership / 3;	// #formula
				}

				if ( iFenrirAttackDmg < 0 )
					iFenrirAttackDmg = 0;

				if ( lpObj->m_CriticalDamage > 0 )
				{
					if ( (rand()%100) < lpObj->m_CriticalDamage )
					{
						MsgDamage = 3;
					}
				}

				if ( lpObj->m_ExcelentDamage > 0 )
				{
					if ( (rand()%100) < lpObj->m_ExcelentDamage )
					{
						MsgDamage = 2;
					}
				}

				if ( MsgDamage == 3 )	// Critical Damage
				{
					AttackDamage =  iFenrirAttackDmg + lpMagic->m_DamageMax;
					AttackDamage += lpObj->SetOpAddCriticalDamage;
					int adddamage = 0;
					gObjCheckUsedBuffEffect(lpObj, AT_INCREASE_CRITICAL_DMG, &adddamage, 0);
					AttackDamage += adddamage;
					gObjCheckUsedBuffEffect(lpObj, 148, &adddamage, 0);
					AttackDamage += adddamage;
					AttackDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
					AttackDamage -= targetdefense;
				}
				else if ( MsgDamage == 2 )	// Excellent
				{
					AttackDamage = iFenrirAttackDmg + lpMagic->m_DamageMax;
					AttackDamage += AttackDamage * 20 / 100;
					AttackDamage += lpObj->SetOpAddExDamage;
					AttackDamage -= targetdefense;
				}
				else
				{
					AttackDamage = (iFenrirAttackDmg + lpMagic->m_DamageMin) + (rand()%(lpMagic->m_DamageMax - lpMagic->m_DamageMin + 1));
					AttackDamage -= targetdefense;
				}
			}
			break;
		default:
			{
				if ( ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_MAGUMSA ) && skill )
				{
					AttackDamage = GetAttackDamageWizard(lpObj, targetdefense, lpMagic, MsgDamage, bIsOnDuel);
	
					if ( AttackDamage > 0 )
					{
						gObjWeaponDurDown(lpObj, lpTargetObj, 1);
					}
				}
				else if( lpObj->Class == CLASS_SUMMONER && skill )
				{
					AttackDamage = GetAttackDamageSummoner(lpObj, targetdefense, lpMagic, MsgDamage, bIsOnDuel);
	
					if ( AttackDamage > 0 )
					{
						gObjWeaponDurDown(lpObj, lpTargetObj, 1);
					}
				}
				else if( lpObj->Class == CLASS_MONK )
				{
					AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel, lpMagic);
	
					if ( AttackDamage > 0 )
					{
						gObjWeaponDurDown(lpObj, lpTargetObj, 0);
					}
				}
				else
				{
					AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel, lpMagic);
	
					if ( AttackDamage > 0 )
					{
						gObjWeaponDurDown(lpObj, lpTargetObj, 0);
					}
				}
			}
			break;
		}

		int nBuffIndex = -1;
		int nValue = 0;

		int attackdamagebefore = AttackDamage;

		switch( MsgDamage )
		{
		case 3:
			nBuffIndex = AT_CSHOP_SCROLL_OF_BATTLE;

			if( nBuffIndex > -1 )
			{
				gObjCheckUsedBuffEffect(lpObj,nBuffIndex,&nValue,0);

				if( nValue > 0 )
				{
					AttackDamage += AttackDamage * nValue / 100;
				}
			}

			nValue = 0;
			nBuffIndex = 0x7A;

			if( nBuffIndex > -1 )
			{
				gObjCheckUsedBuffEffect(lpObj,nBuffIndex,&nValue,0);
				if( nValue > 0 )
				{
					AttackDamage += AttackDamage * nValue / 100;
				}
			}
			break;
		case 2:
			nBuffIndex = AT_CSHOP_SCROLL_OF_STRENGTHENER;
			if( nBuffIndex > -1 )
			{
				gObjCheckUsedBuffEffect(lpObj,nBuffIndex,&nValue,0);
				if( nValue > 0 )
				{
					AttackDamage += AttackDamage * nValue / 100;
				}
			}

			nValue = 0;
			nBuffIndex = 0x7B;

			if( nBuffIndex > -1 )
			{
				gObjCheckUsedBuffEffect(lpObj,nBuffIndex,&nValue,0);
				if( nValue > 0 )
				{
					AttackDamage += AttackDamage * nValue / 100;
				}
			}
			break;
		default:
			nBuffIndex = -1;
			break;
		}

		if( g_bAbilityDebug == TRUE )
		{
			if( MsgDamage == 3 || MsgDamage == 2)
			{
				char szTmpMsg[256];
				sprintf(szTmpMsg,"%s AttackDamage: %d %d",(MsgDamage == 3)?"CRITICAL":"EXCELLENT",
					attackdamagebefore,AttackDamage);
				GCServerMsgStringSend(szTmpMsg,lpObj->m_Index,1);
			}
		}

		if ( bAllMiss )
		{
			AttackDamage = ( AttackDamage * 30 ) / 100;
		}

		if ( lpTargetObj->DamageMinus )
		{
			int beforeDamage = AttackDamage;
			AttackDamage -= ( ( AttackDamage * (int)lpTargetObj->DamageMinus) / 100 );
		}

		int tlevel = (lpObj->Level+lpObj->m_nMasterLevel) / 10;

		if ( AttackDamage < tlevel )
		{
			if ( tlevel < 1 )
			{
				tlevel = 1;
			}

			AttackDamage = tlevel;
		}

		if ( lpTargetObj->m_SkillNumber == 18 )
		{
			if ( AttackDamage > 1 )
			{
				AttackDamage >>= 1;
			}
		}

		gObjSpriteDamage(lpTargetObj, AttackDamage);

		if ( gObjSatanSprite(lpObj) == TRUE )
		{
			if( lpObj->Class == CLASS_MONK )
			{
				if( skill == 263 ||
					skill == 269 ||
					skill == 262 )
				{
					lpObj->Life -= 4.0f;
				}
				else if( skill == 265 )
				{
					lpObj->Life -= 100.0f;
				}
				else if( skill == 260 ||
					skill == 261 ||
					skill == 264 )
				{
					lpObj->Life -= 2.0f;
				}
				else if( skill == 270 )
				{
					lpObj->Life -= 2.0f;
				}
				else
				{
					lpObj->Life -= 3.0f;
				}
			}
			else
			{
				lpObj->Life -= 3.0f;
			}

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * 13 / 10;
			}

			GCReFillSend(lpObj->m_Index,(int)lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjAngelSprite(lpTargetObj) == TRUE )
		{
			if ( AttackDamage > 1 )
			{
				float  damage = (AttackDamage * 8) / 10.0f;
				AttackDamage = (int)damage;
			}
		}
		
		
		if ( gObjDemonSprite(lpTargetObj) == TRUE )
		{
			if ( AttackDamage > 1 )
			{
				float damage = AttackDamage * 7 / 10.0f;
				AttackDamage = (int)damage;
			}
		}

		if ( (lpObj->pInventory[10].IsItem() &&
			 lpObj->pInventory[10].m_Type == ITEMGET(13,76) &&
			 lpObj->pInventory[10].m_Durability > 0.0f) || 

			 (lpObj->pInventory[11].IsItem() &&
			 lpObj->pInventory[11].m_Type == ITEMGET(13,76) &&
			 lpObj->pInventory[11].m_Durability > 0.0f) )
		{
			AttackDamage += 30;
		}

		if ( gObjWingSprite(lpObj) == TRUE )
		{
			CItem * Wing = &lpObj->pInventory[7];

			if ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF )
			{
				lpObj->Life -= 1.0f;
			}
			else if ( lpObj->Class == CLASS_SUMMONER )
			{
				lpObj->Life -= 1.0f;
			}
			else if( lpObj->Class == CLASS_MONK )
			{
				if( skill == 263 ||
					skill == 269 ||
					skill == 262 )
				{
					lpObj->Life -= 4.0f;
				}
				else if( skill == 265 )
				{
					lpObj->Life -= 100.0f;
				}
				else if( skill == 260 ||
					skill == 261 ||
					skill == 264 )
				{
					lpObj->Life -= 2.0f;
				}
				else if( skill == 270 )
				{
					lpObj->Life -= 2.0f;
				}
				else
				{
					lpObj->Life -= 3.0f;
				}
			}
			else
			{
				lpObj->Life -= 3.0f;
			}

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				if( Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,40) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level*2 + 139) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12, 262) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level + 127) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12, 263) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level + 133) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12, 264) || Wing->m_Type == ITEMGET(12,265) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level + 135) / 100;
				}
				else if( Wing->m_Type == ITEMGET(13,30) || Wing->m_Type == ITEMGET(12,130) || Wing->m_Type == ITEMGET(12,135) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level*2 + 120) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12,41) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level*2 + 112) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12,42) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level + 132) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12,43) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level*2 + 139) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12,49) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level*2 + 120) / 100;
				}
				else if( Wing->m_Type == ITEMGET(12,50) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level*2 + 139) / 100;
				}
				else if( Wing->m_Type > ITEMGET(12,2) )
				{
					AttackDamage = AttackDamage * (Wing->m_Level + 132) / 100;
				}
				else
				{
					AttackDamage = AttackDamage * (Wing->m_Level*2 + 112) / 100;
				}
			}

			GCReFillSend(lpObj->m_Index,(int)lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		int RestoreHP = 0;
		int RestoreMP = 0;
		int RestoreSD = 0;

		if ( gObjWingSprite(lpTargetObj) == TRUE )
		{
			CItem * Wing = &lpTargetObj->pInventory[7];

			if ( AttackDamage > 1 )
			{
				if( (Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,40)) ||
					 Wing->m_Type == ITEMGET(12,43) || Wing->m_Type == ITEMGET(12,50) )
				{
					if( Wing->m_Type == ITEMGET(12,40) )
					{
						float damage = AttackDamage * (76 - Wing->m_Level*2) / 100.0f;
						AttackDamage = (int)damage;
					}
					else if( Wing->m_Type == ITEMGET(12,50) )
					{
						float damage = AttackDamage * (76 - Wing->m_Level*2) / 100.0f;
						AttackDamage = (int)damage;
					}
					else
					{
						float damage = AttackDamage * (61 - Wing->m_Level*2) / 100.0f;
						AttackDamage = (int)damage;
					}

					BYTE ExcOp = lpTargetObj->pInventory[7].m_NewOption;

					if( (ExcOp&4) == 4 )
						RestoreHP += 5;
					
					if( (ExcOp&8) == 8 )
						RestoreMP += 5;
				}
				else if( Wing->m_Type >= ITEMGET(12,262) && Wing->m_Type <= ITEMGET(12,265) )
				{
					float damage = AttackDamage * (70 - Wing->m_Level*2) / 100.0f;
					AttackDamage = (int)damage;

					if( rand()%100 < 5 )
					{
						BYTE ExcOp = lpTargetObj->pInventory[7].m_NewOption;

						if( (ExcOp&2) == 2 )
						{
							gObjAddMsgSendDelay(lpTargetObj,13,lpObj->m_Index,100,0);
						}
					}
				}
				else if( Wing->m_Type == ITEMGET(12,49) )
				{
					float damage = AttackDamage * (90 - Wing->m_Level*2) / 100.0f;
					AttackDamage = (int)damage;
				}
				else if( Wing->m_Type == ITEMGET(12,130) || Wing->m_Type == ITEMGET(12,135) )
				{
					float damage = AttackDamage * (80 - Wing->m_Level*2) / 100.0f;
					AttackDamage = (int)damage;
				}
				else if( Wing->m_Type == ITEMGET(13,30) )
				{
					float damage = AttackDamage * (90 - Wing->m_Level) / 100.0f;
					AttackDamage = (int)damage;
				}
				else if( Wing->m_Type > ITEMGET(12,2) )
				{
					float damage = AttackDamage * (75 - Wing->m_Level*2) / 100.0f;
					AttackDamage = (int)damage;
				}
				else
				{
					float damage = AttackDamage * (88 - Wing->m_Level*2) / 100.0f;
					AttackDamage = (int)damage;
				}
			}
		}

		if( lpTargetObj->Type == OBJ_USER )
		{
			//RestoreHP += (int)lpTargetObj->m_MasterSkillEffects->m_RestoreFullLife;
			//RestoreMP += (int)lpTargetObj->m_MasterSkillEffects->m_RestoreFullMana;
			//RestoreSD += (int)lpTargetObj->m_MasterSkillEffects->m_RestoreFullShield;

			if( RestoreHP && rand()%100 < RestoreHP )
			{
				gObjAddMsgSendDelay(lpTargetObj,13,lpObj->m_Index,100,0);
			}
			else if( RestoreMP && rand()%100 < RestoreMP )
			{
				gObjAddMsgSendDelay(lpTargetObj,14,lpObj->m_Index,100,0);
			}
			else if( RestoreSD && rand()%100 < RestoreSD )
			{
				gObjAddMsgSendDelay(lpTargetObj,15,lpObj->m_Index,100,0);
			}
		}

		if ( gObjDenorantSprite(lpObj ) )
		{
			lpObj->Life -= 1.0f;

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * 115 / 100;
			}

			GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjDenorantSprite(lpTargetObj ) )
		{
			CItem * Dinorant = &lpTargetObj->pInventory[8];
			int dinorantdecdamage = 90 - Dinorant->IsDinorantReduceAttackDamaege();
			lpObj->Life -= 1.0f;

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * dinorantdecdamage / 100;
			}

			GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjDarkHorse(lpTargetObj ) )
		{
			CItem * Darkhorse = &lpTargetObj->pInventory[8];
			int decdamage = 100 - ((Darkhorse->m_PetItem_Level + 30) / 2 );

			lpTargetObj->Life -= 1.0f;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * decdamage / 100;
			}

			GCReFillSend(lpTargetObj->m_Index, (int)lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
		}

		if ( lpTargetObj->Live )
		{
			switch ( skill )
			{
				case AT_SKILL_SWORD1:
				case AT_SKILL_SWORD2:
				case AT_SKILL_SWORD3:
				case AT_SKILL_SWORD4:
				case AT_SKILL_SWORD5:
				case AT_SKILL_WHEEL:
				case AT_SKILL_BLOWOFFURY:
				case AT_SKILL_STRIKE:
				case AT_SKILL_RUSH:
				case AT_SKILL_KNIGHTDINORANT:
				case AT_SKILL_DEFENSEDOWN:
				case AT_SKILL_ONE_FLASH:
					if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
					{
						AttackDamage *= 2;
					}
					else if( lpObj->Class == CLASS_MONK )
					{
						AttackDamage *= 2;
					}
					else
					{
						AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100;
					}
					break;
				case AT_SKILL_KNIGHTSPEAR:
					if ( lpObj->pInventory[8].m_Type == ITEMGET(13,3) ||
						 lpObj->pInventory[8].m_Type == ITEMGET(13,2) ||
						 lpObj->pInventory[8].m_Type == ITEMGET(13,37) )
					{
						if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
						{
							AttackDamage *= 2;
						}
						else
						{
							AttackDamage = ( AttackDamage * ( Energy / 10 + 200 )  ) / 100;
						}
					}
					break;
				case AT_SKILL_SWORD6:
					AttackDamage *= 2;
					break;
				case AT_SKILL_DEEPIMPACT:
				case AT_SKILL_ELFHARDEN:
				case AT_SKILL_PENETRATION:
					AttackDamage *= 2;
					break;
				case AT_SKILL_SPEAR:
				case AT_SKILL_FIREBURST:
				case AT_SKILL_DARKHORSE_ATTACK:
				case AT_SKILL_ELECTRICSPARK:
				case AT_SKILL_SPACE_SPLIT:
				case AT_SKILL_FIRESCREAM:
					AttackDamage = ( AttackDamage * ( ( ( lpObj->Energy + lpObj->AddEnergy ) / 20 + 200 ) ) ) / 100;
					break;

				case AT_SKILL_FENRIR_ATTACK:
					{
						int iDamageInc = lpObj->Level - 300;
						iDamageInc += lpObj->m_nMasterLevel;

						if ( iDamageInc < 0 )
							iDamageInc = 0;

						iDamageInc /= 5;
						AttackDamage = ( AttackDamage * ( iDamageInc + 200 ) ) / 100;
					}
					break;
				case 216:
					// rename to SplashDamage
					gObjUseSkill.SkillEletrictSurgeProc(lpObj,lpTargetObj,216,AttackDamage,1,50);
					break;
				case 214:
					{
						int RestoreHP =0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							RestoreHP = (int)(float(lpObj->Energy / 15) + float(lpTargetObj->Level / 2.5f));
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							RestoreHP = AttackDamage * 10 / 100 + lpObj->Energy / 23;
						}
	
						int totalhp = (int)(lpObj->Life + RestoreHP);
	
						if( totalhp > lpObj->MaxLife + lpObj->AddLife )
						{
							lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
						}
						else	
						{
							lpObj->Life = (float)totalhp;
						}
	
						GCReFillSend(lpObj->m_Index,(int)lpObj->Life,0xFF,0,lpObj->iShield);
					}	
					break;
				case 215:
					switch( bCombo )
					{
					case 1:
						break;
					case 2:
						AttackDamage = AttackDamage * 70 / 100;
						break;
					case 3:
						AttackDamage = AttackDamage * 50 / 100;
						break;
					default:
						return FALSE;
					}

					bCombo = FALSE;
					break;
				case 223:
					{
						int Damage;
						if( lpObj->m_MPSkillOpt.MpsFireTomeMastery > 0 )
						{
							Damage = AttackDamage * (60.0f+lpObj->m_MPSkillOpt.MpsFireTomeMastery) / 100.0f;
						}
						else
						{
							Damage = AttackDamage * 60.0f / 100.0f;
						}

						gObjAddBuffEffect(lpTargetObj,75,18,Damage,0,0,5);
					}
					break;
				case 224:
					{
						int Damage = AttackDamage * 60 / 100;
						gObjAddBuffEffect(lpTargetObj,74,18,Damage,0,0,5);
					}
					break;
				case 219:
					{
						int Rate = 0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							Rate = g_SkillAdditionInfo.m_SleepRateStartVsMob + lpObj->Energy / g_SkillAdditionInfo.m_SleepRateDivVsMob + lpObj->m_Curse / 6;

							if(g_SkillAdditionInfo.m_SleepRateMaxVsMob	!= 0)
							{
								if(Rate > g_SkillAdditionInfo.m_SleepRateMaxVsMob)
								{
									Rate = g_SkillAdditionInfo.m_SleepRateMaxVsMob;
								}
							}
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							Rate = g_SkillAdditionInfo.m_SleepRateStartVsUser + lpObj->Energy / g_SkillAdditionInfo.m_SleepRateDivVsUser + lpObj->m_Curse / 6;

							if(g_SkillAdditionInfo.m_SleepRateMaxVsUser	!= 0)
							{
								if(Rate > g_SkillAdditionInfo.m_SleepRateMaxVsUser)
								{
									Rate = g_SkillAdditionInfo.m_SleepRateMaxVsUser;
								}
							}
						}

						if( Rate < rand()%100)
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
							return FALSE;
						}

						int SleepTime = 0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							SleepTime = (g_SkillAdditionInfo.m_SleepTimeStartVsMob + lpObj->Energy /g_SkillAdditionInfo.m_SleepTimeDivVsMob) - lpTargetObj->Level / 20;

							if(g_SkillAdditionInfo.m_SleepTimeMaxVsMob != 0)
							{
								if(SleepTime > g_SkillAdditionInfo.m_SleepTimeMaxVsMob)
								{
									SleepTime = g_SkillAdditionInfo.m_SleepTimeMaxVsMob;
								}
							}
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							SleepTime = g_SkillAdditionInfo.m_SleepTimeStartVsUser + ((lpObj->Energy /g_SkillAdditionInfo.m_SleepTimeDivVsUser) + (lpObj->Level - lpTargetObj->Level) / 100);

							if(g_SkillAdditionInfo.m_SleepTimeMaxVsUser != 0)
							{
								if(SleepTime > g_SkillAdditionInfo.m_SleepTimeMaxVsUser)
								{
									SleepTime = g_SkillAdditionInfo.m_SleepTimeMaxVsUser;
								}
							}
						}

						if( SleepTime < 1 )
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
							return FALSE;
						}

						if( gObjCheckPowerfulEffect(lpTargetObj,0x48,SleepTime,0) == TRUE )
						{
							GCMagicAttackNumberSend(lpObj,219,lpTargetObj->m_Index,0);
							return FALSE;
						}

						gObjAddBuffEffect(lpTargetObj,72,25,SleepTime,0,0,SleepTime);
						return TRUE;
					}
					break;
				case 221:
					{
						int Rate = 0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							Rate = 32 + lpObj->Energy / 50 + lpObj->m_Curse / 6;
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							Rate = 17 + lpObj->Energy / 50 + lpObj->m_Curse / 6;
						}

						if( Rate >= rand() % 100 )
						{
							int DecValue = 0;
							int DecTime = 0;

							if( lpTargetObj->Type == OBJ_MONSTER )
							{
								int MaxEnergy = lpObj->Energy;

								if( lpObj->Energy > 4000 )
								{
									MaxEnergy = 4000;
								}

								DecValue = g_SkillAdditionInfo.m_WeaknessEffectStartVsMob + MaxEnergy / g_SkillAdditionInfo.m_WeaknessEffectDivVsMob;

								if(g_SkillAdditionInfo.m_WeaknessEffectMaxVsMob != 0)
								{
									if(DecValue > g_SkillAdditionInfo.m_WeaknessEffectMaxVsMob)
									{
										DecValue = g_SkillAdditionInfo.m_WeaknessEffectMaxVsMob;
									}
								}

								DecTime = g_SkillAdditionInfo.m_WeaknessTimeStartVsMob + MaxEnergy / g_SkillAdditionInfo.m_WeaknessTimeDivVsMob - lpTargetObj->Level / 20;

								if(g_SkillAdditionInfo.m_WeaknessTimeMaxVsMob != 0)
								{
									if(DecTime > g_SkillAdditionInfo.m_WeaknessTimeMaxVsMob)
									{
										DecTime = g_SkillAdditionInfo.m_WeaknessTimeMaxVsMob;
									}
								}
							}
							else if( lpTargetObj->Type == OBJ_USER )
							{
								int MaxEnergy = lpObj->Energy;

								if( lpObj->Energy > 4000 )
								{
									MaxEnergy = 4000;
								}

								DecValue = g_SkillAdditionInfo.m_WeaknessEffectStartVsUser + MaxEnergy / g_SkillAdditionInfo.m_WeaknessEffectDivVsUser;

								if(g_SkillAdditionInfo.m_WeaknessEffectMaxVsUser != 0)
								{
									if(DecValue > g_SkillAdditionInfo.m_WeaknessEffectMaxVsUser)
									{
										DecValue = g_SkillAdditionInfo.m_WeaknessEffectMaxVsUser;
									}
								}

								DecTime = g_SkillAdditionInfo.m_WeaknessTimeStartVsUser + MaxEnergy / g_SkillAdditionInfo.m_WeaknessTimeDivVsUser + (lpObj->Level - lpTargetObj->Level) / 150;

								if(g_SkillAdditionInfo.m_WeaknessTimeMaxVsUser != 0)
								{
									if(DecTime > g_SkillAdditionInfo.m_WeaknessTimeMaxVsUser)
									{
										DecTime = g_SkillAdditionInfo.m_WeaknessTimeMaxVsUser;
									}
								}
							}

							if( DecValue > 100 )
								DecValue = 100;

							if( DecTime < 1 )
							{
								GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
								return FALSE;
							}

							if( gObjCheckPowerfulEffect(lpObj,AT_WEAKNESS,DecValue,0) == TRUE )
							{
								GCMagicAttackNumberSend(lpObj,skill,lpTargetObj->m_Index,0);
								return FALSE;
							}

							gObjAddBuffEffect(lpTargetObj,AT_WEAKNESS,29,DecValue,0,0,DecTime);
							return TRUE;
						}
						else
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
							return FALSE;
						}
					}
					break;
				case 222:
					{
						int Rate = 0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							Rate = 32 + lpObj->Energy / 50 + lpObj->m_Curse / 6;
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							Rate = 17 + lpObj->Energy / 50 + lpObj->m_Curse / 6;
						}

						if( Rate >= rand()%100 )
						{
							int DecValue = 0;
							int DecTime = 0;

							if( lpTargetObj->Type == OBJ_MONSTER )
							{
								int MaxEnergy = lpObj->Energy;

								if( lpObj->Energy > 4000 )
								{
									MaxEnergy = 4000;
								}

								DecValue = g_SkillAdditionInfo.m_InnovationEffectStartVsMob + MaxEnergy / g_SkillAdditionInfo.m_InnovationEffectDivVsMob;

								if(g_SkillAdditionInfo.m_InnovationEffectMaxVsMob != 0)
								{
									if(DecValue > g_SkillAdditionInfo.m_InnovationEffectMaxVsMob)
									{
										DecValue = g_SkillAdditionInfo.m_InnovationEffectMaxVsMob;
									}
								}

								DecTime = g_SkillAdditionInfo.m_InnovationTimeStartVsMob + MaxEnergy / g_SkillAdditionInfo.m_InnovationTimeDivVsMob - lpTargetObj->Level / 20;

								if(g_SkillAdditionInfo.m_InnovationTimeMaxVsMob != 0)
								{
									if(DecTime > g_SkillAdditionInfo.m_InnovationTimeMaxVsMob)
									{
										DecTime = g_SkillAdditionInfo.m_InnovationTimeMaxVsMob;
									}
								}
							}
							else if( lpTargetObj->Type == OBJ_USER )
							{
								int MaxEnergy = lpObj->Energy;

								if( lpObj->Energy > 4000 )
								{
									MaxEnergy = 4000;
								}

								DecValue = g_SkillAdditionInfo.m_InnovationEffectStartVsUser + MaxEnergy / g_SkillAdditionInfo.m_InnovationEffectDivVsUser;

								if(g_SkillAdditionInfo.m_InnovationEffectMaxVsUser	=! 0)
								{
									if(DecValue > g_SkillAdditionInfo.m_InnovationEffectMaxVsUser)
									{
										DecValue = g_SkillAdditionInfo.m_InnovationEffectMaxVsUser;
									}
								}

								DecTime = g_SkillAdditionInfo.m_InnovationTimeStartVsUser + MaxEnergy / g_SkillAdditionInfo.m_InnovationTimeDivVsUser + (lpObj->Level - lpTargetObj->Level) / 150;

								if(g_SkillAdditionInfo.m_InnovationTimeMaxVsUser != 0)
								{
									if(DecTime > g_SkillAdditionInfo.m_InnovationTimeMaxVsUser)
									{
										DecTime = g_SkillAdditionInfo.m_InnovationTimeMaxVsUser;
									}
								}
							}

							if( DecValue > 100 )
								DecValue = 0;

							if( DecTime < 1 )
							{
								GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
								return FALSE;
							}

							if( gObjCheckPowerfulEffect(lpObj,AT_INNOVATION,DecValue,0) == TRUE )
							{
								GCMagicAttackNumberSend(lpObj,skill,lpTargetObj->m_Index,0);
								return FALSE;
							}

							gObjAddBuffEffect(lpTargetObj,AT_INNOVATION,30,DecValue,0,0,DecTime);
							return TRUE;
						}
						else
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
							return FALSE;
						}
					}
					break;
				case 225:

					if( gObjCheckUsedBuffEffect(lpTargetObj,56) == FALSE )
					{
						lpTargetObj->lpAttackObj = lpObj;
						lpTargetObj->DelayActionTime = 1000;
						lpTargetObj->DelayLevel = 1;

						gObjAddBuffEffect(lpTargetObj,56,20,0,0,0,2);
					}
					break;
				case 230:
					gObjAddBuffEffect(lpTargetObj,85,0,0,0,0,1);
					break;
				case 232:
					lpTargetObj->lpAttackObj = lpObj;
					lpTargetObj->DelayActionTime = 1000;
					lpTargetObj->DelayLevel = 1;
						
					gObjAddBuffEffect(lpTargetObj, 86, 20,0,0,0,10);

					AttackDamage = AttackDamage*(200 + Energy/10)/100;
					break;
				case 236:
					AttackDamage *= 2;
					gObjAddBuffEffect(lpTargetObj,AT_FLAME_STRIKE,0,0,0,0,1);
					break;
				case 237:
					gObjAddBuffEffect(lpTargetObj,AT_GIGANTIC_STORM,0,0,0,0,1);
					break;
				case 238:
					AttackDamage = AttackDamage*(200 + Energy/25)/100;

					if( lpMagic->m_Skill == 238 &&
						lpObj->Class == 561 )	//Medusa, 1.01.00
					{
						if( gObjCheckUsedBuffEffect(lpTargetObj,AT_ICE) )
						{
							lpObj->DelayActionTime = 800;
							lpObj->DelayLevel = 1;
							lpObj->lpAttackObj = lpObj;

							if( rand()%100 < 50 )
								gObjAddBuffEffect(lpTargetObj,AT_STUN,0,0,0,0,5);

							AttackDamage = 1000;
						}
					}
					break;
				case 250:
					AttackDamage *= 2;
					break;
				case 251:
					AttackDamage *= 2.2f;
					break;
				case 252:
					AttackDamage *= 2.3f;
					break;
				case 253:
					AttackDamage *= 2.5;
					break;
				case 239:
					gObjBackSpring(lpTargetObj,lpObj);
					break;
				case 260:
				case 261:
					AttackDamage = AttackDamage * (Vitality / 10 + 50) / 100.0f;
					break;
				case 262:
					AttackDamage = (AttackDamage + Vitality / 10) * (Vitality / 10 + 50) / 100.0f;
					break;
				case 263:
					AttackDamage = (Dexterity / 8 + AttackDamage + Energy / 10) * (Dexterity / 8 + Energy / 10 + 100) / 100.0f;
					break;
				case 264:
					AttackDamage = (AttackDamage + Energy / 10) * (Energy / 10 + 50) / 100.0f;
					break;
				case 265:
					if( lpTargetObj->Type == OBJ_USER )
					{
						AttackDamage = AttackDamage * (Energy / 10 + 50) / 100.0f;
					}
					else
					{
						AttackDamage = (AttackDamage * (Energy / 10 + 50) / 100.0f + 100.0f) * 3.0f;
					}
					break;
				case 269:
				case 270:
					AttackDamage = AttackDamage * (Vitality / 10 + 50) / 100.0f;
					break;

		/*		
				case 262:
				case 270:
				case AT_MSKILL_RF_KILLBLOW1:
				case AT_MSKILL_RF_KILLBLOW2:
				case AT_MSKILL_RF_UPERCUT1:
				case AT_MSKILL_RF_UPERCUT2:
				case AT_MSKILL_RF_CHAINDRIVE1:
					AttackDamage = AttackDamage* (50 + (Vitality/10) ) / 100;

					if( (lpMagic->m_Skill == 262 ||
						lpMagic->m_Skill == AT_MSKILL_RF_CHAINDRIVE1 ) && rand()%8 == FALSE )
					{
						if( gObjCheckUsedBuffEffect(lpTargetObj,56) == FALSE )
						{
							lpTargetObj->lpAttackObj = lpObj;
							lpTargetObj->DelayActionTime = 1000;
							lpTargetObj->DelayLevel = 1;

							gObjAddBuffEffect(lpTargetObj,56,20,0,0,0,0,0,10);
						}
					}
					else if( lpMagic->m_Skill == AT_MSKILL_RF_KILLBLOW2 )
					{
						if( rand()%10000 < lpObj->m_MasterSkillEffects->m_KillBlowDecreaseDefRate )
						{
							gObjAddBuffEffect(lpTargetObj,BUFF_DECREASE_DEFRATE,ADD_OPTION_DECDEFRATE,10,0,0,0,0,5);
						}
					}
					else if( lpMagic->m_Skill == AT_MSKILL_RF_UPERCUT2 )
					{
						if( rand()%10000 < lpObj->m_MasterSkillEffects->m_UpercutDecreaseDefRate )
						{
							gObjAddBuffEffect(lpTargetObj,BUFF_DECREASE_DEFRATE,ADD_OPTION_DECDEFRATE,10,0,0,0,0,5);
						}
					}
					break;
				case 263:
				case AT_MSKILL_RF_DARKSIDE1:
					AttackDamage = AttackDamage* (100 + (Energy/10) + (Dexterity/8) ) / 100;
					break;
				case 264:
				case AT_MSKILL_RF_DRAGONRORE1:
					AttackDamage = AttackDamage* (50 + (Energy/10) ) / 100;
					break;
				case 265:
					AttackDamage = AttackDamage* (50 + (Energy/10) ) / 100;
					AttackDamage *= 2;

					if( rand()%2 )
					{
						AttackDamage *= 2;
					}
					break;

				case AT_MSKILL_DW_EARTHPRISON:
				case AT_MSKILL_DW_EARTHPRISON1:
					if( !magicsend )	{
						if( rand()%2 )
							gObjAddBuffEffect(lpTargetObj,BUFF_EARTHBINDS,0,0,0,0,0,0,5);
					}
					break;*/

			}

			if( g_MasterSkillSystem.CheckMasterLevelSkill(skill) )
			{
				float iSkillValue = g_MasterSkillSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill),lpMagic->m_Level);
				float fPrevValue = 0;

				int iMLSBase = g_MasterSkillSystem.GetBaseMasterLevelSkill(skill);

				switch( iMLSBase )
				{
				case 330:
				case 332:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100;
					break;
				case 481:
					AttackDamage *= 2;
					break;
				case 326:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100.0f;
					break;
				case 479:
					AttackDamage *= 2;
					break;
				case 327:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100.0f;
					break;
				case 328:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100.0f;
					break;
				case 329:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100.0f;
					break;
				case 337:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100.0f;
					break;
				case 416:
					AttackDamage *= 2;
					break;
				case 424:
					AttackDamage *= 2;
					break;
				case 516:
					AttackDamage = ( AttackDamage * ( ( ( lpObj->Energy + lpObj->AddEnergy ) / 20 + 200 ) ) ) / 100;

					if( iSkillValue > 0 )
					{
						float fPercent = rand()%100;

						if( iSkillValue < fPercent )
						{
							gObjAddBuffEffect(lpTargetObj,AT_STUN,0,0,0,0,2);
							gObjSetPosition(lpTargetObj->m_Index,lpTargetObj->X,lpTargetObj->Y);
						}
					}
					break;
				case 514:
					AttackDamage = ( AttackDamage * ( ( ( lpObj->Energy + lpObj->AddEnergy ) / 20 + 200 ) ) ) / 100;

					if( iSkillValue > 0 )
					{
						float fPercent = rand()%100;

						if( fPercent < iSkillValue )
						{
							gObjAddBuffEffect(lpTargetObj,AT_STUN,0,0,0,0,2);
							gObjSetPosition(lpTargetObj->m_Index,lpTargetObj->X,lpTargetObj->Y);
						}
					}
					break;
				case 509:
					AttackDamage = ( AttackDamage * ( ( ( lpObj->Energy + lpObj->AddEnergy ) / 20 + 200 ) ) ) / 100;
					break;
				case 512:
					AttackDamage = ( AttackDamage * ( ( ( lpObj->Energy + lpObj->AddEnergy ) / 20 + 200 ) ) ) / 100;
					break;
				case 508:
					AttackDamage = ( AttackDamage * ( ( ( lpObj->Energy + lpObj->AddEnergy ) / 20 + 200 ) ) ) / 100;
					break;
				case 336:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100;
					break;
				case 331:
					AttackDamage = ( AttackDamage * ( 200 + ( Energy / 10 ) ) ) / 100;
					break;
				case 493:
					AttackDamage *= 2;
					break;
				case 482:
					AttackDamage *= 2;
					break;
				case 518:
					AttackDamage = ( AttackDamage * ( ( ( lpObj->Energy + lpObj->AddEnergy ) / 20 + 200 ) ) ) / 100;
					break;
				case 454:
					{
						int skillSuccessRate = 0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							skillSuccessRate = 20 + lpObj->Energy/30 + lpObj->m_Curse/6;
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							skillSuccessRate = 15 + lpObj->Energy/37 + lpObj->m_Curse/6;
						}

						skillSuccessRate += iSkillValue;

						if( skillSuccessRate < rand()%100 )
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
							return 0;
						}

						int nEffectTime = 0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							nEffectTime = (5 + lpObj->Energy /100) - lpTargetObj->Level / 20;
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							nEffectTime = 4 + ((lpObj->Energy /250) + (lpObj->Level - lpTargetObj->Level) / 100);
						}

						if( nEffectTime < 1 )
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index,0,0,0,0);
							return FALSE;
						}

						if( gObjCheckPowerfulEffect(lpTargetObj,72,nEffectTime,0) == TRUE )
						{
							GCMagicAttackNumberSend(lpObj,219,lpTargetObj->m_Index,0);
							return FALSE;
						}

						gObjAddBuffEffect(lpTargetObj,72,25,nEffectTime,0,0,nEffectTime);
						return TRUE;
					}
					break;
				case 455:
					switch( bCombo )
					{
					case 1:
						break;
					case 2:
						AttackDamage = AttackDamage * 70 / 100;
						break;
					case 3:
						AttackDamage = AttackDamage * 50 / 100;
						break;
					default:
						return FALSE;
					}

					bCombo = FALSE;
					break;
				case 456:
					gObjAddBuffEffect(lpTargetObj,85,0,0,0,0,1);
					break;
				case 458:
					{
						int nAddHP = 0;

						if( lpTargetObj->Type == OBJ_MONSTER )
						{
							nAddHP = (int)(float(lpObj->Energy / 15) + float(lpTargetObj->Level / 2.5f));
							nAddHP += iSkillValue;
						}
						else if( lpTargetObj->Type == OBJ_USER )
						{
							nAddHP = AttackDamage * 10 / 100 + lpObj->Energy / 23;
							nAddHP += iSkillValue;
						}
	
						int tmpLife = (int)(lpObj->Life + nAddHP);
	
						if( tmpLife > lpObj->MaxLife + lpObj->AddLife )
						{
							lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
						}
						else	
						{
							lpObj->Life = (float)tmpLife;
						}
	
						GCReFillSend(lpObj->m_Index,(int)lpObj->Life,0xFF,0,lpObj->iShield);
					}
					break;

				case 551:
				case 554:
				case 552:
				case 555:
					AttackDamage = AttackDamage * (Vitality / 10 + 50) / 100.0f;
					break;
				case 558:
				case 562:
					AttackDamage = (AttackDamage+(Vitality/10))*(50+(Vitality/10))/100.0f;
					break;
				case 559:
				case 563:
					AttackDamage = (AttackDamage+(Vitality/8)+(Energy/10))*(100 + (Vitality/8) + (Energy/10))/100.0f;
					break;
				case 560:
				case 561:
					AttackDamage = (AttackDamage + Energy / 10) * (Energy / 10 + 50) / 100.0f;
					break;
				}
			}

			if ( lpObj->pInventory[1].GetDetailItemType() == 10 &&
				 lpObj->pInventory[1].m_Special[0] == skill )
			{
				if( MagicDamageC.GetSkillAttr(skill) == R_FIRE )
				{
					if( lpObj->m_MPSkillOpt.MpsFireTomeStrength > 0.0f )
					{
						AttackDamage += lpObj->m_MPSkillOpt.MpsFireTomeStrength;
					}
				}
				else if( MagicDamageC.GetSkillAttr(skill) == R_WIND )
				{
					if( lpObj->m_MPSkillOpt.MpsWindTomeStrength > 0.0f )
					{
						AttackDamage += lpObj->m_MPSkillOpt.MpsWindTomeStrength;
					}

					if( lpObj->m_MPSkillOpt.MpsWindTomeMastery > 0.0f )
					{
						float fPercent = rand()%100;

						if( fPercent < lpObj->m_MPSkillOpt.MpsWindTomeMastery )
						{
							gObjAddBuffEffect(lpTargetObj,AT_STUN,0,0,0,0,3);
							gObjSetPosition(lpTargetObj->m_Index,lpTargetObj->X,lpTargetObj->Y);
						}
					}
				}
				else if( MagicDamageC.GetSkillAttr(skill) == R_LIGHTNING )
				{
					if( lpObj->m_MPSkillOpt.MpsLightTomeStrength > 0.0f )
					{
						AttackDamage += lpObj->m_MPSkillOpt.MpsLightTomeStrength;
					}

					if( lpObj->m_MPSkillOpt.MpsLightTomeMastery > 0.0f )
					{
						float fPercent = rand()%100;

						if( fPercent < lpObj->m_MPSkillOpt.MpsLightTomeMastery )
						{
							gObjBackSpring2(lpTargetObj,lpObj,2);
						}
					}
				}
			}

			if ( skill == 0 )
			{
				if ( lpObj->pInventory[8].m_Type == ITEMGET(13, 3) )
				{
					AttackDamage = AttackDamage * 130 / 100;
				}
			}
			
			if ( gObjCheckUsedBuffEffect(lpTargetObj,AT_WIZARD_DEFENSE) == TRUE  && AttackDamage > 0 )
			{
				int iWizardSkillDefense = 0;
				int iManaRate = 0;

				gObjCheckUsedBuffEffect(lpTargetObj,AT_WIZARD_DEFENSE,&iWizardSkillDefense,&iManaRate);

				int iReplacementMana = 0;

				if( iManaRate > 0 )
				{
					iReplacementMana = (lpTargetObj->Mana * iManaRate) / 1000;
				}
				else
				{
					iReplacementMana = (lpTargetObj->Mana * 0.02f);
				}

				if ( iReplacementMana < lpTargetObj->Mana )
				{
					lpTargetObj->Mana -= iReplacementMana;
					float fWizardSkillDefense = iWizardSkillDefense/100.0f;
					int decattackdamage = 0;
					
					if( iWizardSkillDefense > 0 )
					{
						decattackdamage = AttackDamage * fWizardSkillDefense;
					}
					AttackDamage -= decattackdamage;
					ManaChange = TRUE;
				}
			}

			AttackDamage += lpObj->SetOpAddDamage;

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				AttackDamage += lpObj->m_ItemOptionExFor380.OpAddDamage;

				if ( CC_MAP_RANGE(lpObj->MapNumber ) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}

#if defined __ALIEN__ || __WHITE__
			if(byReflect == 0)
			{
				gBalanceSystem.Main(lpObj,lpTargetObj,AttackDamage);
				gItemOption.AttackDamage(lpObj,lpTargetObj,AttackDamage);
			}
#endif

			if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
				&& lpObj->Type == OBJ_USER
				&& lpTargetObj->Type == OBJ_USER
				&& lpObj->MapNumber == MAP_INDEX_CASTLESIEGE
				&& lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if(lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
				{
					AttackDamage = AttackDamage * 20 / 100;
				}
				else
				{
					AttackDamage = AttackDamage * 40 / 100;
				}
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
			{
				if ( lpTargetObj->Class == 283 )
				{
					if ( gObjCheckUsedBuffEffect(lpObj,AT_POTION_OF_BLESS) == TRUE )
					{
						//AttackDamage = AttackDamage;
					}
					else if ( gObjCheckUsedBuffEffect(lpObj,AT_POTION_OF_SOUL) == TRUE )
					{
						//AttackDamage = AttackDamage;
					}
					else 
					{
						if ( lpObj->m_iAccumulatedDamage > 100 )
						{
							gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
							lpObj->m_iAccumulatedDamage = 0;
						}
						else
						{
							lpObj->m_iAccumulatedDamage += AttackDamage;
						}

						AttackDamage = AttackDamage * 5 / 100;
					}
				}

				if ( lpTargetObj->Class == 277 )
				{
					if ( gObjCheckUsedBuffEffect(lpObj,AT_POTION_OF_BLESS) == TRUE )
					{
						//AttackDamage = AttackDamage;
					}
					else if ( gObjCheckUsedBuffEffect(lpObj,AT_POTION_OF_SOUL) == TRUE )
					{
						//AttackDamage = AttackDamage;
					}
					else 
					{
						if ( lpObj->m_iAccumulatedDamage > 100 )
						{
							gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
							lpObj->m_iAccumulatedDamage = 0;
						}
						else
						{
							lpObj->m_iAccumulatedDamage += AttackDamage;
						}

						AttackDamage = AttackDamage * 5 / 100;
					}
				}
			}

			//AttackDamage += AttackDamage/100.0f * lpObj->m_InfinityArrowDmgMul;

			if ( gObjFenrir( lpObj ) )
			{
				int iIncPercent = lpObj->pInventory[8].IsFenrirIncLastAttackDamage();

				if ( iIncPercent > 0 )
				{
					AttackDamage += AttackDamage * iIncPercent / 100;
				}
			}

			if ( gObjFenrir( lpTargetObj ) )
			{
				int iDecPercent = lpTargetObj->pInventory[8].IsFenrirDecLastAttackDamage();

				if ( iDecPercent > 0 )
				{
					AttackDamage -= AttackDamage * iDecPercent / 100;
				}
			}

			if ( AttackDamage < 0 )
				AttackDamage = 0;

			if ( skill == 76 )
			{
				if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
				{
					if ( AttackDamage > 0 )
					{
						if(lpObj->m_btCsJoinSide == 0 || lpObj->m_btCsJoinSide != lpTargetObj->m_btCsJoinSide)
						{
						int iEquipmentPos = rand()%5 + 2;	// Select and Armor
						CItem * lpEquipment = &lpTargetObj->pInventory[iEquipmentPos];

						if ( lpEquipment && lpEquipment->IsItem() /*&& !lpEquipment->m_bLuckySet*/)
						{
							int ItemPerc = 50;

							if( lpObj->m_MPSkillOpt.MpsDownDur1 != 0.0f )
							{
								ItemPerc += lpObj->m_MPSkillOpt.btMpsDownDur1Level*7;
							}

							if( ItemPerc < 0 )
								ItemPerc = 1;

							int iDurEquipment = (int)(lpEquipment->m_Durability * ItemPerc / 100.0f);

							/*for(int iBuffIndex = 0; iBuffIndex < lpTargetObj->btEffectCount; iBuffIndex++)
							{
								if( lpObj->pEffectInfo[iBuffIndex].btEffectNum == BUFF_TALISMAN_ITEMPROTECT )
								{
									iDurEquipment = lpEquipment->m_Durability;
									break;
								}
							}*/

							if( g_LuckyItemManager.IsLuckyItemEquipment(lpEquipment->m_Type) )
							{
								iDurEquipment = lpEquipment->m_Durability;
							}

							lpEquipment->m_Durability = (float)iDurEquipment;

							if ( lpEquipment->m_Durability < 0.0f )
							{
								lpEquipment->m_Durability = 0.0f;
							}

							GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, (BYTE)lpEquipment->m_Durability, 0);
						}

						}
					}
				}
			}

			if ( !byReflect )
			{
				if( lpObj->m_MPSkillOpt.MpsTwoHandSwordMastery > 0.0f &&
					lpObj->Type == OBJ_USER &&
					lpTargetObj->Type == OBJ_USER )
				{
					AttackDamage += lpObj->m_MPSkillOpt.MpsTwoHandSwordMastery;
				}

				if( lpObj->m_MPSkillOpt.MpsTwoHandStaffMastery > 0.0f &&
					lpObj->Type == OBJ_USER &&
					lpTargetObj->Type == OBJ_USER )
				{
					AttackDamage += lpObj->m_MPSkillOpt.MpsTwoHandStaffMastery;
				}

				if( lpObj->m_MPSkillOpt.MpsCrossbowMastery > 0.0f &&
					lpObj->Type == OBJ_USER &&
					lpTargetObj->Type == OBJ_USER )
				{
					AttackDamage += lpObj->m_MPSkillOpt.MpsCrossbowMastery;
				}

				if( lpObj->m_MPSkillOpt.MpsStickMastery > 0.0f &&
					lpObj->Type == OBJ_USER &&
					lpTargetObj->Type == OBJ_USER )
				{
					AttackDamage += lpObj->m_MPSkillOpt.MpsStickMastery;
				}

				if( lpObj->m_MPSkillOpt.MpsScepterMastery > 0.0f &&
					lpObj->Type == OBJ_USER &&
					lpTargetObj->Type == OBJ_USER )
				{
					AttackDamage += lpObj->m_MPSkillOpt.MpsScepterMastery;
				}
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if( skill == 265 && 
					rand()%100 < (10+Energy/100) )
				{
					bDragonKickSDAttackSuccess = TRUE;
					int nDecreaseSDRate = 10 + Energy/30;

					if( nDecreaseSDRate > 100 )
						nDecreaseSDRate = 100;

					int nDecreaseSD = lpTargetObj->iShield*nDecreaseSDRate/100;
					lpTargetObj->iShield -= nDecreaseSD;
					lpTargetObj->Life -= AttackDamage;
					iTotalShieldDamage += nDecreaseSD;
				}
				else
				{
					iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
					lpTargetObj->iShield -= iTempShieldDamage;
					lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
					iTotalShieldDamage += iTempShieldDamage;
				}

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
			else
			{
				lpTargetObj->Life -= AttackDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
		}
	}	
	else
	{
		if( skill == 216 )
		{
			if( g_ShieldSystemOn ==  TRUE )
			{
				if( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
				{
					if( MissCheckPvP(lpObj,lpTargetObj,skill,skillSuccess,magicsend,bAllMiss, byBarrageCount) == FALSE )
					{
						return FALSE;
					}
				}
				else
				{
					if( MissCheck(lpObj,lpTargetObj,skill,skillSuccess,magicsend,bAllMiss, byBarrageCount) == FALSE )
					{
						return FALSE;
					}
				}
			}
			else
			{
				if( MissCheck(lpObj,lpTargetObj,skill,skillSuccess,magicsend,bAllMiss, byBarrageCount) == FALSE )
				{
					return FALSE;
				}
			}
		}

#if defined __ALIEN__ || __WHITE__
		if(byReflect == 0)
		{
			gBalanceSystem.Main(lpObj,lpTargetObj,AttackDamage);
			gItemOption.AttackDamage(lpObj,lpTargetObj,AttackDamage);
		}
#endif

		if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
			&& lpObj->Type == OBJ_USER
			&& lpTargetObj->Type == OBJ_USER
			&& lpObj->MapNumber == MAP_INDEX_CASTLESIEGE
			&& lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			if(lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
			{
				AttackDamage = AttackDamage * 20 / 100;
			}
			else if(g_ShieldSystemOn == FALSE)
			{
				AttackDamage = AttackDamage * 40 / 100;
			}
		}

		if ( skill != AT_SKILL_EXPLOSION && skill != 216 )
		{
			bDamageReflect = TRUE;
			MsgDamage = 4;
		}

		//AttackDamage += AttackDamage/100.0f * lpObj->m_InfinityArrowDmgMul;

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			if( skill == 265 && 
					rand()%100 < (10+Energy/100) )
			{
				bDragonKickSDAttackSuccess = TRUE;
				int nDecreaseSDRate = 10 + Energy/30;

				if( nDecreaseSDRate > 100 )
					nDecreaseSDRate = 100;

				int nDecreaseSD = lpTargetObj->iShield*nDecreaseSDRate/100;
				lpTargetObj->iShield -= nDecreaseSD;
				lpTargetObj->Life -= AttackDamage;
				iTotalShieldDamage += nDecreaseSD;
			}
			else
			{
				iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
				lpTargetObj->iShield -= iTempShieldDamage;
				lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
				iTotalShieldDamage += iTempShieldDamage;
			}

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
		else
		{
			lpTargetObj->Life -= AttackDamage;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100, 0);
		lpTargetObj->LastAttackerID = lpObj->m_Index;

		if ( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro.IncAgro(lpObj->m_Index, AttackDamage / 50);
		}
	}

	BOOL selfdefense = 0;
	lpCallObj = lpTargetObj;
	
	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( AttackDamage >= 1 )
	{
		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			if ( gObjDuelCheck(lpObj, lpTargetObj) )
			{
				selfdefense = 0;
			}
#ifdef NPVP
			else if( g_NewPVP.IsVulcanusMap(lpObj->MapNumber) || g_NewPVP.IsVulcanusMap(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
#endif
			else if ( CC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
			else if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
#ifdef GENS
			else if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) || gGensSystem.IsMapBattleZone(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
#endif
			else
			{
				selfdefense = 1;
			}

			if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
			{
				selfdefense = FALSE;
			}		
			if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE && lpObj->m_btCsJoinSide > 0)
			{
				selfdefense = FALSE;
			}
		}
		else if ( lpTargetObj->Type == OBJ_MONSTER && lpObj->Type == OBJ_USER )
		{
			if ( lpTargetObj->m_RecallMon >= 0 )
			{
				selfdefense = TRUE;
			}
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			gObjArmorRandomDurDown(lpTargetObj, lpObj);

			if( skill == 333 )
			{
				float fDurDownRate = g_MasterSkillSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill),lpMagic->m_Level);

				if( rand()%100 < fDurDownRate )
				{
					if(lpObj->m_btCsJoinSide == 0 || lpObj->m_btCsJoinSide != lpTargetObj->m_btCsJoinSide)
					{
						int item_num[5];
						item_num[0] = 2;
						item_num[1] = 3;
						item_num[2] = 4;
						item_num[3] = 5;
						item_num[4] = 6;
						int iEquipmentPos = rand()%5;	// Select and Armor
						CItem * lpEquipment = &lpTargetObj->pInventory[item_num[iEquipmentPos]];

						if ( lpEquipment && lpEquipment->IsItem() )
						{
							float iDurDecValue = 0;

							if( lpObj->m_MPSkillOpt.MpsDownDur1 != 0.0f )
							{
								iDurDecValue += lpObj->m_MPSkillOpt.btMpsDownDur1Level/5.0f;
							}

							if( iDurDecValue == 0.0f )
								iDurDecValue = 1.0f;

							int damagemin = lpEquipment->m_BaseDurability/100.0f*(10-iDurDecValue);

							int iDurEquipment = lpEquipment->m_Durability - damagemin;

							/*for(int iBuffIndex = 0; iBuffIndex < lpObj->btEffectCount; iBuffIndex++)
							{
								if( lpObj->pEffectInfo[iBuffIndex].btEffectNum == BUFF_TALISMAN_ITEMPROTECT )
								{
									iDurEquipment = lpEquipment->m_Durability;
									break;
								}
							}*/

							if( g_LuckyItemManager.IsLuckyItemEquipment(lpEquipment->m_Type) )
							{
								iDurEquipment = lpEquipment->m_Durability;
							}

							lpEquipment->m_Durability = (float)iDurEquipment;

							if ( lpEquipment->m_Durability < 0.0f )
							{
								lpEquipment->m_Durability = 0.0f;
							}

							GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, (BYTE)lpEquipment->m_Durability, 0);
						}
					}
				}
			}
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj, AT_ICE_ARROW) == TRUE )	//Ice Arrow
		{
			gObjChangeBuffValidTime(lpTargetObj, AT_ICE_ARROW, -1);

			if ( gObjCheckUsedBuffEffect(lpTargetObj, AT_ICE_ARROW) == FALSE )
			{
				GCMagicCancelSend(lpTargetObj, 51);
			}
		}

		if ( gObjCheckUsedBuffEffect(lpTargetObj, 72) == TRUE )	//Sleep
		{
			gObjRemoveBuffEffect(lpTargetObj, 72);
		}
	}

	if ( lpObj->PartyNumber >= 0 &&
		 lpCallObj->PartyNumber >= 0)
	{
		if( lpObj->PartyNumber == lpCallObj->PartyNumber )
		{
			selfdefense = 0;
		}
	}

	if ( selfdefense == TRUE && bDamageReflect == FALSE )
	{
		if ( !gObjTargetGuildWarCheck(lpObj, lpCallObj) )
		{
			if( lpCallObj->PartyNumber >= 0 )
			{
				int bTmp = 0;
				int PartyNumber = lpCallObj->PartyNumber;

				if( gParty.GetPKPartyPenalty(PartyNumber) < 5 )
				{
					gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
				}
			}
			else
			{
				gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
			}
		}
	}

	if ( lpTargetObj->Class == 275 )	// KUNDUN
	{
		if ( lpTargetObj->m_iMonsterBattleDelay <= 0 )
		{
			if ( (rand()%15) < 1 )
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
				lpTargetObj->m_iMonsterBattleDelay = 10;
				GCActionSend(lpTargetObj, 126, lpTargetObj->m_Index, lpObj->m_Index);
			}
		}
	}

	if ( lpTargetObj->Class == 131 ||BC_STATUE_RANGE(lpTargetObj->Class-132) )
	{
		gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
		gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	}
	else if ( AttackDamage >= 5 )	// To make strong hit
	{
		if ( lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( (rand()%26) == 0 )
			{
				gObjAddMsgSendDelay(lpTargetObj,4, lpObj->m_Index, 100, 0);
			}
		}
		else if ( (rand()%4) == 0 )
		{
			if ( !gObjUniriaSprite(lpTargetObj) )
			{
				MSBFlag = 1;
			}
		}
	}

	if ( ManaChange )
	{
		GCManaSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
	}

	if ( magicsend )
	{
		GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_Change == 9 )
		{
			GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);
		}
	}

	if ( lpObj->Class == CLASS_ELF && lpObj->Level == 1 && AttackDamage > 10 )
	{
		LogAdd("error-Level1 : [%s][%s] Str:%d %d %d %d %d %d %d",
			lpObj->AccountID, lpObj->Name, lpObj->Strength,
			lpObj->m_AttackDamageMinRight, lpObj->m_AttackDamageMaxRight,
			lpObj->m_AttackDamageMinLeft, lpObj->m_AttackDamageMaxLeft, 
			lpObj->m_AttackDamageMax, lpObj->m_AttackDamageMin);
	}

	lpObj->m_Rest = 0;

	if ( lpObj->Class >= 504 && lpObj->Class <= 511 )
	{
		if( gObjCheckUsedBuffEffect(lpObj, 81) )
			AttackDamage *= 2;
	}

	if ( lpTargetObj->Class >= 504 && lpTargetObj->Class <= 511 )
	{
		if( gObjCheckUsedBuffEffect(lpTargetObj, 81) )
			AttackDamage /= 2;
	}

	if ( AttackDamage > 0 )
	{
		int atd_reflect = (int)((float)AttackDamage * lpTargetObj->DamageReflect / 100.0f);

		if ( atd_reflect )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, atd_reflect);
		}

		if ( (rand()%100) < lpObj->SetOpReflectionDamage )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, AttackDamage);
		}

		int FullReflect = 0;

		if( gObjWingSprite(lpTargetObj) == TRUE )
		{
			CItem * Wing = &lpTargetObj->pInventory[7];

			if( (Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,40))
				 || Wing->m_Type == ITEMGET(12,43) 
				 || Wing->m_Type == ITEMGET(12,50) )
			{
				BYTE ExcOpt = lpTargetObj->pInventory[7].m_NewOption;

				if( (ExcOpt & 2) == 2 )	{
					FullReflect+=5;
				}
			}
		}

		if( FullReflect && (rand()%100 < FullReflect) )	
		{
			if( lpObj->Type == OBJ_MONSTER )
			{
				gObjAddMsgSendDelay(lpTargetObj, 12, lpObj->m_Index, 10, lpObj->m_AttackDamageMax);
			}
			else if( lpObj->Type == OBJ_USER )
			{
				gObjAddMsgSendDelay(lpTargetObj, 12, lpObj->m_Index, 10, AttackDamage);
			}
		}

		if( bCombo )
		{
#ifdef __ALIEN__
			int iComboDamage = ( Strength + Dexterity + Energy ) / g_ComboAttackPower;
#else
			int iComboDamage = ( Strength + Dexterity + Energy ) / 2;	// #formula
#endif
			AttackDamage += iComboDamage;

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, iComboDamage);
				lpTargetObj->iShield -= iTempShieldDamage;
				lpTargetObj->Life -= iComboDamage - iTempShieldDamage;
				iTotalShieldDamage += iTempShieldDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
			else
			{
				lpTargetObj->Life -= iComboDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}

			MsgDamage |= 0x80;
			skill = 59;
		}

		float fRate = lpObj->SetOpDoubleDamage + lpObj->m_MPSkillOpt.MpsSpearMastery + lpObj->m_MPSkillOpt.MpsFistMastery;

		if ( (rand()%100) < fRate )
		{
			if ( skill == AT_SKILL_FIRESCREAM || skill == 518 || skill == 520 )
			{
				gObjUseSkill.FireScreamExplosionAttack(lpObj, lpTargetObj, AttackDamage);
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if( skill == 265 && 
					rand()%100 < (10+Energy/100) )
				{	
					bDragonKickSDAttackSuccess = TRUE;
					
					int nDecreaseSDRate = 10 + Energy/30;

					if( nDecreaseSDRate > 100 )
					nDecreaseSDRate = 100;

					int nDecreaseSD = lpTargetObj->iShield*nDecreaseSDRate/100;
					lpTargetObj->iShield -= nDecreaseSD;
					lpTargetObj->Life -= AttackDamage;
					iTotalShieldDamage += nDecreaseSD;
				}
				else
				{
					iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
					lpTargetObj->iShield -= iTempShieldDamage;
					lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
					iTotalShieldDamage += iTempShieldDamage;
				}

				AttackDamage += AttackDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
			else
			{
				lpTargetObj->Life -= AttackDamage;
				AttackDamage += AttackDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}

			MsgDamage |= 0x40;
		}

		if ( g_ShieldSystemOn == TRUE && bDragonKickSDAttackSuccess == 0 )
		{
			AttackDamage -= iTotalShieldDamage;
		}

		if ( g_ShieldSystemOn == FALSE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( CC_MAP_RANGE(lpObj->MapNumber) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}

		if( lpObj->Class == CLASS_MONK && byBarrageCount )
		{
			if( lpMagic->m_Skill == 261 ||
				lpMagic->m_Skill == 263 ||
				g_MasterSkillSystem.GetBaseMasterLevelSkill(skill) == 552 ||
				g_MasterSkillSystem.GetBaseMasterLevelSkill(skill) == 555 ||
				g_MasterSkillSystem.GetBaseMasterLevelSkill(skill) == 559 )
			{
				if( (byBarrageCount%2) == 0 )
				{
					MsgDamage |= 0x20;
				}
				else
				{
					MsgDamage |= 0x10;
				}
			}
			else
			{
				if( (byBarrageCount%4) == 0 )
				{
					MsgDamage |= 0x20;
				}
				else
				{
					MsgDamage |= 0x10;
				}
			}
		}

		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill, iTotalShieldDamage);

		if ( iTotalShieldDamage > 0 )
		{
			LogAddTD("[PvP System] Victim:[%s][%s], Attacker:[%s][%s] - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				lpTargetObj->AccountID, lpTargetObj->Name, lpObj->AccountID, lpObj->Name,
				lpTargetObj->iShield + iTotalShieldDamage, lpTargetObj->Life + AttackDamage, 
				lpTargetObj->iShield, lpTargetObj->Life);
		}
	}
	else
	{
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
	}

	if ( lpObj->Life <= 0.0f && lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_CheckLifeTime <= 0 )
		{
			lpObj->lpAttackObj = lpTargetObj;

			if ( lpTargetObj->Type == OBJ_USER )
			{
				lpObj->m_bAttackerKilled = true;
			}
			else
			{
				lpObj->m_bAttackerKilled = false;
			}

			lpObj->m_CheckLifeTime = 3;
		}
	}

	CItem* Right = &lpObj->pInventory[0];
	CItem* Left = &lpObj->pInventory[1];

	if( Right->GetDetailItemType() == 2 ||
		Left->GetDetailItemType() == 2 )
	{
		if( lpObj->m_MPSkillOpt.MpsMaceMastery > 0.0f )
		{
			float fPercent = rand()%100;

			if( fPercent < lpObj->m_MPSkillOpt.MpsMaceMastery )
			{
				gObjAddBuffEffect(lpTargetObj,AT_STUN,0,0,0,0,2);
				gObjSetPosition(lpTargetObj->m_Index,lpTargetObj->X,lpTargetObj->Y);
			}
		}
	}

	if ( lpMagic && lpMagic->m_Skill == 332 )
	{
		float fSpringValue = g_MasterSkillSystem.GetMasterSkillValue(MagicDamageC.SkillGet(lpMagic->m_Skill),lpMagic->m_Level);

		if( fSpringValue > 0.0f )
		{
			float fPercent = rand()%100;

			if( fPercent < fSpringValue )
			{
				gObjBackSpring2(lpTargetObj,lpObj,2);
			}
		}
	}

	if ( lpMagic )
	{
		gObjUseSkill.SpecificSkillAdditionTreat(lpObj, lpTargetObj, lpMagic, AttackDamage);
	}
	return TRUE;
}

//0051A6D0
int CObjAttack::GetAttackDamage(LPOBJ lpObj, int targetDefense, BYTE& effect, BOOL bIsOnDuel, CMagicInf* lpMagic) //004B9FA0
{
	if ( g_ShieldSystemOn == TRUE )
	{
		if ( bIsOnDuel == TRUE )
		{
			bIsOnDuel = FALSE;
		}
	}

	int ad;
	int sub;
	int SkillRightMaxDamage =0;
	int SkillRightMinDamage =0;
	int SkillLeftMaxDamage =0;
	int SkillLeftMinDamage =0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];
	BOOL bTwoHandWeapon = FALSE;

	if( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
	{
		int AttackMin = 0;
		sub = lpObj->m_AttackDamageMax - lpObj->m_AttackDamageMin;
		AttackMin = lpObj->m_AttackDamageMin + (rand()%(sub+1));
		lpObj->m_AttackDamageRight = AttackMin;
		lpObj->m_AttackDamageLeft = AttackMin;
	}
	else
	{
		int cDamage=0;

		if ( lpObj->Class == CLASS_DARKLORD ||
			 lpObj->Class == CLASS_KNIGHT   ||
			 lpObj->Class == CLASS_MAGUMSA  ||
			 lpObj->Class == CLASS_MONK )
		{
			if ( Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) &&
				 Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0) )
			{
				if ( Right->m_IsValidItem && Left->m_IsValidItem )
				{
					bTwoHandWeapon = TRUE;
				}
			}
		}

		if ( lpMagic )
		{
			if ( lpMagic->m_Skill == 60 || lpMagic->m_Skill == 509 && lpObj->SkillLongSpearChange )
			{
				SkillRightMaxDamage = DefMagicInf[66].m_DamageMax;
				SkillRightMinDamage = DefMagicInf[66].m_DamageMin;
			}
			else
			{
				SkillRightMaxDamage = lpMagic->m_DamageMax;
				SkillRightMinDamage = lpMagic->m_DamageMin;
			}
#pragma message("CHECK ME")
			if( g_MasterSkillSystem.CheckMasterLevelSkill(lpMagic->m_Skill) )
			{
				float fDamage = g_MasterSkillSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
				SkillRightMaxDamage += fDamage;
				SkillRightMinDamage += fDamage;
			}

			if ( bTwoHandWeapon == FALSE )
			{
				SkillLeftMaxDamage = SkillRightMaxDamage;
				SkillLeftMinDamage = SkillRightMinDamage;
			}

			SkillRightMaxDamage += lpObj->SetOpAddSkillAttack;
			SkillRightMinDamage += lpObj->SetOpAddSkillAttack;
			SkillLeftMaxDamage += lpObj->SetOpAddSkillAttack;
			SkillLeftMinDamage += lpObj->SetOpAddSkillAttack;

			int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);

			if ( SkillAttr != -1 )
			{
				if((lpObj->Authority&32) == 32 && (lpObj->pInventory[10].m_Type == ITEMGET(13,42) || lpObj->pInventory[11].m_Type == ITEMGET(13,42))) //season 2.5 add-on
				{
					SkillRightMaxDamage += (BYTE)255;
					SkillRightMinDamage += (BYTE)255;
					SkillLeftMaxDamage  += (BYTE)255;
					SkillLeftMinDamage  += (BYTE)255;
				}
				else
				{
					SkillRightMaxDamage += (BYTE)lpObj->m_AddResistance[SkillAttr];
					SkillRightMinDamage += (BYTE)lpObj->m_AddResistance[SkillAttr];
					SkillLeftMaxDamage  += (BYTE)lpObj->m_AddResistance[SkillAttr];
					SkillLeftMinDamage  += (BYTE)lpObj->m_AddResistance[SkillAttr];
				}
			}

			SkillRightMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillRightMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillLeftMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillLeftMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;

			if ( lpMagic->m_Skill == 65 )
			{
				int iPartyCount = this->GetPartyMemberCount(lpObj);
				int addskilldamage = ( lpObj->Leadership + lpObj->AddLeadership ) / 10 + ( iPartyCount * 50 );

				SkillRightMaxDamage += addskilldamage;
				SkillRightMinDamage += addskilldamage;
				SkillLeftMaxDamage += addskilldamage;
				SkillLeftMinDamage += addskilldamage;
			}
			else if( lpMagic->m_Skill == 62 || g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 512 || lpMagic->m_Skill == 516 )
			{
				if ( lpObj->pInventory[8].m_Type == ITEMGET(13,4) )
				{
					int addskilldamage = ( lpObj->Strength + lpObj->AddStrength ) / 10 + ( lpObj->Leadership + lpObj->AddLeadership ) / 5 + lpObj->pInventory[8].m_PetItem_Level * 10;

					SkillRightMaxDamage += addskilldamage;
					SkillRightMinDamage += addskilldamage;
					SkillLeftMaxDamage += addskilldamage;
					SkillLeftMinDamage += addskilldamage;
				}
			}
			else if( lpObj->Class == CLASS_DARKLORD && lpMagic->m_Skill == 238 )
			{
				int iadddamage = ( lpObj->Strength + lpObj->AddStrength ) / 30 + ( lpObj->Energy + lpObj->AddEnergy ) / 55;	// #formula

				SkillRightMaxDamage += iadddamage;
				SkillRightMinDamage += iadddamage;
				SkillLeftMaxDamage += iadddamage;
				SkillLeftMinDamage += iadddamage;
			}
			else if( lpObj->Class == CLASS_DARKLORD )
			{
				int iadddamage = ( lpObj->Strength + lpObj->AddStrength ) / 25 + ( lpObj->Energy + lpObj->AddEnergy ) / 50;	// #formula

				SkillRightMaxDamage += iadddamage;
				SkillRightMinDamage += iadddamage;
				SkillLeftMaxDamage += iadddamage;
				SkillLeftMinDamage += iadddamage;
			}
			
			if ( lpObj->Class == CLASS_ELF && lpMagic->m_Skill == 235 ) //season4 add-on
			{
				SkillLeftMaxDamage = SkillLeftMaxDamage * 80 / 100;
				SkillLeftMinDamage = SkillLeftMinDamage * 80 / 100;
			}
		}

		if ( lpObj->m_CriticalDamage > 0 )
		{
			if ( (rand()%100) < lpObj->m_CriticalDamage )
			{
				cDamage = TRUE;
				effect = 3;
			}
		}

		if ( lpObj->m_ExcelentDamage > 0 )
		{
			if ( (rand()%100) < lpObj->m_ExcelentDamage )
			{
				cDamage = TRUE;
				effect = 2;
			}
		}

		int iValue1 = 0;
		int iValue2 = 0;

		if(gObjCheckUsedBuffEffect(lpObj, AT_BERSERKER) == 1)
		{
			this->GetBerserkerSkillAttackDamage(lpObj, iValue1, iValue2); 
		}
		else if(gObjCheckUsedBuffEffect(lpObj, 150) == 1)
		{
			this->GetBerserkerSkillAttackDamage(lpObj, iValue1, iValue2); 
		}
		else if(gObjCheckUsedBuffEffect(lpObj, 151) == 1)
		{
			this->GetBerserkerSkillAttackDamage(lpObj, iValue1, iValue2); 
		}
		else if(gObjCheckUsedBuffEffect(lpObj, 152) == 1)
		{
			this->GetBerserkerSkillAttackDamage(lpObj, iValue1, iValue2); 
		}

		__try
		{
			sub = ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + iValue2) - ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + iValue1); //season4 changed
			lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + iValue1) + (rand()%(sub+1)); //season4 changed
		}
		__except ( sub=1, 1 )
		{

		}

		__try
		{
			sub = ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage + iValue2) - ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + iValue1); //season4 changed
			lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + iValue1) + (rand()%(sub+1)); //season4 changed
		}
		__except ( sub=1, 1 )
		{

		}

		if ( cDamage )
		{
			lpObj->m_AttackDamageRight = lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + iValue2; //season4 changed
			lpObj->m_AttackDamageLeft = lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage + iValue2; //season4 changed

			lpObj->m_AttackDamageRight += lpObj->SetOpAddCriticalDamage;
			lpObj->m_AttackDamageLeft += lpObj->SetOpAddCriticalDamage;

			lpObj->m_AttackDamageRight += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
			lpObj->m_AttackDamageLeft += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;

			int SkillAddCriticalDamage = 0;
			gObjCheckUsedBuffEffect(lpObj, AT_INCREASE_CRITICAL_DMG, &SkillAddCriticalDamage, 0);

			lpObj->m_AttackDamageRight += SkillAddCriticalDamage;
			lpObj->m_AttackDamageLeft += SkillAddCriticalDamage;

			gObjCheckUsedBuffEffect(lpObj, 148, &SkillAddCriticalDamage, 0);

			lpObj->m_AttackDamageRight += SkillAddCriticalDamage;
			lpObj->m_AttackDamageLeft += SkillAddCriticalDamage;

			if ( effect == 2 )
			{
				lpObj->m_AttackDamageRight += ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage ) * 20 / 100;
				lpObj->m_AttackDamageLeft += ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage ) * 20 / 100;

				lpObj->m_AttackDamageRight += lpObj->SetOpAddExDamage;
				lpObj->m_AttackDamageLeft += lpObj->SetOpAddExDamage;
			}
		}
	}

	if ( bTwoHandWeapon )
	{
		if ( bIsOnDuel )
		{
			ad = ( lpObj->m_AttackDamageRight + lpObj->m_AttackDamageLeft ) * 60 / 100 - targetDefense;
		}
		else
		{
			ad = ( lpObj->m_AttackDamageRight + lpObj->m_AttackDamageLeft ) - targetDefense;
		}
		ad -= lpObj->SetOpAddCriticalDamage; //season4 add-on
	}
	else if ( (Left->m_Type >= ITEMGET(4, 0) && Left->m_Type < ITEMGET(4, 7)) ||
			 Left->m_Type == ITEMGET(4,20) || Left->m_Type == ITEMGET(4,21)  ||
			 Left->m_Type == ITEMGET(4,22) ||
			 Left->m_Type == ITEMGET(4,23) ||//season4 add-on
			 Left->m_Type == ITEMGET(4,24) ) //season4.6 add-on
	{
		if ( bIsOnDuel )
		{
			ad = ( lpObj->m_AttackDamageLeft ) * 60 / 100 - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageLeft - targetDefense;
		}

		if( lpObj->m_MPSkillOpt.MpsBowStrength > 0.0f )
		{
			ad += lpObj->m_MPSkillOpt.MpsBowStrength;
		}
	}
	else if ( (Right->m_Type >= ITEMGET(4, 8) && Right->m_Type < ITEMGET(4, 15)) ||
			  ( Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0) )  )
	{
		if ( bIsOnDuel )
		{
			ad = ( lpObj->m_AttackDamageRight ) * 60 / 100 - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageRight - targetDefense;
		}

		if( lpObj->m_MPSkillOpt.MpsCrossbowStrength > 0.0f )
		{
			ad += lpObj->m_MPSkillOpt.MpsCrossbowStrength;
		}
	}
	else if ( Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) )
	{
		if ( bIsOnDuel )
		{
			ad = ( lpObj->m_AttackDamageRight ) * 60 / 100 - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageRight - targetDefense;
		}

		if( lpObj->m_MPSkillOpt.MpsTwoHandSwordStrength > 0.0f )
		{
			ad += lpObj->m_MPSkillOpt.MpsTwoHandSwordStrength;
		}
	}
	else if ( Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0) )
	{
		if ( bIsOnDuel )
		{
			ad = ( lpObj->m_AttackDamageRight ) * 60 / 100 - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageRight - targetDefense;
		}
	}
	else if ( bIsOnDuel )
	{
		ad = ( lpObj->m_AttackDamageLeft ) * 60 / 100 - targetDefense;
	}
	else
	{
		ad = lpObj->m_AttackDamageLeft - targetDefense;
	}

	if ( lpObj->SetOpTwoHandSwordImproveDamage )
	{
		ad += ad * lpObj->SetOpTwoHandSwordImproveDamage  / 100;
	}

	int SkillAttack = 0;

	SkillAttack = gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_ATTACK_DAMAGE);
	SkillAttack += gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_WRATH);

	ad += SkillAttack;
	
	if ( lpObj->m_SkillAttack )
	{
		ad += 15;
	}

	if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime > 0 )
	{
		ad += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

		if ( ad < 0 )
		{
			ad = 0;
		}
	}

	int iValue = 0;

	gObjCheckUsedBuffEffect(lpObj, AT_WEAKNESS, &iValue, 0);

	ad -= ad * iValue / 100;

	float nAddAttackDamageLeft	= 0;
	float nAddAttackDamageRight = 0;
	switch(Left->GetDetailItemType())
	{
	case 1:
		{
			if( lpObj->m_MPSkillOpt.MpsOneHandSwordStrength > 0.0f )
			{
				nAddAttackDamageLeft += lpObj->m_MPSkillOpt.MpsOneHandSwordStrength;
			}
		}
		break;
	case 2:
		{
			if( lpObj->m_MPSkillOpt.MpsMaceStrength > 0.0f )
			{
				nAddAttackDamageLeft += lpObj->m_MPSkillOpt.MpsMaceStrength;
			}
		}
		break;
	case 3:
		{
			if( lpObj->m_MPSkillOpt.MpsSpearStrength > 0.0f )
			{
				nAddAttackDamageLeft += lpObj->m_MPSkillOpt.MpsSpearStrength;
			}
		}
		break;
	case 11:
		{
			if( lpObj->m_MPSkillOpt.MpsScepterStrength > 0.0f )
			{
				nAddAttackDamageLeft += lpObj->m_MPSkillOpt.MpsScepterStrength;
			}
		}
		break;
	case 12:
		{
			if( lpObj->m_MPSkillOpt.MpsFistStrength > 0.0f )
			{
				nAddAttackDamageLeft += lpObj->m_MPSkillOpt.MpsFistStrength;
			}
		}
		break;
	}

	switch(Right->GetDetailItemType())
	{
	case 1:
		{
			if( lpObj->m_MPSkillOpt.MpsOneHandSwordStrength > 0.0f )
			{
				nAddAttackDamageRight += lpObj->m_MPSkillOpt.MpsOneHandSwordStrength;
			}
		}
		break;
	case 2:
		{
			if( lpObj->m_MPSkillOpt.MpsMaceStrength > 0.0f )
			{
				nAddAttackDamageRight += lpObj->m_MPSkillOpt.MpsMaceStrength;
			}
		}
		break;
	case 3:
		{
			if( lpObj->m_MPSkillOpt.MpsSpearStrength > 0.0f )
			{
				nAddAttackDamageRight += lpObj->m_MPSkillOpt.MpsSpearStrength;
			}
		}
		break;
	case 11:
		{
			if( lpObj->m_MPSkillOpt.MpsScepterStrength > 0.0f )
			{
				nAddAttackDamageRight += lpObj->m_MPSkillOpt.MpsScepterStrength;
			}
		}
		break;
	case 12:
		{
			if( lpObj->m_MPSkillOpt.MpsFistStrength > 0.0f )
			{
				nAddAttackDamageRight += lpObj->m_MPSkillOpt.MpsFistStrength;
			}
		}
		break;
	}

	if( bTwoHandWeapon )
	{
		ad += nAddAttackDamageRight * 0.5f + nAddAttackDamageLeft * 0.5f;
	}
	else
	{
		ad += nAddAttackDamageRight + nAddAttackDamageLeft;
	}

	if( lpObj->Class == CLASS_KNIGHT 
		|| lpObj->Class == CLASS_MAGUMSA 
		|| lpObj->Class == CLASS_DARKLORD 
		|| lpObj->Class == CLASS_MONK )
	{
		if( lpObj->m_MPSkillOpt.MpsWeaponMastery > 0.0f )
		{
			ad += lpObj->m_MPSkillOpt.MpsWeaponMastery;
		}
	}

	if( lpObj->Class == CLASS_ELF )
	{
		if( lpObj->m_MPSkillOpt.MpsWeaponMastery2 > 0.0f )
		{
			ad += lpObj->m_MPSkillOpt.MpsWeaponMastery2;
		}
	}

	if( lpObj->m_MPSkillOpt.MpsCommandAttackInc > 0.0f )
	{
		if( lpObj->pInventory[0].GetDetailItemType() == 11 || lpObj->pInventory[1].GetDetailItemType() == 11 )
		{
			int nAddAttack = (lpObj->AddLeadership + lpObj->Leadership) / lpObj->m_MPSkillOpt.MpsCommandAttackInc;
			ad += nAddAttack;
		}
	}

	return ad;
}

//0051bcc0	-> Ok...
int CObjAttack::GetAttackDamageWizard(LPOBJ lpObj, int targetDefense, CMagicInf* lpMagic, BYTE& effect, BOOL bIsOnDuel) //0x4BAD10
{
	if( g_ShieldSystemOn )
	{
		if( bIsOnDuel )
		{
			bIsOnDuel = false;
		}
	}
	// ----
	int damagemin;
	int damagemax;
	int ad;
	// ----
	if( lpMagic->m_Skill == 40 )
	{
		if( lpObj->SkillHellFire2Count >= 0 )
		{
			int SkillHellFire2CountDamageTable[13] = { 0, 20, 50, 99, 160, 225, 325, 425, 550, 700, 880, 1090, 1320 };
			int CountDamage;
			// ----
			if( lpObj->SkillHellFire2Count > 12 )
			{
				CountDamage = 0;
			}
			else
			{
				CountDamage = SkillHellFire2CountDamageTable[lpObj->SkillHellFire2Count];
			}
			// ----
			ad = (lpObj->Strength + lpObj->AddStrength) / 2 + CountDamage;
			// ----
			damagemin =		ad + lpObj->m_MagicDamageMin;
			damagemax =		ad + lpObj->m_MagicDamageMax;
			damagemin +=	lpObj->SetOpAddSkillAttack;
			damagemax +=	lpObj->SetOpAddSkillAttack;
			// ----
			int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
			// ----
			if( SkillAttr != -1 )
			{
				if( (lpObj->Authority & 32) == 32 
					&& (lpObj->pInventory[10].m_Type == ITEMGET(13,42) 
					|| lpObj->pInventory[11].m_Type == ITEMGET(13,42)) )
				{
					damagemin += (BYTE)255;
					damagemax += (BYTE)255;
				}
			}
			// ----
			damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		}
	}
	else
	{
		ad = lpMagic->GetDamage();
		// ----
		damagemin =	lpMagic->m_DamageMin + lpObj->m_MagicDamageMin;
		damagemax =	lpMagic->m_DamageMax + lpObj->m_MagicDamageMax;
		// ----
		if( g_MasterSkillSystem.CheckMasterLevelSkill(lpMagic->m_Skill) )
		{
			float fDamage	= g_MasterSkillSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
			damagemin		+= (INT64)fDamage;
			damagemax		+= (INT64)fDamage;
		}
		// ----
		damagemin += lpObj->SetOpAddSkillAttack;
		damagemax += lpObj->SetOpAddSkillAttack;
		// ----
		int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
		// ----
		if( SkillAttr != -1 )
		{
			if( (lpObj->Authority&32) == 32 
				&& (lpObj->pInventory[10].m_Type == ITEMGET(13,42) 
				|| lpObj->pInventory[11].m_Type == ITEMGET(13,42)) )
			{
				damagemin += (BYTE)255;
				damagemax += (BYTE)255;
			}
		}
		// ----
		damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	}
	// ----
	CItem * Right = &lpObj->pInventory[0];
	// ----
	if( Right->IsItem() )
	{
		if( (Right->m_Type >= ITEMGET(5, 0) && Right->m_Type < ITEMGET(6, 0)) ||
			 Right->m_Type == ITEMGET(0, 31) ||
			 Right->m_Type == ITEMGET(0, 21) ||
			 Right->m_Type == ITEMGET(0, 23) ||
			 Right->m_Type == ITEMGET(0, 25) ||
			 Right->m_Type == ITEMGET(0, 28) )
		{
			if( Right->m_IsValidItem  )
			{
				int damage	= Right->m_Magic / 2 + Right->m_Level * 2;
				damage		-= (WORD)(Right->m_CurrentDurabilityState * damage);
				damagemin	+= damagemin * damage / 100;
				damagemax	+= damagemax * damage / 100;
			}
		}
	}
	// ----
	int subd = damagemax - damagemin;
	// ----
	__try
	{
		if( bIsOnDuel )
		{
			ad = (damagemin + (rand()%(subd+1))) * 60 / 100 - targetDefense;
		}
		else
		{
			ad = (damagemin + (rand()%(subd+1))) - targetDefense;
		}
		// ----
		int nCritical = lpObj->m_CriticalDamage;
		// ----
		if( gObjCheckUsedBuffEffect(lpObj, 139) )
		{
			float fValue = lpObj->m_MPSkillOpt.ukn_11C;
			nCritical += fValue;
		}
		// ----
		if( nCritical > 0 )
		{
			if( (rand()%100) < nCritical )
			{
				if( bIsOnDuel )
				{
					ad = damagemax * 60 / 100 - targetDefense;
				}
				else
				{
					ad = damagemax - targetDefense;
				}
				// ----
				ad += lpObj->SetOpAddCriticalDamage;
				ad += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
				// ----
				int iValue = 0;
				// ----
				gObjCheckUsedBuffEffect(lpObj, AT_INCREASE_CRITICAL_DMG, &iValue, 0);
				ad += iValue;
				gObjCheckUsedBuffEffect(lpObj, 148, &iValue, 0);
				ad += iValue;
				effect = 3;
			}
		}
		// ----
		if( lpObj->m_ExcelentDamage > 0 )
		{
			if( (rand()%100) < lpObj->m_ExcelentDamage )
			{
				if( bIsOnDuel )
				{
					ad = damagemax * 60 / 100 - targetDefense;
				}
				else
				{
					ad = damagemax - targetDefense;
				}
				// ----
				ad += damagemax * 20 / 100;
				ad += lpObj->SetOpAddExDamage;
				effect = 2;
			}
		}
	}
	__except(subd = 1, 1)
	{
		// --
	}
	// ----
	int iIncreaseDamage = 0;
	// ----
	iIncreaseDamage = gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_ATTACK_DAMAGE);
	iIncreaseDamage += gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_WIZARDRY);
	ad += iIncreaseDamage;
	// ----
	if( lpObj->m_SkillAttack )
	{
		ad += 10;
	}
	// ----
	return ad;
}

//0051C4C0	->
BOOL gObjDenorantSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Denorant = & lpObj->pInventory[8];

	if ( Denorant->m_Type == ITEMGET(13,3) )
	{
		if ( Denorant->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//0051C560	->
BOOL gObjDarkHorse(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Denorant = & lpObj->pInventory[8];

	if ( Denorant->m_Type == ITEMGET(13,4) )
	{
		if ( Denorant->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//0051C600	->
BOOL gObjFenrir(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * lpFenrir = & lpObj->pInventory[8];

	if ( lpFenrir->m_Type == ITEMGET(13,37) )
	{
		if ( lpFenrir->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//0051C6A0	->
int CObjAttack::GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{
	int iShieldDamage = 0;

	if ( g_ShieldSystemOn == FALSE )
		return 0;

	if ( iAttackDamage <= 0 )
		return 0;

	int iReduceLife = 0;
	int iReduceShield = 0;
	int iReduceLifeForEffect = 0; 
	bool bReduceShieldGage = 0;
	int iDamageDevideToSDRate = g_iDamageDevideToSDRate;
	iDamageDevideToSDRate -= lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;
	iDamageDevideToSDRate += lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate;

	if ( iDamageDevideToSDRate < 0 )
		iDamageDevideToSDRate = 0;

	if ( iDamageDevideToSDRate > 100 )
		iDamageDevideToSDRate = 100;

	if ( lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand()%100;

		if ( iRand < lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
		{
			iDamageDevideToSDRate = 0;
		}
	}

	if ( lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate ||
		 lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate ||
		 lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
	{
		LogAddTD("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
			lpObj->AccountID, lpObj->Name,
			lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate,
			lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate,
			lpTargetObj->AccountID, lpTargetObj->Name,
			lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate,
			iDamageDevideToSDRate);
	}

	iReduceShield = iAttackDamage * iDamageDevideToSDRate / 100;
	iReduceLife = iAttackDamage - iReduceShield;

	if ( (lpTargetObj->iShield-iReduceShield) <0 )
	{
		iReduceLife += iReduceShield  - lpTargetObj->iShield;
		iReduceShield = lpTargetObj->iShield;

		if ( lpTargetObj->iShield > 0 )
		{
			bReduceShieldGage = true;
		}
	}

	iReduceLifeForEffect = ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) * 20.0f / 100.0f;

	if ( bReduceShieldGage == true && iReduceLife > iReduceLifeForEffect )
	{
		if ( !CC_MAP_RANGE(lpTargetObj->MapNumber) )
		{
			GCSendEffectInfo(lpTargetObj->m_Index, 17);
		}
	}

	return iReduceShield;
}

//0051c980	->
int CObjAttack::GetAttackDamageSummoner(LPOBJ lpObj, int targetDefense, CMagicInf * lpMagic, BYTE & effect, BOOL bIsOnDuel)
{
	if( g_ShieldSystemOn == true )
	{
		if( bIsOnDuel == true )
		{
			bIsOnDuel = false;
		}
	}
	// ----
	float fCurseDamageMin	= 0;
	float fCurseDamageMax	= 0;
	float fMagicDamageMin	= 0;
	float fMagicDamageMax	= 0;
	int damagemin;
	int damagemax;
	int ad;
	int iIncreaseDamage		= 0;
	// ----
	ad = lpMagic->GetDamage();
	// ----
	switch(lpMagic->m_Skill)
	{
	case 219:
	case 220:
	case 221:
	case 222:
	case 223:
	case 224:
	case 225:
		{
			if( gObjCheckUsedBuffEffect(lpObj, 81) == true )
			{
				int iValue		= gObjGetTotalValueOfEffect(lpObj, 31);
				fCurseDamageMax = ((float)((lpObj->Energy + lpObj->AddEnergy) / 4) + 0.015) * iValue / 100.0;
				fCurseDamageMin = (float)(((lpObj->Energy + lpObj->AddEnergy) / 9) * iValue / 100);
			}
			// ----
			if( gObjCheckUsedBuffEffect(lpObj, 150) == true
				|| gObjCheckUsedBuffEffect(lpObj, 151) == true
				|| gObjCheckUsedBuffEffect(lpObj, 152) == true )
			{
				float iValue = gObjGetTotalValueOfEffect(lpObj, 31);
				// ----
				if( lpObj->m_MPSkillOpt.ukn_D8 > 0.0f )
				{
					iValue += lpObj->m_MPSkillOpt.ukn_D8;
				}
				// ----
				fCurseDamageMax = ((float)((lpObj->Energy + lpObj->AddEnergy) / 4) + 0.015) * iValue / 100.0;
				fCurseDamageMin = (float)(((lpObj->Energy + lpObj->AddEnergy) / 9) * iValue / 100);
			}
			// ----
			damagemin = (int)((float)(lpMagic->m_DamageMin + lpObj->m_CurseDamgeMin) + fCurseDamageMin);
			damagemax = (int)((float)(lpMagic->m_DamageMax + lpObj->m_CurseDamgeMax) + fCurseDamageMax);
			// ----
			if( g_MasterSkillSystem.CheckMasterLevelSkill(lpMagic->m_Skill) )
			{
				float fDamage = g_MasterSkillSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
				damagemin += fDamage;
				damagemax += fDamage;
			}
			// ----
			if( lpObj->m_MPSkillOpt.MpsMagicMastery2 > 0.0f )
			{
				damagemin += lpObj->m_MPSkillOpt.MpsMagicMastery2;
				damagemax += lpObj->m_MPSkillOpt.MpsMagicMastery2;
			}
			// ----
			if( lpObj->m_MPSkillOpt.MpsMinWizCurseInc > 0.0f )
			{
				damagemin += lpObj->m_MPSkillOpt.MpsMinWizCurseInc;
			}
			// ----
			CItem * Left = &lpObj->pInventory[1];
			// ----
			if( Left->IsItem() )
			{
				if( Left->m_Type >= ITEMGET(5, 0) && Left->m_Type < ITEMGET(6, 0) )
				{
					if( Left->m_IsValidItem )
					{
						int damage = Left->m_Curse/2+Left->m_Level*2;
						damage -= (WORD)(Left->m_CurrentDurabilityState*damage);
						damagemin += damagemin*damage/100;
						damagemax += damagemax*damage/100;
					}
				}
			}
			// ----
			iIncreaseDamage += gObjGetTotalValueOfEffect(lpObj, 27);
		}
		break;
	default:
		{
			if( gObjCheckUsedBuffEffect(lpObj, 81) == true )
			{
				int iValue		= gObjGetTotalValueOfEffect(lpObj, 31);
				fMagicDamageMin = (float)(((lpObj->Energy+lpObj->AddEnergy)/9)*iValue/100);
				fMagicDamageMax = (float)(((lpObj->Energy+lpObj->AddEnergy)/4)*iValue/100);
			}
			// ----
			if( gObjCheckUsedBuffEffect(lpObj, 150) == true
				|| gObjCheckUsedBuffEffect(lpObj, 151) == true
				|| gObjCheckUsedBuffEffect(lpObj, 152) == true )
			{
				float iValue = gObjGetTotalValueOfEffect(lpObj, 31);
				// ----
				if( lpObj->m_MPSkillOpt.ukn_DC > 0.0f )
				{
					iValue += lpObj->m_MPSkillOpt.ukn_DC;
				}
				// ----
				fMagicDamageMin = (float)(((lpObj->Energy+lpObj->AddEnergy)/9)*iValue/100);
				fMagicDamageMax = (float)(((lpObj->Energy+lpObj->AddEnergy)/4)*iValue/100);
			}
			// ----
			damagemin = (int)((float)(lpMagic->m_DamageMin + lpObj->m_MagicDamageMin)+fMagicDamageMin);
			damagemax = (int)((float)(lpMagic->m_DamageMax + lpObj->m_MagicDamageMax)+fMagicDamageMax);
			// ----
			if( g_MasterSkillSystem.CheckMasterLevelSkill(lpMagic->m_Skill) )
			{
				float fDamage = g_MasterSkillSystem.GetSkillAttackDamage(lpObj, lpMagic->m_Skill);
				damagemin += fDamage;
				damagemax += fDamage;
			}
			// ----
			if( lpObj->m_MPSkillOpt.MpsMagicMastery2 > 0.0f )
			{
				damagemin += lpObj->m_MPSkillOpt.MpsMagicMastery2;
				damagemax += lpObj->m_MPSkillOpt.MpsMagicMastery2;
			}
			// ----
			if( lpObj->m_MPSkillOpt.MpsMinWizCurseInc > 0.0f )
			{
				damagemin += lpObj->m_MPSkillOpt.MpsMinWizCurseInc;
			}
			// ----
			CItem * Right = &lpObj->pInventory[0];
			// ----
			if( Right->IsItem() )
			{
				if( Right->m_Type >= ITEMGET(5, 0) && Right->m_Type < ITEMGET(6, 0) )
				{
					if( Right->m_IsValidItem )
					{
						int damage = Right->m_Magic/2+Right->m_Level*2;
						damage -= (WORD)(Right->m_CurrentDurabilityState*damage);
						damagemin += damagemin*damage/100;
						damagemax += damagemax*damage/100;
					}
				}
			}
			// ----
			iIncreaseDamage += gObjGetTotalValueOfEffect(lpObj, 15);
		}
		break;
	}
	// ----
	damagemin += lpObj->SetOpAddSkillAttack;
	damagemax += lpObj->SetOpAddSkillAttack;
	int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
	// ----
	if( SkillAttr != -1 )
	{
		if( (lpObj->Authority&32) == 32 && 
			(lpObj->pInventory[10].m_Type == ITEMGET(13, 42) || 
			lpObj->pInventory[11].m_Type == ITEMGET(13, 42)) )
		{
			damagemin += (BYTE)255;
			damagemax += (BYTE)255;
		}
		else
		{
			damagemin += (BYTE)lpObj->m_AddResistance[SkillAttr];
			damagemax += (BYTE)lpObj->m_AddResistance[SkillAttr];
		}
	}
	// ----
	damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	int subd = damagemax-damagemin;
	// ----
	__try
	{
		if( bIsOnDuel )
			ad = ( damagemin+(rand()%(subd+1)))*60/100-targetDefense;
		else ad = (damagemin+(rand()%(subd+1)))-targetDefense;
		if( lpObj->m_CriticalDamage > 0 ) 
		{
			if( (rand()%100) < lpObj->m_CriticalDamage )
			{
				if( bIsOnDuel )
					ad = damagemax*60/100-targetDefense;
				else ad = damagemax-targetDefense;

				ad += lpObj->SetOpAddCriticalDamage;
				ad += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;

				int iValue = 0;
				gObjCheckUsedBuffEffect(lpObj, 5, &iValue, 0);
				ad += iValue;
				gObjCheckUsedBuffEffect(lpObj, 148, &iValue, 0);
				ad += iValue;
				effect = 3;
			}
		}
		if( lpObj->m_ExcelentDamage > 0 )
		{
			if( (rand()%100) < lpObj->m_ExcelentDamage )
			{
				if( bIsOnDuel )
					ad = damagemax*60/100-targetDefense;
				else ad = damagemax-targetDefense;

				ad += damagemax*20/100;
				ad += lpObj->SetOpAddExDamage;
				effect = 2;
			}
		}
	}
	__except( subd= 1, 1 )
	{
	}

	iIncreaseDamage += gObjGetTotalValueOfEffect(lpObj, 2);

	ad += iIncreaseDamage;

	if( lpObj->m_SkillAttack )	
	{
		ad += 10;
	}

	if( lpObj->m_MPSkillOpt.ukn_DC > 0.0f )
	{
		ad += (double)ad * lpObj->m_MPSkillOpt.ukn_DC / 100.0f;
	}

	return ad;
}

//GetBuffTypePhysicalIncrease (0051D690)
BYTE CObjAttack::GetBerserkerSkillAttackDamage(LPOBJ lpObj, int& Value1, int& Value2)
{
	Value1 = 140 + (lpObj->Strength + lpObj->AddStrength + lpObj->Dexterity + lpObj->AddDexterity) / 50;
	Value2 = 160 + (lpObj->Strength + lpObj->AddStrength + lpObj->Dexterity + lpObj->AddDexterity) / 30;

	int iBerserkerValue = 0;

	gObjCheckUsedBuffEffect(lpObj, AT_BERSERKER, &iBerserkerValue, 0);

	Value1 = Value1 * iBerserkerValue / 100;
	Value2 = Value2 * iBerserkerValue / 100;

	return true;
}