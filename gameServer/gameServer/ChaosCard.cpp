// ChaosCard.cpp: implementation of the CChaosCard class.
//	GS-N	1.00.77	JPN	0xXXXXXXXX - Completed
//	GS-CS	1.00.90	JPN	0xXXXXXXXX - Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosCard.h"
#include "CashLotterySystem.h"

CChaosCard g_ChaosCard;

CChaosCard::CChaosCard()
{

}

CChaosCard::~CChaosCard()
{

}

void CChaosCard::Initialize()
{
	for(int i = 0; i < MAX_CHAOS_CARD; i++)
	{
		this->m_ChaosCardList[i].HaveInfo = 0; //0x04
		this->m_ChaosCardList[i].szChaosCardName[0] = NULL; //0x05
		this->m_ChaosCardList[i].ChaosCardInfo = 0; //0x1C
	}
}

bool CChaosCard::Load(char * szChaosCardName, LPSTR lpszFileName)
{
	CCashLotterySystem * ChaosCard = 0; //ebp-14

	for(int i = 0; i < MAX_CHAOS_CARD; i++)
	{
		if(this->m_ChaosCardList[i].HaveInfo == false)
		{
			this->m_ChaosCardList[i].HaveInfo = true;
			strcpy(this->m_ChaosCardList[i].szChaosCardName, szChaosCardName);

			ChaosCard = new CCashLotterySystem; //good

			if(ChaosCard == NULL)
			{
				return 0;
			}

			ChaosCard->Load(lpszFileName);

			this->m_ChaosCardList[i].ChaosCardInfo = ChaosCard; //Chaos Card List Insert

			return 1;
		}
	}

	return 0;
}

CCashLotterySystem * CChaosCard::GetChaosCardData(char * szChaosCardName)
{
	for(int i = 0; i < MAX_CHAOS_CARD; i++)
	{
		if(this->m_ChaosCardList[i].HaveInfo == false)
		{
			continue;
		}

		if( strcmp(this->m_ChaosCardList[i].szChaosCardName, szChaosCardName) == 0) //if success
		{
			return this->m_ChaosCardList[i].ChaosCardInfo;
		}
	}

	return NULL;
}

int CChaosCard::CheckChaosCardInfo(char * szChaosCardName, CItem * lpItem) //Check Item pointer but i think it's ok
{
	CCashLotterySystem * ChaosCard = 0;
	
	ChaosCard = this->GetChaosCardData(szChaosCardName); //find and get chaos card info by name

	if(ChaosCard == NULL) //doesn't exist, empty!
	{
		return 0;
	}

	ChaosCard->GetItem(lpItem);

	if(lpItem == NULL) //wtf?
	{
		return 0;
	}

	return 1;
}