// TMonsterSkillUnit.cpp: implementation of the TMonsterSkillUnit class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
// problebms:
//char * TMonsterSkillElement::GetElementName() - cant fix position
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterSkillUnit.h"
#include "..\include\ReadScript.h"
#include "LogProc.h"

TMonsterSkillUnit TMonsterSkillUnit::s_MonsterSkillUnitArray[MAX_MONSTER_SKILL_UNIT_ARRAY];
BOOL TMonsterSkillUnit::s_bDataLoad = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterSkillUnit::TMonsterSkillUnit()
{
	this->Reset();
}

TMonsterSkillUnit::~TMonsterSkillUnit()
{
	return;
}


void TMonsterSkillUnit::Reset()
{
	this->m_iUnitTargetType = -1;
	this->m_iUnitScopeType = -1;
	this->m_iUnitScopeValue = -1;
	this->m_iDelay = -1;
	this->m_iUnitNumber = -1;
	memset(this->m_szUnitName, 0, sizeof(this->m_szUnitName));
	this->m_iElementsCount = -1;

	for (int i=0;i<5;i++)
	{
		this->m_lpElementsSlot[i] = NULL;
	}
}





BOOL TMonsterSkillUnit::LoadData(LPSTR lpszFileName)
{
	TMonsterSkillUnit::s_bDataLoad = FALSE;

	if ( lpszFileName == NULL || strcmp(lpszFileName, "") == 0 )
	{
		MsgBox("[Monster Skill Unit] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;	
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster Skill Unit] - Can't Open %s ", lpszFileName);
			return FALSE;
		}


		TMonsterSkillUnit::DelAllSkillUnit();
		int iType = -1;

		while ( true )
		{
			Token = (SMDToken)GetToken();

			if ( Token == END )
				break;

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					char szUnitName[20] = {0};
					int iUnitNumber = -1;
					int iUnitTargetType = -1;
					int iUnitScopeType = -1;
					int iUnitScopeValue = -1;
					int iDelay = -1;
					int iElementCount = 0;
					int iElementsSlot[5] = { -1};
	
					Token = (SMDToken)GetToken();

					if ( strcmp("end", TokenString) == 0 )
						break;

					iUnitNumber = TokenNumber;

					Token = (SMDToken)GetToken();
					memcpy(szUnitName, TokenString, sizeof(szUnitName));

					Token = (SMDToken)GetToken();
					iUnitTargetType = TokenNumber;

					Token = (SMDToken)GetToken();
					iUnitScopeType = TokenNumber;

					Token = (SMDToken)GetToken();
					iUnitScopeValue = TokenNumber;

					Token = (SMDToken)GetToken();
					iDelay = TokenNumber;

					for ( int i=0;i<5;i++)
					{
						Token = (SMDToken)GetToken();
						iElementsSlot[i] = TokenNumber;
					}

					if ( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_SKILL_UNIT_ARRAY )
					{
						MsgBox("[Monster SkillUnit] - Unit(%d) Error (%s) File. "
							,iUnitNumber , lpszFileName);
						continue;
					}

					char szMsg[200] = {0};

					wsprintf(szMsg, "¡ÚNum:%d,Name:%s,TType:%d,SType:%d,SValue:%d,Delay:%d, E0:%d,E1:%d,E2:%d,E3:%d,E4:%d\n",
						iUnitNumber, szUnitName, iUnitTargetType, iUnitScopeType, iUnitScopeValue, iDelay,
						iElementsSlot[0], iElementsSlot[1], iElementsSlot[2], iElementsSlot[3], iElementsSlot[4]);

					OutputDebugString(szMsg);

					TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_iUnitNumber = iUnitNumber;
					TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_iUnitTargetType = iUnitTargetType;
					TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_iUnitScopeType = iUnitScopeType;
					TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_iUnitScopeValue = iUnitScopeValue;
					TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_iDelay = iDelay;
					memcpy(TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_szUnitName, szUnitName,
						sizeof(TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_szUnitName));

					for(int i=0;i<5;i++)
					{
						int iElement = iElementsSlot[i];

						if ( iElement != -1 )
						{
							TMonsterSkillElement * pMonsterSkillElement = TMonsterSkillElement::FindSkillElement(iElement);

							if ( pMonsterSkillElement )
							{
								iElementCount++;
								TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_lpElementsSlot[i] = pMonsterSkillElement;

								//LogAddC(4, "[Monster Skill Unit] - Found SkillElement (%d)(%s)",
								//	iElement, pMonsterSkillElement->GetElementName());
							}
							else
							{
								LogAddC(2, "[Monster Skill Unit] - Not Found SkillElement (%d)",
									iElement);	
							}
						}
					}

					TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_iElementsCount = iElementCount;
				}
			}
		
		}

		fclose(SMDFile);
		LogAddC(2,"[Monster Skill Unit] - %s file is Loaded", lpszFileName);
		TMonsterSkillUnit::s_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		MsgBox("[Monster Skill Unit] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}
//005E3DE8  |.  E8 01E9E1FF   |CALL 004026EE



BOOL TMonsterSkillUnit::DelAllSkillUnit()
{
	for(int i=0;i<MAX_MONSTER_SKILL_UNIT_ARRAY;i++)
	{
		TMonsterSkillUnit::s_MonsterSkillUnitArray[i].Reset();
	}

	return FALSE;
}



TMonsterSkillUnit * TMonsterSkillUnit::FindSkillUnit(int iUnitNumber)
{
	if ( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_SKILL_UNIT_ARRAY )
	{
		LogAddTD("[Monster SkillUnit] FindSkillUnit() Error - (UnitNumber=%d) ",
			iUnitNumber);

		return NULL;
	}

	if ( TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber].m_iUnitNumber == iUnitNumber )
	{
		return &TMonsterSkillUnit::s_MonsterSkillUnitArray[iUnitNumber];
	}

	LogAddTD("[Monster SkillUnit] FindSkillUnit() Error - (UnitNumber=%d) ",
		iUnitNumber);

	return FALSE;
}

void TMonsterSkillUnit::RunSkill(int iIndex, int iTargetIndex)
{
	char szElementsName[5][20] = {0};

	for ( int i=0;i<this->m_iElementsCount;i++)
	{
		if ( this->m_lpElementsSlot[i] == NULL )
		{
			LogAddC(2, "[Monster SkillUnit] SkillElement is NULL");
			continue;
		}

		this->m_lpElementsSlot[i]->ForceSkillElement(iIndex, iTargetIndex);
	}
}


char * TMonsterSkillElement::GetElementName()//I cant fix position bcoz its appear between files
{
	return this->m_szElementName;
}