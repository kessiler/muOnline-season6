// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef WZSCRIPTENCODE_H
#define WZSCRIPTENCODE_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum WZSMDToken {
  T_NAME = 0x0,
  T_NUMBER = 0x1,
  T_END = 0x2,
  T_COMMAND = 0x23,
  T_LP = 0x7b,
  T_RP = 0x7d,
  T_COMMA = 0x2c,
  T_SEMICOLON = 0x3b,
  T_SMD_ERROR = 0x3c,
};

class CWZScriptEncode
{
public:

	CWZScriptEncode();
	virtual ~CWZScriptEncode();

	int Open(char* filename);
	void Close();
	void SetScriptParsingBuffer(char* buffer, int size);
	enum WZSMDToken GetToken();
	//enum WZSMDToken __thiscall TokenType();
	int GetNumber();
	char* GetString();
	void Encode(char* buffer, int size);

protected:

	int GetC();
	void UnGetC(int ch);

private:
	
	char* m_pBuffer;	// 4
	int m_iBufferSize;	// 8
	int m_iBufferCount;	// C
	HANDLE m_hFile;	// 10
	char * m_Token;	// 14
	float TokenNumber;	// 18
	char TokenString[100];	// 1C
	enum WZSMDToken CurrentToken;	// 80
};


#endif