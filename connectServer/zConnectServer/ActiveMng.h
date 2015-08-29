// ActiveMng.h: interface for the CActiveMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVEMNG_H__1B9D7659_EF5E_4CCB_8BC1_5BF4CAEF3BBA__INCLUDED_)
#define AFX_ACTIVEMNG_H__1B9D7659_EF5E_4CCB_8BC1_5BF4CAEF3BBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CActiveMng
{
public:
    CActiveMng();
    virtual ~CActiveMng();

    void SetMode(DWORD mode);
    DWORD GetMode();
    void CheckTime(DWORD chTime);

    void StartThread();
    void StopThread();

public:
    DWORD m_dwStateMode;
    DWORD m_dwTime;
    DWORD m_threadId;
    HANDLE m_thread;
    CRITICAL_SECTION m_critSect;
    int m_running;
};

DWORD WINAPI ThreadProc(LPVOID lpParameter);

#endif // !defined(AFX_ACTIVEMNG_H__1B9D7659_EF5E_4CCB_8BC1_5BF4CAEF3BBA__INCLUDED_)
