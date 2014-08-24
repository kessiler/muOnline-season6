// XMasAttackEventData.h: interface for the CXMasAttackEventData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMASATTACKEVENTDATA_H__52CA2363_7F35_4411_8856_1554CDCBECE5__INCLUDED_)
#define AFX_XMASATTACKEVENTDATA_H__52CA2363_7F35_4411_8856_1554CDCBECE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MonsterHerd.h"

class CXMasMonsterHerd : public MonsterHerd
{

public:

	CXMasMonsterHerd();
	virtual ~CXMasMonsterHerd();

	BYTE GetX();
	BYTE GetY();
	int GetMapNumber();

	virtual int Start();	// 4
	virtual int MonsterHerdItemDrop(LPOBJ lpObj);	// 8
};

//*****************************************************

class CEventTimer
{
public:
	CEventTimer();
	~CEventTimer();
	
	void SetSeconds(int Time);
	int GetSeconds();
	void SetMinutes(int Time);
	int GetMinutes();
	void SetMiliSeconds(int Time);
	int GetMiliSeconds();
private:
	int m_iTIME_MSEC_REMAIN; //
};

//*****************************************************

struct XMASMONSTER_DATA
{
	int m_iType;	// 0
	BOOL m_bDoRegen;	// 4
	BOOL m_bDoAttackFirst;	// 8
};

struct XMASMONSTER_EVENT_TIME
{
	int m_iHour;	// 0
	int m_iMinute;	// 4
};

class CXMasAttackEventData  
{
public:
	CXMasAttackEventData();
	//void SetMenualStart(BOOL bMenualStart); //4
	virtual ~CXMasAttackEventData();

	void Clear();
	BOOL Init(int arg1);
	//////////////////////////////////////////////////////////
	BOOL m_bDoEvent;	//4
	BOOL m_bHasData;	//8
	BOOL m_bMenualStart;	//C
	CRITICAL_SECTION m_critMonsterAddData;	//10
	std::vector<XMASMONSTER_EVENT_TIME> m_vtEventTime;	//28
	std::vector<XMASMONSTER_DATA> m_vtMonsterAddData;	//38
	std::vector<CXMasMonsterHerd*> m_vtXMasMonsterHerd;	// 48
	CEventTimer	m_EventTimer; //58
};

#endif // !defined(AFX_XMASATTACKEVENTDATA_H__52CA2363_7F35_4411_8856_1554CDCBECE5__INCLUDED_)
