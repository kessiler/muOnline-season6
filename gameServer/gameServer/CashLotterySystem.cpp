// CashItemPeriodSystem.cpp: implementation of the CCashLotterySystem class.
// GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CashLotterySystem.h"
#include "LargeRand.h"
#include "user.h"
#include "LogProc.h"
#include "..\include\readscript.h"



CCashLotterySystem g_CashLotterySystem;



CCashLotterySystem::CCashLotterySystem()
{
	return;
}

CCashLotterySystem::~CCashLotterySystem()
{
	return;
}


void CCashLotterySystem::Initialize()
{
	for ( int i = 0; i < MAX_LOTTERY_ITEM_CATEGORY; i++ )
	{
		this->LotteryItemListCount[i] = 0;
		this->LotteryItemOptionRate[i].iSkillOptionRate = 0;
		this->LotteryItemOptionRate[i].iLuckOptionRate = 0;
		this->LotteryItemOptionRate[i].iAddOptionRate = 0;
		this->LotteryItemOptionRate[i].iExOptionRate =0 ;
	}

	for ( int i = 0; i < MAX_LOTTERY_ITEM_CATEGORY; i++ )
	{
		for ( int j = 0;j<MAX_LOTTERY_ITEM_COUNT; j++ )
		{
			this->LotteryItemList[i][j].btItemType = -1;
			this->LotteryItemList[i][j].wItemIndex = -1;
			this->LotteryItemList[i][j].btItemLevelMin = -1;
			this->LotteryItemList[i][j].btItemLevelMax = -1;
			this->LotteryItemList[i][j].btSkillOption = -1;
			this->LotteryItemList[i][j].btLuckOption = -1;
			this->LotteryItemList[i][j].btAddOption = -1;
			this->LotteryItemList[i][j].btExOption = -1;
		}
	}

	this->CategoryRandomPool.InitPool();
	this->AddOptionRandomPool.InitPool();
	this->ExOptionRandomPool.InitPool();
	this->iItemMaxHeight = 0;
	this->iItemMaxWidth = 0;
}



void CCashLotterySystem::Load(LPSTR pchFilename)
{
	this->Initialize();

	SMDToken Token;
	SMDFile = fopen(pchFilename, "r");	//ok

	if ( SMDFile == NULL )
	{
		MsgBox("[CashLoterrySystem] Lottery Item List load failed. [%s]", pchFilename);
		return;
	}

	int iType = 0;
	int iCategory = 0;
	int iDropRate = 0;
	int iSkillOptionRate = 0;
	int iLuckOptionRate = 0;
	int iAddOptionRate = 0;
	int iExOptionRate = 0;
	int iItemAddOption = 0;
	int iItemAddOptionRate = 0;
	int iItemExOption = 0;
	int iItemExOptionRate = 0;
	int iItemType = 0;
	int iItemIndex = 0;
	int iItemLevelMin = 0;
	int iItemLevelMax = 0;
	int iSkillOption = 0;
	int iLuckOption = 0;
	int iAddOption = 0;
	int iExOption = 0;

	while ( true )
	{
		Token = (SMDToken)GetToken();

		if (Token == END )
		{
			break;
		}

		iType = TokenNumber;

		while ( true )
		{
			Token = (SMDToken)GetToken();

			if ( Token == END )
			{
				break;
			}

			if ( iType == 0 )
			{
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				iCategory = TokenNumber;

				Token = (SMDToken)GetToken();
				iDropRate = TokenNumber;

				Token = (SMDToken)GetToken();
				iSkillOptionRate = TokenNumber;

				Token = (SMDToken)GetToken();
				iLuckOptionRate = TokenNumber;

				Token = (SMDToken)GetToken();
				iAddOptionRate = TokenNumber;

				Token = (SMDToken)GetToken();
				iExOptionRate = TokenNumber;

				this->SetitemDropRate(iCategory-3, iDropRate, iSkillOptionRate, iLuckOptionRate, iAddOptionRate, iExOptionRate);
			}

			if ( iType == 1 )
			{
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				iItemAddOption = TokenNumber;

				Token = (SMDToken)GetToken();
				iItemAddOptionRate = TokenNumber;

				this->AddOptionRandomPool.AddValue(iItemAddOption, iItemAddOptionRate);
			}

			if ( iType == 2 )
			{
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				iItemExOption = TokenNumber;

				Token = (SMDToken)GetToken();
				iItemExOptionRate = TokenNumber;

				this->ExOptionRandomPool.AddValue(iItemExOption, iItemExOptionRate);
			}
			else if ( iType  >= 3 && iType < 23 )//new changed
			{
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				iItemType = TokenNumber;

				Token = (SMDToken)GetToken();
				iItemIndex = TokenNumber;

				Token = (SMDToken)GetToken();
				iItemLevelMin = TokenNumber;

				Token = (SMDToken)GetToken();
				iItemLevelMax = TokenNumber;

				Token = (SMDToken)GetToken();
				iSkillOption = TokenNumber;

				Token = (SMDToken)GetToken();
				iLuckOption = TokenNumber;

				Token = (SMDToken)GetToken();
				iAddOption = TokenNumber;

				Token = (SMDToken)GetToken();
				iExOption = TokenNumber;

				this->InsertItem(iType-3, iItemType, iItemIndex, iItemLevelMin, iItemLevelMax, iSkillOption, iLuckOption, iAddOption, iExOption);
			}
		}

	}

	fclose(SMDFile);

}




void CCashLotterySystem::SetitemDropRate(int iCategory, int iDropRate, int iSkillOptionRate, int iLuckOptionRate, int iAddOptionRate, int iExOptionRate)
{
	this->CategoryRandomPool.AddValue(iCategory, iDropRate);

	this->LotteryItemOptionRate[iCategory].iSkillOptionRate = iSkillOptionRate;
	this->LotteryItemOptionRate[iCategory].iLuckOptionRate = iLuckOptionRate;
	this->LotteryItemOptionRate[iCategory].iAddOptionRate = iAddOptionRate;
	this->LotteryItemOptionRate[iCategory].iExOptionRate = iExOptionRate;
}




BOOL CCashLotterySystem::InsertItem(int iItemCategory, int iItemType, int iItemIndex, int iItemLevelMin, int iItemLevelMax, int iSkillOption, int iLuckOption, int iAddOption, int iExOption)
{
	if ( iItemCategory < 0 || iItemCategory > MAX_LOTTERY_ITEM_CATEGORY )
		return FALSE;

	int iItemCount = this->LotteryItemListCount[iItemCategory];

	if ( iItemCount < 0 || iItemCount > MAX_LOTTERY_ITEM_COUNT )
		return FALSE;

	this->LotteryItemList[iItemCategory][iItemCount].btItemType = iItemType;
	this->LotteryItemList[iItemCategory][iItemCount].wItemIndex = iItemIndex;
	this->LotteryItemList[iItemCategory][iItemCount].btItemLevelMin = iItemLevelMin;
	this->LotteryItemList[iItemCategory][iItemCount].btItemLevelMax = iItemLevelMax;
	this->LotteryItemList[iItemCategory][iItemCount].btSkillOption = iSkillOption;
	this->LotteryItemList[iItemCategory][iItemCount].btLuckOption = iLuckOption;
	this->LotteryItemList[iItemCategory][iItemCount].btAddOption = iAddOption;
	this->LotteryItemList[iItemCategory][iItemCount].btExOption = iExOption;

	int iItemCode = 0;
	int iItemHeight = 0;
	int iItemWidth = 0;

	iItemCode = ITEMGET(iItemType, iItemIndex);
	iItemHeight = ItemAttribute[iItemCode].Height;
	iItemWidth = ItemAttribute[iItemCode].Width;

	if ( iItemWidth > this->iItemMaxWidth )
		this->iItemMaxWidth = iItemWidth;

	if ( iItemHeight > this->iItemMaxHeight )
		this->iItemMaxHeight = iItemHeight;

	this->LotteryItemListCount[iItemCategory]++;

	return TRUE;
}



int CCashLotterySystem::GetItem(CItem *lpItem)
{
	int iItemCategory = 0;
	int iMaxItemCountInCategory = 0;
	int iItemSelectNumber = 0;
	CItem ReturnItem;
	int iItemNumber = 0;
	int iItemType = 0;
	int iItemIndex = 0;
	int iItemLevel = 0;
	int iItemLevelMin = 0;
	int iItemLevelMax = 0;
	int iSkillOption = 0;
	int iLuckOption = 0;
	int iAddOption = 0;
	int iExOption = 0;
	int iSkillOptionRate = 0;
	int iLuckOptionRate = 0;
	int iAddOptionRate = 0;
	int iExOptionRate = 0;
	BYTE btSkillOption = 0;
	BYTE btLuckOption = 0;
	BYTE btAddOption = 0;
	BYTE btExOption = 0;
	BYTE btExOptionKey = 0;
	BYTE btExOptionValue1 = 0;
	BYTE btExOptionValue2 = 0;
	int iTemp = 0;
	int iRandomKey = 0;
	BYTE ExOption[MAX_EXOPTION_SIZE];

	if ( lpItem == NULL )
		return -1;

	iItemCategory = this->CategoryRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

	if ( iItemCategory < 0 || iItemCategory > MAX_LOTTERY_ITEM_CATEGORY )
		return -1;

	iMaxItemCountInCategory = this->LotteryItemListCount[iItemCategory];

	if ( iMaxItemCountInCategory <= 0 ||  iMaxItemCountInCategory > MAX_LOTTERY_ITEM_COUNT )
		return -1;

	iItemSelectNumber = rand() % iMaxItemCountInCategory;

	if ( iItemSelectNumber < 0 ||  iItemSelectNumber > MAX_LOTTERY_ITEM_COUNT )
		return -1;

	iItemType = this->LotteryItemList[iItemCategory][iItemSelectNumber].btItemType;
	iItemIndex = this->LotteryItemList[iItemCategory][iItemSelectNumber].wItemIndex;
	iItemLevelMin = this->LotteryItemList[iItemCategory][iItemSelectNumber].btItemLevelMin;
	iItemLevelMax = this->LotteryItemList[iItemCategory][iItemSelectNumber].btItemLevelMax;
	iSkillOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btSkillOption;
	iLuckOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btLuckOption;
	iAddOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btAddOption;
	iExOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btExOption;

	if ( iItemType == -1 || iItemIndex == -1 || iItemLevelMin == -1 || iItemLevelMax == -1 || iItemLevelMin == -1 || iItemLevelMax == -1 || iSkillOption == -1 || iLuckOption == -1 || iAddOption == -1 || iExOption == -1 )
		return -1;

	iSkillOptionRate = this->LotteryItemOptionRate[iItemCategory].iSkillOptionRate;
	iLuckOptionRate = this->LotteryItemOptionRate[iItemCategory].iLuckOptionRate;
	iAddOptionRate = this->LotteryItemOptionRate[iItemCategory].iAddOptionRate;
	iExOptionRate = this->LotteryItemOptionRate[iItemCategory].iExOptionRate;

	iRandomKey = rand() % (iItemLevelMax - iItemLevelMin + 1);
	iItemLevel = iItemLevelMin + iRandomKey;

	if ( iItemLevel > iItemLevelMax || iItemLevel < iItemLevelMin )
		iItemLevel = iItemLevelMin;

	if ( iSkillOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iSkillOptionRate )
			btSkillOption = 1;
	}

	if ( iLuckOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iLuckOptionRate )
			btLuckOption = 1;
	}

	if ( iAddOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iAddOptionRate )
		{
			btAddOption = this->AddOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
			iTemp = btAddOption;
		}
	}

	if ( iExOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iExOptionRate )
		{
			btExOptionValue1 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
			btExOptionKey =  1 << (int)(btExOptionValue1);
			btExOption |= btExOptionKey;

			iItemLevel = 0;

			if ( (rand() % 4) == 0 )
			{
				btExOptionValue2 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

				if ( btExOptionValue1 != btExOptionValue2 )
				{
					btExOptionKey =  1 << (int)(btExOptionValue2);
					btExOption |= btExOptionKey;
				}
			}
		}
	}
	else if ( iExOption == 2 )
	{
		btExOptionValue1 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
		btExOptionKey =  1 << (int)(btExOptionValue1);
		btExOption |= btExOptionKey;
		iItemLevel = 0;

		if ( (rand() % 4) == 0 )
		{
			btExOptionValue2 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

			if ( btExOptionValue1 != btExOptionValue2 )
			{
				btExOptionKey =  1 << (int)(btExOptionValue2);
				btExOption |= btExOptionKey;
			}
		}
	}

	if ( btAddOption != iTemp )
		btAddOption = 0;

	iItemNumber = ItemGetNumberMake(iItemType, iItemIndex);
	lpItem->Convert(iItemNumber, btSkillOption, btLuckOption, btAddOption, btExOption, 0, 0, NULL,0xFF, 0, CURRENT_DB_VERSION);
	lpItem->m_Level = iItemLevel;

	if ( !lpItem->IsItem() )
		return -1;

	ItemIsBufExOption(ExOption, lpItem);

	LogAddTD("[CashItem][LotteryItem] Get Item Category:%d,Index:%d (Name:%s,Type:%d,Index:%d,Level:%d) Skill:%d,Luck:%d,AddOption:%d,ExOption(%d,%d,%d,%d,%d,%d)",
		iItemCategory, iItemSelectNumber, ItemAttribute[iItemNumber].Name, iItemType,
		iItemIndex, iItemLevel, btSkillOption, btLuckOption, btAddOption, 
		ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5]);

	return iItemCategory;
}





void CCashLotterySystem::GetRequireInvenSize(int * piHeight, int * piWidth)
{
	*piHeight = this->iItemMaxHeight;
	*piWidth  = this->iItemMaxWidth;
}