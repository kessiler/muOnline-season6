// ZenEvenDb.h: interface for the CZenEvenDb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZENEVENDB_H__052594FF_D152_47D9_B3B5_7BFD7C5CB4FD__INCLUDED_)
#define AFX_ZENEVENDB_H__052594FF_D152_47D9_B3B5_7BFD7C5CB4FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CZenEvenDb  
{
public:
	CZenEvenDb();
	virtual ~CZenEvenDb();

	void Connect();
	BOOL InsertAccount(char* AccountID, int Zen);
	BOOL GetZen(char* AccountID, int& Zen);
	BOOL SetZen(char* AccountID, int& Zen);

public:
	CQuery m_DBQuery;
};

#endif // !defined(AFX_ZENEVENDB_H__052594FF_D152_47D9_B3B5_7BFD7C5CB4FD__INCLUDED_)
