// TMonsterAIAutomata.h: interface for the TMonsterAIAutomata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIAUTOMATA_H__FAE2FB5C_F6E2_4A4B_ACBB_10A04FD1C3FA__INCLUDED_)
#define AFX_TMONSTERAIAUTOMATA_H__FAE2FB5C_F6E2_4A4B_ACBB_10A04FD1C3FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TMonsterAIState.h"

#define MAX_MONSTER_AI_AUTOMATA	300
#define MAX_AI_STATE			8
#define MAX_AI_PRIORITY			30

#define MAX_AI_STATE_RANGE(x) ( ( (x)< 0)?FALSE:((x) > MAX_AI_STATE-1 )? FALSE : TRUE  )

class TMonsterAIAutomata  
{

public:

	TMonsterAIAutomata();
	virtual ~TMonsterAIAutomata();

	TMonsterAIState* RunAutomata(int iIndex);
	void Reset();

	static BOOL LoadData(char* lpszFileName);
	static BOOL DelAllAutomata();
	static TMonsterAIAutomata* FindAutomata(int iAutomataNumber);
	static BOOL s_bDataLoad;
	static TMonsterAIAutomata s_MonsterAIAutomataArray[MAX_MONSTER_AI_AUTOMATA];

private:

	int m_iAutomataNumber;	// 4
	TMonsterAIState m_AIState[MAX_AI_STATE][MAX_AI_PRIORITY];	// 8
	int m_AIStateTransCount[MAX_AI_STATE];	// 7BC8


};

#endif // !defined(AFX_TMONSTERAIAUTOMATA_H__FAE2FB5C_F6E2_4A4B_ACBB_10A04FD1C3FA__INCLUDED_)
