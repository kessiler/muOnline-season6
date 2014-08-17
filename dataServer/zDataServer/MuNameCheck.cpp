// MuNameCheck.cpp: implementation of the CMuNameCheck class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MuNameCheck.h"
#include "ReadScript.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMuNameCheck::CMuNameCheck()
{
	m_nMuNameCount = 0;
}

CMuNameCheck::~CMuNameCheck()
{

}

int CMuNameCheck::NameAdd(char *sz_name)
{
	if(strlen(sz_name) < 1)
	{
		return 0;
	}

	memcpy(m_szName[m_nMuNameCount], sz_name, MAX_MUNAMECHECK_SIZE);

	m_szName[m_nMuNameCount][MAX_MUNAMECHECK_SIZE-1] = 0;

	if(m_nMuNameCount < MAX_MUNAMECHECK_COUNT-1)
	{
		m_nMuNameCount++;
	}
	else
	{
//		MsgBox("Error : System Name Overflow %d", m_nMuNameCount);
		return 0;
	}

	return 1;
}

int CMuNameCheck::CmpString(char *cmp_name)
{
	if(strlen(cmp_name) < 1)
	{
		return 0;
	}

	for ( int i = 0; i < m_nMuNameCount; ++i )
    {
		if (m_szName[i][0] == *cmp_name && 
			m_szName[i][1] == cmp_name[1] && 
			!strcmp(m_szName[i], cmp_name) )
		{
			return 1;
		}
    }

	return 0;
}

#pragma warning ( disable : 4244 )
int CMuNameCheck::NameLoad(char *filename)
{
	SMDFile = fopen(filename, "r");

	if(SMDFile == NULL)
	{
		return 0;
	}

	while( true )
	{
		SMDToken Token = GetToken();	if(Token == END) break;

		int iIndex;

		if(Token == NUMBER)
		{
			iIndex = (int)TokenNumber;

			while( true )
			{
				Token = GetToken();

				int iIndex2 = TokenNumber; //unused :)

				if(Token == NAME)
				{
					if(strcmp("end", TokenString) == 0) break;
				}

				if(Token == NAME) NameAdd(TokenString);
			}
		}
	}

	fclose(SMDFile);

	return 1;
}