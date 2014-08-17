// OptionDbSet.h: interface for the COptionDbSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONDBSET_H__2F9B337F_D8BB_4E88_8DA0_92FD0A8D58A5__INCLUDED_)
#define AFX_OPTIONDBSET_H__2F9B337F_D8BB_4E88_8DA0_92FD0A8D58A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COptionDbSet  
{
public:
	BOOL Connect();
	COptionDbSet();
	virtual ~COptionDbSet();

	BOOL IsUser(char* name);
	BOOL Load(char* name, BYTE* KeyBuffer, BYTE& GO, BYTE& QK, BYTE& WK, BYTE& EK, BYTE& CW, BYTE& RK, int& QWERLevel);
	void Save(char* name, BYTE* KeyBuffer, BYTE GO, BYTE QK, BYTE WK, BYTE EK, BYTE CW, BYTE RK, int QWERLevel);

public:
	CQuery m_DBQuery;
};

#endif // !defined(AFX_OPTIONDBSET_H__2F9B337F_D8BB_4E88_8DA0_92FD0A8D58A5__INCLUDED_)
