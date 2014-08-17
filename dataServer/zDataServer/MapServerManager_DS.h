// MapServerManager_DS.h: interface for the CMapServerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPSERVERMANAGER_DS_H__4C91F263_7B45_45F1_BF11_70DCE1A3BFD1__INCLUDED_)
#define AFX_MAPSERVERMANAGER_DS_H__4C91F263_7B45_45F1_BF11_70DCE1A3BFD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
using namespace std;

struct _MAPSVR_DATA
{
public:
	_MAPSVR_DATA();
	virtual ~_MAPSVR_DATA();
	void Clear(int iInitSetVal);

public:
	BOOL m_bIN_USE;
	BYTE m_btMAPSVR_GROUP;
	short m_sSVR_CODE;
	char m_cIPADDR[16];
	WORD m_wPORT;
	short m_sMAP_MOVE[MAX_MAP];	
};

class CMapServerManager  
{
public:
	CMapServerManager();
	virtual ~CMapServerManager();

	BOOL LoadMapData(char* lpszFileName);
	BOOL GetSvrCodeData(WORD wServerCode, char* lpszIpAddress, WORD* lpwPort);
	BOOL GetMapSvrGroup(WORD wServerCode);

public:
	BOOL	m_bMapDataLoadOk;
	_MAPSVR_DATA m_MAPSVR_DATA[20][20];
	int m_iMAPSVR_GROUP_COUNT[20];
	std::map<int,_MAPSVR_DATA *> m_mapSVRCODE_MAP;
	CRITICAL_SECTION m_critSVRCODE_MAP;
	_MAPSVR_DATA* m_lpThisMapSvrData;
};

extern CMapServerManager	g_MapServerManager;

#endif // !defined(AFX_MAPSERVERMANAGER_DS_H__4C91F263_7B45_45F1_BF11_70DCE1A3BFD1__INCLUDED_)
