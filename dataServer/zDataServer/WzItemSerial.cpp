// WzItemSerial.cpp: implementation of the CWzItemSerial class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WzItemSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWzItemSerial gWzItemSerial;

CWzItemSerial::CWzItemSerial(): m_dwItemSerial(0), m_dwLastSerial(0)
{
	InitializeCriticalSection(&m_csItemSerial);
}

CWzItemSerial::~CWzItemSerial()
{
	DeleteCriticalSection(&m_csItemSerial);
}

int CWzItemSerial::MakeSerial()
{
	if( gGSDbSet.GetItemSerial(m_dwItemSerial, 3000) == FALSE )
	{
		return FALSE;
	}

	m_dwLastSerial = m_dwItemSerial + 3000;
	return 1;
}

DWORD CWzItemSerial::GetSerial()
{
	EnterCriticalSection(&m_csItemSerial);
	
	if ( m_dwItemSerial >= m_dwLastSerial )
	{
		MakeSerial();
	}

	DWORD retserial = m_dwItemSerial;

	m_dwItemSerial++;

	LeaveCriticalSection(&m_csItemSerial);

	return retserial;
}