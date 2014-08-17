// ItemExtDbSet.cpp: implementation of the CItemExtDbSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemExtDbSet.h"
#include "CharPreviewDBSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemExtDbSet::CItemExtDbSet()
{

}

CItemExtDbSet::~CItemExtDbSet()
{

}

BOOL CItemExtDbSet::Connect()
{
	if( m_DBQuery.Connect(3, gSettings.ConnectDNS, gSettings.ConnectLogin, gSettings.ConnectPassword) == FALSE )
	{
		//MsgBox(" CItemExt DB set connect fail.");
		return FALSE;
	}

	return TRUE;
}


BOOL CItemExtDbSet::IsUser(char* name)
{
	CString qSQL;
	char szname[MAX_IDSTRING+1] = "";

	qSQL.Format("CItemExtSPGet '%s'", name);

	if(m_DBQuery.Exec(qSQL) == TRUE)
	{
		if(m_DBQuery.Fetch() == SQL_NO_DATA)
		{
			m_DBQuery.Clear();
			return FALSE;
		}

		m_DBQuery.GetStr(1, szname);
		m_DBQuery.Clear();

		if(strlen(szname) < 1)
		{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CItemExtDbSet::Add(char* name)
{
	CString qSQL;
	char szname[MAX_IDSTRING+1] = "";

	qSQL.Format("CItemExtSPAdd '%s'", name);

	if(m_DBQuery.Exec(qSQL) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}