// BadSyntaxChk.cpp: implementation of the CBadSyntaxChk class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadSyntaxChk.h"
#include "ReadScript.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBadSyntaxChk cBadStrChk;

CBadSyntaxChk::CBadSyntaxChk()
{
	m_BadStringCount = 0;
}

CBadSyntaxChk::~CBadSyntaxChk()
{

}

int CBadSyntaxChk::Load(char *filename)
{
	SMDFile = fopen(filename, "r");

	if(SMDFile == NULL)
	{
		return 0;
	}

	m_BadStringCount = 0;

	while( true )
	{
		SMDToken Token = GetToken();	if(Token == END) break;

		memcpy(m_BadString[m_BadStringCount], TokenString, MAX_BADSYNTAX_SIZE);

		m_BadString[m_BadStringCount++][MAX_BADSYNTAX_SIZE-1] = 0;
		
		if ( m_BadStringCount > MAX_BADSYNTAX_COUNT-1 )
		{
//			MsgBox("Bad syntax overflow error : %d", MAX_BADSYNTAX_COUNT);
			break;
		}
	}

	return 1;
}

int CBadSyntaxChk::CmpString(char *String)
{
	char * temp;
	for ( int n = 0; n < m_BadStringCount; ++n )
	{
		temp = strstr(String, m_BadString[n]);
		if ( temp != NULL )
		{
			return 1;
		}
	}

	return 0;
}