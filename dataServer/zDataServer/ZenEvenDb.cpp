// ZenEvenDb.cpp: implementation of the CZenEvenDb class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZenEvenDb.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZenEvenDb::CZenEvenDb()
{

}

CZenEvenDb::~CZenEvenDb()
{

}

void CZenEvenDb::Connect()
{
	if( m_DBQuery.Connect(3, gSettings.ConnectDNS, gSettings.ConnectLogin, gSettings.ConnectPassword) == FALSE )
	{
		//MsgBox("EventDBSet ODBC Connect Fail");
	}
}

BOOL CZenEvenDb::SetZen(char* AccountID, int& Zen)
{
	CString qSql;

	int TotalZen = Zen;

	qSql.Format("SELECT AccountID, Zen FROM ZenEvent WHERE AccountID='%s'", AccountID);

	if( m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if( m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();

		if(InsertAccount(AccountID, Zen) == FALSE)
		{
			Zen = 0;
			gWindow.PrintLog("[ZEN-EVENT]%s¥‘ %d Zen √ﬂ∞° Ω«∆–", AccountID, Zen);
			return FALSE;
		}

		gWindow.PrintLog("[ZEN-EVENT]%s¥‘ %d Zen Ω≈√ﬂ∞° √— = %d", AccountID, Zen, Zen+TotalZen);
		return TRUE;
	}

	TotalZen = m_DBQuery.GetInt("Zen");

	m_DBQuery.Clear();

	qSql.Format("UPDATE ZenEvent SET Zen=Zen+%d WHERE AccountID='%s'", Zen, AccountID);

	if( m_DBQuery.Exec(qSql) == FALSE)
	{
		return FALSE;
	}

	gWindow.PrintLog("[ZEN-EVENT](%s)¥‘ %d Zen √ﬂ∞° √— = %d", AccountID, Zen, Zen+TotalZen);

	Zen += TotalZen;

	return TRUE;
}

BOOL CZenEvenDb::GetZen(char* AccountID, int& Zen)
{
	CString qSql;

	qSql.Format("SELECT AccountID, Zen FROM ZenEvent WHERE AccountID='%s'", AccountID);

	if( m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if( m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	Zen = m_DBQuery.GetInt("Zen");

	if(Zen < 0)
	{
		Zen = 0;
	}

	m_DBQuery.Clear();

	return TRUE;
}

BOOL CZenEvenDb::InsertAccount(char* AccountID, int Zen)
{
	CString qSql;

	qSql.Format("INSERT INTO ZenEvent (AccountID, Zen) VALUES ('%s',%d)", AccountID, Zen);

	if( m_DBQuery.Exec(qSql) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}