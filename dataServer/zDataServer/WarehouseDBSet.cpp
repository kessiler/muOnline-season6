// WarehouseDBSet.cpp: implementation of the CWarehouseDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WarehouseDBSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarehouseDBSet::CWarehouseDBSet()
{

}

CWarehouseDBSet::~CWarehouseDBSet()
{

}

BOOL CWarehouseDBSet::Connect()
{
	if( m_DBQuery.Connect(3, gSettings.ConnectDNS, gSettings.ConnectLogin, gSettings.ConnectPassword) == FALSE )
	{
		gWindow.PrintLog("Warehouse database connect fail.");
		return FALSE;
	}

	return TRUE;
}
BOOL CWarehouseDBSet::Get(char* AccountID)
{
	CString qSQL;
	
	char buffer[50];

	qSQL.Format("SELECT AccountID FROM warehouse WHERE AccountID='%s'", AccountID);
	
	if (m_DBQuery.ExecGetStr(qSQL,buffer) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CWarehouseDBSet::Create(char* AccountID)
{
	CString qSQL;
	
	qSQL.Format("INSERT INTO warehouse (AccountID, Money, EndUseDate, DbVersion) VALUES ('%s',0, getdate(), 3)", AccountID);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CWarehouseDBSet::Save(char* AccountID, BYTE* itembuf, int money, int pw)
{
	CString qSQL;
	
	qSQL.Format("UPDATE warehouse SET Money=%d, pw=%d, DbVersion=3 where AccountID='%s'", money, pw, AccountID);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		gWindow.PrintLog("Money Set error [%d] %s",money,AccountID);
	}

	qSQL.Format("UPDATE warehouse SET Items=? where AccountID='%s'", AccountID);
	
	m_DBQuery.WriteBlob(qSQL, itembuf, MAX_WAREHOUSEDBSIZE);

	return TRUE;
}

BOOL CWarehouseDBSet::Load(char* AccountID, BYTE* itembuf, int& money, BYTE& _dbversion, short& pw)
{
	CString qSQL;

	int dbversion = 0;
	
	qSQL.Format("SELECT Money, DbVersion, pw from warehouse where AccountID='%s'", AccountID);
	
	if (m_DBQuery.Exec(qSQL) == TRUE)
	{
		if (m_DBQuery.Fetch() != SQL_NO_DATA)
		{
			money = m_DBQuery.GetInt("Money");
			
			if (money < 0)
			{
				money = 0;
			}
			
			dbversion = m_DBQuery.GetInt("DbVersion");
			
			if (dbversion < 0)
			{
				dbversion = 0;
			}
			
			pw = m_DBQuery.GetInt("pw");
			
			if (pw == NULL)
			{
				pw = 0;
			}
		}
		else
		{
			money = 0;
			gWindow.PrintLog("error-L2 : get money error %s", AccountID);
		}

		m_DBQuery.Clear();
	}
	else
	{
		money = 0;
		gWindow.PrintLog("error-L2 : [%s] %s", AccountID, qSQL);
	}

	_dbversion = dbversion;
	
	qSQL.Format("SELECT Items from warehouse where AccountID='%s'", AccountID);
	
	int nRet = m_DBQuery.ReadBlob(qSQL,itembuf);

	if (nRet == 0)
	{
		for (int i = 0; i < MAX_WAREHOUSEDBSIZE; i++)
		{
			itembuf[i] = 0xFF;
		}
	}
	else if (nRet == -1)
	{
		return FALSE;
	}
	else if (nRet > 0 && nRet < MAX_WAREHOUSEDBSIZE)
	{
		for (int i = nRet; i < MAX_WAREHOUSEDBSIZE; i++)
		{
			itembuf[i] = 0xFF;
		}
	}

	return TRUE;
}

BOOL CWarehouseDBSet::MoneyUpdate(char* AccountID, int money)
{
	CString qSQL;
	
	qSQL.Format("UPDATE warehouse SET Money=%d where AccountID='%s'", money, AccountID);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{	
		gWindow.PrintLog("Money Set error [%d] %s", money, AccountID);
	}

	return TRUE;
}