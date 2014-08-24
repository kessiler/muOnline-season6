// CastleDeepEvent.cpp: implementation of the CCastleDeepEvent class.
// GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GameServer.h"
#include "CastleSiege.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#if (_GSCS==1)
#include "user.h"
#include "LogProc.h"
#include "CastleDeepEvent.h"
#include "..\include\readscript.h"
#include "GameMain.h"

CCastleDeepEvent g_CastleDeepEvent;

CCastleDeepEvent::CCastleDeepEvent() //Identical
{
	this->m_bDoEvent = FALSE;
	this->m_bHasData = FALSE;

	srand(time(NULL));

	InitializeCriticalSection(&this->m_critEventData);
}

CCastleDeepEvent::~CCastleDeepEvent() //Identical
{
	DeleteCriticalSection(&this->m_critEventData);
}

//////////////////////////////////////////////////////////////////////
// Class Members
//////////////////////////////////////////////////////////////////////

void CCastleDeepEvent::StartEvent() //Identical
{
	if(m_vtAssultType.size() <= 0)
	{
		LogAddTD("[CastleDeep Event] Start Fail - No Assult Data");
		SetState(CD_STATE_NONE);
		return;
	}

	int iCheckCount = 100;
	int iASSULT_RATE = 0;
	int bDoStart = FALSE;

	while(iCheckCount-- > 0)
	{
		int iRndIndex = rand()%m_vtAssultType.size();

		this->m_iCUR_ASSULT_TYPE = m_vtAssultType[iRndIndex];
		iASSULT_RATE = m_vtAssultTypeRate[iRndIndex];

		if(rand()%100 < iASSULT_RATE)
		{
			bDoStart = TRUE;
			break;
		}
	}

	if(bDoStart == FALSE)
	{
		LogAddTD("[CastleDeep Event] Start Fail - bDoStart == FALSE");
		SetState(CD_STATE_NONE);
		return;
	}

	this->m_iCUR_SPAWN_STEP = 0;

	//std::map<int,vector<CASTLEDEEP_SPAWNTIME > >::iterator it = this->m_mapSpawnTime.find(m_iCUR_ASSULT_TYPE); //HermeX Fix
	map<int,vector<CASTLEDEEP_SPAWNTIME> >::iterator it = (map<int,vector<CASTLEDEEP_SPAWNTIME> >::iterator)m_mapSpawnTime.find(m_iCUR_ASSULT_TYPE);

	if(it == m_mapSpawnTime.end())
	{
		SetState(CD_STATE_NONE);
		return;
	}

	m_mapRunCheck.clear();

	for(vector<CASTLEDEEP_SPAWNTIME>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
	{
		//CASTLEDEEP_SPAWNTIME * stSpawnTime = it2;
		m_mapRunCheck.insert(pair<int,int>(it2->m_iMonsterGroup,it2->m_iMonsterSpawnMinute)); //HermeX Fix
	}
}

void CCastleDeepEvent::StopEvent()
{
;
}

void CCastleDeepEvent::EnableEvent(int bDoEvent) //Identical
{
	this->m_bDoEvent = bDoEvent;

	if(this->m_bDoEvent != 0)
	{
		this->SetState(CD_STATE_CLOSED);
	}
	else
	{
		this->SetState(CD_STATE_NONE);
	}
}

int CCastleDeepEvent::Load(char * lpszFileName) //Identical
{
using namespace std;

	if(lpszFileName == NULL)
	{
		return false;
	}

	SMDFile = fopen(lpszFileName,"r");	//ok

	if(SMDFile == NULL)
	{
		MsgBox("[CastleDeep Event] Info file Load Fail [%s]",lpszFileName);
		return false;
	}

	EnterCriticalSection(&this->m_critEventData);
	this->m_vtAssultType.clear();
	this->m_vtAssultTypeRate.clear();
	this->m_mapSpawnTime.clear();
	this->m_mapMonsterInfo.clear();
	this->m_vtEventTime.clear();
	this->m_mapRunCheck.clear();
	LeaveCriticalSection(&this->m_critEventData);

	this->m_bHasData = FALSE;

	SMDToken Token;
	int type = -1;

	while( true )
	{
		Token = GetToken();

		if(Token == END)
		{
			break;
		}

		type = (int)TokenNumber;

		while( true )
		{
			if(type == 0)
			{
				CASTLEDEEP_EVENTTIME pRET;

				Token = GetToken();
				if(!strcmp("end",TokenString))
					break;
				
				pRET.m_iHour = (int)TokenNumber;
				Token = GetToken();
				pRET.m_iMinute = (int)TokenNumber;
				this->m_vtEventTime.push_back(pRET);
			}
			else if(type == 1)
			{
				std::vector<CASTLEDEEP_MONSTERINFO> vtTempMonsterInfo; //HermeX Fix
				std::vector<CASTLEDEEP_SPAWNTIME> vtTempSpawnTime; //HermeX Fix

				int iAssultType;
				int iAssultRate;

				Token = GetToken();
				if(!strcmp("end",TokenString))
					break;

				iAssultType = (int)TokenNumber;

				Token = GetToken();
				iAssultRate = (int)TokenNumber;

				this->m_mapMonsterInfo.insert(std::pair<int,vector<CASTLEDEEP_MONSTERINFO> >(iAssultType,vtTempMonsterInfo)); //HermeX Fix
				this->m_mapSpawnTime.insert(std::pair<int,vector<CASTLEDEEP_SPAWNTIME> >(iAssultType,vtTempSpawnTime)); //HermeX Fix

				this->m_vtAssultType.push_back(iAssultType);
				this->m_vtAssultTypeRate.push_back(iAssultRate);
			}
			else if(type == 2)
			{
				CASTLEDEEP_SPAWNTIME stSpawnTime;
				int iAssultType;

				Token = GetToken();
				if(!strcmp("end",TokenString))
					break;

				iAssultType = (int)TokenNumber;

				Token = GetToken();
				stSpawnTime.m_iMonsterGroup = (int)TokenNumber;

				Token = GetToken();
				stSpawnTime.m_iMonsterSpawnMinute = (int)TokenNumber;

				EnterCriticalSection(&this->m_critEventData);

				std::map<int,vector<CASTLEDEEP_SPAWNTIME > >::iterator it = this->m_mapSpawnTime.find(iAssultType); //HermeX Fix

				if(it != this->m_mapSpawnTime.end())
				{
					it->second.push_back(stSpawnTime);
				}

				LeaveCriticalSection(&this->m_critEventData);
			}
			else if(type == 3)
			{
				CASTLEDEEP_MONSTERINFO stMonsterInfo;
				int iAssultType;

				Token = GetToken();
				if(!strcmp("end",TokenString))
					break;

				iAssultType = (int)TokenNumber;

				Token = GetToken();
				stMonsterInfo.m_iMonsterGroup = (int)TokenNumber;

				Token = GetToken();
				stMonsterInfo.m_iMonsterType = (int)TokenNumber;

				Token = GetToken();
				stMonsterInfo.m_iMonsterNumber = (int)TokenNumber;

				Token = GetToken();
				stMonsterInfo.m_iSX = (int)TokenNumber;

				Token = GetToken();
				stMonsterInfo.m_iSY = (int)TokenNumber;

				Token = GetToken();
				stMonsterInfo.m_iDX = (int)TokenNumber;

				Token = GetToken();
				stMonsterInfo.m_iDY = (int)TokenNumber;

				EnterCriticalSection(&this->m_critEventData);

				std::map<int,vector<CASTLEDEEP_MONSTERINFO > >::iterator it = this->m_mapMonsterInfo.find(iAssultType); //HermeX Fix

				if(it != this->m_mapMonsterInfo.end())
				{
					it->second.push_back(stMonsterInfo);
				}

				LeaveCriticalSection(&this->m_critEventData);
			}
		}
	}

	fclose(SMDFile);
	LogAdd("%s file load!",lpszFileName);
	this->m_bHasData = TRUE;
	return true;
}

void CCastleDeepEvent::CheckSync() //Identical
{
	if(this->m_vtEventTime.empty())
	{
		LogAddC(2,"[CastleDeep Event] No Schedule Time Data");
		this->SetState(CD_STATE_NONE);
		return;
	}

	_SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	int bTIME_CHANGED = 0;

	for(vector<CASTLEDEEP_EVENTTIME>::iterator it = this->m_vtEventTime.begin(); it != this->m_vtEventTime.end();it++)
	{
		CASTLEDEEP_EVENTTIME & pRET = * it;

		if( (sysTime.wHour * 60 + sysTime.wMinute) < (pRET.m_iHour * 60 + pRET.m_iMinute) )
		{
			if( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute) )
			{
				bTIME_CHANGED = 1;
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;
			}
		}
	}

	if(bTIME_CHANGED == 0)
	{
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;
		for(vector<CASTLEDEEP_EVENTTIME>::iterator it = this->m_vtEventTime.begin(); it != this->m_vtEventTime.end();it++)
		{
			CASTLEDEEP_EVENTTIME pRET = *it;

			if( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute) )
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;
			}
		}
	}

	switch(bTIME_CHANGED)
	{
	case 1:
		this->m_iTIME_MSEC_REMAIN = ( ( (iMIN_HOUR * 60) * 60 + iMIN_MINUTE * 60 ) - ( (sysTime.wHour * 60) * 60 + sysTime.wMinute * 60 + sysTime.wSecond) ) * 1000;
		break;
	case 2:
		this->m_iTIME_MSEC_REMAIN = ( ( ((iMIN_HOUR + 24) * 60) * 60 + iMIN_MINUTE * 60 ) - ( (sysTime.wHour * 60) * 60 + sysTime.wMinute * 60 + sysTime.wSecond ) ) * 1000;
		break;
	default:
		LogAddC(2,"[CastleDeep Event] No Schedule Time Data");
		this->SetState(CD_STATE_NONE);
		return;
	}

	this->m_iTIME_TICK_COUNT = GetTickCount();
	LogAddTD("[CastleDeep Event] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		this->m_iTIME_MSEC_REMAIN / 60000,iMIN_HOUR,iMIN_MINUTE);
	this->m_iTIME_NOTIFY_COUNT = 5;
}

void CCastleDeepEvent::SetState(int iSTATE) //Identical
{
	if(m_iEVENT_STATE < 0 || m_iEVENT_STATE > 2)
		return;

	m_iEVENT_STATE = iSTATE;

	switch(m_iEVENT_STATE)
	{
		case 0: SetState_None(); break;
		case 1: SetState_Closed(); break;
		case 2: SetState_Playing(); break;
	}
}

void CCastleDeepEvent::SetState_None() //Identical
{
	this->StopEvent();
}

void CCastleDeepEvent::SetState_Closed() //Identical
{
	StopEvent();

	if(m_bDoEvent != 0)
	{
		CheckSync();
	}
	else
	{
		SetState(CD_STATE_NONE);
	}
}

void CCastleDeepEvent::SetState_Playing() //Identical
{
	m_iTIME_MSEC_REMAIN = 0;
	m_bCheckEnd = 0;
	m_uCheckEndTick = 0;

	ClearMonster(); //Fixed
	StartEvent(); //Fixed 
}

void CCastleDeepEvent::Run() //Identical
{
	if(m_bDoEvent)
	{
		switch(m_iEVENT_STATE)
		{
			case 0: ProcState_None(); break;
			case 1: ProcState_Closed(); break;
			case 2: ProcState_Playing(); break;
		}
	}
}

void CCastleDeepEvent::ProcState_Closed() //Identical
{
	int iTICK_MSEC = GetTickCount() - m_iTIME_TICK_COUNT;

	if(iTICK_MSEC >= 1000)
	{
		m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		m_iTIME_TICK_COUNT = GetTickCount();

		if(m_bDoEvent != 0)
		{
			if(m_iTIME_MSEC_REMAIN <= 300000
				&& m_iTIME_MSEC_REMAIN > 0
				&& m_iTIME_MSEC_REMAIN / 60000 != m_iTIME_NOTIFY_COUNT)
			{
				m_iTIME_NOTIFY_COUNT = m_iTIME_MSEC_REMAIN / 60000;

				if(m_iTIME_NOTIFY_COUNT + 1 == 5)
				{
					char szTemp[256];
					wsprintf(szTemp,lMsg.Get(3224)); // # translation here
					g_CastleSiege.SendMapServerGroupMsg(szTemp);
				}
			}
		}

		LogAddC(3,"[CastleDeep Event] ProcState_Closed()	LEFT-MSEC:%d",m_iTIME_MSEC_REMAIN);
	}

	if(m_iTIME_MSEC_REMAIN <= 0)
	{
		char szTemp[256];
		wsprintf(szTemp,lMsg.Get(3225)); // # translation here
		g_CastleSiege.SendMapServerGroupMsg(szTemp);

		if(m_bDoEvent != 0)
		{
			SetState(CD_STATE_PLAYING);
		}
		else
		{
			SetState(CD_STATE_NONE);
		}

		LogAddTD("[CastleDeep Event] - Event Started");
	}
}

void CCastleDeepEvent::ProcState_Playing() //Identical
{
using namespace std;
	int iTICK_MSEC = GetTickCount() - m_iTIME_TICK_COUNT;

	if(iTICK_MSEC >= 1000)
	{
		m_iTIME_MSEC_REMAIN +=  iTICK_MSEC;
		m_iTIME_TICK_COUNT = GetTickCount();

		for ( std::map<int, int>::iterator it = this->m_mapRunCheck.begin() ; it != this->m_mapRunCheck.end() ; it++) //HermeX Fix
		{
			int iSpawnMin = it->second;

			if(iSpawnMin == m_iTIME_MSEC_REMAIN / 60000)
			{
				AddMonster(m_iCUR_ASSULT_TYPE,it->first);
				m_mapRunCheck.erase(it);
				break;
			}
		}

		LogAddC(3,"[CastleDeep Event] ProcState_Playing()	LEFT-MSEC:%d",m_iTIME_MSEC_REMAIN);
	}

	if(m_mapRunCheck.size() <= 0)
	{
		if(m_bCheckEnd == 0)
		{
			m_bCheckEnd = 1;
			m_uCheckEndTick = GetTickCount();
		}
		else
		{
			if(m_uCheckEndTick + 600000 < GetTickCount()) //HermeX Fix
			{
				char szTemp[256];
				wsprintf(szTemp,lMsg.Get(3226)); // # translation here (HermeX Fix)
				AllSendServerMsg(szTemp);

				if(m_bDoEvent != 0)
				{
					SetState(CD_STATE_CLOSED);
				}
				else
				{
					SetState(CD_STATE_NONE);
				}
			}
		}

		LogAddTD("[CastleDeep Event] - Event Ended");
	}
}

void CCastleDeepEvent::ClearMonster() //Identical
{
	for(int iIndex = 0; iIndex < OBJ_MAXMONSTER; iIndex++)
	{
		if(gObjIsConnected(iIndex) && gObj[iIndex].m_Attribute == 62)
		{
			gObjDel(iIndex);
		}
	}
}

void CCastleDeepEvent::AddMonster(int iAssultType,int iGroup) //Identical
{
	//std::map<int,vector<CASTLEDEEP_MONSTERINFO > >::iterator it = this->m_mapMonsterInfo.find(iAssultType); //HermeX Fix
	map<int,vector<CASTLEDEEP_MONSTERINFO >,less<int> >::iterator it =  (map<int,vector<CASTLEDEEP_MONSTERINFO >,less<int> >::iterator)this->m_mapMonsterInfo.find(iAssultType);
	
	if(it == this->m_mapMonsterInfo.end())
	{
		return;
	}

	CASTLEDEEP_MONSTERINFO * it2;

	for(std::vector<CASTLEDEEP_MONSTERINFO>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
	{
		CASTLEDEEP_MONSTERINFO & stMonsterInfo = * it2;

		if(stMonsterInfo.m_iMonsterGroup != iGroup)
			continue;

		BYTE cSX;
		BYTE cSY;

		for(int iMonCount = 0; iMonCount < stMonsterInfo.m_iMonsterNumber; iMonCount++)
		{
			if(gObjGetRandomFreeArea(MAP_INDEX_CASTLESIEGE,(BYTE &)cSX,(BYTE &)cSY,
				stMonsterInfo.m_iSX,stMonsterInfo.m_iSY,stMonsterInfo.m_iDX,stMonsterInfo.m_iDY,MAP_INDEX_CASTLESIEGE) == TRUE)
			{
				int result = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

				if(result >= 0)
				{
					gObj[result].m_PosNum = -1;
					gObj[result].X = cSX;
					gObj[result].Y = cSY;
					gObj[result].MapNumber = MAP_INDEX_CASTLESIEGE;
					gObj[result].TX = gObj[result].X;
					gObj[result].TY = gObj[result].Y;
					gObj[result].m_OldX = gObj[result].X;
					gObj[result].m_OldY = gObj[result].Y;
					gObj[result].Dir = 1;
					gObj[result].StartX = gObj[result].X;
					gObj[result].StartY = gObj[result].Y;

					gObjSetMonster(result,stMonsterInfo.m_iMonsterType);

					gObj[result].m_Attribute = 62;
					gObj[result].MaxRegenTime = 0;
					gObj[result].Dir = rand()%8;
					gObj[result].DieRegen = 0;
					gObj[result].RegenTime = 1;
					gObj[result].MaxRegenTime = 1000;
					gObj[result].m_dwLastCheckTick = GetTickCount();
				}
			}
		}
	}
}
//#endif