// IllusionTempleProcess.cpp: implementation of the CIllusionTempleProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IllusionTempleProcess.h"
#include "IllusionTempleEvent.h"
#include "..\include\readscript.h"
#include "..\\Common\\Winutil.h"
#include "..\\Include\\Prodef.H"
#include "User.h"
#include "GameMain.h"
#include "Protocol.h"
#include "DSprotocol.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSystem.h"
#include "ProbabilityItemBag.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "TNotice.h"
#include "MonsterSetBase.h"
#include "Event.h"
#include "..\\Common\\Classdef.h"
#include "MagicDamage.h"
#include "logproc.h"
#include "ItemDef.h"
#include "BuffEffectSlot.h"
#ifdef WZQUEST
#include "QuestExpProgMng.h"
#endif
#ifdef __CUSTOMS__
#include "ShopPointEx.h"
#include "ResetSystem.h"
#endif

extern PartyClass		gParty;
extern CMonsterSetBase	gMSetBase;
extern classdef			DCInfo;
extern CProbabilityItemBag *IllusionTempleGhostItemBagA;
extern CProbabilityItemBag *IllusionTempleGhostItemBagB;
extern CProbabilityItemBag *IllusionTempleGhostItemBagC;

int CIllusionTempleEvent::m_nIllusionTempleOpenTime;
int CIllusionTempleEvent::m_nIllusionTempleReadyTime;
int CIllusionTempleEvent::m_nIllusionTemplePlayTime;
int CIllusionTempleEvent::m_nIllusionTempleEndTime;
std::list<__ILLUSION_TEMPLE_STARTTIME> CIllusionTempleEvent::m_lstIllusionTempleStartTime;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIllusionTempleProcess::CIllusionTempleProcess(): m_nTempleNumber(-1), m_btIllusionTempleState(0x00), m_nNextTimeMin(0), m_nStatusCount(0), m_nMonsterCount(0)
{
	InitializeCriticalSection(&m_critUserData);

	m_nRemainTimeMSec = -1;

	ResetPosNum();
}

CIllusionTempleProcess::~CIllusionTempleProcess()
{
	DeleteCriticalSection(&m_critUserData);
}

void CIllusionTempleProcess::IllusionTempleProcessInit(int nTempleNum)
{
	if( m_nTempleNumber == 0xFF )
	{
		m_nTempleNumber = nTempleNum;
	}

	int i;

	for(i = 0; i < 10; i++)
	{
		ClearUserData(i);
	}

	m_nTickCount = GetTickCount();

	m_nRelicsUserIndex = -1;
	
	m_btInventoryPos = 0xFF;
	
	m_btRelicsTeam = 0xFF;
	
	m_nAlliedUserCount = 0;
	m_nIllusionUserCount = 0;
	
	m_btAlliedPoint = 0x00;
	m_btIllusionPoint = 0x00;
	
	m_StatusRegenTime = -1;
	
	m_btWinTeam = 0xFF;
	
	m_bEntrance = FALSE;

	m_nRemainTimeMSec = -1;
	
	m_bGameEndPosMove = FALSE;
	
	m_bSendMsgBeforeReady = FALSE;
	m_bSendMsgBeforePlay = FALSE;
	
	m_nEnventEndRemainTime = 0;

	ResetSkillPoint();
	CheckSync();

	m_bAllCheckSync = FALSE;

	for(i = 0; i < MAX_TYPE_PLAYER; i++)
	{
		m_btAlliedClassCnt[i] = 0;
		m_btIllusionClassCnt[i] = 0;
	}

	m_bSendResult = FALSE;
}

void CIllusionTempleProcess::ClearUserData(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	m_dwViewSkillState[nIndex] = 0;
	
	m_UserData[nIndex].m_dwSkillProdectionTime = 0;
	m_UserData[nIndex].m_dwSkillRestraintTime = 0;
	
	m_UserData[nIndex].m_nIndex = -1;
	m_UserData[nIndex].m_btTeam = -1;
	
	m_UserData[nIndex].m_btUserKillCount = 0;
	m_UserData[nIndex].m_btMonsterKillCount = 0;
	
	m_UserData[nIndex].m_btRelicsMoveCount = 0;
	m_UserData[nIndex].m_btUseSkillCount = 0;
	
	m_UserData[nIndex].m_nRewardExp = 0;
	m_UserData[nIndex].m_nRankingPoint = 0;
	m_UserData[nIndex].m_bReward = FALSE;
}

void CIllusionTempleProcess::CheckSync()
{
	std::list<__ILLUSION_TEMPLE_STARTTIME>::iterator iter;

	__ILLUSION_TEMPLE_STARTTIME prevStartTime;
	__ILLUSION_TEMPLE_STARTTIME nextStartTime;

	DWORD dwPrevTime = 0;
	DWORD dwNextTime = 0;
	DWORD dwCurTime = 0;

	if(CIllusionTempleEvent::m_lstIllusionTempleStartTime.size() == 0)
	{
		MsgBox("Error : Illusion Temple StartTime size is 0");
		return;
	}

	tm* today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	dwCurTime = (today->tm_hour * 60) + today->tm_min;
	prevStartTime = *CIllusionTempleEvent::m_lstIllusionTempleStartTime.begin();

	for( iter = CIllusionTempleEvent::m_lstIllusionTempleStartTime.begin(); iter != CIllusionTempleEvent::m_lstIllusionTempleStartTime.end(); ++iter )
	{
		nextStartTime = *iter;

		dwPrevTime = (prevStartTime.nHour * 60) + prevStartTime.nMin;
		dwNextTime = (nextStartTime.nHour * 60) + nextStartTime.nMin;

		if( dwPrevTime == dwNextTime )
		{
			if( dwCurTime < dwNextTime )
			{
				nextStartTime = *iter;
				break;
			}
			continue;
		}
		
		if( dwCurTime >= dwPrevTime && 
			dwCurTime < dwNextTime )
		{
			break;
		}

		prevStartTime = *iter;
	}

	int Cnt = 2;
	
	while(Cnt-- != 0)
	{
		if(iter == CIllusionTempleEvent::m_lstIllusionTempleStartTime.end())
		{
			iter = CIllusionTempleEvent::m_lstIllusionTempleStartTime.begin();

			nextStartTime = (*iter);
		}

		dwNextTime = nextStartTime.nHour*60+nextStartTime.nMin;

		if(today->tm_hour <= nextStartTime.nHour)
		{
			m_nRemainTimeMin = dwNextTime - dwCurTime;
		}
		else
		{
			m_nRemainTimeMin = ((24*60)-dwCurTime) + dwNextTime;
		}

		if(m_nRemainTimeMin <= CIllusionTempleEvent::m_nIllusionTempleOpenTime)
		{
			iter++;

			if(iter != CIllusionTempleEvent::m_lstIllusionTempleStartTime.end())
			{
				nextStartTime = (*iter);
			}
		}
		else
		{
			break;
		}
	}

	int tmpTime = GetTickCount();
	m_nNextTimeMin = tmpTime + ((1000*60) - (today->tm_sec * 1000));

	LogAddTD("[Illusion Temple] (%d) Sync Open Time. [%d]min remain", m_nTempleNumber+1, m_nRemainTimeMin);
}

void CIllusionTempleProcess::ProcIllusionTemple(int nCurTime)
{
	BYTE btState = m_btIllusionTempleState;

	switch ( btState )
	{
	case 0:	ProcIllusionTempleState_None(nCurTime);		break;
	case 1:	ProcIllusionTempleState_Ready(nCurTime);		break;
	case 2:	ProcIllusionTempleState_Playing(nCurTime);	break;
	case 3:	ProcIllusionTempleState_End(nCurTime);		break;
	}
}

void CIllusionTempleProcess::ProcIllusionTempleState_None(int nCurTime)
{
	int iTICK_MSEC = nCurTime - m_nTickCount;

	if(m_nNextTimeMin < nCurTime)
	{
		m_nRemainTimeMin--;
		m_nNextTimeMin = nCurTime + 1000*60;

		if(m_nRemainTimeMin <= CIllusionTempleEvent::m_nIllusionTempleOpenTime)
		{
			if(m_nRemainTimeMin > 0)
			{
				if(m_nTempleNumber == 0)
				{
					PMSG_NOTICE pNotice;
					TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(3394), m_nRemainTimeMin);
					SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);
				}
			}
		}
	}

	if(iTICK_MSEC >= 1000)
	{
		if(m_nRemainTimeMSec > 0)
		{
			m_nRemainTimeMSec -= iTICK_MSEC;
		}

		m_nTickCount = nCurTime;

		if(m_nRemainTimeMin <= CIllusionTempleEvent::m_nIllusionTempleOpenTime+1)
		{
			if(m_nRemainTimeMSec == -1)
			{
				m_nRemainTimeMSec = 1000 * ( (CIllusionTempleEvent::m_nIllusionTempleOpenTime+1) * 60);
			}
		}

		if(m_nRemainTimeMin <= CIllusionTempleEvent::m_nIllusionTempleOpenTime)
		{
			if(m_nRemainTimeMSec > 1000*5)
			{
				if(m_bEntrance == FALSE)
				{
					m_bEntrance = TRUE;
				}
			}
		}

		if(m_nRemainTimeMSec <= 1000*30 && m_nRemainTimeMSec > 0)
		{
			if(m_nTempleNumber == 0)
			{
				if(m_bSendMsgBeforeReady == FALSE)
				{
					m_bSendMsgBeforeReady = TRUE;

					PMSG_SET_DEVILSQUARE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));

					pMsg.Type = 14;

					for(int n = OBJ_STARTUSERINDEX; n < OBJMAX; n++)
					{
						if( gObj[n].Connected == 3 && 
							gObj[n].Type == OBJ_USER)
						{
							if( !BC_MAP_RANGE(gObj[n].MapNumber) )
							{
								if( !CC_MAP_RANGE(gObj[n].MapNumber) )
								{
									DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
								}
							}
						}
					}
				}
			}
		}

		if(m_nRemainTimeMSec <= 1000*5)
		{
			if(m_bEntrance == TRUE)
			{
				m_bEntrance = FALSE;

				if(m_nTempleNumber == 0)
				{
					PMSG_NOTICE pNotice;
					TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(3395));
					SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);
				}
			}
		}
	}

	if(m_nRemainTimeMin <= 0)
	{
		if(m_nRemainTimeMSec <= 0)
		{
			m_bEntrance = FALSE;

			CheckUsersOnConnect();

			if( m_nUserCount < g_IllusionTempleEvent.m_MinPlayerCount )
			{
				for( int i = 0; i < 10; i++ )
				{
					if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
					{
						if( gObj[m_UserData[i].m_nIndex].MapNumber == (m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1) )
						{
							int nScroolPos = m_UserData[i].m_btItemPos; //142

							if(nScroolPos > -1)
							{
								if(gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Type == (ITEM_HELPER+51))
								{
									gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability = 1.0f;
									GCItemDurSend(gObj[m_UserData[i].m_nIndex].m_Index, nScroolPos,  gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability, 0);
								}
								else if(gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Type == (ITEM_HELPER+61))
								{
									gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability += 1.0f;
									GCItemDurSend(gObj[m_UserData[i].m_nIndex].m_Index, nScroolPos,  gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability, 0);
								}

								LogAddTD("[Illusion Temple] (%d) Payback to User (%s)(%s) 피의 두루마리 (Serial:%u)", m_nTempleNumber+1, gObj[m_UserData[i].m_nIndex].AccountID, gObj[m_UserData[i].m_nIndex].Name, gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Number);
							}
							else
							{
								LogAddTD("[Illusion Temple] (%d) Payback to User Failed (%s)(%s) Scroll Not Found", m_nTempleNumber+1, gObj[m_UserData[i].m_nIndex].AccountID, gObj[m_UserData[i].m_nIndex].Name);
							}
							gObjMoveGate(m_UserData[i].m_nIndex, 267); //Elbeland
						}
					}
				}

				LogAddTD("[Illusion Temple] (%d) Failed to Start Illusion Temple (UserCount: %d)", m_nTempleNumber+1, m_nUserCount);
				SetIllusionTempleState(3);
				return;
			}
			
			int nTeam = 0;
			int nAlliedPartyNum = -1;
			int nIllusionPartyNum = -1;

			char szAlliedTeamLog[256];
			sprintf(szAlliedTeamLog,"[Illusion Temple] (%d) [AlliedTeam] ", m_nTempleNumber+1);

			char szIllusionTeamLog[256];
			sprintf(szIllusionTeamLog,"[Illusion Temple] (%d) [IllusiondTeam] ", m_nTempleNumber+1);

			for( int i = 0; i < 10; i++)
			{
				if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].m_iIllusionTempleIndex == i)
				{
					if( gObj[m_UserData[i].m_nIndex].Class >= MAX_TYPE_PLAYER || gObj[m_UserData[i].m_nIndex].Class < 0)
					{
						continue;
					}

					int nScrollPos = m_UserData[i].m_btItemPos; //season 3.0 changed

					if(nScrollPos > -1)
					{
						if(gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Durability <= 0.0f) //season 3.0 add-on
						{
							if(gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Type == (ITEM_HELPER+51))
							{
								gObjInventoryDeleteItem(m_UserData[i].m_nIndex, nScrollPos);
								GCInventoryItemDeleteSend(m_UserData[i].m_nIndex, nScrollPos, 0);
							}
							else if(gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Type == (ITEM_HELPER+61))
							{
								if(gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Durability <= 0.0f)
								{
									gObjInventoryDeleteItem(m_UserData[i].m_nIndex, nScrollPos);
									GCInventoryItemDeleteSend(m_UserData[i].m_nIndex, nScrollPos, 0);
								}
							}
						}
					}

					if(m_btAlliedClassCnt[gObj[m_UserData[i].m_nIndex].Class] == m_btIllusionClassCnt[gObj[m_UserData[i].m_nIndex].Class])
					{
						if(m_nAlliedUserCount == m_nIllusionUserCount)
						{
							m_UserData[i].m_btTeam = rand()%2;
						}
						else if(m_nAlliedUserCount > m_nIllusionUserCount)
						{
							m_UserData[i].m_btTeam = 1;
						}
						else
						{
							m_UserData[i].m_btTeam = 0;
						}
					}
					else if(m_btAlliedClassCnt[gObj[m_UserData[i].m_nIndex].Class] < m_btIllusionClassCnt[gObj[m_UserData[i].m_nIndex].Class])
					{
						if(m_nAlliedUserCount > m_nIllusionUserCount)
						{
							m_UserData[i].m_btTeam = 1;
						}
						else
						{
							m_UserData[i].m_btTeam = 0;
						}
					}
					else if(m_nAlliedUserCount < m_nIllusionUserCount)
					{
						m_UserData[i].m_btTeam = 0;
					}
					else
					{
						m_UserData[i].m_btTeam = 1;
					}

					if(m_UserData[i].m_btTeam == 0)
					{
						m_UserData[i].m_btTeam = 0; //wtf??
						m_btAlliedClassCnt[gObj[m_UserData[i].m_nIndex].Class]++;

						if(nAlliedPartyNum == -1 && gObj[m_UserData[i].m_nIndex].PartyNumber < 0)
						{
							nAlliedPartyNum = gParty.Create(m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);

							if(nAlliedPartyNum == -1)
							{
								LogAddTD("[Illusion Temple] (%d) Party Create Error, (Team: %d)", m_nTempleNumber+1, m_UserData[i].m_btTeam);	
							}
							gObj[m_UserData[i].m_nIndex].PartyNumber = nAlliedPartyNum;
						}
						else
						{
							int nRet = gParty.Add(nAlliedPartyNum, m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);

							if(nRet >= 0)
							{
								gObj[m_UserData[i].m_nIndex].PartyNumber = nAlliedPartyNum;
							}
							else
							{
								LogAddTD("[Illusion Temple] (%d) Party Add Error, (Team: %d)", m_nTempleNumber+1, m_UserData[i].m_btTeam);
							}
						}

						gObjMoveGate(m_UserData[i].m_nIndex, m_nTempleNumber+148); //Temple
						gObj[m_UserData[i].m_nIndex].m_Change = 404; //"MU Allies"
						gObjViewportListProtocolCreate(&gObj[m_UserData[i].m_nIndex]);

						m_nAlliedUserCount++;
						nTeam++;

						int tmpSize = strlen(szAlliedTeamLog);

						sprintf(&szAlliedTeamLog[tmpSize],"(%s),(%s) ",gObj[m_UserData[i].m_nIndex].AccountID,gObj[m_UserData[i].m_nIndex].Name);
					}
					else
					{
						m_UserData[i].m_btTeam = 1;

						m_btIllusionClassCnt[gObj[m_UserData[i].m_nIndex].Class]++;

						if(nIllusionPartyNum == -1 && gObj[m_UserData[i].m_nIndex].PartyNumber < 0)
						{
							nIllusionPartyNum = gParty.Create(m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);

							if(nIllusionPartyNum == -1)
							{
								LogAddTD("[Illusion Temple] (%d) Party Create Error, (Team: %d)", m_nTempleNumber+1, m_UserData[i].m_btTeam);	
							}

							gObj[m_UserData[i].m_nIndex].PartyNumber = nIllusionPartyNum;
						}
						else
						{
							int nRet = gParty.Add(nIllusionPartyNum, m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);

							if(nRet >= 0)
							{
								gObj[m_UserData[i].m_nIndex].PartyNumber = nIllusionPartyNum;
							}
							else
							{
								LogAddTD("[Illusion Temple] (%d) Party Add Error, (Team: %d)", m_nTempleNumber+1, m_UserData[i].m_btTeam);
							}
						}

						gObjMoveGate(m_UserData[i].m_nIndex, m_nTempleNumber+154); //Temple
						gObj[m_UserData[i].m_nIndex].m_Change = 405; //"Illusion Sorcerer"
						gObjViewportListProtocolCreate(&gObj[m_UserData[i].m_nIndex]);

						m_nIllusionUserCount++;
						nTeam++;

						int tmpSize = strlen(szIllusionTeamLog);

						sprintf(&szIllusionTeamLog[tmpSize],"(%s),(%s) ",gObj[m_UserData[i].m_nIndex].AccountID,gObj[m_UserData[i].m_nIndex].Name);
					}
				}
			}

			CGPartyListAll(nAlliedPartyNum);
			CGPartyListAll(nIllusionPartyNum);

			SetIllusionTempleState(1);

			LogAddTD("[Illusion Temple] (%d) Allied Team 법사(%d),기사(%d), 요정(%d), 마검사(%d), 다크로드(%d)", m_nTempleNumber+1, m_btAlliedClassCnt[0], m_btAlliedClassCnt[1], m_btAlliedClassCnt[2], m_btAlliedClassCnt[3], m_btAlliedClassCnt[4]);

			LogAddTD(szAlliedTeamLog);

			LogAddTD("[Illusion Temple] (%d) Illusion Team 법사(%d),기사(%d), 요정(%d), 마검사(%d), 다크로드(%d)", m_nTempleNumber+1, m_btIllusionClassCnt[0], m_btIllusionClassCnt[1], m_btIllusionClassCnt[2], m_btIllusionClassCnt[3], m_btIllusionClassCnt[4]);

			LogAddTD(szIllusionTeamLog);
		}
	}
}

void CIllusionTempleProcess::ProcIllusionTempleState_Ready(int nCurTime)
{
	int iTICK_MSEC = nCurTime - m_nTickCount;

	if(iTICK_MSEC >= 1000)
	{
		m_nRemainTimeMSec -= iTICK_MSEC;
		m_nTickCount = nCurTime;

		if(m_nRemainTimeMSec <= 1000*30)
		{
			if(m_nRemainTimeMSec > 0)
			{
				if(m_bSendMsgBeforePlay == FALSE)
				{
					m_bSendMsgBeforePlay = TRUE;

					PMSG_SET_DEVILSQUARE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
					pMsg.Type = 15;

					for( int i = 0; i < 10; i++ )
					{
						if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
						{
							if( gObj[m_UserData[i].m_nIndex].MapNumber == (m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1) )
							{
								DataSend(m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
							}
						}
					}
				}
			}
		}
	}

	if(m_nUserCount == 0)
	{
		SetIllusionTempleState(3);
		return;
	}

	if(m_nRemainTimeMSec <= 0)
	{
		SetIllusionTempleState(2);
		SendIllusionTemplePlayState(0);
		SendIllusionTemplePlayState(1);
	}
}

void CIllusionTempleProcess::ProcIllusionTempleState_Playing(int nCurTime)
{
	int iTICK_MSEC = nCurTime - m_nTickCount;

	if(iTICK_MSEC >= 1000)
	{
		m_nRemainTimeMSec -= iTICK_MSEC;
		m_nTickCount = nCurTime;

		if(m_nRemainTimeMSec > 0)
		{
			SendIllusionTemplePlayState(0);
			SendIllusionTemplePlayState(1);
		}

		if(m_StatusRegenTime != -1 && m_StatusRegenTime < nCurTime)
		{
			RegenStatus();
		}
	}

	if(m_nUserCount == 0)
	{
		SetIllusionTempleState(3);
		return;
	}

	if( m_nRemainTimeMSec <= 0 || 
		m_btAlliedPoint >= 0x07 || m_btIllusionPoint >= 0x07 || 
		m_nAlliedUserCount == 0 || m_nIllusionUserCount == 0)
	{
		if(m_btAlliedPoint >= 0x07 || m_btIllusionPoint >= 0x07)
		{
			if(m_nRemainTimeMSec <= 0)
			{
				m_nEnventEndRemainTime = 0;
			}
			else
			{
				m_nEnventEndRemainTime = m_nRemainTimeMSec / 1000;
			}
		}

		SendIllusionTemplePlayState(0); //A-Team
		SendIllusionTemplePlayState(1); //B-Team

		SetIllusionTempleState(3);
	}
}

void CIllusionTempleProcess::ProcIllusionTempleState_End(int nCurTime)
{
	int iTICK_MSEC = nCurTime - m_nTickCount;

	if(iTICK_MSEC >= 1000)
	{
		m_nRemainTimeMSec -= iTICK_MSEC;
		m_nTickCount = nCurTime;

		if(m_nRemainTimeMSec <= 1000 * (CIllusionTempleEvent::m_nIllusionTempleEndTime * 60) - 1000*3)
		{
			if(m_nRemainTimeMSec > 0)
			{
				if(m_bGameEndPosMove == FALSE)
				{
					m_bGameEndPosMove = TRUE;

					for( int i = 0; i < 10; i++ )
					{
						if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
						{
							if( gObj[m_UserData[i].m_nIndex].MapNumber == (m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1) )
							{
								if(m_UserData[i].m_btTeam == 0)
								{
									gObjMoveGate(m_UserData[i].m_nIndex, m_nTempleNumber + 148); //Battle Map Scene
								}
								else if(m_UserData[i].m_btTeam == 1)
								{
									gObjMoveGate(m_UserData[i].m_nIndex, m_nTempleNumber + 154); //Battle Map Scene
								}

								gObjSetPosition(m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].X, gObj[m_UserData[i].m_nIndex].Y);
							}
						}
					}

					if(m_nRelicsUserIndex >= 0 && m_btInventoryPos >= 0)
					{
						if(gObj[m_nRelicsUserIndex].pInventory[m_btInventoryPos].m_Type == (ITEM_POTION+64))
						{
							::gObjInventoryDeleteItem(m_nRelicsUserIndex, m_btInventoryPos);
							::GCInventoryItemDeleteSend(m_nRelicsUserIndex, m_btInventoryPos, 0);

							m_nRelicsUserIndex = -1;
							m_btInventoryPos = 0xFF;
							m_btRelicsTeam = 0xFF;
						}
					}

					CalCursetTempleResult();
				}
			}
		}
	}

	if(m_nRemainTimeMSec <= 1000 * (CIllusionTempleEvent::m_nIllusionTempleEndTime * 60) - 1000*6)
	{
		if(m_nRemainTimeMSec > 0)
		{
			if(m_bGameEndPosMove == TRUE)
			{
				if(m_bSendResult == FALSE)
				{
					SendIllusionTempleResult();
					m_bSendResult = TRUE;
				}
			}
		}
	}

	if(m_nRemainTimeMSec <= 0)
	{
		SetIllusionTempleState(0);
	}
}

void CIllusionTempleProcess::SetIllusionTempleState(BYTE btState)
{
	m_btIllusionTempleState = btState;

	switch( btState )
	{
	case 0:	SetIllusionTempleStateNone();		break;
	case 1:	SetIllusionTempleStateReady();	break;
	case 2:	SetIllusionTempleStatePlaying();	break;
	case 3:	SetIllusionTempleStateEnd();		break;
	}
}

BYTE CIllusionTempleProcess::GetIllusionTempleState()
{
	return m_btIllusionTempleState;
}

void CIllusionTempleProcess::SetIllusionTempleStateNone()
{
	SendIllusionTempleState(0, -1);

	for(int i = 0; i < 10; i++)
	{
		if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			if( gObj[m_UserData[i].m_nIndex].MapNumber == (m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1) )
			{
				gObjMoveGate(m_UserData[i].m_nIndex, 267); //Elbeland
			}
		}
	}

	IllusionTempleProcessInit(m_nTempleNumber);
}

void CIllusionTempleProcess::SetIllusionTempleStateReady()
{
	m_nRemainTimeMSec = 1000 * (CIllusionTempleEvent::m_nIllusionTempleReadyTime * 60);

	SendIllusionTempleState(1, -1);

	for(int i = 0; i < 10; i++)
	{
		if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			if( gObj[m_UserData[i].m_nIndex].MapNumber == (m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1) )
			{
				GCSendIllusionTempleKillCount(m_UserData[i].m_nIndex, 10);
			}
		}
	}
}

void CIllusionTempleProcess::SetIllusionTempleStatePlaying()
{
	m_nRemainTimeMSec = 1000 * (CIllusionTempleEvent::m_nIllusionTemplePlayTime * 60);

	AddNpcStatus();
	AddNpcRelicsBox();
	AddMonsterGhost();
	SetStatusRegenTime();

	SendIllusionTempleState(2, -1);
}

void CIllusionTempleProcess::SetIllusionTempleStateEnd()
{
	m_nRemainTimeMSec = 1000 * (CIllusionTempleEvent::m_nIllusionTempleEndTime * 60);

	SendIllusionTempleState(3, -1);

	for(int i = 0; i < 10; i++)
	{
		if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			if( gObj[m_UserData[i].m_nIndex].MapNumber == (m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1) )
			{
				if( m_UserData[i].m_dwSkillProdectionTime > 0 )
				{
					m_UserData[i].m_dwSkillProdectionTime = 0;
					gObjRemoveBuffEffect(&gObj[m_UserData[i].m_nIndex], AT_ILLUSION_SHIELD_SPELL);
					SendIllusionTempleSkillEnd(&gObj[m_UserData[i].m_nIndex], 210);
				}

				if( m_UserData[i].m_dwSkillRestraintTime > 0 )
				{
					m_UserData[i].m_dwSkillRestraintTime = 0;
					gObjRemoveBuffEffect(&gObj[m_UserData[i].m_nIndex], AT_ILLUSION_RESTRICTION_SPELL);
					SendIllusionTempleSkillEnd(&gObj[m_UserData[i].m_nIndex], 211);
				}
			}
		}
	}

	RemoveAllObj();
}

BOOL CIllusionTempleProcess::EnterUserIllusionTemple(int aIndex, BYTE btTempleIndex, BYTE btItemPos)
{
	if( !CHECK_LIMIT(aIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if(GetIllusionTempleState() >= 1)
	{
		return FALSE;
	}

	if(gObj[aIndex].m_iIllusionTempleIndex != -1)
	{
		LogAddTD("Enter Error: IllusionTempleIndex Fail");
		return FALSE;
	}

	int nItemPos = SearchRelicsItem(aIndex);

	if(nItemPos >= 0)
	{
		if( gObj[aIndex].pInventory[nItemPos].m_Type == (ITEM_POTION+64) )
		{
			::gObjInventoryDeleteItem(aIndex, nItemPos);
			::GCInventoryItemDeleteSend(aIndex, nItemPos, 0);
		}
	}

	BOOL bResult = FALSE;

	EnterCriticalSection(&m_critUserData);

	for( int i = 0; i < 10; i++ )
	{
		if( m_UserData[i].m_nIndex == -1 )
		{
			m_UserData[i].m_nIndex = aIndex;
			m_nUserCount++;
			gObj[aIndex].m_iIllusionTempleIndex = i;

			bResult = TRUE;

			m_UserData[i].m_btItemPos = btItemPos;
			break;
		}
	}

	LeaveCriticalSection(&m_critUserData);

	return bResult;
}

BOOL CIllusionTempleProcess::LeaveUserIllusionTemple(int aIndex)
{
	if( !CHECK_LIMIT(aIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	EnterCriticalSection(&m_critUserData);

	if(gObj[aIndex].m_iIllusionTempleIndex != -1)
	{
		if(aIndex == m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_nIndex)
		{
			__ILLUSION_TEMPLE_USER_DATA * lpUserData = &m_UserData[gObj[aIndex].m_iIllusionTempleIndex];

			if(lpUserData->m_btItemPos > -1)
			{
				CItem * lpItem = &gObj[aIndex].pInventory[lpUserData->m_btItemPos];

				if(lpItem != NULL)
				{
					if(lpItem->IsItem() == TRUE)
					{
						if(lpItem->m_Durability < 1.0f)
						{
							gObjInventoryDeleteItem(aIndex, lpUserData->m_btItemPos);
							GCInventoryItemDeleteSend(aIndex, lpUserData->m_btItemPos, 0);
						}
					}
				}
			}

			if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
			{
				gObjRemoveBuffEffect(&gObj[aIndex], AT_ILLUSION_SHIELD_SPELL); //season 3.0 add-on
				SendIllusionTempleSkillEnd(&gObj[aIndex], 210);
			}

			if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
			{
				gObjRemoveBuffEffect(&gObj[aIndex], AT_ILLUSION_RESTRICTION_SPELL); //season 3.0 add-on
				SendIllusionTempleSkillEnd(&gObj[aIndex], 211);
			}

			if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_btTeam == 0)
			{
				m_nAlliedUserCount--;
			}
			else if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_btTeam == 1)
			{
				m_nIllusionUserCount--;
			}

			ClearUserData(gObj[aIndex].m_iIllusionTempleIndex);

			gObj[aIndex].m_iIllusionTempleIndex = -1;

			m_nUserCount--;

			LogAddTD("[Illusion Temple] (%d) LeaveUser: (%s)(%s)", m_nTempleNumber+1, gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}

	LeaveCriticalSection(&m_critUserData);

	if(gObj[aIndex].PartyNumber >= 0)
	{
		PMSG_PARTYDELUSER pMsg;

		int pindex = gParty.GetIndex(gObj[aIndex].PartyNumber, gObj[aIndex].m_Index, gObj[aIndex].DBNumber);

		if(pindex >= 0)
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, gObj[aIndex].m_Index);
		}
	}

	gObj[aIndex].m_Change = -1;
	gObjViewportListProtocolCreate(&gObj[aIndex]);

	return TRUE;
}

int CIllusionTempleProcess::CheckCanEnterLevel(int aIndex)
{
	int iLevel = 0;

	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( !CHECK_LIMIT(aIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	if( lpObj->Level < 220 ) return 0;
	if( lpObj->Level < 271 ) return 1;
	if( lpObj->Level < 321 ) return 2;
	if( lpObj->Level < 351 ) return 3;
	if( lpObj->Level < 381 ) return 4;
	if( lpObj->Level <= 400 && lpObj->ThirdChangeUp == 0) return 5;

	if( lpObj->Level == 400 && lpObj->ThirdChangeUp )
	{
		if(g_bUseMaxLevelIllusionTemple != 0){
			return 6;
		}
		else return 5;
	}

	return -1;
}

void CIllusionTempleProcess::CheckUsersOnConnect()
{
	EnterCriticalSection(&m_critUserData);

	m_nUserCount = 0;

	for( int i = 0; i < 10; i++ )
	{
		if( m_UserData[i].m_nIndex != -1 )
		{
			if( gObjIsConnected(m_UserData[i].m_nIndex) == FALSE )
			{
				gObj[m_UserData[i].m_nIndex].m_iIllusionTempleIndex = -1;
				ClearUserData(i);
			}
			else if( gObj[m_UserData[i].m_nIndex].MapNumber != (MAP_INDEX_ILLUSION_TEMPLE1 + m_nTempleNumber) )
			{
				gObj[m_UserData[i].m_nIndex].m_iIllusionTempleIndex = -1;
				ClearUserData(i);
			}
			else m_nUserCount++;
		}
	}

	LeaveCriticalSection(&m_critUserData);
}

BYTE CIllusionTempleProcess::GetRemainTimeMin()
{
	if(GetIllusionTempleState() == 0 && GetEntrance() == TRUE)
	{
		return m_nRemainTimeMin;
	}
	return 0;
}

BYTE CIllusionTempleProcess::GetEnterRemainTimeMin()
{
	BYTE btRemainTime = 0;

	if(GetIllusionTempleState() == 0 && GetEntrance() == TRUE)
	{
		btRemainTime = 0;
	}
	else if(GetIllusionTempleState() == 0 && GetEntrance() == FALSE)
	{
		btRemainTime = m_nRemainTimeMin - CIllusionTempleEvent::m_nIllusionTempleOpenTime;
	}
	else
	{
		std::list<__ILLUSION_TEMPLE_STARTTIME>::iterator iter;

		__ILLUSION_TEMPLE_STARTTIME prevStartTime;
		__ILLUSION_TEMPLE_STARTTIME nextStartTime;

		DWORD dwPrevTime = 0;
		DWORD dwNextTime = 0;
		DWORD dwCurTime = 0;

		if(CIllusionTempleEvent::m_lstIllusionTempleStartTime.size() == 0)
		{
			MsgBox("Error : Illusion Temple StartTime size is 0");
			return 0;
		}

		tm* today;
		time_t ltime;

		time(&ltime);
		today = localtime(&ltime);

		dwCurTime = (today->tm_hour * 60) + today->tm_min;
		prevStartTime = *CIllusionTempleEvent::m_lstIllusionTempleStartTime.begin();

		for( iter = CIllusionTempleEvent::m_lstIllusionTempleStartTime.begin(); iter != CIllusionTempleEvent::m_lstIllusionTempleStartTime.end(); ++iter )
		{
			nextStartTime = *iter;

			dwPrevTime = (prevStartTime.nHour * 60) + prevStartTime.nMin;
			dwNextTime = (nextStartTime.nHour * 60) + nextStartTime.nMin;

			if( dwPrevTime == dwNextTime )
			{
				if( dwCurTime < dwNextTime )
				{
					nextStartTime = *iter;
					break;
				}
				continue;
			}
			
			if( dwCurTime >= dwPrevTime && 
				dwCurTime < dwNextTime )
			{
				break;
			}

			prevStartTime = *iter;
		}

		int Cnt = 2;
		
		while(Cnt-- != 0)
		{
			if(iter == CIllusionTempleEvent::m_lstIllusionTempleStartTime.end())
			{
				iter = CIllusionTempleEvent::m_lstIllusionTempleStartTime.begin();

				nextStartTime = (*iter);
			}

			dwNextTime = nextStartTime.nHour*60+nextStartTime.nMin;

			if(today->tm_hour <= nextStartTime.nHour){
				btRemainTime = dwNextTime - dwCurTime;
			}
			else btRemainTime = ((24*60)-dwCurTime) + dwNextTime;

			if(btRemainTime <= CIllusionTempleEvent::m_nIllusionTempleOpenTime)
			{
				iter++;

				if(iter != CIllusionTempleEvent::m_lstIllusionTempleStartTime.end())
				{
					nextStartTime = (*iter);
				}
			}
			else
			{
				btRemainTime -= CIllusionTempleEvent::m_nIllusionTempleOpenTime;
				break;
			}
		}

		int tmpTime = GetTickCount();
		m_nNextTimeMin = tmpTime + ((1000*60) - (today->tm_sec * 1000));
	}

	return btRemainTime;
}

void CIllusionTempleProcess::IllusionHolyRelicsStatusAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(lpNpc->m_State == 0x0000 || lpNpc->m_PosNum == -1)
	{
		return;
	}

	if( (abs(lpObj->Y - lpNpc->Y)) > 3 || (abs(lpObj->X - lpNpc->X)) > 3)
	{
		return;
	}

	lpNpc->m_PosNum = -1;
	lpNpc->Life = 0;

	gObjLifeCheck(lpNpc, lpObj, 0, 1, 0, 0, 0, 0);

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iDropItemNum = 0;
	int iDropItemRate = 0;
	int iExOption = 0;

	iType = ItemGetNumberMake(14, 64);

	iOption1 = 0;
	iOption2 = 0;
	iOption3 = 0;
	
	iLevel = 0;

	ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpNpc->X, lpNpc->Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

	LogAddTD("[Illusion Temple] (%d) [%s][%s] Click To Status (X:%d, Y:%d)", m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, lpNpc->X, lpNpc->Y);

	RegenMonster();
}

void CIllusionTempleProcess::HolyRelicsBoxAlliedAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(m_btRelicsTeam == 0xFF || m_btRelicsTeam != 0 || lpObj->m_Index != m_nRelicsUserIndex)
	{
		return;
	}

	if(lpObj->pInventory[m_btInventoryPos].m_Type == (ITEM_POTION+64))
	{
		m_btAlliedPoint++;
		DeleteRelicsItem(lpObj);
		m_btRelicsTeam = 0xFF;
		m_nRelicsUserIndex = -1;
		m_btInventoryPos = 0xFF;
		IncRelicsMoveCount(lpObj->m_Index);

		LogAddTD("[Illusion Temple] (%d) Increase AlliedTeam Relics Point: (%s)(%s)(Score:%d)", m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, m_btAlliedPoint);
	}

	SetStatusRegenTime();
}

void CIllusionTempleProcess::HolyRelicsBoxIllusionAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(m_btRelicsTeam == 0xFF || m_btRelicsTeam != 1 || lpObj->m_Index != m_nRelicsUserIndex)
	{
		return;
	}

	if(lpObj->pInventory[m_btInventoryPos].m_Type == (ITEM_POTION+64))
	{
		m_btIllusionPoint++;
		DeleteRelicsItem(lpObj);
		m_btRelicsTeam = 0xFF;
		m_nRelicsUserIndex = -1;
		m_btInventoryPos = 0xFF;
		IncRelicsMoveCount(lpObj->m_Index);

		LogAddTD("[Illusion Temple] (%d) Increase IllusionTeam Relics Point: (%s)(%s)(Score:%d)", m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, m_btIllusionPoint);
	}

	SetStatusRegenTime();
}

int CIllusionTempleProcess::SearchRelicsItem(int iIndex)
{
	if( !CHECK_LIMIT(iIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	for( int x = 0; x < INVENTORY_SIZE; x++ )
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == (ITEM_POTION+64) ) //Ball
			{
				return x;
			}
		}
	}
	return -1;
}

void CIllusionTempleProcess::DeleteRelicsItem(LPOBJECTSTRUCT lpObj)
{
	if(lpObj->pInventory[m_btInventoryPos].m_Type == (ITEM_POTION+64))
	{
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) to Delete Relics Item [Serial:%u]", m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, lpObj->pInventory[m_btInventoryPos].m_Number);
		gObjInventoryDeleteItem(lpObj->m_Index, m_btInventoryPos);
		GCInventoryItemDeleteSend(lpObj->m_Index, m_btInventoryPos, 0);
	}
}

void CIllusionTempleProcess::DropRelicsItem(int iIndex)
{
	if( !CHECK_LIMIT(iIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(m_nRelicsUserIndex != iIndex)
	{
		return;
	}

	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[iIndex];

	if(m_btInventoryPos == -1)
	{
		return;
	}

	if(lpObj->pInventory[m_btInventoryPos].m_Type != (ITEM_POTION+64))
	{
		return;
	}

	int iItemPos = m_btInventoryPos;

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = TRUE;
	pResult.Ipos = iItemPos;

	int map_num = gObj[iIndex].MapNumber;
	int type = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur = gObj[iIndex].pInventory[iItemPos].m_Durability;
	BOOL ret = gObj[iIndex].pInventory[iItemPos].IsItem();
	BYTE Option1 = gObj[iIndex].pInventory[iItemPos].m_Option1;
	BYTE Option2 = gObj[iIndex].pInventory[iItemPos].m_Option2;
	BYTE Option3 = gObj[iIndex].pInventory[iItemPos].m_Option3;
	BYTE NOption = gObj[iIndex].pInventory[iItemPos].m_NewOption;
	DWORD s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	BYTE ItemExOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[iIndex].pInventory[iItemPos]);
	BYTE ItemExLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[iIndex].pInventory[iItemPos]); //loc17

	BYTE NewOption[8];

	::ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);

	int PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;

	int PetExp = gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
	BYTE SOption = gObj[iIndex].pInventory[iItemPos].m_SetOption;

	BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	int item_number = gObj[iIndex].pInventory[iItemPos].m_Number;

	char szItemName[50] = "저주받은성물";

	int aAntiLootIndex = -1;
	short cX = 0;
	short cY = 0;

	BYTE btMapAttr = MapC[map_num].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);

		if((btMapAttr&4) == 4 || (btMapAttr&8) == 8)
	{
		cX = 168;
		cY = 85;
	}
	else
	{
		cX = gObj[iIndex].X;
		cY = gObj[iIndex].Y;
	}

	if( MapC[map_num].ItemDrop(type, level, dur, cX, cY,Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, NULL, 0xFF) == TRUE )
	{
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) to Drop Relics Item [Serial:%u]",m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, lpObj->pInventory[m_btInventoryPos].m_Number);
		
		::gObjInventoryDeleteItem(iIndex, iItemPos);
		pResult.Result = TRUE;

		LogAddTD(lMsg.Get(479), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber,
			gObj[iIndex].X, gObj[iIndex].Y, s_num, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0],
			NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption,
			ItemEffectEx>>7, ItemExOption, ItemExLevel);
	}
	else
	{
		pResult.Result = FALSE;
	}

	m_nRelicsUserIndex = -1;
	m_btInventoryPos = 0xFF;
	m_btRelicsTeam = 0xFF;

	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
}

void CIllusionTempleProcess::SetInventoryPos(int aIndex, BYTE btPos)
{
	if( !CHECK_LIMIT(aIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	m_nRelicsUserIndex = aIndex;
	m_btInventoryPos = btPos;

	if(gObj[aIndex].m_iIllusionTempleIndex != -1 && m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_nIndex != -1 && m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_nIndex == aIndex)
	{
		m_btRelicsTeam = m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_btTeam;
	}
}

void CIllusionTempleProcess::SendRelicsUser(LPOBJECTSTRUCT lpObj) //Seongmul Effect
{
	PMSG_RELICS_GET_USER pRelicsUser;
	PHeadSubSetB((LPBYTE)&pRelicsUser, 0xBF, 0x08, sizeof(pRelicsUser));

	pRelicsUser.wUserIndex = lpObj->m_Index;

	memcpy(pRelicsUser.Name, lpObj->Name, MAX_IDSTRING);

	for( int i = 0; i < 10; i++ )
	{
		if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			DataSend(m_UserData[i].m_nIndex,(LPBYTE)&pRelicsUser, pRelicsUser.h.size);
		}
	}
}

void CIllusionTempleProcess::SetRemainTime()
{
	m_nRemainTimeMin = 0;
	m_nRemainTimeMSec = 1000;
}

void CIllusionTempleProcess::AddNpcPosNum(int nClass, int nPosNum)
{
	if( nClass == 380 )
	{
		if( m_nStatusCount > 2 )	return;
			
		m_StatusData[m_nStatusCount].m_nIndex = -1;
		m_StatusData[m_nStatusCount].m_wPosNum = nPosNum;
		m_nStatusCount++;
	}
	else if( nClass == 383 )
	{
		m_RelicsBoxAlliedData.m_nIndex = -1;
		m_RelicsBoxAlliedData.m_wPosNum = nPosNum;
	}
	else if( nClass == 384 )
	{
		m_RelicsBoxIllusionData.m_nIndex = -1;
		m_RelicsBoxIllusionData.m_wPosNum = nPosNum;
	}
}

void CIllusionTempleProcess::AddMonsterPosNum(int nClass, int nPosNum)
{
	if( m_nMonsterCount > 40 )
	{
		return;
	}

	m_MonsterData[m_nMonsterCount].m_nIndex = -1;
	m_MonsterData[m_nMonsterCount].m_wPosNum = nPosNum;
	m_nMonsterCount++;
}

void CIllusionTempleProcess::ResetPosNum()
{
	int i;

	for(i = 0; i < 2; i++)
	{
		m_StatusData[i].m_nIndex = -1;
		m_StatusData[i].m_wPosNum = -1;
	}

	m_nStatusCount = 0;
	m_RelicsBoxAlliedData.m_nIndex = -1;
	m_RelicsBoxAlliedData.m_wPosNum = -1;
	m_RelicsBoxIllusionData.m_nIndex = -1;
	m_RelicsBoxIllusionData.m_wPosNum = -1;

	for(i = 0; i < 40; i++)
	{
		m_MonsterData[i].m_nIndex = -1;
		m_MonsterData[i].m_wPosNum = -1;
	}

	m_nMonsterCount = 0;
}

void CIllusionTempleProcess::AddNpcStatus()
{
	for( int i = 0; i < m_nStatusCount; i++)
	{
		int result = gObjAddMonster(m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1);
		if(result >= 0)
		{
			gObjSetPosMonster(result, m_StatusData[i].m_wPosNum);
			gObjSetMonster(result, 380);
			gObj[result].m_PosNum = -1;
			gObj[result].Live = 0;
			gObj[result].DieRegen = 2;
			gObj[result].m_State = 0x0004;
			gObj[result].RegenTime = GetTickCount();
			m_StatusData[i].m_nIndex = result;

			LogAddTD("[Illusion Temple] (%d) Create NPC Status Name: %s, (%d: %d/%d)", m_nTempleNumber+1, gObj[result].Name, gObj[result].MapNumber, gObj[result].X, gObj[result].Y);
		}
		else LogAddTD("[Illusion Temple] (%d) Add Status Error", m_nTempleNumber+1);
	}
}

void CIllusionTempleProcess::AddNpcRelicsBox()
{
	int result;

	result = gObjAddMonster(m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1);
	if(result >= 0)
	{
		gObjSetPosMonster(result, m_RelicsBoxAlliedData.m_wPosNum);
		gObjSetMonster(result, 383);
		gObj[result].Dir = 1;
		gObj[result].m_PosNum = -1;
		gObj[result].Live = 1;
		gObj[result].DieRegen = 0;
		gObj[result].m_State = 0x0001;
		m_RelicsBoxAlliedData.m_nIndex = result;

		LogAddTD("[Illusion Temple] (%d) Create NPC AlliedRelicsBox Name: %s, (%d: %d/%d)", m_nTempleNumber+1, gObj[result].Name, gObj[result].MapNumber, gObj[result].X, gObj[result].Y);
	}
	else LogAddTD("[Illusion Temple] (%d) Add AlliedRelicsBox Error", m_nTempleNumber+1);

	result = gObjAddMonster(m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1);

	if(result >= 0)
	{
		gObjSetPosMonster(result, m_RelicsBoxIllusionData.m_wPosNum);
		gObjSetMonster(result, 384);
		gObj[result].Dir = 1;
		gObj[result].m_PosNum = -1;
		gObj[result].Live = 1;
		gObj[result].DieRegen = 0;
		gObj[result].m_State = 0x0001;
		m_RelicsBoxIllusionData.m_nIndex = result;

		LogAddTD("[Illusion Temple] (%d) Create NPC IllusionRelicsBox Name: %s, (%d: %d/%d)", m_nTempleNumber+1, gObj[result].Name, gObj[result].MapNumber, gObj[result].X, gObj[result].Y);
	}
	else LogAddTD("[Illusion Temple] (%d) Add IllusionRelicsBox Error", m_nTempleNumber+1);
}

void CIllusionTempleProcess::AddMonsterGhost()
{
	for(int i = 0; i < m_nMonsterCount; i++)
	{
		int result = gObjAddMonster(m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1);
		if(result >= 0)
		{
			gObjSetPosMonster(result, m_MonsterData[i].m_wPosNum);
			gObjSetMonster(result, gMSetBase.m_Mp[m_MonsterData[i].m_wPosNum].m_Type);
			gObj[result].Dir = 1;
			gObj[result].m_PosNum = -1;
			gObj[result].Live = 0;
			gObj[result].DieRegen = 0;
			gObj[result].m_State = 0x0004;
			gObj[result].RegenTime = GetTickCount();
			m_MonsterData[i].m_nIndex = result;
		}
		else LogAddTD("[Illusion Temple] (%d) Add Monster Error", m_nTempleNumber+1);
	}
}

void CIllusionTempleProcess::RemoveAllObj()
{
	int i;

	for(i = 0; i < m_nStatusCount; i++)
	{
		if(m_StatusData[i].m_nIndex != -1)
		{
			LogAddTD("[Illusion Temple] (%d) RemoveStatus Name: %s, (%d: %d/%d)",m_nTempleNumber+1, gObj[m_StatusData[i].m_nIndex].Name, gObj[m_StatusData[i].m_nIndex].MapNumber, gObj[m_StatusData[i].m_nIndex].X, gObj[m_StatusData[i].m_nIndex].Y);
			gObjDel(m_StatusData[i].m_nIndex);
			m_StatusData[i].m_nIndex = -1;
		}
	}

	if(m_RelicsBoxAlliedData.m_nIndex != -1)
	{
		LogAddTD("[Illusion Temple] (%d) RemoveAlliedRelicsBox Name: %s, (%d: %d/%d)",m_nTempleNumber+1, gObj[m_RelicsBoxAlliedData.m_nIndex].Name, gObj[m_RelicsBoxAlliedData.m_nIndex].MapNumber, gObj[m_RelicsBoxAlliedData.m_nIndex].X, gObj[m_RelicsBoxAlliedData.m_nIndex].Y);
		gObjDel(m_RelicsBoxAlliedData.m_nIndex);
		m_RelicsBoxAlliedData.m_nIndex = -1;
	}

	if(m_RelicsBoxIllusionData.m_nIndex != -1)
	{
		LogAddTD("[Illusion Temple] (%d) RemoveIllusionRelicsBox Name: %s, (%d: %d/%d)",m_nTempleNumber+1, gObj[m_RelicsBoxIllusionData.m_nIndex].Name, gObj[m_RelicsBoxIllusionData.m_nIndex].MapNumber, gObj[m_RelicsBoxIllusionData.m_nIndex].X, gObj[m_RelicsBoxIllusionData.m_nIndex].Y);
		gObjDel(m_RelicsBoxIllusionData.m_nIndex);
		m_RelicsBoxIllusionData.m_nIndex = -1;
	}

	for(i = 0; i < m_nMonsterCount; i++)
	{
		if(m_MonsterData[i].m_nIndex != -1)
		{
			gObjDel(m_MonsterData[i].m_nIndex);
			m_MonsterData[i].m_nIndex = -1;
		}
	}

	LogAddTD("[Illusion Temple] (%d) Remove All Object", m_nTempleNumber+1);
}

void CIllusionTempleProcess::SetStatusRegenTime()
{
	m_StatusRegenTime = GetTickCount() + 1000*5;
}

void CIllusionTempleProcess::RegenStatus()
{
	if(m_nRelicsUserIndex >= 0)
	{
		return;
	}

	int nPos = rand()%m_nStatusCount;

	if(m_StatusData[nPos].m_nIndex == -1)
	{
		return;
	}

	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[m_StatusData[nPos].m_nIndex];

	lpObj->m_PosNum = m_StatusData[nPos].m_wPosNum;
	lpObj->DieRegen = 2;
	m_StatusRegenTime = -1;

	LogAddTD("[Illusion Temple] (%d) RegenStatus Name: %s, (%d: %d/%d), PosNum(%d), connected:%d", m_nTempleNumber+1, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, lpObj->m_PosNum, lpObj->Connected);
}

void CIllusionTempleProcess::RegenMonster()
{
	for(int i = 0; i < 40; i++)
	{
		if(m_MonsterData[i].m_nIndex != -1 && gObj[m_MonsterData[i].m_nIndex].m_PosNum == -1)
		{
			gObj[m_MonsterData[i].m_nIndex].m_PosNum = m_MonsterData[i].m_wPosNum; 
			gObj[m_MonsterData[i].m_nIndex].DieRegen = 2;
		}
	}
}

void CIllusionTempleProcess::SendNoticeMessage(char* lpMsg)
{
	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, lpMsg);

	for(int i = 0; i < 10; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			DataSend(m_UserData[i].m_nIndex,(LPBYTE)&pNotice,pNotice.h.size);
		}
	}
}

void CIllusionTempleProcess::SendIllusionTemplePlayState(BYTE btTeam)
{
	int	nOffSet = 0;

	BYTE SendByte[256];

	PMSG_ILLUSION_TEMPLE_PLAY_STATE	pMsg;
	
	if( m_nRemainTimeMSec < 0 )	m_nRemainTimeMSec = 0;

	pMsg.wRemainSec = m_nRemainTimeMSec / 1000;
		
	if( m_nRelicsUserIndex != -1 ) //Ball
	{
		pMsg.wUserIndex		= m_nRelicsUserIndex;
		pMsg.btX			= gObj[m_nRelicsUserIndex].X;
		pMsg.btY			= gObj[m_nRelicsUserIndex].Y;
	}
	else
	{
		pMsg.wUserIndex		= -1;
		pMsg.btX			= 0xFF;
		pMsg.btY			= 0xFF;
	}
	
	pMsg.btAlliedPoint		= m_btAlliedPoint;
	pMsg.btIllusionPoint	= m_btIllusionPoint;
	pMsg.btMyTeam			= btTeam;
	
	nOffSet += sizeof(pMsg);

	int tmpCount = 0;

	for(int i = 0; i < 10; i++ )
	{
		if( m_UserData[i].m_nIndex != -1 && m_UserData[i].m_btTeam == btTeam && gObj[m_UserData[i].m_nIndex].Connected > 2 )
		{
			PMSG_ILLUSION_TEMPLE_PARTY_POS pPartyPos;

			pPartyPos.wPartyUserIndex   = m_UserData[i].m_nIndex;
			pPartyPos.byMapNumber		= gObj[m_UserData[i].m_nIndex].MapNumber;
			pPartyPos.btX				= gObj[m_UserData[i].m_nIndex].X;
			pPartyPos.btY				= gObj[m_UserData[i].m_nIndex].Y;
			
			tmpCount++;
			
			memcpy((SendByte+nOffSet), &pPartyPos, sizeof(pPartyPos));
			
			nOffSet += sizeof(pPartyPos);
		}
	}
	
	if( btTeam == 0 )
	{
		if( m_nAlliedUserCount != tmpCount )
		{
			m_nAlliedUserCount = tmpCount;
		}

		pMsg.btPartyCount = m_nAlliedUserCount;
	}
	else 
	{
		if( m_nIllusionUserCount != tmpCount )
		{
			m_nIllusionUserCount = tmpCount;
		}

		pMsg.btPartyCount = m_nIllusionUserCount;
	}
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x01, nOffSet);
	
	memcpy(SendByte, &pMsg, sizeof(pMsg));
	
	for( int i = 0; i < 10; i++ )
	{
		if( m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 && m_UserData[i].m_btTeam == btTeam )		
		{
			DataSend(m_UserData[i].m_nIndex, (LPBYTE)SendByte, pMsg.h.size);
		}
	}
}

void CIllusionTempleProcess::SendIllusionTempleState(BYTE btTempleState, int nIndex)
{
	PMSG_ILLUSION_TEMPLE_STATE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));

	pMsg.btTempleNumber = m_nTempleNumber;
	pMsg.btIllusionTempleState = btTempleState;

	if(nIndex != -1)
	{
		DataSend(nIndex,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	for(int i = 0; i < 10; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			DataSend(m_UserData[i].m_nIndex,(LPBYTE)&pMsg,pMsg.h.size);
		}
	}
}

BYTE CIllusionTempleProcess::GetUserTeam(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btTeam;
		}
	}

	return 0xFF;
}

BYTE CIllusionTempleProcess::IncSkillPoint(int nIndex, BYTE btIncPoint)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return 0;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint += btIncPoint;

			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint > 90)
			{
				m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint = 90;
			}

			return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint;
		}
	}

	return 0;
}

BYTE CIllusionTempleProcess::DecSkillPoint(int nIndex, BYTE btDecPoint)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return 0;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint >= btDecPoint)
			{
				m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint -= btDecPoint;
				return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint;
			}
		}
	}

	return 0;
}

BYTE CIllusionTempleProcess::GetSkillPoint(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return 0;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint;
		}
	}

	return 0;
}

BOOL CIllusionTempleProcess::SetSkillPoint(int nIndex, BYTE btSkillPoint)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint = btSkillPoint;
			return TRUE;
		}
	}

	return FALSE;
}

void CIllusionTempleProcess::ResetSkillPoint()
{
	for(int i = 0; i < 10; i++)
	{
		m_UserData[i].m_btSkillPoint = 10;
	}
}

void CIllusionTempleProcess::IncUserKillCount(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUserKillCount > 0xFF)
			{
				return;
			}

			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUserKillCount++;
		}
	}
}

void CIllusionTempleProcess::IncMonsterKillCount(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btMonsterKillCount > 0xFF)
			{
				return;
			}

			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btMonsterKillCount++;
		}
	}
}

void CIllusionTempleProcess::ResetKillCount()
{
	for(int i = 0; i < 10; i++)
	{
		m_UserData[i].m_btUserKillCount = 0;
		m_UserData[i].m_btMonsterKillCount = 0;
	}
}

void CIllusionTempleProcess::IncRelicsMoveCount(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btRelicsMoveCount > 0xFF)
			{
				return;
			}

			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btRelicsMoveCount++;
		}
	}
}

void CIllusionTempleProcess::ResetRelicsMoveCount()
{
	for(int i = 0; i < 10; i++)
	{
		m_UserData[i].m_btRelicsMoveCount = 0;
	}
}

void CIllusionTempleProcess::IncUseSkillCount(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex || gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUseSkillCount > 0xFF)
			{
				return;
			}

			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUseSkillCount++;
		}
	}
}

void CIllusionTempleProcess::ResetUseSkillCount()
{
	for(int i = 0; i < 10; i++)
	{
		m_UserData[i].m_btUseSkillCount = 0;
	}
}

void CIllusionTempleProcess::IllusionTempleUseSkill(int nIndex, WORD btSkillNumber, int nTargetIndex, BYTE btDis)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if( !CHECK_LIMIT(nTargetIndex, OBJMAX) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[nIndex];
	LPOBJECTSTRUCT lpTargetObj = (LPOBJECTSTRUCT)&gObj[nTargetIndex];

	if(GetIllusionTempleState() != 2)
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);
		return;
	}

	if(lpObj->Connected <= 2 || lpTargetObj->Connected <= 2)
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);
		return;
	}

	if(lpObj->MapNumber != lpTargetObj->MapNumber)
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);
		return;
	}

	if(lpObj->m_iIllusionTempleIndex == -1 || lpTargetObj->m_iIllusionTempleIndex == -1)
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);
		return;
	}

	int nResultCount = -1;
	BOOL ret = TRUE;

	switch(btSkillNumber)
	{
		case 210:
			{
				nResultCount = MagicDamageC.CheckKillCount(210, GetSkillPoint(lpObj->m_Index) );
				
				if(nResultCount < 0)
				{
					ret = 0;
					LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
					break;
				}
				ret = UseSkillProdection(lpObj);
			}
			break;
		case 211:
			{
				nResultCount = MagicDamageC.CheckKillCount(211, GetSkillPoint(lpObj->m_Index) );
				
				if(nResultCount < 0)
				{
					ret = 0;
					LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
					break;
				}
				ret = UseSkillRestraint(lpObj, lpTargetObj);
			}
			break;
		case 212:
			{
				nResultCount = MagicDamageC.CheckKillCount(212, GetSkillPoint(lpObj->m_Index) );
				
				if(nResultCount < 0)
				{
					ret = 0;
					LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
					break;
				}
				ret = UseSkillTeleport(lpObj);
			}
			break;
		case 213:
			{
				nResultCount = MagicDamageC.CheckKillCount(213, GetSkillPoint(lpObj->m_Index) );
				
				if(nResultCount < 0)
				{
					ret = 0;
					LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
					break;
				}
				ret = UseSkillShieldBurn(lpObj, lpTargetObj);
			}
			break;
		default:
			ret = 0;
			break;
	}

	if(ret == TRUE)
	{
		int result = SetSkillPoint(nIndex, nResultCount);

		GCSendIllusionTempleKillCount(nIndex, nResultCount);

		IncUseSkillCount(nIndex);
		
		LogAddTD("[Illusion Temple] (%d) Use Skill (%d), (%s)(%s), (%d)", m_nTempleNumber+1, btSkillNumber, lpObj->AccountID, lpObj->Name, nResultCount);
	}

	if(btSkillNumber != 212)
	{
		if(ret == TRUE)
		{
			SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 1);
			return;
		}
	}

	if( !ret )
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);
	}
}

void CIllusionTempleProcess::IllusionTempleSkillProc(LPOBJECTSTRUCT lpObj)
{
	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_nIndex != lpObj->m_Index)
	{
		return;
	}

	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime--;

		if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime <= 0)
		{
			m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime = 0;
			gObjRemoveBuffEffect(lpObj, AT_ILLUSION_SHIELD_SPELL);
			SendIllusionTempleSkillEnd(lpObj, 210);
		}
	}
	
	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime--;

		if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime <= 0)
		{
			m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime = 0;
			gObjRemoveBuffEffect(lpObj, AT_ILLUSION_RESTRICTION_SPELL);
			SendIllusionTempleSkillEnd(lpObj, 211);
		}
	}
}

void CIllusionTempleProcess::SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, WORD btState)
{
	PMSG_ILLUSION_TEMPLE_SKILL_END pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x07, sizeof(pMsg));

	pMsg.MagicNumberH = HIBYTE(btState);
	pMsg.MagicNumberL = LOBYTE(btState);

	pMsg.wObjIndex = lpObj->m_Index;

	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

BOOL CIllusionTempleProcess::UseSkillProdection(LPOBJECTSTRUCT lpObj)
{
	m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime = 15;
	gObjAddBuffEffect(lpObj, AT_ILLUSION_SHIELD_SPELL, 0, 0, 0, 0, -10);
	return TRUE;
}

BOOL CIllusionTempleProcess::UseSkillRestraint(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	if(!gCheckSkillDistance(lpObj->m_Index, lpTargetObj->m_Index, 211))
	{
		return FALSE;
	}

	m_UserData[lpTargetObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime = 15;

	lpTargetObj->lpAttackObj = lpObj;
	gObjAddBuffEffect(lpTargetObj, AT_ILLUSION_RESTRICTION_SPELL, 0, 0, 0, 0, -10);
	lpTargetObj->PathCount = 0;
	lpTargetObj->PathStartEnd = 0;

	gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	return TRUE;
}

BOOL CIllusionTempleProcess::UseSkillTeleport(LPOBJECTSTRUCT lpObj)
{
	if(gObjCheckUsedBuffEffect(lpObj, AT_STUN) == true)
	{
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);
		return FALSE;
	}

	if(gObjCheckUsedBuffEffect(lpObj, AT_SLEEP) == true)
	{
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);
		return FALSE;
	}

	int tmpRelicsUserIndex = m_nRelicsUserIndex;

	if(tmpRelicsUserIndex == -1)
	{
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);
		return FALSE;
	}

	if(tmpRelicsUserIndex == lpObj->m_Index)
	{
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 0);
		return FALSE;
	}

	SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, 212, 1);

	int addx = 0;
	int addy = 0;
	int count = 50; //ebp14
	BOOL Find = FALSE;

	while(count-- != 0)
	{
		addx = gObj[tmpRelicsUserIndex].X + rand()%3 - 1;
		addy = gObj[tmpRelicsUserIndex].Y + rand()%3 - 1;

		if(gObjCheckTeleportArea(lpObj->m_Index,addx,addy))
		{
			Find = TRUE;
			break;
		}
	}

	if(Find == FALSE)
	{
		addx = gObj[tmpRelicsUserIndex].X;
		addy = gObj[tmpRelicsUserIndex].Y;
	}

	gObjTeleportMagicUse(lpObj->m_Index,addx,addy);
	return TRUE;
}

BOOL CIllusionTempleProcess::UseSkillShieldBurn(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	if(gCheckSkillDistance(lpObj->m_Index, lpTargetObj->m_Index, 213) == FALSE)
	{
		return FALSE;
	}

	lpTargetObj->lpAttackObj = lpObj;

	if(lpTargetObj->iShield <= 1)
	{
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
		return TRUE;
	}

	int nShieldDamage = lpTargetObj->iShield / 2;

	lpTargetObj->iShield = nShieldDamage;

	GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, nShieldDamage);
	return TRUE;
}

void CIllusionTempleProcess::SendUseSkillResult(int nIndex, int nTargetIndex, WORD wSkillNumber, BYTE btResult)
{
	PMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x02, sizeof(pResult));

	pResult.btResult = btResult;
	pResult.MagicNumberH = HIBYTE(wSkillNumber);
	pResult.MagicNumberL = LOBYTE(wSkillNumber);
	pResult.wSourceObjIndex = nIndex;
	pResult.wTargetObjIndex = nTargetIndex;

	DataSend(nIndex,(LPBYTE)&pResult,pResult.h.size);

	if(btResult == 1)
	{
		MsgSendV2(&gObj[nIndex], (LPBYTE)&pResult, pResult.h.size);
	}
}

void CIllusionTempleProcess::IllusionTempleUserDie(LPOBJECTSTRUCT lpObj)
{
	if(lpObj->m_iIllusionTempleIndex == -1 || m_UserData[lpObj->m_iIllusionTempleIndex].m_nIndex != lpObj->m_Index)
	{
		return;
	}

	DropRelicsItem(lpObj->m_Index);

	m_UserData[lpObj->m_iIllusionTempleIndex].m_btSkillPoint = 10;

	GCSendIllusionTempleKillCount(lpObj->m_Index, 10);

	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime = 0;
		gObjRemoveBuffEffect(lpObj, AT_ILLUSION_SHIELD_SPELL);
		SendIllusionTempleSkillEnd(lpObj, 210);
	}

	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime = 0;
		gObjRemoveBuffEffect(lpObj, AT_ILLUSION_RESTRICTION_SPELL);
		SendIllusionTempleSkillEnd(lpObj, 211);
	}
}

void CIllusionTempleProcess::CalCursetTempleResult()
{
	CheckUsersOnConnect();

	int nAlliedExp = 0;
	int nIllusionExp = 0;

	if(m_btAlliedPoint >= 2 && m_btAlliedPoint > m_btIllusionPoint)
	{
		m_btWinTeam = 0;

		nAlliedExp = (g_IllusionTemple_Default_Exp[m_nTempleNumber]+((m_btAlliedPoint-m_btIllusionPoint)*g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber])) * 2;

		if(m_btIllusionPoint == 0){
			nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber]+(m_btIllusionPoint*g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber]);
	}
	else if(m_btIllusionPoint >= 2 && m_btAlliedPoint < m_btIllusionPoint)
	{
		m_btWinTeam = 1;

		nIllusionExp = (g_IllusionTemple_Default_Exp[m_nTempleNumber]+((m_btIllusionPoint-m_btAlliedPoint)*g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber])) * 2;

		if(m_btIllusionPoint == 0){
			nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber]+(m_btAlliedPoint*g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber]);
	}
	else
	{
		if(m_btAlliedPoint == 0){
			nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber]+(m_btAlliedPoint*g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber]);

		if(m_btIllusionPoint == 0){
			nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber]+(m_btIllusionPoint*g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber]);
	}

	LogAddTD("[Illusion Temple] (%d) Result (WinTeam: %d)(Score:[%d][%d])", m_nTempleNumber+1, m_btWinTeam, m_btAlliedPoint, m_btIllusionPoint);

	int i;
	for(i = 0; i < 10; i++)
	{
		if(m_UserData[i].m_btTeam == 0xFF)	continue;

		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 && gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1 && m_UserData[i].m_btUseSkillCount >= g_btIllusionTempleMinUseSkillCnt)
		{
			switch(m_UserData[i].m_btTeam)
			{
				case 0:	m_UserData[i].m_nRewardExp = nAlliedExp;	break;
				case 1:	m_UserData[i].m_nRewardExp = nIllusionExp;	break;
			}

			if( gObj[m_UserData[i].m_nIndex].ThirdChangeUp ){
				m_UserData[i].m_nRewardExp = m_UserData[i].m_nRewardExp*g_iIllusionTempleRewardPercentMaster/100;
			}
			else m_UserData[i].m_nRewardExp = m_UserData[i].m_nRewardExp*g_iIllusionTempleRewardPercent/100;

			CheckItemOptForGetExpEx(&gObj[m_UserData[i].m_nIndex], m_UserData[i].m_nRewardExp, TRUE);

			int nPartyCount = 0;

			if( gObj[m_UserData[i].m_nIndex].PartyNumber >= 0 ){
				nPartyCount = gParty.GetCount(gObj[m_UserData[i].m_nIndex].PartyNumber);
			}
			else nPartyCount = 1;

			m_UserData[i].m_nRankingPoint = ( m_UserData[i].m_nRewardExp + ( m_UserData[i].m_btUserKillCount * g_IllusionTemple_Add_Kill_Exp[m_nTempleNumber] ) + ( m_UserData[i].m_btRelicsMoveCount * g_IllusionTemple_Move_Relics_Exp[m_nTempleNumber] ) + g_IllusionTemple_PartyUser_Point[nPartyCount] );

			int iCAL_EXP = m_UserData[i].m_nRewardExp;
			int iRET_EXP = 0;

			while(iCAL_EXP > 0)
			{
				iRET_EXP = iCAL_EXP;
				iCAL_EXP = LevelUp(m_UserData[i].m_nIndex, iCAL_EXP);
			}

			int iIndex = m_UserData[i].m_nIndex;

			if( !g_MasterLevelSystem.IsMasterLevelUser(&gObj[iIndex]) )
			{
				GCSendExp_INT64(iIndex, (WORD)-1, iRET_EXP, 0, 0);
			}
		}
	}

	if(g_bSaveIllusionTempleRankingPoint != FALSE)
	{
		for(i = 0; i < 10; i++)
		{
			if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
			{
				if(m_UserData[i].m_btTeam == 0xFF) continue;

				PMSG_ANS_ILLUSION_TEMPLE_SCORE pMsgScore = {0};

				pMsgScore.h.c = PMHC_BYTE;
				pMsgScore.h.headcode = 0x26;
				pMsgScore.h.size = sizeof(pMsgScore);

				pMsgScore.nINDEX = m_UserData[i].m_nIndex;

				memcpy(pMsgScore.szAccountID, gObj[m_UserData[i].m_nIndex].AccountID, MAX_IDSTRING);
				pMsgScore.szAccountID[MAX_IDSTRING] = '\0';

				memcpy(pMsgScore.szGameID, gObj[m_UserData[i].m_nIndex].Name, MAX_IDSTRING);
				pMsgScore.szGameID[MAX_IDSTRING] = '\0';

				pMsgScore.wServerCode = gGameServerCode/20;

				pMsgScore.nTempleNumber = m_nTempleNumber+1;
				pMsgScore.nClass = gObj[m_UserData[i].m_nIndex].DbClass;
				pMsgScore.nLevel = gObj[m_UserData[i].m_nIndex].Level;

				if(m_btWinTeam == 0xFF){
					pMsgScore.btWin = 0;
				}
				else pMsgScore.btWin = (m_UserData[i].m_btTeam == m_btWinTeam);

				pMsgScore.nRankPoint = m_UserData[i].m_nRankingPoint;
#ifdef OPT
				pMsgScore.nPCBangGuid = gObj[m_UserData[i].m_nIndex].m_iPcBangRoom;
#endif
				DataSendEventChip((char*)&pMsgScore,sizeof(pMsgScore));
			}
		}
	}
}

void CIllusionTempleProcess::DropChaosGem(int nIndex)
{
	if( !CHECK_LIMIT(nIndex, OBJMAX) ) return;
	
	if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex != gObj[nIndex].m_Index) return;
	
	if(GetIllusionTempleState() != 3) return;
	
	if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btTeam == 0xFF) return;
	
	if(m_btWinTeam == 0xFF) return;
	
	if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btTeam != m_btWinTeam) return;
	
	if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_bReward == TRUE) return;
	
#if defined __REEDLAN__ || __BEREZNUK__
	g_ShopPointEx.AddEventBonus(nIndex, ShopPointExEvent::IT);
#endif

	m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_bReward = TRUE;

#ifdef __CUSTOMS__

	BYTE CastleLevel = gObj[nIndex].MapNumber - 45;

	for( int i = 0; i < g_IllusionTempleEvent.m_ItemReward[CastleLevel].ItemCount; i++ )
	{
		ItemSerialCreateSend(gObj[nIndex].m_Index, gObj[nIndex].MapNumber, gObj[nIndex].X, gObj[nIndex].Y, g_IllusionTempleEvent.m_ItemReward[CastleLevel].ItemID, g_IllusionTempleEvent.m_ItemReward[CastleLevel].ItemLevel, 0, 0, 0, 0, nIndex, 0, 0);
	}

#else
	int iMaxHitUser = nIndex;
	int iType = ItemGetNumberMake(12, 15);
	
	ItemSerialCreateSend(gObj[nIndex].m_Index, gObj[nIndex].MapNumber, gObj[nIndex].X, gObj[nIndex].Y, iType, 0, 0, 0, 0, 0, iMaxHitUser, 0, 0);
#endif
}

int CIllusionTempleProcess::SearchScollOfBloodItem(int iIndex, float ScrollDur)
{
	if(!CHECK_LIMIT(iIndex, OBJMAX))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	int iFreeTicketIndex = -1;

	for( int x = 0; x < INVENTORY_SIZE; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if(gObj[iIndex].pInventory[x].m_Type == (ITEM_HELPER+51) && gObj[iIndex].pInventory[x].m_Level == m_nTempleNumber+1)
			{
				if(gObj[iIndex].pInventory[x].m_Durability == ScrollDur)
				{
					return x;
				}
			}

			if(gObj[iIndex].pInventory[x].m_Type == (ITEM_HELPER+61))
			{
				if(gObj[iIndex].pInventory[x].m_Durability > 0.0f)
				{
					iFreeTicketIndex = x;
				}
			}
		}
	}

	if(iFreeTicketIndex >= 0)
	{
		return iFreeTicketIndex;
	}
	return -1;
}

int CIllusionTempleProcess::SearchTicketItem(int iIndex)
{
	if(!CHECK_LIMIT(iIndex, OBJMAX))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	int iFreeTicketIndex = -1;

	for( int x = 0; x < INVENTORY_SIZE; x++ )
	{
		CItem * lpItem = &gObj[iIndex].pInventory[x];

		if( lpItem->IsItem() == TRUE )
		{
			if(lpItem->m_Type == (ITEM_HELPER+51) && lpItem->m_Level == m_nTempleNumber+1)
			{
				if(lpItem->m_Durability > 0.0f)
				{
					iFreeTicketIndex = x;
					break;
				}

				gObjInventoryDeleteItem(iIndex, x);
				GCInventoryItemDeleteSend(iIndex, x, 0);
			}

			if(lpItem->m_Type == (ITEM_HELPER+61))
			{
				if(lpItem->m_Durability > 0.0f)
				{
					if(lpItem->m_Durability > 0.0f)
					{
						iFreeTicketIndex = x;
						break;
					}

					gObjInventoryDeleteItem(iIndex, x);
					GCInventoryItemDeleteSend(iIndex, x, 0);
				}
			}
		}
	}
	return iFreeTicketIndex;
}

void CIllusionTempleProcess::MonsterDieItemDrop(LPOBJECTSTRUCT lpObj)
{
	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_nIndex != lpObj->m_Index) return;

	switch(lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1)
	{
		case 0:
		case 1:	IllusionTemple1ItemBag->DropIllusionTempleGhostItem(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);	break;
		case 2:
		case 3:	IllusionTemple2ItemBag->DropIllusionTempleGhostItem(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);	break;
		case 4:
		case 5:	IllusionTemple3ItemBag->DropIllusionTempleGhostItem(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);	break;
	}
}

void CIllusionTempleProcess::SendAllUserAnyMsg(BYTE *lpMsg, int iSize)
{
	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if(gObj[i].Connected == 3 && gObj[i].Type == OBJ_USER)
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}

int CIllusionTempleProcess::LevelUp(int iIndex, int iAddExp) 
{
	if(!CHECK_LIMIT(iIndex, OBJMAX))
	{
		return 0;
	}

	if( g_MasterLevelSystem.MasterLevelUp(&gObj[iIndex], iAddExp, true, 0) )
	{
		return 0;
	}

	gObjSetExpPetItem(iIndex, iAddExp);

	int iLEFT_EXP = 0;

	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d ", gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].X, gObj[iIndex].AccountID, gObj[iIndex].Name,	gObj[iIndex].Level, gObj[iIndex].Experience, iAddExp);

	if( gObj[iIndex].Level >= 400 )
	{
		GCServerMsgStringSend(lMsg.Get(1136), gObj[iIndex].m_Index, 1);
		return 0;
	}

	if( (gObj[iIndex].Experience + iAddExp) < gObj[iIndex].NextExp )
	{
		gObj[iIndex].Experience += iAddExp;
	}
	else
	{
		iLEFT_EXP = gObj[iIndex].Experience + iAddExp - gObj[iIndex].NextExp;
		gObj[iIndex].Experience = gObj[iIndex].NextExp;
		gObj[iIndex].Level++;

		if( gObj[iIndex].Class == CLASS_DARKLORD || 
			gObj[iIndex].Class == CLASS_MAGUMSA ){
			gObj[iIndex].LevelUpPoint += 7;
		}
		else gObj[iIndex].LevelUpPoint += 5;

		if( gObj[iIndex].PlusStatQuestClear && gObj[iIndex].Level >= g_ResetSystem.m_MarlonStatMinLevel )
		{
			gObj[iIndex].LevelUpPoint++;
			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].LevelUpPoint);
		}

		gObj[iIndex].MaxLife += DCInfo.DefClass[gObj[iIndex].Class].LevelLife;
		gObj[iIndex].MaxMana += DCInfo.DefClass[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;
		gObjNextExpCal(&gObj[iIndex]);
		gObjSetBP(gObj[iIndex].m_Index);
		GCLevelUpMsgSend(gObj[iIndex].m_Index, 1);
		gObjCalcMaxLifePower(gObj[iIndex].m_Index);

		LogAddTD(lMsg.Get(520), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level);

		//----------------------------------------------------------------------------------------------

		if( gObj[iIndex].Level == 400 && gObj[iIndex].PartyNumber >= 0 )
		{
			int partynum = gObj[iIndex].PartyNumber;

			char partyLog[256];
			sprintf(partyLog,"400 LevelUp (%s)(%s) Party ",gObj[iIndex].AccountID,gObj[iIndex].Name);

			int number;
			for(int n = 0; n<MAX_PARTYUSER; n++ )
			{
				number = gParty.m_PartyS[partynum].Number[n];
				if( number >= 0 )
				{
					int noffSet = strlen(partyLog);
					sprintf(&partyLog[noffSet],",(%s)(%s) ",gObj[number].AccountID,gObj[number].Name);
				}
			}
			LogAddTD(partyLog);
		}
	}
	GJSetCharacterInfo(&gObj[iIndex], gObj[iIndex].m_Index, FALSE, 0);
	return iLEFT_EXP;
}

BYTE CIllusionTempleProcess::GetRealRemainTimeMin()
{
	if(!GetIllusionTempleState())
	{
		return m_nRemainTimeMin;
	}
	return 0;
}

BOOL CIllusionTempleProcess::CheckSkillProdection(int iIllusionTempleIndex)
{
	if(!CHECK_LIMIT(iIllusionTempleIndex, 10))
	{
		return FALSE;
	}

	if(m_UserData[iIllusionTempleIndex].m_nIndex != -1)
	{
		if(m_UserData[iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CIllusionTempleProcess::CheckSkillRestraint(int iIllusionTempleIndex)
{
	if(CHECK_LIMIT( (iIllusionTempleIndex), 10) == FALSE)
	{
		return FALSE;
	}

	if(m_UserData[iIllusionTempleIndex].m_nIndex != -1)
	{
		if(m_UserData[iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CIllusionTempleProcess::SendIllusionTempleResult()
{
	int nOffSet = 0;

	BYTE SendByte[256];

	PMSG_ILLUSION_TEMPLE_RESULT pMsg;

	pMsg.btAlliedPoint = m_btAlliedPoint;
	pMsg.btIllusionPoint = m_btIllusionPoint;
	pMsg.btUserCount = m_nUserCount;

	nOffSet += sizeof(PMSG_ILLUSION_TEMPLE_RESULT);

	int i;
	for(i = 0; i < 10; i++)
	{
		PMSG_ILLUSION_TEMPLE_USER_ADD_EXP pAddExpMsg;

		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			memcpy(pAddExpMsg.GameID, gObj[m_UserData[i].m_nIndex].Name, MAX_IDSTRING);
			
			pAddExpMsg.btMapNumber = m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1;
			pAddExpMsg.btTeam = m_UserData[i].m_btTeam;
			pAddExpMsg.btClass = gObj[m_UserData[i].m_nIndex].Class;
			pAddExpMsg.nAddExp = m_UserData[i].m_nRewardExp;

			memcpy(&SendByte[nOffSet], &pAddExpMsg, sizeof(PMSG_ILLUSION_TEMPLE_USER_ADD_EXP));

			nOffSet += sizeof(PMSG_ILLUSION_TEMPLE_USER_ADD_EXP);

			LogAddTD("[Illusion Temple] (%d) RewardExp Result  (Account:%s, Name:%s, Team:%d, Class:%d, UserKillCount:%d, RelicsMoveCount:%d, RewardExp:%d, RankingPoint: %d)", 
				m_nTempleNumber+1, gObj[m_UserData[i].m_nIndex].AccountID, gObj[m_UserData[i].m_nIndex].Name, m_UserData[i].m_btTeam, gObj[m_UserData[i].m_nIndex].DbClass, 
				m_UserData[i].m_btUserKillCount, m_UserData[i].m_btRelicsMoveCount, m_UserData[i].m_nRewardExp, m_UserData[i].m_nRankingPoint);
		}
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x04, nOffSet);

	memcpy(SendByte,&pMsg,sizeof(pMsg));

	for(i = 0; i < 10; i++)
	{
		if(m_UserData[i].m_btTeam == 0xFF)
		{
			continue;
		}

		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2) //3
		{
			DataSend(m_UserData[i].m_nIndex,(LPBYTE)SendByte,pMsg.h.size);
#ifdef WZQUEST
			if( m_btWinTeam == m_UserData[i].m_btTeam )
            {
                g_QuestExpProgMng.ChkUserQuestTypeEventMap(259, &gObj[m_UserData[i].m_nIndex], this->m_nTempleNumber, 0);
            }
#endif
		}
	}
}