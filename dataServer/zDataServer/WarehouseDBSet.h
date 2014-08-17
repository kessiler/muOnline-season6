// WarehouseDBSet.h: interface for the CWarehouseDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAREHOUSEDBSET_H__401A30C9_6C84_4E31_9806_7673F44E5A0E__INCLUDED_)
#define AFX_WAREHOUSEDBSET_H__401A30C9_6C84_4E31_9806_7673F44E5A0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWarehouseDBSet  
{
public:
	CWarehouseDBSet();
	virtual ~CWarehouseDBSet();

	BOOL	Connect();
	BOOL	Get(char* AccountID);
	BOOL	Create(char* AccountID);
	BOOL	Save(char* AccountID, BYTE* itembuf, int money, int pw);
	BOOL	Load(char* AccountID, BYTE* itembuf, int& money, BYTE& _dbversion, short& pw);
	BOOL	MoneyUpdate(char* AccountID, int money);

public:
	CQuery m_DBQuery;
};

#endif // !defined(AFX_WAREHOUSEDBSET_H__401A30C9_6C84_4E31_9806_7673F44E5A0E__INCLUDED_)
