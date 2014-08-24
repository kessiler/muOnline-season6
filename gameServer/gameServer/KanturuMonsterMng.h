// KanturuMonsterMng.h: interface for the CKanturuMonsterMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUMONSTERMNG_H__250ABD41_01B7_47A1_AB68_094FED15A3BC__INCLUDED_)
#define AFX_KANTURUMONSTERMNG_H__250ABD41_01B7_47A1_AB68_094FED15A3BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "KanturuObjInfo.h"


#define MAX_KANTURU_MONSTER	200


struct KANTURU_MONSTER_SETBASE_INFO
{
	BYTE btGroup;	// 0
	WORD wType;	// 2
	BYTE btMapNumber;	// 4
	BYTE btDistance;	// 5
	BYTE btX;	// 6
	BYTE btY;	// 7
	BYTE btDir;	// 8
};


class CKanturuMonsterMng  
{

public:

	CKanturuMonsterMng();
	virtual ~CKanturuMonsterMng();

	void ResetLoadData();
	void ResetRegenMonsterObjData();
	void MonsterDie(int iIndex);
	BOOL LoadData(LPSTR lpszFileName);
	void SetMonsterSetBaseInfo(int iIndex, BYTE btGroup, WORD wType, BYTE btMapNumber, BYTE btDistance, BYTE btX, BYTE btY, BYTE btDir);
	//void __thiscall SetMaxMonsterCount(int);
	//int __thiscall GetMaxMonsterCount();
	int SetKanturuMonster(int iGroupNumber);
	int GetPosition(int TableNum, short MapNumber, short& x, short& y);
	int GetAliveMonsterCount(){return this->m_iAliveMonsterCount;}
	//int __thiscall GetMaxRegenMonsterCount();
	BOOL IsExistAliveMonster();
	int GetMayaObjIndex(){return this->m_iMayaObjIndex;}

private:

	int m_iMaxMonsterCount;	// 4
	KANTURU_MONSTER_SETBASE_INFO m_SetBaseInfo[MAX_KANTURU_MONSTER];	// 8
	int m_iMayaObjIndex;	// 7D8
	int m_bFileDataLoad;	// 7DC
	CKanturuObjInfo m_KanturuMonster;	// 7E0
	int m_iMaxRegenMonsterCount;	// 1468
	int m_iAliveMonsterCount;	// 146C

};


extern CKanturuMonsterMng g_KanturuMonsterMng;

#endif // !defined(AFX_KANTURUMONSTERMNG_H__250ABD41_01B7_47A1_AB68_094FED15A3BC__INCLUDED_)
