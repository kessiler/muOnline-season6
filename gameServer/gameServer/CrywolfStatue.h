// CrywolfStatue.h: interface for the CCrywolfStatue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFSTATUE_H__74030238_F088_4B74_A543_22B27C44C469__INCLUDED_)
#define AFX_CRYWOLFSTATUE_H__74030238_F088_4B74_A543_22B27C44C469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#define CRYWOLF_STATUE_CHECK(iClass) ( ( ((iClass)) == 204 )?TRUE:FALSE)

class CCrywolfShield
{
public:
	CCrywolfShield()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_iPriestNumber = 0;
		this->m_iShieldHP = 0;	
		this->m_iShieldMaxHP = 0;	
		this->m_iShieldState = 0;	
	}
		
	int GetHPPercentage()
	{
		if ( this->m_iShieldMaxHP )
		{
			return (this->m_iShieldHP * 100) / this->m_iShieldMaxHP;
		}

		return 0;
	}

public:
	int m_iShieldMaxHP;	// 0
	int m_iShieldHP;	// 4
	int m_iShieldState;	// 8
	int m_iPriestNumber;	// C
};

class CCrywolfStatue 
{
	
public:
	CCrywolfStatue();
	virtual ~CCrywolfStatue();

	void CrywolfStatueAct(int iIndex);
	int GetStatueViewState(int iPriestNumber);
	void SetStatueViewState(LPOBJ lpObj, int iAltarNumber);

public:

	CCrywolfShield m_Shield;	// 4

};

extern CCrywolfStatue g_CrywolfNPC_Statue;

#endif // !defined(AFX_CRYWOLFSTATUE_H__74030238_F088_4B74_A543_22B27C44C469__INCLUDED_)
