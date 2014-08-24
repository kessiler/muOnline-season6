// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

#ifndef WHISPERCASH_H
#define WHISPERCASH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct WCHSH_STRUCT
{
	BOOL Live;	// 0
	char szId[10];	// 4
	int m_Index;	// 10
	DWORD m_Time;	// 14
};



class CWhisperCash
{

public:

	CWhisperCash();
	virtual ~CWhisperCash();

	int AddCash(char* id);
	void DelCash(char* id);
	int SearchCash(char* id);
	void TimeCheckCashDelete();
	//void __thiscall Init();
	
private:

	WCHSH_STRUCT WChash[2];	// 4
	int WChashCount;	// 34

};

#endif