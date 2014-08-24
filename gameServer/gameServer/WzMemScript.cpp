// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// WzMemScript.cpp

// GS-N 0.99.60T Status : Completed
//	GS-N	1.00.18	JPN	0x00596280	-	Completed

#include "stdafx.h"
#include "WzMemScript.h"



CWzMemScript::CWzMemScript()
{
	return;
}

CWzMemScript::~CWzMemScript()
{
	return;
}

int CWzMemScript::SetBuffer(char* buffer, int buffersize)
{
	this->SetScriptParsingBuffer(buffer, buffersize);
	return 1;
}


int CWzMemScript::GetC()
{
	if ( this->m_iBufferCount >= this->m_iBufferSize )
	{
		return -1;
	}
	else
	{
		return this->m_pBuffer[ this->m_iBufferCount++ ];
	}
}



void CWzMemScript::UnGetC(int ch)
{
	if ( this->m_iBufferCount <= 0)
	{
		return;
	}
	else
	{
		this->m_iBufferCount--;

		this->m_pBuffer [ this->m_iBufferCount ] = ch;
	}
}


void CWzMemScript::SetScriptParsingBuffer( char* buffer, int size)
{
	this->m_pBuffer = buffer;
	this->m_iBufferSize = size;
	this->m_iBufferCount = 0;
}


int CWzMemScript::GetNumber()
{
	return this->TokenNumber;
}

char* CWzMemScript::GetString()
{
	return &this->TokenString[0];
}

enum SMDToken CWzMemScript::GetToken()
{
	char ch;
	char* p;
	char TempString[100];

	this->TokenString[0]=0;	// Set String NULL

	do
	{
		if ( (ch=this->GetC ()) == (-1) )
		{
			return END;
		}
		else
		{	
			if (ch=='/')		// this is /
			{
				if((ch=this->GetC ()) == '/')
				{
					while ( ((ch=this->GetC()) != ('\n')) )	// End of Line (EOL)
					{
						continue;
					}
				}
			}
		}
	}
	while (isspace(ch) != 0);

	switch(ch)
	{

	case 0x23:	// #
		return this->CurrentToken=COMMAND;
		break;
	
	case 0x3B:	// ;
		return this->CurrentToken=SEMICOLON;
		break;
	
	case 0x2C:	// ,

		return this->CurrentToken=COMMA;
		break;
	
	case 0x7B:	// {
		return this->CurrentToken = LP;
		break;

	case 0x7D:	// }
		return this->CurrentToken = RP;
		break;
	// Nice Index Table :)	by Deathway
	case 0x2D:	//-
	case 0x2E:	//.
	case 0x30:	//0
	case 0x31:	//1
	case 0x32:	//2
	case 0x33:	//3
	case 0x34:	//4		
	case 0x35:	//5
	case 0x36:	//6
	case 0x37:	//7
	case 0x38:	//8
	case 0x39:	//9
		this->UnGetC (ch);
		p = TempString;
		
		while (((ch=this->GetC()) != -1) && ((ch == 0x2E) || (isdigit(ch) != 0) || (ch == 0x2D))) 
		{
			*p = ch;
			p++;
		}		
		*p = 0;
		this->TokenNumber = atof( TempString);
		return this->CurrentToken  = NUMBER ;
		break;

	case 0x22:	// "	
		p=&this->TokenString[0];
		
		while (((ch=this->GetC() ) != -1 ) && (ch != 0x22))	// nice
		{
			*p = ch;
			p++;
		}
		if (ch != 0x22 )
		{
			this->UnGetC(ch);
		}
		*p = 0;
		return this->CurrentToken = NAME;
		break;

	default:	
		if (isalpha(ch))
		{
			p=&this->TokenString[0];
			*p=ch;
			p++;

			while ( ((ch=this->GetC() ) != -1) && ( (ch == 0x2E) || (ch == 0x5F) || (isalnum(ch) != 0) ) )
			{
				*p=ch;
				p++;
				
			}

			this->UnGetC(ch);
			*p=0;
			this->CurrentToken=NAME;
			return NAME;
		}
		else
		{
			return this->CurrentToken = SMD_ERROR;
		}
		break;

	}

}


