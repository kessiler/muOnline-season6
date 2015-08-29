// CriticalSec.h: interface for the CCriticalSec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITICALSEC_H__16786BC1_2201_11D5_BBA8_00A02490CE8E__INCLUDED_)
#define AFX_CRITICALSEC_H__16786BC1_2201_11D5_BBA8_00A02490CE8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCriticalSec
{
    CRITICAL_SECTION	sec;

public:

    CCriticalSec();
    ~CCriticalSec();

    inline void Lock() {
        EnterCriticalSection(&sec);
    }
    inline void UnLock() {
        LeaveCriticalSection(&sec);
    }
};

class CCSLock
{
    CCriticalSec*	m_pCriticalSec;
public:

    CCSLock(CCriticalSec* pCriticalSec);
    ~CCSLock();
};

#endif // !defined(AFX_CRITICALSEC_H__16786BC1_2201_11D5_BBA8_00A02490CE8E__INCLUDED_)
