//	GS-CS	1.00.90	JPN		-	Completed
#include "stdafx.h"
#include "WZScriptEncode.h"

unsigned char XorTable[10]={0xA1, 0xB2, 0xAA, 0x12, 0x23, 0xF1, 0xF3, 0xD3, 0x78, 0x02}; // For some reason this is 16

CWZScriptEncode::CWZScriptEncode()
{
	this->m_pBuffer=0;
	this->m_iBufferSize=0;
	this->m_iBufferCount=0;
	this->m_hFile=INVALID_HANDLE_VALUE;
}

CWZScriptEncode::~CWZScriptEncode()
{
	this->Close();
}

int CWZScriptEncode::Open(char* filename)
{
	int filesize;
	char* pBuffer;
	int iReadFileSize;

	if ( this->m_hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( this->m_hFile );
	}

	this->m_hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80 , NULL); // 0x80 == NORMAL

	if ( this->m_hFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	filesize = GetFileSize(this->m_hFile , NULL);
	pBuffer = (char*)malloc(filesize);
	ReadFile(this->m_hFile, (void*)pBuffer, filesize, (unsigned long*)&iReadFileSize, 0);

	if ( iReadFileSize == 0 )
	{
		this->Close();
		return 0;
	}

	this->Encode(pBuffer, iReadFileSize);
	this->SetScriptParsingBuffer(pBuffer, iReadFileSize);

	CloseHandle(this->m_hFile );
	return 1;
}

void CWZScriptEncode::Close()
{
	if ( this->m_pBuffer != 0)
	{
		free( this->m_pBuffer );
	}
}

void CWZScriptEncode::Encode(char* buffer, int size)
{
	int i;

	for (i=0;i<size; i++)
	{
		buffer[i]=buffer[i]^XorTable[i%10];	
	}
}

int CWZScriptEncode::GetC()
{
	if ( this->m_iBufferCount >= this->m_iBufferSize )
	{
		return -1;
	}
	else
	{
		return this->m_pBuffer [ this->m_iBufferCount++ ];
	}
}

void CWZScriptEncode::UnGetC(int ch)
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


void CWZScriptEncode::SetScriptParsingBuffer( char* buffer, int size)
{
	this->m_pBuffer = buffer;
	this->m_iBufferSize = size;
	this->m_iBufferCount = 0;
}

int CWZScriptEncode::GetNumber()
{
	return this->TokenNumber;
}

char* CWZScriptEncode::GetString()
{
	return &this->TokenString[0];
}

enum WZSMDToken CWZScriptEncode::GetToken()
{
	char ch;
	char* p;
	char TempString[100];

	this->TokenString[0]=0;	// Set String NULL

	do
	{
		if ( (ch=this->GetC ()) == (-1) )
		{
			return T_END;
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
		return this->CurrentToken=T_COMMAND;
		break;
	
	case 0x3B:	// ;
		return this->CurrentToken=T_SEMICOLON;
		break;
	
	case 0x2C:	// ,

		return this->CurrentToken=T_COMMA;
		break;
	
	case 0x7B:	// {
		return this->CurrentToken = T_LP;
		break;

	case 0x7D:	// }
		return this->CurrentToken = T_RP;
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
		return this->CurrentToken  = T_NUMBER ;
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
		return this->CurrentToken = T_NAME;
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
			this->CurrentToken=T_NAME;
			return T_NAME;
		}
		else
		{
			return this->CurrentToken = T_SMD_ERROR;
		}
		break;

	}

}
