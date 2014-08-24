#ifndef MAPSERVERMANAGER_H
#define MAPSERVERMANAGER_H


#include "MapClass.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MAP_GROUPS 20
#define MAX_MAP_SUBGROUPS 20
#define MAX_SVR_PER_GROUP 400


struct _MAPSVR_DATA
{

public:

	_MAPSVR_DATA();
	virtual ~_MAPSVR_DATA();

	void Clear(int iInitSetVal);

public:

	BOOL m_bIN_USE;	// 4
	BYTE m_btMAPSVR_GROUP;	// 8
	short m_sSVR_CODE;	// A
	char m_cIPADDR[16];	// C
	WORD m_wPORT;	// 1C
	short m_sMAP_MOVE[MAX_NUMBER_MAP];	// 1E

};




class CMapServerManager
{

public:

	CMapServerManager();
	virtual ~CMapServerManager();

	void Clear();
	BOOL LoadData(char* lpszFileName);
	BOOL CheckMapCanMove(int iMAP_NUM);
	short CheckMoveMapSvr(int iIndex, int iMAP_NUM, short sSVR_CODE_BEFORE);
	BOOL GetSvrCodeData(WORD wServerCode, char* lpszIpAddress, unsigned short* lpwPort);
	int GetMapSvrGroup()
	{
		if ( this->m_lpThisMapSvrData != NULL )
		{
			return m_lpThisMapSvrData->m_btMAPSVR_GROUP;
		}
		return -1;
	}

private:

	BOOL m_bMapDataLoadOk;	// 4
	_MAPSVR_DATA m_MAPSVR_DATA[MAX_MAP_GROUPS][MAX_MAP_SUBGROUPS];	// 8
	int m_iMAPSVR_GROUP_COUNT[MAX_MAP_GROUPS];	// AF08
	std::map<int,_MAPSVR_DATA *> m_mapSVRCODE_MAP;	// AF58
	CRITICAL_SECTION m_critSVRCODE_MAP;	// AF68
	_MAPSVR_DATA* m_lpThisMapSvrData;	// AF80


};

extern CMapServerManager g_MapServerManager;

#endif

