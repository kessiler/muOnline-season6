#include "stdafx.h"
#include "QuestMonKillDBSet.h"

CQuestMonKillDBSet::CQuestMonKillDBSet()
{

}

CQuestMonKillDBSet::~CQuestMonKillDBSet()
{

}

BOOL CQuestMonKillDBSet::Connect()
{
	return CDBConBase::Connect(gSettings.ConnectLogin, gSettings.ConnectPassword);
}

BOOL CQuestMonKillDBSet::DSDB_QueryQuestMonKillInfoLoad(char* szCharName, LPSDHP_QUEST_MONSTER_KILL_INFO lpReqInfo)
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

	szQuery.Format("EXEC WZ_QMK_InfoLoad '%s'", szId);

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

	lpReqInfo->btQuestDBIndex = m_DBQuery.GetInt("QUEST_INDEX");

	lpReqInfo->iQuestMonIndex1 = m_DBQuery.GetInt("MON_INDEX_1");
	lpReqInfo->iKillCount1 = m_DBQuery.GetInt("KILL_COUNT_1");

	lpReqInfo->iQuestMonIndex2 = m_DBQuery.GetInt("MON_INDEX_2");
	lpReqInfo->iKillCount2 = m_DBQuery.GetInt("KILL_COUNT_2");

	lpReqInfo->iQuestMonIndex3 = m_DBQuery.GetInt("MON_INDEX_3");
	lpReqInfo->iKillCount3 = m_DBQuery.GetInt("KILL_COUNT_3");

	lpReqInfo->iQuestMonIndex4 = m_DBQuery.GetInt("MON_INDEX_4");
	lpReqInfo->iKillCount4 = m_DBQuery.GetInt("KILL_COUNT_4");

	lpReqInfo->iQuestMonIndex5 = m_DBQuery.GetInt("MON_INDEX_5");
	lpReqInfo->iKillCount5 = m_DBQuery.GetInt("KILL_COUNT_5");

	m_DBQuery.Clear();

	return FALSE;
}

BOOL CQuestMonKillDBSet::DSDB_QueryQuestMonKillInfoSave(LPSDHP_QUEST_MONSTER_KILL_INFO lpRecv)
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

	szQuery.Format("EXEC WZ_QMK_InfoSave '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", szId, lpRecv->btQuestDBIndex, 
		lpRecv->iQuestMonIndex1, lpRecv->iKillCount1, 
		lpRecv->iQuestMonIndex2, lpRecv->iKillCount2, 
		lpRecv->iQuestMonIndex3, lpRecv->iKillCount3, 
		lpRecv->iQuestMonIndex4, lpRecv->iKillCount4,
		lpRecv->iQuestMonIndex5, lpRecv->iKillCount5);

	if(m_DBQuery.Exec(szQuery) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	m_DBQuery.Clear();

	return FALSE;
}