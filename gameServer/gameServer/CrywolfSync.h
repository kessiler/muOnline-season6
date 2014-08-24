// CrywolfSync.h: interface for the CCrywolfSync class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFSYNC_H__CC49C33A_FF1E_45A7_9904_8CAD6B57D46C__INCLUDED_)
#define AFX_CRYWOLFSYNC_H__CC49C33A_FF1E_45A7_9904_8CAD6B57D46C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrywolfSync
{

public:

	CCrywolfSync();
	virtual ~CCrywolfSync();

	BOOL CheckEnableCrywolf();
	void SetEnableCrywolf(BOOL bEnable);
	int GetOccupationState();
	int GetCrywolfState();
	void SetOccupationState(int iOccupationState);
	void SetCrywolfState(int iCrywolfState);
	int GetPlusChaosRate();
	void SetPlusChaosRate(int iPlusChaosRate);
	int GetGemDropPenaltiyRate();
	void SetGemDropPenaltiyRate(int iGemDropPenaltyRate);
	int GetGettingExpPenaltyRate();
	void SetGettingExpPenaltyRate(int iGettingExpPenaltyRate);
	int GetMonHPBenefitRate();
	void SetMonHPBenefitRate(int iMinusMonHPBenefitRate);
	int GetKundunHPRefillState();
	void SetKundunHPRefillState(int iKundunHPRefillState);

private:

	BOOL m_bCrywolfEnable;	// 4
	BOOL m_bApplyBenefit;	// 8
	BOOL m_bApplyPenalty;	// C
	int m_iCrywolfState;	// 10
	int m_iOccupationState;	// 14
	int m_iPlusChaosRate;	// 18
	int m_iGemDropPenaltyRate;	// 1C
	int m_iGettingExpPenaltyRate;	// 20
	int m_iMinusMonHPBenefitRate;	// 24
	int m_iKundunHPRefillState;	// 28


};


extern CCrywolfSync g_CrywolfSync;

#endif // !defined(AFX_CRYWOLFSYNC_H__CC49C33A_FF1E_45A7_9904_8CAD6B57D46C__INCLUDED_)
