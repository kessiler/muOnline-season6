// BadSyntaxChk.h: interface for the CBadSyntaxChk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADSYNTAXCHK_H__EA017447_8A66_4891_ACDF_A35D800EFB63__INCLUDED_)
#define AFX_BADSYNTAXCHK_H__EA017447_8A66_4891_ACDF_A35D800EFB63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BADSYNTAX_COUNT	500
#define MAX_BADSYNTAX_SIZE	25

class CBadSyntaxChk  
{
public:
	CBadSyntaxChk();
	virtual ~CBadSyntaxChk();

	int Load(char* filename);
	int CmpString(char* String);

public:
	char m_BadString[MAX_BADSYNTAX_COUNT][MAX_BADSYNTAX_SIZE];
	int m_BadStringCount;
};

extern CBadSyntaxChk cBadStrChk;

#endif // !defined(AFX_BADSYNTAXCHK_H__EA017447_8A66_4891_ACDF_A35D800EFB63__INCLUDED_)
