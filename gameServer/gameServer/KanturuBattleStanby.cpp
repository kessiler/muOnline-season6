// KanturuBattleStanby.cpp: implementation of the CKanturuBattleStanby class.
//	GS-CS	1.00.90	JPN		-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuBattleStanby.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "KanturuBattleUserMng.h"

#include "Gamemain.h"

#include "LogProc.h"
#include "..\include\ReadScript.h"
#include "KanturuUtil.h"

static CKanturuUtil KANTURU_UTIL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//006067B0  GS_CS 90 - Identical
CKanturuBattleStanby::CKanturuBattleStanby():m_iBattleStanbyState(0),m_bIsSucccess(0),m_bFileDataLoad(0),m_StateInfoCount(0)
{
	this->ResetAllData();
	this->SetBattleStanbyState(KANTURU_STANBY_NONE);
}

CKanturuBattleStanby::~CKanturuBattleStanby()
{
	return;
}

void CKanturuBattleStanby::Init()
{
	return;
}

void CKanturuBattleStanby::ResetAllData()
{
	for ( int iCount=0;iCount < KANTURU_STANBY_STATE_INFO ; iCount++ )
	{
		this->m_StateInfo[iCount].ResetTimeInfo();
	}

	this->m_StateInfoCount = 0;
}

BOOL CKanturuBattleStanby::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[ KANTURU ][ BattleStanby ] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			DWORD dwLastError = GetLastError();

			MsgBox("[ KANTURU ][ BattleStanby ] - Can't Open %s ",
				lpszFileName);

			return FALSE;
		}

		SMDToken Token;
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
				if ( iType == 1 )
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

					if ( this->m_StateInfoCount < 0 || this->m_StateInfoCount >= KANTURU_STANBY_STATE_INFO )
					{
						MsgBox("[ KANTURU ][ BattleStanby ] - Exceed Max State Time (%d)",
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
				else
				{
					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}
				}

			}	// while
		}	// while

		fclose(SMDFile);

		LogAddC(2, "[ KANTURU ][ BattleStanby ] %s file is Loaded",
			lpszFileName);

		this->m_bFileDataLoad = TRUE;
	}	// __try
	catch (DWORD)
	{
		MsgBox("[ KANTURU ][ BattleStanby ] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}



void CKanturuBattleStanby::Run()
{
	this->CheckStateTime();

	switch ( this->m_iBattleStanbyState )
	{
		case KANTURU_STANBY_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_STANBY_START:
			this->ProcState_START();
			break;
		case KANTURU_STANBY_NOTIFY:
			this->ProcState_NOTIFY();
			break;
		case KANTURU_STANBY_END:
			this->ProcState_END();
			break;
	}
}



void CKanturuBattleStanby::SetState(int iBattleStanbyState)
{
	if ( this->m_StateInfo[iBattleStanbyState].GetCondition() > 0 )
	{
		this->m_StateInfo[iBattleStanbyState].SetAppliedTime();
	}

	g_KanturuMonsterMng.ResetRegenMonsterObjData();

//#if (_GSCS == 0)
	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		gObj[iCount].m_bKanturuEntranceByNPC = FALSE;
	}
//#endif

	g_KanturuBattleUserMng.ResetAllData();

	switch ( iBattleStanbyState )
	{
		case KANTURU_STANBY_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_STANBY_START:
			this->SetState_START();
			break;
		case KANTURU_STANBY_NOTIFY:
			this->SetState_NOTIFY();
			break;
		case KANTURU_STANBY_END:
			this->SetState_END();
			break;
	}

	KANTURU_UTIL.NotifyKanturuChangeState(KANTURU_STATE_BATTLE_STANTBY, this->m_iBattleStanbyState);
	g_KanturuBattleUserMng.ResetAllData();
}



void CKanturuBattleStanby::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;

	if ( iNextState >= KANTURU_STANBY_STATE_INFO )
	{
		iNextState = KANTURU_STANBY_NONE;
	}

	this->SetState(iNextState);
}



void CKanturuBattleStanby::SetState_NONE()
{
	LogAddC(5, "[ KANTURU ][ BattleStanby ] State(%d) -> NONE",
		this->m_iBattleStanbyState);

	this->SetBattleStanbyState(KANTURU_STANBY_NONE);
}



void CKanturuBattleStanby::SetState_START()
{
	LogAddC(5, "[ KANTURU ][ BattleStanby ] State(%d) -> START",
		this->m_iBattleStanbyState);

	this->SetBattleStanbyState(KANTURU_STANBY_START);
}


void CKanturuBattleStanby::SetState_NOTIFY()
{
	LogAddC(5, "[ KANTURU ][ BattleStanby ] State(%d) -> NOTIFY",
		this->m_iBattleStanbyState);

	this->SetBattleStanbyState(KANTURU_STANBY_NOTIFY);
	KANTURU_UTIL.SendMsgKauturuMapUser(lMsg.Get(MSGGET(13, 31)));
}


void CKanturuBattleStanby::SetState_END()
{
	LogAddC(5, "[ KANTURU ][ BattleStanby ] State(%d) -> END",
		this->m_iBattleStanbyState);

	this->SetBattleStanbyState(KANTURU_STANBY_END);
}


void CKanturuBattleStanby::ProcState_NONE()
{
	return;
}


void CKanturuBattleStanby::ProcState_START()
{
	return;
}


void CKanturuBattleStanby::ProcState_NOTIFY()
{
	return;
}


void CKanturuBattleStanby::ProcState_END()
{
	return;
}



void CKanturuBattleStanby::CheckStateTime()
{
	int iCurrentState = this->GetBattleStanbyState();

	if ( iCurrentState )
	{
		if ( this->m_StateInfo[iCurrentState].GetCondition() == 1 )
		{
			if ( this->m_StateInfo[iCurrentState].IsTimeOut() == TRUE )
			{
				this->SetNextState(iCurrentState);
			}
		}
	}
}



int CKanturuBattleStanby::GetRemainTime()
{
	int iCurrentState = this->GetBattleStanbyState();
	return this->m_StateInfo[iCurrentState].GetRemainTime();
}
