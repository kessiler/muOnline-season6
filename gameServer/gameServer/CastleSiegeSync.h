#ifndef CASTLESIEGESYNC_H
#define CASTLESIEGESYNC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_TRIBUTE_MONEY MAX_ZEN

class CCastleSiegeSync
{

public:

	CCastleSiegeSync();
	virtual ~CCastleSiegeSync();

	void Clear();
	void SetCastleOwnerGuild(char * lpszGuildName);
	int GetTaxRateChaos(int iIndex);
	int GetTaxRateStore(int iIndex);
	int GetTaxHuntZone(int iIndex, BOOL bCheckOwnerGuild);
	void AddTributeMoney(int iMoney);
	void ResetTributeMoney();
	void AdjustTributeMoney();
	BOOL CheckCastleOwnerMember(int iIndex);
	BOOL CheckCastleOwnerUnionMember(int iIndex);
	int CheckOverlapCsMarks(int iIndex);

	void SetCastleState(int iCastleState){this->m_iCurCastleState = iCastleState;};	// line : 56
	void SetTaxRateChaos(int iTaxRate){this->m_iCurTaxRateChaos = iTaxRate;};	// line : 59
	void SetTaxRateStore(int iTaxRate){this->m_iCurTaxRateStore = iTaxRate;};	// line : 62
	void SetTaxHuntZone(int iTaxRate){this->m_iCurTaxHuntZone = iTaxRate;};	// line : 65
	int  GetCastleState(){return this->m_iCurCastleState;};	// line : 70
	int GetTributeMoney(){return this->m_lCastleTributeMoney;};	// line : 77
	LPSTR GetCastleOwnerGuild(){return this->m_szCastleOwnerGuild;};	// line : 85

	

private:

	int m_iCurCastleState;	// 4
	int m_iCurTaxRateChaos;	// 8
	int m_iCurTaxRateStore;	// C
	int m_iCurTaxHuntZone;	// 10
	long m_lCastleTributeMoney;	// 14
	char m_szCastleOwnerGuild[16];	// 18
	int m_iCsTributeMoneyTimer;	// 28
	
};

extern CCastleSiegeSync g_CastleSiegeSync;

#endif