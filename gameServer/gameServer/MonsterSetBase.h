#ifndef MONSTERSETBASE_H
#define MONSTERSETBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"


typedef struct MONSTER_POSITION
{
	BYTE m_ArrangeType;	// 0
	WORD m_Type;	// 2
	BYTE m_MapNumber;	// 4
	BYTE m_Dis;	// 5
	BYTE m_X;	// 6
	BYTE m_Y;	// 7
	BYTE m_Dir;	// 8
	BYTE m_W;	// 9
	BYTE m_H;	// A
	WORD m_IG_ZoneIndex;	//1.01.00
	WORD m_IG_RegenTable;

} MONSTER_POSITION, * LPMONSTER_POSITION;
class CMonsterSetBase
{

public:

	void LoadSetBase(char* Buffer, int iSize);
	void LoadSetBase(char* filename);

	CMonsterSetBase();
	virtual ~CMonsterSetBase();

	int GetPosition(int TableNum, short MapNumber, short& x, short& y);
	int GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short& mx, short& my);
	void SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah);

public:

  MONSTER_POSITION m_Mp[OBJ_MAXMONSTER];	// 4
  int m_Count;	// 10FE4


};

#endif