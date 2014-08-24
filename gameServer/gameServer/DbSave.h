// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef DBSAVE_H
#define DBSAVE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "WZQueue.h"

class CDbSave
{
public:

	CDbSave();
	virtual ~CDbSave();

	BOOL Initialize();
	BOOL Feee();
	BOOL Add(LPBYTE pObject, int nSize, BYTE headcode,  int index);
	DWORD ThreadProc();
	BOOL Begin();
	void End();
  
private:

	WZQueue * m_lpWzQueue;	// 4
	HANDLE m_ThreadHandle;	// 8
	DWORD m_dwThreadID;	// C
	BOOL m_bIsRunning;	// 10
	CRITICAL_SECTION criti;	// 14
};

#endif