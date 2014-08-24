// ChaosCard.h: interface for the CChaosCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAOSCARD_H__47216F93_45BD_4DB1_B59F_E533BABB23D7__INCLUDED_)
#define AFX_CHAOSCARD_H__47216F93_45BD_4DB1_B59F_E533BABB23D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CashLotterySystem.h"

#define MAX_CHAOS_CARD	5

typedef struct
{
	bool HaveInfo;
	char szChaosCardName[20];
	CCashLotterySystem * ChaosCardInfo;
} CHAOS_CARD_LIST, *LPCHAOS_CARD_LIST;

class CChaosCard  
{
public:
	CChaosCard();
	virtual ~CChaosCard();
	
	void Initialize();
	bool Load(char * szChaosCardName, LPSTR lpszFileName);
	CCashLotterySystem * GetChaosCardData(char * szChaosCardName);
	int CheckChaosCardInfo(char * szChaosCardName, CItem * lpItem);

private:
	CHAOS_CARD_LIST m_ChaosCardList[MAX_CHAOS_CARD];
};

extern CChaosCard g_ChaosCard;

#endif // !defined(AFX_CHAOSCARD_H__47216F93_45BD_4DB1_B59F_E533BABB23D7__INCLUDED_)
