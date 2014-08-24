// TMonsterAIRuleInfo.cpp: implementation of the TMonsterAIRuleInfo class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIRuleInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIRuleInfo::TMonsterAIRuleInfo()
{
	this->Reset();
}


void TMonsterAIRuleInfo::Reset()
{
	this->m_iRuleNumber = -1;
	this->m_iMonsterClass = -1;
	this->m_iMonsterAIUnit = 0;
	this->m_iRuleCondition = -1;
	this->m_iWaitTime = -1;
	this->m_iContinuanceTime = -1;
	this->m_iMonth = -1;
	this->m_iDay = -1;
	this->m_iWeekDay = -1;
	this->m_iHour = -1;
	this->m_iMinute = -1;
	this->m_iAppliedTime = -1;
	this->m_iApplyingTime = -1;
	memset(this->m_szRuleDesc, 0, sizeof(this->m_szRuleDesc));
}



BOOL TMonsterAIRuleInfo::IsValid()
{
	BOOL bIsValid = FALSE;

	switch ( this->m_iRuleCondition )
	{
		case MAR_CONDITION_SPECIFIC_DATE:
			bIsValid = this->CheckConditionSpecificDate();
			break;

		case MAR_CONDITION_CRYWOLF_START:
			bIsValid = this->CheckConditionCrywolfStart();
			break;

		case MAR_CONDITION_CRYWOLF_END:
			bIsValid = this->CheckConditionCrywolfEnd();
			break;
	}

	if ( !bIsValid )
	{
		this->m_iAppliedTime = -1;
		this->m_iApplyingTime = -1;

		return FALSE;
	}

	if ( this->m_iAppliedTime == -1 )
		this->m_iAppliedTime = GetTickCount();

	if ( (this->m_iAppliedTime + (this->m_iWaitTime * 1000)) > GetTickCount() )
		return FALSE;

	if ( (  this->m_iAppliedTime + this->m_iWaitTime * 1000 + this->m_iContinuanceTime * 1000) < GetTickCount() )
		return FALSE;

	return TRUE;
}



BOOL TMonsterAIRuleInfo::CheckConditionSpecificDate()
{
	CTime tCurrentTime = CTime::GetTickCount();
	CTimeSpan tValidRange(0, 0, 0, this->m_iWaitTime+this->m_iContinuanceTime);

	int iYear = tCurrentTime.GetYear();
	int iMonth = tCurrentTime.GetMonth();
	int iDay = tCurrentTime.GetDay();
	int iDayOfWeek = tCurrentTime.GetDayOfWeek();
	int iHour = tCurrentTime.GetHour();
	int iMinute = tCurrentTime.GetMinute();

	if ( this->m_iMonth != -1 )
	{
		if ( this->m_iMonth != iMonth )
		{
			CTime tConditionTime(iYear, this->m_iMonth, 0, 0, 0, 0, -1) ;

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}

	if ( this->m_iDay != -1 )
	{
		if ( this->m_iDay != iDay )
		{
			CTime tConditionTime(iYear, (this->m_iMonth == -1)?iMonth : this->m_iMonth , this->m_iDay, 0, 0, 0, -1);

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}

	if ( this->m_iWeekDay != -1 )
	{
		if ( this->m_iWeekDay != iDayOfWeek )
		{
			return FALSE;
		}
	}

	if ( this->m_iHour != -1 )
	{
		if ( this->m_iHour != iHour )
		{
			CTime tConditionTime(iYear, (this->m_iMonth == -1)?iMonth : this->m_iMonth,
				(this->m_iDay == -1)?iDay : this->m_iDay, this->m_iHour, 0, 0, -1) ;

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}

	if ( this->m_iMinute != -1 )
	{
		if ( this->m_iMinute != iMinute )
		{
			CTime tConditionTime(iYear, (this->m_iMonth == -1)?iMonth : this->m_iMonth,
										(this->m_iDay == -1)?iDay : this->m_iDay,
										(this->m_iHour == -1)?iHour : this->m_iHour,
										 this->m_iMinute, 0, -1) ;

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}	


BOOL TMonsterAIRuleInfo::CheckConditionCrywolfStart()
{
	return FALSE;
}


BOOL TMonsterAIRuleInfo::CheckConditionCrywolfEnd()
{
	return FALSE;
}