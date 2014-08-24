// TMonsterSkillManager.cpp: implementation of the TMonsterSkillManager class.
//	GS-CS	1.00.90	JPN	0xXXXXXXXX	-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TMonsterSkillManager.h"
#include "ObjUseSkill.h"
#include "SkillHitBox.h"
#include "..\include\ReadScript.h"
#include "LogProc.h"
#include "ImperialGuardian.h"
#include "GameMain.h"

TMonsterSkillInfo TMonsterSkillManager::s_MonsterSkillInfoArray[MAX_MONSTER_SKILL_INFO_ARRAY];
_ST_MONSTER_SKILL_DELAYTIME_INFO TMonsterSkillManager::s_MonsterSkillDelayInfoArray[MAX_MONSTER_SKILL_DELAY_INFO_ARRAY];
TSync TMonsterSkillManager::s_Sync;
BOOL TMonsterSkillManager::s_bDataLoad;

int gGetLowHPMonster(int nZoneIndex, int nIndex, int nDistance)
{
	OBJECTSTRUCT *lpObj = &gObj[nIndex];
	int nTargetIndex = -1;
	int nTempTargetLife = 0;
	int nTargetLife = 0;

	int nMonsterCount = g_ImperialGuardian.GetZoneInfo(nZoneIndex).vtMonsterInfo.size();

	for (int i = 0; i < nMonsterCount; ++i )
	{

		stMonsterIndexInfo monsterInfo = g_ImperialGuardian.GetZoneInfo(nZoneIndex).vtMonsterInfo.at(i);
		if ( gObjCalDistance(lpObj, &gObj[monsterInfo.ObjIndex]) >= nDistance )
			continue;

		if ( !monsterInfo.bLive )
			continue;

		int nTempTargetLife = (gObj[monsterInfo.ObjIndex].Life / gObj[monsterInfo.ObjIndex].MaxLife * 100.0);

		if ( nTempTargetLife < nTargetLife || !nTargetLife )
		{
			if ( nTempTargetLife != 100 && nTempTargetLife > 0)
			{
				nTargetLife = nTempTargetLife;
				nTargetIndex = monsterInfo.ObjIndex;
			}
		}

	}

	return nTargetIndex;
}

int gGetPartyMaxLevel(int nPartyNumber)
{
	int nResult = -1;
	for (int i = 0; i < 5; ++i )
	{
		if ( gParty.m_PartyS[nPartyNumber].Number[i] > 0 )
		{
			int nUserIndex = gParty.m_PartyS[nPartyNumber].Number[i];
			int nLevel = gObj[nUserIndex].Level + gObj[nUserIndex].m_nMasterLevel;

			if ( nResult < nLevel )
				nResult = nLevel;
		}
	}

	return nResult;
}

TMonsterSkillManager::TMonsterSkillManager()
{
	return;
}

TMonsterSkillManager::~TMonsterSkillManager()
{
	return;
}

BOOL TMonsterSkillManager::LoadData(LPSTR lpszFileName)
{
	TMonsterSkillManager::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Monster Skill Manager] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster Skill Manager] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		TMonsterSkillManager::DelAllSkillManagerInfo();
		int iType = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					BOOL bVerified = TRUE;
					char szMonsterName[20]={0};
					TMonsterSkillInfo MonsterSkillInfo;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					MonsterSkillInfo.m_iMonsterIndex = TokenNumber;

					Token = GetToken();
					memcpy(szMonsterName, TokenString, 20);

					for ( int i = 0;i < 10;i++)//Season 4.5 changed from 5 to 10
					{
						int iSkillUnitNumber = -1;
						Token = GetToken();
						MonsterSkillInfo.m_iSkillUnitTypeArray[i] = TokenNumber;

						Token = GetToken();
						iSkillUnitNumber = TokenNumber;

						if ( iSkillUnitNumber == -1 )
							continue;

						TMonsterSkillUnit * lpSkillUnit = TMonsterSkillUnit::FindSkillUnit(iSkillUnitNumber);
						
						if ( lpSkillUnit == NULL )
						{
							bVerified = FALSE;
							continue;
						}

						MonsterSkillInfo.m_lpSkillUnitArray[i] = lpSkillUnit;
						MonsterSkillInfo.m_iSkillUnitCount++;
					}



					if ( false || MonsterSkillInfo.m_iMonsterIndex >= MAX_MONSTER_SKILL_INFO_ARRAY )
					{
						MsgBox("[Monster Manager] - MonsterIndex(%d) Error (%s) File. ", 
							MonsterSkillInfo.m_iMonsterIndex, lpszFileName);
						continue;
					}

					if ( bVerified ==FALSE )
					{
						LogAddC(2, "[Monster Manager] - Invalid SkillInfo : MIndex(%s/%d)",
							szMonsterName, MonsterSkillInfo.m_iMonsterIndex,lpszFileName);
					}

					memcpy(&TMonsterSkillManager::s_MonsterSkillInfoArray[MonsterSkillInfo.m_iMonsterIndex],
						&MonsterSkillInfo, sizeof(MonsterSkillInfo));

					char szMsg[200] = {0};
					TMonsterSkillInfo * pMonSInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[MonsterSkillInfo.m_iMonsterIndex];

					wsprintf(szMsg, "¡ÛIndex:%d,Name:%s,Count:%d,Type0:%d,Unit0:%d,Type1:%d,Unit1:%d,Type2:%d,Unit2:%d,Type3:%d,Unit3:%d,Type4:%d,Unit4:%d\n",
						pMonSInfo->m_iMonsterIndex, szMonsterName, pMonSInfo->m_iSkillUnitCount,
						pMonSInfo->m_iSkillUnitTypeArray[0], pMonSInfo->m_lpSkillUnitArray[0],
						pMonSInfo->m_iSkillUnitTypeArray[1], pMonSInfo->m_lpSkillUnitArray[1],
						pMonSInfo->m_iSkillUnitTypeArray[2], pMonSInfo->m_lpSkillUnitArray[2],
						pMonSInfo->m_iSkillUnitTypeArray[3], pMonSInfo->m_lpSkillUnitArray[3],
						pMonSInfo->m_iSkillUnitTypeArray[4], pMonSInfo->m_lpSkillUnitArray[4]);

					OutputDebugString(szMsg);
				}
			}
		}

		fclose(SMDFile);

		LogAddC(2, "[Monster Skill Manager] - %s file is Loaded", lpszFileName);

		TMonsterSkillManager::s_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		MsgBox("[Monster Skill Manager] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}



void TMonsterSkillManager::DelAllSkillManagerInfo()
{
	for ( int i=0;i<MAX_MONSTER_SKILL_INFO_ARRAY;i++)
	{
		TMonsterSkillManager::s_MonsterSkillInfoArray[i].Reset();
	}

	for ( int j=0;j<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;j++)
	{
		TMonsterSkillManager::s_MonsterSkillDelayInfoArray[j].RESET();
	}
}

BOOL TMonsterSkillManager::CheckMonsterSkill(int iMonsterClass)
{
	if ( iMonsterClass < 0 || iMonsterClass > MAX_MONSTER_SKILL_INFO_ARRAY )	// #error Change to MAX_MONSTER_SKILL_INFO_ARRAY-1
		return FALSE;

	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[iMonsterClass];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return FALSE;

	return TRUE;
}


BOOL TMonsterSkillManager::AddMonsterSkillDelayInfo(int iIndex, int iTargetIndex, int iDelayTime, TMonsterSkillUnit * lpMonsterSkillUnit)
{
	for ( int i=0;i<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;i++)
	{
		if ( TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed == FALSE )
		{
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].iIndex = iIndex;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].iTargetIndex = iTargetIndex;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].dwDelayTime = GetTickCount() + iDelayTime;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].lpMonsterSkillUnit = lpMonsterSkillUnit;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed = TRUE;

			return TRUE;
		}
	}

	LogAddC(2, "[Monster Skill Manager] - (MonsterSkillDelayInfo) Slot Full!! ");
	return FALSE;
}



void TMonsterSkillManager::MonsterSkillProc() 
{
	DWORD dwCurrentTick = GetTickCount();

	for ( int i=0;i<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;i++)
	{
		if ( TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed == TRUE )
		{
			if ( dwCurrentTick < TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].dwDelayTime )
			{
				_ST_MONSTER_SKILL_DELAYTIME_INFO & stInfo = TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i];

				if ( !stInfo.IsValid() )
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				LPOBJ lpObj = &gObj[stInfo.iIndex];
				LPOBJ lpTargetObj = &gObj[stInfo.iTargetIndex];

				if ( !lpObj->Live || !lpTargetObj->Live )
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				if ( !gObjIsConnected(lpTargetObj))
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
				stInfo.lpMonsterSkillUnit->RunSkill(stInfo.iIndex, stInfo.iTargetIndex);
				TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
			}
		}
	}
}


TMonsterSkillUnit * TMonsterSkillManager::FindMonsterSkillUnit(int iIndex, int iMonsterSkillUnitType)
{
	LPOBJ lpObj = &gObj[iIndex];
	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[lpObj->Class];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return NULL;

	int iFoundSkillArray[10] = {-1};
	int iFoundSkillCount = 0;

	for(int i=0;i<10;i++)
	{
		if ( iMonsterSkillUnitType == lpMonsterSkillInfo->m_iSkillUnitTypeArray[i] )
		{
			iFoundSkillArray[iFoundSkillCount] = i;
			iFoundSkillCount++;
		}
	}

	if ( iFoundSkillCount > 0 )
	{
		int iSlot = iFoundSkillArray[rand() % iFoundSkillCount];

		if ( iSlot == -1 )
			return NULL;

		TMonsterSkillUnit * lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[iSlot];
		
		return lpMonsterSkillUnit;
	}
	
	return NULL;
}


//good
void TMonsterSkillManager::UseMonsterSkill(int iIndex, int iTargetIndex, int iMonsterSkillUnitType, int iMonsterSkillUnit,CMagicInf * lpMagic)
{
	if (iIndex >= OBJ_MAXMONSTER || iIndex < 0)
	{
		LogAddC(2,"[TMonsterSkillManager][UserMonsterSkill] error: iIndex(%d)",iIndex);
		return;
	}

	if (iTargetIndex >= OBJMAX || iTargetIndex < 0)
	{
		LogAddC(2,"[TMonsterSkillManager][UserMonsterSkill] error: iTargetIndex(%d)",iTargetIndex);
		return;
	}

	LPOBJ lpObj = &gObj[iIndex]; //loc1
	LPOBJ lpTargetObj = &gObj[iTargetIndex]; //loc2
	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[lpObj->Class]; //loc3

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return;
	
	TMonsterSkillUnit * lpMonsterSkillUnit = NULL; //loc4

	if ( iMonsterSkillUnit > -1) //arg4
	{
		int iMonsterSkill = iMonsterSkillUnit; //loc5

		lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[iMonsterSkill];
	}
	else
	{
		lpMonsterSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit(iIndex, iMonsterSkillUnitType);
	}

	if( lpMonsterSkillUnit == NULL )
	{
		LogAddC(2, "[Monster Skill Manager] SkillUnit is NULL : ObjIndex[%d], SkillUnitType[%d] ",iIndex,iMonsterSkillUnitType);
		return;
	}

	CMagicInf cMagicInf;

	if( lpObj->MapNumber >= 69 && lpObj->MapNumber <= 72 )
	{
		if( lpObj->Class == 519 && lpMonsterSkillUnit->m_iUnitNumber == 46 )
		{
			int nTargetIndex = gGetLowHPMonster(lpObj->m_ImperialZoneID, lpObj->m_Index, 6);
			// ----
			if( nTargetIndex == -1 )
			{
				GCActionSend(lpObj, 120, lpObj->m_Index, lpObj->TargetNumber);
				gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
				return;
			}
			// ----
			lpTargetObj		= &gObj[nTargetIndex];
			iTargetIndex	= nTargetIndex;
		}
	}
	// ----
	if( lpMonsterSkillUnit->m_iUnitTargetType == 5 )
	{
		lpTargetObj = &gObj[iIndex];
		iTargetIndex = iIndex;
	}
	// ----
	if( !lpMagic )
	{
		lpMagic = &cMagicInf;
		TMonsterSkillManager::FindMagicInf(lpMonsterSkillUnit, &cMagicInf);
	}
	// ----
	GCUseMonsterSkillSend(lpObj, lpTargetObj, lpMonsterSkillUnit->m_iUnitNumber);
	// ----
	if( lpMonsterSkillUnit->m_iUnitScopeType == -1 )
	{
		if ( lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD )
		{
			gObjAttack(lpObj, lpTargetObj, NULL, FALSE, 0, 0, FALSE, 0, 0);
		}
        if ( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
        {
			if ( lpObj->Class == 459 )
			{
                if ( lpMonsterSkillUnit->m_iUnitNumber == 40 )
                {
					gObjAttack(lpObj, lpTargetObj, NULL, FALSE, 0, 0, FALSE, 0, 0);
                }
				else
                {
					if ( lpMagic )
					{
						gObjAttack(lpObj, lpTargetObj,lpMagic, 0, 0, 0, 0, 0, 0);
					}
                }
            }
            else
            {
                gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
            }
        }

		lpMonsterSkillUnit->RunSkill(iIndex, iTargetIndex);
	}
	else
	{
		int iCount = 0;
		int iRangeTargetIndex = -1;
		int iAngle = gObjUseSkill.GetAngle(lpObj->X, lpObj->Y, lpTargetObj->X, lpTargetObj->Y);

		while ( true )
		{
			if ( lpObj->VpPlayer2[iCount].state )
			{
				iRangeTargetIndex = lpObj->VpPlayer2[iCount].number;

				if ( OBJMAX_RANGE(iRangeTargetIndex))
				{
					if ( gObj[iRangeTargetIndex].Type == OBJ_USER )
					{
						BOOL bTargetOK = FALSE;

						if ( lpMonsterSkillUnit->m_iUnitScopeType == 0 )
						{
							if ( gObjCalDistance(lpObj, &gObj[iRangeTargetIndex]) < lpMonsterSkillUnit->m_iUnitScopeValue )
							{
								bTargetOK = TRUE;
							}
						}
						else  if ( lpMonsterSkillUnit->m_iUnitScopeType == 1 )
						{
							if ( SkillElectricSparkHitBox.HitCheck(iAngle, lpObj->X, lpObj->Y,
								gObj[iRangeTargetIndex].X, gObj[iRangeTargetIndex].Y) )
							{
								bTargetOK = TRUE;
							}
						}
						else if ( lpMonsterSkillUnit->m_iUnitScopeType == 2 )
						{
							bTargetOK = TRUE;
						}
						else if ( lpMonsterSkillUnit->m_iUnitScopeType == 3 )
						{
							bTargetOK = TRUE;
						}

						if ( bTargetOK )
						{
							if ( lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD )
							{
								gObjAttack(lpObj, lpTargetObj, NULL, FALSE, 0, 0, FALSE, 0, 0);
							}
							if ( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
							{
								if ( lpObj->Class == 459 )
								{
									if ( lpMonsterSkillUnit->m_iUnitNumber == 40 )
									{
										gObjAttack(lpObj, lpTargetObj, NULL, FALSE, 0, 0, FALSE, 0, 0);
									}
									else
									{
										if ( lpMagic )
										{
											gObjAttack(lpObj, lpTargetObj,lpMagic, 0, 0, 0, 0, 0, 0);
										}
									}
								}
								else
								{
									gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
								}
							}

#ifdef IMPERIAL
							if ( lpObj->Class >= 504 && lpObj->Class <= 521)
							{
								if ( lpMagic )
									gObjAttack(lpObj, lpTargetObj, lpMagic, 0, 0, 0, 0, 0, 0);
								else
									gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
							}
#endif

							lpMonsterSkillUnit->RunSkill(iIndex, iRangeTargetIndex);
						}
					}
				}
			}

			iCount++;

			if ( iCount > MAX_VIEWPORT_MONSTER-1)
				break;
		}
	}
}
// -------------------------------------------------------------------------------

//0061d860	-> OK
bool TMonsterSkillManager::FindMagicInf(TMonsterSkillUnit * lpMonsterSkillUnit, CMagicInf * lpOutMagic)
{
	if( !lpMonsterSkillUnit )
	{
		return false;
	}
	// ----
	if( !lpOutMagic )
	{
		return false;
	}
	// ----
	int nUnitNumber = lpMonsterSkillUnit->m_iUnitNumber;
	// ----
	switch(nUnitNumber)
	{
	case 64:
		lpOutMagic->m_Skill = 55;
		break;
		// --
	case 65:
		lpOutMagic->m_Skill = 237;
		break;
		// --
	case 66:
		lpOutMagic->m_Skill = 236;
		break;
		// --
	case 51:
		lpOutMagic->m_Skill = 5;
		break;
		// --
	default:
		return false;
		break;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------