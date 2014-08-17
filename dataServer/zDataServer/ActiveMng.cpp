// ActiveMng.cpp: implementation of the CActiveMng class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dataserver.h"
#include "ActiveMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActiveMng::CActiveMng()
{
	InitializeCriticalSection(&m_critSect);
	SetMode(AM_NONE);
	m_running = 0;
}

CActiveMng::~CActiveMng()
{
	DeleteCriticalSection(&m_critSect);
}

void CActiveMng::SetMode(DWORD mode)
{
	EnterCriticalSection(&m_critSect);

	m_dwStateMode = mode;
	m_dwTime = GetTickCount();

	LeaveCriticalSection(&m_critSect);
}

DWORD CActiveMng::GetMode()
{
	return m_dwStateMode;
}

void CActiveMng::CheckTime(DWORD chTime)
{
	if(m_dwStateMode == AM_STANDBY)
	{
		return;
	}

	if(GetTickCount() - m_dwTime > chTime)
	{
		m_dwStateMode = AM_STANDBY;
		LogAdd("StateMode Change %d", m_dwStateMode);
	}
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	CActiveMng* pThis = (CActiveMng *)lpParameter;

	EnterCriticalSection(&pThis->m_critSect);

	pThis->m_running = 1;

	LeaveCriticalSection(&pThis->m_critSect);

	while ( true )
	{
		EnterCriticalSection(&pThis->m_critSect);
		pThis->CheckTime(30000);
		LeaveCriticalSection(&pThis->m_critSect);
		Sleep(1000);
	}

	return FALSE;
}

void CActiveMng::StartThread()
{
	EnterCriticalSection(&m_critSect);

	m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, &m_threadId);

	LeaveCriticalSection(&m_critSect);
}

void CActiveMng::StopThread()
{
	EnterCriticalSection(&m_critSect);

	m_running = 0;

	if(m_thread != NULL)
	{
		int result = WaitForSingleObject(m_thread, 10000);

		if (result == PST_TCPIP_TELNET )
		{
			TerminateThread(m_thread, 0);
		}
		
		CloseHandle(m_thread);
		m_thread = 0;
	}

	LeaveCriticalSection(&m_critSect);
}