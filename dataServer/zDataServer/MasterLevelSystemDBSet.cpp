// MasterLevelSystemDBSet.cpp: implementation of the CMasterLevelSystemDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MasterLevelSystemDBSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterLevelSystemDBSet::CMasterLevelSystemDBSet()
{

}

CMasterLevelSystemDBSet::~CMasterLevelSystemDBSet()
{

}

BOOL CMasterLevelSystemDBSet::Connect()
{
	return CDBConBase::Connect(gSettings.ConnectLogin, gSettings.ConnectPassword);
}

BOOL CMasterLevelSystemDBSet::DSDB_QueryMasterLevelSystemLoad(char* szCharName, LPMLP_ANS_MASTERLEVEL_INFO lpReqInfo)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, szCharName, MAX_IDSTRING);

	int len = strlen(szId);

	if(len <= 0 || len > MAX_IDSTRING)
	{
		gWindow.PrintLog("%s] 저장 에러 %s %d", szId, __FILE__, __LINE__);
		return TRUE;
	}

	CString szQuery;

	szQuery.Format("EXEC WZ_MLS_InfoLoad '%s'", szId);

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

	lpReqInfo->nMLevel = m_DBQuery.GetInt("MASTER_LEVEL");
	lpReqInfo->i64MLExp = m_DBQuery.GetInt64("ML_EXP");
	lpReqInfo->i64NextMLExp = m_DBQuery.GetInt64("ML_NEXTEXP");
	lpReqInfo->nMLPoint = m_DBQuery.GetInt("ML_POINT");

	m_DBQuery.Clear();
	return FALSE;
}

BOOL CMasterLevelSystemDBSet::DSDB_QueryMasterLevelSystemSave(LPMLP_REQ_MASTERLEVEL_INFOSAVE lpRecv)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpRecv->szCharName, MAX_IDSTRING);

	int len = strlen(szId);

	if(len <= 0 || len > MAX_IDSTRING)
	{
		gWindow.PrintLog("%s] 저장 에러 %s %d", szId, __FILE__, __LINE__);
		return TRUE;
	}

	CString szQuery;
	szQuery.Format("EXEC WZ_MLS_InfoSave '%s', %d, %I64d, %I64d, %d", szId, lpRecv->nMLevel, lpRecv->i64MLExp, lpRecv->i64NextMLExp, lpRecv->nMLPoint);

	if(m_DBQuery.Exec(szQuery) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	m_DBQuery.Clear();
	return FALSE;
}