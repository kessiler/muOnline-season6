// KanturuBattleOfMaya.cpp: implementation of the CKanturuBattleOfMaya class.
//	GS-CS	1.00.90	JPN		-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "KanturuBattleOfMaya.h"
#include "Gamemain.h"
#include "TMonsterAIGroup.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "KanturuBattleUserMng.h"
#include "LogProc.h"
#include "..\include\ReadScript.h"
#include "KanturuUtil.h"

static CKanturuUtil KANTURU_UTIL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleOfMaya::CKanturuBattleOfMaya()
{
	this->SetBattleOfMayaState(0);
	this->SetBattleSceneState(-1);
	this->m_iMayaLeftHandObjIndex = -1;
	this->m_iMayaRightHandObjIndex = -1;

	for ( int iCount=0;iCount<KANTURU_MAYA_AI_CHANGE_TIME;iCount++)
	{
		this->m_iMayaHandAIChangeTime[iCount] = -1;
	}

	this->m_iMayaHandAIAppliedTime = 0;
	this->m_iMayaHandCurrentAI = 0;

	for ( iCount=0;iCount<KANTURU_MAYA_GROUP_NUMBER;iCount++)
	{
		this->m_iMayaHandGroupNumber[iCount] = -1;
	}

	this->m_iMayaHandDieTimeCounter = 0;
	this->ResetAllData();
}




CKanturuBattleOfMaya::~CKanturuBattleOfMaya()
{
	return;
}


void CKanturuBattleOfMaya::ResetAllData()
{
	this->m_bFileDataLoad = FALSE;

	for ( int iCount=0;iCount<KANTURU_MAYA_TIME_INFO;iCount++)
	{
		this->m_BattleTimeInfo[iCount].ResetTimeInfo();
	}

	for ( iCount=0;iCount<KANTURU_MAYA_STATE_INFO;iCount++)
	{
		this->m_StateInfo[iCount].ResetTimeInfo();
	}

	this->m_StateInfoCount = 0;
	this->m_BattleTimeInfoCount = 0;

	this->SetSuccessValue(FALSE);
	this->SetSceneSuccessValue(FALSE);
	this->SetEntrancePermit(FALSE);

	this->m_iMayaHandAIChangeTimeCount = 0;
	this->m_iMayaHAndGroupNumberCount = 0;
	this->m_iMayaIceStormUsingRate = 0;
}


BOOL CKanturuBattleOfMaya::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName , "") )
	{
		MsgBox("[ KANTURU ][ BattleOfMaya ] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		enum SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[ KANTURU ][ BattleOfMaya ] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		this->ResetAllData();
	
		int iType = -1;
		int iState = 0;
		int iCondition = 0;
		int iValue = 0;

		while ( true )
		{
			Token = GetToken();
			
			if ( Token == END )
			{
				break;
			}

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 2 )
				{
					iState = 0;
					iCondition = 0;
					iValue = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					iState = TokenNumber;

					Token = GetToken();
					iCondition = TokenNumber;

					Token = GetToken();
					iValue = TokenNumber;

					if ( this->m_BattleTimeInfoCount < 0 ||
						 this->m_BattleTimeInfoCount >= KANTURU_MAYA_TIME_INFO )
					{
						MsgBox("[ KANTURU ][ BattleOfMaya ] - Exceed Max Scene (%d)",
							this->m_BattleTimeInfoCount);

						break;
					}

					this->m_BattleTimeInfo[this->m_BattleTimeInfoCount].SetStateInfo(iState);
					this->m_BattleTimeInfo[this->m_BattleTimeInfoCount].SetCondition(iCondition);

					if ( iCondition == 1 )
					{
						iValue *= 1000;
					}

					this->m_BattleTimeInfo[this->m_BattleTimeInfoCount].SetValue(iValue);
					this->m_BattleTimeInfoCount++;
				}
				else if ( iType == 3 )
				{
					iState = 0;
					iCondition = 0;
					iValue = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					iState = TokenNumber;

					Token = GetToken();
					iCondition = TokenNumber;

					Token = GetToken();
					iValue = TokenNumber;

					if ( this->m_StateInfoCount < 0 ||
						 this->m_StateInfoCount >= KANTURU_MAYA_STATE_INFO )
					{
						MsgBox("[ KANTURU ][ BattleOfMaya ] - Exceed Max State Time (%d)",
							this->m_StateInfoCount);

						break;
					}

					this->m_StateInfo[this->m_StateInfoCount].SetStateInfo(iState);
					this->m_StateInfo[this->m_StateInfoCount].SetCondition(iCondition);

					if ( iCondition == 1 )
					{
						iValue *= 1000;
					}

					this->m_StateInfo[this->m_StateInfoCount].SetValue(iValue);
					this->m_StateInfoCount++;
				}
				else if ( iType == 4 )
				{
					int iValue = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					iValue = TokenNumber;

					if ( this->m_iMayaHandAIChangeTimeCount < 0 ||
						 this->m_iMayaHandAIChangeTimeCount >= KANTURU_MAYA_AI_CHANGE_TIME )
					{
						MsgBox("[ KANTURU ][ BattleOfMaya ] - Exceed Maya Hand AI Change Time(%d)",
							this->m_iMayaHandAIChangeTime);	// #error Change to m_iMayaHandAIChangeTimeCount

						break;
					}
					
					this->m_iMayaHandAIChangeTime[this->m_iMayaHandAIChangeTimeCount] = iValue * 1000;
					this->m_iMayaHandAIChangeTimeCount++;
				}
				else if ( iType == 5 )
				{
					int iValue = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					iValue = TokenNumber;

					if ( this->m_iMayaHAndGroupNumberCount < 0 ||
						 this->m_iMayaHAndGroupNumberCount >= KANTURU_MAYA_GROUP_NUMBER )
					{
						MsgBox("[ KANTURU ][ BattleOfMaya ] - Maya Hand AI Group Read Error(%d)",
							this->m_iMayaHAndGroupNumberCount);

						break;
					}
					
					this->m_iMayaHandGroupNumber[this->m_iMayaHAndGroupNumberCount] = iValue;
					this->m_iMayaHAndGroupNumberCount++;
				}
				else if ( iType == 6 )
				{
					int iValue = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					iValue = TokenNumber;

					if ( iValue < 0 )
					{
						MsgBox("[ KANTURU ][ BattleOfMaya ] - Maya IceStorm Using Rate Load Error(%d)",
							iValue);

						iValue = 60;
					}
					
					this->m_iMayaIceStormUsingRate = iValue;
					
					LogAddTD("[ KANTURU ][ BattleOfMaya ] IceStorm Using Rate Load: %d",
						this->m_iMayaIceStormUsingRate);
				}
				else
				{
					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}
				}
			}	// while ( true )
		}	// while ( true )

		fclose(SMDFile);
		LogAddC(2, "[ KANTURU ][ BattleOfMaya ] %s file is Loaded",
			lpszFileName);

		this->m_bFileDataLoad = TRUE;
	}	// __try
	catch ( DWORD )
	{
		MsgBox("[ KANTURU ][ BattleOfMaya ] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}




void CKanturuBattleOfMaya::Run()
{
	this->CheckStateTime();
	this->CheckMayaBattleUser();
	g_KanturuBattleUserMng.CheckUserState();

	switch ( this->m_iBattleOfMayaState )
	{
		case KANTURU_MAYA_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_MAYA_STANBY1:
			this->ProcState_STANBY1();
			break;
		case KANTURU_MAYA_NOTIFY1:
			this->ProcState_NOTIFY1();
			break;
		case KANTURU_MAYA_START1:
			this->ProcState_START1();
			break;
		case KANTURU_MAYA1:
			this->ProcState_MAYA1();
			break;
		case KANTURU_MAYA_END1:
			this->ProcState_END_MAYA1();
			break;
		case KANTURU_MAYA_ENDCYCLE1:
			this->ProcState_ENDCYCLE_MAYA1();
			break;
		case KANTURU_MAYA_STANBY2:
			this->ProcState_STANBY2();
			break;
		case KANTURU_MAYA_START2:
			this->ProcState_START2();
			break;
		case KANTURU_MAYA2:
			this->ProcState_MAYA2();
			break;
		case KANTURU_MAYA_END2:
			this->ProcState_END_MAYA2();
			break;
		case KANTURU_MAYA_ENDCYCLE2:
			this->ProcState_ENDCYCLE_MAYA2();
			break;
		case KANTURU_MAYA_STANBY3:
			this->ProcState_STANBY3();
			break;
		case KANTURU_MAYA_START3:
			this->ProcState_START3();
			break;
		case KANTURU_MAYA3:
			this->ProcState_MAYA3();
			break;
		case KANTURU_MAYA_END3:
			this->ProcState_END_MAYA3();
			break;
		case KANTURU_MAYA_ENDCYCLE3:
			this->ProcState_ENDCYCLE_MAYA3();
			break;
		case KANTURU_MAYA_END:
			this->ProcState_END();
			break;
		case KANTURU_MAYA_ENDCYCLE:
			this->ProcState_ENDCYCLE();
			break;
	}
}



void CKanturuBattleOfMaya::SetState(int iBattleOfMayaState)
{
	if ( this->m_StateInfo[iBattleOfMayaState].GetCondition() > 0 )
	{
		this->m_StateInfo[iBattleOfMayaState].SetAppliedTime();
	}

	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	this->m_KanturuMaya.Init();
	this->ResetAIMonster();

	switch ( iBattleOfMayaState )
	{
		case KANTURU_MAYA_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_MAYA_STANBY1:
			this->SetState_STANBY1();
			break;
		case KANTURU_MAYA_NOTIFY1:
			this->SetState_NOTIFY1();
			break;
		case KANTURU_MAYA_START1:
			this->SetState_START1();
			break;
		case KANTURU_MAYA1:
			this->SetState_MAYA1();
			break;
		case KANTURU_MAYA_END1:
			this->SetState_END_MAYA1();
			break;
		case KANTURU_MAYA_ENDCYCLE1:
			this->SetState_ENDCYCLE_MAYA1();
			break;
		case KANTURU_MAYA_STANBY2:
			this->SetState_STANBY2();
			break;
		case KANTURU_MAYA_START2:
			this->SetState_START2();
			break;
		case KANTURU_MAYA2:
			this->SetState_MAYA2();
			break;
		case KANTURU_MAYA_END2:
			this->SetState_END_MAYA2();
			break;
		case KANTURU_MAYA_ENDCYCLE2:
			this->SetState_ENDCYCLE_MAYA2();
			break;
		case KANTURU_MAYA_STANBY3:
			this->SetState_STANBY3();
			break;
		case KANTURU_MAYA_START3:
			this->SetState_START3();
			break;
		case KANTURU_MAYA3:
			this->SetState_MAYA3();
			break;
		case KANTURU_MAYA_END3:
			this->SetState_END_MAYA3();
			break;
		case KANTURU_MAYA_ENDCYCLE3:
			this->SetState_ENDCYCLE_MAYA3();
			break;
		case KANTURU_MAYA_END:
			this->SetState_END();
			break;
		case KANTURU_MAYA_ENDCYCLE:
			this->SetState_ENDCYCLE();
			break;
	}

	KANTURU_UTIL.NotifyKanturuChangeState(KANTURU_STATE_BATTLE_OF_MAYA, this->m_iBattleOfMayaState);
}


void CKanturuBattleOfMaya::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;

	if ( iNextState >= KANTURU_MAYA_STATE_INFO )
	{
		iNextState = KANTURU_MAYA_NONE;
	}

	this->SetState(iNextState);
}


void CKanturuBattleOfMaya::SetState_NONE()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> NONE",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_NONE);
	this->SetEntrancePermit(FALSE);
}


void CKanturuBattleOfMaya::SetState_STANBY1()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY1",
		this->m_iBattleOfMayaState);
	LogAddTD("[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY1",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_STANBY1);
	g_KanturuBattleUserMng.ResetAllData();
	this->SetEntrancePermit(TRUE);
}


void CKanturuBattleOfMaya::SetState_NOTIFY1()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> NOTIFY1",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_NOTIFY1);
	this->SetEntrancePermit(FALSE);
}



void CKanturuBattleOfMaya::SetState_START1()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START1",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_START1);
	this->SetBattleSceneState(0);
	this->SetEntrancePermit(FALSE);
	this->SetSuccessValue(FALSE);
	this->SetSceneSuccessValue(FALSE);
	this->m_BattleTimeInfo[this->m_iSceneState].SetAppliedTime();

	if ( this->m_BattleTimeInfo[this->m_iSceneState].GetCondition() == 1 )
	{
		KANTURU_UTIL.NotifyKanturuBattleTime(this->m_BattleTimeInfo[this->m_iSceneState].GetValue());
	}

	g_KanturuMonsterMng.SetKanturuMonster(0);
	this->m_KanturuMaya.SetMayaObjIndex(g_KanturuMonsterMng.GetMayaObjIndex());
}


void CKanturuBattleOfMaya::SetState_MAYA1()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA1",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA1);
	this->SetEntrancePermit(FALSE);
	this->SetSuccessValue(FALSE);
	this->SetSceneSuccessValue(FALSE);
	this->m_iMayaHandDieTimeCounter = 0;
	g_KanturuMonsterMng.SetKanturuMonster(3);
	this->m_KanturuMaya.SetMayaObjIndex(g_KanturuMonsterMng.GetMayaObjIndex());
	this->SetAIMonsterGroup(this->m_iMayaHandGroupNumber[0]);
	this->m_iMayaLeftHandObjIndex = TMonsterAIGroup::FindGroupLeader(this->m_iMayaHandGroupNumber[0]);
}



void CKanturuBattleOfMaya::SetState_END_MAYA1()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA1",
		this->m_iBattleOfMayaState);

	this->SetBattleSceneState(-1);
	this->SetBattleOfMayaState(KANTURU_MAYA_END1);
	this->SetEntrancePermit(FALSE);
	
	KANTURU_UTIL.NotifyKanturuSuccessValue(this->GetSuccessValue());
}


void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA1()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA1",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE1);
	this->SetEntrancePermit(FALSE);
	
	KANTURU_UTIL.SendMsgKauturuMapUser(lMsg.Get(MSGGET(13, 33)));
}



void CKanturuBattleOfMaya::SetState_STANBY2()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY2",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_STANBY2);
	this->SetEntrancePermit(TRUE);
}



void CKanturuBattleOfMaya::SetState_START2()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START2",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_START2);
	this->SetEntrancePermit(FALSE);
	this->SetBattleSceneState(1);
	this->SetSuccessValue(FALSE);
	this->SetSceneSuccessValue(FALSE);
	this->m_BattleTimeInfo[this->m_iSceneState].SetAppliedTime();

	if ( this->m_BattleTimeInfo[this->m_iSceneState].GetCondition() == 1 )
	{
		KANTURU_UTIL.NotifyKanturuBattleTime(this->m_BattleTimeInfo[this->m_iSceneState].GetValue());
	}

	g_KanturuMonsterMng.SetKanturuMonster(0);
	this->m_KanturuMaya.SetMayaObjIndex(g_KanturuMonsterMng.GetMayaObjIndex());
}



void CKanturuBattleOfMaya::SetState_MAYA2()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA2",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA2);
	this->SetEntrancePermit(FALSE);
	this->SetSuccessValue(FALSE);
	this->SetSceneSuccessValue(FALSE);
	this->m_iMayaHandDieTimeCounter = 0;
	g_KanturuMonsterMng.SetKanturuMonster(4);
	this->m_KanturuMaya.SetMayaObjIndex(g_KanturuMonsterMng.GetMayaObjIndex());
	this->SetAIMonsterGroup(this->m_iMayaHandGroupNumber[1]);
	this->m_iMayaRightHandObjIndex = TMonsterAIGroup::FindGroupLeader(this->m_iMayaHandGroupNumber[1]);
}


void CKanturuBattleOfMaya::SetState_END_MAYA2()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA2",
		this->m_iBattleOfMayaState);

	this->SetBattleSceneState(-1);
	this->SetBattleOfMayaState(KANTURU_MAYA_END2);
	this->SetEntrancePermit(FALSE);
	
	KANTURU_UTIL.NotifyKanturuSuccessValue(this->GetSuccessValue());
}


void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA2()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA2",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE2);
	this->SetEntrancePermit(FALSE);
	
	KANTURU_UTIL.SendMsgKauturuMapUser(lMsg.Get(MSGGET(13, 33)));
}



void CKanturuBattleOfMaya::SetState_STANBY3()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY3",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_STANBY3);
	this->SetEntrancePermit(TRUE);
}



void CKanturuBattleOfMaya::SetState_START3()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START3",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_START3);
	this->SetBattleSceneState(2);
	this->SetEntrancePermit(FALSE);
	this->SetSuccessValue(FALSE);
	this->SetSceneSuccessValue(FALSE);
	this->m_BattleTimeInfo[this->m_iSceneState].SetAppliedTime();

	if ( this->m_BattleTimeInfo[this->m_iSceneState].GetCondition() == 1 )
	{
		KANTURU_UTIL.NotifyKanturuBattleTime(this->m_BattleTimeInfo[this->m_iSceneState].GetValue());
	}

	g_KanturuMonsterMng.SetKanturuMonster(1);
	this->m_KanturuMaya.SetMayaObjIndex(g_KanturuMonsterMng.GetMayaObjIndex());
}



void CKanturuBattleOfMaya::SetState_MAYA3()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA3",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA3);
	this->SetEntrancePermit(FALSE);
	this->SetSuccessValue(FALSE);
	this->SetSceneSuccessValue(FALSE);
	this->m_iMayaHandDieTimeCounter = 0;
	g_KanturuMonsterMng.SetKanturuMonster(5);
	this->m_KanturuMaya.SetMayaObjIndex(g_KanturuMonsterMng.GetMayaObjIndex());
	this->SetAIMonsterGroup(this->m_iMayaHandGroupNumber[2]);
	this->m_iMayaLeftHandObjIndex = TMonsterAIGroup::FindGroupLeader(this->m_iMayaHandGroupNumber[2]);
	this->m_iMayaRightHandObjIndex = TMonsterAIGroup::FindGroupMemberObjectIndex(this->m_iMayaHandGroupNumber[2], 1);
}


void CKanturuBattleOfMaya::SetState_END_MAYA3()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA3",
		this->m_iBattleOfMayaState);

	this->SetBattleSceneState(-1);
	this->SetBattleOfMayaState(KANTURU_MAYA_END3);
	this->SetEntrancePermit(FALSE);
	
	KANTURU_UTIL.NotifyKanturuSuccessValue(this->GetSuccessValue());
}


void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA3()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA3",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE3);
	this->SetEntrancePermit(FALSE);
}



void CKanturuBattleOfMaya::SetState_END()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END",
		this->m_iBattleOfMayaState);

	this->SetBattleSceneState(-1);
	this->SetBattleOfMayaState(KANTURU_MAYA_END);
	this->SetEntrancePermit(FALSE);

	if ( this->GetSuccessValue() == TRUE )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser(lMsg.Get(MSGGET(13, 34)));
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuSuccessValue(this->GetSuccessValue());
		KANTURU_UTIL.SendMsgKanturuBattleUser(lMsg.Get(MSGGET(13, 36)));
	}
	
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] Success Value[%d]",
		this->GetSuccessValue());
}



void CKanturuBattleOfMaya::SetState_ENDCYCLE()
{
	LogAddC(5, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE",
		this->m_iBattleOfMayaState);

	LogAddTD("[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE",
		this->m_iBattleOfMayaState);

	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE);
	this->SetEntrancePermit(FALSE);

	if ( this->GetSuccessValue() == FALSE )
	{
		g_KanturuBattleUserMng.MoveAllUser(137);
		g_KanturuBattleUserMng.ResetAllData();
	}
}




void CKanturuBattleOfMaya::ProcState_NONE()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_STANBY1()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_NOTIFY1()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_START1()
{
	if ( g_KanturuMonsterMng.IsExistAliveMonster() == FALSE )
	{
		this->SetSuccessValue(TRUE);

		LogAddTD("[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d",
			this->m_iBattleOfMayaState);

		this->SetNextState(this->m_iBattleOfMayaState);
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_IceStorm(this->m_iMayaIceStormUsingRate);
		KANTURU_UTIL.NotifyKanturuUserMonsterCount(g_KanturuMonsterMng.GetAliveMonsterCount(),
			g_KanturuBattleUserMng.GetUserCount());
	}
}


void CKanturuBattleOfMaya::ProcState_MAYA1()
{
	if(this->m_iMayaLeftHandObjIndex == -1)//Season 4.5 addon
	{
		return;
	}

	if ( gObj[this->m_iMayaLeftHandObjIndex].Live == FALSE )
	{
		if ( this->m_iMayaHandDieTimeCounter == 0 )
		{
			this->SetSuccessValue(TRUE);
			this->SetSceneSuccessValue(TRUE);

			LogAddTD("[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d",
				this->m_iBattleOfMayaState);

			g_KanturuBattleUserMng.LogBattleWinnerUserInfo(0,
				this->m_BattleTimeInfo[this->m_iSceneState].GetElapsedTime());

			KANTURU_UTIL.NotifyKanturuUserMonsterCount(0,
				g_KanturuBattleUserMng.GetUserCount());

			this->m_iMayaHandDieTimeCounter = GetTickCount();
		}

		if ( (GetTickCount() - this->m_iMayaHandDieTimeCounter ) >= 3000 )
		{
			this->SetNextState(this->m_iBattleOfMayaState);
		}
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuUserMonsterCount(1,
			g_KanturuBattleUserMng.GetUserCount());

		this->ChangeAI(this->m_iMayaHandGroupNumber[0]);
		this->m_KanturuMaya.KanturuMayaAct_Hands();
	}
}



void CKanturuBattleOfMaya::ProcState_END_MAYA1()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA1()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_STANBY2()
{
	return;
}



void CKanturuBattleOfMaya::ProcState_START2()
{
	if ( g_KanturuMonsterMng.IsExistAliveMonster() == FALSE )
	{
		this->SetSuccessValue(TRUE);

		LogAddTD("[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d",
			this->m_iBattleOfMayaState);

		this->SetNextState(this->m_iBattleOfMayaState);
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_IceStorm(this->m_iMayaIceStormUsingRate);
		KANTURU_UTIL.NotifyKanturuUserMonsterCount(g_KanturuMonsterMng.GetAliveMonsterCount(),
			g_KanturuBattleUserMng.GetUserCount());
	}
}



void CKanturuBattleOfMaya::ProcState_MAYA2()
{
	if(this->m_iMayaRightHandObjIndex == -1)//Season 4.5 addon
	{
		return;
	}

	if ( gObj[this->m_iMayaRightHandObjIndex].Live == FALSE )
	{
		if ( this->m_iMayaHandDieTimeCounter == 0 )
		{
			this->SetSuccessValue(TRUE);
			this->SetSceneSuccessValue(TRUE);

			LogAddTD("[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d",
				this->m_iBattleOfMayaState);

			g_KanturuBattleUserMng.LogBattleWinnerUserInfo(1,
				this->m_BattleTimeInfo[this->m_iSceneState].GetElapsedTime());

			KANTURU_UTIL.NotifyKanturuUserMonsterCount(0,
				g_KanturuBattleUserMng.GetUserCount());

			this->m_iMayaHandDieTimeCounter = GetTickCount();
		}

		if ( (GetTickCount() - this->m_iMayaHandDieTimeCounter ) >= 3000 )
		{
			this->SetNextState(this->m_iBattleOfMayaState);
		}
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuUserMonsterCount(1,
			g_KanturuBattleUserMng.GetUserCount());

		this->ChangeAI(this->m_iMayaHandGroupNumber[1]);
		this->m_KanturuMaya.KanturuMayaAct_Hands();
	}
}


void CKanturuBattleOfMaya::ProcState_END_MAYA2()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA2()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_STANBY3()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_START3()
{
	if ( g_KanturuMonsterMng.IsExistAliveMonster() == FALSE )
	{
		this->SetSuccessValue(TRUE);

		LogAddTD("[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d",
			this->m_iBattleOfMayaState);

		this->SetNextState(this->m_iBattleOfMayaState);
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_IceStorm(this->m_iMayaIceStormUsingRate);
		KANTURU_UTIL.NotifyKanturuUserMonsterCount(g_KanturuMonsterMng.GetAliveMonsterCount(),
			g_KanturuBattleUserMng.GetUserCount());
	}
}



void CKanturuBattleOfMaya::ProcState_MAYA3()
{
	int iAliveCount = 0;
	
	//Season 4.5 change
	if ( this->m_iMayaLeftHandObjIndex != -1 && gObj[this->m_iMayaLeftHandObjIndex].Live != FALSE )
	{
		iAliveCount++;
	}
	//Season 4.5 change
	if ( this->m_iMayaRightHandObjIndex != -1 && gObj[this->m_iMayaRightHandObjIndex].Live != FALSE )
	{
		iAliveCount++;
	}

	if ( iAliveCount == 0 )
	{
		if ( this->m_iMayaHandDieTimeCounter == 0 )
		{
			this->SetSuccessValue(TRUE);
			this->SetSceneSuccessValue(TRUE);

			LogAddTD("[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d",
				this->m_iBattleOfMayaState);

			g_KanturuBattleUserMng.LogBattleWinnerUserInfo(2,
				this->m_BattleTimeInfo[this->m_iSceneState].GetElapsedTime());

			this->m_iMayaHandDieTimeCounter = GetTickCount();
		}

		if ( (GetTickCount() - this->m_iMayaHandDieTimeCounter ) >= 3000 )
		{
			this->SetNextState(this->m_iBattleOfMayaState);
			return;
		}
	}

	KANTURU_UTIL.NotifyKanturuUserMonsterCount(iAliveCount,
		g_KanturuBattleUserMng.GetUserCount());

	this->ChangeAI(this->m_iMayaHandGroupNumber[2]);
	this->m_KanturuMaya.KanturuMayaAct_Hands();
}

void CKanturuBattleOfMaya::ProcState_END_MAYA3()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA3()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_END()
{
	return;
}


void CKanturuBattleOfMaya::ProcState_ENDCYCLE()
{
	return;
}


BOOL CKanturuBattleOfMaya::CheckStateTime()
{
	if ( this->GetBattleOfMayaState() )
	{
		if ( this->m_StateInfo[this->m_iBattleOfMayaState].GetCondition() == 1 )
		{
			if ( this->m_StateInfo[this->m_iBattleOfMayaState].IsTimeOut() == TRUE )
			{
				this->SetNextState(this->m_iBattleOfMayaState);
			}
		}
	}

	if ( this->GetBattleSceneState() != -1 )
	{
		if ( this->m_BattleTimeInfo[this->m_iSceneState].GetCondition() != 1 )
		{
			return TRUE;
		}

		if ( this->GetSceneSuccessValue() == TRUE )
		{
			return TRUE;
		}

		if ( this->m_BattleTimeInfo[this->m_iSceneState].IsTimeOut() == TRUE )
		{
			if ( this->GetBattleOfMayaState() != KANTURU_MAYA_END )
			{
				this->SetSceneSuccessValue(FALSE);
				this->SetSuccessValue(FALSE);

				LogAddTD("[ KANTURU ][ BattleOfMaya ] Fail!! TimeOut - DetailState:%d",
					this->m_iBattleOfMayaState);

				this->SetState(KANTURU_MAYA_END);
			}
		}
	}

	return TRUE;
}



int CKanturuBattleOfMaya::GetRemainTime()
{
	int iCurrentState = this->GetBattleOfMayaState();
	return this->m_StateInfo[iCurrentState].GetRemainTime();
}


void CKanturuBattleOfMaya::CheckMayaBattleUser()
{
	if ( this->GetBattleOfMayaState() == KANTURU_MAYA_NOTIFY1 ||
		 this->GetBattleOfMayaState() == KANTURU_MAYA_START1 ||
		 this->GetBattleOfMayaState() == KANTURU_MAYA1 ||
		 this->GetBattleOfMayaState() == KANTURU_MAYA_START2 ||
		 this->GetBattleOfMayaState() == KANTURU_MAYA2 ||
		 this->GetBattleOfMayaState() == KANTURU_MAYA_START3 ||
		 this->GetBattleOfMayaState() == KANTURU_MAYA3 )
	{
		if ( g_KanturuBattleUserMng.IsEmpty() == TRUE )
		{
			this->SetSuccessValue(FALSE);
			this->SetSceneSuccessValue(FALSE);

			LogAddTD("[ KANTURU ][ BattleOfMaya ] Fail!! All Battle User Die - DetailState:%d",
				this->m_iBattleOfMayaState);

			this->SetState(KANTURU_MAYA_END);
		}
	}
}



void CKanturuBattleOfMaya::SetAIMonsterGroup(int iGroupNumber)
{
	TMonsterAIGroup::Init(iGroupNumber);
	TMonsterAIGroup::ChangeAIOrder(iGroupNumber, 0);

	this->m_iMayaHandAIAppliedTime = GetTickCount();
}


void CKanturuBattleOfMaya::ResetAIMonster()
{
	if(this->m_iMayaLeftHandObjIndex != -1)//Season 4.5 addon
	{
		gObj[this->m_iMayaLeftHandObjIndex].Life = 0;
	}

	if(this->m_iMayaRightHandObjIndex != -1)//Season 4.5 addon
	{
		gObj[this->m_iMayaRightHandObjIndex].Life = 0;
	}

	for ( int iCount=0;iCount < KANTURU_MAYA_AI_CHANGE_TIME ; iCount++)
	{
		TMonsterAIGroup::DelGroupInstance(this->m_iMayaHandGroupNumber[iCount]);
	}

	this->m_iMayaLeftHandObjIndex = -1;
	this->m_iMayaRightHandObjIndex = -1;
	this->m_iMayaHandAIAppliedTime = 0;
	this->m_iMayaHandCurrentAI = 0;
}



void CKanturuBattleOfMaya::ChangeAI(int iGroupNumber)
{
	if ( (GetTickCount() - this->m_iMayaHandAIAppliedTime) >= this->m_iMayaHandAIChangeTime[this->m_iMayaHandCurrentAI] )
	{
		this->m_iMayaHandAIAppliedTime = GetTickCount();

		if ( this->m_iMayaHandCurrentAI != KANTURU_MAYA_GROUP_NUMBER )
		{
			this->m_iMayaHandCurrentAI++;
			TMonsterAIGroup::ChangeAIOrder(iGroupNumber, this->m_iMayaHandCurrentAI);
		}
	}
}

