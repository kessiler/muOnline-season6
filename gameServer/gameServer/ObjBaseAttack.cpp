#include "stdafx.h"
#include "ObjBaseAttack.h"
#include "GameMain.h"
#include "LogProc.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "CrywolfAltar.h"
#include "CrywolfStatue.h"
#include "IllusionTempleEvent.h"
#include "MasterLevelSystem.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSkillTreeSystem.h"
#include "ItemAddOption.h"
#ifdef __CUSTOMS__
#include "ClassCalc.h"
#endif
// -------------------------------------------------------------------------------

CObjBaseAttack::CObjBaseAttack()
{
	
}
// -------------------------------------------------------------------------------

CObjBaseAttack::~CObjBaseAttack()
{

}
// -------------------------------------------------------------------------------

//0051d8a0	-> Checked
BOOL CObjBaseAttack::DecreaseArrow(LPOBJ lpObj)
{
	if (lpObj->Class == CLASS_ELF && lpObj->Type == OBJ_USER )
	{
		CItem * Right = &lpObj->pInventory[0];
		CItem * Left  = &lpObj->pInventory[1];

		if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type <= ITEMGET(4,14)) ||
			  Right->m_Type == ITEMGET(4,16) || Right->m_Type == ITEMGET(4,18) ||
			  Right->m_Type == ITEMGET(4,19) )
		{
			if ( Left->m_Type == ITEMGET(4,7) )
			{
				if ( Left->m_Durability < 1.0f )
				{
					return FALSE;
				}

				Left->m_Durability -= 1.0f;
				GCItemDurSend(lpObj->m_Index, 1, Left->m_Durability, 0);

				if ( Left->m_Durability < 1.0f )
				{
					lpObj->pInventory[1].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, 1, 0);
				}
			}
			else
			{
				return FALSE;
			}
		}
		else if ( (Left->m_Type >= ITEMGET(4,0) && Left->m_Type <= ITEMGET(4,6) ) ||
				   Left->m_Type == ITEMGET(4,17) ||
				   Left->m_Type == ITEMGET(4,20) ||
				   Left->m_Type == ITEMGET(4,21) ||
				   Left->m_Type == ITEMGET(4,22) ||
				   Left->m_Type == ITEMGET(4,23) ||
				   Left->m_Type == ITEMGET(4,24)) //season4.6 add-on
		{
			if ( Right->m_Type == ITEMGET(4,15) )
			{
				if ( Right->m_Durability < 1.0f )
				{
					return FALSE;
				}

				Right->m_Durability -= 1.0f;
				GCItemDurSend(lpObj->m_Index, 0, Right->m_Durability, 0);

				if ( Right->m_Durability < 1.0f )
				{
					lpObj->pInventory[0].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
				}
			}
			else
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}
// -------------------------------------------------------------------------------

//0051db90	-> Checked
BOOL CObjBaseAttack::CheckAttackArea(LPOBJ lpObj, LPOBJ lpTargetObj) //Identical gs-cs 56
{
	if ( lpTargetObj->Type == OBJ_USER || lpTargetObj->m_RecallMon >= 0 )
	{
		BYTE attr = MapC[lpTargetObj->MapNumber].GetAttr(lpTargetObj->X, lpTargetObj->Y);

//#if (__ONLYMU__ == 1)
		if ( (attr&1) == 1 && lpObj->Type != OBJ_MONSTER )
		{
			return FALSE;
		}
//#else
		//if ( (attr&1) == 1 )
		//{
		//	return FALSE;
		//}
//#endif
	}

	if ( lpObj->Type == OBJ_USER )
	{
		int iRet = gObjCheckAttackArea(lpObj->m_Index, lpTargetObj->m_Index);

		if ( iRet != 0 )
		{
			//LogAddC(2, "[DEBUG] Point 3");
			//LogAddTD("[%s][%s] Try Attack In Not Attack Area (%s,%d,%d) errortype = %d",
			///	lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 208) + lpObj->MapNumber),
			//	lpObj->X, lpObj->Y, iRet);

			if ( bIsIgnorePacketSpeedHackDetect != FALSE )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}
// -------------------------------------------------------------------------------

//0051dcd0	->
BOOL CObjBaseAttack::PkCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( gLanguage == 0 || gLanguage == 2 || gLanguage == 4)
	{
		if ( lpObj->Type == OBJ_USER )
		{
			BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
			if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
			{
				if(gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= 6)
				{
					bPlayerKiller = TRUE;
				}
			}
			else if(lpObj->m_PK_Level >= 6)
			{
				bPlayerKiller = TRUE;
			}

			if(bPlayerKiller == TRUE) //Season 2.5 add-on
			{
				if (lpObj->PartyNumber >= 0 )
				{
					if ( gPkLimitFree == FALSE )
					{
						return FALSE;
					}
				}
				else if ( lpObj->m_PK_Count >= 3 )
				{
					if ( gPkLimitFree == FALSE )
					{
						return FALSE;
					}
				}
			}
		}
	}

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if ( lpTargetObj->Level <= 5 || lpObj->Level <= 5 )
		{
			return FALSE;
		}

		if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 ) // Rivals
		{
			if ( gNonPK == FALSE )
			{
				return TRUE;
			}
		}

		if ( lpObj->lpGuild != NULL && lpTargetObj->lpGuild != NULL )
		{
			if ( lpObj->lpGuild->WarState == 1 && lpTargetObj->lpGuild->WarState == 1 )
			{
				if ( lpObj->lpGuild->Number == lpTargetObj->lpGuild->Number )
				{
					return FALSE;
				}
			}
		}

		if ( gObjTargetGuildWarCheck(lpObj, lpTargetObj) == FALSE && lpTargetObj->lpGuild != NULL && lpTargetObj->lpGuild->WarState != 0)
		{
			if ( lpTargetObj->lpGuild->WarType == 1 )
			{
				if ( lpTargetObj->MapNumber != 6 )
				{
					if ( !gNonPK )
					{
						return TRUE;
					}
				}
			}

			if ( CC_MAP_RANGE(lpTargetObj->MapNumber) == FALSE )
			{
				return FALSE;
			}
		}

		if ( IT_MAP_RANGE(lpObj->MapNumber) != FALSE ) //season2.5 add-on
		{
			//
		}
		else if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
		{
			if ( g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpObj->MapNumber)) != 2 )
			{
				return FALSE;
			}
		}
		else if ( gNonPK != FALSE )
		{
			return FALSE;
		}

		if ( DS_MAP_RANGE(lpObj->MapNumber) ) // DEvil
		{
			return FALSE;
		}

		if ( BC_MAP_RANGE(lpObj->MapNumber) )
		{
			return FALSE;
		}

		if ( gObjDuelCheck(lpObj, lpTargetObj) == FALSE )
		{
			if ( gObjDuelCheck(lpTargetObj) )
			{
				return FALSE;
			}
		}

		if ( gLanguage == 0 || gLanguage == 2 )
		{
			BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
			if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
			{
				if(gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= 6)
				{
					bPlayerKiller = TRUE;
				}
			}
			else if(lpObj->m_PK_Level >= 6)
			{
				bPlayerKiller = TRUE;
			}

			if(bPlayerKiller == TRUE) //Season 2.5 add-on
			{
				if (lpObj->PartyNumber >= 0 )
				{
					if ( gPkLimitFree == FALSE )
					{
						return FALSE;
					}
				}
				else if ( lpObj->m_PK_Count >= 3 )
				{
					if ( gPkLimitFree == FALSE )
					{
						return FALSE;
					}
				}
			}

			bPlayerKiller = FALSE; //Season 2.5 add-on
	
			if(lpTargetObj->PartyNumber >= 0) //Season 2.5 add-on
			{
				if(gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber) >= 6)
				{
					bPlayerKiller = TRUE;
				}
			}
			else if(lpTargetObj->m_PK_Level >= 6)
			{
				bPlayerKiller = TRUE;
			}

			if(bPlayerKiller == 1) //Season 2.5 add-on
			{
				if (lpTargetObj->PartyNumber >= 0 )
				{
					if ( gPkLimitFree == FALSE )
					{
						if(g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
						{
							return FALSE;
						}
					}
				}
				else if ( lpTargetObj->m_PK_Count >= 3 )
				{
					if ( gPkLimitFree == FALSE )
					{
						if(g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
						{
							return FALSE;
						}
					}
				}
			}


		}
	}
	return TRUE;
}
// -------------------------------------------------------------------------------

//
BOOL CObjBaseAttack::ResistanceCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill)
{
	//missing objects? maybe from _CS side or deleted in 1.01.00
	if( lpTargetObj->Type != OBJ_USER)	
	{
		if(lpTargetObj->Class == 277 || lpTargetObj->Class == 283 || lpTargetObj->Class == 288 || lpTargetObj->Class == 278 || lpTargetObj->Class == 215 || lpTargetObj->Class == 216 || lpTargetObj->Class == 217 || lpTargetObj->Class == 218 || lpTargetObj->Class == 219)
		{
			return FALSE;
		}
	}

	if( lpTargetObj->Type != OBJ_USER)
	{
		if( CRYWOLF_ALTAR_CLASS_RANGE(lpTargetObj->Class) != FALSE || CRYWOLF_STATUE_CHECK(lpTargetObj->Class) != FALSE ) //HermeX Fix @28/01/2010
		{
			return FALSE;
		}
	}

	if ( skill == 62 )
	{
		gObjBackSpring2(lpTargetObj, lpObj, 3);
	}

	if(g_MasterSkillSystem.GetBaseMasterLevelSkill(skill) == 512) //Earth Quake Master
	{
		gObjBackSpring2(lpTargetObj, lpObj, 3);
	}

	if( skill == 516)
	{
		gObjBackSpring2(lpTargetObj, lpObj, 3);
	}

	if ( skill == 19
		|| skill == 20
		|| skill == 21
		|| skill == 22
		|| skill == 23
		|| skill == 260
		|| skill == 261
		|| skill == 262
		|| skill == 270
		|| skill == 326
		|| skill == 327
		|| skill == 328
		|| skill == 329
		|| skill == 479 )
	{
		gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150, 0);
	}
	else if ( skill == 3 || skill == 379 || skill == 480 )	// Poison
	{
		if ( retResistance(lpTargetObj, 2) == 0 )
		{
			gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150, 0);
			return TRUE;
		}
		return FALSE;
	}
	else if ( skill == 1 || skill == 384 )
	{
		if(gObjCheckUsedBuffEffect(lpObj, AT_POISON) == 0)
		{
			if ( retResistance(lpTargetObj, 1) == 0 )
			{
				lpTargetObj->lpAttackObj = lpObj;
				gObjAddBuffEffect(lpTargetObj, AT_POISON, 19, 3, 0, 0, 20);
				return TRUE;
			}
			else
			{
				return FALSE;
			}	
		}
		else
		{
			return FALSE;
		}
	}
	else if ( skill == 38 || skill == 387 ) // Death Poison
	{
		if(gObjCheckUsedBuffEffect(lpTargetObj, AT_POISON) == 0)
		{
			if ( retResistance(lpTargetObj, 1) == 0 )
			{
				lpTargetObj->lpAttackObj = lpObj;
				gObjAddBuffEffect(lpTargetObj, AT_POISON, 19, 3, 0, 0, 10);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}
	else if ( skill == 7 || skill == 39 || 
		g_MasterSkillSystem.GetBaseMasterLevelSkill(skill) == 450 || skill == 389 || skill == 489 ) //Season4 add-on
	{
		if(gObjCheckUsedBuffEffect(lpTargetObj, AT_ICE) == 0)
		{
			if ( retResistance(lpTargetObj, 0) == 0 )
			{
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->DelayActionTime = 800;
				lpTargetObj->DelayLevel = 1;
				gObjAddBuffEffect(lpTargetObj, AT_ICE, 20, 0, 0, 0, 10);
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else if ( skill == 51 || skill == 424 ) // Ice Arrow
	{
		if(gObjCheckUsedBuffEffect(lpTargetObj, AT_ICE_ARROW) == 0)
		{
			if ( retResistance(lpTargetObj, 0) == 0 )
			{
				gObjAddBuffEffect(lpTargetObj, AT_ICE_ARROW, 0, 0, 0, 0, 7);
				lpTargetObj->PathCount = 0;
				lpTargetObj->PathStartEnd = 0; //Season3 add-on
				gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}
// -------------------------------------------------------------------------------

//0051e940	-> Revised
BOOL CObjBaseAttack::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL& bAllMiss, BYTE byBarrageCount)
{
	__try
	{
		int SuccessAttackRate			= 0;	//ebp-20
		int TargetSuccessfulBlocking	= lpTargetObj->m_SuccessfulBlocking;	//ebp-24
		int MsgDamage					= 0;	//ebp-28
		// ----
		if( IT_MAP_RANGE(lpTargetObj->MapNumber) )
		{
			if( g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == 2 )
			{
				if( lpTargetObj->Type == OBJ_USER )
				{
					if( g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber) )
					{
						GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
						return false;
					}	
				}
				// ----
				if( lpObj->PartyNumber == lpTargetObj->PartyNumber )
				{
					GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
					return false;
				}
			}
		}
		// ----
		if( lpObj->Type == OBJ_USER )
		{
			int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
			// ----
			if( lpObj->Class == CLASS_DARKLORD )
			{
				SuccessAttackRate = iTotalLevel * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 6) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4  + ((lpObj->Leadership + lpObj->AddLeadership)/10);
				SuccessAttackRate += SuccessAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
			}
			else
			{
				SuccessAttackRate = iTotalLevel * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				SuccessAttackRate += SuccessAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
			}
		}
		else
		{
			SuccessAttackRate = lpObj->m_AttackRating;
		}
		// ----
		SuccessAttackRate += lpObj->m_MPSkillOpt.MpsAttackSuccessRate;//lpObj->m_MLPassiveSkill.m_iML_IncreaseAttackSuccessRate;
		// ----
		if( SuccessAttackRate < TargetSuccessfulBlocking )
		{
			bAllMiss = true;
		}
		// ----
		if( bAllMiss )
		{
			if( (rand()%100) >= 5 )
			{
#ifdef MONK
				if( lpObj->Class == CLASS_MONK && byBarrageCount )
				{
					if( skill != 261 && skill != 263 )
					{
						if( byBarrageCount % 4 )
						{
							MsgDamage |= 0x10u;
						}
						else
						{
							MsgDamage |= 0x20;
						}
					}
					else
					{
						if( byBarrageCount % 2 )
						{
							MsgDamage |= 0x10u;
						}
						else
						{
							MsgDamage |= 0x20;
						}
					}
				}
#endif
				// ----
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
				// ----
				if( magicsend != 0 )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
				}
				// ----
				return false;
			}
		}
		else
		{
			if( (rand() % SuccessAttackRate) < TargetSuccessfulBlocking )
			{
#ifdef MONK
				if( lpObj->Class == CLASS_MONK && byBarrageCount )
				{
					if( skill != 261 && skill != 263 )
					{
						if( byBarrageCount % 4 )
						{
							MsgDamage |= 0x10u;
						}
						else
						{
							MsgDamage |= 0x20;
						}
					}
					else
					{
						if( byBarrageCount % 2 )
						{
							MsgDamage |= 0x10u;
						}
						else
						{
							MsgDamage |= 0x20;
						}
					}
				}
#endif
				// ----
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
				// ----
				if( magicsend != 0 )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
				}
				// ----
				return false;
			}
		}
	}
	__except( lpObj->Level = 1,-1 )
	{
		LogAdd(lMsg.Get(MSGGET(2, 12)), lpObj->Name);
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//0051f060 -> Revised
BOOL CObjBaseAttack::MissCheckPvP(LPOBJ lpObj , LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL & bAllMiss, BYTE byBarrageCount)
{
	float iAttackRate		= 0;	//ebp-8
	float iDefenseRate		= 0;	//ebp-0c
	int	iAttackSuccessRate	= 0;	//ebp-10
	// ----
	if( IT_MAP_RANGE(lpTargetObj->MapNumber) )
	{
		if( g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == 2 )
		{
			if( lpTargetObj->Type == OBJ_USER )
			{
				if( g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber ) != FALSE)
				{
					GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
					return false;
				}	
			}
			// ----
			if( lpObj->PartyNumber == lpTargetObj->PartyNumber )
			{
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return false;
			}
		}
	}
	// ----
#ifdef __CUSTOMS__
	int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
	// ----
	if( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = (float)(iTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact1 + ( lpObj->Dexterity * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact2) / g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateDiv);
	}
	else if( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = (float)(iTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact1 + ( lpObj->Dexterity * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact2 ));
	}
	else if( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = (float)(iTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact1 + ( lpObj->Dexterity * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact2 ) / g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateDiv);
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		iAttackRate = (float)(iTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact1 + ( lpObj->Dexterity * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact2 ) / g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateDiv);
	}
	else if( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = (float)(iTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact1 + ( lpObj->Dexterity * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact2 ));
	}
	else if( lpObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = (float)(iTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact1 + ( lpObj->Dexterity * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact2 ) / g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateDiv);
	}
	else if( lpObj->Class == CLASS_MONK )
	{
		iAttackRate = (float)((iTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact1RF) + (lpObj->Dexterity * g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateFact2) / g_ClassCalc.m_Data[lpObj->Class].PvPAttackRateDiv);
	}
#else
	int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
	// ----
	if( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = (float)(iTotalLevel * 3 + ( lpObj->Dexterity * 45) / 10);
	}
	else if( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = (float)(iTotalLevel * 3 + ( lpObj->Dexterity * 4 ));
	}
	else if( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = (float)(iTotalLevel * 3 + ( lpObj->Dexterity * 6 ) / 10);
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		iAttackRate = (float)(iTotalLevel * 3 + ( lpObj->Dexterity * 35 ) / 10);
	}
	else if( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = (float)(iTotalLevel * 3 + ( lpObj->Dexterity * 4 ));
	}
	else if( lpObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = (float)(iTotalLevel * 3 + ( lpObj->Dexterity * 35 ) / 10);
	}
	else if( lpObj->Class == CLASS_MONK )
	{
		iAttackRate = (float)(iTotalLevel * 2.6f + ( lpObj->Dexterity * 36 ) / 10);
	}
#endif
	// ----
#ifdef __CUSTOMS__
	int iTargetTotalLevel = lpTargetObj->Level + lpTargetObj->m_nMasterLevel;
	// ----
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = (float)(iTargetTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateFact + lpTargetObj->Dexterity / g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateDiv);
	}
	else if( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = (float)(iTargetTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateFact + lpTargetObj->Dexterity / g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateDiv);
	}
	else if( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = (float)(iTargetTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateFact + lpTargetObj->Dexterity / g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateDiv);
	}
	else if( lpTargetObj->Class == CLASS_MAGUMSA )
	{
		iDefenseRate = (float)(iTargetTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateFact + lpTargetObj->Dexterity / g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateDiv);
	}
	else if( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = (float)(iTargetTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateFact + lpTargetObj->Dexterity / g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateDiv);
	}
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = (float)(iTargetTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateFact + lpTargetObj->Dexterity / g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateDiv);
	}
	else if( lpTargetObj->Class == CLASS_MONK )
	{
		iDefenseRate = (float)((iTargetTotalLevel * g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateFactRF) + (lpTargetObj->Dexterity / g_ClassCalc.m_Data[lpObj->Class].PvPDefenseRateDiv));
	}
#else
	int iTargetTotalLevel = lpTargetObj->Level + lpTargetObj->m_nMasterLevel;
	// ----
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = iTargetTotalLevel * 2 + lpTargetObj->Dexterity / 2;
	}
	else if( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = iTargetTotalLevel * 2 + lpTargetObj->Dexterity / 2;
	}
	else if( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = iTargetTotalLevel * 2 + lpTargetObj->Dexterity / 10;
	}
	else if( lpTargetObj->Class == CLASS_MAGUMSA )
	{
		iDefenseRate = iTargetTotalLevel * 2 + lpTargetObj->Dexterity / 4;
	}
	else if( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = iTargetTotalLevel * 2 + lpTargetObj->Dexterity / 4;
	}
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = iTargetTotalLevel * 2 + lpTargetObj->Dexterity / 2;
	}
	else if( lpTargetObj->Class == CLASS_MONK )
	{
		iDefenseRate = (iTargetTotalLevel * 1.5f) + lpTargetObj->Dexterity / 5;
	}
#endif
	// ----
	if( iAttackRate <= 0.0f || iDefenseRate <= 0.0f || lpObj->Level <= 0 || lpTargetObj->Level <= 0 )
	{
		return false;
	}
	// ----
	iAttackRate		+= lpObj->m_ItemOptionExFor380.OpAddAttackSuccessRatePVP;
	iDefenseRate	+= lpTargetObj->m_ItemOptionExFor380.OpAddDefenseSuccessRatePvP;
	// ----
	iAttackRate		+= lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP;
	iDefenseRate	+= lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddDefenseSuccessRatePvP;
	// ----
	iAttackRate		+= lpObj->m_MPSkillOpt.MpsPVPAttackDmgRate;//lpObj->m_MLPassiveSkill.m_iML_AttackRate;
	iDefenseRate	+= lpTargetObj->m_MPSkillOpt.MpsPVPBlockingRate;//lpTargetObj->m_MLPassiveSkill.m_iML_IncreasePvPDefenceRate;
	// ----
	float iExpressionA = (iAttackRate * 10000.0f) / (iAttackRate + iDefenseRate);
	float iExpressionB = (iTotalLevel * 10000) / (iTotalLevel + iTargetTotalLevel);
	// ----
	iExpressionA /= 10000.0f;
	iExpressionB /= 10000.0f;
	// ----
	iAttackSuccessRate = 100.0f * iExpressionA * g_fSuccessAttackRateOption * iExpressionB;
	// ----
	if( (iTargetTotalLevel - iTotalLevel) >= 100 )
	{
		iAttackSuccessRate -= 5;
	}
	else if( (iTargetTotalLevel - iTotalLevel) >= 200 )
	{
		iAttackSuccessRate -= 10;
	}
	else if( (iTargetTotalLevel - iTotalLevel) >= 300 )
	{
		iAttackSuccessRate -= 15;
	}
	// ----
	DWORD dwRate = rand() % 100;
	// ----
	if( dwRate > iAttackSuccessRate )
	{
		int MsgDamage = 0;	//1.01.00 update
		// ----
		if( lpObj->Class == CLASS_MONK && byBarrageCount )
		{
			if( skill != 261 && skill != 263 )
			{
				if( byBarrageCount % 4 )
				{
					MsgDamage |= 0x10;
				}
				else
				{
					MsgDamage |= 0x20;
				}
			}
			else
			{
				if( byBarrageCount % 2 )
				{
					MsgDamage |= 0x10;
				}
				else
				{
					MsgDamage |= 0x20;
				}
			}
		}
		// ----
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
		// ----
		if( g_bShieldComboMissOptionOn )
		{
			if( lpObj->comboSkill.ProgressIndex >= 0 )
			{
				LogAddTD("[Shield] ComboSkill Cancel! [%s][%s]", lpObj->AccountID, lpObj->Name);
				lpObj->comboSkill.Init();
			}
		}
		// ----
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

int CObjBaseAttack::GetTargetDefense(LPOBJ lpObj, LPOBJ lpTargetObj, BYTE & MsgDamage) //Identical gs-cs 56
{
	int targetdefense = lpTargetObj->m_Defense;

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		targetdefense += lpTargetObj->m_ItemOptionExFor380.OpAddDefense / 2;
	}

	int iSkillMagumReduceDefense = 0;
	iSkillMagumReduceDefense = gObjGetTotalValueOfEffect(lpTargetObj, ADD_OPTION_REDUCE_DEFENSE);
	targetdefense -= (targetdefense * iSkillMagumReduceDefense)/100;
	
	if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefenseTime > 0 )
	{
		targetdefense += lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefense;

		if ( targetdefense <0 )
		{
			targetdefense = 0;
		}
	}

	int percentdamage = 0; //loc4

	if ( lpObj->pInventory[7].IsItem() != FALSE )
	{
		percentdamage = lpObj->pInventory[7].IsWingOpGetOnePercentDamage();
		percentdamage += lpObj->pInventory[7].IsThirdWingOpGetOnePercentDamage();
#ifdef NEWWINGS
		percentdamage += lpObj->pInventory[7].IsNewWingOpGetOnePercentDamage();
#endif
	}

	percentdamage += lpObj->SetOpIgnoreDefense;

	if ( percentdamage != 0)
	{
		if ( (rand()%100) <= percentdamage)
		{
			targetdefense = 0;
			MsgDamage = 1;
		}
	}

	return targetdefense;
}
// -------------------------------------------------------------------------------

int CObjBaseAttack::GetPartyMemberCount(LPOBJ lpObj)
{
	LPOBJ lpPartyObj;
	int partynum = lpObj->PartyNumber;

	if ( OBJMAX_RANGE(partynum) == FALSE )
	{
		LogAdd("error : DecPartyMemberHPandMP %s %d", __FILE__, __LINE__);
		return 0;
	}

	int partycount = gParty.m_PartyS[partynum].Count;
	int retcount = 0;

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		int memberindex = gParty.m_PartyS[partynum].Number[n];

		if ( memberindex >= 0 )
		{
			lpPartyObj = &gObj[memberindex];

			if ( lpObj->MapNumber == lpPartyObj->MapNumber )
			{
				int dis = gObjCalDistance(lpObj, &gObj[memberindex]);
				
				if ( dis < MAX_PARTY_DISTANCE_EFFECT )
				{
					retcount++;
				}
			}
		}
	}

	return retcount;
}
// -------------------------------------------------------------------------------

void ComboSkillData::Init() //forced location
{
	this->dwTime = 0;
	this->Skill[0] = -1;
	this->Skill[1] = -1;
	this->Skill[2] = -1;
	this->ProgressIndex = -1;
}
// -------------------------------------------------------------------------------