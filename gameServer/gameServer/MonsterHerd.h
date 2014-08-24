// ------------------------------
// Decompiled by Deathway
// Date : 2007-07-09
// ------------------------------
#ifndef MONSTERHERD_H
#define MONSTERHERD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

struct _MONSTER_HERD_DATA 
{
	int m_iIndex;	// 0
	int m_iType;	// 4
	BYTE m_iX;	// 8
	BYTE m_iY;	// 9
	BOOL m_bRegen;	// C

	_MONSTER_HERD_DATA()
	{
		this->m_iIndex = -1;
		this->m_iType = -1;
		this->m_iX = -1;
		this->m_iY = -1;
		this->m_bRegen = FALSE;
	}
};



class MonsterHerd
{


public:	

	MonsterHerd();
	virtual ~MonsterHerd();

  	virtual BOOL SetTotalInfo(int iMapNumber, int iRadius, int iStartX, int iStartY);	// 4
	virtual BOOL AddMonster(int iMonsterType, BOOL bRegen, BOOL bAttackFirst);	// 8
	virtual void SetRadius(int iRadius);	// C
	virtual void SetPosition(BYTE iTX, BYTE iTY);	// 10
	virtual BOOL Start();	// 14
	virtual void Stop();	// 18
	virtual BOOL CheckInRadius(int iIndex);	// 1C
	virtual BOOL GetCurrentLocation(BYTE&cX, BYTE& cY);	// 20
	virtual BOOL GetRandomLocation(BYTE& cX, BYTE& cY);	// 24
	virtual BOOL CheckLocation(BYTE& cX, BYTE& cY);	// 28
	virtual int MoveHerd(BYTE iTX, BYTE iTY);	// 2C
	virtual int MonsterHerdItemDrop(LPOBJ lpObj);	// 30
	virtual _MONSTER_HERD_DATA* GetMonsterData(int iIndex);	// 34
	virtual void BeenAttacked(LPOBJ lpObj, LPOBJ lpTargetObj);	// 38
	virtual void OrderAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackPercent);	// 3C
	virtual void MonsterBaseAct(LPOBJ lpObj);	// 40
	virtual void MonsterMoveAction(LPOBJ lpObj);	// 44
	virtual void MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj);	// 48
	virtual void MonsterDieAction(LPOBJ lpObj);	// 4C
	virtual int MonsterRegenAction(LPOBJ lpObj);	// 50


public:

	int m_iHerdIndex;	// 4
	int m_iMapNumber;	// 8
	int m_iCUR_X;	// C
	int m_iCUR_Y;	// 10
	int m_iRADIUS;	// 14
	BOOL m_bHasInfo;	// 18
	BOOL m_bHerdActive;	// 1C
	std::map<int,_MONSTER_HERD_DATA> m_mapMonsterHerd;	// 20
	CRITICAL_SECTION m_critMonsterHerd;	// 30

};

#endif

