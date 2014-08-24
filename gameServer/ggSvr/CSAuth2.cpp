// CSAuth2.cpp: implementation of the CCSAuth2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSAuth2.h"
#include "GGSvr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCSAuth2::CCSAuth2()
{
	this->m_pProtocol = ggGlobal.lpCurrentAuthProtocol;
	memset(&this->m_AuthQuery, 0, sizeof(this->m_AuthQuery));
	memset(&this->m_AuthAnswer, 0, sizeof(this->m_AuthAnswer));
	this->m_bPrtcRef = 0;
	this->m_dwUserFlag = 0;
	this->m_pProtocol = NULL;
	srand(time(NULL));
}

CCSAuth2::~CCSAuth2()
{
	if ( this->m_bPrtcRef == TRUE )
	{
		this->m_pProtocol->dwQueryCount--;

		if ( this->m_pProtocol->dwQueryCount == 0 )
		{
			if ( this->m_pProtocol !=  ggGlobal.lpCurrentAuthProtocol )
			{
				UnloadAuthProtocol(this->m_pProtocol);
			}
		}
	}
}



void CCSAuth2::Init()
{
	this->m_pProtocol = ggGlobal.lpCurrentAuthProtocol;
	memset(&this->m_AuthQuery, 0, sizeof(this->m_AuthQuery));
	memset(&this->m_AuthAnswer, 0, sizeof(this->m_AuthAnswer));
	this->m_bPrtcRef = 0;
	this->m_dwUserFlag = 0;
	this->m_pProtocol = NULL;
	srand(time(NULL));
}



DWORD CCSAuth2::GetAuthQuery()
{
	if ( !this->m_pProtocol )
		return 4;

	if ( !this->m_bPrtcRef )
	{
		this->m_pProtocol->dwQueryCount++;

		if ( ggGlobal.szGGAuthName[0] == 2 )
		{
			if ( ggGlobal.lpCurrentAuthProtocol->dwQueryCount > ggGlobal.nMaxCount )
			{
				ggGlobal.szGGAuthName[0] |= 2;
			}
		}

		this->m_bPrtcRef = TRUE;
	}

	return this->m_pProtocol->PtrcGetAuthQuery(this->m_pProtocol->unkC, &this->m_dwUserFlag, &this->m_AuthQuery, &this->m_AuthAnswer);
}


DWORD CCSAuth2::CheckAuthAnswer()
{
	if ( !this->m_pProtocol )
		return 4;

	if ( this->m_AuthAnswer.dwIndex != this->m_pProtocol->dwPtrcVersion )
	{
		if ( ggGlobal.lpCurrentAuthProtocol->unkC != 2 )
			return 12;

		if ( ggGlobal.lpCurrentAuthProtocol->lpPrevProtocol )
			return 12;

		this->m_pProtocol->dwQueryCount--;
		this->m_pProtocol = ggGlobal.lpCurrentAuthProtocol->lpPrevProtocol;
		ggGlobal.lpCurrentAuthProtocol->dwQueryCount++;
	}

	return this->m_pProtocol->PtrcCheckAuthAnswer(this->m_pProtocol->unkC, &this->m_dwUserFlag, &this->m_AuthQuery, &this->m_AuthAnswer);
}


