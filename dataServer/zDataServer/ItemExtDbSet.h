// ItemExtDbSet.h: interface for the CItemExtDbSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMEXTDBSET_H__902AAA98_7B98_459D_B1CA_5B3B1D6415D8__INCLUDED_)
#define AFX_ITEMEXTDBSET_H__902AAA98_7B98_459D_B1CA_5B3B1D6415D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemExtDbSet  
{
public:
	CItemExtDbSet();
	virtual ~CItemExtDbSet();

	BOOL Add(char* name);
	BOOL IsUser(char* name);
	BOOL Connect();

public:
	CQuery m_DBQuery;
};

#endif // !defined(AFX_ITEMEXTDBSET_H__902AAA98_7B98_459D_B1CA_5B3B1D6415D8__INCLUDED_)
