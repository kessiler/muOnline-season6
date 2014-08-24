// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef DRAGONEVENT_H
#define DRAGONEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDragonEvent
{

public:

	CDragonEvent();
	virtual ~CDragonEvent();

	BYTE GetState(){return this->EventState;}	// Dagon event line 30.h
	BYTE GetMapNumber(){return this->m_MapNumber;}	// Dragon event line 35.h 
	void Init();
	void Start();
	void End();
	void Run();
	void DragonActive();
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;}
	void Start_Menual();
	void End_Menual();

private:

	BYTE EventState;	// 4
	DWORD EventStartTime;	// 8
	BYTE m_MapNumber;	// C
	BOOL m_bMenualStart;	// 10

};

#endif