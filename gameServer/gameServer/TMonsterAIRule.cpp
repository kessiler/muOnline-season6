// TMonsterAIRule.cpp: implementation of the TMonsterAIRule class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIRule.h"


#include "..\include\ReadScript.h"
#include "LogProc.h"



BOOL TMonsterAIRule::s_bDataLoad = FALSE;
int TMonsterAIRule::s_iMonsterCurrentAIUnitTable[MAX_MONSTER_AI_RULE_TABLE];
TMonsterAIRuleInfo TMonsterAIRule::s_MonsterAIRuleInfoArray[MAX_MONSTER_AI_RULE_INFO];
int TMonsterAIRule::s_iMonsterAIRuleInfoCount;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIRule::TMonsterAIRule()
{
	this->DelAllAIRule();
}

TMonsterAIRule::~TMonsterAIRule()
{
	return;
}




BOOL TMonsterAIRule::LoadData(LPSTR lpszFileName)
{
	TMonsterAIRule::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Monster AI Rule] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster AI Rule] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		TMonsterAIRule::DelAllAIRule();
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
					int iAIRuleNumber = -1;
					int iMonsterClass = -1;
					TMonsterAIRuleInfo MonsterAIRuleInfo;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iAIRuleNumber = TokenNumber;

					Token = GetToken();
					iMonsterClass = TokenNumber;

					Token = GetToken();
					memcpy(MonsterAIRuleInfo.m_szRuleDesc, TokenString, sizeof(MonsterAIRuleInfo.m_szRuleDesc));

					Token = GetToken();
					MonsterAIRuleInfo.m_iMonsterAIUnit = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iRuleCondition = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iWaitTime = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iContinuanceTime = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iMonth = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iDay = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iWeekDay = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iHour = TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iMinute = TokenNumber;

					if ( iAIRuleNumber  < 0 || iAIRuleNumber >= MAX_MONSTER_AI_RULE_INFO )
					{
						MsgBox("[Monster AI Rule] - AIRuleNumber(%d) Error (%s) File. ",
							iAIRuleNumber, lpszFileName);

						continue;
					}

					if ( iMonsterClass  < 0 || iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
					{
						MsgBox("[Monster AI Rule] - MonsterClass(%d) Error (%s) File. ",
							iMonsterClass, lpszFileName);

						continue;
					}

					MonsterAIRuleInfo.m_iRuleNumber = iAIRuleNumber;
					MonsterAIRuleInfo.m_iMonsterClass = iMonsterClass;

					memcpy(&TMonsterAIRule::s_MonsterAIRuleInfoArray[TMonsterAIRule::s_iMonsterAIRuleInfoCount], 
						&MonsterAIRuleInfo,
						sizeof(MonsterAIRuleInfo));

					TMonsterAIRule::s_iMonsterAIRuleInfoCount++;
				}
			}
		}

		fclose(SMDFile);

		LogAddC(2, "[Monster AI Rule ] - %s file is Loaded", lpszFileName);

		TMonsterAIRule::s_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		MsgBox("[Monster AI Rule] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}



BOOL TMonsterAIRule::DelAllAIRule()
{
	for ( int i=0;i<MAX_MONSTER_AI_RULE_INFO;i++)
	{
		TMonsterAIRule::s_MonsterAIRuleInfoArray[i].Reset();
	}

	for ( int j=0;j<MAX_MONSTER_AI_RULE_TABLE;j++)
	{
		TMonsterAIRule::s_iMonsterCurrentAIUnitTable[j] = 0;
	}

	TMonsterAIRule::s_iMonsterAIRuleInfoCount = 0;

	return FALSE;
}


int TMonsterAIRule::GetCurrentAIUnit(int iMonsterClass)
{
	if ( iMonsterClass < 0 || iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
	{
		LogAddTD("[Monster AI Rule] GetCurrentAIUnit() Error - (MonsterClass=%d) ", iMonsterClass);
		return 0;
	}

	return TMonsterAIRule::s_iMonsterCurrentAIUnitTable[iMonsterClass];
}




void TMonsterAIRule::MonsterAIRuleProc()
{
	int iChangedMonsterClassList[MAX_MONSTER_AI_RULE_INFO] = {-1};
	int iChangedAIUnitList[MAX_MONSTER_AI_RULE_INFO] = {0};
	int iChangedListCount = 0;
	
	for (int i=0;i<TMonsterAIRule::s_iMonsterAIRuleInfoCount;i++)
	{
		TMonsterAIRuleInfo & RuleInfo = TMonsterAIRule::s_MonsterAIRuleInfoArray[i];

		if ( RuleInfo.m_iMonsterClass < 0 || RuleInfo.m_iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
			return;

		if ( RuleInfo.IsValid() )
		{
			iChangedMonsterClassList[iChangedListCount] = RuleInfo.m_iMonsterClass;
			iChangedAIUnitList[iChangedListCount] = RuleInfo.m_iMonsterAIUnit;
			iChangedListCount++;
		}
		else
		{
			TMonsterAIRule::s_iMonsterCurrentAIUnitTable[RuleInfo.m_iMonsterClass] = 0;
		}
	}

	for(int j=0;j<iChangedListCount;j++)
	{
		TMonsterAIRule::s_iMonsterCurrentAIUnitTable[iChangedMonsterClassList[j]] = iChangedAIUnitList[j];
	}
}
