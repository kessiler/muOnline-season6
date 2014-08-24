// Crywolf.cpp: implementation of the CCrywolf class.
//	GS-N	1.00.77	JPN	0xXXXXXXXX - Completed
//	GS-CS	1.00.90	JPN	0xXXXXXXXX - Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CrywolfStatue.h"
#include "CrywolfUtil.h"
#include "CrywolfSync.h"
#include "TMonsterAIGroup.h"
#include "Gamemain.h"
#include "user.h"

#include "MapServerManager.h"
#include "DSprotocol.h"

#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
#include "LogProc.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSystem.h"

#ifdef __CUSTOMS__
#include "ShopPointEx.h"
#endif

static CCrywolfUtil UTIL;
CCrywolf g_Crywolf;

BOOL g_bDBCrywolfOccupied = FALSE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolf::CCrywolf()
{
	this->m_bFileDataLoad = FALSE;
	this->m_bDBDataLoadOK = FALSE;
	this->m_bDBDataLoading = FALSE;
	this->m_iCrywolfState = 0;
	this->m_iScheduleMode =0;
	this->m_dwCrywolfNotifyMsgStartTick = 0;
	this->m_dwCrywolfStartProcTick = 0;	
	this->m_bTurnUpBoss = FALSE;
	this->m_bChangeAI = FALSE ;	
	this->m_iBossIndex = -1;
	this->m_iBossGroupNumber = 0;
	memset(this->m_iMonsterGroupNumberArray, 0, MAX_CRYWOLF_MONSTER_GROUP);
}

CCrywolf::~CCrywolf()
{
	return;
}

BOOL CCrywolf::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, "") )
	{
		MsgBox("[Crywolf Main Job] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Crywolf Main Job] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		this->DelAllData();
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
					int iScheduleMode = 0;
					int iStateNumber = -1;
					int iMonth = -1;
					int iDay = -1;
					int iDayOfWeek = -1;
					int iHour = -1;
					int iMinute = -1;
					int iContinuanceTime = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iScheduleMode = TokenNumber;

					Token = GetToken();
					iStateNumber = TokenNumber;

					Token = GetToken();
					iMonth = TokenNumber;

					Token = GetToken();
					iDay = TokenNumber;

					Token = GetToken();
					iDayOfWeek = TokenNumber;

					Token = GetToken();
					iHour = TokenNumber;

					Token = GetToken();
					iMinute = TokenNumber;

					Token = GetToken();
					iContinuanceTime = TokenNumber;

					if ( this->m_StartTimeInfoCount <= -1 || this->m_StartTimeInfoCount >= MAX_CRYWOLF_STATE_TIME )
					{
						MsgBox("[Crywolf Main Job] - Excced MAX StartTime (%d)", this->m_StartTimeInfoCount);
					}

					this->m_iScheduleMode = iScheduleMode;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_iStateNumber = iStateNumber;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_iMonth = iMonth;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_iDay = iDay;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_iDayOfWeek = iDayOfWeek;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_iHour = iHour;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_iMinute = iMinute;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_iContinuanceTime = iContinuanceTime;
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_bUsed = TRUE;
					this->m_StartTimeInfoCount++;
				}

				if ( iType == 1 )
				{
					int iStateNumber = -1;
					int iHour = 0;
					int iMinute = 0;
					int iSecond = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iStateNumber = TokenNumber;
					
					Token = GetToken();
					iHour = TokenNumber;

					Token = GetToken();
					iMinute = TokenNumber;

					Token = GetToken();
					iSecond = TokenNumber;

					this->m_StateTimeInfo[iStateNumber].m_iStateNumber = iStateNumber;
					this->m_StateTimeInfo[iStateNumber].m_iHour = iHour;
					this->m_StateTimeInfo[iStateNumber].m_iMinute = iMinute;
					this->m_StateTimeInfo[iStateNumber].m_iSecond = iSecond;
					this->m_StateTimeInfo[iStateNumber].m_iContinuanceTime = iHour * 60 * 60 * 1000 + iMinute * 60 * 1000 + iSecond * 1000;
					this->m_StateTimeInfo[iStateNumber].m_bUsed = TRUE;
				}

				if ( iType == 2 )
				{
					int iGroupNumber = -1;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iGroupNumber = TokenNumber;

					if ( iGroupNumber <= -1 && iGroupNumber >= MAX_CRYWOLF_MONSTER_GROUP )
					{
						MsgBox("[Crywolf Main Job] - Invalid Monster Group Number (%d)", iGroupNumber);
					}

					this->m_iMonsterGroupNumberArray[this->m_iMonsterGroupNumberCount] = iGroupNumber;
					this->m_iMonsterGroupNumberCount++;
				}

				if ( iType == 3 )
				{
					int iGroupNumber = -1;
					int iTurnUpTime = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iGroupNumber = TokenNumber;
					
					Token = GetToken();
					iTurnUpTime = TokenNumber;

					if ( iGroupNumber <= -1 && iGroupNumber >= MAX_CRYWOLF_MONSTER_GROUP )
					{
						MsgBox("[Crywolf Main Job] - Invalid Monster Boss Group Number (%d)", iGroupNumber);
					}

					this->m_iBossGroupNumber = iGroupNumber;
					this->m_iBossTurnUpTime = iTurnUpTime;
				}

				if ( iType == 4 )
				{
					int iChangeAITime = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iChangeAITime = TokenNumber;
					this->m_iMonsterGroupChangeAITime = iChangeAITime;
				}

				if ( iType == 5 )
				{
					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					this->m_iMVPScoreTable[0] = TokenNumber;

					Token = GetToken();
					this->m_iMVPScoreTable[1] = TokenNumber;

					Token = GetToken();
					this->m_iMVPScoreTable[2] = TokenNumber;

					Token = GetToken();
					this->m_iMVPScoreTable[3] = TokenNumber;

					Token = GetToken();
					this->m_iMVPScoreTable[4] = TokenNumber;

					Token = GetToken();
					this->m_iMVPScoreTable[5] = TokenNumber;

					Token = GetToken();
					this->m_iMVPScoreTable[6] = TokenNumber;
				}

				if ( iType == 6 )
				{
					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					this->m_iMVPRankScoreTable[0] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankScoreTable[1] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankScoreTable[2] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankScoreTable[3] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankScoreTable[4] = TokenNumber;
				}

				if ( iType == 7 )
				{
					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					this->m_iMVPRankExpTable[0] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankExpTable[1] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankExpTable[2] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankExpTable[3] = TokenNumber;

					Token = GetToken();
					this->m_iMVPRankExpTable[4] = TokenNumber;
				}
			}
		}

		fclose(SMDFile);
		LogAddC(2, "[Crywolf Main Job] - %s file is Loaded", lpszFileName);
		this->m_bFileDataLoad = TRUE;
	}
	catch ( DWORD )
	{
		MsgBox("[Crywolf Main Job] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}

void CCrywolf::DelAllData()
{
	for ( int i=0;i<MAX_CRYWOLF_STATE;i++)
	{
		this->m_StateTimeInfo[i].Reset();
	}

	for ( int j=0;j<MAX_CRYWOLF_STATE_TIME;j++)
	{
		this->m_StartTimeInfo[j].Reset();
	}

	this->m_StartTimeInfoCount = 0;

	for ( int k=0;k<MAX_CRYWOLF_MONSTER_GROUP;k++)
	{
		this->m_iMonsterGroupNumberArray[k] = -1;
	}

	this->m_iMonsterGroupNumberCount = 0;
	this->m_iBossGroupNumber = 0;
	this->m_iBossTurnUpTime = 0;
	this->m_iMonsterGroupChangeAITime  = 0;
	this->m_bDBDataLoadOK = FALSE;
	this->m_bDBDataLoading = FALSE;

	memset(this->m_iMVPScoreTable, 0, 7);
	memset(this->m_iMVPRankScoreTable, 0, 5);
	memset(this->m_iMVPRankExpTable, 0, 5);

	this->SetState(CRYWOLF_STATE_NONE);
}

void CCrywolf::Init()
{
	return;
}

void CCrywolf::LoadCrywolfMapAttr(LPSTR lpszFileName, int iOccupationState)
{
	if ( !lpszFileName )
		return;

	if ( iOccupationState < 0 || iOccupationState >= 3 )
		return;

	this->m_CrywolfMapAttr[iOccupationState].LoadMapAttr(lpszFileName, MAP_INDEX_CRYWOLF_FIRSTZONE);
}


void CCrywolf::SetCrywolfMapAttr(int iOccupationState)
{
	if ( iOccupationState < 0 || iOccupationState >= 3 )
		return;

	MapClass * CrywolfMap = &this->m_CrywolfMapAttr[iOccupationState];

	if ( CHECK_CLASS(CrywolfMap->thisMapNumber, MAP_INDEX_CRYWOLF_FIRSTZONE) == FALSE )
	{
		LogAddC(2, "[ Crywolf ] Fail - Chang Map Attribute (State:%d) ", iOccupationState);
		return;
	}

	memcpy(MapC[MAP_INDEX_CRYWOLF_FIRSTZONE].m_attrbuf, CrywolfMap->m_attrbuf, 256*256);
}


void CCrywolf::SetCrywolfCommonNPC(int iOccupationState)
{
	for ( int i=0;i<this->m_ObjCommonNPC.m_iObjCount ; i++)
	{
		if ( !gObjIsConnected(this->m_ObjCommonNPC.m_iObjIndex[i]) )
			break;

		LPOBJ lpObj = &gObj[this->m_ObjCommonNPC.m_iObjIndex[i]];

		if ( iOccupationState == 2 || iOccupationState == 1)
		{
			gObjAddBuffEffect(lpObj, AT_CW_NPC_TRANSPARENCY, 0, 0, 0, 0, -10);
		}
		else if ( iOccupationState == 0 )
		{
			gObjRemoveBuffEffect(lpObj, AT_CW_NPC_TRANSPARENCY);
		}
	}
}

void CCrywolf::SetCrywolfAllCommonMonsterState(int iMonsterState, int iMode)
{
	for ( int i=0;i<this->m_ObjCommonMonster.m_iObjCount ; i++)
	{
		if ( !gObjIsConnected(this->m_ObjCommonMonster.m_iObjIndex[i]) )
			break;

		LPOBJ lpObj = &gObj[this->m_ObjCommonMonster.m_iObjIndex[i]];

		if ( iMode == 0 )
		{
		}
		else if ( iMode == 1 )
		{
		}
	}
}

void CCrywolf::RemoveCrywolfCommonMonster()
{
	for ( int i=0;i<this->m_ObjCommonMonster.m_iObjCount ; i++)
	{
		gObjDel(this->m_ObjCommonMonster.m_iObjIndex[i]);
	}

	this->m_ObjCommonMonster.Reset();
}

void CCrywolf::CreateCrywolfCommonMonster()
{
	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( CHECK_CLASS(gMSetBase.m_Mp[n].m_MapNumber, MAP_INDEX_CRYWOLF_FIRSTZONE) )
		{
			if ( gMSetBase.m_Mp[n].m_Type >= 204 && gMSetBase.m_Mp[n].m_Type <= 257 )
				continue;

			int iIndex = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

			if ( iIndex >= 0 )
			{
				if ( gObjSetPosMonster(iIndex, n) == FALSE )
				{
					gObjDel(iIndex);
					continue;
				}
				
				if ( gObjSetMonster(iIndex, gMSetBase.m_Mp[n].m_Type) == FALSE )
				{
					gObjDel(iIndex);
					continue;
				}

				if ( gObj[iIndex].Type == OBJ_MONSTER )
				{
					g_Crywolf.m_ObjCommonMonster.AddObj(iIndex);
				}
			}
		}
	}
}

void CCrywolf::ChangeAI(int iAIOrder)
{
	for ( int n=0;n<this->m_iMonsterGroupNumberCount;n++)
	{
		TMonsterAIGroup::ChangeAIOrder(this->m_iMonsterGroupNumberArray[n], iAIOrder);
	}
}

void CCrywolf::TurnUpBoss()
{
	TMonsterAIGroup::Init(this->m_iBossGroupNumber);
	TMonsterAIGroup::ChangeAIOrder(this->m_iBossGroupNumber, 1);
	this->m_iBossIndex = TMonsterAIGroup::FindGroupLeader(this->m_iBossGroupNumber);
}

int CCrywolf::CheckStateTimeSync()
{
	if (this->m_iCrywolfState != CRYWOLF_STATE_NONE )
	{
		if ( this->m_StateTimeInfo[this->m_iCrywolfState].m_bUsed == FALSE )
		{
			return -1;
		}

		DWORD dwTime = GetTickCount();

		if ( this->m_StateTimeInfo[this->m_iCrywolfState].CheckContinuanceTime() == FALSE )
		{
			this->m_StateTimeInfo[this->m_iCrywolfState].ResetAppliedTime();

			if ( this->m_iCrywolfState == CRYWOLF_STATE_ENDCYCLE )
			{
				if ( this->m_iScheduleMode == 1 )
				{
					this->SetState(CRYWOLF_STATE_NOTIFY_1);
				}
				else
				{
					this->SetState(CRYWOLF_STATE_NONE);
				}
			}
			else
			{
				this->SetNextState(this->m_iCrywolfState);
			}
		}
		return 1;
	}

	for ( int i=0;i<m_StartTimeInfoCount;i++)
	{
		if ( this->m_StartTimeInfo[i].CheckScheduleTime() != FALSE )
		{
			LogAddTD("[ Crywolf ] Start Crywolf");
			this->SetState(this->m_StartTimeInfo[i].m_iStateNumber);
		}
	}

	return 0;
}

void CCrywolf::Run()
{
	if ( !g_CrywolfSync.CheckEnableCrywolf())
		return;

	if ( this->m_StartTimeInfo[0].m_bUsed == FALSE )
		return;

	this->CheckStateTimeSync();

	switch ( this->m_iCrywolfState )
	{
		case CRYWOLF_STATE_NONE:
			this->ProcState_NONE();
			break;
		case CRYWOLF_STATE_NOTIFY_1:
			this->ProcState_NOTIFY_1();
			break;
		case CRYWOLF_STATE_NOTIFY_2:
			this->ProcState_NOTIFY_2();
			break;
		case CRYWOLF_STATE_READY:
			this->ProcState_READY();
			break;
		case CRYWOLF_STATE_START:
			this->ProcState_START();
			break;
		case CRYWOLF_STATE_END:
			this->ProcState_END();
			break;
		case CRYWOLF_STATE_ENDCYCLE:
			this->ProcState_ENDCYCLE();
			break;
	}
}

void CCrywolf::SetState(int iCrywolfState)
{
	switch ( iCrywolfState )
	{
		case CRYWOLF_STATE_NONE:
			this->SetState_NONE();
			break;
		case CRYWOLF_STATE_NOTIFY_1:
			this->SetState_NOTIFY_1();
			break;
		case CRYWOLF_STATE_NOTIFY_2:
			this->SetState_NOTIFY_2();
			break;
		case CRYWOLF_STATE_READY:
			this->SetState_READY();
			break;
		case CRYWOLF_STATE_START:
			this->SetState_START();
			break;
		case CRYWOLF_STATE_END:
			this->SetState_END();
			break;
		case CRYWOLF_STATE_ENDCYCLE:
			this->SetState_ENDCYCLE();
			break;
	}

	this->NotifyCrywolfCurrentState();
	this->CrywolfInfoDBSave();
}

void CCrywolf::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;
	this->SetState(iNextState);
}

void CCrywolf::SetState_NONE()
{
	LogAddC(4, "[ Crywolf ] State (%d) -> NONE", this->m_iCrywolfState);

	this->SetCrywolfState(CRYWOLF_STATE_NONE);
	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_NONE);
}

void CCrywolf::SetState_NOTIFY_1()
{
	UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 228)));
	LogAddC(4, "[ Crywolf ] State (%d) -> NOTIFY_1", this->m_iCrywolfState);

	this->SetCrywolfState(CRYWOLF_STATE_NOTIFY_1);
	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_NOTIFY_1);
	this->m_dwCrywolfNotifyMsgStartTick = GetTickCount();
}

void CCrywolf::SetState_NOTIFY_2()
{
	UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 229)));
	LogAddC(4, "[ Crywolf ] State (%d) -> NOTIFY_2", this->m_iCrywolfState);

	this->SetCrywolfState(CRYWOLF_STATE_NOTIFY_2);
	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_NOTIFY_2);
	this->SetOccupationState(2);
	this->SetCrywolfCommonNPC(2);
	this->SetCrywolfMapAttr(2);
	this->RemoveCrywolfCommonMonster();
	this->m_dwCrywolfNotifyMsgStartTick = GetTickCount();
}

void CCrywolf::SetState_READY()
{
	UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 230)));
	LogAddC(4, "[ Crywolf ] State (%d) -> READY", this->m_iCrywolfState);

	this->SetCrywolfState(CRYWOLF_STATE_READY);
	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_READY);
	this->SetOccupationState(2);
	this->SetCrywolfCommonNPC(2);
	this->SetCrywolfMapAttr(2);
	this->RemoveCrywolfCommonMonster();
	g_CrywolfNPC_Altar.SetAllAltarObjectIndex();
	g_CrywolfNPC_Altar.SetAllAltarViewState(0x10000);

	for ( int n=0;n<this->m_iMonsterGroupNumberCount;n++)
	{
		TMonsterAIGroup::Init(this->m_iMonsterGroupNumberArray[n]);
	}

	this->m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	this->ResetAllUserMVPScore();
}

void CCrywolf::SetState_START()
{
	UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 231)));
	LogAddC(4, "[ Crywolf ] State (%d) -> START", this->m_iCrywolfState);

	this->SetCrywolfState(CRYWOLF_STATE_START);
	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_START);

	if ( g_CrywolfNPC_Altar.GetContractedAltarCount() == 0 )
	{
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 232)));
		this->SetOccupationState(1);
		this->SetState(CRYWOLF_STATE_END);

		return;
	}

	for ( int n=0;n<this->m_iMonsterGroupNumberCount;n++)
	{
		int iLeaderIndex = TMonsterAIGroup::FindGroupLeader(this->m_iMonsterGroupNumberArray[n]);

		if ( !CHECK_LIMIT(iLeaderIndex, OBJ_MAXMONSTER) )
			continue;

		if ( gObj[iLeaderIndex].Class != 340 )
			continue;

		LPOBJ lpObj = &gObj[iLeaderIndex];

		for ( int i=0;i<MAX_VIEWPORT_MONSTER;i++)
		{
			if ( lpObj->VpPlayer2[i].state )
			{
				int number = lpObj->VpPlayer2[i].number;

				if ( OBJMAX_RANGE(number) != FALSE )
				{
					if ( gObj[number].Type == OBJ_USER )
					{
						gObjBackSpring2(&gObj[number], lpObj, 3);
					}
				}
			}
		}
	}

	this->ChangeAI(1);
	this->m_dwCrywolfStartProcTick = GetTickCount();
	this->m_bTurnUpBoss = FALSE;
	this->m_bChangeAI = FALSE;
}

void CCrywolf::SetState_END()
{
	this->m_dwCrywolfStartProcTick = 0;
	this->m_bChangeAI = FALSE;

	LogAddC(4, "[ Crywolf ] State (%d) -> END", this->m_iCrywolfState);

	if ( this->m_bTurnUpBoss != FALSE && CHECK_LIMIT(this->m_iBossIndex, OBJ_MAXMONSTER) )
	{
		if ( gObj[this->m_iBossIndex].Live != FALSE )
		{
			UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 233)));
			this->SetOccupationState(1);
		}
	}

	this->m_bTurnUpBoss = FALSE;
	this->SetCrywolfBossIndex(-1);

	if ( this->GetOccupationState() == 1 )
	{
		UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 234)));
		this->SetOccupationState(1);

		LogAddTD("[ Crywolf ][MVP] FAIL.......!!");
	}
	else
	{
		UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 235)));
		this->SetOccupationState(0);

		LogAddTD("[ Crywolf ][MVP] SUCCESS.......!!");
	}

	this->SetCrywolfState(CRYWOLF_STATE_END);
	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_END);
	this->NotifyCrywolfCurrentState();
	this->ChangeAI(0);
	
	for ( int iAltar = 205 ; iAltar <= 209 ; iAltar++)
	{
		int iUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(iAltar);

		if ( iUserIndex != -1 )
		{
			this->CalcGettingScore(iUserIndex, -1, 6);

			if ( this->GetOccupationState() != 1 )
			{
				this->MakeRewardForAltarElf(iUserIndex);
			}
		}
	}

	g_CrywolfNPC_Altar.ResetAllAltar();
	this->NotifyCrywolfPersonalRank();
}

void CCrywolf::SetState_ENDCYCLE()
{
	LogAddC(4, "[ Crywolf ] State (%d) -> ENDCYCLE", this->m_iCrywolfState);

	this->SetCrywolfState(CRYWOLF_STATE_ENDCYCLE);
	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_ENDCYCLE);
	this->SetCrywolfMapAttr(this->GetOccupationState());
	this->SetCrywolfCommonNPC(this->GetOccupationState());

	for ( int n=0;n<this->m_iMonsterGroupNumberCount;n++)
	{
		TMonsterAIGroup::DelGroupInstance(this->m_iMonsterGroupNumberArray[n]);
	}

	TMonsterAIGroup::DelGroupInstance(this->m_iBossGroupNumber);

	this->CreateCrywolfCommonMonster();
	this->NotifyCrywolfHeroList();
	this->ResetAllUserMVPScore();
}

void CCrywolf::ProcState_NONE()
{
	return;
}

void CCrywolf::ProcState_NOTIFY_1()
{
	if ( (GetTickCount() - this->m_dwCrywolfNotifyMsgStartTick) > 70000 )
	{
		UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 228)));
		this->m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	}
}

void CCrywolf::ProcState_NOTIFY_2()
{
	if ( (GetTickCount() - this->m_dwCrywolfNotifyMsgStartTick) > 70000 )
	{
		int iTime = ( this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime() / 1000 ) / 60;

		if ( iTime == 0 )
		{
			iTime = ( this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime() / 1000 );

			UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 236)), iTime);
		}
		else
		{
			UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 237)), iTime);
		}

		this->m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	}
}

void CCrywolf::ProcState_READY()
{
	this->NotifyCrywolfStatueAndAltarInfo();

	if ( (GetTickCount() - this->m_dwCrywolfNotifyMsgStartTick) > 70000 )
	{
		int iTime = ( this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime() / 1000 ) / 60;

		if ( iTime == 0 )
		{
			iTime = ( this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime() / 1000 );

			UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 238)), iTime);
		}
		else
		{
			UTIL.SendMapServerGroupMsg(lMsg.Get(MSGGET(12, 239)), iTime);
		}

		this->m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	}
}

void CCrywolf::ProcState_START()
{
	if ( g_CrywolfNPC_Altar.GetContractedAltarCount() == 0 )
	{
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 240)));

		this->SetOccupationState(1);
		this->SetState(CRYWOLF_STATE_END);

		return;
	}

	this->NotifyCrywolfStatueAndAltarInfo();
	this->NotifyCrywolfBossMonsterInfo();
	this->NotifyCrywolfStateLeftTime();

	if ( (GetTickCount() - this->m_dwCrywolfStartProcTick) > (this->m_iBossTurnUpTime * 1000) &&
		 !this->m_bTurnUpBoss )
	{
		this->m_bTurnUpBoss = TRUE;
		
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 241)));

		this->TurnUpBoss();
		
		LogAddTD("[ Crywolf ][MVP] Barlgars TurnUp !!!");
	}

	if ( (GetTickCount() - this->m_dwCrywolfStartProcTick) > (this->m_iMonsterGroupChangeAITime * 1000) &&
		 !this->m_bChangeAI )
	{
		this->m_bChangeAI = TRUE;
		
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 242)));

		this->ChangeAI(2);
		
		LogAddTD("[ Crywolf ][MVP] Start Monster Rush !!!");
	}

	if ( this->m_bTurnUpBoss != FALSE && CHECK_LIMIT(this->m_iBossIndex, OBJ_MAXMONSTER) != FALSE )
	{
		if ( gObj[this->m_iBossIndex].Live == FALSE )
		{
			UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 243)));	

			this->SetOccupationState(0);
			this->SetState(CRYWOLF_STATE_END);
		}
	}
}

void CCrywolf::ProcState_END()
{
	return;
}

void CCrywolf::ProcState_ENDCYCLE()
{
	return;
}

void CCrywolf::NotifyCrywolfCurrentState()
{
	PMSG_ANS_CRYWOLF_INFO pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x00, sizeof(pMsg));
	pMsg.btOccupationState = this->GetOccupationState();
	pMsg.btCrywolfState = this->GetCrywolfState();

	UTIL.SendCrywolfUserAnyData((LPBYTE)&pMsg,sizeof(pMsg));

	LogAddC(3, "[ Crywolf ] SetState - StateInfo : Occupation/%d, State/%d",
		pMsg.btOccupationState, pMsg.btCrywolfState);
}

void CCrywolf::NotifyCrywolfStateLeftTime()
{
	static DWORD dwTickCount = 0;

	if ( (GetTickCount() - dwTickCount) < 20000 )
		return;

	dwTickCount = GetTickCount();

	PMSG_ANS_CRYWOLF_LEFTTIME pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x04, sizeof(pMsg));
	pMsg.btHour = ( ( this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime() / 1000 ) / 60 ) / 24;
	pMsg.btMinute = ( ( this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime() / 1000 ) / 60 );

	UTIL.SendCrywolfUserAnyData((LPBYTE)&pMsg,sizeof(pMsg));

	LogAddC(3, "[ Crywolf ] War LeftTime - (%02d:%02d)", pMsg.btHour, pMsg.btMinute);
}

void CCrywolf::NotifyCrywolfStatueAndAltarInfo()
{
	static DWORD dwTickCount = 0;

	if ( (GetTickCount() - dwTickCount) < 2000 )
		return;

	dwTickCount = GetTickCount();

	PMSG_ANS_CRYWOLF_STATUE_ALTAR_INFO pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x02, sizeof(pMsg));

	pMsg.iCrywolfStatueHP = g_CrywolfNPC_Statue.m_Shield.GetHPPercentage();
	pMsg.btAltarState1 = g_CrywolfNPC_Altar.GetAltarRemainContractCount(205);
	pMsg.btAltarState2 = g_CrywolfNPC_Altar.GetAltarRemainContractCount(206);
	pMsg.btAltarState3 = g_CrywolfNPC_Altar.GetAltarRemainContractCount(207);
	pMsg.btAltarState4 = g_CrywolfNPC_Altar.GetAltarRemainContractCount(208);
	pMsg.btAltarState5 = g_CrywolfNPC_Altar.GetAltarRemainContractCount(209);
	pMsg.btAltarState1 |= g_CrywolfNPC_Altar.GetAltarState(205) << 4;
	pMsg.btAltarState2 |= g_CrywolfNPC_Altar.GetAltarState(206) << 4;
	pMsg.btAltarState3 |= g_CrywolfNPC_Altar.GetAltarState(207) << 4;
	pMsg.btAltarState4 |= g_CrywolfNPC_Altar.GetAltarState(208) << 4;
	pMsg.btAltarState5 |= g_CrywolfNPC_Altar.GetAltarState(209) << 4;

	UTIL.SendCrywolfUserAnyData((LPBYTE)&pMsg,sizeof(pMsg));

	LogAddTD("[ Crywolf ][Statue Info] Statue HP : %d",
		pMsg.iCrywolfStatueHP);

	for ( int iAltar = 205 ; iAltar <= 209 ; iAltar ++ )
	{
		int iUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(iAltar);

		if ( iUserIndex != -1 )
		{
			LogAddTD("[ Crywolf ][Altar Info] Altar(%d) Contractor: [%s][%s] ",	iAltar, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name);
		}
	}
}

struct PMSG_ANS_CRYWOLF_BOSSMONSTER_INFO
{
	PBMSG_HEAD2 h;	// C1:BD:05
	int iMonster1HP;	// 4
	BYTE btMonster2;	// 8
};

void CCrywolf::NotifyCrywolfBossMonsterInfo()
{
	static DWORD dwTickCount = 0;

	if ( (GetTickCount() - dwTickCount) < 5000 )
		return;

	dwTickCount = GetTickCount();

	PMSG_ANS_CRYWOLF_BOSSMONSTER_INFO pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x05, sizeof(pMsg));
	pMsg.iMonster1HP = -1;
	pMsg.btMonster2 = 0;

	for ( int i=0;i<OBJ_MAXMONSTER;i++)
	{
		if (!gObjIsConnected(i))
			continue;

		if ( gObj[i].MapNumber != MAP_INDEX_CRYWOLF_FIRSTZONE )
			continue;

		if ( !gObj[i].Live )
			continue;

		switch ( gObj[i].Class )
		{
			case 340:
				pMsg.btMonster2++;
				break;
			case 349:
				pMsg.iMonster1HP = (gObj[i].Life * 100.0f) / (gObj[i].MaxLife + 1.0f);
				break;
		}
	}

	UTIL.SendCrywolfUserAnyData((LPBYTE)&pMsg,sizeof(pMsg));

	LogAddTD("[ Crywolf ][Boss Monster Info] Balgars HP:%d, DarkElf:%d", pMsg.iMonster1HP, pMsg.btMonster2);
}

struct PMSG_ANS_CRYWOLF_STAGE_EFFECT_ONOFF
{
	PBMSG_HEAD2 h;	// C1:BD:06
	BYTE btStageEffectOnOff;	// 4
};

void CCrywolf::NotifyCrywolfStageEffectOnOff(BYTE btOnOff)
{
	PMSG_ANS_CRYWOLF_STAGE_EFFECT_ONOFF pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x06, sizeof(pMsg));
	pMsg.btStageEffectOnOff = btOnOff;

	UTIL.SendCrywolfUserAnyData((LPBYTE)&pMsg,sizeof(pMsg));
}

struct PMSG_ANS_CRYWOLF_PERSONAL_RANK
{
	PBMSG_HEAD2 h;	// C1:BD:07
	BYTE btRank;	// 4
	int iGettingExp;	// 8
};

void CCrywolf::NotifyCrywolfPersonalRank()
{
	PMSG_ANS_CRYWOLF_PERSONAL_RANK pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x07, sizeof(pMsg));
	pMsg.btRank = 0;

	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER && gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		{
			pMsg.btRank = this->CalcGettingRank(i);
			pMsg.iGettingExp = this->CalcGettingRewardExp(i, pMsg.btRank);

			this->GiveRewardExp(i, pMsg.iGettingExp);

			MsgOutput(i, lMsg.Get(MSGGET(12, 244)), pMsg.btRank);
			MsgOutput(i, lMsg.Get(MSGGET(12, 245)), pMsg.iGettingExp);

			LogAddTD("[ Crywolf ][Personal Rank & Exp.] [%s][%s] Score(%d) Rank(%d) Exp(%d)", gObj[i].AccountID, gObj[i].Name, gObj[i].m_iCrywolfMVPScore, pMsg.btRank, pMsg.iGettingExp);
			DataSend(i, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
}

struct PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT
{
	PBMSG_HEAD2 h;	// C1:BD:08
	BYTE btCount;	// 4
};

struct PMSG_ANS_CRYWOLF_HERO_LIST_INFO
{
	BYTE iRank;	// 0
	char szHeroName[10];	// 1
	int iHeroScore;	// C
	BYTE btHeroClass;	// 10
};

void CCrywolf::NotifyCrywolfHeroList()
{

	char cBUFFER[6000];
	int iHeroCount = 0;
	memset(cBUFFER, 0, sizeof(cBUFFER));
	PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT * lpMsg = (PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT *)cBUFFER;
	PMSG_ANS_CRYWOLF_HERO_LIST_INFO * lpMsgBody = (PMSG_ANS_CRYWOLF_HERO_LIST_INFO *)(cBUFFER + 5);
	std::set<LPOBJ,CCrywolfScoreSort> HeroSet;
	
	for (int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER && gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		{
			HeroSet.insert(&gObj[i]);
		}
	}

	std::set<LPOBJ, CCrywolfScoreSort>::iterator _Itor = HeroSet.begin();
	
	for(int j=0;j<5&& _Itor != HeroSet.end();j++, _Itor++)
	{
		LPOBJ lpHeroObj = (*(_Itor));

		lpMsgBody[j].iRank = iHeroCount;
		lpMsgBody[j].btHeroClass = lpHeroObj->Class;
		lpMsgBody[j].iHeroScore = lpHeroObj->m_iCrywolfMVPScore;
		memcpy(lpMsgBody[j].szHeroName, lpHeroObj->Name, MAX_ACCOUNT_LEN);
		iHeroCount++;

		LogAddTD("[ Crywolf ][Hero List] [%s][%s] Score(%d) Ranking(%d)",
			lpHeroObj->AccountID, lpHeroObj->Name, lpHeroObj->m_iCrywolfMVPScore, iHeroCount);

		if ( this->GetOccupationState() != 1 )
		{
			if ( lpMsgBody[j].iRank <= 5 )
			{
				this->MakeRewardForHeroListTop5(lpHeroObj->m_Index);
			}
		}
	}

	lpMsg->btCount = iHeroCount;

	PHeadSubSetB((LPBYTE)lpMsg, 0xBD, 0x08, iHeroCount*sizeof(PMSG_ANS_CRYWOLF_HERO_LIST_INFO) + sizeof(PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT));

	UTIL.SendCrywolfUserAnyData((LPBYTE)lpMsg, lpMsg->h.size);
}

void CCrywolf::OperateGmCommand(int iUserIndex, int iCommand)
{
	if ( !gObjIsConnected(iUserIndex))
		return;

	if ( (gObj[iUserIndex].Authority & 0x02) != 0x02 )
		return;

	LogAddTD("[ Crywolf ][Use GM Command] [%s][%s] Command : %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, iCommand);

	switch ( iCommand )
	{
		case 0:
			this->SetState(CRYWOLF_STATE_NOTIFY_2);
			break;

		case 1:
			this->SetState(CRYWOLF_STATE_READY);
			break;

		case 2:
			if ( this->GetCrywolfState() == CRYWOLF_STATE_READY )
			{
				this->SetState(CRYWOLF_STATE_START);
			}
			else
			{
				MsgOutput(iUserIndex, lMsg.Get(MSGGET(12, 246)));
			}
			break;

		case 3:
			this->SetState(CRYWOLF_STATE_END);
			break;
	}
}

void CCrywolf::CrywolfServerGroupSync()
{
	GDReqCrywolfSync(g_MapServerManager.GetMapSvrGroup(), g_Crywolf.GetCrywolfState(), g_Crywolf.GetOccupationState());
}

void CCrywolf::CrywolfInfoDBSave()
{
	if ( this->m_bDBDataLoadOK != FALSE )
	{
		GDReqCrywolfInfoSave(g_MapServerManager.GetMapSvrGroup(), this->GetCrywolfState(), this->GetOccupationState());
	}
}

void CCrywolf::CrywolfInfoDBLoad()
{
	this->m_bDBDataLoading = TRUE;

	GDReqCrywolfInfoLoad(g_MapServerManager.GetMapSvrGroup());
}

void CCrywolf::ApplyCrywolfDBInfo(int iCrywolfState, int iOccupationState)
{
	this->SetDBDataLoad(TRUE);
	this->SetOccupationState(iOccupationState);
	this->SetCrywolfCommonNPC(this->GetOccupationState());
	this->SetCrywolfMapAttr(this->GetOccupationState());

	LogAddTD("[ Crywolf ][ ApplyDBInfo] OccupationState : %d", iOccupationState);
}

void CCrywolf::CrywolfMonsterDieProc(int iMonIndex, int iKillerIndex)
{
	LPOBJ lpMonObj = &gObj[iMonIndex];
	LPOBJ lpKillerObj = &gObj[iKillerIndex];
	BOOL bExistKiller = gObjIsConnected(iKillerIndex);

	if ( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START )
	{
		if ( lpMonObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
		{
			if ( lpMonObj->m_iCurrentAI != 0 )
			{
				if ( lpMonObj->Class == 349 )
				{
					if ( bExistKiller )
					{
						UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 247)), lpKillerObj->Name);

						LogAddTD("[ Crywolf ][MVP] [Balgars Dead] by [%s][%s]",	lpKillerObj->AccountID, lpKillerObj->Name);
					}
					else
					{
						UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 248)));

						LogAddTD("[ Crywolf ][MVP] [Balgars Dead] by Unknown User");
					}
				}

				if ( lpMonObj->Class == 340 )
				{
					if ( bExistKiller )
					{
						UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 249)), lpMonObj->m_iGroupNumber, lpKillerObj->Name);

						LogAddTD("[ Crywolf ][MVP] [DarkElf Dead] by [%s][%s]",	lpKillerObj->AccountID, lpKillerObj->Name);
					}
				}
		
				int iScore = this->CalcGettingScore(iKillerIndex, iMonIndex, 100);

				if ( iScore != -1 )
				{
					MsgOutput(iKillerIndex, lMsg.Get(MSGGET(12, 250)), iScore);
				}
			}
		}
	}
}

void CCrywolf::MakeRewardForAltarElf(int iAltarUserIndex)
{
#if defined __REEDLAN__ || __BEREZNUK__
	g_ShopPointEx.AddEventBonus(iAltarUserIndex, ShopPointExEvent::CW);
#endif
	int iItemNumber = ITEMGET(14,13);
	ItemSerialCreateSend(gObj[iAltarUserIndex].m_Index, gObj[iAltarUserIndex].MapNumber, gObj[iAltarUserIndex].X, gObj[iAltarUserIndex].Y, iItemNumber, 0, 0, 0, 0, 0, iAltarUserIndex, 0, 0);
	LogAddTD("[ Crywolf ][Reward] [Altar Elf] [%s][%s] Success Reward - Gem Of Bless", gObj[iAltarUserIndex].AccountID, gObj[iAltarUserIndex].Name);
}

void CCrywolf::MakeRewardForHeroListTop5(int iUserIndex)
{
#if defined __REEDLAN__ || __BEREZNUK__
	g_ShopPointEx.AddEventBonus(iUserIndex, ShopPointExEvent::CW);
#endif
	int iItemNumber = ITEMGET(14,13);
	ItemSerialCreateSend(gObj[iUserIndex].m_Index, gObj[iUserIndex].MapNumber, gObj[iUserIndex].X, gObj[iUserIndex].Y, iItemNumber, 0, 0, 0, 0, 0, iUserIndex, 0, 0);
	LogAddTD("[ Crywolf ][Reward] [Hero List Top 5] [%s][%s] Success Reward - Gem Of Bless", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name);
}

int CCrywolf::CalcGettingScore(int iUserIndex, int iMonIndex, int iScoreType)
{
	if ( !gObjIsConnected(iUserIndex))
		return -1;

	int iAddMVPScore = 0;

	if ( CHECK_LIMIT(iMonIndex, OBJ_MAXMONSTER) != FALSE )
	{
		switch ( gObj[iMonIndex].Class )
		{
			case 349:
				iAddMVPScore = this->m_iMVPScoreTable[0];
				break;
			case 340:
				iAddMVPScore = this->m_iMVPScoreTable[1];
				break;
			case 344:
				iAddMVPScore = this->m_iMVPScoreTable[2];
				break;
			case 341:
				iAddMVPScore = this->m_iMVPScoreTable[3];
				break;
			case 345:
				iAddMVPScore = this->m_iMVPScoreTable[4];
				break;
			case 348:
				iAddMVPScore = this->m_iMVPScoreTable[5];
				break;
		}
	}

	if ( iScoreType == 6 )
	{
		iAddMVPScore = this->m_iMVPScoreTable[6];
	}

	gObj[iUserIndex].m_iCrywolfMVPScore += iAddMVPScore;
	LogAddTD("[ Crywolf ][MVP] Plus MVP Score : %d, Accumulated : %d [%s][%s]",	iAddMVPScore, gObj[iUserIndex].m_iCrywolfMVPScore,	gObj[iUserIndex].AccountID, gObj[iUserIndex].Name);
	return gObj[iUserIndex].m_iCrywolfMVPScore;
}

int CCrywolf::CalcGettingRewardExp(int iUserIndex, int iMVPRank)
{
	if ( !gObjIsConnected(iUserIndex))
		return 0;

	if ( CHECK_LIMIT(iMVPRank, 5) == FALSE )
		return 0;

	__int64 iRewardExp = this->m_iMVPRankExpTable[iMVPRank];

	if ( this->GetOccupationState() == 1 )
	{
		iRewardExp = (iRewardExp * 10) / 100;
	}

	CheckItemOptForGetExpEx(&gObj[iUserIndex], iRewardExp, TRUE); //Seal Exp

	return iRewardExp;
}

int CCrywolf::CalcGettingRank(int iUserIndex)
{
	int iRank = 0;

	for ( int i=0;i<=4;i++)
	{
		if ( gObj[iUserIndex].m_iCrywolfMVPScore < this->m_iMVPRankScoreTable[i] )
			break;

		iRank = i;
	}

	return iRank;
}

void CCrywolf::GiveRewardExp(int iUserIndex, int iRewardExp)
{
	if ( !gObjIsConnected(iUserIndex))
		return;

	if ( iRewardExp <= -1 )
		return;

	int iRET_EXP = 0;
	int iCAL_EXP = iRewardExp;
	int iMAX_LEVCOUNT = 0;

	if ( !gObjIsConnected(iUserIndex))
		return;

	iRET_EXP = iCAL_EXP;

	if ( gObj[iUserIndex].Type == OBJ_USER )
	{
		while ( iCAL_EXP > 0 )
		{
			if ( iCAL_EXP > 0 )
			{
				iCAL_EXP = UTIL.CrywolfMVPLevelUp(iUserIndex, iCAL_EXP);
			}

			iMAX_LEVCOUNT++;

			if ( iMAX_LEVCOUNT > 5 )
			{
				LogAddTD("[ Crywolf ][GiveRewardExp] LevelUp Error [%s][%s]", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name);
				break;
			}
		}

		if(g_MasterLevelSystem.IsMasterLevelUser(&gObj[iUserIndex]) == FALSE) //season3 add-on
		{
			GCSendExp_INT64(iUserIndex, (WORD)-1, iRewardExp, 0, 0);
		}
	}
}

void CCrywolf::ResetAllUserMVPScore()
{
	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER && gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		{
			gObj[i].m_iCrywolfMVPScore = 0;
		}
	}
}

void CCrywolf::CrywolfNpcAct(int iIndex)
{
	if ( this->GetCrywolfState() == CRYWOLF_STATE_READY || this->GetCrywolfState() == CRYWOLF_STATE_START )
	{
		g_CrywolfNPC_Statue.CrywolfStatueAct(iIndex);
		g_CrywolfNPC_Altar.CrywolfAltarAct(iIndex);
	}
}

void CCrywolf::CrywolfMonsterAct(int iIndex)
{
	return;
}

void CCrywolf::CrywolfSecondAct()
{
	this->CrywolfServerGroupSync();

	if ( this->m_bDBDataLoadOK == FALSE && this->m_bDBDataLoading == FALSE )
	{
		this->CrywolfInfoDBLoad();
	}
}
