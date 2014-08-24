//-> Revised by DarkSim | 27.06.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "DarkSpirit.h"
#include "MapClass.h"
#include "GameMain.h"
#include "protocol.h"
#include "BattleSoccerManager.h"
#include "gObjMonster.h"
#include "ObjAttack.h"
#include "ChaosCastle.h"
#include "IllusionTempleEvent.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"
#include "ItemAddOption.h"
#include "ImperialGuardian.h"
#ifdef __CUSTOMS__
#include "ClassCalc.h"
#endif
// -------------------------------------------------------------------------------

CDarkSpirit gDarkSpirit[OBJMAX];
// -------------------------------------------------------------------------------

//0053bf30	-> 100%
CDarkSpirit::CDarkSpirit() : m_ActionMode(PetItem_Mode_Normal)
{
	this->Init();
}
// -------------------------------------------------------------------------------

//0053c000	-> 100%
CDarkSpirit::~CDarkSpirit()
{
	return;
}
// -------------------------------------------------------------------------------

//0053c030	-> 100%
void CDarkSpirit::Init()
{
	this->m_AttackDamageMin		= 0;
	this->m_AttackDamageMax		= 0;
	this->m_AttackSpeed			= 0;
	this->m_SuccessAttackRate	= 0;
	this->m_dwLastAttackTime	= 0;
	this->m_iMasterIndex		= -1;
	this->m_iTargetIndex		= -1;
	this->m_pPetItem			= 0;
}
// -------------------------------------------------------------------------------

//0053c0b0	-> 100%
void CDarkSpirit::Run()
{
	if( this->m_iMasterIndex == -1 )
	{
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	// ----
	if( lpObj->Class != CLASS_DARKLORD )
	{
		return;
	}
	// ----
	if( this->m_pPetItem == 0 )
	{
		return;
	}
	// ----
	if( this->m_pPetItem->m_Durability <= 0.0f )
	{
		return;
	}
	// ----
	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	// ----
	if( (attr&1) != 0 )
	{
		return;
	}
	// ----
	if( this->m_pPetItem->m_RequireLeaderShip != 0 && (lpObj->Leadership + lpObj->AddLeadership < this->m_pPetItem->m_RequireLeaderShip) )
	{
		return;
	}
	// ----
	if( this->m_dwLastAttackTime > GetTickCount() )
	{
		return;
	}
	// ----
	this->m_dwLastAttackTime = (GetTickCount() + 1500) - (this->m_AttackSpeed * 10);
	// ----
	switch(this->m_ActionMode)
	{
	case PetItem_Mode_Normal:
		{
			this->ModeNormal();
		}
		break;
		// --
	case PetItem_Mode_Attack_Random:
		{
			this->ModeAttackRandom();
		}
		break;
		// --
	case PetItem_Mode_Attack_WithMaster:
		{
			this->ModeAttackWithMaster();
		}
		break;
		// --
	case PetItem_Mode_Attack_Target:
		{
			this->ModeAttakTarget();
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//0053c290	-> 100%
void CDarkSpirit::ModeNormal()
{
	LPOBJ lpObj = &gObj[this->m_iMasterIndex];	//mmm...
}
// -------------------------------------------------------------------------------

//0053c2d0	-> 100%
void CDarkSpirit::ModeAttackRandom()
{
	LPOBJ lpObj			= &gObj[this->m_iMasterIndex];
	int tObjNum;
	int count			= 0;
	int FindObj[MAX_VIEWPORT];
	int FindObjCount	= 0;
	int dis;
	int DuelIndex		= lpObj->m_iDuelUser;
	BOOL EnableAttack;
	int criticaldamage	= 0;
	int iActionType		= 0;
	// ----
	if( this->m_CriticalAttackRate + lpObj->m_MPSkillOpt.MpsDarkSpiritStr2 > rand() % 100 )
	{
		criticaldamage	= 1;
		iActionType		= 1;	
	}
	// ----
	if( this->m_ExcellentAttackRate + lpObj->m_MPSkillOpt.MpsDarkSpiritStr3 > rand() % 100 )
	{
		criticaldamage	= 2;
		iActionType		= 1;
	}
	// ----
	while(true)
	{
		if( lpObj->VpPlayer2[count].state != 0 )
		{
			if( lpObj->VpPlayer2[count].type == OBJ_MONSTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				// ----
				if( tObjNum >= 0 )
				{
					EnableAttack = false;
					// ----
					if( gObj[tObjNum].Life > 0.0f && (gObj[tObjNum].Class < 100 || gObj[tObjNum].Class  > 110 ) )
					{
						if( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
						{
							EnableAttack = true;
						}
						else if( DuelIndex == tObjNum )
						{
							EnableAttack = true;
						}
						else
						{
							int CallMonIndex = gObj[tObjNum].m_Index;
							// ----
							if( gObj[tObjNum].Type == OBJ_MONSTER )
							{
								if( gObj[tObjNum].m_RecallMon >= 0 )
								{
									CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index; 
								}
							}
							// ----
							if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == true )
							{
								EnableAttack = true;
							}
						}
						// ----
#ifdef IMPERIAL
						if( g_ImperialGuardian.IsEventMap(gObj[tObjNum].MapNumber) )
						{
							if( gObj[tObjNum].Live && g_ImperialGuardian.IsAttackAbleMonster(gObj[tObjNum].m_Index) )
							{
								EnableAttack = true;
							}
						}
#endif
						// ----
						if( EnableAttack != false )
						{
							if( lpObj->MapNumber == gObj[tObjNum].MapNumber )
							{
								dis = gObjCalDistance(lpObj, &gObj[tObjNum]);
								// ---
								if( dis < RAVEN_ATTACK_DISTANCE-2 )
								{
									FindObj[FindObjCount] = tObjNum;
									FindObjCount++;
								}
							}
						}
					}
				}
			}
		}
		// ----
		count++;
		// ----
		if( count > MAX_VIEWPORT-1 )
		{
			break;
		}
	}
	// ----
	if( FindObjCount != 0 )
	{
		if( criticaldamage != 0 )
		{
			int targetindex = FindObj[rand() % FindObjCount];
			this->SendAttackMsg(lpObj->m_Index, targetindex, criticaldamage, iActionType);
		}
		else
		{
			int targetindex = FindObj[rand() % FindObjCount];
			this->RangeAttack(lpObj->m_Index, targetindex);
		}
	}
}
// -------------------------------------------------------------------------------

//0053c860	-> 100%
void CDarkSpirit::ModeAttackWithMaster()
{
	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	int count	= 0;
	int dis;
	// ----
	if( this->m_iTargetIndex == -1 )
	{
		return;
	}
	// ----
	if( lpObj->MapNumber == gObj[this->m_iTargetIndex].MapNumber )
	{
		if( gObj[this->m_iTargetIndex].Life > 0.0f )
		{
			dis = gObjCalDistance(lpObj, &gObj[this->m_iTargetIndex]);
			// ----
			if( dis < RAVEN_ATTACK_DISTANCE )
			{
				int criticaldamage	= 0;
				int iActionType		= 0;
				// ----
				if( this->m_CriticalAttackRate + lpObj->m_MPSkillOpt.MpsDarkSpiritStr2 > rand() % 100 )
				{
					criticaldamage	= 1;
					iActionType		= 1;	
				}
				// ----
				if( this->m_ExcellentAttackRate + lpObj->m_MPSkillOpt.MpsDarkSpiritStr3 > rand() % 100 )
				{
					criticaldamage	= 2;
					iActionType		= 1;
				}
				// ----
				if( criticaldamage != false )
				{
					this->SendAttackMsg(lpObj->m_Index, this->m_iTargetIndex, criticaldamage, iActionType);
				}
				else
				{
					this->RangeAttack(lpObj->m_Index, this->m_iTargetIndex);
				}
			}
		}
		else
		{
			this->ReSetTarget(this->m_iTargetIndex);
		}
	}
}
// -------------------------------------------------------------------------------

//0053ca50	-> 100%
void CDarkSpirit::ModeAttakTarget()
{
	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	int count	= 0;
	int dis;
	// ----
	if( this->m_iTargetIndex == -1 )
	{
		return;
	}
	// ----
	if( lpObj->MapNumber == gObj[this->m_iTargetIndex].MapNumber )
	{
		if ( gObj[this->m_iTargetIndex].Life > 0.0f )
		{
			dis = gObjCalDistance(lpObj, &gObj[this->m_iTargetIndex]);
			// ----
			if( dis < RAVEN_ATTACK_DISTANCE )
			{
				int criticaldamage	= 1;
				int iActionType		= 1;
				// ----
				if( criticaldamage != false )	//ahahah, and again nice trick by WZ :D
				{
					this->SendAttackMsg(lpObj->m_Index, this->m_iTargetIndex, criticaldamage, iActionType);
				}
				else
				{
					this->RangeAttack(lpObj->m_Index, this->m_iTargetIndex);
				}
			}
		}
		else
		{
			this->ReSetTarget(this->m_iTargetIndex);
		}
	}
}
// -------------------------------------------------------------------------------

//0053cbb0	-> 100%
void CDarkSpirit::RangeAttack(int aIndex, int aTargetIndex)
{
	LPOBJ lpObj		= &gObj[aIndex];
	int StartDis	= 1;
	int tObjNum;
	int count		= 0;
	int loopcount	= 0;
	int attackcheck;
	int EnableAttack;
	int HitCount	= 0;
	// ----
	this->SendAttackMsg(lpObj->m_Index, aTargetIndex, 0, 0);
	// ----
	while(true)
	{
		if( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			// ----
			if( tObjNum >= 0 && aTargetIndex != tObjNum )
			{
				EnableAttack = false;
				// ----
				if( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = true;
				}
				else if( tObjNum == aTargetIndex )
				{
					EnableAttack = true;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;
					// ----
					if( gObj[tObjNum].Type == OBJ_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 )
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					// ----
					if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == true )
					{
						EnableAttack = true;
					}
				}
				// ----
				if( EnableAttack != false )
				{
					attackcheck = false;
					// ----
					if( loopcount == 0 )
					{
						attackcheck = true;
					}
					else if( (rand()%3) == 0 )
					{
						attackcheck = true;
					}
					// ----
					if( attackcheck != false )
					{
						if( gObjCalDistance(&gObj[aTargetIndex], &gObj[tObjNum]) < RAVEN_ATTACK_DISTANCE-3 )
						{
							this->SendAttackMsg(lpObj->m_Index, tObjNum, 2, 0);
							HitCount++;
							// ----
							if( HitCount > 3 )
							{
								break;
							}
						}
					}
				}
			}				
		}
		// ----
		count++;
		// ----
		if( count > MAX_VIEWPORT-1 )
		{
			break;
		}
	}
}
// -------------------------------------------------------------------------------

struct PMSG_PET_ITEM_ATTACK_COMMAND
{
	PBMSG_HEAD h;
	BYTE PetType;
	BYTE SkillType;
	BYTE NumberH;
	BYTE NumberL;
	BYTE TargetNumberH;
	BYTE TargetNumberL;
};
// -------------------------------------------------------------------------------

//0053ce90	->
void CDarkSpirit::SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage, int iActionType)
{
	PMSG_PET_ITEM_ATTACK_COMMAND pMsg;

	if( criticaldamage != 3 )
	{
		pMsg.PetType		= 0;
		pMsg.SkillType		= (criticaldamage) ? 0 : 1;
		pMsg.NumberH		= SET_NUMBERH(aIndex);
		pMsg.NumberL		= SET_NUMBERL(aIndex);
		pMsg.TargetNumberH	= SET_NUMBERH(aTargetIndex);
		pMsg.TargetNumberL	= SET_NUMBERL(aTargetIndex);
		// ----
		pMsg.h.set((LPBYTE)&pMsg, 0xA8, sizeof(pMsg));
		// ----
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
		// ----
		if( iActionType == 1 )
		{
			criticaldamage			= 0;
			LPOBJECTSTRUCT lpObj	= &gObj[this->m_iMasterIndex];
			// ----
			if( this->m_CriticalAttackRate + lpObj->m_MPSkillOpt.MpsDarkSpiritStr2 > rand() % 100 )
			{
				criticaldamage	= 1;	
			}
			// ----
			if( this->m_ExcellentAttackRate + lpObj->m_MPSkillOpt.MpsDarkSpiritStr3 > rand() % 100 )
			{
				criticaldamage	= 2;
			}
		}
	}
	else
	{
		criticaldamage = 0;
	}
	// ----
	gObjAddAttackProcMsgSendDelay(&gObj[aIndex], 51, aTargetIndex, 600, criticaldamage, iActionType);
}
// -------------------------------------------------------------------------------

//0053d060	-> 100%
void CDarkSpirit::SetTarget(int aTargetIndex)
{
	if( this->m_iMasterIndex == -1 )
	{
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	// ----
	if( lpObj->Class != CLASS_DARKLORD )
	{
		return;
	}
	// ----
	this->m_iTargetIndex = aTargetIndex;
}
// -------------------------------------------------------------------------------

//0053d0d0	-> 100%
void CDarkSpirit::ReSetTarget(int aTargetIndex)
{
	if( this->m_iMasterIndex == -1 )
	{
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	// ----
	if( lpObj->Class != CLASS_DARKLORD )
	{
		return;
	}
	// ----
	if( aTargetIndex == this->m_iTargetIndex )
	{
		this->m_iTargetIndex = -1;
		// ----
		if( this->m_ActionMode == PetItem_Mode_Attack_Target )
		{
			this->SetMode(PetItem_Mode_Normal, -1);
		}
	}
}
// -------------------------------------------------------------------------------

struct PMSG_SEND_PET_ITEM_COMMAND
{
	PBMSG_HEAD h;
	BYTE PetType;
	BYTE Command;
	BYTE NumberH;
	BYTE NumberL;
};
// -------------------------------------------------------------------------------

//0053d160	-> 100%
void CDarkSpirit::SetMode(ePetItem_Mode mode, int iTargetindex)
{
	if( this->m_iMasterIndex == -1 )
	{
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	// ----
	if( lpObj->Class != CLASS_DARKLORD )
	{
		return;
	}
	// ----
	if( mode == PetItem_Mode_Attack_Target || this->m_ActionMode != mode)
	{
		if( mode == PetItem_Mode_Attack_Target )
		{
			if( OBJMAX_RANGE(iTargetindex) != false )
			{
				this->m_iTargetIndex = iTargetindex;
			}
			else
			{
				return;
			}
		}
		// ----
		this->m_ActionMode = mode;
		// ----
		PMSG_SEND_PET_ITEM_COMMAND pMsg;
		// ----
		pMsg.h.set((LPBYTE)&pMsg, 0xA7, sizeof(pMsg));
		pMsg.PetType = 0;
		pMsg.Command = mode;
		pMsg.NumberH = SET_NUMBERH(this->m_iTargetIndex);
		pMsg.NumberL = SET_NUMBERL(this->m_iTargetIndex);
		// ----
		DataSend(this->m_iMasterIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		// ----
		char strMode[4][255] =
		{
			"[????]",
			"[·???°?°?]",
			"[????°?°°??°?°?]",
			"[»????????¤]"
		};
	}
}
// -------------------------------------------------------------------------------

//0053d3e0	-> 100%
void CDarkSpirit::Set(int aIndex, CItem * pPetItem)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if( lpObj->Class != CLASS_DARKLORD )
	{
		return;
	}
	// ----
	BOOL bChange = false;
	// ----
	if( this->m_iMasterIndex == -1 )
	{
		bChange = true;
	}
	// ----
	this->Init();
	// ----
	if( pPetItem->m_Type != ITEMGET(13, 5) )
	{
		return;
	}
	// ----
	if( pPetItem->m_Durability <= 0.0f )
	{
		return;
	}
	// ----
	int petitemlevel				= pPetItem->m_PetItem_Level;
	int leadership					= lpObj->Leadership + lpObj->AddLeadership;
	// ----
#ifdef __CUSTOMS__
	this->m_AttackDamageMin			= petitemlevel * g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackDamageMinFact + leadership / g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackDamageMinDiv + g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackDamageMinStart;
	this->m_AttackDamageMax			= petitemlevel * g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackDamageMaxFact + leadership / g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackDamageMaxDiv + g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackDamageMaxStart;
	this->m_CriticalAttackRate		= (float)g_ClassCalc.m_Data[CLASS_DARKLORD].PetCriticalAttackRate;
	this->m_AttackSpeed				= petitemlevel * g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackSpeedFact / g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackSpeedDiv1 + leadership / g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackSpeedDiv2 + g_ClassCalc.m_Data[CLASS_DARKLORD].PetAttackSpeedStart;
	this->m_SuccessAttackRate		= petitemlevel + petitemlevel / g_ClassCalc.m_Data[CLASS_DARKLORD].PetSuccessAttackRateDiv + g_ClassCalc.m_Data[CLASS_DARKLORD].PetSuccessAttackRateStart;
#else
	this->m_AttackDamageMin			= petitemlevel * 15 + leadership / 8 + 180;
	this->m_AttackDamageMax			= petitemlevel * 15 + leadership / 4 + 200;
	this->m_CriticalAttackRate		= 30.0f;
	this->m_AttackSpeed				= petitemlevel * 4 / 5 + leadership / 50 + 20;
	this->m_SuccessAttackRate		= petitemlevel + petitemlevel / 15 + 1000;
#endif
	// ----
	this->m_ExcellentAttackRate		= 0;
	this->m_Unknown28				= 0;
	this->m_Unknown32				= 0;
	this->m_iMasterIndex			= aIndex;
	this->m_pPetItem				= pPetItem;
	// ----
	if( bChange != false )
	{
		this->SetMode(PetItem_Mode_Normal, -1);
	}
}
// -------------------------------------------------------------------------------

//0053d5c0	->
BOOL CDarkSpirit::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, int criticaldamage, int iActionType)
{
	int skillSuccess = 0;
	LPOBJ lpCallObj;
	LPOBJ lpCallTargetObj;
	BYTE MsgDamage = 0;
	int ManaChange = 0;
	int iTempShieldDamage = 0;
	int iTotalShieldDamage = 0;

	if ( (lpTargetObj->Authority&2) == 2 )
	{
		return false;
	}

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
	{
		return false;
	}

#ifdef IMPERIAL
	if( g_ImperialGuardian.IsEventMap(lpObj->MapNumber) )
	{
		if( !g_ImperialGuardian.IsAttackAbleMonster(lpTargetObj->m_Index) )
		{
			return false;
		}
	}
#endif

//#if(_GSCS==1)
	if( g_Crywolf.GetCrywolfState() == 3 || g_Crywolf.GetCrywolfState() == 5 )
	{
		if(CRYWOLF_MAP_RANGE(lpTargetObj->MapNumber))
		{
			if(lpTargetObj->Type == OBJ_MONSTER)
			{
				return FALSE;
			}
		}
	}
//#endif

	int skill = 0;
	
	if ( lpMagic )
	{
		skill = lpMagic->m_Skill;
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
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
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

			return TRUE;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpObj))
		{
			return FALSE;
		}
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpTargetObj))
		{
			return FALSE;
		}
	}

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
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
	{
		return FALSE;
	}

//#if(_GSCS==1)
	if ( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		if( lpObj->m_btCsJoinSide > 0 )
		{
			if( lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide )
			{
				return FALSE;
			}
		}
	}
//#endif

	lpObj->m_TotalAttackCount++;

	if ( this->CheckAttackArea(lpObj, lpTargetObj) == FALSE )
	{
		return FALSE;
	}

	lpCallObj = lpObj;
	lpCallTargetObj = lpTargetObj;

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( this->PkCheck(lpCallObj, lpTargetObj) == FALSE )
	{
		return FALSE;
	}

	int Strength = lpObj->Strength + lpObj->AddStrength;
	int Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
	int Vitality = lpObj->Vitality + lpObj->AddVitality;
	int Energy = lpObj->Energy + lpObj->AddEnergy;
	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

	if ( bIsOnDuel )
	{
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}

	int MSBFlag = 0;
	
	if( criticaldamage == 1 )
	{
		MsgDamage = 3;
	}
	else if( criticaldamage == 2 )
	{
		MsgDamage = 2;
	}
	else
	{
		MsgDamage = 0;
	}

	BOOL bAllMiss = FALSE;
	int AttackDamage = 0;

	if ( g_ShieldSystemOn == TRUE )
	{
		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			if ( !this->MissCheckPvP(lpObj, lpTargetObj, skill, skillSuccess, bAllMiss) )
			{
				return FALSE;
			}
		}
		else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, bAllMiss) )
		{
			return FALSE;
		}

	}
	else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, bAllMiss) )
	{
		return FALSE;
	}

	int targetdefense = this->GetTargetDefense(lpObj, lpTargetObj, MsgDamage);
	AttackDamage = this->GetAttackDamage(lpObj, targetdefense, bIsOnDuel, criticaldamage);

if (criticaldamage == 0 && iActionType == 0)
	{
		AttackDamage /= 1.5;
	}
	
	if (criticaldamage == 2)
    {
		AttackDamage += AttackDamage * 20.0f / 100.0f;
	}

	if (bAllMiss)
	{
		AttackDamage = (AttackDamage * 30) / 100;
	}
	
	if (lpTargetObj->DamageMinus)
	{
		int beforeDamage = AttackDamage;
		AttackDamage	-= ((AttackDamage * (int)lpTargetObj->DamageMinus) / 100);
	}

	int tlevel = lpObj->Level / 10;

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

	lpObj->pInventory[1].m_DurabilitySmall += AttackDamage * 2 / 100;

	int Defense = 800 + (800 * gIncreaseDarkSpiritDefencePercent / 100);

	if( lpObj->pInventory[1].m_DurabilitySmall >= (800 + lpObj->m_MPSkillOpt.MpsPetDurDownSpeed) )
	{
		lpObj->pInventory[1].m_DurabilitySmall	 = 0;
		lpObj->pInventory[1].m_Durability		-= 1.0f;
		
		if (lpObj->pInventory[1].m_Durability < 1.0f)
		{
			lpObj->pInventory[1].m_Durability = 0;

			int exp = lpObj->pInventory[1].m_PetItem_Exp;
			
			if (lpObj->pInventory[1].DecPetItemExp(10))
			{
				LogAddTD("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%d]DecExp[%d]", lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[1].GetName(),	lpObj->pInventory[1].m_PetItem_Level, lpObj->pInventory[1].m_PetItem_Exp,
					exp - lpObj->pInventory[1].m_PetItem_Exp);
				
				this->Set(lpObj->m_Index, &lpObj->pInventory[1]);
				this->SendLevelmsg(lpObj->m_Index, 1, 0, 0xFF);
			}
			
			LogAddTD("[%s][%s] ´ÙÅ©½ºÇÇ¸´ÀÇ¹ßÅé Item is Broken because durability is exhausted [%d]", lpObj->AccountID, lpObj->Name,
				lpObj->pInventory[1].m_Number);
		}
		
		GCItemDurSend(lpObj->m_Index, 1, lpObj->pInventory[1].m_Durability, 0);
	}

	if ( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if ( AttackDamage > 1 )
		{
			float  damage = (AttackDamage * 8) / 10.0f;
			AttackDamage = damage;
		}
	}

	if ( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem * Wing = &lpTargetObj->pInventory[7];

		if ( AttackDamage > 1 )
		{
			if (Wing->m_Type == ITEMGET(12,36) || 
				Wing->m_Type == ITEMGET(12,37) || 
				Wing->m_Type == ITEMGET(12,38) || 
				Wing->m_Type == ITEMGET(12,39))
			{
				float damage = (float)(AttackDamage * (61 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
			else if (Wing->m_Type == ITEMGET(12,40))
			{
				float damage = (float)(AttackDamage * (76 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
			else if (Wing->m_Type == ITEMGET(12,41))
			{
				float damage = (float)(AttackDamage * (88 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
			else if (Wing->m_Type == ITEMGET(12,42))
			{
				float damage = (float)(AttackDamage * (75 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
			else if (Wing->m_Type == ITEMGET(12,43))
			{
				float damage = (float)(AttackDamage * (61 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
			else if (Wing->m_Type == ITEMGET(12,50))
			{
				float damage = (float)(AttackDamage * (76 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
			else if (Wing->m_Type > ITEMGET(12,2))
			{
				float damage = (float)(AttackDamage * (75 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
#ifdef NEWWINGS
			else if( IS_NEWWINGS(Wing->m_Type) )
			{
				float damage = (float)(AttackDamage * (50 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);
			}
#endif
			else
			{
				float damage = (float)(AttackDamage * (88 - (Wing->m_Level*2))) / 100.0f;
				AttackDamage = (int)(damage);	//  #formula
			}
		}
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

		GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
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

		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
	}

	if ( lpTargetObj->Live )
	{
		if(gObjCheckUsedBuffEffect(lpTargetObj, AT_WIZARD_DEFENSE) == 1 && AttackDamage > 0)
		{
			int iWizardSkillDefense = 0;
			int iWizardSkillTime = 0;

			gObjCheckUsedBuffEffect(lpTargetObj, AT_WIZARD_DEFENSE, &iWizardSkillDefense, &iWizardSkillTime);

			int iReplacementMana = 0;

			if(iWizardSkillTime > 0)
			{
				iReplacementMana = (WORD)lpTargetObj->Mana * iWizardSkillTime / 1000;
			}
			else
			{
				iReplacementMana = (WORD)lpTargetObj->Mana * 2 / 100;
			}

			if ( iReplacementMana < lpTargetObj->Mana )
			{
				lpTargetObj->Mana -= iReplacementMana;

				int decattackdamage = 0; //200

				if(iWizardSkillDefense > 0)
				{
					decattackdamage = AttackDamage * iWizardSkillDefense / 100;
				}

				AttackDamage -= decattackdamage;
				ManaChange = TRUE;
			}
		}

		if ( g_ShieldSystemOn == FALSE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( CC_MAP_RANGE(lpObj->MapNumber ) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}

//#if(_GSCS==1)
		if ( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE )
				{
					if( lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
					{
						AttackDamage = AttackDamage * 20 / 100;
					}
					else if(g_ShieldSystemOn == FALSE)
					{
						AttackDamage = AttackDamage * 40 / 100;
					}
				}
			}
		}
//#endif

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( lpTargetObj->Class == 283 )
			{
				if(gObjCheckUsedBuffEffect(lpObj, AT_POTION_OF_BLESS) != 0 || gObjCheckUsedBuffEffect(lpObj, AT_POTION_OF_SOUL) != 0)
				{
					AttackDamage += (AttackDamage * 20) / 100;
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

					AttackDamage = AttackDamage * 20 / 100;
				}
			}

			if ( lpTargetObj->Class == 277 )
			{
				if(gObjCheckUsedBuffEffect(lpObj, AT_POTION_OF_BLESS) != 0 || gObjCheckUsedBuffEffect(lpObj, AT_POTION_OF_SOUL) != 0)
				{
					AttackDamage += (AttackDamage * 20) / 100;
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

					AttackDamage = AttackDamage * 20 / 100;
				}
			}
		}

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
			lpTargetObj->iShield -= iTempShieldDamage;
			lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
			iTotalShieldDamage += iTempShieldDamage;

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
			lpTargetObj->m_Agro.IncAgro(lpObj->m_Index, AttackDamage / 100);
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
			else if ( CC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
			else
			{
				selfdefense = 1;
			}
			if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
			{
				selfdefense = FALSE;
			}
//#if(_GSCS==1)
			if ( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				if( lpObj->m_btCsJoinSide > 0 )
				{
					selfdefense = FALSE;
				}
			}
//#endif
			else if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) ) //season 2.5 add-on
			{
				selfdefense = 0;
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
		}

		if(gObjCheckUsedBuffEffect(lpTargetObj, AT_ICE_ARROW) == 1)
		{
			gObjRemoveBuffEffect(lpTargetObj, AT_ICE_ARROW);
			GCMagicCancelSend(lpTargetObj, 51);
		}
	}

	if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
	{
		if(lpCallObj->PartyNumber >= 0)
		{
			if(lpObj->PartyNumber == lpCallObj->PartyNumber)
			{
				selfdefense = FALSE;
			}
		}

	}

	if ( selfdefense == TRUE )
	{
		if ( !gObjTargetGuildWarCheck(lpObj, lpCallObj) )
		{
			if(lpCallObj->PartyNumber >= 0) //Season 2.5 add-on
			{
				//season3 removed party for LOL
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( (gParty.GetPKPartyPenalty(partynum)) < 5)
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

	if ( AttackDamage >= 5 )
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
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_Change == 9 )
		{
			GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);
		}
	}

	lpObj->m_Rest = 0;

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

		AttackDamage -= iTotalShieldDamage;

		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill, iTotalShieldDamage);
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

	return TRUE;
}
// -------------------------------------------------------------------------------

//0053f160	-> 100%
int CDarkSpirit::GetAttackDamage(LPOBJ lpObj, int targetDefense, BOOL bIsOnDuel, int crititcaldamage)
{
	if( g_ShieldSystemOn == true )
	{
		if( bIsOnDuel == true )
		{
			bIsOnDuel = false;
		}
	}
	// ----
	int ad;
	int sub = this->m_AttackDamageMax - this->m_AttackDamageMin;
	int AttackDamage;
	// ----
	if( crititcaldamage )
	{
		AttackDamage = this->m_AttackDamageMin + sub;
	}
	else
	{
		AttackDamage = this->m_AttackDamageMin + (rand()%(sub+1));
	}
	// ----
	if( (lpObj->pInventory[0].m_Type >= ITEMGET(2,8) && lpObj->pInventory[0].m_Type <= ITEMGET(2,12) ) ||
		lpObj->pInventory[0].m_Type == ITEMGET(2,13) || 
		lpObj->pInventory[0].m_Type == ITEMGET(2,14) || 
		lpObj->pInventory[0].m_Type == ITEMGET(2,15) ||
		lpObj->pInventory[0].m_Type == ITEMGET(2,17) ||
		lpObj->pInventory[0].m_Type == ITEMGET(2,18) )
	{
		int addpetdamage = lpObj->pInventory[0].m_Magic / 2;
		addpetdamage	-= addpetdamage * lpObj->pInventory[0].m_CurrentDurabilityState;
		addpetdamage	= AttackDamage * addpetdamage / 100;
		AttackDamage	+= addpetdamage;
	}
	// ----
	if( bIsOnDuel )
	{
		ad = AttackDamage * 60 / 100 - targetDefense;
	}
	else
	{
		ad = AttackDamage - targetDefense;
	}
	// ----
	if( lpObj->m_MPSkillOpt.MpsDarkSpiritStr1 > 0.0f )
	{
		AttackDamage += int(lpObj->m_MPSkillOpt.MpsDarkSpiritStr1);
	}
	// ----
	if( lpObj->m_MPSkillOpt.MpsUseScepterPetStr > 0.0f )
	{
		AttackDamage += int(lpObj->m_MPSkillOpt.MpsUseScepterPetStr);
	}
	// ----
	ad += ad * gIncreaseDarkSpiritAttackDamagePercent / 100;
	// ----
	return ad;
}
// -------------------------------------------------------------------------------

//0053f3d0	-> 100%
BOOL CDarkSpirit::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, BOOL & bAllMiss)
{
	int SuccessAttackRate			= 0;
	int TargetSuccessfulBlocking	= lpTargetObj->m_SuccessfulBlocking;
	int MsgDamage					= 0;
	// ----
	SuccessAttackRate				+= lpObj->m_MPSkillOpt.MpsAttackSuccessRate;
	// ----
	if( this->m_SuccessAttackRate < TargetSuccessfulBlocking )
	{
		bAllMiss = true;
	}
	// ----
	if( IT_MAP_RANGE(lpTargetObj->MapNumber) != false )
	{
		if( g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == 2 )
		{
			if( lpTargetObj->Type == OBJ_USER )
			{
				if( g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber) != false )
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
	if( bAllMiss != false )
	{
		if( (rand()%100) >= 5 )
		{
			GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
			return false;
		}
	}
	else if( this->m_SuccessAttackRate )
	{
		if( (rand()%this->m_SuccessAttackRate) < TargetSuccessfulBlocking)
		{
			GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//0053f600	-> 100%
BOOL CDarkSpirit::MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, BOOL& bAllMiss)
{
	float iAttackRate		= 0;
	float iDefenseRate		= 0;
	int iAttackSuccessRate	= 0;
	// ----
	if( IT_MAP_RANGE(lpTargetObj->MapNumber) != false )
	{
		if( g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == 2 )
		{
			if( lpTargetObj->Type == OBJ_USER )
			{
				if( g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber) != false )
				{
					GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
					return false;
				}	
			}
			if( lpObj->PartyNumber == lpTargetObj->PartyNumber )
			{
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return false;
			}
		}
	}
	// ----
	int iBaseLevel = lpObj->Level + lpObj->m_nMasterLevel;
	// ----
	if( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = iBaseLevel * 3 + ( lpObj->Dexterity * 45) / 10;
	}
	else if ( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = iBaseLevel * 3 + ( lpObj->Dexterity * 4 );
	}
	else if( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = iBaseLevel * 3 + ( lpObj->Dexterity * 6 ) / 10;
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		iAttackRate = iBaseLevel * 3 + ( lpObj->Dexterity * 35 ) / 10;
	}
	else if( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = iBaseLevel * 3 + ( lpObj->Dexterity * 4 );
	}
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = iBaseLevel * 3 + ( lpObj->Dexterity * 35 ) / 10;
	}
#ifdef MONK
	else if( lpTargetObj->Class == CLASS_MONK )
	{
		iAttackRate = iBaseLevel * 3 + ( lpObj->Dexterity * 48 ) / 10;
	}
#endif
	int iTargetBaseLevel = lpTargetObj->Level + lpTargetObj->m_nMasterLevel;
	// ----
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = iTargetBaseLevel * 2 + lpTargetObj->Dexterity / 2;
	}
	else if( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = iTargetBaseLevel * 2 + lpTargetObj->Dexterity / 2;
	}
	else if( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = iTargetBaseLevel * 2 + lpTargetObj->Dexterity / 10;
	}
	else if( lpTargetObj->Class == CLASS_MAGUMSA )
	{
		iDefenseRate = iTargetBaseLevel * 2 + lpTargetObj->Dexterity / 4;
	}
	else if( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = iTargetBaseLevel * 2 + lpTargetObj->Dexterity / 4;
	}
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = iTargetBaseLevel * 2 + lpTargetObj->Dexterity / 2;
	}
#ifdef MONK
	else if( lpTargetObj->Class == CLASS_MONK )
	{
		iDefenseRate = iTargetBaseLevel * 2 + lpTargetObj->Dexterity / 5;
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
	iAttackRate		+= lpObj->m_MPSkillOpt.MpsPVPAttackDmgRate;
	iDefenseRate	+= lpTargetObj->m_MPSkillOpt.MpsPVPBlockingRate;
	// ----
	float iExpressionA	= ( iAttackRate * 10000.0f ) / ( iAttackRate + iDefenseRate );
	float iExpressionB	= ( iBaseLevel * 10000 ) / ( iBaseLevel + iTargetBaseLevel );
	iExpressionA		/= 10000.0f;
	iExpressionB		/= 10000.0f;
	// ----
	iAttackSuccessRate = 100.0f * iExpressionA * g_fSuccessAttackRateOption * iExpressionB;
	// ----
	if( (iTargetBaseLevel - iBaseLevel) >= 100 )
	{
		iAttackSuccessRate -= 5;
	}
	else if( (iTargetBaseLevel - iBaseLevel) >= 200 )
	{
		iAttackSuccessRate -= 10;
	}
	else if( (iTargetBaseLevel - iBaseLevel) >= 300 )
	{
		iAttackSuccessRate -= 15;
	}
	// ----
	DWORD dwRate = rand() % 100;
	// ----
	if( dwRate > iAttackSuccessRate )
	{
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//0053fd50	-> 100%
void CDarkSpirit::ChangeCommand(int command, int targetindex)
{
	CDarkSpirit::ePetItem_Mode setmode = PetItem_Mode_Normal;
	// ----
	if( command == PetItem_Mode_Normal )
	{
		setmode = PetItem_Mode_Normal;
	}
	else if( command == PetItem_Mode_Attack_Random )
	{
		setmode = PetItem_Mode_Attack_Random;
	}
	else if( command == PetItem_Mode_Attack_WithMaster )
	{
		setmode = PetItem_Mode_Attack_WithMaster;
	}
	else if( command == PetItem_Mode_Attack_Target )
	{
		setmode = PetItem_Mode_Attack_Target;
	}
	// ----
	this->SetMode(setmode, targetindex);
}
// -------------------------------------------------------------------------------

//0053fdd0	-> 100%
void CDarkSpirit::SendLevelmsg(int aIndex, int nPos, int PetType, int InvenType)
{
	PMSG_SEND_PET_ITEMINFO pMsg;
	// ----
	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));
	pMsg.PetType	= PetType;
	pMsg.InvenType	= 0xFE;
	pMsg.nPos		= nPos;
	pMsg.Level		= gObj[aIndex].pInventory[nPos].m_PetItem_Level;
	pMsg.Exp		= gObj[aIndex].pInventory[nPos].m_PetItem_Exp;
	pMsg.Life		= gObj[aIndex].pInventory[nPos].m_Durability;
	// ----
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -------------------------------------------------------------------------------

//0053fed0	-> 100%
int CDarkSpirit::GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{
	int iShieldDamage = 0;
	// ----
	if( g_ShieldSystemOn == false )
	{
		return 0;
	}
	// ----
	if( iAttackDamage <= 0 )
	{
		return 0;
	}
	// ----
	int iReduceLife				= 0;
	int iReduceShield			= 0;
	int iReduceLifeForEffect	= 0; 
	bool bReduceShieldGage		= 0;
	int iDamageDevideToSDRate	= g_iDamageDevideToSDRate;
	iDamageDevideToSDRate		-= lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;
	iDamageDevideToSDRate		+= lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate;
	// ----
	if( iDamageDevideToSDRate < 0 )
	{
		iDamageDevideToSDRate = 0;
	}
	// ----
	if( iDamageDevideToSDRate > 100 )
	{
		iDamageDevideToSDRate = 100;
	}
	// ----
	if( lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand()%100;
		// ----
		if( iRand < lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
		{
			iDamageDevideToSDRate = 0;
		}
	}
	// ----
	if( lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate 
		|| lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate 
		|| lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
	{
		LogAddTD("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
			lpObj->AccountID, lpObj->Name,
			lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate, lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate, lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate, iDamageDevideToSDRate);
	}
	// ----
	iReduceShield	= iAttackDamage * iDamageDevideToSDRate / 100;
	iReduceLife		= iAttackDamage - iReduceShield;
	// ----
	if( (lpTargetObj->iShield-iReduceShield) < 0 )
	{
		iReduceLife		+= iReduceShield  - lpTargetObj->iShield;
		iReduceShield	= lpTargetObj->iShield;
		// ----
		if( lpTargetObj->iShield > 0 )
		{
			bReduceShieldGage = true;
		}
	}
	// ----
	iReduceLifeForEffect = ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) * 20.0f / 100.0f;
	// ----
	if( bReduceShieldGage == true && iReduceLife > iReduceLifeForEffect )
	{
		if( !CC_MAP_RANGE(lpTargetObj->MapNumber) )
		{
			GCSendEffectInfo(lpTargetObj->m_Index, 17);
		}
	}
	// ----
	return iReduceShield;
}
// -------------------------------------------------------------------------------