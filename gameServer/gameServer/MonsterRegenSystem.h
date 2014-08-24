#pragma once

#define MAX_MONSTER_REGEN		255
#define MAX_MONSTER_INDEXINFO	20
#define MONSTER_REGEN_RANGE(nGroupIndex) ( ((nGroupIndex) < 0 )?FALSE:( (nGroupIndex) > MAX_MONSTER_REGEN-1 )?FALSE:TRUE   )

struct _stRegenManagedInfo
{
	_stRegenManagedInfo()
	{
		iUnk1 = 0;
		iUnk2 = 0;
		iUnk3 = -1;
	}

	int iUnk1;	// 0
	int iUnk2;	// 4
	int iUnk3;	// 8
};

struct _stRegenArea
{
	_stRegenArea()
	{
		sx = -1;
		sy = -1;
		ex = -1;
		ey = -1;
	}
	int sx;
	int sy;
	int ex;
	int ey;
};

struct _stMonsterIndexInf
{
	_stMonsterIndexInf()
	{
		iIndex = -1;
		iCount = 0;
	}
	int iIndex;
	int iCount;
};

struct _stMonsterPosition
{
	_stMonsterPosition()
	{
		iGroupNumber = -1;
		iAreaCount = 0;
	}

	int iGroupNumber;
	int iAreaCount;

	_stRegenArea m_RegenArea[50];
	_stMonsterIndexInf m_MonsterIndexInf[MAX_MONSTER_INDEXINFO];
};

struct _stRegenTimeTable
{
	_stRegenTimeTable()
	{
		nBeginHour = -1;
		nEndHour = -1;
		nRegenHour = -1;
		nRegenMin = -1;
		bIsRegen = 0;
	}
	short nBeginHour;	// 0
	short nEndHour;	// 2
	short nRegenHour;	// 4
	short nRegenMin;	// 6
	BYTE bIsRegen;	// 8
};

struct _stMonsterAccountNumInfo
{
	_stMonsterAccountNumInfo()
	{
		index = -1;
	}
	int index;
};

struct _stMonsterGroupInfo
{
	_stMonsterGroupInfo()
	{
		iGroupNumber = -1;
		iMapNumber = -1;
		iBossMonsterIndex = -1;
		dwRegenTick = 0;
		aType = 0;
		iBossMonsterClass = -1;
	}

	int iGroupNumber;	// 0
	int iMapNumber;	// 4
	int iBossMonsterClass;	// 8
	int iBossMonsterIndex;	// C
	DWORD dwRegenTick;	// 10
	int tmpUnk;	// 14
	short aType;	// 18

	_stRegenTimeTable m_RegenTimeTable[24];	// 1A
	_stMonsterAccountNumInfo m_MonsterAccountNumInfo[MAX_MONSTER_REGEN];	// 10C
};

class CMonsterRegenSystem
{
public:
	CMonsterRegenSystem();
	virtual ~CMonsterRegenSystem();

	void LoadScript(const char* lpFileName);
	void Run();
	void RegenMonster(int nGroupNumber);
	bool IsLiveBossState(int nGroupNumber);
	void DeleteMonster(int nGroupNumber);
	void SendAllUserAnyMsg(int iType,char* lpszMsg,...);
	
	bool IsRegenTime(int nGroupNumber,int nCurHour,int nCurMin,bool bSetRegen);
	void SetRegenTimeTable();

private:
	int SetPosMonster(int aIndex,int nMapNumber,int nBeginX,int nBeginY,int nEndX,int nEndY);
	int GetBoxPosition(int mapnumber,int ax,int ay,int aw,int ah,short & mx,short & my);

public:
	BYTE					m_bLoaded;								// 4
	short					m_CurDay;								// 6
	_stRegenManagedInfo		m_RegenInfo[MAX_MONSTER_REGEN];			// 8
	_stMonsterPosition		m_MonsterPosition[MAX_MONSTER_REGEN];	// BFC
	_stMonsterGroupInfo		m_MonsterGroupInfo[MAX_MONSTER_REGEN];	// 3D034
	// ----
}; extern CMonsterRegenSystem g_MonsterRegenSystem;