// CharPreviewDBSet.h: interface for the CCharPreviewDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARPREVIEWDBSET_H__9278C328_E1A7_449D_B695_72E6CCBBFF97__INCLUDED_)
#define AFX_CHARPREVIEWDBSET_H__9278C328_E1A7_449D_B695_72E6CCBBFF97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCharPreviewDBSet  
{
public:
	CCharPreviewDBSet();
	virtual ~CCharPreviewDBSet();

	BOOL Conenect();
	BOOL GetChar(char* Name, int& _level, int& _class, BYTE* Inventory, BYTE& _ctlcode, BYTE& _dbverstion, BYTE& _btGuildStatus);
  
public:
	CQuery m_DBQuery;
};

#endif // !defined(AFX_CHARPREVIEWDBSET_H__9278C328_E1A7_449D_B695_72E6CCBBFF97__INCLUDED_)
