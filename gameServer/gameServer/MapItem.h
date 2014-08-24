#ifndef MAPITEM_H
#define MAPITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\common\zzzitem.h"
#include "user.h"


class CMapItem : public CItem
{

public:

	CMapItem();
	virtual ~CMapItem();

	void Init();
	void CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, BYTE ItemEffectEx,BYTE SocketOption[5], BYTE SocketIndex);
	void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx,BYTE SocketOption[5], BYTE SocketIndex);

public:
	
	BYTE px;	// BD
	BYTE py;	// BE
	bool live;	// BF
	bool Give;	// C4
	DWORD m_State;	// B8
	DWORD m_Time;	// BC
	int m_UserIndex;	// C0
	DWORD m_LootTime;	// C4
	VIEWPORT_PLAYER_STRUCT VpPlayer[MAX_VIEWPORT];	// C8
	int VPCount;	// 44C
};


#endif