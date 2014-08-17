// BaseDAO.cpp: implementation of the CBaseDAO class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zDataServer.h"
#include "BaseDAO.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseDAO::CBaseDAO(): m_lpFuncFlat(NULL), m_lpFuncUnFlat(NULL), m_lpFuncFlatBindCol(NULL)
{

}

CBaseDAO::~CBaseDAO()
{

}

HANDLE CBaseDAO::AllocStmtHandle(HANDLE hDbc)
{
	int bDbcMapping = 0; //loc2
	HANDLE hStmt; //loc3
	
	if(hDbc == NULL)
	{
		return NULL;
	}

	SQLRETURN sr = SQLAllocHandle(3, hDbc, &hStmt);

	if(sr != 0)
	{
		DisplayError(sr, 2, hDbc, 0, 3);
		return NULL;
	}

	return hStmt;
}

void CBaseDAO::FreeStmtHandle(HANDLE hStmt)
{
	SQLFreeHandle(3, hStmt);
}

int CBaseDAO::ExecuteDirectTask(DWORD dwTaskId, void *lpParam, unsigned long *pFetchedRows)
{
	HANDLE hDbc, hStmt;

	int bReturn;

	hDbc = GetDbcHandle(); //loc2

	if(hDbc == NULL)
	{
		return NULL;
	}

	hStmt = AllocStmtHandle(hDbc);

	if(hStmt == NULL)
	{
		return NULL;
	}

	if(m_lpFuncFlat != NULL)
	{
		m_lpFuncFlat(dwTaskId, hStmt, lpParam);
	}
	else
	{
		FlatCallback(dwTaskId, hStmt, lpParam);
	}

	DWORD nNumRowsFetched;
	WORD nRowStatusArray[700];

	SQLRETURN sr = SQLExecute(hStmt);

	if(sr == SQL_SUCCESS || sr == SQL_SUCCESS_WITH_INFO)
	{
		DWORD dwRet;

		if(m_lpFuncFlatBindCol != NULL)
		{
			dwRet = m_lpFuncFlatBindCol(dwTaskId, hStmt, lpParam);
		}
		else
		{
			dwRet = FlatBindCol(dwTaskId, hStmt, lpParam);
		}

		if(dwRet != 0)
		{
			 SQLSetStmtAttr(hStmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)(WORD)(dwRet>>16), -5);
			 SQLSetStmtAttr(hStmt, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)(WORD)(dwRet&0xFFFF), -5);
			 SQLSetStmtAttr(hStmt, SQL_ATTR_ROW_STATUS_PTR, (SQLPOINTER)nRowStatusArray, -5);
			 SQLSetStmtAttr(hStmt, SQL_ATTR_ROWS_FETCHED_PTR, (SQLPOINTER)&nNumRowsFetched, 4);

			sr = SQLFetch(hStmt);

			if(pFetchedRows != NULL)
			{
				*pFetchedRows = nNumRowsFetched;
			}

			if(sr == SQL_ERROR || sr == SQL_SUCCESS_WITH_INFO)
			{
				DisplayError(sr, 3, hStmt, dwTaskId, 7);
			}
		}

		while((sr = SQLMoreResults(hStmt)) == SQL_SUCCESS)
		{
		}

		if(sr == SQL_ERROR || sr == SQL_SUCCESS_WITH_INFO)
		{
			DisplayError(sr, 3, hStmt, dwTaskId, 8);
		}

		bReturn = TRUE;
	}
	else
	{
		DisplayError(sr, 3, hStmt, dwTaskId, 9);
		bReturn = FALSE;
	}

	FreeStmtHandle(hStmt);

	return bReturn;
}

int CBaseDAO::DisplayError(short nResult, short fHandleType, HANDLE handle, DWORD dwTaskID, int nErrorContext)
{
	return 1;
}