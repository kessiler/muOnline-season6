// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef MULTICHECKSUM_H
#define MULTICHECKSUM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#define MAX_MULTICHECKSUM	10

class CMultiCheckSum
{
public:

	int LoadFile();


	CMultiCheckSum();
	virtual ~CMultiCheckSum();

	void Init();
	void SetFilePath(char* filepath);
	int CompareCheckSum(int aIndex, DWORD TableNum, DWORD Key);

private:

	BOOL LoadFile(char* filename, int index);
	int m_dwLoaded[MAX_MULTICHECKSUM];	// 4
	DWORD m_dwgCheckSum[MAX_MULTICHECKSUM][MAX_CHECKSUM_KEY];	// 2C
	char m_sFilePath[255];	// A02C

	
};

extern CMultiCheckSum g_MultiChecksum;

#endif /*MULTICHECKSUM_H*/