// CrywolfStateTimeInfo.h: interface for the CCrywolfStateTimeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_)
#define AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrywolfStateTimeInfo
{
public:
	CCrywolfStateTimeInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_bUsed = FALSE;
		this->m_iStateNumber = -1;	
		this->m_iYear = -1;	
		this->m_iMonth = -1;	
		this->m_iDay = -1;	
		this->m_iDayOfWeek = -1;	
		this->m_iHour = -1;	
		this->m_iMinute = -1;	
		this->m_iSecond = -1;	
		this->m_iContinuanceTime = 0;	
		this->m_dwAppliedTickCount = 0;	
	}

	void SetAppliedTime()
	{
		this->m_dwAppliedTickCount = GetTickCount();
	}

	void ResetAppliedTime()
	{
		this->m_dwAppliedTickCount = 0;
	}

	BOOL CheckContinuanceTime()
	{
		if ( (GetTickCount() - this->m_dwAppliedTickCount ) > this->m_iContinuanceTime )
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL CheckScheduleTime()
	{
		CTime tCurrentTime = CTime::GetTickCount();

		int iYear = tCurrentTime.GetYear();
		int iMonth = tCurrentTime.GetMonth();
		int iDay = tCurrentTime.GetDay();
		int iDayOfWeek = tCurrentTime.GetDayOfWeek();
		int iHour = tCurrentTime.GetHour();
		int iMinute = tCurrentTime.GetMinute();

		int iCrywolfState = -1;

		CTimeSpan tTimeRange(0,0,this->m_iContinuanceTime,0);

		if(m_iMonth != -1 && m_iMonth != iMonth)
		{
			CTime tCheckTime(iYear,m_iMonth,0,0,0,0,-1);

			if(tCurrentTime < tCheckTime || tCurrentTime > (tCheckTime+tTimeRange))
			{
				return FALSE;
			}
		}

		if(m_iDay != -1 && m_iDay != iDay)
		{
			CTime tCheckTime(iYear,(m_iMonth == -1 ? iMonth : m_iMonth),m_iDay,0,0,0,-1);
			
			if(tCurrentTime < tCheckTime || tCurrentTime > (tCheckTime+tTimeRange))
			{
				return FALSE;
			}
		}

		if(m_iDayOfWeek != -1 && m_iDayOfWeek != iDayOfWeek)
			return FALSE;

		if(m_iHour != -1 && m_iHour != iHour)
		{
			CTime tCheckTime(iYear,(m_iMonth == -1 ? iMonth : m_iMonth),(m_iDay == -1 ? iDay : m_iDay),m_iHour,0,0,-1);

			if(tCurrentTime < tCheckTime || tCurrentTime > (tCheckTime+tTimeRange))
			{
				return FALSE;
			}
		}

		if(m_iMinute != -1 && m_iMinute != iMinute)
		{
			CTime tCheckTime(iYear,(m_iMonth == -1 ? iMonth : m_iMonth),(m_iDay == -1 ? iDay : m_iDay),(m_iHour == -1 ? iHour : m_iHour),m_iMinute,0,-1);

			if(tCurrentTime < tCheckTime || tCurrentTime > (tCheckTime+tTimeRange))
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	int GetLeftTime()
	{
		if ( this->CheckContinuanceTime() )
		{
			int iLeftTime = this->m_iContinuanceTime - (GetTickCount() - this->m_dwAppliedTickCount);

			if ( iLeftTime < 0 )
			{
				iLeftTime = 0;
			}

			return iLeftTime;
		}

		return 0;
	}

public:

	BOOL m_bUsed;	// 0
	int m_iStateNumber;	// 4
	int m_iYear;	// 8
	int m_iMonth;	// C
	int m_iDay;	// 10
	int m_iDayOfWeek;	// 14
	int m_iHour;	// 18
	int m_iMinute;	// 1C
	int m_iSecond;	// 20
	int m_iContinuanceTime;	// 24
	DWORD m_dwAppliedTickCount;	// 28

};

#endif // !defined(AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_)