// TMonsterAIGroupMember.h: interface for the TMonsterAIGroupMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIGROUPMEMBER_H__968E48F0_A3DB_4E2E_9339_8FF4C1C704EE__INCLUDED_)
#define AFX_TMONSTERAIGROUPMEMBER_H__968E48F0_A3DB_4E2E_9339_8FF4C1C704EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TMonsterAIGroupMember  
{

public:

	TMonsterAIGroupMember();
	virtual ~TMonsterAIGroupMember();

	void Reset();

public:

	int m_iGroupNumber;	// 4
	int m_iGuid;	// 8
	int m_iClass;	// C
	int m_iRank;	// 10
	int m_iStartAI;	// 14
	int m_iAI01;	// 18
	int m_iAI02;	// 1C
	int m_iAI03;	// 20
	int m_iCreateType;	// 24
	int m_iMapNumber;	// 28
	int m_iStartX;	// 2C
	int m_iStartY;	// 30
	int m_iStartDir;	// 34
	int m_iRegenType;	// 38
	int m_iObjIndex;	// 3C

};

#endif // !defined(AFX_TMONSTERAIGROUPMEMBER_H__968E48F0_A3DB_4E2E_9339_8FF4C1C704EE__INCLUDED_)
