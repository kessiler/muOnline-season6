// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "EventDungeonItemBag.h"
#include "..\common\ReadScript.h"
#include "logproc.h"
#include "user.h"
#include "LargeRand.h"
#include "DSProtocol.h"
#include "..\common\SetItemOption.h"
#include "GameMain.h"


CEventDungeonItemBag::CEventDungeonItemBag(void)
{
}


CEventDungeonItemBag::~CEventDungeonItemBag(void)
{
}

void CEventDungeonItemBag::LoadScript(const char *szFileName)
{
	int nOldLevelIndex;
	int nCnt;
	int nDropRate;

	int Type;
	SMDToken Token;
	m_bScriptLoad = 0;

	SMDFile = fopen(szFileName, "r");	//ok
	if ( !SMDFile )
	{
		LogAdd(lMsg.Get(453), szFileName);
		return;
	}

	while ( 1 )
	{
		Token = GetToken();
		if ( Token == SMDToken::END )
			break;

		if ( Token == SMDToken::NUMBER )
		{
			Type = TokenNumber;
			if ( Type )
			{
				switch ( Type )
				{
					case 1:
						nDropRate = 0;
						nCnt = 0;
						nOldLevelIndex = 0;
						while ( 1 )
						{
							Token = GetToken();
							if ( !Token )
							{
								if ( !strcmp("end", TokenString) )
									break;
							}

							CEventItemBagAttr eventItemBagAttr;

							int nLevelIndex = TokenNumber;
							if ( nOldLevelIndex != nLevelIndex )
							{
								nOldLevelIndex = TokenNumber;
								nCnt = 0;
							}

							if ( nLevelIndex >= MAX_LEVEL_INDEX )
							{
								MsgBox("Level index max over - %s", szFileName);
								return;
							}

							Token = GetToken();
							eventItemBagAttr.type = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.index = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.minLevel = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.maxLevel = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.skill = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.luck = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.option = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.ExOption = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.SetOption = TokenNumber;

							Token = GetToken();
							eventItemBagAttr.DropRate = TokenNumber;

							if ( nCnt > 0 )
							{
								eventItemBagAttr.DropRate += m_EventDungeonItemInfo[nLevelIndex].EventItemBagAttr.at(nCnt-1).DropRate;
								nOldLevelIndex = nLevelIndex;
							}

							m_EventDungeonItemInfo[nLevelIndex].EventItemBagAttr.push_back(eventItemBagAttr);
							++nCnt;
						}
						break;

					case 2:
						while ( 1 )
						{
							Token = GetToken();
							if ( !Token )
							{
								if ( !strcmp("end", TokenString) )
									break;
							}

							int nLevelIndex = TokenNumber;
							Token = GetToken();
							m_EventDungeonItemInfo[nLevelIndex].dropZen = TokenNumber;
						}
						break;

					case 3:
						while ( 1 )
						{
							Token = GetToken();
							if ( !Token )
							{
								if ( !strcmp("end", TokenString) )
									break;
							}

							int nLevelIndex = TokenNumber;
							Token = GetToken();
							m_EventDungeonItemInfo[nLevelIndex].dropCount = TokenNumber;
						}
						break;

					case 4:
						while ( 1 )
						{
							Token = GetToken();
							if ( !Token )
							{
								if ( !strcmp("end", TokenString) )
									break;
							}
							Token = GetToken();
							m_ExOptionRate = TokenNumber;
						}
						break;
				}
			}
			else
			{
				while ( 1 )
				{
					Token = GetToken();
					if ( !Token )
					{
						if ( !strcmp("end", TokenString) )
						break;
					}

					int nLevelIndex = TokenNumber;
					if ( nLevelIndex >= MAX_LEVEL_INDEX )
					{
						MsgBox("Level index max over - %s", szFileName);
						return;
					}
					m_EventDungeonItemInfo[nLevelIndex].levelIndex = nLevelIndex;
					m_EventDungeonItemInfo[nLevelIndex].bInfoLoad = 1;

					Token = GetToken();
					m_EventDungeonItemInfo[nLevelIndex].minLevel = TokenNumber;

					Token = GetToken();
					m_EventDungeonItemInfo[nLevelIndex].maxLevel = TokenNumber;
				}
			}
		}
	}


    fclose(SMDFile);

    LogAdd(lMsg.Get(454), szFileName);
    m_bScriptLoad = 1;
}

int CEventDungeonItemBag::GetLevelIndex(int nUserLevel, int nUserReset)
{
	for (int i = 0; i < MAX_LEVEL_INDEX; ++i )
	{

		if ( this->m_EventDungeonItemInfo[i].minLevel <= nUserLevel
			&& nUserLevel <= this->m_EventDungeonItemInfo[i].maxLevel )
		{
			return this->m_EventDungeonItemInfo[i].levelIndex;
		}
	}

	return -1;
}

int CEventDungeonItemBag::GetRegItemCount(int nUserLevel, int nUserReset)
{
	int nLevelIndex = GetLevelIndex(nUserLevel, nUserReset);

	if ( nLevelIndex >= 0 )
		return m_EventDungeonItemInfo[nLevelIndex].EventItemBagAttr.size();
	else
		return 0;
}

int CEventDungeonItemBag::GetDropZen(int nUserLevel, int nUserReset)
{
	int nLevelIndex = GetLevelIndex(nUserLevel, nUserReset);

	if ( nLevelIndex >= 0 )
		return m_EventDungeonItemInfo[nLevelIndex].dropZen;
	else
		return 0;
}

int CEventDungeonItemBag::GetDropItemCount(int nUserLevel, int nUserReset)
{
	int nLevelIndex = GetLevelIndex(nUserLevel, nUserReset);

	if ( nLevelIndex >= 0 )
		return m_EventDungeonItemInfo[nLevelIndex].dropCount;
	else
		return 0;
}

int CEventDungeonItemBag::GetLevel(int nMinLevel, int nMaxLevel)
{
	if ( nMinLevel == nMaxLevel )
		return nMinLevel;
	else
		return rand() % (nMaxLevel - nMinLevel + 1) + nMinLevel;
}


int CEventDungeonItemBag::DropEventItem(int nIndex, int nMaxUserLevel, int nMaxUserReset)
{
	if ( !this->m_bScriptLoad )
		return 0;

    int Option1 = 0;
    int Option2 = 0;
    int Option3 = 0;
    int DropIndex = 0;
    int ExOption = 0;
    OBJECTSTRUCT *lpObj = &gObj[nIndex];

    CEventItemBagAttr BagObject;
    CEventItemBagAttr BagObject2;

    int nItemCount = GetRegItemCount(nMaxUserLevel, nMaxUserReset);
    int nLevelIndex = GetLevelIndex(nMaxUserLevel, nMaxUserReset);
    int nDropItemCount = GetDropItemCount(nMaxUserLevel, nMaxUserReset);
	int x = 0;
	int y = 0;

	if ( nItemCount > 0 && nLevelIndex > -1 )
	{
		for (int i = 0; i < nDropItemCount; ++i )
		{
			int dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			int nRand = GetLargeRand() % 1000000;
			int bDropItem = 0;

			for (int j = nItemCount - 1; j >= 0; --j )
			{
				if ( !j )
				{
					BagObject = m_EventDungeonItemInfo[nLevelIndex].EventItemBagAttr.at(0);
					bDropItem = 1;
					break;
				}


				BagObject = m_EventDungeonItemInfo[nLevelIndex].EventItemBagAttr.at(j);
				BagObject2 = m_EventDungeonItemInfo[nLevelIndex].EventItemBagAttr.at(j-1);

				if ( nRand >= 1000000 - BagObject.DropRate && nRand < 1000000 - BagObject2.DropRate )
				{
					bDropItem = 1;
					break;
				}
			}

			if ( bDropItem )
			{
				if ( BagObject.SetOption )
					CreateSetItem(nIndex, BagObject);
				else
					CreateNormalItem(nIndex, BagObject);
			}
		}
	}

	x = lpObj->X;
	y = lpObj->Y;
	int nDropZen = GetDropZen(nMaxUserLevel, nMaxUserReset);

	if ( nDropZen > 0 )
		MapC[lpObj->MapNumber].MoneyItemDrop(nDropZen, x, y);

	return 1;
}

int CEventDungeonItemBag::CreateNormalItem(int nIndex, CEventItemBagAttr BagObject)
{
	int nLevel = GetLevel(BagObject.minLevel, BagObject.maxLevel);
	int nType = ItemGetNumberMake(BagObject.type, BagObject.index);
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int ExOption = 0;
	int iExOption = 0;
	OBJECTSTRUCT *lpObj = &gObj[nIndex];

	if ( nType == -1 )
	{
		return 0;
	}


    if ( BagObject.skill )
		Option1 = rand() % 2;

    if ( BagObject.luck )
		Option2 = rand() % 2;

    if ( BagObject.option )
	{
		if ( rand() % 5 >= 1 )
			Option3 = rand() % 3;
		else
			Option3 = 3;
    }

    if ( BagObject.ExOption )
    {
		int nExOptCnt = 0;
		int nRandValue = GetLargeRand() % 1000000;

		if ( nRandValue >= m_ExOptionRate )
			nExOptCnt = 1;
		else
			nExOptCnt = 2;

		for (int i = 0; i < nExOptCnt; ++i )
		{
			int nExOptIndex = rand() % 6;
			int nTempExOpt = 1 << nExOptIndex;

			if ( (ExOption & (1 << nExOptIndex)) == 1 << nExOptIndex )
			{
				nTempExOpt = 0;
				++nExOptIndex;
				if ( nExOptIndex > 5 )
				nExOptIndex = 0;
				nTempExOpt = 1 << nExOptIndex;
			}

			ExOption |= 1 << nExOptIndex;
		}

		if ( ExOption )
		{
			Option1 = 1;
			Option2 = 0;
			nLevel = 0;
		}
    }

    if ( nType == ITEMGET(12, 15) || nType == ITEMGET(14, 13) || nType == ITEMGET(14, 14) )
    {
		Option1 = 0;
		Option2 = 0;
		Option3 = 0;
		nLevel = 0;
    }

    if ( nType == ITEMGET(13, 0) || nType == ITEMGET(13, 1) || nType == ITEMGET(13, 2) 
			|| nType == ITEMGET(13, 8) || nType == ITEMGET(13, 9) || nType == ITEMGET(13, 12) 
			|| nType == ITEMGET(13, 13) )
	{
		nLevel = 0;
	}

    int dur = 0;
    BYTE btDropX = lpObj->X;
    BYTE btDropY = lpObj->Y;

    if ( !gObjGetRandomItemDropLocation( lpObj->MapNumber, btDropX, btDropY, 2, 2, 10) )
    {
		btDropX = lpObj->X;
		btDropY = lpObj->Y;
    }

	ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, btDropX, btDropY,
			nType, nLevel, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

    LogAddTD("[IMPERIALGUARDIAN] Drop Item : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->MapNumber, btDropX, btDropY, ItemAttribute[nType].Name, 
			nType, nLevel, Option1, Option2, Option3, ExOption);


	return 1;
}

int CEventDungeonItemBag::CreateSetItem(int nIndex, CEventItemBagAttr BagObject)
{
	int nLevel = GetLevel(BagObject.minLevel, BagObject.maxLevel);
	int nType = ItemGetNumberMake(BagObject.type, BagObject.index);
	int SetOption = gSetItemOption.GenSetOption(nType);

	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	OBJECTSTRUCT *lpObj = &gObj[nIndex];
	BYTE btMapNumber = gObj[nIndex].MapNumber;

	int option1rand = 6;
	int option2rand = 4;
	int option3rand = rand() % 100;
	int optionc = rand() % 3;

	if ( rand() % 100 < 4 )
		Option2 = 1;

	switch(optionc)
	{
		case 0:
			if ( option3rand < 4 )
				Option3 = 3;
		break;

		case 1:
			if ( option3rand < 8 )
				Option3 = 2;
		break;

		case 2:
			if ( option3rand < 12 )
				Option3 = 1;
		break;
	}


	Option1 = 1;

	BYTE btDropX = lpObj->X;
	BYTE btDropY = lpObj->Y;

	if ( !gObjGetRandomItemDropLocation(lpObj->MapNumber, btDropX, btDropY, 2, 2, 10) )
	{
		btDropX = lpObj->X;
		btDropY = lpObj->Y;
	}

	ItemSerialCreateSend(nIndex, btMapNumber, btDropX, btDropY, nType, 0, 0,
			Option1, Option2, Option3, nIndex, 0, SetOption);

	int  tmpSetOption = 0;

	if ( SetOption & 1 )
	{
		tmpSetOption = 1;
	}
	else if ( SetOption & 2 )
	{
		tmpSetOption = 2;
	}

	LogAddTD("[IMPERIALGUARDIAN] Drop Set Item [%s][%s] : (%d)(%d/%d) Itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",
			gObj[nIndex].AccountID, gObj[nIndex].Name, btMapNumber, btDropX, btDropY,
			nType, Option1, Option2, Option3, SetOption,
			gSetItemOption.GetSetOptionName(nType, tmpSetOption));

	return 1;
}