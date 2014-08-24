#pragma once
// -----------------------------------------------------------------------

#include "..\include\readscript.h"
// -----------------------------------------------------------------------

enum WZSMDToken 
{
  T_NAME		= 0x0,
  T_NUMBER		= 0x1,
  T_END			= 0x2,
  T_COMMAND		= 0x23,
  T_LP			= 0x7b,
  T_RP			= 0x7d,
  T_COMMA		= 0x2c,
  T_SEMICOLON	= 0x3b,
  T_SMD_ERROR	= 0x3c,
};
// -----------------------------------------------------------------------

class ScriptEncode  
{
public:
	ScriptEncode();
	virtual ~ScriptEncode();
	// ---
	int			ReadScript(char * File);
	void		Close();
	void		SetScriptBuffer(char * Buffer, int Ssize);
	// ----
	int			GetNumber();
	char *		GetString();
	void		Encode(char * Buffer, int Size);
	int			GetC();
	void		UnGetC(int ch);
	// ----
	char *		m_pBuffer;
	int			m_iBufferSize;
	int			m_iBufferCount;
	HANDLE		m_hFile;
	char *		m_Token;
	float		TokenNumber;
	char		TokenString[100];
	// ----
	WZSMDToken	CurrentToken;
	WZSMDToken	GetToken();
	WZSMDToken	TokenType();
	// ----
}; extern ScriptEncode g_ScriptEncode;
// -----------------------------------------------------------------------