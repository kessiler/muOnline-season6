// DelayHandler.cpp: implementation of the DelayHandler class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelayHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DelayHandler	g_DelayHandler;

DelayHandler::DelayHandler()
{
	InitializeCriticalSection(&m_critHeadCode);
	m_lQuerySessionId = 0;
	Initialize();
}

DelayHandler::~DelayHandler()
{
	DeleteCriticalSection(&m_critHeadCode);
}

void DelayHandler::Initialize()
{
	AddHeadCode(0x00);
	AddHeadCode(0x02);
	AddHeadCode(0x04);
	AddHeadCode(0x05);
	AddHeadCode(0x07);
	AddHeadCode(0x09);
	AddHeadCode(0x11);
	AddHeadCode(0x12);
	AddHeadCode(0x20);
	AddHeadCode(0x52);
	AddHeadCode(0x54);
	AddHeadCode(0x55);
	AddHeadCode(0x57);
	AddHeadCode(0x60);
	AddHeadCode(0x80);
	AddHeadCode(0x81);
	AddHeadCode(0x82);
	AddHeadCode(0x83);
	AddHeadCode(0x84);
	AddHeadCode(0x85);
	AddHeadCode(0x86);
	AddHeadCode(0x87);
	AddHeadCode(0x88);
	AddHeadCode(0x89);
	AddHeadCode(0xCF);
}

void DelayHandler::AddHeadCode(BYTE btHeadCode)
{
	EnterCriticalSection(&m_critHeadCode);

	m_mapHeadCode.insert( std::pair<int, int>(btHeadCode, 0) );

	LeaveCriticalSection(&m_critHeadCode);
}

bool DelayHandler::CheckHeadCode(BYTE btHeadCode)
{
	bool bRetVal = false;

	EnterCriticalSection(&m_critHeadCode);

	std::map<int, int>::iterator it = m_mapHeadCode.find(btHeadCode);

	if ( it != m_mapHeadCode.end() )
	{
		bRetVal = true;
	}

	LeaveCriticalSection(&m_critHeadCode);

	return bRetVal;
}