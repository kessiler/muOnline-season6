// BaseDAO.h: interface for the CBaseDAO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDAO_H__F3C2AC88_A4B0_4E41_A468_DF0091E95EA4__INCLUDED_)
#define AFX_BASEDAO_H__F3C2AC88_A4B0_4E41_A468_DF0091E95EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void (*lpFuncFlat)(DWORD, void*, void*);
typedef void (*lpFuncUnFlat)(DWORD, void*, DWORD, DWORD);
typedef DWORD (*lpFuncFlatBindCol)(DWORD, void*, void*);

class CBaseDAO  
{
public:
	CBaseDAO();
	virtual ~CBaseDAO();

	virtual void FlatCallback(DWORD dwTaskId, HANDLE hStmt, void* lpParam)
	{
	}

	virtual DWORD FlatBindCol(DWORD dwTaskId, HANDLE hStmt, void* lpParam)
	{
		return 0;
	}

	virtual void UnFlatCallback(DWORD dwTaskId, void* lpParam, DWORD dwContext, DWORD dwFetchedRows)
	{
	}

	virtual HANDLE GetDbcHandle() = 0; //0x10

	int ExecuteDirectTask(DWORD dwTaskId, void* lpParam, unsigned long* pFetchedRows);
	int DisplayError(short nResult, short fHandleType, HANDLE handle, DWORD dwTaskID, int nErrorContext);

	HANDLE AllocStmtHandle(HANDLE hDbc);
	void FreeStmtHandle(HANDLE hStmt);
	int GetParamCount(void*);

public:
	lpFuncFlat	m_lpFuncFlat; //4
	lpFuncUnFlat m_lpFuncUnFlat; //8
	lpFuncFlatBindCol m_lpFuncFlatBindCol; //c
};

#endif // !defined(AFX_BASEDAO_H__F3C2AC88_A4B0_4E41_A468_DF0091E95EA4__INCLUDED_)
