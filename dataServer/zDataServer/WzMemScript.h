// WzMemScript.h: interface for the CWzMemScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZMEMSCRIPT_H__89643546_CDCA_4D61_8E23_D37B06AC9362__INCLUDED_)
#define AFX_WZMEMSCRIPT_H__89643546_CDCA_4D61_8E23_D37B06AC9362__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScriptEncode.h"

class CWzMemScript  
{
public:
	CWzMemScript();
	virtual ~CWzMemScript();

	int SetBuffer(char* buffer, int buffersize);
	void SetScriptParsingBuffer(char* buffer, int size);
	WZSMDToken GetToken();
	WZSMDToken TokenType();
	int GetNumber();
	char* GetString();
	int GetC();
	void UnGetC(int ch);

public:
	char* m_pBuffer;
	int m_iBufferSize;
	int m_iBufferCount;
	HANDLE m_hFile;
	char* m_Token;
	float TokenNumber;
	char TokenString[100];
	WZSMDToken CurrentToken;
};

#endif // !defined(AFX_WZMEMSCRIPT_H__89643546_CDCA_4D61_8E23_D37B06AC9362__INCLUDED_)
