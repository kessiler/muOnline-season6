//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "EventManagement.h"
#include "LogProc.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "EledoradoEvent.h"
#include "RingAttackEvent.h"
#include "XMasAttackEvent.h"
#include "..\include\readscript.h"

CEventManagement g_EventManager;
CEventItemList	g_EventItemList; //nice position wz

CEventManagement::CEventManagement()
{
	this->m_bEventStart = false;
	this->m_bHasData = false;
	this->Clear();
}

CEventManagement::~CEventManagement()
{
	return;
}

void CEventManagement::Clear()
{
	this->m_vtEventTime.clear();
	this->m_mapEventObj.clear();
	this->m_wToday_Year = 0;
	this->m_wToday_Month = 0;
	this->m_wToday_Day = 0;
}

void CEventManagement::Init(BOOL bEventStart)
{
	this->m_bEventStart = bEventStart;

	if ( this->m_bHasData != false && this->m_bEventStart != false )
	{
		SYSTEMTIME sysTime;

		GetLocalTime(&sysTime);
		this->m_wToday_Year = sysTime.wYear;
		this->m_wToday_Month = sysTime.wMonth;
		this->m_wToday_Day = sysTime.wDay;

		std::map<int, void *>::iterator _Itor;
		
		_Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_DRAGONHERD )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_UNDERTROOP )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_ELDORADO )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}


		_Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_ELDORADO ) //wtf?
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		//Season 4.5 addon
		_Itor = this->m_mapEventObj.find(EVENT_ID_XMASATTACK);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CXMasAttackEvent * lpEvent = (CXMasAttackEvent *)_Itor->second;
			
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_XMASATTACK )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

	}
	else
	{
		std::map<int, void *>::iterator _Itor;
		
		_Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

	
		_Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		//Season 4.5 addon
		_Itor = this->m_mapEventObj.find(EVENT_ID_XMASATTACK);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CXMasAttackEvent * lpEvent = (CXMasAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}
	}
}

void CEventManagement::Run()
{
	if ( this->m_bEventStart == false || this->m_bHasData == false || this->m_vtEventTime.empty() != FALSE )
	{
		return;
	}

	SYSTEMTIME sysTime;
	std::vector<EVENT_ID_TIME>::iterator it;

	GetLocalTime(&sysTime);

	if ( this->m_wToday_Year != sysTime.wYear || this->m_wToday_Month != sysTime.wMonth || this->m_wToday_Day != sysTime.wDay )
	{
		for ( it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end() ; it++ )
		{
			(*(it)).m_bEventStarted = false;
		}

		LogAddTD("○●[Event Management] 날짜 변경됨. %02d %02d %02d", this->m_wToday_Year, this->m_wToday_Month, this->m_wToday_Day);

		this->m_wToday_Year = sysTime.wYear;
		this->m_wToday_Month = sysTime.wMonth;
		this->m_wToday_Day = sysTime.wDay;
	}

	for ( it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end() ; it++ )
	{
		if ( sysTime.wHour == (*(it)).m_iHour && sysTime.wMinute == (*(it)).m_iMinute && (*(it)).m_bEventStarted == false )
		{
			LogAddTD("○●[Event Management] 이벤트시작함. (%d) %02d %02d (state=%d)", (*(it)).m_eEventKind , (*(it)).m_iHour, sysTime.wMinute , (*(it)).m_bEventStarted);

			(*(it)).m_bEventStarted = true;
			this->StartEvent((*(it)).m_eEventKind);
		}
	}
}

void CEventManagement::StartEvent(int eEventKind)
{
	switch ( eEventKind )
	{
		case EVENT_ID_DRAGONHERD:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_UNDERTROOP:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_ELDORADO:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_WHITEMAGE:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_XMASATTACK:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_XMASATTACK);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CXMasAttackEvent * lpEvent = (CXMasAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;
	}
}

bool CEventManagement::Load(LPSTR lpszFileName)
{
	this->m_bHasData = false;
	this->Clear();

	SMDFile = fopen(lpszFileName, "r");	//ok

	if ( SMDFile == NULL )
	{
		MsgBox("[DOTH] Info file Load Fail [%s]", lpszFileName);

		return false;
	}

	int Token;
	int type = -1;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type == 0 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				EVENT_ID_TIME EIT;

				EIT.m_eEventKind = TokenNumber;

				Token = GetToken();
				EIT.m_iHour = TokenNumber;

				Token = GetToken();
				EIT.m_iMinute = TokenNumber;

				EIT.m_bEventStarted = false;

				this->m_vtEventTime.push_back(EIT);
			}
		}
	}

	fclose(SMDFile);
	LogAdd("[DOTH] %s file load!", lpszFileName);

	this->m_bHasData = true;

	return true;
}

void CEventManagement::RegEvent(int eEventKind, void * lpEventObj)
{
	this->m_mapEventObj.insert( std::pair<int, void *>(eEventKind, lpEventObj) );
}

CEventItemList::CEventItemList()
{
	//empty
}

CEventItemList::~CEventItemList()
{
	this->m_vtEventItemList.clear();
}

BOOL CEventItemList::Load(LPSTR lpszFileName) //004A5950
{
	if ( lpszFileName == NULL || !strcmp(lpszFileName, ""))
	{
		LogAddTD("[EventItemList.txt] - File load error : File Name Error");
		return FALSE;
	}

	SMDFile = fopen(lpszFileName, "r");	//ok

	if ( SMDFile == NULL )
	{
		MsgBox("[DOTH] Info file Load Fail [%s]", lpszFileName);

		return FALSE;
	}

	int Token;
	int type = -1;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type == 0 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				EVENT_ID_ITEM_LIST EIIL;

				EIIL.m_btSection = TokenNumber;

				Token = GetToken();
				EIIL.m_btType = TokenNumber;

				Token = GetToken();
				EIIL.m_btIndex = TokenNumber;

				Token = GetToken();
				EIIL.m_btLevel = TokenNumber;

				Token = GetToken();
				EIIL.m_btDur = TokenNumber;

				this->m_vtEventItemList.push_back(EIIL);
			}
		}
	}

	fclose(SMDFile);
	return TRUE;
}

int CEventItemList::SortItem(BYTE giftsection, BYTE * level, BYTE * dur) //004A5B30 
{
	std::vector<EVENT_ID_ITEM_LIST>::iterator it;
	
	giftsection = rand() % this->m_vtEventItemList.size();

	for(it = this->m_vtEventItemList.begin(); it != this->m_vtEventItemList.end(); ++it)
	{
		if( (*it).m_btSection == giftsection)
		{
			BYTE btIndex = (*it).m_btIndex;
			BYTE btType = (*it).m_btType;
			
			*level = (*it).m_btLevel;
			*dur = (*it).m_btDur;
			
			return ItemGetNumberMake(btType, btIndex);
		}
	}
	return -1;
}
