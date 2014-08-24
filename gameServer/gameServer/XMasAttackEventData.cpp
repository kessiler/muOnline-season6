// XMasAttackEventData.cpp: implementation of the CXMasAttackEventData class.
//GameServer 1.00.90	-	Completed (Not all function order are correct but code is 100% identical)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMasAttackEventData.h"
#include "XMasAttackEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventTimer::CEventTimer() //identical
{
	this->m_iTIME_MSEC_REMAIN = GetTickCount();
}

CEventTimer::~CEventTimer() //identical
{

}

void CEventTimer::SetSeconds(int Time) //identical
{
	this->m_iTIME_MSEC_REMAIN = GetTickCount() + (1000 * Time);
}

int CEventTimer::GetSeconds() //identical
{
	int Time = GetTickCount(); //loc2

	int loc3 = this->m_iTIME_MSEC_REMAIN - Time;

	if( loc3 < 0 )
	{
		this->m_iTIME_MSEC_REMAIN = Time;
		return 0;
	}

	return ((loc3+999)/1000);
}

void CEventTimer::SetMinutes(int Time) //identical
{
	this->m_iTIME_MSEC_REMAIN = GetTickCount() + (60000 * Time);
}

int CEventTimer::GetMinutes() //identical
{
	int Time = GetTickCount(); //loc2

	int loc3 = this->m_iTIME_MSEC_REMAIN - Time;

	if( loc3 < 0 )
	{
		this->m_iTIME_MSEC_REMAIN = Time;
		return 0;
	}

	return ((loc3+59999)/60000);
}

void CEventTimer::SetMiliSeconds(int Time) //identical
{
	this->m_iTIME_MSEC_REMAIN = GetTickCount() + Time;
}

int CEventTimer::GetMiliSeconds() //identical
{
	int Time = GetTickCount(); //loc2

	int loc3 = this->m_iTIME_MSEC_REMAIN - Time;

	if( loc3 < 0 )
	{
		this->m_iTIME_MSEC_REMAIN = Time;
		return 0;
	}

	return loc3;
}

CXMasAttackEventData::CXMasAttackEventData() : m_bDoEvent(0), m_bHasData(0), m_bMenualStart(0)
{
	InitializeCriticalSection(&this->m_critMonsterAddData);
}

//identical
void CXMasAttackEvent::SetMenualStart(BOOL bMenualStart)
{
	this->m_bMenualStart = bMenualStart;
}

CXMasAttackEventData::~CXMasAttackEventData()
{
	DeleteCriticalSection(&this->m_critMonsterAddData);
}

//identical
void CXMasAttackEventData::Clear() //0049D570
{
	EnterCriticalSection(&this->m_critMonsterAddData);
	this->m_vtMonsterAddData.erase( this->m_vtMonsterAddData.begin(), this->m_vtMonsterAddData.end() );
	this->m_vtEventTime.erase( this->m_vtEventTime.begin(), this->m_vtEventTime.end() );
	LeaveCriticalSection(&this->m_critMonsterAddData);

	this->m_bHasData = FALSE;
}

//identical
BOOL CXMasAttackEventData::Init(int arg1) //0049D610
{
	return FALSE;
}