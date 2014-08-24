// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef TSYNC_H
#define TSYNC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class TSync
{
public:

	TSync();
	~TSync();

	void Lock();
	void Unlock();

private:

	CRITICAL_SECTION m_cs;	// 0
	LONG m_nLock;	// 18
};

#endif