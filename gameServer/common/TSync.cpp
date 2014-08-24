//	GS-CS	1.00.90	JPN	-	Completed
#include "stdafx.h"
#include "TSync.h"

TSync::TSync()
{
	this->m_nLock=0;
	InitializeCriticalSection(&this->m_cs);
}

TSync::~TSync()
{
	DeleteCriticalSection(&this->m_cs);
}

void TSync::Lock()
{
	EnterCriticalSection(&this->m_cs);
	InterlockedIncrement(&this->m_nLock);
}

void TSync::Unlock()
{
	long nResult=InterlockedDecrement(&this->m_nLock );
	LeaveCriticalSection(&this->m_cs );
}




