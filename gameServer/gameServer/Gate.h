#ifndef GATE_H
#define GATE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_GATES 512

class CGate
{
public:
	CGate();
	virtual ~CGate();

	void Init();
	void Load(LPSTR Buffer, int iSize);
	void Load(char *filename="..\\Data\\MoveGate.txt");
	int IsGate(int GateNumber);
	int GetGate(int mgt, short& x, short& y, BYTE& MapNumber, BYTE& dir, short& Level);
	int GetLevel(int GateNumber);
	int IsInGate(int aIndex, int GateNumber);
	int CheckGateLevel(int aIndex, int GateNumber);
private:
	BYTE m_This[MAX_GATES];	//
	BYTE m_Flag[MAX_GATES];	//
	BYTE m_MapNumber[MAX_GATES];	//
	BYTE m_Sx[MAX_GATES];	//
	BYTE m_Sy[MAX_GATES];	//
	BYTE m_Ex[MAX_GATES];	//
	BYTE m_Ey[MAX_GATES];	//
	WORD m_TargetGate[MAX_GATES];	//
	BYTE m_Dir[MAX_GATES];	//
	short m_Level[MAX_GATES];	//
};

extern CGate gGateC;

#endif