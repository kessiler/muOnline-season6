// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef MOVECHECK_H
#define MOVECHECK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMoveCheck
{

public:

	CMoveCheck();
	virtual ~CMoveCheck();
	void Init();
	BOOL Insert(int x, int y);
	BOOL Check();

private:

	DWORD m_Time;	// 4
	int m_PosX[5];	// 8
	int m_PosY[5];	// 1C
	int m_PosCount;	// 30
	int m_TmpPosX;	// 34
	int m_TmpPosY;	// 38
};

#endif