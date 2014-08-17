// PeriodItemDBSet.cpp: implementation of the CPeriodItemDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PeriodItemDBSet.h"
#include "winutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPeriodItemDBSet::CPeriodItemDBSet()
{

}

CPeriodItemDBSet::~CPeriodItemDBSet()
{

}

BOOL CPeriodItemDBSet::Connect()
{
	if( m_Query.Connect(3, gSettings.ConnectDNS, gSettings.ConnectLogin, gSettings.ConnectPassword) == FALSE )
	{
		//MsgBox("PeriodtemExDBSet ODBC Connect Fail");
		return FALSE;
	}

	return TRUE;
}

int CPeriodItemDBSet::Insert(int iUserGUID, char* pchCharacterName, int iItemCode, BYTE btEffectCategory, BYTE btEffectType1, BYTE btEffectType2, DWORD dwDuration, char * chExpireDate)
{
	int iReturnCode = 0;

	CString QuerySql;

	QuerySql.Format("EXEC WZ_PeriodItemInsert %d, '%s', %d, %d, %d, %d, %d, '%s'",
		iUserGUID, pchCharacterName, iItemCode, btEffectCategory, btEffectType1, btEffectType2, dwDuration, chExpireDate);

	if( m_Query.Exec(QuerySql) == FALSE)
	{
		m_Query.Clear();
		return -1;
	}

	if( m_Query.Fetch() != SQL_NO_DATA)
	{
		iReturnCode = m_Query.GetInt(1);
	}
	else
	{
		m_Query.Clear();
		return -1;
	}

	m_Query.Clear();

	return iReturnCode;
}

int CPeriodItemDBSet::Delete(int iUserGUID, char* pchCharacterName)
{
	CString QuerySql;

	if(SQLSyntexCheck(pchCharacterName) == FALSE)
	{
		return 2;
	}

	QuerySql.Format("EXEC WZ_PeriodItemDelete %d, '%s'", iUserGUID, pchCharacterName);

	if( m_Query.Exec(QuerySql) == FALSE)
	{
		m_Query.Clear();
		return 2;
	}

	if( m_Query.Fetch() == SQL_NO_DATA)
	{
		m_Query.Clear();
		return 1;
	}

	m_Query.Clear();

	return 0;
}

int CPeriodItemDBSet::Select(LPITEMPERIOD_INFO lpPeriodItemList)
{
	int iReturnCode = 0;
	int iItemCount = 0;

	CString QuerySql;

	QuerySql.Format("EXEC WZ_PeriodItemSelect %d, '%s'", lpPeriodItemList->iUserGuid, lpPeriodItemList->chCharacterName);

	if( m_Query.Exec(QuerySql) == FALSE)
	{
		m_Query.Clear();
		return -3;
	}

	short i = m_Query.Fetch();

	while(i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		iReturnCode = m_Query.GetInt(1);

		if(iReturnCode < 0)
		{
			break;
		}

		lpPeriodItemList->DateInfo[iItemCount].btUsedInfo = 1;
		lpPeriodItemList->DateInfo[iItemCount].iItemCode = m_Query.GetInt("ItemCode");
		lpPeriodItemList->DateInfo[iItemCount].btEffectType1 = m_Query.GetInt("EffectType1");
		lpPeriodItemList->DateInfo[iItemCount].btEffectType2 = m_Query.GetInt("EffectType2");
		lpPeriodItemList->DateInfo[iItemCount].btEffectCategory = m_Query.GetInt("OptionType");
		lpPeriodItemList->DateInfo[iItemCount].dwItemUsedTime = m_Query.GetInt("UsedMinutes");
		lpPeriodItemList->DateInfo[iItemCount].dwItemLeftTime = m_Query.GetInt("LeftMinutes");
		iItemCount++;

		if(iItemCount >= 10)
		{
			gWindow.PrintLog("[PeriodItem][Select] Period Item List Count Over %d, User Id : %s(%d)", 10, lpPeriodItemList->chCharacterName, lpPeriodItemList->iUserGuid);
			break;
		}

		i = m_Query.Fetch();
	}

	m_Query.Clear();
	lpPeriodItemList->btDateInfoUseCount = iItemCount;

	return iReturnCode;
}

int CPeriodItemDBSet::Update(int iUserGUID, char* pchCharacterName, int iItemCode, DWORD dwDuration, int iLeftTime)
{
	int iReturnCode = 0;

	CString QuerySql;

	QuerySql.Format("EXEC WZ_PeriodItemUpdate %d, '%s', %d, %d, %d",
		iUserGUID, pchCharacterName, iItemCode, dwDuration, iLeftTime);

	if( m_Query.Exec(QuerySql) == FALSE)
	{
		m_Query.Clear();
		return -1;
	}

	if( m_Query.Fetch() != SQL_NO_DATA)
	{
		iReturnCode = m_Query.GetInt(1);
	}
	else
	{
		m_Query.Clear();
		return -1;
	}

	m_Query.Clear();

	return iReturnCode;
}
