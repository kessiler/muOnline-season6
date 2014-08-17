// WzItemSerial.h: interface for the CWzItemSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZITEMSERIAL_H__5DA9C0E0_60FF_4252_A5D6_D9CFC2FFD80B__INCLUDED_)
#define AFX_WZITEMSERIAL_H__5DA9C0E0_60FF_4252_A5D6_D9CFC2FFD80B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWzItemSerial  
{
public:
	CWzItemSerial();
	virtual ~CWzItemSerial();
	
	int MakeSerial();
	DWORD GetSerial();

public:
	DWORD m_dwItemSerial;
	DWORD m_dwLastSerial;
	CRITICAL_SECTION m_csItemSerial;
};

extern CWzItemSerial gWzItemSerial;

#endif // !defined(AFX_WZITEMSERIAL_H__5DA9C0E0_60FF_4252_A5D6_D9CFC2FFD80B__INCLUDED_)
