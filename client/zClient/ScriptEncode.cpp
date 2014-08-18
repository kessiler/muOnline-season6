#include "stdafx.h"
#include "ScriptEncode.h"
// -----------------------------------------------------------------------

BYTE XorTableLock[]	= { 0xD4, 0x7B, 0x2B };
// -----------------------------------------------------------------------

ScriptEncode	g_ScriptEncode;
// -----------------------------------------------------------------------

ScriptEncode::ScriptEncode()
{
	m_pBuffer		= NULL;
	m_iBufferSize	= 0;
	m_iBufferCount	= 0;
	m_hFile			= INVALID_HANDLE_VALUE;
}
// -----------------------------------------------------------------------

ScriptEncode::~ScriptEncode()
{
	Close();
}
// -----------------------------------------------------------------------

int ScriptEncode::ReadScript(char * File)
{
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hFile);
	}
	// ----
	m_hFile = CreateFile(File, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80, NULL);
	// ----
	if( m_hFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	// ----
	int filesize = GetFileSize(m_hFile, NULL);
	// ----
	char* pBuffer = (char*)malloc(filesize);
	// ----
	DWORD iReadFileSize;
	// ----
	ReadFile(m_hFile, pBuffer, filesize, &iReadFileSize, 0);
	// ----
	if(iReadFileSize == 0)
	{
		Close();
	}
	// ----
	Encode(pBuffer, iReadFileSize);
	// ----
	SetScriptBuffer(pBuffer, iReadFileSize);
	// ----
	CloseHandle(m_hFile);
	// ----
	return 1;
}
// -----------------------------------------------------------------------

void ScriptEncode::Close()
{
	if ( m_pBuffer != 0)
	{
		free( m_pBuffer );
	}
}
// -----------------------------------------------------------------------

void ScriptEncode::Encode(char* buffer, int size)
{
	VMBEGIN
	// ----
	int i;
	for (i=0;i<size; i++)
	{
		buffer[i]=buffer[i]^XorTableLock[i%3];	
	}
	// ----
	VMEND
}
// -----------------------------------------------------------------------

int ScriptEncode::GetC()
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
// -----------------------------------------------------------------------

void ScriptEncode::UnGetC(int ch)
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
// -----------------------------------------------------------------------

void ScriptEncode::SetScriptBuffer( char* buffer, int size)
{
	m_pBuffer = buffer;
	m_iBufferSize = size;
	m_iBufferCount = 0;
}
// -----------------------------------------------------------------------

int ScriptEncode::GetNumber()
{
	return (int)TokenNumber;
}
// -----------------------------------------------------------------------

char* ScriptEncode::GetString()
{
	return TokenString;
}
// -----------------------------------------------------------------------

WZSMDToken ScriptEncode::GetToken()
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
				while ((ch != '\n') && (ch != EOF))	ch =(char) GetC();
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
		return CurrentToken = T_NUMBER;
	case '"':
		p = TokenString;
		while ( (  (ch = GetC() ) !=EOF) && (ch!='"'))
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
// -----------------------------------------------------------------------