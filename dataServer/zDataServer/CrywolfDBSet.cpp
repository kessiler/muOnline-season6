// CrywolfDBSet.cpp: implementation of the CCrywolfDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfDBSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfDBSet::CCrywolfDBSet()
{

}

CCrywolfDBSet::~CCrywolfDBSet()
{

}

BOOL CCrywolfDBSet::Connect()
{
	return CDBConBase::Connect(gSettings.ConnectLogin, gSettings.ConnectPassword);
}

BOOL CCrywolfDBSet::DSDB_QueryCrywolfStateUpdate()
{
	return FALSE;
}

BOOL CCrywolfDBSet::DSDB_QueryCrywolfInfoLoad(int iMapSvrGroup, LPCWP_ANS_CRYWOLFINFOLOAD lpAnsMsg)
{
	CString szQuery;

	szQuery.Format("EXEC WZ_CW_InfoLoad %d", iMapSvrGroup);

	if(m_DBQuery.Exec(szQuery) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	if(m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	lpAnsMsg->iOccupationState = m_DBQuery.GetInt("CRYWOLF_OCCUFY");
	lpAnsMsg->iCrywolfState = m_DBQuery.GetInt("CRYWOLF_STATE");

	m_DBQuery.Clear();

	return FALSE;
}

BOOL CCrywolfDBSet::DSDB_QueryCrywolfInfoSave(int iMapSvrGroup, LPCWP_REQ_CRYWOLFINFOSAVE lpReqMsg)
{
	CString szQuery;

	szQuery.Format("EXEC WZ_CW_InfoSave %d, %d, %d", iMapSvrGroup, lpReqMsg->iCrywolfState, lpReqMsg->iOccupationState);

	if(m_DBQuery.Exec(szQuery) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	m_DBQuery.Clear();

	return FALSE;
}