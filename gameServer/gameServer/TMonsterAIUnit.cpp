// TMonsterAIUnit.cpp: implementation of the TMonsterAIUnit class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIUnit.h"


#include "..\include\ReadScript.h"
#include "LogProc.h"


BOOL TMonsterAIUnit::s_bDataLoad = FALSE;
TMonsterAIUnit TMonsterAIUnit::s_MonsterAIUnitArray[MAX_MONSTER_AI_UNIT];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIUnit::TMonsterAIUnit()
{
	this->Reset();
}

TMonsterAIUnit::~TMonsterAIUnit()
{
	return;
}


void TMonsterAIUnit::Reset()
{
	this->m_iUnitNumber = -1;
	this->m_iDelayTime = NULL;
	this->m_lpAutomata = NULL;
	this->m_lpAIClassNormal = NULL;
	this->m_lpAIClassMove = NULL;
	this->m_lpAIClassAttack = NULL;
	this->m_lpAIClassHeal = NULL;
	this->m_lpAIClassAvoid = NULL;
	this->m_lpAIClassHelp = NULL;
	this->m_lpAIClassSpecial = NULL;
	this->m_lpAIClassEvent = NULL;
	memset(this->m_szUnitName, 0, sizeof(this->m_szUnitName));
	this->m_lppAIClassMap[0] = &this->m_lpAIClassNormal;
	this->m_lppAIClassMap[1] = &this->m_lpAIClassMove;
	this->m_lppAIClassMap[2] = &this->m_lpAIClassAttack;
	this->m_lppAIClassMap[3] = &this->m_lpAIClassHeal;
	this->m_lppAIClassMap[4] = &this->m_lpAIClassAvoid;
	this->m_lppAIClassMap[5] = &this->m_lpAIClassHelp;
	this->m_lppAIClassMap[6] = &this->m_lpAIClassSpecial;
	this->m_lppAIClassMap[7] = &this->m_lpAIClassEvent;
}

BOOL TMonsterAIUnit::LoadData(LPSTR lpszFileName)
{
	TMonsterAIUnit::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Monster AI Unit] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster AI Unit] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		TMonsterAIUnit::DelAllAIUnit();
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
					char szUnitName[50]={0};
					int iUnitNumber = -1;
					int iDelayTime = 0;
					int iAutomata = -1;
					int iAIClassNormal = -1;
					int iAIClassMove = -1;
					int iAIClassAttack = -1;
					int iAIClassHeal = -1;
					int iAIClassAvoid = -1;
					int iAIClassHelp = -1;
					int iAIClassSpecial = -1;
					int iAIClassEvent = -1;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iUnitNumber = TokenNumber;

					Token = GetToken();
					memcpy(szUnitName, TokenString, 20);

					Token = GetToken();
					iDelayTime = TokenNumber;

					Token = GetToken();
					iAutomata = TokenNumber;

					Token = GetToken();
					iAIClassNormal = TokenNumber;

					Token = GetToken();
					iAIClassMove = TokenNumber;

					Token = GetToken();
					iAIClassAttack = TokenNumber;

					Token = GetToken();
					iAIClassHeal = TokenNumber;

					Token = GetToken();
					iAIClassAvoid = TokenNumber;

					Token = GetToken();
					iAIClassHelp = TokenNumber;

					Token = GetToken();
					iAIClassSpecial = TokenNumber;

					Token = GetToken();
					iAIClassEvent = TokenNumber;

					if ( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_AI_UNIT )
					{
						MsgBox("[Monster AI Unit] - UnitNumber(%d) Error (%s) File. ", 
							iUnitNumber, lpszFileName);
						continue;
					}

					if ( iAutomata < 0 || iAutomata >= MAX_MONSTER_AI_AUTOMATA )
					{
						MsgBox("[Monster AI Unit] - AutomatNumber(%d) Error (%s) File. ", 
							iAutomata, lpszFileName);
						continue;
					}

					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_iUnitNumber = iUnitNumber;
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_iDelayTime = iDelayTime;
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAutomata = TMonsterAIAutomata::FindAutomata(iAutomata);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassNormal = TMonsterAIElement::FindAIElement(iAIClassNormal);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassMove = TMonsterAIElement::FindAIElement(iAIClassMove);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassAttack = TMonsterAIElement::FindAIElement(iAIClassAttack);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassHeal = TMonsterAIElement::FindAIElement(iAIClassHeal);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassAvoid = TMonsterAIElement::FindAIElement(iAIClassAvoid);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassHelp = TMonsterAIElement::FindAIElement(iAIClassHelp);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassSpecial = TMonsterAIElement::FindAIElement(iAIClassSpecial);
					TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassEvent = TMonsterAIElement::FindAIElement(iAIClassEvent);
					memcpy(TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_szUnitName,
						szUnitName, sizeof(TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_szUnitName));
				}
			}
		}

		fclose(SMDFile);

		LogAddC(2, "[Monster AI Unit] - %s file is Loaded", lpszFileName);

		TMonsterAIUnit::s_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		MsgBox("[Monster AI Unit] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}


BOOL TMonsterAIUnit::DelAllAIUnit()
{
	for (int i=0;i<MAX_MONSTER_AI_UNIT;i++)
	{
		TMonsterAIUnit::s_MonsterAIUnitArray[i].Reset();
	}

	return FALSE;
}


TMonsterAIUnit * TMonsterAIUnit::FindAIUnit(int iUnitNumber)
{
	if ( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_AI_UNIT )
	{
		LogAddTD("[Monster AI Unit] FindAIUnit() Error - (UnitNumber=%d) ",
			iUnitNumber);

		return NULL;
	}

	if ( TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber].m_iUnitNumber == iUnitNumber )
	{
		return &TMonsterAIUnit::s_MonsterAIUnitArray[iUnitNumber];
	}

	LogAddTD("[Monster AI Unit] FindAIUnit() Error - (UnitNumber=%d) ",
		iUnitNumber);

	return NULL;
}



BOOL TMonsterAIUnit::RunAIUnit(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( this->m_lpAutomata == NULL )
		return FALSE;

	if ( (GetTickCount() - lpObj->m_iLastAutomataRuntime) < lpObj->m_iLastAutomataDelay )
		return FALSE;

	TMonsterAIState * pAIState = this->m_lpAutomata->RunAutomata(iIndex);

	if ( pAIState == NULL )
		return FALSE;

	lpObj->m_iLastAutomataRuntime = GetTickCount();
	TMonsterAIElement * pAIElement = *this->m_lppAIClassMap[pAIState->m_iNextState];

	if ( pAIElement == NULL )
		return FALSE;

	int iRetExec = pAIElement->ForceAIElement(iIndex, 0, pAIState);

	if ( iRetExec == 0 )
		return FALSE;

	lpObj->m_iCurrentAIState = pAIState->m_iNextState;
	
	return TRUE;
}
