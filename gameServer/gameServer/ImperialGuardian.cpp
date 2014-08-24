// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "ImperialGuardian.h"
#include "gamemain.h"
#include "logproc.h"
#include "MapClass.h"
#include "gObjMonster.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"
#include "BloodCastle.h"
#include "DSProtocol.h"
#include "LogToFile.h"
#include "globalfunc.h"
#include "MonsterStatCalc.h"
#include <atltime.h>
#ifdef __CUSTOMS__
#include "ShopPointEx.h"
#include "ResetSystem.h"
#endif

CImperialGuardian g_ImperialGuardian;

CImperialGuardian::CImperialGuardian(void)
{
	Init();
}


CImperialGuardian::~CImperialGuardian(void)
{
}


void CImperialGuardian::Init()	//OK
{
	this->m_DayOfWeekGM = -1;
	this->MixNeedZen = -1;
	this->m_CheatModeGM = 0;
	this->m_nDayOfWeek = -1;
	this->ukn0Ch = 0;
	this->m_bScriptLoad = false;
	this->m_bEventOn = false;
	InitializeCriticalSection(&m_cs);
}

void CImperialGuardian::InitEventDungeon()	//OK
{
	for (int i = 0; i < MAX_EVENTZONE; ++i )
	{
		InitZone(i);
	}
}

void CImperialGuardian::LoadScript(LPCSTR lpFileName)	//OK
{
	m_bEventOn = GetPrivateProfileIntA("GameServerInfo", "IGEventOn", 0, lpFileName);
	m_LootTime = GetPrivateProfileIntA("GameServerInfo", "LootTime", 20, lpFileName);
	m_WaitPlayerTime = GetPrivateProfileIntA("GameServerInfo", "WaitPlayerTime", 20, lpFileName);
	TimeAttackTime = GetPrivateProfileIntA("GameServerInfo", "TimeAttackTime", 600, lpFileName);
	MixNeedZen = GetPrivateProfileIntA("GameServerInfo", "MixNeedZen", 1000000, lpFileName);
	g_nMysteriousPaperDropRate = GetPrivateProfileIntA("GameServerInfo", "MysteriousPaperDropRate", 0, lpFileName);
	m_bPromotionOn = GetPrivateProfileIntA("GameServerInfo", "IGPromotionOn", 0, lpFileName);

	char* szFileName = gDirPath.GetNewPath("Event\\ImperialGuardianReward.dat");
	m_RewardExpInfo.LoadScript(szFileName);

	m_bScriptLoad = 1;

	if ( m_bEventOn )
		InitEventDungeon();
}

void CImperialGuardian::Run()	//OK
{
	if ( !this->m_bEventOn )
	{
		return;
	}

	for ( int zoneNum = 0; zoneNum < MAX_EVENTZONE; ++zoneNum )
	{
		switch ( m_ZoneInfo[zoneNum].m_State )
		{
			case STATE_READY:
				ProcReady(zoneNum);
				break;

			case STATE_TIMEATTACK:
				ProcBeginTimeAttack(zoneNum);
				break;

			case STATE_LOOTTIME:
				ProcBeginLootTime(zoneNum);
				break;

			case STATE_WAITPLAYER:
				ProcBeginWaitPlayer(zoneNum);
				break;

			case STATE_WARPNEXTZONE:
				ProcAllWarpNextZone(zoneNum);
				break;

			case STATE_ALLKICK:
				ProcAllKick(zoneNum);
				break;

			case STATE_MISSIONFAIL:
				ProcMissionFail(zoneNum);
				break;

			case STATE_MISSIONCLEAR:
				ProcMissionClear(zoneNum);
				break;

			default:
				break;
		}

		ProcCheckDungeon(zoneNum);
	}

	KickInvalidUser();
}

void CImperialGuardian::ProcReady(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}


	if ( GetPlayerCount(nZoneIndex) > 0 )
		SetZoneState(nZoneIndex, 6);
}

void CImperialGuardian::ProcBeginTimeAttack(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

    if ( m_ZoneInfo[nZoneIndex].m_bTimeAttack )
    {
		int iTickSec = GetTickCount() - m_ZoneInfo[nZoneIndex].m_TimeAttack_TickCount;

		if ( !m_ZoneInfo[nZoneIndex].ukn0Eh )
		{
			if ( m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec <= 60000u )
			{
				char szText[256] = {0};
				wsprintf(szText, lMsg.Get(3440));

				GCSendServerMsgAll(nZoneIndex, szText);
				m_ZoneInfo[nZoneIndex].ukn0Eh = true;
			}
		}

		if ( (signed int)iTickSec >= 1000 )
		{
			if ( m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec > iTickSec )
				m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec -= iTickSec;
			else
				m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec = 0;

			m_ZoneInfo[nZoneIndex].m_TimeAttack_TickCount = GetTickCount();
			GCNotifyRemainTickCount(nZoneIndex, 2, m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec);
		}

		if ( IsLastZone(nZoneIndex) )
		{
			if ( GetLiveMonsterCount(nZoneIndex) < 1 )
				m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec = 0;
		}
		else
		{
			if ( m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec >= 60000u )
			{
				int nRemainMin = m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec / 60000u + 1;
				if ( nRemainMin != m_ZoneInfo[nZoneIndex].ukn18h )
				{
					char szText[256] = {0};
					wsprintf(szText, lMsg.Get(3441), nRemainMin);
					GCSendServerMsgAll(nZoneIndex, szText);
					m_ZoneInfo[nZoneIndex].ukn18h = nRemainMin;
				}
			}
		}

		if ( m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec == 0)
		{
			m_ZoneInfo[nZoneIndex].m_bTimeAttack = 0;

			if (GetLiveMonsterCount(nZoneIndex) >= 1 )
			{
				SetZoneState(nZoneIndex, 5);
				DeleteMonster(nZoneIndex, m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex);
			}
			else
			{
				if ( IsLastZone(nZoneIndex) )
					SetZoneState(nZoneIndex, 7);
				else
					SetZoneState(nZoneIndex, 3);

				m_ZoneInfo[nZoneIndex].ukn0Ch = 1;
			}

			LogAddTD("[IMPERIALGUARDIAN] END TIMEATTACK -> [ZONE]:%d ", nZoneIndex + 1);
		}
    }
    else
    {
		m_ZoneInfo[nZoneIndex].m_TimeAttack_TickCount = GetTickCount();
		m_ZoneInfo[nZoneIndex].m_bTimeAttack = true;

		char szText[256] = {0};
		wsprintf(szText, lMsg.Get(3435));

		GCSendServerMsgAll(nZoneIndex, szText);
		SetAtackAbleState(nZoneIndex, 525, 1);
		SetAtackAbleState(nZoneIndex, 528, 1);

		int nMonsterRegenTable = m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex;
		int nMaxLevel = m_ZoneInfo[nZoneIndex].m_nMaxUserLevel;
		int nMaxReset = m_ZoneInfo[nZoneIndex].m_nMaxUserReset;

		RegenMonster(nZoneIndex, nMonsterRegenTable, nMaxLevel, nMaxReset, 0);
		GCNotifyRemainTickCount(nZoneIndex, 2, m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec);

		LogAddTD("[IMPERIALGUARDIAN] BEGIN TIMEATTACK -> [ZONE]:%d ", nZoneIndex + 1);
		LogAddTD("[IMPERIALGUARDIAN] OBJECT COUNT -> [ZONE]:%d [COUNT]:%d", nZoneIndex + 1, m_ZoneInfo[nZoneIndex].vtMonsterInfo.size());
    }

}


void CImperialGuardian::ProcBeginLootTime(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return;

	if ( m_ZoneInfo[nZoneIndex].m_bLootTime )
	{
		int iTickCount = GetTickCount() - m_ZoneInfo[nZoneIndex].m_LootTime_TickCount;

		if ( iTickCount >= 1000 )
		{
			if ( m_ZoneInfo[nZoneIndex].m_LootTime_MSec > iTickCount )
				m_ZoneInfo[nZoneIndex].m_LootTime_MSec -= iTickCount;
			else
				m_ZoneInfo[nZoneIndex].m_LootTime_MSec = 0;

			m_ZoneInfo[nZoneIndex].m_LootTime_TickCount = GetTickCount();
			GCNotifyRemainTickCount(nZoneIndex, 0, m_ZoneInfo[nZoneIndex].m_LootTime_MSec);
		}

		if ( m_ZoneInfo[nZoneIndex].m_LootTime_MSec == 0)
		{
			m_ZoneInfo[nZoneIndex].m_bLootTime = false;
			LogAddTD("[IMPERIALGUARDIAN] END LOOTTIME -> [ZONE]:%d ", nZoneIndex + 1);

			if ( IsLastZone(nZoneIndex) )
				SetZoneState(nZoneIndex, 6);
			else
				SetZoneState(nZoneIndex, 5);

			DeleteMonster(nZoneIndex, m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex);
		}
	}
	else
	{
		m_ZoneInfo[nZoneIndex].m_LootTime_TickCount = GetTickCount();
		m_ZoneInfo[nZoneIndex].m_bLootTime = true;
		GCNotifyRemainTickCount(nZoneIndex, 0, m_ZoneInfo[nZoneIndex].m_LootTime_MSec);

		LogAddTD("[IMPERIALGUARDIAN] BEGIN LOOTTIME -> [ZONE]:%d ", nZoneIndex + 1);

		GCNotifyRemainTickCount(nZoneIndex, 0, m_ZoneInfo[nZoneIndex].m_LootTime_MSec);
		if ( !IsLastZone(nZoneIndex) )
		{
			char szText[256] = {0};
			wsprintf(szText, lMsg.Get(3436));
			GCSendServerMsgAll(nZoneIndex, szText);

			wsprintf(szText, lMsg.Get(3437));
			GCSendServerMsgAll(nZoneIndex, szText);
		}
	}
}


void CImperialGuardian::ProcBeginWaitPlayer(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return;

	if ( m_ZoneInfo[nZoneIndex].m_bWaitPlayer )
	{
		int iTickCount = GetTickCount() - m_ZoneInfo[nZoneIndex].m_WaitPlayer_TickCount;

		if ( iTickCount >= 1000 )
		{
			if ( m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec > iTickCount )
				m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec -= iTickCount;
			else
				m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec = 0;

			m_ZoneInfo[nZoneIndex].m_WaitPlayer_TickCount = GetTickCount();
			GCNotifyRemainTickCount(nZoneIndex, 1, m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec);
		}

		if ( m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec == 0 )
			SetZoneState(nZoneIndex, 1);
	}
	else
	{
		m_ZoneInfo[nZoneIndex].m_WaitPlayer_TickCount = GetTickCount();
		m_ZoneInfo[nZoneIndex].m_bWaitPlayer = 1;
		LogAddTD("[IMPERIALGUARDIAN] BEGIN WAITPLAYER -> [ZONE]:%d ", nZoneIndex + 1);

		GCNotifyRemainTickCount(nZoneIndex, 1, m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec);

		int nRemainMin = m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec / 60000;

		char szText[256]={0};
		wsprintf(szText, lMsg.Get(3434), nRemainMin);
		GCSendServerMsgAll(nZoneIndex, szText);
	}
}

void CImperialGuardian::ProcAllWarpNextZone(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return;

	int nPartyNumber = m_ZoneInfo[nZoneIndex].m_nPartyNumber;
	int nPartyCount = gParty.m_PartyS[nPartyNumber].Count;

	if ( (nZoneIndex + 1) < 4 )
	{
		std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers);

		for ( auto it = vtTemp.begin(); it != vtTemp.end();	++it )
		{
			int nResult = *it;
			if ( gObj[nResult].Connected > 0 )
				gObjMoveGate(nResult, 22);
		}
	}
	else
	{
		std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers);

		for ( auto it = vtTemp.begin(); it != vtTemp.end();	++it )
		{
			int nResult = *it;
			gObjMoveGate(nResult, 22);
		}
	}
}

void CImperialGuardian::ProcAllKick(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return;

	std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers);

	for ( auto it = vtTemp.begin(); it != vtTemp.end(); ++it )
	{
		int nResult = *it;

		if ( gObj[nResult].Connected > 0 )
		{
			gObjMoveGate(nResult, 22);
			LogAddTD("[IMPERIALGUARDIAN] Leave Player Zone -> [ZONE]:%d [AccountID]:%s [Name]:%s ",
				nZoneIndex + 1, gObj[nResult].AccountID, gObj[nResult].Name);
		}
	}

	InitZone(nZoneIndex);
	SetZoneState(nZoneIndex, 0);

	LogAddTD("[IMPERIALGUARDIAN] ALL KICK USER -> [ZONE]:%d ", nZoneIndex + 1);
}

void CImperialGuardian::ProcMissionFail(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return;

	GCMissionFail(nZoneIndex);

	LogAddTD("[IMPERIALGUARDIAN] MISSION FAIL -> [ZONE]:%d ", nZoneIndex + 1);

	SetZoneState(nZoneIndex, 6);
}


void CImperialGuardian::ProcMissionClear(int nZoneIndex)	//OK
{
	GCNotifyAllZoneClear(nZoneIndex);

	LogAddTD("[IMPERIALGUARDIAN] SUCCESS MISSION CLEAR  -> [ZONE]:%d ", nZoneIndex + 1);

	std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers);


	for ( auto it = vtTemp.begin(); it != vtTemp.end(); ++it )
	{
		int nResult = *it;

		if ( gObj[nResult].Connected > 0 )
		{

			if ( gObj[nResult].MapNumber >= 69 && gObj[nResult].MapNumber <= 72 )
			{
				LogAddTD("[IMPERIALGUARDIAN] MISSION CLEAR USER ->[AccountID]:%s [Name]:%s ",
						gObj[nResult].AccountID, gObj[nResult].Name);

				if ( nZoneIndex == 3 )
				{
					if ( m_bPromotionOn )
						;//GEReqRegIGMissionClear(nResult);
				}
			}
		}
	}

	SetZoneState(nZoneIndex, STATE_LOOTTIME);
}

bool CImperialGuardian::IsEmptyZone(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return false;


	if ( this->m_ZoneInfo[nZoneIndex].m_State == STATE_READY )
	{
		if ( GetPlayerCount(nZoneIndex) )
			return false;
		else
			return true;
	}
	else
	{
		return false;
	}
}

void CImperialGuardian::KickInvalidUser()	//OK
{
	for (int iCount = OBJ_STARTUSERINDEX; iCount < OBJMAX; ++iCount )
	{
		if ( (gObj[iCount].Connected == 3 || gObj[iCount].Connected == 4)
			&& gObj[iCount].Type == OBJ_USER
			&& (gObj[iCount].MapNumber >= 69 && gObj[iCount].MapNumber <= 72 ))
		{
			if ( (gObj[iCount].Authority & 2) == 2 )
			{
				continue;
			}

			if ( CheckValidUser(iCount) )
			{
				if ( !this->m_CheatModeGM )
				{
					if ( gObj[iCount].PartyNumber < 0 )
					{
						gObjMoveGate(iCount, 22);
						LogAddTD("[IMPERIALGUARDIAN][ Invalid User ] User is not a party [%s][%s]",
								gObj[iCount].AccountID, gObj[iCount].Name);
					}
				}
			}
			else
			{
				if ( !gObj[iCount].RegenOk )
				{
					if ( gObj[iCount].m_State == 2 && gObj[iCount].Live == 1)
					{
						gObjMoveGate(iCount, 22);
						LogAddTD("[IMPERIALGUARDIAN][ Invalid User ] [%s][%s]", 
								gObj[iCount].AccountID, gObj[iCount].Name);
					}
				}
			}
		}
		else
		{
			if ( CheckValidUser(iCount) )
				RemoveUserInAllZone(iCount);
		}
	}
}

bool CImperialGuardian::CheckValidUser(int nUserNumber)	//OK
{
  return (GetCurrentZoneIndex(nUserNumber) > -1);
}

int CImperialGuardian::GetZoneState(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return STATE_INVALID;

	return this->m_ZoneInfo[nZoneIndex].m_State;
}

bool CImperialGuardian::SetZoneState(int nZoneIndex, int nState)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return false;

    if ( nState <= STATE_CHECKDUNGEON )
    {
		this->m_ZoneInfo[nZoneIndex].m_State = nState;
		return true;
    }

	return false;
}

struct PMSG_IG_CURRENT_STATE
{
  PBMSG_HEAD2 h;
  BYTE ukn05;
  BYTE DayOfWeek;
  BYTE ZoneIndex;
  BYTE EventEnded;
  int RemainTick;
};

void CImperialGuardian::CGEnterPortal(int nUserIndex, int nDestZoneIndex)	//OK
{
	if( !OBJMAX_RANGE(nUserIndex) )
	{
		return;
	}

	if( !IsGoodZone(nDestZoneIndex) )
	{
		return;
	}

	OBJECTSTRUCT *lpObj = &gObj[nUserIndex];

	if ( gObj[nUserIndex].m_IfState.use && lpObj->m_IfState.type != 12 )
	{
		return;
	}

	if ( /*gFreeServer &&*/ lpObj->Level < 150 )
	{
		PMSG_IG_CURRENT_STATE pMsg = {0};
		pMsg.ukn05 = 6;
		PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x02, sizeof(pMsg));

		DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( lpObj->m_bPShopOpen == 1 )
		CGPShopReqClose(lpObj->m_Index);

	int nEnterItemPos = -1;
	PMSG_IG_CURRENT_STATE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x02, sizeof(pMsg));
	pMsg.ukn05 = 0;
	pMsg.ZoneIndex = nDestZoneIndex + 1;

	pMsg.RemainTick = m_ZoneInfo[nDestZoneIndex].m_WaitPlayer_MSec
		+ m_ZoneInfo[nDestZoneIndex].m_LootTime_MSec
		+ m_ZoneInfo[nDestZoneIndex].m_TimeAttack_RemainMSec;

	if ( !m_bEventOn )
	{
		return;
	}


	CTime tCurrentTime(CTime::GetTickCount());
	int nDayOfWeek = tCurrentTime.GetDayOfWeek();

	if ( m_CheatModeGM )
	{
		if ( m_DayOfWeekGM > -1 )
			nDayOfWeek = m_DayOfWeekGM;
	}

	if ( m_CheatModeGM != 1 )
	{
		if ( gParty.m_PartyS[gObj[nUserIndex].PartyNumber].Count < 1 )
		{
			pMsg.ukn05 = 5;
			GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}

		if ( nDestZoneIndex == 0)
		{
			if ( nDayOfWeek == 1 )
			{
				nEnterItemPos = CheckFullSecromicon(nUserIndex);
				if ( nEnterItemPos < 0 && nEnterItemPos == -1 )
				{
					pMsg.ukn05 = 2;
					GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
					return;
				}
			}
			else
			{
				nEnterItemPos = CheckGaionOrderPaper(nUserIndex);
				if ( nEnterItemPos < 0 && nEnterItemPos == -1 )
				{
					pMsg.ukn05 = 2;
					GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
					return;
				}
			}
		}
	}

	if (m_CheatModeGM != 1 && nDestZoneIndex > 0 && GetCurrentZoneIndex(nUserIndex) != nDestZoneIndex - 1 )
	{
		return;
	}

	if ( GetZoneState(nDestZoneIndex) != STATE_READY && GetZoneState(nDestZoneIndex) != STATE_WAITPLAYER )
	{
		GCServerMsgStringSend( lMsg.Get(3464), nUserIndex, 1);

		if ( nDestZoneIndex != 0 )
		{
			RollBackUserPos(nUserIndex);
		}
		else
		{
			pMsg.ukn05 = 1;
			GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
		return;
	}


	if ( nDestZoneIndex == 0 && m_CheatModeGM == 0)
	{
		bool bFlag = 0;
		for ( int i = 1; i < MAX_EVENTZONE; ++i )
		{
			if ( m_ZoneInfo[i].m_nPartyNumber == gObj[nUserIndex].PartyNumber )
			{
				bFlag = true;
				break;
			}
		}

		if(bFlag)
		{
			GCServerMsgStringSend(lMsg.Get(3465), nUserIndex, 1);
			pMsg.ukn05 = 1;
			GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

			if ( nDestZoneIndex != 0)
			{
				RollBackUserPos(nUserIndex);
			}
			return;
		}

	}

	pMsg.DayOfWeek = nDayOfWeek - 1;
	if ( nDayOfWeek - 1 < 1 )
		pMsg.DayOfWeek = 7;

	if ( m_ZoneInfo[nDestZoneIndex].m_nPartyNumber < 0
		&& m_ZoneInfo[nDestZoneIndex].vtPlayers.size() < 1 )
	{
		/*
		if ( nDestZoneIndex > 0 && m_CheatModeGM != 1 && !m_ZoneInfo[nDestZoneIndex - 1].ukn0Ch )
		{
		GCServerMsgStringSend(lMsg.Get(3466), nUserIndex, 1);
		RollBackUserPos(nUserIndex);
		return;
		}
		*/
		int nTest = gObj[nUserIndex].PartyNumber;
		m_ZoneInfo[nDestZoneIndex].m_nPartyNumber = gObj[nUserIndex].PartyNumber;
		if ( nDestZoneIndex != 0)
		{
			if ( m_ZoneInfo[nDestZoneIndex - 1].m_nMonsterRegenTableIndex <= 0 || m_CheatModeGM )
			{
				m_ZoneInfo[nDestZoneIndex].m_nMonsterRegenTableIndex = nDayOfWeek;
			}
			else
			{
				m_ZoneInfo[nDestZoneIndex].m_nMonsterRegenTableIndex = m_ZoneInfo[nDestZoneIndex - 1].m_nMonsterRegenTableIndex;
				m_ZoneInfo[nDestZoneIndex].m_nMaxUserLevel = m_ZoneInfo[nDestZoneIndex - 1].m_nMaxUserLevel;
				m_ZoneInfo[nDestZoneIndex].m_nMaxUserReset = m_ZoneInfo[nDestZoneIndex - 1].m_nMaxUserReset;
			}
		}
		else
		{
			m_ZoneInfo[0].m_nMonsterRegenTableIndex = nDayOfWeek;
		}

		int nMaxLevel = 0;
		int nMaxReset = 0;

		if ( gParty.m_PartyS[m_ZoneInfo[nDestZoneIndex].m_nPartyNumber].m_MaxLevel != 0)
		{
			gGetPartyMaxLevelAndReset(m_ZoneInfo[nDestZoneIndex].m_nPartyNumber, nMaxLevel, nMaxReset);
		}
		else
		{
			nMaxLevel = gObj[nUserIndex].m_nMasterLevel + gObj[nUserIndex].Level;
			nMaxReset = gObj[nUserIndex].Reset;
		}

		m_ZoneInfo[nDestZoneIndex].m_nMaxUserLevel = nMaxLevel;
		m_ZoneInfo[nDestZoneIndex].m_nMaxUserReset = nMaxReset;
		RegAllPartyUser(nDestZoneIndex, nUserIndex);
		RegenMonster(nDestZoneIndex, m_ZoneInfo[nDestZoneIndex].m_nMonsterRegenTableIndex, nMaxLevel, nMaxReset, 1);
		SetZoneState(nDestZoneIndex, 2);
	}
	else
	{
		if ( m_ZoneInfo[nDestZoneIndex].m_nPartyNumber != gObj[nUserIndex].PartyNumber )
		{
			GCServerMsgStringSend(lMsg.Get(3467), nUserIndex, 1);
			pMsg.ukn05 = 1;
			GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}

		if ( !m_CheatModeGM && !nDestZoneIndex && !IsRegPartyUser(0, nUserIndex) )
		{
			GCServerMsgStringSend(lMsg.Get(3467), nUserIndex, 1);
			pMsg.ukn05 = 1;
			GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}

		if ( GetPlayerCount(nDestZoneIndex) > 5 )
		{
			GCServerMsgStringSend(lMsg.Get(3468), nUserIndex, 1u);
			pMsg.ukn05 = 3;
			GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
	}


	pMsg.EventEnded = this->ukn0Ch;

	if ( nDestZoneIndex > 0 )
		RemoveUserInZone(nDestZoneIndex - 1, nUserIndex);

	int nGateIndex = GetGateNumber(nDestZoneIndex, m_ZoneInfo[nDestZoneIndex].m_nMonsterRegenTableIndex);
	if ( nGateIndex != -1 )
	{
		AddUserInZone(nDestZoneIndex, nUserIndex);
		pMsg.ukn05 = 0;

		GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		if ( gObjCheckUsedBuffEffect(lpObj, 3) == 1 )
			gObjRemoveBuffEffect(lpObj, 3);

		gObjMoveGate(nUserIndex, nGateIndex);
		if ( m_CheatModeGM != 1 )
		{
			if ( nDestZoneIndex == 0)
			{
				if ( nEnterItemPos >= 0 )
				{
					if ( lpObj->pInventory[nEnterItemPos].m_Type != ITEMGET(14, 102)
						&& lpObj->pInventory[nEnterItemPos].m_Type != ITEMGET(14, 109) )
					{
						if ( lpObj->pInventory[nEnterItemPos].m_Type == ITEMGET(13, 126)
							|| lpObj->pInventory[nEnterItemPos].m_Type == ITEMGET(13, 127) )
						{
							lpObj->pInventory[nEnterItemPos].m_Durability = lpObj->pInventory[nEnterItemPos].m_Durability - 1.0;
							if ( lpObj->pInventory[nEnterItemPos].m_Durability > 0.0 )
							{
								GCItemDurSend2(nUserIndex, nEnterItemPos, 
									gObj[nUserIndex].pInventory[nEnterItemPos].m_Durability, 0);
							}
							else
							{
								gObjInventoryDeleteItem(nUserIndex, nEnterItemPos);
								GCInventoryItemDeleteSend(nUserIndex, nEnterItemPos, 1u);
							}
						}
					}
					else
					{
						gObjInventoryDeleteItem(nUserIndex, nEnterItemPos);
						GCInventoryItemDeleteSend(nUserIndex, nEnterItemPos, 1u);
					}
				}
			}
		}

		LogAddTD("[IMPERIALGUARDIAN] Enter Zone User -> [AccountID]:%s [NAME]:%s [ZONE]:%d ",
			gObj[nUserIndex].AccountID, gObj[nUserIndex].Name, nDestZoneIndex + 1);

		LogAddTD("[IMPERIALGUARDIAN] [%d ZONE USER COUNT] : %d  ", nDestZoneIndex + 1, GetPlayerCount(nDestZoneIndex));
	}
}

void CImperialGuardian::RegenMonster(int nZoneIndex, int nMonsterRegenTableIndex, int nMaxUserLevel, int nMaxUserReset, char bOnlyCreateGate)	//OK
{
	if( !IsGoodZone(nZoneIndex) )
	{
		LogAddTD("[IMPERIALGUARDIAN][RegenMonsterFunc] Invalid zone index => %d  ", nZoneIndex + 1);
		return;
	}

	if ( nMonsterRegenTableIndex < 1 || nMonsterRegenTableIndex > 7 )
	{
		LogAddTD("[IMPERIALGUARDIAN][RegenMonsterFunc] Invalid MonsterRegenTable Index => %d  ", nMonsterRegenTableIndex);
		return;
	}

//    int nMaxLevel = gParty.m_PartyS[m_ZoneInfo[nZoneIndex].m_nPartyNumber].m_MaxLevel;
//				int nMaxReset = gParty.m_PartyS[m_ZoneInfo[nZoneIndex].m_nPartyNumber].m_MaxReset;

				int nMaxLevel = 0;
				int nMaxReset = 0;
	   gGetPartyMaxLevelAndReset(m_ZoneInfo[nZoneIndex].m_nPartyNumber, nMaxLevel, nMaxReset);

    if ( nMaxUserLevel )
      nMaxLevel = nMaxUserLevel;

				if( nMaxUserReset )
					nMaxReset = nMaxUserReset;

    for (int i = 0; i < gMSetBase.m_Count; ++i )
    {

		if ( gMSetBase.m_Mp[i].m_ArrangeType == 5
			&& gMSetBase.m_Mp[i].m_IG_ZoneIndex == nZoneIndex
			&& gMSetBase.m_Mp[i].m_IG_ZoneIndex == nZoneIndex
			&& gMSetBase.m_Mp[i].m_IG_RegenTable == nMonsterRegenTableIndex )
		{
			if ( bOnlyCreateGate )
			{
				if ( gMSetBase.m_Mp[i].m_Type < 524 || gMSetBase.m_Mp[i].m_Type > 528 )
					continue;
			}
			else
			{
				if ( gMSetBase.m_Mp[i].m_Type >= 524 && gMSetBase.m_Mp[i].m_Type <= 528 )
					continue;
			}

			int nResult = gObjAddMonster(gMSetBase.m_Mp[i].m_MapNumber);

			if ( nResult < 0 )
			{
				LogAddTD("[IMPERIALGUARDIAN][RegenMonsterFunc] gObjAddMonster fail (ErrorCode: %d, MonsterClass: %d)",
						nResult, gMSetBase.m_Mp[i].m_Type);
				continue;
			}

			if ( gObjSetPosMonster(nResult, i) )
			{
				if ( SetMonster(nResult, gMSetBase.m_Mp[i].m_Type, nMaxLevel, nMaxReset) )
				{

					gObj[nResult].m_ImperialZoneID = nZoneIndex;
					stMonsterIndexInfo stMonsterIndex;
					if ( gMSetBase.m_Mp[i].m_Type < 523 || gMSetBase.m_Mp[i].m_Type > 528 )
					{
						stMonsterIndex.bLive = 1;
						stMonsterIndex.bState = 1;
						stMonsterIndex.ObjIndex = nResult;

						m_ZoneInfo[nZoneIndex].ukn0Dh = 1;
						LogAddTD("[IMPERIALGUARDIAN] AddMonster => %s, MapNumber => %d ,Zone => %d PosX => %d, PosY => %d, Connected => %d, STATE => %d ",
							gObj[nResult].Name, gObj[nResult].MapNumber, gObj[nResult].m_ImperialZoneID + 1,
							gObj[nResult].X, gObj[nResult].Y, gObj[nResult].Connected, gObj[nResult].m_State);
					}
					else
					{
						stMonsterIndex.bLive = 0;
						stMonsterIndex.bState = 0;
						stMonsterIndex.ObjIndex = nResult;

						if ( gMSetBase.m_Mp[i].m_Type >= 524 && gMSetBase.m_Mp[i].m_Type <= 528
								&& gMSetBase.m_Mp[i].m_Type != 526)
						{
							LogAddTD("[IMPERIALGUARDIAN] AddGate => %s, MapNumber => %d ,Zone => %d PosX => %d, PosY => %d ",
								gObj[nResult].Name, gObj[nResult].MapNumber, gObj[nResult].m_ImperialZoneID + 1,
								gObj[nResult].X, gObj[nResult].Y);
						}
					}

					this->m_ZoneInfo[nZoneIndex].vtMonsterInfo.push_back(stMonsterIndex);

					if ( gMSetBase.m_Mp[i].m_Type >= 524 && gMSetBase.m_Mp[i].m_Type <= 528
							&& gMSetBase.m_Mp[i].m_Type != 526)
					{
						SetGateBlockState(gMSetBase.m_Mp[i].m_MapNumber, nZoneIndex,
							gMSetBase.m_Mp[i].m_X, gMSetBase.m_Mp[i].m_Y, 1, gMSetBase.m_Mp[i].m_Dir);

					}
				}
				else
				{
					gObjDel(nResult);
					LogAdd("error : %s %d", __FILE__, __LINE__);
				}
			}
			else
			{
				gObjDel(nResult);
				LogAdd("error : %s %d", __FILE__, __LINE__);
			}

		}
	}
}

void CImperialGuardian::DeleteMonster(int nZoneIndex, int nMonsterRegenTableIndex)	//OK
{
	int nCount = 0;
	for ( auto it = m_ZoneInfo[nZoneIndex].vtMonsterInfo.begin();
			it != m_ZoneInfo[nZoneIndex].vtMonsterInfo.end();
			++it)
	{
		stMonsterIndexInfo& stMonsterIndex = *it;
		if ( gObjDel(stMonsterIndex.ObjIndex) )
		{
			gObj[stMonsterIndex.ObjIndex].m_ImperialZoneID = -1;
			++nCount;
		}
	}

	m_ZoneInfo[nZoneIndex].vtMonsterInfo.clear();
	LogAddTD("[IMPERIALGUARDIAN] DELETE ALL MONSTER -> [ZONE]:%d [COUNT]:%d ", nZoneIndex + 1, nCount);
}

void CImperialGuardian::InitZone(int nZoneIndex)	//OK
{
	if ( !m_bScriptLoad )
	{
		MsgBox("[EVENTDUNGEON] script file not loaded");
		return;
	}

	this->m_ZoneInfo[nZoneIndex].m_bLootTime = 0;
	this->m_ZoneInfo[nZoneIndex].m_bWaitPlayer = 0;
	this->m_ZoneInfo[nZoneIndex].m_bTimeAttack = 0;
	this->m_ZoneInfo[nZoneIndex].ukn0Ch = 0;
	this->m_ZoneInfo[nZoneIndex].ukn0Dh = 0;
	this->m_ZoneInfo[nZoneIndex].ukn0Eh = 0;

	if ( nZoneIndex )
		this->m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec = 1000 * this->m_WaitPlayerTime;
	else
		this->m_ZoneInfo[0].m_WaitPlayer_MSec = 1000 * this->m_WaitPlayerTime /*+ 120000*/;

	this->m_ZoneInfo[nZoneIndex].m_LootTime_MSec = 1000 * this->m_LootTime;
	this->m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec = 1000 * this->TimeAttackTime;
	this->m_ZoneInfo[nZoneIndex].m_WaitPlayer_TickCount = 0;
	this->m_ZoneInfo[nZoneIndex].m_LootTime_TickCount = 0;
	this->m_ZoneInfo[nZoneIndex].m_TimeAttack_TickCount = 0;
	this->m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex = -1;
	this->m_ZoneInfo[nZoneIndex].m_nPartyNumber = -1;
	this->m_ZoneInfo[nZoneIndex].m_nMaxUserLevel = 0;
	this->m_ZoneInfo[nZoneIndex].m_nMaxUserReset = -1;
	this->m_ZoneInfo[nZoneIndex].m_State = 0;
	this->m_ZoneInfo[nZoneIndex].ukn18h = m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec / 60000u + 1;
	this->m_ZoneInfo[nZoneIndex].vtUkn3Ch.clear();
	this->m_ZoneInfo[nZoneIndex].vtPlayers.clear();

	m_ZoneInfo[nZoneIndex].vtMonsterInfo.clear();
	m_ZoneInfo[nZoneIndex].vtGateInfo.clear();
}

bool CImperialGuardian::SetMonster(int nIndex, int MonsterClass, int nMaxLevel, int nMaxUserReset)	//OK
{
	if( !OBJMAX_RANGE(nIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

    OBJECTSTRUCT *lpObj = &gObj[nIndex];

    lpObj->ConnectCheckTime = GetTickCount();
    lpObj->ShopNumber = -1;
    lpObj->TargetNumber = -1;
    lpObj->m_RecallMon = -1;
    lpObj->Connected = 3;
    lpObj->Live = 1;
    lpObj->m_State = 1;
    lpObj->DieRegen = 0;
    lpObj->Type = OBJ_MONSTER;
    lpObj->Class = MonsterClass;
    lpObj->LastAttackerID = -1;

    g_MonsterStatCalc.ConvertMonsterStat(lpObj, nMaxLevel, nMaxUserReset);

    *(BYTE *)lpObj->pInventoryCount = 0;

    if ( lpObj->m_AttackType )
    {
							gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType, 1);
    }

    gObjMonsterHitDamageInit(lpObj);
    gObjSetInventory1Pointer(lpObj);
    CreateFrustrum(lpObj->X, lpObj->Y, nIndex);

	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);

    lpObj->m_OldX = lpObj->X;
    lpObj->m_OldY = lpObj->Y;

	return true;
}

int CImperialGuardian::GetGateNumber(int nZoneIndex, int nDayOfWeek)	//OK
{
	int nGateNumber = -1;

	switch ( nZoneIndex )
	{
		case 0:
			if ( nDayOfWeek == 1 )
			{
				nGateNumber = 322;
			}
			else if ( nDayOfWeek == 2 || nDayOfWeek == 5 )
			{
				nGateNumber = 307;
			}
			else if ( nDayOfWeek == 3 || nDayOfWeek == 6 )
			{
				nGateNumber = 312;
			}
			else if ( nDayOfWeek == 4 || nDayOfWeek == 7 )
			{
				nGateNumber = 317;
			}
		break; // case 0

		case 1:
			if ( nDayOfWeek == 1 )
			{
				nGateNumber = 324;
			}
			else if ( nDayOfWeek == 2 || nDayOfWeek == 5 )
			{
				nGateNumber = 309;
			}
			else if ( nDayOfWeek == 3 || nDayOfWeek == 6 )
			{
				nGateNumber = 314;
			}
			else if ( nDayOfWeek == 4 || nDayOfWeek == 7 )
			{
				nGateNumber = 319;
			}
		break; // case 1

		case 2:
			if ( nDayOfWeek == 1 )
			{
				nGateNumber = 326;
			}
			else if ( nDayOfWeek == 2 || nDayOfWeek == 5 )
			{
				nGateNumber = 311;
			}
			else if ( nDayOfWeek == 3 || nDayOfWeek == 6 )
			{
				nGateNumber = 316;
			}
			else if ( nDayOfWeek == 4 || nDayOfWeek == 7 )
			{
				nGateNumber = 321;
			}
		break; // case 2

		case 3:
			if ( nDayOfWeek == 1 )
				nGateNumber = 328;
		break;

		default:
			LogAddTD("[IMPERIALGUARDIAN][GetGateNumberFunc] Invalid zoneIndex : %d ", nZoneIndex + 1);
		break;
	}


  return nGateNumber;
}

int CImperialGuardian::GetMapNumber(int nDayOfWeek)	//OK
{
	int nMapNumber = -1;

  	if ( nDayOfWeek == 1 )
	{
		nMapNumber = 72;
	}
	else if ( nDayOfWeek == 2 || nDayOfWeek == 5 )
	{
		nMapNumber = 69;
	}
	else if ( nDayOfWeek == 3 || nDayOfWeek == 6 )
	{
		nMapNumber = 70;

	}
	else if ( nDayOfWeek == 4 || nDayOfWeek == 7 )
	{
		nMapNumber = 71;
	}

	return nMapNumber;
}

void CImperialGuardian::ProcCheckDungeon(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return;


	CTime tCurrentTime(CTime::GetTickCount());
    int nDayOfWeek = tCurrentTime.GetDayOfWeek();

    if ( m_nDayOfWeek != nDayOfWeek )
    {
		m_nDayOfWeek = nDayOfWeek;
		this->ukn0Ch = rand() % 4;
    }

    CheckLiveMonster(nZoneIndex);

    if ( GetZoneState(nZoneIndex) == STATE_READY )
		return;


    if ( GetZoneState(nZoneIndex) == STATE_TIMEATTACK )
    {
		if ( m_ZoneInfo[nZoneIndex].ukn0Dh == 1 )
		{
			if ( GetLiveMonsterCount(nZoneIndex) < 1 )
			{
				SetAtackAbleState(nZoneIndex, 524, 1);
				SetAtackAbleState(nZoneIndex, 527, 1);
				SetAtackAbleState(nZoneIndex, 528, 1);
			}
		}
    }

    if ( GetPlayerCount(nZoneIndex) < 1 )
    {
		int nMonsterRegenTable = m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex;
		DeleteMonster(nZoneIndex, nMonsterRegenTable);
		InitZone(nZoneIndex);
    }
}

int CImperialGuardian::GetLiveMonsterCount(int nZoneIndex)	//OK
{
	int nCount = 0;

	for ( auto it = m_ZoneInfo[nZoneIndex].vtMonsterInfo.begin(); 
			it != m_ZoneInfo[nZoneIndex].vtMonsterInfo.end();
			++it)
	{
		stMonsterIndexInfo& stMonsterIndex = *it;

		if ( stMonsterIndex.bLive )
			++nCount;
	}

	return nCount;
}

void CImperialGuardian::CheckLiveMonster(int nZoneIndex)	//OK
{
	for ( auto it = this->m_ZoneInfo[nZoneIndex].vtMonsterInfo.begin();
			it != this->m_ZoneInfo[nZoneIndex].vtMonsterInfo.end();
			++it )
	{
		stMonsterIndexInfo& stMonsterIndex = *it;

		int ObjIndex = stMonsterIndex.ObjIndex;

		if ( !gObj[ObjIndex].Live && stMonsterIndex.bLive == 1 || gObj[ObjIndex].Connected != 3 )
			stMonsterIndex.bLive = false;

		if ( gObj[ObjIndex].Live )
		{
			if ( gObj[ObjIndex].m_State != 1 && gObj[ObjIndex].m_State != 2)
			{
				LogAddTD("[IMPERIALGUARDIAN][RESTORE] [NAME]:%s [INDEX]:%d [TYPE]:%d [STATE]:%d",
						gObj[ObjIndex].Name, ObjIndex, gObj[ObjIndex].Class, gObj[ObjIndex].m_State);

				gObj[ObjIndex].m_State = 1;
			}
		}
	}

}

int CImperialGuardian::GetCurrentZoneIndex(int nUserNumber)	//OK
{
	int nResult = -1;
	for ( int i = 0; i < MAX_EVENTZONE; ++i )
	{
		EnterCriticalSection(&m_cs);
		std::vector<int> vtTemp(m_ZoneInfo[i].vtPlayers);

		for ( auto it = vtTemp.begin(); it != vtTemp.end(); ++it )
		{
			if ( *it == nUserNumber )
			{
				LeaveCriticalSection(&m_cs);
				nResult = i;
				return nResult;
			}
		}

		LeaveCriticalSection(&m_cs);
	}

	return -1;
}


bool CImperialGuardian::AddUserInZone(int nZoneIndex, int nUserNumber)	//OK
{
	EnterCriticalSection(&this->m_cs);

	bool bAddUser = true;

	for ( auto it = m_ZoneInfo[nZoneIndex].vtPlayers.begin();
			it != m_ZoneInfo[nZoneIndex].vtPlayers.end();
			++it )
	{
		if ( *it == nUserNumber )
		{
			bAddUser = false;
			break;
		}
	}


	if ( bAddUser )
	{
		m_ZoneInfo[nZoneIndex].vtPlayers.push_back(nUserNumber);
		LeaveCriticalSection(&this->m_cs);
		return true;
	}
	else
	{
		LeaveCriticalSection(&this->m_cs);
		return false;
	}

}

void CImperialGuardian::RemoveUserInZone(int nZoneIndex, int nUserNumber)	//OK
{
	EnterCriticalSection(&this->m_cs);

	for ( auto it = m_ZoneInfo[nZoneIndex].vtPlayers.begin();
			it != m_ZoneInfo[nZoneIndex].vtPlayers.end();
			++it)
	{
		if ( *it == nUserNumber )
		{
			m_ZoneInfo[nZoneIndex].vtPlayers.erase(it);
			LogAddTD("[IMPERIALGUARDIAN] Leave Player Zone [ZONE]:%d, [AccountID]:%s, [Name]:%s",
					nZoneIndex + 1, gObj[nUserNumber].AccountID, gObj[nUserNumber].Name);
			break;
		}
	}

	LeaveCriticalSection(&this->m_cs);
}

void CImperialGuardian::RemoveUserInAllZone(int nUserNumber)	//OK
{
	for ( int i = 0; i < MAX_EVENTZONE; ++i )
		RemoveUserInZone(i, nUserNumber);
}

int CImperialGuardian::GetPlayerCount(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return 0;

    return m_ZoneInfo[nZoneIndex].vtPlayers.size();
}

int CImperialGuardian::GetTotalPlayerCount()	//OK
{
	int nCount = 0;

	for (int i = 0; i < MAX_EVENTZONE; ++i )
		nCount += GetPlayerCount(i);

	return nCount;
}

bool CImperialGuardian::IsLastZone(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return false;

	if ( this->m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex == 1 && nZoneIndex == 3 )
		return true;

	if(this->m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex != 1 && nZoneIndex == 2)
		return true;
	else
		return false;
}

struct PMSG_IG_ZONE_CLEAR
{
	PBMSG_HEAD2 h;
	BYTE ClearType;
	int RewardExp;
};


bool CImperialGuardian::GCNotifyZoneClear(int nZoneIndex)	//OK
{
	if(!IsGoodZone(nZoneIndex))
		return false;

	PMSG_IG_ZONE_CLEAR pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x06, sizeof(pMsg));

	pMsg.ClearType = 1;

	std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers); 

	for ( auto it = vtTemp.begin();	it != vtTemp.end();	++it )
	{
		int nResult = *it;

		if(IsEventMap(gObj[nResult].MapNumber))
			GCSendDataToUser(nResult, (LPBYTE)&pMsg, sizeof(pMsg));
	}

	return true;
}

bool CImperialGuardian::GCNotifyAllZoneClear(int nZoneIndex)	//OK
{
	if( !IsGoodZone(nZoneIndex) )
		return false;

	PMSG_IG_ZONE_CLEAR pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x06, sizeof(pMsg));

	pMsg.ClearType = 2;

	std::vector<int> vtTemp(this->m_ZoneInfo[nZoneIndex].vtPlayers);

	for ( auto it = vtTemp.begin(); it != vtTemp.end(); ++it )
	{
		int nResult = *it;

		if ( IsEventMap(gObj[nResult].MapNumber) )
		{
#if defined __REEDLAN__ || __BEREZNUK__
			g_ShopPointEx.AddEventBonus(nResult, ShopPointExEvent::IG);
#endif
			pMsg.RewardExp = m_RewardExpInfo.GetRewardExp(nResult);

			if ( nZoneIndex == 3 )
				pMsg.RewardExp *= 2;

			if ( gObjCheckUsedBuffEffect(&gObj[nResult], 42)
				|| gObjCheckUsedBuffEffect(&gObj[nResult], 31) )
			{
				pMsg.RewardExp = 0;
			}

			ImperialGuardianLevelUp(nResult, pMsg.RewardExp);

			LogAddTD("[IMPERIALGUARDIAN] [ACCOUNTID]:%s, [NAME]:%s, [Reward Exp] : %d ",
					gObj[nResult].AccountID, gObj[nResult].Name, pMsg.RewardExp);

			//if ( !g_MasterLevelSystem.IsMasterLevelUser(&gObj[nResult]) )
			//	GCSendExp_INT64(nResult, -1, pMsg.RewardExp, 0, 0);

			if( !g_MasterLevelSystem.IsMasterLevelUser(&gObj[nResult]) )
							GCSendExp_INT64(nResult, -1, pMsg.RewardExp, 0, 0);

			GCSendDataToUser(nResult, (LPBYTE)&pMsg, sizeof(pMsg));

			char szText[256] = {0};

			wsprintf(szText, lMsg.Get(3438));
			GCServerMsgStringSend(szText, nResult, 0);
		}
	}


	return true;
}

void CImperialGuardian::GCMissionFail(int nZoneIndex)	//OK
{
	if( !IsGoodZone(nZoneIndex) )
		return;

	PMSG_IG_ZONE_CLEAR pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x06, sizeof(pMsg));
	pMsg.ClearType = 0;

	std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers);

	for ( auto it = vtTemp.begin(); it != vtTemp.end(); ++it )
	{
		int nResult = *it;

		if ( IsEventMap(gObj[nResult].MapNumber) )
			GCSendDataToUser(nResult, (LPBYTE)&pMsg, sizeof(pMsg));

		char szText[256] = {0};

		wsprintf(szText, lMsg.Get(3439));
		GCServerMsgStringSend(szText, nResult, 0);
	}
}

void CImperialGuardian::GCMissionFailUseDie(int nUserIndex)	//OK
{
	if( !OBJMAX_RANGE(nUserIndex) )
		return;

	PMSG_IG_ZONE_CLEAR pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x06, sizeof(pMsg));
	pMsg.ClearType = 0;

	GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	char szText[256] = {0};

	wsprintf(szText, lMsg.Get(3439));
	GCServerMsgStringSend(szText, nUserIndex, 0);
}

void CImperialGuardian::GCSendDataToUser(int nIndex, LPBYTE lpMsg, DWORD nSize)	//OK
{
	if( !OBJMAX_RANGE(nIndex) )
		return;

	if ( gObj[nIndex].Connected == 3 )
	{
		if ( gObj[nIndex].Type == OBJ_USER )
			DataSend(nIndex, lpMsg, nSize);
	}
}


struct PMSG_IG_REMAIN_TICK
{
	PBMSG_HEAD2 h;
	BYTE TickType;
	BYTE MonsterTableIndex;
	BYTE ZoneIndex;
	int RemainTick;
	BYTE LiveMonsterCount;
};

bool CImperialGuardian::GCNotifyRemainTickCount(int nZoneIndex, BYTE btMsgType, DWORD dwRemainTick)	//OK
{
	PMSG_IG_REMAIN_TICK pMsg = {0};
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x04, sizeof(pMsg));

	pMsg.TickType = btMsgType;
	pMsg.ZoneIndex = nZoneIndex + 1;
	pMsg.LiveMonsterCount = GetLiveMonsterCount(nZoneIndex);
	pMsg.MonsterTableIndex = m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex - 1;

	if ( pMsg.MonsterTableIndex < 1 )
		pMsg.MonsterTableIndex = 7;

	switch(btMsgType)
	{
		case 0:	pMsg.RemainTick = m_ZoneInfo[nZoneIndex].m_LootTime_MSec;			break;
		case 1: pMsg.RemainTick = m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec;			break;
		case 2: pMsg.RemainTick = m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec;	break;

		default:
			LogAddTD("[IMPERIALGUARDIAN][GCNotifyRemainTickCountFunc] Invalid MsgType : %d ", btMsgType);
		return false;

	}

	std::vector<int> vtTtemp(m_ZoneInfo[nZoneIndex].vtPlayers);

	for (auto it = vtTtemp.begin(); it != vtTtemp.end(); ++it )
	{
		int nResult = *it;

		if ( IsEventMap(gObj[nResult].MapNumber) )
			GCSendDataToUser(nResult, (LPBYTE)&pMsg, sizeof(pMsg));
	}

	return true;
}

void CImperialGuardian::SetGateBlockState(int nMapNumber, int nZoneIndex, signed int nX, signed int nY, int nGateState, int nDir)	//OK
{
	if ( nX < 0 || nX > 255 )
		return;

	if ( nY < 0 || nY > 255 )
		return;

	if( nGateState == 0 )
	{
		switch ( nDir )
		{
			case 3:
				for ( int i = nX - 3; i <= nX; ++i )
				{
					for ( int j = nY - 2; j <= nY + 2; ++j )
					{
						if ( i != nX || j != nY )
							MapC[nMapNumber].m_attrbuf[i + (j *256)] &= (~16);
					}
				}
			break;

			case 1:
				for ( int i = nX - 2; i <= nX + 2; ++i )
				{
					for ( int j = nY; j <= nY + 3; ++j )
					{
						if ( i != nX || j != nY )
							MapC[nMapNumber].m_attrbuf[i + (j *256)] &= (~16);
					}
				}
			break;

			case 5:
				for ( int i = nX - 2; i <= nX + 2; ++i )
				{
					for ( int j = nY - 3; j <= nY; ++j )
					{
						if ( i != nX || j != nY )
							MapC[nMapNumber].m_attrbuf[i + (j *256)] &= (~16);
					}
				}
			break;

			case 7:
				for ( int i = nX; i <= nX + 3; ++i )
				{
					for ( int j = nY - 2; j <= nY + 2; ++j )
					{
						if ( i != nX || j != nY ) { }
							MapC[nMapNumber].m_attrbuf[i + (j *256)] &= (~16);
					}
				}
			break;
		}
	}
    else if ( nGateState == 1 )
    {
        switch ( nDir )
        {
			case 3:
				for ( int i = nX - 3; i <= nX; ++i )
				{
					for ( int j = nY - 2; j <= nY + 2; ++j )
					{
						if ( i != nX || j != nY )
							MapC[nMapNumber].m_attrbuf[i + (j *256)] |= 16;
					}
				}
				break;

			case 1:
				for ( int i = nX - 2; i <= nX + 2; ++i )
				{
					for ( int j = nY; j <= nY + 3; ++j )
					{
						if ( i != nX || j != nY )
							MapC[nMapNumber].m_attrbuf[i + (j *256)] |= 16;
					}
				}
				break;

			case 5:
				for ( int i = nX - 2; i <= nX + 2; ++i )
				{
					for ( int j = nY; j <= nY - 3; ++j )
					{
						if ( i != nX || j != nY )
							MapC[nMapNumber].m_attrbuf[i + (j *256)] |= 16;
					}
				}
				break;

			case 7:
				for ( int i = nX; i <= nX + 3; ++i )
				{
					for ( int j = nY - 2; j <= nY + 2; ++j )
					{
						if ( i != nX || j != nY )
							MapC[nMapNumber].m_attrbuf[i + (j *256)] |= 16;
					}
				}
				break;
		}

    }
}


void CImperialGuardian::GCSendCastleGateInfo(int nGateIndex, int nZoneIndex, int nUserIndex)	//OK
{
	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 16;


	int iGateState = 0;

	if ( gObj[nGateIndex].Live )
		iGateState = 1;
	else
		iGateState = 0;

	int nDir = gObj[nGateIndex].Dir;
	int nY = gObj[nGateIndex].X;
	int nX = gObj[nGateIndex].Y;

	if( iGateState == 0 )
	{
		switch ( nDir )
		{
			case 3:
				lpMsg->btMapSetType = 1;
				lpMsgBody[0].btX = nY - 3;
				lpMsgBody[0].btY = nX - 2;
				lpMsgBody[1].btX = nY;
				lpMsgBody[1].btY = nX + 2;
			break;

			case 1:
				lpMsg->btMapSetType = 1;
				lpMsgBody[0].btX = nY - 2;
				lpMsgBody[0].btY = nX;
				lpMsgBody[1].btX = nY + 2;
				lpMsgBody[1].btY = nX + 3;
			break;

			case 5:
				lpMsg->btMapSetType = 1;
				lpMsgBody[0].btX = nY - 2;
				lpMsgBody[0].btY = nX - 3;
				lpMsgBody[1].btX = nY + 2;
				lpMsgBody[1].btY = nX;
			break;

			case 7:
				lpMsg->btMapSetType = 1;
				lpMsgBody[0].btX = nY;
				lpMsgBody[0].btY = nX - 2;
				lpMsgBody[1].btX = nY + 3;
				lpMsgBody[1].btY = nX + 2;
			break;
		}	
	}
	else if( iGateState == 1 )
	{
		switch ( nDir )
		{
			case 3:
				lpMsg->btMapSetType = 0;
				lpMsgBody[0].btX = nY - 3;
				lpMsgBody[0].btY = nX - 2;
				lpMsgBody[1].btX = nY;
				lpMsgBody[1].btY = nX + 2;
			break;

			case 1:
				lpMsg->btMapSetType = 0;
				lpMsgBody[0].btX = nY - 2;
				lpMsgBody[0].btY = nX;
				lpMsgBody[1].btX = nY + 2;
				lpMsgBody[1].btY = nX + 3;
			break;

			case 5:
				lpMsg->btMapSetType = 0;
				lpMsgBody[0].btX = nY - 2;
				lpMsgBody[0].btY = nX - 3;
				lpMsgBody[1].btX = nY + 2;
				lpMsgBody[1].btY = nX;
			break;

			case 7:
				lpMsg->btMapSetType = 0;
				lpMsgBody[0].btX = nY;
				lpMsgBody[0].btY = nX - 2;
				lpMsgBody[1].btX = nY + 3;
				lpMsgBody[1].btY = nX + 2;
			break;
		}
	}

	if ( gObj[nUserIndex].Connected == 3 )
	{
		if ( DataSend(nUserIndex, (LPBYTE)lpMsg, lpMsg->h.size) )
		{
			LogAddTD("[IMPERIALGUARDIAN] SEND GATE STATE -> [ZONE]:%d [AccountID]:%s, [NAME]:%s [STATE]:%d",
					nZoneIndex + 1, gObj[nUserIndex].AccountID, gObj[nUserIndex].Name, iGateState);

			LogAddTD("[IMPERIALGUARDIAN] beginX : %d, beginY : %d , endX :%d , endY : %d",
					lpMsgBody[0].btX, lpMsgBody[0].btY, lpMsgBody[1].btX, lpMsgBody[1].btY);
		}
	}

}


void CImperialGuardian::DropItem(int nIndex, int nMonsterIndex)	//OK
{
	int nZoneIndex = GetCurrentZoneIndex(nIndex);

	if( !IsGoodZone(nZoneIndex) )
		return;

    int nMaxLevel = m_ZoneInfo[nZoneIndex].m_nMaxUserLevel;
				int nMaxReset = m_ZoneInfo[nZoneIndex].m_nMaxUserReset;
    int nType = 0;

				if( nMaxReset == -1 )
				{
					nMaxReset = gObj[nIndex].Reset;
				}

    if ( nMaxLevel == 0)
      nMaxLevel = gObj[nIndex].Level + gObj[nIndex].m_nMasterLevel;
#ifdef IMPERIAL
	if ( gObj[nMonsterIndex].Class == 504 )
	{
		LogAddTD("[IMPERIALGUARDIAN] Kill the boss monster => %s [ZONE]:%d [AccountID]:%s [Name]:%s",
			gObj[nMonsterIndex].Name, gObj[nMonsterIndex].m_ImperialZoneID + 1, 
			gObj[nIndex].AccountID, gObj[nIndex].Name);
		pEventDungeonItemBagGaion->DropEventItem(nIndex, nMaxLevel, nMaxReset);
	}
	else if ( gObj[nMonsterIndex].Class == 526 )
	{
		LogAddTD("[IMPERIALGUARDIAN] Broken Statue => %s [ZONE]:%d [AccountID]:%s [Name]:%s",
			gObj[nMonsterIndex].Name, gObj[nMonsterIndex].m_ImperialZoneID + 1,
			gObj[nIndex].AccountID, gObj[nIndex].Name);

		pEventDungeonItemBagStone->DropEventItem(nIndex, nMaxLevel, nMaxReset);
	}
	else
	{
		LogAddTD("[IMPERIALGUARDIAN] Kill the boss monster => %s [ZONE]:%d [AccountID]:%s [Name]:%s",
			gObj[nMonsterIndex].Name, gObj[nMonsterIndex].m_ImperialZoneID + 1,
			gObj[nIndex].AccountID, gObj[nIndex].Name);

		pEventDungeonItemBag->DropEventItem(nIndex, nMaxLevel, nMaxReset);
	}
#endif

	if ( m_ZoneInfo[nZoneIndex].m_nMonsterRegenTableIndex != 1 )
	{
		switch ( gObj[nMonsterIndex].Class )
		{
			case 508:
				nType = ItemGetNumberMake(14, 103);
				break;

			case 509:
				nType = ItemGetNumberMake(14, 104);
				break;

			case 510:
				nType = ItemGetNumberMake(14, 105);
				break;

			case 511:
				nType = ItemGetNumberMake(14, 106);
				break;

			case 507:
				nType = ItemGetNumberMake(14, 107);
				break;

			case 506:
				nType = ItemGetNumberMake(14, 108);
				break;

			default:
				return;
		}

		int nCount = 0;
		int nRand = rand() % 100;

		if( nRand >=0 && nRand < 50)
		{
			nCount = 1;
		}
		else if( nRand >= 50 && nRand < 79 )
		{
			nCount = 2;
		}
		else
		{
			nCount = 3;
		}


		for (int i = 0; i < nCount; ++i )
		{
			BYTE btDropX = gObj[nIndex].X;
			BYTE btDropY = gObj[nIndex].Y;

			if ( !gObjGetRandomItemDropLocation(gObj[nIndex].MapNumber, btDropX, btDropY, 2, 2, 10) )
			{
				btDropX = gObj[nIndex].X;
				btDropY = gObj[nIndex].Y;
			}

			ItemSerialCreateSend(nIndex, gObj[nIndex].MapNumber, btDropX, btDropY,
					nType, 0, 0, 0, 0, 0, nIndex, 0, 0);

			LogAddTD("[IMPERIALGUARDIAN] Drop Item : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
				gObj[nIndex].MapNumber, btDropX, btDropY, &ItemAttribute[nType], nType, 0, 0, 0, 0);
		}
	}
}

int CImperialGuardian::CheckOverlapMysteriousPaper(int nIndex, int nItem_nMasterLevel)	//OK
{
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if ( gObj[nIndex].pInventory[i].IsItem() )
		{
			if ( gObj[nIndex].pInventory[i].m_Type == ITEMGET(14, 101) )
			{
				if ( gObj[nIndex].pInventory[i].m_Level == nItem_nMasterLevel )
				{
					int nItemDur = gObj[nIndex].pInventory[i].m_Durability;

					if ( nItemDur >= 0 && nItemDur <= 4 )
						return i;
				}
			}
		}
	}

	return -1;
}


void CImperialGuardian::RollBackUserPos(int nUserNumber)	//OK
{
	if ( gObj[nUserNumber].Type == OBJ_USER )
	{
		if ( gObj[nUserNumber].Connected > 2 )
		{
			int x = gObj[nUserNumber].X;
			int y = gObj[nUserNumber].Y;
			int mapNumber = gObj[nUserNumber].MapNumber;

			gObj[nUserNumber].RegenMapNumber = gObj[nUserNumber].MapNumber;
			gObj[nUserNumber].RegenMapX = x;
			gObj[nUserNumber].RegenMapY = y;

			gObjClearViewport(&gObj[nUserNumber]);

			GCTeleportSend(&gObj[nUserNumber], -1, mapNumber, 
				gObj[nUserNumber].X, gObj[nUserNumber].Y, gObj[nUserNumber].Dir);

			if ( gObj[nUserNumber].m_Change >= 0 )
				gObjViewportListProtocolCreate(&gObj[nUserNumber]);

			gObj[nUserNumber].RegenOk = 1;
		}
	}
}

int CImperialGuardian::CheckGaionOrderPaper(int nIndex)	//OK
{
 	if( !OBJMAX_RANGE(nIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return -2;
	}

    OBJECTSTRUCT *lpObj = &gObj[nIndex];
	if ( gObj[nIndex].m_IfState.use )
    {
      return  -3;
    }

	if ( gObj[nIndex].Type == OBJ_USER && gObj[nIndex].Connected > 2 )
	{
		for ( int nPos = 0; nPos < INVENTORY_SIZE; ++nPos )
		{
			if ( gObj[nIndex].pInventory[nPos].IsItem() && gObj[nIndex].pInventory[nPos].m_Type == ITEMGET(14, 102) )
				return nPos;
		}

		for ( int nPos = 0; nPos < INVENTORY_SIZE; ++nPos )
		{
			if ( lpObj->pInventory[nPos].m_Type == ITEMGET(13, 126) && lpObj->pInventory[nPos].m_Durability > 0.0 )
				return nPos;
		}
		
		return -1;
	}

	return -2;
}


int CImperialGuardian::CheckFullSecromicon(int nIndex)	//OK
{
 	if( !OBJMAX_RANGE(nIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return -2;
	}

    OBJECTSTRUCT *lpObj = &gObj[nIndex];
	if ( gObj[nIndex].m_IfState.use )
    {
      return  -3;
    }

	if ( gObj[nIndex].Type == OBJ_USER && gObj[nIndex].Connected > 2 )
	{
		for ( int nPos = 0; nPos < INVENTORY_SIZE; ++nPos )
		{
			if ( gObj[nIndex].pInventory[nPos].IsItem() && gObj[nIndex].pInventory[nPos].m_Type == ITEMGET(14, 109) )
				return nPos;
		}

		for ( int nPos = 0; nPos < INVENTORY_SIZE; ++nPos )
		{
			if ( lpObj->pInventory[nPos].m_Type == ITEMGET(13, 127) && lpObj->pInventory[nPos].m_Durability > 0.0 )
				return nPos;
		}
		
		return -1;
	}

	return -2;
}

bool CImperialGuardian::IsAttackAbleMonster(int nMonsterIndex)	//OK
{
	for (int i = 0; i < MAX_EVENTZONE; ++i )
	{
		int nSize = m_ZoneInfo[i].vtMonsterInfo.size();
		for (int k = 0; k < nSize; ++k )
		{
			stMonsterIndexInfo& monsterInfo = m_ZoneInfo[i].vtMonsterInfo.at(k);

			if ( monsterInfo.ObjIndex == nMonsterIndex )
				return monsterInfo.bState;
		}
	}

	return false;
}

void CImperialGuardian::SetAtackAbleState(int nZoneIndex, int nMonsterClass, bool bState)	//OK
{
 	if( !IsGoodZone(nZoneIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	if(m_ZoneInfo[nZoneIndex].vtMonsterInfo.size() < 1)
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

    for ( auto it = m_ZoneInfo[nZoneIndex].vtMonsterInfo.begin();
				it != m_ZoneInfo[nZoneIndex].vtMonsterInfo.end();
				++it)
    {
		stMonsterIndexInfo& stMonsterIndex = *it;

		OBJECTSTRUCT* lpObj = &gObj[stMonsterIndex.ObjIndex];

		if ( lpObj->Class == nMonsterClass )
			stMonsterIndex.bState = bState;
    }
}

void CImperialGuardian::SetDayOfWeekGM(int nDayOfWeeks)
{
	m_DayOfWeekGM = nDayOfWeeks;
}

void CImperialGuardian::SetCheatModeGM(int nCheatMode)
{
	m_CheatModeGM = nCheatMode;
}

void CImperialGuardian::WarpZoneGM(int nUserIndex, int nZoneIndex)
{
	if ( m_CheatModeGM )
		CGEnterPortal(nUserIndex, nZoneIndex);
}

stZoneInfo CImperialGuardian::GetZoneInfo(int nZoneIndex)
{
	return this->m_ZoneInfo[nZoneIndex];
}

void CImperialGuardian::SetTargetMoveAllMonster(int nZoneIndex, int nTargetNumber)	//OK
{
	if( !OBJMAX_RANGE(nTargetNumber) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	if ( gObj[nTargetNumber].Connected < 1 )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	for ( auto it = m_ZoneInfo[nZoneIndex].vtMonsterInfo.begin(); it != m_ZoneInfo[nZoneIndex].vtMonsterInfo.end(); ++it)
	{
		stMonsterIndexInfo& stMonsterIndex = *it;

		if ( stMonsterIndex.bLive )
		{
			OBJECTSTRUCT* lpObj = &gObj[stMonsterIndex.ObjIndex];
			if ( gObj[stMonsterIndex.ObjIndex].TargetNumber == -1 && !(lpObj->m_ActState.Emotion) )
			{
				lpObj->TargetNumber = nTargetNumber;
				lpObj->m_ActState.Emotion = 1;
				lpObj->m_ActState.EmotionCount = 50;
			}
			else if ( lpObj->TargetNumber != nTargetNumber )
			{	
				if ( rand() % 100 < 30 )
				{
					lpObj->TargetNumber = nTargetNumber;
					lpObj->m_ActState.Emotion = 1;
					lpObj->m_ActState.EmotionCount = 50;
				}
			}
		}
	}
}

void CImperialGuardian::DestroyGate(int nZoneIndex, int nIndex, int nTargetIndex)	//OK
{
	SetGateBlockState(gObj[nIndex].MapNumber, nZoneIndex, 
			gObj[nIndex].X, gObj[nIndex].Y, 0, gObj[nIndex].Dir);

	int nSize = m_ZoneInfo[nZoneIndex].vtPlayers.size();
	for (int i = 0; i < nSize; ++i )
	{
		int nUserIndex = m_ZoneInfo[nZoneIndex].vtPlayers.at(i);
		GCSendCastleGateInfo(nIndex, nZoneIndex, nUserIndex);
	}

	m_ZoneInfo[nZoneIndex].vtGateInfo.push_back(nIndex);

	LogAddTD("[IMPERIALGUARDIAN] DestroyGate -> [ZONE]:%d, [AccountID]:%s, [NAME]:%s, [GATE INDEX]:%d, [USER COUNT]:%d ",
			nZoneIndex + 1, gObj[nTargetIndex].AccountID, gObj[nTargetIndex].Name, nIndex, nSize);
}

void CImperialGuardian::GCSendServerMsgAll(int nZoneIndex, char *szMsg)	//OK
{
	std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers);

	for ( auto it = vtTemp.begin(); it != vtTemp.end(); ++it )
	{
		int nResult = *it;
		if ( gObj[nResult].Connected > 0 )
			GCServerMsgStringSend(szMsg, nResult, 0);
	}
}

int CImperialGuardian::ImperialGuardianLevelUp(int iIndex, int iAddExp)	//OK
{
  	if( !OBJMAX_RANGE(iIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0;
	}

    if ( g_MasterLevelSystem.MasterLevelUp(&gObj[iIndex], iAddExp, true, 0) )
    {
		return 0;
    }

	gObjSetExpPetItem(iIndex, iAddExp);
	int iLEFT_EXP = 0;
	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %I64u %d ", 
		gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y,
		gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level, 
		gObj[iIndex].Experience, iAddExp);
	

	if ( gObj[iIndex].Level >= MAX_CHAR_LEVEL )
	{
		GCServerMsgStringSend(lMsg.Get(1136), gObj[iIndex].m_Index, 1);
		return 0;
	}


	if ( iAddExp + gObj[iIndex].Experience >= gObj[iIndex].NextExp )
	{
		iLEFT_EXP = iAddExp + gObj[iIndex].Experience - gObj[iIndex].NextExp;
		gObj[iIndex].Experience = gObj[iIndex].NextExp;

		++gObj[iIndex].Level;

		if ( gObj[iIndex].Class != 4 && gObj[iIndex].Class != 3 && gObj[iIndex].Class != 6 )
			gObj[iIndex].LevelUpPoint += 5;
		else
			gObj[iIndex].LevelUpPoint += 7;

		if ( gObj[iIndex].PlusStatQuestClear && gObj[iIndex].Level >= g_ResetSystem.m_MarlonStatMinLevel )
		{
			gObj[iIndex].LevelUpPoint++;
			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].LevelUpPoint);
		}

		gObj[iIndex].MaxLife = gObj[iIndex].MaxLife + DCInfo.DefClass[gObj[iIndex].Class].LevelLife;
		gObj[iIndex].MaxMana = gObj[iIndex].MaxMana + DCInfo.DefClass[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;

		gObjNextExpCal(&gObj[iIndex]);
		gObjSetBP(gObj[iIndex].m_Index);

		GCLevelUpMsgSend(gObj[iIndex].m_Index, 1);

		gObjCalcMaxLifePower(gObj[iIndex].m_Index);

		LogAddTD(lMsg.Get(520), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level);

		if ( gObj[iIndex].Level == MAX_CHAR_LEVEL )
		{
			if ( gObj[iIndex].PartyNumber >= 0 )
			{
				char partyLog[256] = {0};

				int partynum = gObj[iIndex].PartyNumber;
				sprintf(partyLog, "400 LevelUp (%s)(%s) Party ", gObj[iIndex].AccountID, gObj[iIndex].Name);

				for (int n = 0; n < 5; ++n )
				{
					int number = gParty.m_PartyS[partynum].Number[n];

					if ( number >= 0 )
					{
						sprintf(&partyLog[strlen(partyLog)], ",(%s)(%s) ", gObj[number].AccountID, gObj[number].Name);
					}
				}

				LogAddTD(partyLog);
			}
		}
	}
	else
	{
		gObj[iIndex].Experience += iAddExp;
	}

	GJSetCharacterInfo(&gObj[iIndex], gObj[iIndex].m_Index, 0, 0);

	return iLEFT_EXP;
}

void CImperialGuardian::RegAllPartyUser(int nZoneIndex, int nFirstEnterUserIndex)	//OK
{
	if ( gObj[nFirstEnterUserIndex].Connected >= 1 )
	{
		bool bAddUser = 1;
		int nPartyNumber = gObj[nFirstEnterUserIndex].PartyNumber;
		int nPartyCount = gParty.GetPartyCount(nPartyNumber);

		for (int i = 0; i < nPartyCount; ++i )
		{
			int nUserNumber = gParty.m_PartyS[nPartyNumber].Number[i];
			m_ZoneInfo[nZoneIndex].vtUkn3Ch.push_back(nUserNumber);
		}
	}
}

bool CImperialGuardian::IsRegPartyUser(int nZoneIndex, int nUserIndex)	//OK
{
	bool bFlag = false;

	int nSize = m_ZoneInfo[nZoneIndex].vtUkn3Ch.size();
	for (int i = 0; i < nSize; ++i )
	{
		int nResult = m_ZoneInfo[nZoneIndex].vtUkn3Ch.at(i);

		if ( nResult == nUserIndex )
			bFlag = 1;
	}

	return bFlag;
}


//CLogToFile IG_TEST_LOG("IG_TEST_LOG", ".\\LOG\\IMPERIALGUARDIAN_TEST_LOG", 1);


void CImperialGuardian::UserMonsterCountCheck()	//OK
{
	return;

	// there is a code in gs, can be decompiled, if needed
}


void CImperialGuardian::MonsterBaseAct(OBJECTSTRUCT *lpObj)	//OK
{

	OBJECTSTRUCT *lpTargetObj = NULL;

	if ( lpObj->TargetNumber < 0 )
	{
		lpObj->m_ActState.Emotion = 0;
	}
	else
	{
		lpTargetObj = &gObj[lpObj->TargetNumber];
	}


	if(lpObj->m_ActState.Emotion == 0)
	{
		if ( lpObj->m_ActState.Attack )
		{
			lpObj->m_ActState.Attack = 0;
			lpObj->TargetNumber = -1;
			lpObj->NextActionTime = 500;
		}

		int actcode1 = rand() % 2;

		if ( lpObj->m_MoveRange > 0 )
		{
			if ( !gObjCheckUsedBuffEffect(lpObj, 57) )
				gObjMonsterMoveAction(lpObj);
		}

		if ( lpObj->m_bIsMonsterAttackFirst )
		{
			lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, 1);

			if ( lpObj->TargetNumber >= 0 )
			{
				lpObj->m_ActState.EmotionCount = 100;
				lpObj->m_ActState.Emotion = 1;
				SetTargetMoveAllMonster(lpObj->m_ImperialZoneID, lpObj->TargetNumber);
			}
		}


	}
	else if(lpObj->m_ActState.Emotion == 1)
	{
		if ( lpObj->m_ActState.EmotionCount )
		{
			lpObj->m_ActState.EmotionCount--;
		}
		else
		{
			lpObj->m_ActState.Emotion = 0;
		}

		if ( lpObj->TargetNumber >= 0 )
		{
			if ( !lpObj->PathStartEnd )
			{
				int dis = gObjCalDistance(lpObj, lpTargetObj);
				int attackRange = 0;

				if ( lpObj->m_AttackType < 100 )
					attackRange = lpObj->m_AttackRange;
				else
					attackRange = lpObj->m_AttackRange + 2;

				if ( dis > attackRange )
				{
					if ( gObjMonsterGetTargetPos(lpObj) == 1 )
					{
						if ( MapC[lpObj->MapNumber].CheckWall(
								lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == 1 )
						{
							lpObj->m_ActState.Move = 1;
							lpObj->NextActionTime = 400;
							lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
						}
						else
						{
							gObjMonsterMoveAction(lpObj);
							lpObj->m_ActState.Emotion = 3;
							lpObj->m_ActState.EmotionCount = 10;
						}
					}
					else
					{
						int tx = 0;
						int ty = 0;
						if ( lpTargetObj->Connected > 2 )
						{
							if ( gObjGetTargetPos(lpObj, lpTargetObj->X, lpTargetObj->Y, tx, ty) == 1 )
							{
								if ( MapC[lpObj->MapNumber].CheckWall(
										lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == 1 )
								{
									lpObj->MTX = tx;
									lpObj->MTY = ty;
									lpObj->m_ActState.Move = 1;
									lpObj->NextActionTime = 400;
									lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
								}
								else
								{
									gObjMonsterMoveAction(lpObj);
									lpObj->m_ActState.Emotion = 3;
									lpObj->m_ActState.EmotionCount = 10;
								}
							}
						}

					}
				}
				else
				{
					int tuser = lpObj->TargetNumber;
					int map = gObj[tuser].MapNumber;
					if ( MapC[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == 1 )
					{
						int attr = MapC[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);

						if ( (attr & 1) == 1 )
						{
							lpObj->TargetNumber = -1;
							lpObj->m_ActState.EmotionCount = 30;
							lpObj->m_ActState.Emotion = 1;
						}
						else
						{
							lpObj->m_ActState.Attack = 1;
							lpObj->m_ActState.EmotionCount = rand() % 30 + 20;
						}

						lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
						lpObj->NextActionTime = lpObj->m_AttackSpeed;
					}
				}
			}
		}

	}
	else if(lpObj->m_ActState.Emotion == 3)
	{
		if ( lpObj->m_ActState.EmotionCount )
		{
			lpObj->m_ActState.EmotionCount--;
		}
		else
		{
			lpObj->m_ActState.Emotion = 0;
		}

		lpObj->m_ActState.Move = 0;
		lpObj->m_ActState.Attack = 0;
		lpObj->NextActionTime = 400;
	}
}

bool CImperialGuardian::ChangeUserIndex(int nUserIndex, int nCurrentUserIndex, int nZoneIndex)	//OK
{
 	if( !IsGoodZone(nZoneIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

    EnterCriticalSection(&m_cs);

	int nTotalUserCount = m_ZoneInfo[nZoneIndex].vtPlayers.size();
    for (int i = 0; i < nTotalUserCount; ++i )
    {
		if ( m_ZoneInfo[nZoneIndex].vtPlayers[i] == nUserIndex )
		{
			m_ZoneInfo[nZoneIndex].vtPlayers[i] = nCurrentUserIndex;

			LeaveCriticalSection(&m_cs);

			return true;
		}
    }

    LeaveCriticalSection(&m_cs);

	return false;
}

bool CImperialGuardian::SendCurStateToUser(int nUserIndex, int nZoneIndex)	//OK
{
	if( !IsGoodZone(nZoneIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	int nTotalUserCount = m_ZoneInfo[nZoneIndex].vtPlayers.size();

    for (int i = 0; i < nTotalUserCount; ++i )
    {
		if ( m_ZoneInfo[nZoneIndex].vtPlayers[i] == nUserIndex )
		{
			CTime tCurrentTime(CTime::GetTickCount());

			int nDayOfWeek = tCurrentTime.GetDayOfWeek();
			PMSG_IG_CURRENT_STATE pMsg = {0};

			PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x02, sizeof(pMsg));

			pMsg.EventEnded = this->ukn0Ch;
			pMsg.ukn05 = 0;
			pMsg.ZoneIndex = nZoneIndex + 1;
			pMsg.RemainTick = m_ZoneInfo[nZoneIndex].m_WaitPlayer_MSec
							+ m_ZoneInfo[nZoneIndex].m_LootTime_MSec
							+ m_ZoneInfo[nZoneIndex].m_TimeAttack_RemainMSec;

			pMsg.DayOfWeek = nDayOfWeek - 1;

			GCSendDataToUser(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

			return true;
		}
    }
 
	return false;
}

bool CImperialGuardian::SendCurGateInfoToUser(int nUserIndex, int nZoneIndex)	//OK
{
	if( !IsGoodZone(nZoneIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

    int nSize = m_ZoneInfo[nZoneIndex].vtGateInfo.size();
    for (int i = 0; i < nSize; ++i )
    {
		int nGateIndex = m_ZoneInfo[nZoneIndex].vtGateInfo.at(i);
		GCSendCastleGateInfo(nGateIndex, nZoneIndex, nUserIndex);
    }

	return true;
}

int CImperialGuardian::GetPlayUserCountRightNow(int nZoneIndex)	//OK
{
	if( !IsGoodZone(nZoneIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return -1;
	}

    int nPlayUserCount = 0;
    std::vector<int> vtTemp(m_ZoneInfo[nZoneIndex].vtPlayers);

	for(auto it = vtTemp.begin(); it != vtTemp.end(); ++it)
    {
		int nResult = *it;

		if( OBJMAX_RANGE(nResult) )
		{
			if ( gObj[nResult].Connected == 3 )
			{
				if ( IsEventMap(gObj[nResult].MapNumber) )
					++nPlayUserCount;
			}
		}

	}

	return nPlayUserCount;
}
