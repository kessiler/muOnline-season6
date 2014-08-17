// DelayHandler.h: interface for the DelayHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYHANDLER_H__1667AE73_CE35_4AB1_B115_DEE906B5ACEE__INCLUDED_)
#define AFX_DELAYHANDLER_H__1667AE73_CE35_4AB1_B115_DEE906B5ACEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
using namespace std;

class DelayHandler  
{
public:
	DelayHandler();
	virtual ~DelayHandler();

	void Initialize();

	int GetQuerySessionId()
	{
		return m_lQuerySessionId;
	}

	void IncreaseQuerySessionId()
	{
		InterlockedIncrement(&m_lQuerySessionId);
	}

	void AddHeadCode(BYTE btHeadCode);
	bool CheckHeadCode(BYTE btHeadCode);

public:
	long m_lQuerySessionId;
	std::map<int, int> m_mapHeadCode;
	CRITICAL_SECTION m_critHeadCode;
};

extern DelayHandler	g_DelayHandler;

#endif // !defined(AFX_DELAYHANDLER_H__1667AE73_CE35_4AB1_B115_DEE906B5ACEE__INCLUDED_)
