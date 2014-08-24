#include "StdAfx.h"
#include "PacketStatistics.h"
// -------------------------------------------------------------------------------

Timer::Timer(bool bOnceWake = 1)
{
	this->m_iWakeTick	= 0;
	this->m_bOnceWake	= bOnceWake;
	this->m_bActive		= 0;
}
// -------------------------------------------------------------------------------

void Timer::Init()
{
	this->m_bActive		= 0;
	this->m_iWakeTick	= 0;
}
// -------------------------------------------------------------------------------

Timer::~Timer()
{
	//Unfilled
}
// -------------------------------------------------------------------------------

bool Timer::IsWake()
{
	if( this->m_iWakeTick > GetTickCount() )
	{
		return false;
	}
	// ----
	if( this->m_bOnceWake == 1 )
	{
		this->m_bActive = 0;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

void Timer::SetTimer(DWORD dwMilliSecond)
{
	this->m_bActive		= 1;
	this->m_iWakeTick	= dwMilliSecond + GetTickCount();
}
// -------------------------------------------------------------------------------

PacketStatistics::PacketStatistics()
{
	//this->m_kTimer(1);
	this->m_iCheckDuration		= 0;
	this->m_iPacketCount		= 0;
	this->m_iEncryptPacketCount = 0;
	this->m_iPacketTotalSize	= 0;
}
// -------------------------------------------------------------------------------

void PacketStatistics::Init(DWORD iCheckDuration)
{
	this->m_iCheckDuration		= iCheckDuration;
	this->m_iPacketCount		= 0;
	this->m_iEncryptPacketCount = 0;
	this->m_iPacketTotalSize	= 0;
	this->m_kTimer.Init();
	this->m_kTimer.SetTimer(this->m_iCheckDuration);
}
// -------------------------------------------------------------------------------

PacketStatistics::~PacketStatistics()
{
	//Unfilled
}
// -------------------------------------------------------------------------------

PacketStatistics::PROCESS_RETURN PacketStatistics::Process()
{
	if( this->m_kTimer.IsWake() == 1 )
	{
		this->Init(this->m_iCheckDuration);
		return PS_RESET;
	}
	// ----
	return PS_NORMAL;
}
// -------------------------------------------------------------------------------

void PacketStatistics::AddPacketInfo(bool bEncrypt, DWORD iPackSize)
{
	this->m_iPacketCount++;
	this->m_iPacketTotalSize += iPackSize;
	// ----
	if( bEncrypt )
	{
		this->m_iEncryptPacketCount++;
	}
}
// -------------------------------------------------------------------------------

void PacketStatistics::GetStatistics(int & iOutPacketCount, int & iOutEncryptPacketCount, DWORD & iOutPacketTotalSize)
{
	iOutPacketCount			= this->m_iPacketCount;
	iOutEncryptPacketCount	= this->m_iEncryptPacketCount;
	iOutPacketTotalSize		= this->m_iPacketTotalSize;
}
// -------------------------------------------------------------------------------