// WzMemScript.cpp: implementation of the CWzMemScript class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WzMemScript.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWzMemScript::CWzMemScript()
{

}

CWzMemScript::~CWzMemScript()
{

}

int CWzMemScript::SetBuffer(char* buffer, int buffersize)
{
	SetScriptParsingBuffer(buffer, buffersize);
	return 1;
}

int CWzMemScript::GetC()
{
	if ( m_iBufferCount >= m_iBufferSize )
	{
		return -1;
	}
	else
	{
		return m_pBuffer [ m_iBufferCount++ ];
	}
}

void CWzMemScript::UnGetC(int ch)
{
	if ( m_iBufferCount <= 0)
	{
		return;
	}
	else
	{
		m_iBufferCount--;

		m_pBuffer [ m_iBufferCount ] = ch;
	}
}


void CWzMemScript::SetScriptParsingBuffer( char* buffer, int size)
{
	m_pBuffer = buffer;
	m_iBufferSize = size;
	m_iBufferCount = 0;
}

int CWzMemScript::GetNumber()
{
	return (int)TokenNumber;
}

char* CWzMemScript::GetString()
{
	return TokenString;
}

WZSMDToken CWzMemScript::GetToken()
{
	char ch;
	TokenString[0] = '\0';
	do
	{
		if ( (ch =(char) GetC() ) == EOF) return T_END;

		if (ch == '/')
		{
			if ((ch =(char) GetC() )=='/')	
			{
				while ((ch =(char) GetC()) != ('\n'))	continue;
			}
		}
	} while(  isspace(ch) );

	char *p, TempString[100];
	switch(ch)
	{	
	case '#':
		return CurrentToken = T_COMMAND;
	case ';':
		return CurrentToken = T_SEMICOLON;
	case ',':
		return CurrentToken = T_COMMA;
	case '{':
		return CurrentToken = T_LP;
	case '}':
		return CurrentToken = T_RP;
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':
		UnGetC(ch);
		p = TempString;
		while ( (  (ch = GetC() ) !=EOF) && (ch=='.' || isdigit(ch) || ch=='-') )
			*p++ = ch;
		*p = 0;
		TokenNumber = (float)atof(TempString);
		//			sscanf(TempString," %f ",&TokenNumber);
		return CurrentToken = T_NUMBER;
	case '"':
		p = TokenString;
		while ( (  (ch = GetC() ) !=EOF) && (ch!='"'))// || isalnum(ch)) )
			*p++ = ch;
		if (ch!='"')
			UnGetC(ch);
		*p = 0;
		return CurrentToken = T_NAME;
	default:
		if (isalpha(ch))	
		{
			p = TokenString;
			*p++ = ch;
			while ( (  (ch = GetC() ) !=EOF) && (ch=='.' || ch=='_' || isalnum(ch)) )
				*p++ = ch;
			UnGetC(ch);
			*p = 0;
			return CurrentToken = T_NAME;
		}
		return CurrentToken = T_SMD_ERROR;
	}
}