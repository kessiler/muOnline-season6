// CastleDeepEvent.h: interface for the CCastleDeepEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLEDEEPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_)
#define AFX_CASTLEDEEPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

//#if (_GSCS==1)

#define CD_STATE_NONE 0
#define CD_STATE_CLOSED 1
#define CD_STATE_PLAYING 2

struct CASTLEDEEP_MONSTERINFO
{
	int  m_iMonsterGroup;  //0x0
	int  m_iMonsterType;  //0x4
	int  m_iMonsterNumber;  //0x8
	int  m_iSX;  //0xC
	int  m_iSY;  //0x10
	int  m_iDX;  //0x14
	int  m_iDY;  //0x18
};

struct CASTLEDEEP_SPAWNTIME
{
	int  m_iMonsterGroup;  //0x0
	int  m_iMonsterSpawnMinute;  //0x4
};

struct CASTLEDEEP_EVENTTIME
{
	int  m_iHour;  //0x0
	int  m_iMinute	;  //0x4
};

class CCastleDeepEvent  
{
public:
	int m_bDoEvent;	// 4
	int m_bHasData;	// 8
	int m_bCheckEnd;	// C
	UINT m_uCheckEndTick;	// 10
	int m_iTIME_MSEC_REMAIN;	// 14
	int m_iTIME_MIN_PLAY;	// 18
	int m_iTIME_TICK_COUNT;	// 1C
	int m_iTIME_NOTIFY_COUNT;	// 20
	int m_iEVENT_STATE;	// 24
	int m_iCUR_ASSULT_TYPE;	// 28
	int m_iCUR_SPAWN_STEP;	// 2C
	std::map<int,vector<CASTLEDEEP_MONSTERINFO > > m_mapMonsterInfo;	// 30
	std::map<int,vector<CASTLEDEEP_SPAWNTIME > > m_mapSpawnTime;	// 40
	std::vector<CASTLEDEEP_EVENTTIME> m_vtEventTime;	// 50
	std::vector<int> m_vtAssultType;	// 60
	std::vector<int> m_vtAssultTypeRate;	// 70
	std::map<int,int> m_mapRunCheck;	// 80
	_RTL_CRITICAL_SECTION m_critEventData;	// 90

public:
	CCastleDeepEvent();
	virtual ~CCastleDeepEvent();

	void StartEvent();
	void StopEvent(); //HermeX Fix
	void EnableEvent(int bDoEvent);
	int Load(char * lpszFileName);
	void Run();
	void CheckSync();
	void SetState(int iSTATE);
	void ProcState_None() {};
	void ProcState_Closed();
	void ProcState_Playing();
	void SetState_None();
	void SetState_Closed();
	void SetState_Playing();
	void ClearMonster();
	void AddMonster(int iAssultType,int iGroup);
};

extern CCastleDeepEvent g_CastleDeepEvent;

//#endif

#endif // !defined(AFX_CASTLEDEEPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_)
