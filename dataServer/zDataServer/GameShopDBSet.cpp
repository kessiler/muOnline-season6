#include "stdafx.h"
#include "GameShopDBSet.h"
#include "winutil.h"
// -------------------------------------------------------------------------------

GameShopDBSet::GameShopDBSet()
{

}
// -------------------------------------------------------------------------------

GameShopDBSet::~GameShopDBSet()
{

}
// -------------------------------------------------------------------------------

bool GameShopDBSet::Connect()
{
	if( m_Query.Connect(3,gSettings.ConnectDNS,gSettings.ConnectLogin,gSettings.ConnectPassword) == FALSE )
	{
		//MsgBox("PeriodtemExDBSet ODBC Connect Fail");
		return FALSE;
	}

	return TRUE;
}
// -------------------------------------------------------------------------------

bool GameShopDBSet::SavePoint(char * AccountID, long WCoinP, long WCoinC, long GoblinPoint)
{
	CString Query;
	// ----
	Query.Format("UPDATE GameShopPoint SET WCoinC = %d, WCoinP = %d, GoblinPoint = %d WHERE AccountID = '%s'", 
		WCoinC, WCoinP, GoblinPoint, AccountID);
	// ----
	if( !m_Query.Exec(Query) )
	{
		return false;
	}
	// ----
	m_Query.Clear();
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool GameShopDBSet::ReqPoint(char * AccountID, GAMESHOP_DG_GET_POINT * Result)
{
	CString Query;
	// ----
	Query.Format("SELECT * FROM GameShopPoint WHERE AccountID = '%s'", AccountID);
	// ----
	if( !m_Query.Exec(Query) || m_Query.Fetch() == SQL_NO_DATA )
	{
		m_Query.Clear();
		// ----
		gWindow.PrintLog("[%s] Failed to select values [Next: INSERT]", __FUNCTION__);
		// ----
		CString InsertQuery;
		// ----
		InsertQuery.Format("INSERT INTO GameShopPoint (AccountID, WCoinC, WCoinP, GoblinPoint) VALUES ('%s', %d, %d, %d)", 
			AccountID, 0, 0, 0);
		// ----
		if( !m_Query.Exec(InsertQuery) )
		{
			gWindow.PrintLog("[%s] Failed to insert new values", __FUNCTION__);
			return false;
		}
		// ----
		m_Query.Clear();
	}
	else
	{
		Result->WCoinC		= m_Query.GetInt("WCoinC");
		Result->WCoinP		= m_Query.GetInt("WCoinP");
		Result->GoblinPoint	= m_Query.GetInt("GoblinPoint");
		m_Query.Clear();
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

void GameShopDBSet::InsertPoint(char * AccountID, long WCoinP, long WCoinC, long GoblinPoint)
{
	CString InsertQuery;
	// ----
	InsertQuery.Format("INSERT INTO GameShopPoint (AccountID, WCoinC, WCoinP, GoblinPoint) VALUES ('%s', %d, %d, %d)", 
		AccountID, 0, 0, 0);
	// ----
	if( !m_Query.Exec(InsertQuery) )
	{
		gWindow.PrintLog("[%s] Failed to insert new values", __FUNCTION__);
	}
	// ----
	m_Query.Clear();
}
// -------------------------------------------------------------------------------