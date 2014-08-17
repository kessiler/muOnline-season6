// DBConBase.h: interface for the CDBConBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONBASE_H__BE3ED0E9_1405_4FBF_AFA1_BD5CAA52A86E__INCLUDED_)
#define AFX_DBCONBASE_H__BE3ED0E9_1405_4FBF_AFA1_BD5CAA52A86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDBConBase  
{
public:
	CQuery m_DBQuery;

public:
	CDBConBase();
	virtual ~CDBConBase();

	BOOL Connect(char* szConID, char* szConPass);
};

#endif // !defined(AFX_DBCONBASE_H__BE3ED0E9_1405_4FBF_AFA1_BD5CAA52A86E__INCLUDED_)
