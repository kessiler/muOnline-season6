// CriticalSec.cpp: implementation of the CCriticalSec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CriticalSec.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCriticalSec::CCriticalSec()
{
#if _WIN32_WINNT >= 500
    InitializeCriticalSectionAndSpinCount(&sec, 4000 );
#else
    InitializeCriticalSection(&sec);
#endif
}

CCriticalSec::~CCriticalSec()
{
    DeleteCriticalSection(&sec);
}


CCSLock::CCSLock(CCriticalSec* pCriticalSec)
{
    if( !pCriticalSec )
        return;

    m_pCriticalSec = pCriticalSec;

    m_pCriticalSec->Lock();
}

CCSLock::~CCSLock()
{
    m_pCriticalSec->UnLock();
}

