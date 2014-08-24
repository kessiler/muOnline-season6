#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct EVENT_ID_TIME
{
	int m_eEventKind;	// 0
	int m_iHour;	// 4
	int m_iMinute;	// 8
	bool m_bEventStarted;	// C
};

class CEventManagement
{
public:

	CEventManagement();
	virtual ~CEventManagement();

	void Clear();
	void Init(BOOL bEventStart);
	bool Load(LPSTR lpszFileName);
	void Run();
	void StartEvent(int eEventKind);
	void RegEvent(int eEventKind, void* lpEventObj);

private:

	BOOL m_bEventStart;	// 4
	BOOL m_bHasData;	// 8
	std::vector<EVENT_ID_TIME> m_vtEventTime;	// C
	std::map<int,void *> m_mapEventObj;	// 1C
	WORD m_wToday_Year;	// 2C
	WORD m_wToday_Month;	// 2E
	WORD m_wToday_Day;	// 30

	enum {
	  EVENT_ID_DRAGONHERD = 0x0,
	  EVENT_ID_UNDERTROOP = 0x1,
	  EVENT_ID_ELDORADO = 0x2,
	  EVENT_ID_WHITEMAGE = 0x3,
	  EVENT_ID_XMASATTACK = 0x4,
	};
};

extern CEventManagement g_EventManager;

struct EVENT_ID_ITEM_LIST
{
	BYTE m_btSection;	// 0
	BYTE m_btType;	// 1
	BYTE m_btIndex;	// 2
	BYTE m_btLevel;	// 3
	BYTE m_btDur;	// 4
};

class CEventItemList  
{
public:
	CEventItemList();
	~CEventItemList();
	BOOL Load(LPSTR lpszFileName);
	int	SortItem(BYTE giftsection, BYTE * level, BYTE * dur);
	std::vector<EVENT_ID_ITEM_LIST> m_vtEventItemList;
};

extern	CEventItemList	g_EventItemList;
#endif