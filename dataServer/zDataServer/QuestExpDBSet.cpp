// QuestExpDBSet.cpp: implementation of the CQuestExpDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestExpDBSet.h"
#include "winutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestExpDBSet::CQuestExpDBSet()
{

}

CQuestExpDBSet::~CQuestExpDBSet()
{

}

BOOL CQuestExpDBSet::Connect()
{
	return this->CDBConBase::Connect(gSettings.ConnectLogin, gSettings.ConnectPassword);
}

BOOL CQuestExpDBSet::DSDB_QueryQuestInfoSave(LP_PMSG_QUESTEXP_INFO lpRecv)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpRecv->szCharName, MAX_IDSTRING);

	int len = strlen(szId);

	if(len <= 0 || len > MAX_IDSTRING)
	{
//		LogAddC(LOGC_RED,"%s] 저장 에러 %s %d", szId, __FILE__, __LINE__);
		return TRUE;
	}

	int iQuestCnt = lpRecv->btQuestCnt;

	_QUESTEXP_INFO QuestInfo[25];

	if(CHECK_LIMIT(iQuestCnt, 25) == FALSE)
	{
		//LogAddC(LOGC_RED,"%s] 저장 에러(에피소드 갯수) %s %d", szId, __FILE__, __LINE__);
		return TRUE;
	}

	memcpy(&QuestInfo, lpRecv+=1, iQuestCnt * sizeof(_QUESTEXP_INFO));

	CString szQuery;

	long nLen = -2;
	char szTemp[2048] = {0};

	for(int i = 0; i < iQuestCnt; i++)
	{
		DWORD dwQuestInfoIndexID = QuestInfo[i].dwQuestIndexID;
        int iEpisode = dwQuestInfoIndexID >> 16;
        int iQuestSwitch = dwQuestInfoIndexID % 0x10000;

		//if(CHECK_LIMIT(iQuestSwitch, 25) == FALSE)
		if( (iEpisode<=0?FALSE:iEpisode>(25-1)?FALSE:TRUE) == FALSE )
		{
			//LogAddC(LOGC_RED,"%s] 저장 에러(에피소드) %s %d", szId, __FILE__, __LINE__);
			continue;
		}

		if(iQuestSwitch > 255 || iQuestSwitch < 0)
		{
			//LogAddC(LOGC_RED,"%s] 저장 에러(퀘스트 스위치) %s %d", szId, __FILE__, __LINE__);
			continue;
		}

		WORD wProgState = QuestInfo[i].wProgState;
		long lStatrDate = QuestInfo[i].lStartDate;
		long lEndDate = QuestInfo[i].lEndDate;
		sprintf(szTemp, "{CALL WZ_QuestExpUserInfoSave ('%s', %d, %d, %d, %d, %d, ?, ?, ?)}", szId, iEpisode, iQuestSwitch, wProgState, lStatrDate, lEndDate);

		int iRet = SQLPrepare(m_DBQuery.hStmt, (SQLTCHAR *)szTemp, SQL_NTS);

		if (iRet)
		{
			bool bReConnect = false;

			m_DBQuery.PrintDiag(bReConnect);	
			
			//LogAddC(LOGC_RED,"Error SQLPrepare Ret [%d] %s %d", iRet, __FILE__, __LINE__);	
			
			m_DBQuery.Clear();
			return TRUE;
		}

		int size = 5;

		long cbBlob;
		SQLBindParameter(m_DBQuery.hStmt, 1, SQL_PARAM_INPUT, -2, -3, size, 0, (SQLPOINTER)1, 0, &cbBlob);
		SQLBindParameter(m_DBQuery.hStmt, 2, SQL_PARAM_INPUT, -2, -3, size, 0, (SQLPOINTER)2, 0, &cbBlob);
		SQLBindParameter(m_DBQuery.hStmt, 3, SQL_PARAM_INPUT, -2, -3, size, 0, (SQLPOINTER)3, 0, &cbBlob);

		cbBlob = -size - 100;

		short retcode = SQLExecute(m_DBQuery.hStmt);
		
		void *pToken;

		if ( retcode == SQL_NULL_DATA )
		{
			bool bReConnect = false;

			m_DBQuery.PrintDiag(bReConnect);
			
			m_DBQuery.Clear();
			
			//LogAddC(LOGC_RED,"Error SQLExecute %s %d", __FILE__, __LINE__);
			return TRUE;
		}

		retcode = SQLParamData(m_DBQuery.hStmt, &pToken);
		
		if ( retcode == SQL_NEED_DATA )
		{
			SQLPutData(m_DBQuery.hStmt, QuestInfo[i].btAskIndex, size);
		}
		
		retcode = SQLParamData(m_DBQuery.hStmt, &pToken);
		
		if ( retcode == SQL_NEED_DATA )
		{
			SQLPutData(m_DBQuery.hStmt, QuestInfo[i].btAskValue, size);
		}
		
		retcode = SQLParamData(m_DBQuery.hStmt, &pToken);
		
		if ( retcode == SQL_NEED_DATA )
		{
			SQLPutData(m_DBQuery.hStmt, QuestInfo[i].btAskState, size);
		}
		
		retcode = SQLParamData(m_DBQuery.hStmt, &pToken);
		
		m_DBQuery.Clear();
		memset(szTemp, 0, 2048);
	}

	return FALSE;
}

_QUESTEXP_INFO::_QUESTEXP_INFO()
{
	this->dwQuestIndexID = 0;
	this->wProgState = 0;
	this->lStartDate = 0;
	this->lEndDate = 0;

	memset(this->btAskIndex, 0, 5);
	memset(this->btAskValue, 0, 5);
	memset(this->btAskState, 0, 5);
}

BOOL CQuestExpDBSet::DSDB_QueryQuestInfoLoad(char* szCharName, LP_QUESTEXP_INFO pQuestInfo, LPPMSG_ANS_QUESTEXP_INFO pMsg)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, szCharName, MAX_IDSTRING);

	int len = strlen(szId);

	if ( len <= 0 || len > MAX_IDSTRING )
	{
		//LogAddC(LOGC_RED,"%s] 로드 에러 %s %d", szId, __FILE__, __LINE__);
		return TRUE;
	}

	int iCnt = 0;
	CString szQuery;
	char szTemp[2048] = {'\0'};

	int iEpisode = 0;
	int iQuestSwitch = 0;
	int wProgState = 0;
	
	long nLen;

	int arrEp[20] = {0};

	szQuery.Format("WZ_QuestExpUserInfoLoad_Fir '%s'", szId);
	
	if (m_DBQuery.Exec(szQuery) == FALSE)
	{
		bool bTmp = false;
		
		m_DBQuery.PrintDiag(bTmp);
		
		//LogAddC(LOGC_RED, "Error m_DBQuery.Exec %s %d", __FILE__, __LINE__);
		
		m_DBQuery.Clear();
		return TRUE;		
	}

	short sqlRet = m_DBQuery.Fetch();

	while(sqlRet != SQL_NO_DATA && sqlRet != SQL_NULL_DATA)
	{
        bool bLoadChk = false;
        int iEpisode1 = m_DBQuery.GetInt("EPISODE");
        int iQuestSwitch1 = m_DBQuery.GetInt("QUEST_SWITCH");
        WORD wProgState1 = m_DBQuery.GetInt("PROG_STATE");
        long lStatrDate = (long)m_DBQuery.GetInt64("StartDateConvert");
        long lEndDate = (long)m_DBQuery.GetInt64("EndDateConvert");

		//if(CHECK_LIMIT(iQuestSwitch1, 25) == FALSE)
		if( (iEpisode1<=0?FALSE:iEpisode1>(25-1)?FALSE:TRUE) == FALSE )
		{
			//LogAddC(LOGC_RED,"%s] 로드 에러(에피소드) %s %d", szId, __FILE__, __LINE__);
			bLoadChk = true;
		}

		if(iQuestSwitch1 > 255 || iQuestSwitch1 < 0)
		{
			//LogAddC(LOGC_RED,"%s] 로드 에러(퀘스트 스위치) %s %d", szId, __FILE__, __LINE__);
			bLoadChk = 1;
		}

		if (bLoadChk == false)
		{
			pQuestInfo[iCnt].dwQuestIndexID = (iEpisode1 << 16) +iQuestSwitch1;
			pQuestInfo[iCnt].wProgState = wProgState1;

			arrEp[iCnt] = iEpisode1;

			pQuestInfo[iCnt].lStartDate = lStatrDate;
			pQuestInfo[iCnt].lEndDate = lEndDate;
			iCnt++;

			if ( iCnt >= 25 )
			{
				break;
			}
		}

		sqlRet = m_DBQuery.Fetch();
	}
	
	m_DBQuery.Clear();
	
	for (int j = 0; j < iCnt; j++)
	{
		sprintf(szTemp, "{CALL WZ_QuestExpUserInfoLoad_Sec('%s', %d, ?, ?, ?)}", szId, arrEp[j]);
        
		m_DBQuery.BindParameterBinaryOutput(1, pQuestInfo[j].btAskIndex, 5, &nLen);
        m_DBQuery.BindParameterBinaryOutput(2, pQuestInfo[j].btAskValue, 5, &nLen);
        m_DBQuery.BindParameterBinaryOutput(3, pQuestInfo[j].btAskState, 5, &nLen);
        
		if ( m_DBQuery.Exec(szTemp) == FALSE)
        {
			bool bReConnect = FALSE;
		
			m_DBQuery.PrintDiag(bReConnect);
			
			//LogAddC(LOGC_RED,"Error m_DBQuery.Exec %s %d",__FILE__ ,__LINE__);
			
			m_DBQuery.Clear();
			return TRUE;
		}
	}

	pMsg->btQuestCnt = iCnt;
	
	m_DBQuery.Clear();
	
	return FALSE;
}