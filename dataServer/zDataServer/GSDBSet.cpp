// GSDBSet.cpp: implementation of the CGSDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GSDBSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGSDBSet::CGSDBSet()
{

}

CGSDBSet::~CGSDBSet()
{

}

BOOL CGSDBSet::Connect()
{
	if( m_Query.Connect(3, gSettings.ConnectDNS, gSettings.ConnectLogin, gSettings.ConnectPassword) == FALSE )
	{
		//MsgBox("GSDBSet ODBC Connect Fail");
		return FALSE;
	}
	return TRUE;
}

DWORD CGSDBSet::GetCount()
{
	CString qSQL;

	qSQL.Format("SELECT ItemCount, Number from GameServerInfo where Number=0");

	return m_Query.ExecGetInt(qSQL);
}

BOOL CGSDBSet::SetCount(int count)
{
	CString qSQL;

	qSQL.Format("UPDATE GameServerInfo SET ItemCount = %d where Number=0", count);

	return m_Query.Exec(qSQL);
}

BOOL CGSDBSet::CreateZen()
{
	CString qSQL;

	qSQL.Format("SELECT ZenCount from GameServerInfo where Number=0");

	int ZenCount = m_Query.ExecGetInt(qSQL);

	if(ZenCount < 0) return FALSE;

	if(ZenCount > 12500) return FALSE;

	qSQL.Format("UPDATE GameServerInfo SET ZenCount= ZenCount+1 where Number=0");
	if( m_Query.Exec(qSQL) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CGSDBSet::CheckMuDBIdentification()
{
	CString qSql;

	qSql.Format("exec WZ_Get_DBID");

	if( m_Query.Exec(qSql) == FALSE)
	{
		m_Query.Clear();
		return FALSE;
	}

	if( m_Query.Fetch() != SQL_NO_DATA )
	{
		//m_Query.GetStr("DESC", g_szDBID_DESC);
	}
	else
	{
		m_Query.Clear();
		return FALSE;
	}

	m_Query.Clear();
	return TRUE;
}

BOOL CGSDBSet::CreateItem(DWORD &sirial_number)
{
	sirial_number = 0;

	m_Query.Exec("WZ_GetItemSerial");

	if( m_Query.Fetch() == SQL_NO_DATA )
	{
		m_Query.Clear();
		return FALSE;
	}

	sirial_number = m_Query.GetInt(1);

	m_Query.Clear();

	return TRUE;
}

int CGSDBSet::GetItemSerial(DWORD& sirial_number, int MakeSeiralCount)
{
	sirial_number = 0;

	CString strQuery;

	strQuery.Format("WZ_GetItemSerial2 '%d'", MakeSeiralCount);

	m_Query.Exec(strQuery);

	if( m_Query.Fetch() == SQL_NO_DATA )
	{
		m_Query.Clear();
		return 0;
	}

	sirial_number = (DWORD)m_Query.GetInt64(1);

	gWindow.PrintLog("[GetItemSerial] Get Last Item Serial from DB : %u", sirial_number);
	m_Query.Clear();

	return 1;
}

BOOL CGSDBSet::CreatePropertyItemInfo(DWORD serial, SDHP_PROPERTY_ITEMCREATE* lpMsg)
{
	CString qSQL;

	char_ID Name1(lpMsg->szName);
	char_ID Name2(lpMsg->szName_T);

	qSQL.Format("INSERT INTO T_PropertyItemInfo (ItemSerial,Name1,Level1,Class1,Name2,Level2,Class2,RemainTime,CDate) VALUES(%u,'%s',%d,%d,'%s',%d,%d,%d,getdate())", serial, Name1.GetBuffer(), lpMsg->wLevel, lpMsg->btClass, Name2.GetBuffer(), lpMsg->wLevel_T, lpMsg->btClass_T, lpMsg->wRemainTime);

	if( m_Query.Exec(qSQL) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CGSDBSet::GetPropertyItemInfo(DWORD serial, SDHP_LOAD_MEDALINFO_RESULT* lpMsg)
{
	CString qSQL;

	char_ID Name = 0;

	char Date[20] = "";

	qSQL.Format("SELECT * FROM T_PropertyItemInfo where ItemSerial = %u", serial);

	if( m_Query.Exec(qSQL) == FALSE)
	{
		m_Query.Clear();
		return FALSE;
	}

	if( m_Query.Fetch() == SQL_NO_DATA )
	{
		m_Query.Clear();
		return FALSE;
	}

	m_Query.GetStr("Name1", Name.GetBuffer());

	memcpy(lpMsg->szName, Name.GetBuffer(), 10);

	lpMsg->wLevel = m_Query.GetInt("Level1");
	lpMsg->btClass = m_Query.GetInt("Class1");

	m_Query.GetStr("Name2", Name.GetBuffer());

	memcpy(lpMsg->szName_T, Name.GetBuffer(), 10);

	lpMsg->wLevel_T = m_Query.GetInt("Level2");
	lpMsg->btClass_T = m_Query.GetInt("Class2");

	lpMsg->wRemainTime = m_Query.GetInt("RemainTime");

	gWindow.PrintLog("%d %d %d %d", lpMsg->wLevel, lpMsg->wLevel_T, lpMsg->btClass, lpMsg->btClass_T);

	m_Query.Clear();

	return TRUE;
}