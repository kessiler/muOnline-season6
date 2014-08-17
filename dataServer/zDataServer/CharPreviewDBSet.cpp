// CharPreviewDBSet.cpp: implementation of the CCharPreviewDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharPreviewDBSet.h"
#include "winutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharPreviewDBSet::CCharPreviewDBSet()
{

}

CCharPreviewDBSet::~CCharPreviewDBSet()
{

}

BOOL CCharPreviewDBSet::Conenect()
{
	if( m_DBQuery.Connect(3, gSettings.ConnectDNS, gSettings.ConnectLogin, gSettings.ConnectPassword) == FALSE )
	{
		//MsgBox("CharPreviewDBSet ODBC Connect Fail");
		return FALSE;
	}

	return TRUE;
}

BOOL CCharPreviewDBSet::GetChar(char* Name, int& _level, int& _class, BYTE* Inventory, BYTE& _ctlcode, BYTE& _dbverstion, BYTE& _btGuildStatus)
{
	CString qSql;

	if(SQLSyntexCheck(Name) == FALSE)
	{
		return FALSE;
	}

	qSql.Format("SELECT Name, cLevel, Class, CtlCode, DbVersion FROM vCharacterPreview WHERE Name='%s'", Name);

	if(m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if(m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	m_DBQuery.GetStr("Name", Name);

	_level = m_DBQuery.GetInt("cLevel");
	_class = m_DBQuery.GetInt("Class");

	int ctlcode = m_DBQuery.GetInt("CtlCode");

	if(ctlcode < 0)
	{
		ctlcode = 0;
	}

	_ctlcode = ctlcode;

	int dbverstion = m_DBQuery.GetInt("DbVersion");

	if(dbverstion < 0)
	{
		dbverstion = 0;
	}

	_dbverstion = dbverstion;

	m_DBQuery.Clear();

	qSql.Format("SELECT Inventory FROM Character WHERE Name='%s'", Name);
	int nRet = m_DBQuery.ReadBlob(qSql, Inventory);

	if(nRet == 0)
	{
		memset(Inventory, 0xFF, 120);
	}
	else if(nRet == -1)
	{
		return FALSE;
	}

	m_DBQuery.Clear();

	qSql.Format("SELECT G_Status FROM GuildMember WHERE Name='%s'", Name);

	_btGuildStatus = 0xFF;

	if(m_DBQuery.Exec(qSql) == TRUE)
	{
		if(m_DBQuery.Fetch() == SQL_NO_DATA)
		{
		}
		else
		{
			_btGuildStatus = m_DBQuery.GetInt("G_Status");
		}
	}

	m_DBQuery.Clear();

	return TRUE;
}